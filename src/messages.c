/*
 *
 * (c) 2004-2014 Denora Team
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

/*************************************************************************/

/**
 * Handle nick collide messages to make sure we are still connected
 *
 * @param user is the nick of the person whom was part of the collide
 *
 * @return always returns MOD_CONT
 */
int m_nickcoll(char *user)
{
	if (nickIsServices(user))
	{
		introduce_user(user);
	}
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle AWAY messages
 *
 * @param source is the nick of the person whom is now away
 * @param msg is the text of the away message
 *
 * @return always returns MOD_CONT
 */
int m_away(char *source, char *msg)
{
	User *u;

	if (!source)
	{
		return MOD_CONT;
	}

	u = user_find(source);
	if (!u)
	{
		return MOD_CONT;
	}

	DenoraSQLGetStatsUpdateData(DenoraDB, "stats", "away", (msg ? 1 : 0));
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET away=\'%s\', awaymsg=\'%q\' WHERE nick=%q",
		 UserTable, (msg ? (char *) "Y" : (char *) "N"),  msg, u->nick);
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle KILL messages
 *
 * @param source is the server or nick that send the message
 * @param nick is the user name that was killed
 * @param msg is the text of the kill message
 *
 * @return void - no returend value
 */
void m_kill(char *source, char *nick, char *msg)
{
	User *u;
	ServStats *s;
	int id;

	if ((u = user_find(source)))
	{
		if ((s = findserverstats(u->server->name)))
		{
			s->ircopskills++;
		}
		send_event(EVENT_GLOBAL_KILL, 3, source, nick, msg);
	}
	else if ((s = findserverstats(source)))
	{
		s->serverkills++;
		send_event(EVENT_SERVER_KILL, 3, source, nick, msg);
	}
	if (denora->do_sql)
	{
		db_removenick(nick, msg);
		if (UserCacheTime)
		{
			db_cleanuser();
		}
	}
	if (s && denora->do_sql)
	{
		id = db_getserver(s->name);
		if (id > 0)
		{
			sql_query
			(
			 "UPDATE %s SET ircopskills=%d, serverkills=%ld WHERE servid=%d",
			 ServerTable, s->ircopskills, s->serverkills, id);
		}
	}
	do_kill(nick, msg);
	if (nickIsServices(nick))
	{
		introduce_user(nick);
	}
}

/*************************************************************************/

/**
 * Handle TIME messages
 *
 * @param source is the nick of the person whom requested
 * @param ac is the parameter count
 * @param av is the array
 *
 * @return always returns MOD_CONT
 */
int m_time(char *source, int ac, char **av)
{
	time_t t;
	struct tm tm;
	char buf[64];

	USE_VAR(ac);
	USE_VAR(av);

	*buf = '\0';

	if (!source)
	{
		return MOD_CONT;
	}

	time(&t);
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	tm = *localtime(&t);
#endif
	strftime(buf, 63, "%a %b %d %H:%M:%S %Y %Z", &tm);
	denora_cmd_391(source, buf);

	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle MOTD messages
 *
 * @param source is the nick of the person whom requested
 *
 * @return always returns MOD_CONT
 */
int m_motd(char *source)
{
	FILE *f;
	char buf[BUFSIZE];
	*buf = '\0';

	

	if (!source)
	{
		return MOD_CONT;
	}

	denora_cmd_375(source);
	if (!MOTDFilename)
	{
		denora_cmd_422(source);
	}
	else
	{
		if ((f = FileOpen(MOTDFilename, FILE_READ)) != NULL)
		{
			
			while (fgets(buf, BUFSIZE - 1, f))
			{
				buf[strlen(buf) - 1] = 0;
				denora_cmd_372(source, buf);
			}
			fclose(f);
		}
		else
		{
			denora_cmd_422(source);
		}
	}
	
	denora_cmd_376(source);
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle NOTICE commands
 *
 * @param source is the nick of the person whom sent the notice
 * @param receiver is the nick whom it was sent to
 * @param msg is the message that was sent
 *
 * @return always returns MOD_CONT
 */
int m_notice(char *source, char *receiver, char *msg)
{
	char *temp = NULL;
	char *version;
	char *clean;

	if (BadPtr(source) || BadPtr(receiver) || BadPtr(msg))
	{
		return MOD_CONT;
	}

	if (!stricmp(receiver, s_StatServ)
	        || (s_StatServ_alias && !stricmp(receiver, s_StatServ_alias)))
	{
		clean = normalizeBuffer(msg);
		doCleanBuffer((char *) clean);
		temp = myStrGetToken(clean, ' ', 0);
		if (!temp)
		{
			free(clean);
			return MOD_CONT;
		}
		if (!stricmp(temp, "VERSION"))
		{
			version = myStrGetTokenRemainder(clean, ' ', 1);
			handle_ctcp_version(source, version);
			free(version);
		}
		free(clean);
		free(temp);
	}

	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle PRIVMSG commands
 *
 * @param source is the nick of the person whom sent the privmsg
 * @param receiver is the nick whom it was sent to
 * @param msg is the message that was sent
 *
 * @return always returns MOD_CONT
 */
int m_privmsg(char *source, char *receiver, char *msg)
{
	char *s;
	User *u;
	Server *srv;
	PrivMsg *p;

	if (BadPtr(source) || BadPtr(receiver) || BadPtr(msg))
	{
		return MOD_CONT;
	}

	u = user_find(source);

	if (!u)
	{
		srv = server_find(source);
		/* We just ignore the message if sent from a server (inspircd) */
		if (!srv)
		{
			alog(LOG_NONEXISTANT, "%s: user record for %s not found", msg,
			     source);
			denora_cmd_notice(receiver, source,
			                  getstring(NULL, USER_RECORD_NOT_FOUND));
		}
		return MOD_CONT;
	}

	if (*receiver == '#')
	{
		if (denora->do_sql && !LargeNet)
		{
			if (stricmp(u->nick, s_StatServ)
			        || stricmp(u->nick, s_StatServ_alias))
			{
				do_cstats(u, receiver, msg);
			}
		}
	}
	else
	{
		/* If a server is specified (nick@server format), make sure it matches
		 * us, and strip it off. */
		s = strchr(receiver, '@');
		if (s)
		{
			*s++ = 0;
			if (stricmp(s, ServerName) != 0)
				return MOD_CONT;
		}
		p = findPrivMsg(receiver);
		if (p)
		{
			p->handler(u, msg);
			
		}
	}
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle STATS commands
 *
 * @param source is the nick of the person whom sent the stats command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int m_stats(char *source, int ac, char **av)
{
	User *u;
	int i;
	int rows;
	sqlite3_stmt *stmt;
	sqlite3 *db;
	char ***data;

	if (ac < 1)
		return MOD_CONT;

	switch (*av[0])
	{
		case 'l':
			u = user_find(source);

			if (u && is_oper(u))
			{

				denora_cmd_numeric
				(source, 211,
				 "Server SendBuf SentBytes SentMsgs RecvBuf RecvBytes RecvMsgs ConnTime");
				denora_cmd_numeric(source, 211, "%s %d %d %d %d %d %d %ld",
				                   RemoteServer, write_buffer_len(),
				                   total_written, total_recmsg,
				                   read_buffer_len(), total_read,
				                   total_sendmsg,
				                   time(NULL) - denora->start_time);
			}

			denora_cmd_219(source, NULL);
			break;
		case 'o':
		case 'O':
			/* Check whether the user is an operator */
			u = user_find(source);
			if (u && !is_oper(u) && HideStatsO)
			{
				denora_cmd_219(source, av[0]);
			}
			else
			{
				db = DenoraOpenSQL(DenoraDB);
				rows = DenoraSQLGetNumRows(db, "admin");
				stmt = DenoraPrepareQuery(db, "SELECT * FROM admin");
				data = DenoraSQLFetchArray(db, "admin", stmt, FETCH_ARRAY_NUM);
				for (i = 0; i < rows; i++)
				{
					denora_cmd_numeric(source, 243, "O * * %s Admin 0",  data[i][0]);
				}
				free(data);
				sqlite3_finalize(stmt);
				DenoraCloseSQl(db);
				denora_cmd_219(source, av[0]);
			}

			break;

		case 'u':
			{
				int uptime = time(NULL) - denora->start_time;
				denora_cmd_numeric(source, 242,
				                   ":Stats up %d day%s, %02d:%02d:%02d",
				                   uptime / 86400,
				                   (uptime / 86400 == 1) ? "" : "s",
				                   (uptime / 3600) % 24, (uptime / 60) % 60,
				                   uptime % 60);
				denora_cmd_numeric(source, 250,
				                   ":Current users: %d (%d ops); maximum %d",
				                   stats->users, stats->opers,
				                   stats->users_max);
				denora_cmd_219(source, av[0]);
				break;
			}                       /* case 'u' */

		default:
			denora_cmd_219(source, av[0]);
			break;
	}
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle VERSION commands
 *
 * @param source is the nick of the person whom sent the version command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int m_version(char *source, int ac, char **av)
{

	USE_VAR(ac);
	USE_VAR(av);

	if (source)
	{
		denora_cmd_351(source);
	}
	return MOD_CONT;
}


/*************************************************************************/

/**
 * Handle WHOIS commands
 *
 * @param source is the nick of the person whom sent the stats command
 * @param who is the person being whois'd
 *
 * @return always returns MOD_CONT
 */
int m_whois(char *source, char *who)
{
	const char *clientdesc;

	if (source && who)
	{
		if (stricmp(who, s_StatServ) == 0)
		{
			clientdesc = desc_StatServ;
		}
		else if (s_StatServ_alias && stricmp(who, s_StatServ_alias) == 0)
		{
			clientdesc = desc_StatServ_alias;
		}
		else
		{
			denora_cmd_401(source, who);
			return MOD_CONT;
		}
		denora_cmd_numeric(source, 311, "%s %s %s * :%s", who,
		                   ServiceUser, ServiceHost, clientdesc);
		/* denora_cmd_219(source, who); */
		denora_cmd_numeric(source, 317,
		                   "%s %ld %ld :seconds idle, signon time", who,
		                   time(NULL) - denora->start_time,
		                   denora->start_time);
		denora_cmd_318(source, who);
	}
	return MOD_CONT;
}

/*************************************************************************/

/**
 * NULL route events
 *
 * @param source is the nick of the person whom sent the command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int denora_event_null(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Add Internal Messages that are not wired to any ircd
 *
 * @return void - no returend value
 *
 */
void moduleAddMsgs(void)
{
	Message *m;
	int status;

	m = createMessage("STATS", m_stats);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for STATS [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("TIME", m_time);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for TIME [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("VERSION", m_version);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for VERSION [%d][%s]",
		     status, ModuleGetErrStr(status));
	}

}

/*************************************************************************/
