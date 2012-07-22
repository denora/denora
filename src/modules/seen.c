/*
 * seen module for Denora 1.4.3+
 * (c) 2006-2009 Hal9000 (hal9000@denorastats.org)
 */

#include "denora.h"
#include <time.h>
#define AUTHOR "Hal9000"
#define VERSION "1.2.2"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int do_fantasy_seen(int argc, char **argv);
int do_msg_seen(User * u, int argc, char **argv);
char *do_seen(User * u, char *target);
char *get_timestring(User * u, int timestamp);
void mAddLanguages(void);

/* Language definitions */
#define SEEN_USAGE		0
#define SEEN_ONLINE		1
#define SEEN_ONLINEC		2
#define SEEN_AWAY		3
#define SEEN_AWAYC		4
#define SEEN_OFFLINE		5
#define SEEN_UNKNOWN		6
#define SEEN_DAYS		7
#define SEEN_HOURS		8
#define SEEN_MINUTES		9
#define SEEN_SECONDS		10
#define SEEN_DAY		11
#define SEEN_HOUR		12
#define SEEN_MINUTE		13
#define SEEN_SECOND		14
#define SEEN_EMPTY		15
#define SEEN_FAILED		16

#define LANG_NUM_STRINGS	17

/**
 * Create the hook, and tell Denora about it.
 **/
int DenoraInit(int argc, char **argv)
{
	EvtHook *hook;
	Command *c;

	if (denora->debug)
	    protocol_debug(NULL, argc, argv);

	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);
	moduleSetType(THIRD);

	if ((VERSION_MAJOR != 1) && (VERSION_MINOR < 4)) {
	    alog(LOG_ERROR,
	         "[seen] Your Denora version is incompatible with this module.");
	    return MOD_STOP;
	}

	if (!denora->do_sql) {
	    alog(LOG_ERROR,
	         "[seen] SQL needs to be enabled to load this module.");
	    return MOD_STOP;
	}

	hook = createEventHook(EVENT_FANTASY, do_fantasy_seen);
	moduleAddEventHook(hook);

	c = createCommand("SEEN", do_msg_seen, NULL, -1, -1, -1, -1);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);

	mAddLanguages();

	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/**
 * Handle seen fantasy command.
 **/
int do_fantasy_seen(int argc, char **argv)
{
	User *u;
	char *target = NULL;
	ChannelStats *cs;
	char *message = NULL;

	if (denora->debug)
	    protocol_debug(NULL, argc, argv);

	if (argc >= 3 && stricmp(argv[0], "seen") == 0) {
	    u = finduser(argv[1]);
	    cs = find_cs(argv[2]);
	    if (argc == 3) {
	        if (cs->flags & CS_NOTICE)
	            moduleNoticeLang(s_StatServ, u, SEEN_USAGE,
	                             ChanStatsTrigger);
	        else
	            moduleNoticeChanLang(s_StatServ, cs, SEEN_USAGE,
	                                 ChanStatsTrigger);
	    } else {
	        target = myStrGetToken(argv[3], ' ', 0);
	        message = do_seen(u, target);
		free(target);
	        if (cs->flags & CS_NOTICE)
	            notice(s_StatServ, u->nick, message);
	        else
	            denora_cmd_privmsg(s_StatServ, cs->name, message);
		free(message);
	    }
	}
	return MOD_CONT;
}

/**
 * Handle msg fantasy command.
 **/
int do_msg_seen(User * u, int argc, char **argv)
{
	char *target = NULL;
	char *message = NULL;

	if (denora->debug)
		protocol_debug(NULL, argc, argv);

	if (argc < 1 || BadChar(argv[0])) {
		moduleNoticeLang(s_StatServ, u, SEEN_USAGE, "");
	} else {
		target = myStrGetToken(argv[0], ' ', 0);
		message = do_seen(u, target);
		free(target);
		notice(s_StatServ, u->nick, message);
		free(message);
	}
	return MOD_CONT;
}

/**
 * Seen
 **/
char *do_seen(User * u, char *target)
{
#ifdef USE_MYSQL
	char *split1 = NULL;
	char *seenhost = NULL;
	char *seennick = NULL;
	char *seenuname = NULL;
	struct tm tm;
	char buf[255];
	int i;
	char *sqltarget;
	char usrchans[1024] = "\0";
	time_t tsnow = time(NULL);
	MYSQL_RES *mysql_res;
	MYSQL_RES *mysql_res2;
	MYSQL_ROW mysql_row2;
	char cmodep[32] = "\0";
	char cmodes[32] = "\0";
	char cmodeA[32] = "\0";
	char cmodeO[32] = "\0";
	char umodep[32] = "\0";
	char umodeQ[32] = "\0";
	char umodeI[32] = "\0";
	double ctsdiff;
	time_t time0 = time(NULL);
	time_t time1 = time(NULL);
	time_t tt = time(NULL);
	char *message;
	char *uname = NULL;
	message = malloc(1024);

	sqltarget = rdb_escape(rdb_escape(target));
	for (i = 0; sqltarget[i]; i++)
	    if (sqltarget[i] == '*')
	        sqltarget[i] = '%';

	seenhost = myStrGetToken(sqltarget, '@', 1);
	split1 = myStrGetToken(sqltarget, '@', 0);
	seennick = myStrGetToken(split1, '!', 0);
	seenuname = myStrGetToken(split1, '!', 1);
	free(split1);

	if (seenhost == NULL)
	    seenhost = sstrdup("%");
	if (seennick == NULL)
	    seennick = sstrdup("%");
	if (seenuname == NULL)
	    seenuname = sstrdup("%");
	free(sqltarget);
	
	if (!strcmp(seennick, "%")) {
		uname = sstrdup("%");
	} else {	
		rdb_query(QUERY_HIGH, "SELECT uname FROM %s WHERE nick LIKE \'%s\' ",
					  AliasesTable, seennick);
		mysql_res = mysql_store_result(mysql);
		if (mysql_res && mysql_num_rows(mysql_res)) {
			mysql_row = mysql_fetch_row(mysql_res);
			uname = rdb_escape(mysql_row[0]);
		}
	}
	
	if (uname) {
		rdb_query(QUERY_HIGH,
	          "SELECT %s.nickid, %s.nick, %s.hostname, %s.hiddenhostname, %s.username, UNIX_TIMESTAMP(%s.connecttime), %s.away, %s.awaymsg, %s.online, UNIX_TIMESTAMP(%s.lastquit), %s.lastquitmsg FROM %s,%s,%s WHERE %s.uname LIKE \"%s\" AND %s.nick = %s.nick AND %s.username LIKE \"%s\" AND (%s.hostname LIKE \"%s\" OR %s.hiddenhostname LIKE \"%s\") AND %s.server = %s.server AND %s.uline = \"0\" ORDER BY online,lastquit DESC, %s.connecttime ASC LIMIT 1;",
	          UserTable, UserTable, UserTable, UserTable, UserTable,
	          UserTable, UserTable, UserTable, UserTable, UserTable,
	          UserTable, UserTable, ServerTable, AliasesTable,
	          AliasesTable, uname, UserTable,
	          AliasesTable, UserTable, seenuname, UserTable, seenhost,
	          UserTable, seenhost, UserTable, ServerTable, ServerTable, UserTable);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
#endif
		free(uname);
	} else {
		rdb_query(QUERY_HIGH,
	          "SELECT %s.nickid, %s.nick, %s.hostname, %s.hiddenhostname, %s.username, UNIX_TIMESTAMP(%s.connecttime), %s.away, %s.awaymsg, %s.online, UNIX_TIMESTAMP(%s.lastquit), %s.lastquitmsg FROM %s,%s WHERE %s.nick LIKE \"%s\" AND %s.username LIKE \"%s\" AND (%s.hostname LIKE \"%s\" OR %s.hiddenhostname LIKE \"%s\") AND %s.server = %s.server AND %s.uline = \"0\" ORDER BY online,lastquit DESC, %s.connecttime ASC LIMIT 1;",
	          UserTable, UserTable, UserTable, UserTable, UserTable,
	          UserTable, UserTable, UserTable, UserTable, UserTable,
	          UserTable, UserTable, ServerTable, UserTable, seennick,
	          UserTable, seenuname, UserTable, seenhost,
	          UserTable, seenhost, UserTable, ServerTable, ServerTable, UserTable);
		mysql_res = mysql_store_result(mysql);
	}
	if (mysql_num_rows(mysql_res) > 0) {
	    SET_SEGV_LOCATION();
	    while ((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
	        double tsdiff;
	        if (mysql_row[5] != NULL) { 
	            time0 = atoi(mysql_row[5]); /* connect time */
	        }
	        if (mysql_row[9] != NULL) {   
	            time1 = atoi(mysql_row[9]); /* quit time */
	        }
	        if (strlen(mysql_row[3]) <= 1)
	            mysql_row[3] = mysql_row[2];    /* no vhost, so using real host */
	        if (stricmp(mysql_row[8], "Y") == 0) {
	            if (mysql_row[5] != NULL) {
					tt = atoi(mysql_row[5]);
					tm = *localtime(&tt);
	                strftime(buf, sizeof(buf), "%d.%m %H:%M", &tm);
	                mysql_row[5] = buf;
	            }

	            /* Display channels the user is in */
	            /* check for ircd compatibility: chanmodes +AOsp and usermode +p */
	            if (denora_cmode(CMODE_p) == 1)
	                sprintf(cmodep, "AND %s.mode_lp = 'N' ", ChanTable);
	            if (denora_cmode(CMODE_s) == 1)
	                sprintf(cmodes, "AND %s.mode_ls = 'N' ", ChanTable);
	            if (denora_cmode(CMODE_A) == 1)
	                sprintf(cmodeA, "AND %s.mode_ua = 'N' ", ChanTable);
	            if (denora_cmode(CMODE_O) == 1)
	                sprintf(cmodeO, "AND %s.mode_uo = 'N' ", ChanTable);
	            if (denora_umode(UMODE_p) == 1)
	                sprintf(umodep, "AND %s.mode_lp = 'N' ", UserTable);
				if (denora_get_ircd() == IRC_INSPIRCD11 || denora_get_ircd() == IRC_INSPIRCD12) {
					if (denora_umode(UMODE_Q) == 1)
						sprintf(umodeQ, "AND %s.mode_uq = 'N' ", UserTable);
					if (denora_umode(UMODE_I) == 1)
						sprintf(umodeI, "AND %s.mode_ui = 'N' ", UserTable);
				}
	            rdb_query(QUERY_HIGH,
	                      "SELECT %s.channel FROM %s,%s,%s WHERE %s.nickid =%s AND %s.chanid = %s.chanid AND %s.nickid = %s.nickid %s%s%s%s%s%s%s ORDER BY %s.channel ASC",
	                      ChanTable, ChanTable, UserTable, IsOnTable,
	                      IsOnTable, mysql_row[0], ChanTable, IsOnTable,
	                      UserTable, IsOnTable, cmodep, cmodes, cmodeA,
	                      cmodeO, umodep, umodeQ, umodeI, ChanTable);
	            mysql_res2 = mysql_store_result(mysql);
	            while ((mysql_row2 = mysql_fetch_row(mysql_res2)) != NULL) {
	                strlcat(usrchans, mysql_row2[0], sizeof(usrchans));
	                strlcat(usrchans, " ", sizeof(usrchans));
	            }
	            mysql_free_result(mysql_res2);

	            ctsdiff = difftime(tsnow, time0);       /* total online time */

	            /* Prepare the reply for the user */
	            if (stricmp(mysql_row[6], "N") == 0) {
	                if (strlen(usrchans) < 3) {
	                    sprintf(message,
	                            moduleGetLangString(u, SEEN_ONLINE),
	                            mysql_row[1], mysql_row[4], mysql_row[3],
	                            get_timestring(u, ctsdiff), mysql_row[5]);
	                } else {
	                    sprintf(message,
	                            moduleGetLangString(u, SEEN_ONLINEC),
	                            mysql_row[1], mysql_row[4], mysql_row[3],
	                            get_timestring(u, ctsdiff), mysql_row[5],
	                            usrchans);
	                }
	            } else {
	                if (strlen(usrchans) < 3) {
	                    sprintf(message, moduleGetLangString(u, SEEN_AWAY),
	                            mysql_row[1], mysql_row[4], mysql_row[3],
	                            get_timestring(u, ctsdiff), mysql_row[5],
	                            mysql_row[7]);
	                } else {
	                    sprintf(message,
	                            moduleGetLangString(u, SEEN_AWAYC),
	                            mysql_row[1], mysql_row[4], mysql_row[3],
	                            get_timestring(u, ctsdiff), mysql_row[5],
	                            mysql_row[7], usrchans);
	                }
	            }
	        } else {
	            double dtsdiff;
	            if (mysql_row[9] != NULL && mysql_row[5] != NULL) {
					tt = atoi(mysql_row[9]);
					tm = *localtime(&tt);
	                strftime(buf, sizeof(buf), "%d.%m %H:%M", &tm);
	                mysql_row[9] = buf;
	                dtsdiff = difftime(tsnow, time1);   /* total offline time */
	                tsdiff = difftime(time1, time0);    /* total online time */
	                sprintf(message, moduleGetLangString(u, SEEN_OFFLINE),
	                        mysql_row[1], mysql_row[4], mysql_row[3],
	                        get_timestring(u, dtsdiff), mysql_row[9],
	                        get_timestring(u, tsdiff), mysql_row[10]);
	            } else {
	                sprintf(message, moduleGetLangString(u, SEEN_UNKNOWN),
	                        mysql_row[1], mysql_row[4], mysql_row[3]);
	            }
	        }
	    }
	} else {
	    sprintf(message, moduleGetLangString(u, SEEN_EMPTY), target);
	}
	SET_SEGV_LOCATION();
	mysql_free_result(mysql_res);
	return message;
#else
	return;
#endif
}

char *get_timestring(User * u, int timestamp)
{
	int days, hours, mins, secs;
	char *tsbuf;
	char dd[16] = "\0";
	char dh[16] = "\0";
	char dm[16] = "\0";
	char ds[16] = "\0";
	
	tsbuf = malloc(64);
	days = timestamp / (3600 * 24);
	timestamp %= (3600 * 24);
	hours = timestamp / 3600;
	timestamp %= 3600;
	mins = timestamp / 60;
	secs = timestamp % 60;

	if (days == 1)
		sprintf(dd, "%i %s", days, moduleGetLangString(u, SEEN_DAY));
	else if (days > 1)
		sprintf(dd, "%i %s", days, moduleGetLangString(u, SEEN_DAYS));

	if (hours == 1)
		sprintf(dh, "%i %s", hours, moduleGetLangString(u, SEEN_HOUR));
	else if (hours > 1 || (hours == 0 && days > 0))
		sprintf(dh, "%i %s", hours,
				moduleGetLangString(u, SEEN_HOURS));

	if (mins == 1)
		sprintf(dm, "%i %s", mins,
				moduleGetLangString(u, SEEN_MINUTE));
	else if (mins > 1 || (mins == 0 && hours > 0))
		sprintf(dm, "%i %s", mins,
				moduleGetLangString(u, SEEN_MINUTES));

	if (days == 0 && hours == 0 && mins == 0) {
		if (secs == 1)
			sprintf(ds, "%i %s", secs,
					moduleGetLangString(u, SEEN_SECOND));
		else
			sprintf(ds, "%i %s", secs,
					moduleGetLangString(u, SEEN_SECONDS));
	}

	sprintf(tsbuf, "%s%s%s%s", dd, dh, dm, ds);

	return tsbuf;
}

void mAddLanguages(void)
{
	const char *langtable_en_us[] = {
	    "Usage: \00310\002%sseen <nick/mask>\002\003",
	    "\00310\002%s\002\003 (%s@%s) is \0033online\003 since %s (%s)",
	    "\00310\002%s\002\003 (%s@%s) is \0033online\003 since %s (%s) and is in %s",
	    "\00310\002%s\002\003 (%s@%s) is \0033online\003 since %s (%s) and is currently \0037away\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) is \0033online\003 since %s (%s), is currently \0037away\003 (%s) and is in %s",
	    "\00310\002%s\002\003 (%s@%s) is \0034offline\003 since %s (%s) after being connected for %s saying (%s)",
	    "\00310\002%s\002\003 (%s@%s) is \0034offline\003 but I don't know since when",
	    "days, ",
	    "hours and ",
	    "minutes",
	    "seconds",
	    "day, ",
	    "hour and ",
	    "minute",
	    "second",
	    "\0034No results\003 for \00310\002%s\002\003",
	    "Internal failure",
	};

	const char *langtable_de[] = {
	    "Syntax: \00310\002%sseen <nick/mask>\002\003",
	    "\00310\002%s\002\003 (%s@%s) ist seit %s \0033online\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) ist seit %s \0033online\003 (%s) und befindet sich in %s",
	    "\00310\002%s\002\003 (%s@%s) ist seit %s \0033online\003 (%s) und ist momentan \0037abwesend\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) ist seit %s \0033online\003 (%s), ist momentan \00377abwesend\003 (%s) und befindet sich in %s",
	    "\00310\002%s\002\003 (%s@%s) ist seit %s \0034offline\003 (%s) nach %s Aufenthalt (%s)",
	    "\00310\002%s\002\003 (%s@%s) ist \0034offline\003 aber ich weiss nicht wie lange schon",
	    "Tage, ",
	    "Stunden und ",
	    "Minuten",
	    "Sekunden",
	    "Tag, ",
	    "Stunde und ",
	    "Minute",
	    "Sekunde",
	    "\0034Keine Ergebnisse\003 für \00310\002%s\002\003",
	    "Interner Fehler",
	};

	const char *langtable_es[] = {
	    "Uso: \00310\002%sseen <nick/mask>\002\003",
	    "\00310\002%s\002\003 (%s@%s) esta \0033online\003 desde %s (%s)",
	    "\00310\002%s\002\003 (%s@%s) esta \0033online\003 desde %s (%s) y esta en %s",
	    "\00310\002%s\002\003 (%s@%s) esta \0033online\003 desde %s (%s) y actualmente esta \0037ausente\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) esta \0033online\003 desde %s (%s), actualmente esta \0037ausente\003 (%s) y esta en %s",
	    "\00310\002%s\002\003 (%s@%s) esta \0034offline\003 desde %s (%s) despues de haber estado conectado por %s diciendo (%s)",
	    "\00310\002%s\002\003 (%s@%s) esta \0034offline\003 pero no se desde cuando",
	    "dias, ",
	    "horas y ",
	    "minutos",
	    "segundos",
	    "dia, ",
	    "hora y ",
	    "minuto",
	    "segundo",
	    "\0034Sin resultados\003 para \00310\002%s\002\003",
	    "Falla interna",
	};

	const char *langtable_fr[] = {
	    "Utilisation: \00310\002%sseen <pseudo/masque>\002\003",
	    "\00310\002%s\002\003 (%s@%s) est \0033connecté(e)\003 depuis %s (%s)",
	    "\00310\002%s\002\003 (%s@%s) est \0033connecté(e)\003 depuis %s (%s) et se trouve sur %s",
	    "\00310\002%s\002\003 (%s@%s) est \0033connecté(e)\003 depuis %s (%s) et est actuellement \0037absent\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) est \0033connecté(e)\003 depuis %s (%s), actuellement \0037absent\003 (%s) et se trouve sur %s",
	    "\00310\002%s\002\003 (%s@%s) est \0034déconnecté(e)\003 depuis %s (%s) après avoir été connecté(e) pendant %s disant (%s)",
	    "\00310\002%s\002\003 (%s@%s) est \0034déconnecté(e)\003 mais je ne sais pas depuis quand.",
	    "jours, ",
	    "heures et ",
	    "minutes",
	    "secondes",
	    "jour, ",
	    "heure et ",
	    "minute",
	    "seconde",
	    "\0034Pas de résultat\003 pour \00310\002%s\002\003",
	    "Erreur interne",
	};

	const char *langtable_it[] = {
	    "Sintassi: \00310\002%sseen <nick/mask>\002\003",
	    "\00310\002%s\002\003 (%s@%s) è \0033online\003 da %s (%s)",
	    "\00310\002%s\002\003 (%s@%s) è \0033online\003 da %s (%s) e si trova in %s",
	    "\00310\002%s\002\003 (%s@%s) è \0033online\003 da %s (%s) ed è momentaneamente \0037assente\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) è \0033online\003 da %s (%s), è momentaneamente \0037assente\003 (%s) e si trova in %s",
	    "\00310\002%s\002\003 (%s@%s) è \0034offline\003 da %s (%s) dopo una permanenza di %s dicendo (%s)",
	    "\00310\002%s\002\003 (%s@%s) è \0034offline\003 ma non so da quanto",
	    "giorni, ",
	    "ore e ",
	    "minuti",
	    "secondi",
	    "giorno, ",
	    "ora e ",
	    "minuto",
	    "secondo",
	    "\0034Nessun risultato\003 per \00310\002%s\002\003",
	    "Errore interno",
	};

	const char *langtable_nl[] = {
	    "Gebruik: \00310\002%sseen <gebruikersnaam/masker>\002\003",
	    "\00310\002%s\002\003 (%s@%s) is sinds %s \0033online\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) is sinds %s \0033online\003 (%s) en bevindt zich in %s",
	    "\00310\002%s\002\003 (%s@%s) is sinds %s \0033online\003 (%s) en is momenteel \0037afwezig\003 (%s)",
	    "\00310\002%s\002\003 (%s@%s) is sinds %s \0033online\003 (%s), is momenteel \0037afwezig\003 (%s) en bevindt zich in %s",
	    "\00310\002%s\002\003 (%s@%s) is sinds %s \0034offline\003 (%s) na %s verbonden te zijn vertrokken (%s)",
	    "\00310\002%s\002\003 (%s@%s) is \0034offline\003, maar het is onbekend sinds wanneer",
	    "dagen, ",
	    "uren en ",
	    "minuten",
	    "seconden",
	    "dag, ",
	    "uur en ",
	    "minuut",
	    "seconde",
	    "\0034Geen informatie\003 over \00310\002%s\002\003",
	    "Interne fout",
	};

	moduleInsertLanguage(LANG_EN_US, LANG_NUM_STRINGS,
	                     (char **) langtable_en_us);
	moduleInsertLanguage(LANG_DE, LANG_NUM_STRINGS,
	                     (char **) langtable_de);
	moduleInsertLanguage(LANG_ES, LANG_NUM_STRINGS,
	                     (char **) langtable_es);
	moduleInsertLanguage(LANG_FR, LANG_NUM_STRINGS,
	                     (char **) langtable_fr);
	moduleInsertLanguage(LANG_IT, LANG_NUM_STRINGS,
	                     (char **) langtable_it);
	moduleInsertLanguage(LANG_NL, LANG_NUM_STRINGS,
	                     (char **) langtable_nl);
}
