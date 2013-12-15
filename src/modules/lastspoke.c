/*
 * lastspoke module for Denora 1.4.3+
 * (c) 2007-2009 Hal9000 (hal9000@denorastats.org)
 */

#include "denora.h"
#include <time.h>
#define AUTHOR "Hal9000"
#define VERSION "1.2.2"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int do_fantasy_lastspoke(int argc, char **argv);
int do_msg_lastspoke(User * u, int argc, char **argv);
char *do_lastspoke(User * u, char *target);
char *get_timestring(User * u, int timestamp);
void mAddLanguages(void);

/* Language definitions */
#define LASTSPOKE_USAGE		0
#define LASTSPOKE_ONLINE_C	1
#define LASTSPOKE_ONLINE_G	2
#define LASTSPOKE_AWAY_C	3
#define LASTSPOKE_AWAY_G	4
#define LASTSPOKE_OFFLINE_C	5
#define LASTSPOKE_OFFLINE_G	6
#define LASTSPOKE_DAYS		7
#define LASTSPOKE_HOURS		8
#define LASTSPOKE_MINUTES	9
#define LASTSPOKE_SECONDS	10
#define LASTSPOKE_DAY		11
#define LASTSPOKE_HOUR		12
#define LASTSPOKE_MINUTE	13
#define LASTSPOKE_SECOND	14
#define LASTSPOKE_EMPTY		15
#define LASTSPOKE_FAILED	16

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
			 "[lastspoke] Your Denora version is incompatible with this module.");
		return MOD_STOP;
	}

	hook = createEventHook(EVENT_FANTASY, do_fantasy_lastspoke);
	moduleAddEventHook(hook);

	c = createCommand("LASTSPOKE", do_msg_lastspoke, NULL, -1, -1, -1, -1);
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
 * Handle lastspoke fantasy command.
 **/
int do_fantasy_lastspoke(int argc, char **argv)
{
	User *u;
	char *target = NULL;
	ChannelStats *cs;
	char *message = NULL;

	if (denora->debug)
		protocol_debug(NULL, argc, argv);

	if (argc >= 3 && stricmp(argv[0], "lastspoke") == 0) {	  /* React on the lastspoke command */
		u = finduser(argv[1]);
		cs = find_cs(argv[2]);
		if (argc == 3) {
			if (cs->flags & CS_NOTICE)
				moduleNoticeLang(s_StatServ, u, LASTSPOKE_USAGE,
						 ChanStatsTrigger);
			else
				moduleNoticeChanLang(s_StatServ, cs, LASTSPOKE_USAGE,
						     ChanStatsTrigger);
		} else {
			target = myStrGetToken(argv[3], ' ', 0);
			message = do_lastspoke(u, target);
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
int do_msg_lastspoke(User * u, int argc, char **argv)
{
	char *target = NULL;
	char *message = NULL;

	if (denora->debug)
		protocol_debug(NULL, argc, argv);

	if (argc < 1 || BadChar(argv[0])) {
		moduleNoticeLang(s_StatServ, u, LASTSPOKE_USAGE, "");
	} else {
		target = myStrGetToken(argv[0], ' ', 0);
		message = do_lastspoke(u, target);
		free(target);
		notice(s_StatServ, u->nick, message);
		free(message);
	}
	return MOD_CONT;
}

/**
 * Lastspoke
 **/
char *do_lastspoke(User * u, char *target)
{
	char *split1 = NULL;
	char *lastspokehost = NULL;
	char *lastspokenick = NULL;
	char *lastspokeuname = NULL;
	char buf[255];
	int i;
	SQLres *sql_res;
	char **sql_row;
	char *sqltarget;
	char *usrchan = NULL;
	SQLres *sql_res2;
	char **sql_row2;
	char cmodep[32] = "\0";
	char cmodes[32] = "\0";
	char cmodeA[32] = "\0";
	char cmodeO[32] = "\0";
	char umodep[32] = "\0";
	char umodeQ[32] = "\0";
	char umodeI[32] = "\0";	
	struct tm tm;
	time_t tsnow = time(NULL);
	double tsdiff;
	time_t time0 = time(NULL);
	char *message;
	char *uname = NULL;
	time_t tt = time(NULL);
	message = malloc(1024);

	sqltarget = sql_escape(sql_escape(target));
	for (i = 0; sqltarget[i]; i++)
		if (sqltarget[i] == '*')
			sqltarget[i] = '%';
	lastspokehost = myStrGetToken(sqltarget, '@', 1);
	split1 = myStrGetToken(sqltarget, '@', 0);
	lastspokenick = myStrGetToken(split1, '!', 0);
	lastspokeuname = myStrGetToken(split1, '!', 1);
	if (lastspokehost == NULL)
		lastspokehost = (char *) "%";
	if (lastspokenick == NULL)
		lastspokenick = (char *) "%";
	if (lastspokeuname == NULL)
		lastspokeuname = (char *) "%";
	free(sqltarget);
	
	if (!strcmp(lastspokenick, "%")) {
		uname = sstrdup("%");
	} else {	
		sql_query("SELECT uname FROM %s WHERE nick LIKE \'%s\' ",
					  AliasesTable, lastspokenick);
		sql_res = sql_set_result(sqlcon);
		if (sql_res && sql_num_rows(sql_res)) {
			sql_row = sql_fetch_row(sql_res);
			uname = sql_escape(sql_row[0]);
		}
	}

	if (uname) {
		sql_query("SELECT %s.nick, %s.username, %s.hostname, %s.hiddenhostname, %s.online, %s.away, %s.chan, %s.lastspoke FROM %s, %s, %s, %s WHERE %s.uname LIKE \"%s\" AND %s.nick = %s.nick AND %s.username LIKE \"%s\" AND (%s.hostname LIKE \"%s\" OR %s.hiddenhostname LIKE \"%s\") AND %s.server = %s.server AND %s.uline = \"0\" AND %s.uname = %s.uname AND %s.chan != \"global\" ORDER BY %s.online, %s.lastquit, %s.lastspoke DESC, %s.connecttime ASC LIMIT 1",
			  UserTable, UserTable, UserTable, UserTable, UserTable,
			  UserTable, UStatsTable, UStatsTable, UserTable, ServerTable,
			  AliasesTable, UStatsTable,
			  AliasesTable, uname, UserTable, AliasesTable,
			  UserTable, lastspokeuname, UserTable, lastspokehost,
			  UserTable, lastspokehost, UserTable, ServerTable,
			  ServerTable, AliasesTable, UStatsTable, UStatsTable,
			  UserTable, UserTable, UStatsTable, UserTable);
		sql_res = sql_set_result(sqlcon);
	}

	if (sql_num_rows(sql_res) > 0) {
		
		while ((sql_row = sql_fetch_row(sql_res)) != NULL) {
			if (strlen(sql_row[3]) <= 1)
				sql_row[3] = sql_row[2];	/* no vhost, so using real host */
			tt = atoi(sql_row[7]);
			tm = *localtime(&tt);
			strftime(buf, sizeof(buf), "%d.%m %H:%M", &tm);
			time0 = mktime(&tm);
			tsdiff = difftime(tsnow, time0);
			/* Check if we can display the channel */
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
			sql_query("SELECT %s.channel FROM %s, %s WHERE %s.nick = \"%s\" AND %s.channel = \"%s\" %s%s%s%s%s%s%s ORDER BY %s.channel ASC LIMIT 1",
					  ChanTable, ChanTable, UserTable, UserTable,
					  sql_row[0], ChanTable, sql_row[6], cmodep,
					  cmodes, cmodeA, cmodeO, umodep, umodeQ, umodeI, ChanTable);
			sql_res2 = sql_set_result(sqlcon);
			while ((sql_row2 = sql_fetch_row(sql_res2)) != NULL) {
				usrchan = sstrdup(sql_row2[0]);
			}
			sql_free_result(sql_res2);

			if (stricmp(sql_row[4], "Y") == 0) {
				if (stricmp(sql_row[5], "Y") != 0) {  /* online */
					if (usrchan) {
						sprintf(message,
								moduleGetLangString(u, LASTSPOKE_ONLINE_C),
								sql_row[0], sql_row[1], sql_row[3],
								get_timestring(u, tsdiff), buf,
								sql_row[6]);
					} else {
						sprintf(message,
								moduleGetLangString(u, LASTSPOKE_ONLINE_G),
								sql_row[0], sql_row[1], sql_row[3],
								get_timestring(u, tsdiff), buf);
					}
				} else {		/* away */
					if (usrchan) {
						sprintf(message,
								moduleGetLangString(u, LASTSPOKE_AWAY_C),
								sql_row[0], sql_row[1], sql_row[3],
								get_timestring(u, tsdiff), buf,
								sql_row[6]);
					} else {
						sprintf(message,
								moduleGetLangString(u, LASTSPOKE_AWAY_G),
								sql_row[0], sql_row[1], sql_row[3],
								get_timestring(u, tsdiff), buf);
					}
				}
			} else {			/* offline */
				if (usrchan) {
					sprintf(message,
							moduleGetLangString(u, LASTSPOKE_OFFLINE_C),
							sql_row[0], sql_row[1], sql_row[3],
							get_timestring(u, tsdiff), buf, sql_row[6]);
				} else {
					sprintf(message,
							moduleGetLangString(u, LASTSPOKE_OFFLINE_G),
							sql_row[0], sql_row[1], sql_row[3],
							get_timestring(u, tsdiff), buf);
				}
			}
		}
	} else {
		sprintf(message, moduleGetLangString(u, LASTSPOKE_EMPTY), target);
	}
	
	sql_free_result(sql_res);
	if (lastspokenick)
		free(lastspokenick);
	if (usrchan)
		free(usrchan);
	if (uname)
		free(uname);

	return message;
}

char *get_timestring(User * u, int timestamp)
{
	int days, hours, mins, secs;
	char *tsbuf;
	char dd[16] = "\0";
	char dh[16] = "\0";
	char dm[16] = "\0";
	char ds[16] = "\0";

	alog(LOG_DEBUG, "DEBUG: got timestamp %i", timestamp);

	tsbuf = malloc(64);
	days = timestamp / (3600 * 24);
	timestamp %= (3600 * 24);
	hours = timestamp / 3600;
	timestamp %= 3600;
	mins = timestamp / 60;
	secs = timestamp % 60;

	alog(LOG_DEBUG, "DEBUG: got timestamp %i", timestamp);

	if (days == 1)
		sprintf(dd, "%i %s", days,
				moduleGetLangString(u, LASTSPOKE_DAY));
	else if (days > 1)
		sprintf(dd, "%i %s", days,
				moduleGetLangString(u, LASTSPOKE_DAYS));

	if (hours == 1)
		sprintf(dh, "%i %s", hours,
				moduleGetLangString(u, LASTSPOKE_HOUR));
	else if (hours > 1 || (hours == 0 && days > 0))
		sprintf(dh, "%i %s", hours,
				moduleGetLangString(u, LASTSPOKE_HOURS));

	if (mins == 1)
		sprintf(dm, "%i %s", mins,
				moduleGetLangString(u, LASTSPOKE_MINUTE));
	else if (mins > 1 || (mins == 0 && hours > 0))
		sprintf(dm, "%i %s", mins,
				moduleGetLangString(u, LASTSPOKE_MINUTES));

	if (days == 0 && hours == 0 && mins == 0) {
		if (secs == 1)
			sprintf(ds, "%i %s", secs,
					moduleGetLangString(u, LASTSPOKE_SECOND));
		else
			sprintf(ds, "%i %s", secs,
					moduleGetLangString(u, LASTSPOKE_SECONDS));
	}

	sprintf(tsbuf, "%s%s%s%s", dd, dh, dm, ds);

	return tsbuf;
}

void mAddLanguages(void)
{
	const char *langtable_en_us[] = {
		"Usage: \00310\002%slastspoke <nick/mask>\002\003",
		"\00310\002%s\002\003 (%s@%s) is \0033online\003 and has last been seen saying something %s ago (%s) on %s",
		"\00310\002%s\002\003 (%s@%s) is \0033online\003 and has last been seen saying something %s ago (%s)",
		"\00310\002%s\002\003 (%s@%s) is \0037away\003 and has last been seen saying something %s ago (%s) on %s",
		"\00310\002%s\002\003 (%s@%s) is \0037away\003 and has last been seen saying something %s ago (%s)",
		"\00310\002%s\002\003 (%s@%s) is \0034offline\003 and has last been seen saying something %s ago (%s) on %s",
		"\00310\002%s\002\003 (%s@%s) is \0034offline\003 and has last been seen saying something %s ago (%s)",
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
		"Syntax: \00310\002%slastspoke <nick/mask>\002\003",
		"\00310\002%s\002\003 (%s@%s) ist \0033online\003 und hat zulezt vor %s (%s) etwas in %s gesagt",
		"\00310\002%s\002\003 (%s@%s) ist \0033online\003 und hat zulezt vor %s (%s) etwas gesagt",
		"\00310\002%s\002\003 (%s@%s) ist \00377abwesend\003 und hat zulezt vor %s (%s) etwas in %s gesagt",
		"\00310\002%s\002\003 (%s@%s) ist \00377abwesend\003 und hat zulezt vor %s (%s) etwas gesagt",
		"\00310\002%s\002\003 (%s@%s) ist \0034offline\003 und hat zulezt vor %s (%s) etwas in %s gesagt",
		"\00310\002%s\002\003 (%s@%s) ist \0034offline\003 und hat zulezt vor %s (%s) etwas gesagt",
		"Tage, ",
		"Stunden und ",
		"Minuten",
		"Sekunden",
		"Tag, ",
		"Stunde und ",
		"Minute",
		"Sekunde",
#ifdef __clang__
		"\0034Keine Ergebnisse\003 f\u00FCr \00310\002%s\002\003",
#else
		"\0034Keine Ergebnisse\003 für \00310\002%s\002\003",
#endif
		"Interner Fehler",
	};

	const char *langtable_es[] = {
		"Uso: \00310\002%slastspoke <nick/mask>\002\003",
		"\00310\002%s\002\003 (%s@%s) esta \0033online\003 y ha sido visto diciendo algo %s atras (%s) en %s",
		"\00310\002%s\002\003 (%s@%s) esta \0033online\003 y ha sido visto diciendo algo %s atras (%s)",
		"\00310\002%s\002\003 (%s@%s) esta \0037ausente\003 y ha sido visto diciendo algo %s atras (%s) en %s",
		"\00310\002%s\002\003 (%s@%s) esta \0037ausente\003 y ha sido visto diciendo algo %s atras (%s)",
		"\00310\002%s\002\003 (%s@%s) esta \0034offline\003 y ha sido visto diciendo algo %s atras (%s) en %s",
		"\00310\002%s\002\003 (%s@%s) esta \0034offline\003 y ha sido visto diciendo algo %s atras (%s)",
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

	const char *langtable_it[] = {
#ifdef __clang__
		"Usage: \00310\002%slastspoke <nick/mask>\002\003",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0033online\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0033online\003 e ha detto qualcosa %s fa (%s)",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0037assente\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0037assente\003 e ha detto qualcosa %s fa (%s)",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0034offline\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) \u00E8 \0034offline\003 e ha detto qualcosa %s fa (%s)",
#else
		"Usage: \00310\002%slastspoke <nick/mask>\002\003",
		"\00310\002%s\002\003 (%s@%s) è \0033online\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) è \0033online\003 e ha detto qualcosa %s fa (%s)",
		"\00310\002%s\002\003 (%s@%s) è \0037assente\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) è \0037assente\003 e ha detto qualcosa %s fa (%s)",
		"\00310\002%s\002\003 (%s@%s) è \0034offline\003 e ha detto qualcosa %s fa (%s) in %s",
		"\00310\002%s\002\003 (%s@%s) è \0034offline\003 e ha detto qualcosa %s fa (%s)",
#endif
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

	moduleInsertLanguage(LANG_EN_US, LANG_NUM_STRINGS,
						 (char **) langtable_en_us);
	moduleInsertLanguage(LANG_DE, LANG_NUM_STRINGS,
						 (char **) langtable_de);
	moduleInsertLanguage(LANG_ES, LANG_NUM_STRINGS,
						 (char **) langtable_es);
	moduleInsertLanguage(LANG_IT, LANG_NUM_STRINGS,
						 (char **) langtable_it);
}
