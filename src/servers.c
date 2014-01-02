/*
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 *
 *
 */

#include "denora.h"

Server *servlist = NULL;
Server *me_server = NULL;
char *uplink;
char *TS6SID;
char p10id[3];
char *TS6UPLINK;
Server *serverlist[1024];
ServStats *servstatlist[1024];
static ServStats *currentss;
static int next_index;
static Server *server_cur;
Server *server_ping(Server * s);
Server *server_uptime(Server * s);
Server *make_server(const char *servername, char *descript,
                    Server * servuplink, char *numeric);
int find_server_link(Server * s, const char *servname);
int UplinkSynced;

/*************************************************************************/

CapabInfo capab_info[] =
{
	{"NOQUIT", CAPAB_NOQUIT},
	{"TSMODE", CAPAB_TSMODE},
	{"UNCONNECT", CAPAB_UNCONNECT},
	{"NICKIP", CAPAB_NICKIP},
	{"SSJOIN", CAPAB_NSJOIN},
	{"ZIP", CAPAB_ZIP},
	{"BURST", CAPAB_BURST},
	{"TS5", CAPAB_TS5},
	{"TS3", CAPAB_TS3},
	{"DKEY", CAPAB_DKEY},
	{"PT4", CAPAB_PT4},
	{"SCS", CAPAB_SCS},
	{"QS", CAPAB_QS},
	{"UID", CAPAB_UID},
	{"KNOCK", CAPAB_KNOCK},
	{"CLIENT", CAPAB_CLIENT},
	{"IPV6", CAPAB_IPV6},
	{"SSJ5", CAPAB_SSJ5},
	{"SN2", CAPAB_SN2},
	{"TOK1", CAPAB_TOKEN},
	{"TOKEN", CAPAB_TOKEN},
	{"VHOST", CAPAB_VHOST},
	{"SSJ3", CAPAB_SSJ3},
	{"SJB64", CAPAB_SJB64},
	{"CHANMODES", CAPAB_CHANMODE},
	{"NICKCHARS", CAPAB_NICKCHARS},
	{NULL, 0}
};

/*************************************************************************/

void update_sync_state(char *source, int ac)
{
	if (ac == SYNC_COMPLETE)
	{
		if (!BadPtr(source))
		{
			DenoraSQLQuery(DenoraDB, "UPDATE %s SET sync=1 WHERE name=%s", ServerTable, source);
			send_event(EVENT_SERVER_SYNC_COMPLETE, 1, source);
		}
		else
		{
			DenoraSQLQuery(DenoraDB, "UPDATE %s SET sync=1 WHERE name=%s", ServerTable, denora->uplink);
			send_event(EVENT_UPLINK_SYNC_COMPLETE, 1, denora->uplink);
			UplinkSynced = 1;
		}
	}
}

/*************************************************************************/

void sql_motd_store(Server * s)
{
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET motd=\'%q\' WHERE server=\'%q\'",
	          ServerTable, s->motd, s->name);

}

/*************************************************************************/

/**
 * This function makes a new Server structure
 * @param server Server Name
 * @return Server Struct
 */
ServStats *make_servstats(const char *server)
{
	ServStats *s, **list;

	
	s = calloc(sizeof(ServStats), 1);
	if (!server)
	{
		server = "";
	}
	strlcpy(s->name, server, SERVERMAX);
	list = &servstatlist[STATSSERVERHASH(s->name)];
	s->next = *list;
	if (*list)
		(*list)->prev = s;
	*list = s;
	s->currentusers = 0;
	return s;
}

/*************************************************************************/

/**
 * This function makes a new Server structure
 * @param servernam Server Name
 * @param descript is the server description
 * @param servuplink is the server struct for the uplink
 * @param numeric is the server numeric
 * @return Server Struct
 */
Server *make_server(const char *servername, char *descript,
                    Server * servuplink, char *numeric)
{
	Server *serv;
	ServStats *ss;
	const char *country_name;
	const char *country_code;
	int country_id = 0;
	GeoIPLookup *gl;

	serv = calloc(sizeof(Server), 1);
	serv->name = sstrdup(servername);
	if (!BadPtr(descript))
	{
		serv->desc = sstrdup(descript);
	}
	serv->uplink = servuplink;
	if (numeric)
	{
		serv->suid = sstrdup(numeric);
	}
	else
	{
		serv->suid = NULL;
	}
	serv->sync = -1;
	serv->links = NULL;
	serv->prev = NULL;

	if (!servuplink)
	{
		serv->hops = 0;
		serv->next = servlist;
		if (servlist)
		{
			servlist->prev = serv;
		}
		servlist = serv;
	}
	else
	{
		serv->hops = servuplink->hops + 1;
		serv->next = servuplink->links;
		if (servuplink->links)
		{
			servuplink->links->prev = serv;
		}
		servuplink->links = serv;
	}
	ss = findserverstats(servername);
	
	if (!ss)
	{
		serv->ss = make_servstats(servername);
	}
	else
	{
		serv->ss = ss;
		serv->ss->split_stats = 0;
	}
	if (!serv->ss->maxusertime)
	{
		serv->ss->maxusertime = time(NULL);
	}
	if (!serv->country || !serv->countrycode)
	{
		if (!LargeNet)
		{
			country_id = GeoIP_id_by_name_gl(gidb, servername, gl);
			if (country_id == 0)
				country_id = GeoIP_id_by_name_v6_gl(gidb_v6, servername, gl);

			country_name = GeoIP_name_by_id(country_id);
			country_code = GeoIP_code_by_id(country_id);
		}

		if (country_id == 0)
		{
			country_name = "Unknown";
			country_code = "??";
		}

		serv->country = (char *)country_name;
		serv->countrycode = (char *)country_code;
	}

	return serv;
}

/*************************************************************************/

Server *do_server(const char *source, char *servername, char *hops,
                  char *descript, char *numeric)
{
	char *uplinkserver;
	char *sqlservername = NULL;
	Server *serv;
	char buf[BUFSIZ];
	char mbuf[NET_BUFSIZE];
	Server *servuplink;
	int servid;
	int add = 1;
	int juped = 0;
	int upservid = 0;

	*buf = '\0';


	if ((!source || !*source))
	{
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO), servername);
		if (stricmp(servername, ServerName))
		{
			servuplink = NULL;
			uplinkserver = NULL;
		}
		else
		{
			uplinkserver = sstrdup(ServerName);
			servuplink = me_server;
		}
	}
	else
	{
		servuplink = server_find(source);
		if (!servuplink)
		{
			uplinkserver = sstrdup(ServerName);
			servuplink = me_server;
			alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO), servername);
		}
		else
		{
			uplinkserver = sstrdup(servuplink->name);
			alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO_FORM), servername,
			     servuplink->name);
		}
	}

	if (servuplink)
	{
		ARRAY_EXTEND(servuplink->slinks);
		servuplink->slinks[servuplink->slinks_count - 1] =
		    sstrdup(servername);
	}

	
	serv = make_server(servername, descript, servuplink, numeric);
	

	if (servuplink && JupeMaster)
	{
		if (!stricmp(servuplink->name, JupeMaster))
		{
			serv->flags = SERVER_JUPED;
			juped = 1;
		}
	}

	if (!juped && stricmp(servername, ServerName))
	{
		denora_cmd_version(servername);
		denora_cmd_stats(s_StatServ, "u", servername);
		if (ircd->uline)
		{
			denora_cmd_stats(s_StatServ, "U", servername);
		}
		if (ircd->spamfilter)
		{
			char sbuf[2];
			ircsnprintf(sbuf, sizeof(sbuf), "%c", ircd->spamfilter);
			denora_cmd_stats(s_StatServ, sbuf, servername);
		}
		denora_motd(s_StatServ, servername);
	}
	DenoraSQLUpdateStatsInt(DenoraDB, CurrentTable, "servers", 1);

	if (juped)
	{
		send_event(EVENT_JUPED_SERVER, 1, servername);
	}
	else
	{
		send_event(EVENT_SERVER, 1, servername);
	}

	
	if (denora->do_sql)
	{
		servid = db_getserver(servername);
		if (uplinkserver)
		{
			upservid = db_getserver(uplinkserver);
		}

		if (ServerCacheTime && servid > 0)
		{
			enoraSQLQuery(DenoraDB, "UPDATE %s SET server=\'%s\', hops=\'%s\', comment=\'%s\', connecttime=NOW(), linkedto=%d, online=\'Y\', maxusers=%d, maxusertime=%d, lastsplit=FROM_UNIXTIME(%ld) WHERE servid=%d",
			          ServerTable, servername, hops, descript, upservid,
			          serv->ss->maxusers, serv->ss->maxusertime,
			          (long int) serv->ss->lastseen, servid);
			add = 0;
		}

		if (add)
		{
			if (KeepServerTable)
			{
				enoraSQLQuery(DenoraDB, "INSERT INTO %s (server, country, countrycode, hops, comment, linkedto, connecttime, maxusers, maxusertime, lastsplit) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d, NOW(), %d, %d, FROM_UNIXTIME(%ld)) ON DUPLICATE KEY UPDATE hops=\'%s\', comment=\'%s\', linkedto=%d, connecttime=NOW(), maxusers=%d, maxusertime=%d, lastsplit=FROM_UNIXTIME(%ld)",
				 ServerTable, servername, serv->country, serv->countrycode, hops, descript, upservid,
				 serv->ss->maxusers, serv->ss->maxusertime,
				 serv->ss->lastseen, hops, descript,
				 upservid, serv->ss->maxusers,
				 serv->ss->maxusertime, (long int) serv->ss->lastseen);
			}
			else
			{
				enoraSQLQuery(DenoraDB, "INSERT INTO %s (server, country, countrycode, hops, comment, linkedto, connecttime, maxusers, maxusertime, lastsplit) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d, NOW(), %d, %d, FROM_UNIXTIME(%ld))",
				 ServerTable, servername, serv->country, serv->countrycode, hops, descript, upservid,
				 serv->ss->maxusers, serv->ss->maxusertime,
				 (long int) serv->ss->lastseen);
			}
			servid = db_getserver(servername);
		}

		free(descript);

		if (!stricmp(servername, ServerName))
		{
			DenoraSQLQuery(DenoraDB, "UPDATE %s SET uptime=%ld, version=\'Denora-%s\' WHERE name=%q",	 ServerTable, (((long int) time(NULL)) - denora->start_time), denora->version, servername);
			if (MOTDFilename)
			{
				*buf = '\0';
				buf = FileGetContents(MOTDFilename);
				DenoraSQLQuery(DenoraDB, "UPDATE %s SET motd=q WHERE name=%q",	 ServerTable, buf, servername);
				free(buf);
			}
		}
	}

	
	if (ServerCacheTime)
	{
		db_cleanserver();
	}
	
	do_checkservsmax();
	return serv;
}

/*************************************************************************/

void sql_uline(char *server)
{
	/* On Ratbox - ulines are also shared so you can
	   get *.your.net at times so we ignore any server
	   that starts with * 
    */
	if (*server != '*')
	{
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET uline=1 WHERE name=%q",
			          ServerTable, server);
	}
}

/*************************************************************************/

void server_store_pong(char *source, uint32 ts)
{
	uint32 lastping, ping, highestping;

	lastping = DenoraSQLGetFieldInt(DenoraDB, ServerTable, "lastping", "name", source);
	ping = ts - lastping;
	DenoraSQLUpdateInt(DenoraDB, ServerTable, "ping", ping, "name", source);
	alog(LOG_DEBUG, langstr(ALOG_DEBUG_PINGTIME), source, ping, ts,  lastping);
	highestping = DenoraSQLGetFieldInt(DenoraDB, ServerTable, "highestping", "name", source);
	if (ping >= highestping)
	{
		DenoraSQLUpdateInt(DenoraDB, ServerTable, "highestping", ping, "name", source);
		DenoraSQLUpdateInt(DenoraDB, ServerTable, "maxpingtime", time(NULL), "name", source);
	}
	DenoraSQLUpdateInt(DenoraDB, ServerTable, "lastpingtime", ts, "name", source);
}

/*************************************************************************/

void ping_servers(void)
{
	sqlite3 *db;
	int rows;
	sqlite3_stmt * stmt;
	char **data;

/*
	 Note to self to fix 
	s->flags != SERVER_JUPED
*/
	db = DenoraOpenSQL(DenoraDB);
	rows = DenoraSQLGetNumRows(db, ServerTable);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", ServerTable);
	data = DenoraSQLFetchArray(db, ServerTable, stmt, FETCH_ARRAY_NUM);
	for (i = 0; i < rows; i++)
	{
			denora_cmd_ping(data[0]);
	}
	free(data);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);
}

/*************************************************************************/

void server_uptime(void)
{
	sqlite3 *db;
	int rows;
	sqlite3_stmt * stmt;
	char **data;

	db = DenoraOpenSQL(DenoraDB);
	rows = DenoraSQLGetNumRows(db, ServerTable);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", ServerTable);
	data = DenoraSQLFetchArray(db, ServerTable, stmt, FETCH_ARRAY_NUM);
	for (i = 0; i < rows; i++)
	{
			denora_cmd_stats(s_StatServ, "u", data[0]);
	}
	free(data);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	DenoraSQLUpdateInt(DenoraDB, ServerTable, "uptime", (((long int) time(NULL)) - denora->start_time), "name", ServerName);
}

/*************************************************************************/

/**
 * Remove and free a Server structure. This function is the most complete
 * remove treatment a server can get, as it first quits all clients which
 * still pretend to be on this server, then it walks through all connected
 * servers and disconnects them too. If all mess is cleared, the server
 * itself will be too.
 * @param serv is the Server struct
 * @param quitreason the server quit message
 * @return void
 */
void delete_server(char *serv, const char *quitreason, int depth)
{
	User *u, *unext;
	Server *s;
	int i, x;

	if (!serv)
	{
		alog(LOG_DEBUG, langstr(ALOG_ERR_DEL_SERVER));
		return;
	}

	alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_FOR), serv);

	if (ircdcap->noquit || ircdcap->qs)
	{
		alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_NOQUIT));
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET online=\'N\',lastquitmsg=%q, lastquit=%ld WHERE server=%q", UserTable, (char *) quitreason, time(NULL), serv);
	}
	else
	{
		alog(LOG_DEBUG, "uplink does not support NOQUIT or QS on SQUIT");
		alog(LOG_DEBUG, "ircdcap->noquit %d ircdcap->qs %d",
		     ircdcap->noquit, ircdcap->qs);
	}

	DenoraSQLQuery(DenoraDB, "UPDATE %s SET online=\'N\',lastquitmsg=%q, lastsplit=%ld WHERE linkedto=%q", ServerTable, (char *) quitreason, time(NULL), serv);

	send_event(EVENT_SQUIT, 2, serv, quitreason);

	sql_do_squit(serv->name);

	DenoraSQLUpdateStatsInt(DenoraDB, "servers", 0);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET splits=splits+1,split_stats=1,lastseen=%ld WHERE server=%q", ServerTable, time(NULL), serv);


	alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_DONE));
}

/*************************************************************************/

void sql_do_uptime(char *source, char *uptime)
{
	char *tmp = NULL;
	char *tmp4 = NULL;
	char *days = NULL;
	int32 days_int = 0;
	char *hours = NULL;
	int32 hours_int = 0;
	char *mins = NULL;
	int32 mins_int = 0;
	char *secs = NULL;
	int32 secs_int = 0;
	int32 total = 0;
	Server *s;
	int servid = 0;

	if (LargeNet)
	{
		return;
	}
	s = server_find(source);

	if (!s)
	{
		alog(LOG_DEBUG, "Nonexistant server (%s) uptime", source);
		return;
	}

	if (uptime)
	{
		tmp = myStrGetToken(uptime, ' ', 0);
	}

	if (tmp)
	{
		/* Thales/NeoStats send more then the rest */
		if (!stricmp(tmp, "GNU") || !stricmp(tmp, "Statistical"))
		{
			days = myStrGetToken(uptime, ' ', 3);
			tmp4 = myStrGetTokenRemainder(uptime, ' ', 5);
			/* Sentinel - very none standard reply */
			/* :sentinel.nomadirc.net 242 Denora :Statistics up 8 minutes, 53 seconds */
		}
		else if (!stricmp(tmp, "Statistics"))
		{
			tmp4 = myStrGetToken(uptime, ' ', 3);
			if (!stricmp(tmp4, "minutes,") || !stricmp(tmp4, "minute,"))
			{
				mins = myStrGetToken(uptime, ' ', 2);
				secs = myStrGetToken(uptime, ' ', 4);
			}
			else if (!stricmp(tmp4, "hours,") || !stricmp(tmp4, "hour,"))
			{
				hours = myStrGetToken(uptime, ' ', 2);
				mins = myStrGetToken(uptime, ' ', 4);
			}
			else if (!stricmp(tmp4, "days,") || !stricmp(tmp4, "day,"))
			{
				days = myStrGetToken(uptime, ' ', 2);
				tmp4 = myStrGetTokenRemainder(uptime, ' ', 4);
				hours = myStrGetToken(tmp4, ':', 0);
				mins = myStrGetToken(tmp4, ':', 1);
			}
			else
			{
				alog(LOG_DEBUG, "Non-Standard Reply for UPTIME");
				free(tmp4);
				free(tmp);
				return;
			}
			tmp4 = NULL;
		}
		else
		{
			days = myStrGetToken(uptime, ' ', 2);
			tmp4 = myStrGetTokenRemainder(uptime, ' ', 4);
		}
	}

	if (tmp4)
	{
		hours = myStrGetToken(tmp4, ':', 0);
		mins = myStrGetToken(tmp4, ':', 1);
		secs = myStrGetToken(tmp4, ':', 2);
		free(tmp4);
	}

	if (mins)
	{
		mins_int = atol(mins) * 60;
		free(mins);
	}
	if (secs)
	{
		secs_int = atol(secs);
		free(secs);
	}
	if (hours)
	{
		hours_int = atol(hours) * 3600;
		free(hours);
	}
	if (days)
	{
		days_int = atol(days) * 86400;
		free(days);
	}

	total = mins_int + secs_int + hours_int + days_int;

	DenoraSQLQuery(DenoraDB, "UPDATE %s SET uptime=%ld WHERE name=%d",
	          ServerTable, total, source);

	if (tmp)
		free(tmp);
	return;
}

/*************************************************************************/

/* SDESC */
void sql_do_sdesc(char *user, char *msg)
{
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET comment=\'%q\' WHERE name=%q",
	          ServerTable, msg, db_getservfromnick(user));
}

/*************************************************************************/

void server_set_desc(char *server, char *msg)
{
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET comment=\'%q\' WHERE name=%q",
	          ServerTable, msg, server);
}

/*************************************************************************/

/* Asuka
   AB 351 BjAAA u2.10.11.07+asuka(1.2.1). Auska.Nomadirc.net :B96AeEFfIKMpSU

   Viagra
   viagra.nomadirc.net 351 Denora VIAGRA_IRCd 1.3.0 - BETA 7. viagra.nomadirc.net :Ci TS5ow-r[BETA]
                            0       1          2    3  4   5   6                    7

   RageIRCD
   rage2.nomadirc.net 351 Denora :rage(bluemoon)-2.0.0.beta-8(1.956) rage2.nomadirc.net EBTsIFcMO TS5ow-r[BETA]
                           0       1

    IRCNet
    :ircnet.nomadirc.net 351 StatServ 2.11.1p1. ircnet.nomadirc.net 000A :aEFHJKMRTu

    Atheme IRC Services
    :services.nomadirc.net 351 StatServ :atheme-0.2. services.nomadirc.net FljR TS5ow
    :services.nomadirc.net 351 StatServ :Compile time: Thu Jun 16 2005 at 00:21:15 EDT, build-id 525, build 4
    :services.nomadirc.net 351 StatServ :Compiled on: [Linux denora.elite-irc.net 2.6.9 #1 Fri Oct 22 21:07:15 EDT 2004 i686 unknown]
*/
void sql_do_server_version(char *server, int ac, char **av)
{
	char *version = NULL;
	char *sqlversion = NULL;
	char *temp = NULL;
	char buf[100];
	int servid;
	Server *s;
	char *tmp2;
	*buf = '\0';

	/* Viagra */
	if (ac == 8)
	{
		ircsnprintf(buf, 100, "%s %s %s %s %s", av[1], av[2], av[3], av[4],
		            av[5]);
		version = sstrdup(buf);
	}

	/* Unreal / P10 / X3 */
	if (ac == 4)
	{
		if (!stricmp(av[1], "x3"))
		{
			ircsnprintf(buf, 100, "%s %s", av[1], av[2]);
			version = sstrdup(buf);
		}
		else
		{
			version = sstrdup(av[1]);
		}
	}

	/* Anope/Thales respond like so */
	/* QuakeIRCD uses 6 */
	/* X3 (versions as of 1.8) */
	if (ac == 5 || ac == 6)
	{
		if (!stricmp(av[1], "x3") && ac == 5)
		{
			ircsnprintf(buf, 100, "%s %s %s", av[1], av[2], av[3]);
		}
		else if ((denora_get_ircd() == IRC_ULTIMATE3) && (ac == 6))
		{
			ircsnprintf(buf, 100, "%s %s %s", av[1], av[2], av[3]);
		}
		else
		{
			ircsnprintf(buf, 100, "%s %s", av[1], av[2]);
		}
		version = sstrdup(buf);
	}

	/* SolarStats and NeoStats like this */
	if (ac == 2)
	{
		tmp2 = myStrGetToken(av[1], ' ', 0);
		temp = sstrdup(tmp2);
		free(tmp2);
		/* if you have NeoStats mods load they to will respond on VERSION saying they are a
		   module */
		if (!stricmp(temp, "MODULE"))
		{
			free(temp);
			return;
			/* Atheme uses 351 to tell the user what os it was
			   built on */
		}
		else if (!stricmp(temp, "Compile"))
		{
			free(temp);
			return;
		}
		else if (!stricmp(temp, "Compiled"))
		{
			free(temp);
			return;
		}
		else
		{
			version = sstrdup(temp);
			free(temp);
		}
	}

	/* inspircd */
	if (ac == 1 && !BadPtr(av[0]))
	{
		version = sstrdup(av[0]);
	}

	if (!version)
	{
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_UNKNOWN_VERSION_REPLY));
		alog(LOG_DEBUG, "debug: ac count is %d", ac);
		return;
	}

	DenoraSQLQuery(DenoraDB, "UPDATE %s SET version=\'%q\' WHERE name=%q",
			          ServerTable, version, server);
	free(version);
}

/*************************************************************************/

/* SQUIT */
void sql_do_squit(char *server)
{
	if (ServerCacheTime)
	{
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET online=\'N\', lastsplit=NOW(),linkedto=NULL WHERE name=%q",
		 ServerTable, server);
		db_cleanserver();
	}
	else
	{
		DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE server=\'%s\'",
		          ServerTable, server);
	}
}

/*************************************************************************/

/**
 * Handle removing the server from the Server struct
 * @param servername Name of the server leaving
 * @return void
 */
void do_squit(char *servername)
{
	char buf[BUFSIZE];
	Server *s;
	*buf = '\0';
	s = server_find(servername);
	if (!s)
	{
		alog(LOG_NONEXISTANT, "SQUIT for nonexistent server (%s)!!",
		     servername);
		return;
	}

	ircsnprintf(buf, BUFSIZE - 1, "%s %s", s->name,
	            (s->uplink ? s->uplink->name : ""));

	if (ircdcap->unconnect)
	{
		if (s->uplink && me_server->uplink)
		{
			if (!stricmp(s->uplink->name, me_server->uplink->name)
			        && (denora->capab & ircdcap->unconnect))
			{
				alog(LOG_DEBUG, "debug: Sending UNCONNECT SQUIT for %s",
				     s->name);
				denora_cmd_squit(s->name, buf);
			}
		}
	}
	delete_server(servername, buf, 0);
}

/*************************************************************************/

/**
 * Handle parsing the CAPAB/PROTOCTL messages
 * @param ac Number of arguments in av
 * @param av Agruments
 * @return void
 */
void capab_parse(int ac, char **av)
{
	int i;
	int j;
	char *s, *tmp;

	char *temp;

	for (i = 0; i < ac; i++)
	{
		temp = av[i];

		s = myStrGetToken(temp, '=', 0);
		tmp = myStrGetTokenRemainder(temp, '=', 1);

		if (!s)
		{
			free(tmp);
			continue;
		}

		for (j = 0; capab_info[j].token; j++)
		{
			if (stricmp(s, capab_info[j].token) == 0)
				denora->capab |= capab_info[j].flag;
			/* Special cases */
			if ((stricmp(s, "NICKIP") == 0) && !ircd->nickip)
				ircd->nickip = 1;
			if ((stricmp(s, "CHANMODES") == 0) && tmp)
				ircd->chanmodes = sstrdup(tmp);
			if ((stricmp(s, "NICKCHARS") == 0) && tmp)
				ircd->nickchars = sstrdup(tmp);
		}

		free(s);
		free(tmp);
	}
}

/*************************************************************************/

/* TS6 UID generator common code.
 *
 * Derived from atheme-services, uid.c (hg 2954:116d46894b4c).
 *         -nenolod
 */
static int ts6_uid_initted = 0;
static char ts6_new_uid[10];    /* allow for \0 */
static unsigned int ts6_uid_index = 9;  /* last slot in uid buf */

void ts6_uid_init(void)
{
	/* check just in case... you can never be too safe. */
	if (TS6SID != NULL)
	{
		ircsnprintf(ts6_new_uid, 10, "%sAAAAAA", TS6SID);
		ts6_uid_initted = 1;
	}
	else
	{
		alog(LOG_NORMAL,
		     "warning: no TS6SID specified, disabling TS6 support.");
		UseTS6 = 0;
		return;
	}
}

void ts6_uid_increment(unsigned int slot)
{
	if (slot != strlen(TS6SID))
	{
		if (ts6_new_uid[slot] == 'Z')
			ts6_new_uid[slot] = '0';
		else if (ts6_new_uid[slot] == '9')
		{
			ts6_new_uid[slot] = 'A';
			ts6_uid_increment(slot - 1);
		}
		else
			ts6_new_uid[slot]++;
	}
	else
	{
		if (ts6_new_uid[slot] == 'Z')
			for (slot = 3; slot < 9; slot++)
				ts6_new_uid[slot] = 'A';
		else
			ts6_new_uid[slot]++;
	}
}

char *ts6_uid_retrieve(void)
{
	if (ts6_uid_initted != 1)
		ts6_uid_init();

	ts6_uid_increment(ts6_uid_index - 1);

	return ts6_new_uid;
}
