/* Next Generation IRC Server IRCD functions
 *
 * (c) 2004-2012 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * last tested with ngircd-0.9.2
 *
 */

#include "denora.h"
#include "ngircd.h"

IRCDVar myIrcd[] =
{
	{
		"NGIRCd 0.9.*+",           /* ircd name                 */
		"+oi",                     /* StatServ mode             */
		IRCD_DISABLE,              /* Vhost                     */
		IRCD_DISABLE,              /* Supports SGlines          */
		IRCD_DISABLE,              /* sgline sql table          */
		IRCD_DISABLE,              /* Supports SQlines          */
		IRCD_DISABLE,              /* sqline sql table          */
		IRCD_DISABLE,              /* Supports SZlines          */
		IRCD_DISABLE,              /* Has exceptions +e         */
		IRCD_DISABLE,              /* vidents                   */
		IRCD_DISABLE,              /* NICKIP                    */
		IRCD_DISABLE,              /* VHOST ON NICK             */
		IRCD_DISABLE,              /* +f                        */
		IRCD_DISABLE,              /* +j                           */
		IRCD_DISABLE,              /* +L                        */
		IRCD_DISABLE,              /* +f Mode                   */
		IRCD_DISABLE,              /* +j                           */
		IRCD_DISABLE,              /* +L Mode                   */
		NULL,                      /* CAPAB Chan Modes          */
		IRCD_DISABLE,              /* We support Unreal TOKENS  */
		IRCD_DISABLE,              /* TOKENS are CASE Sensitive */
		IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
		IRCD_DISABLE,              /* +I support                */
		IRCD_DISABLE,              /* SJOIN ban char            */
		IRCD_DISABLE,              /* SJOIN except char         */
		IRCD_DISABLE,              /* SJOIN invite char         */
		IRCD_DISABLE,              /* umode for vhost           */
		IRCD_DISABLE,              /* owner                     */
		IRCD_DISABLE,              /* protect                   */
		IRCD_DISABLE,              /* halfop                    */
		NULL,                      /* user modes                */
		NULL,                      /* channel modes             */
		IRCD_DISABLE,              /* flood                     */
		IRCD_DISABLE,              /* flood other               */
		IRCD_DISABLE,              /* join throttle             */
		IRCD_DISABLE,              /* nick change flood         */
		IRCD_DISABLE,              /* vhost                     */
		IRCD_DISABLE,              /* vhost other               */
		IRCD_DISABLE,              /* channek linking           */
		IRCD_DISABLE,              /* p10                       */
		IRCD_DISABLE,              /* ts6                       */
		IRCD_ENABLE,               /* numeric ie.. 350 etc      */
		IRCD_DISABLE,              /* channel mode gagged       */
		IRCD_DISABLE,              /* spamfilter                */
		'b',                       /* ban char                  */
		IRCD_DISABLE,              /* except char               */
		IRCD_DISABLE,              /* invite char               */
		IRCD_DISABLE,              /* zip                       */
		IRCD_DISABLE,              /* ssl                       */
		IRCD_ENABLE,               /* uline                     */
		NULL,                      /* nickchar                  */
		IRCD_DISABLE,              /* svid                      */
		IRCD_DISABLE,              /* hidden oper               */
		IRCD_ENABLE,               /* extra warning             */
		IRCD_DISABLE,              /* Report sync state         */
		'P'                        /* Persistent channel mode   */
	}
	,
};

IRCDCAPAB myIrcdcap[] =
{
	{
		0,                         /* NOQUIT       */
		0,                         /* TSMODE       */
		0,                         /* UNCONNECT    */
		0,                         /* NICKIP       */
		0,                         /* SJOIN        */
		0,                         /* ZIP          */
		0,                         /* BURST        */
		0,                         /* TS5          */
		0,                         /* TS3          */
		0,                         /* DKEY         */
		0,                         /* PT4          */
		0,                         /* SCS          */
		0,                         /* QS           */
		0,                         /* UID          */
		0,                         /* KNOCK        */
		0,                         /* CLIENT       */
		0,                         /* IPV6         */
		0,                         /* SSJ5         */
		0,                         /* SN2          */
		0,                         /* TOKEN        */
		0,                         /* VHOST        */
		0,                         /* SSJ3         */
		0,                         /* NICK2        */
		0,                         /* UMODE2       */
		0,                         /* VL           */
		0,                         /* TLKEXT       */
		0,                         /* DODKEY       */
		0,                         /* DOZIP        */
		0,                         /* CHANMODE             */
		0, 0
	}
};


/*************************************************************************/

void IRCDModeInit(void)
{
	ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
	ModuleUpdateSQLUserMode();
	CreateChanBanMode(CMODE_b, add_ban, del_ban);

	/* Channel Modes */
	CreateChanMode(CMODE_P, NULL, NULL);
	CreateChanMode(CMODE_i, NULL, NULL);
	CreateChanMode(CMODE_k, set_key, get_key);
	CreateChanMode(CMODE_l, set_limit, get_limit);
	CreateChanMode(CMODE_m, NULL, NULL);
	CreateChanMode(CMODE_n, NULL, NULL);
	CreateChanMode(CMODE_p, NULL, NULL);
	CreateChanMode(CMODE_r, NULL, NULL);
	CreateChanMode(CMODE_s, NULL, NULL);
	CreateChanMode(CMODE_t, NULL, NULL);

	ModuleSetChanUMode('+', 'v', STATUS_VOICE);
	ModuleSetChanUMode('@', 'o', STATUS_OP);

	ModuleUpdateSQLChanMode();
}

int denora_event_pong(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	server_store_pong(source, time(NULL));
	return MOD_CONT;
}

void ngircd_cmd_stats(char *sender, const char *letter, char *server)
{
	send_cmd(sender, "STATS %s %s", letter, server);
}

/*
NICK <nickname> <hopcount> <username> <host> <servertoken> <umode> <realname>
*/
int denora_event_nick(char *source, int ac, char **av)
{
	char *ipchar = NULL;
	Server *s;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 1)
	{
		s = findserver_uid(servlist, av[4]);
		ipchar = host_resolve(av[3]);
		do_nick("", av[0], av[2], av[3], s ? s->name : source, av[6],
		        UplinkSynced ? time(NULL) : 0, 0, ipchar, NULL, NULL,
		        strtoul(av[1], NULL, 10), av[5], NULL);
		DenoraFree(ipchar);
	}
	else
	{
		do_nick(source, av[0], NULL, NULL, NULL, NULL,
		        0, 0, NULL, NULL, NULL, 0, NULL, NULL);
	}
	return MOD_CONT;
}

void ngircd_cmd_version(char *server)
{
	send_cmd(s_StatServ, "VERSION %s", server);
}

void ngircd_cmd_motd(char *sender, char *server)
{
	send_cmd(sender, "MOTD %s", server);
}

int denora_event_notice(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
	{
		return MOD_CONT;
	}
	m_notice(source, av[0], av[1]);
	return MOD_CONT;
}

int denora_event_436(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
		return MOD_CONT;

	m_nickcoll(av[0]);
	return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void)
{
	Message *m;

	m = createMessage("401",       denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("436",       denora_event_436);
	addCoreMessage(IRCD,m);
	m = createMessage("402",       denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("AWAY",      denora_event_away);
	addCoreMessage(IRCD,m);
	m = createMessage("INVITE",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("JOIN",      denora_event_join);
	addCoreMessage(IRCD,m);
	m = createMessage("KICK",      denora_event_kick);
	addCoreMessage(IRCD,m);
	m = createMessage("KILL",      denora_event_kill);
	addCoreMessage(IRCD,m);
	m = createMessage("MODE",      denora_event_mode);
	addCoreMessage(IRCD,m);
	m = createMessage("MOTD",      denora_event_motd);
	addCoreMessage(IRCD,m);
	m = createMessage("NICK",      denora_event_nick);
	addCoreMessage(IRCD,m);
	m = createMessage("NOTICE",    denora_event_notice);
	addCoreMessage(IRCD,m);
	m = createMessage("PART",      denora_event_part);
	addCoreMessage(IRCD,m);
	m = createMessage("PASS",      denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("PING",      denora_event_ping);
	addCoreMessage(IRCD,m);
	m = createMessage("PRIVMSG",   denora_event_privmsg);
	addCoreMessage(IRCD,m);
	m = createMessage("QUIT",      denora_event_quit);
	addCoreMessage(IRCD,m);
	m = createMessage("SERVER",    denora_event_server);
	addCoreMessage(IRCD,m);
	m = createMessage("SQUIT",     denora_event_squit);
	addCoreMessage(IRCD,m);
	m = createMessage("TOPIC",     denora_event_topic);
	addCoreMessage(IRCD,m);
	m = createMessage("USER",      denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("WALLOPS",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("WHOIS",     denora_event_whois);
	addCoreMessage(IRCD,m);
	m = createMessage("AKILL",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("REHASH",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("ADMIN",      denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("CREDITS",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("ERROR",      denora_event_error);
	addCoreMessage(IRCD,m);
	m = createMessage("PONG",       denora_event_pong);
	addCoreMessage(IRCD,m);
	m = createMessage("NJOIN",      denora_event_sjoin);
	addCoreMessage(IRCD,m);
	m = createMessage("CHANINFO",   denora_event_chaninfo);
	addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

/* :ngirc.nomadirc.net CHANINFO #TheName +Ptn :a great topic */


int denora_event_chaninfo(char *source, int ac, char **av)
{
	char *v[32];
	char buf[BUFSIZE];
	Channel *c;
	char *chan;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	c = chan_create(av[0], time(NULL));

	if (!c)
	{
		return MOD_CONT;
	}

	chan = rdb_escape(av[0]);
	db_getchancreate(chan);
	DenoraFree(chan);

	ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
	v[0] = av[0];               /* channel */
	v[1] = sstrdup(source);     /* setter */
	v[2] = sstrdup(buf);
	v[3] = sstrdup(av[2]);
	do_topic(4, v);
	DenoraFree(v[1]);
	DenoraFree(v[2]);
	DenoraFree(v[3]);
	v[1] = sstrdup(av[1]);
	do_cmode(source, 2, v);
	DenoraFree(v[1]);
	return MOD_CONT;
}

int denora_event_sjoin(char *source, int ac, char **av)
{
	char *v[32];
	char *temp, *temp2 = NULL;
	char buf[BUFSIZE];
	int num, i;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	v[0] = (char *) "0";
	v[1] = sstrdup(av[0]);

	num = myNumToken(av[1], ',');

	if (num)
	{
		for (i = 0; i < num; i++)
		{
			temp = myStrGetToken(av[1], ',', i);
			if (temp2)
			{
				ircsnprintf(buf, BUFSIZE, "%s %s", temp2, temp);
				DenoraFree(temp2);
				temp2 = sstrdup(buf);
			}
			else
			{
				ircsnprintf(buf, BUFSIZE, "%s", temp);
				temp2 = sstrdup(buf);
			}
			DenoraFree(temp);
		}
		if (temp2)
		{
			v[2] = sstrdup(temp2);
		}
		else
		{
			v[2] = sstrdup("");
		}
	}
	else
	{
		v[2] = sstrdup(av[1]);
	}
	do_sjoin(source, 3, v);
	DenoraFree(v[0]);
	DenoraFree(v[1]);
	DenoraFree(v[2]);
	return MOD_CONT;
}

void ngircd_cmd_join(char *user, char *channel, time_t chantime)
{
	USE_VAR(chantime);
	send_cmd(user, "JOIN %s", channel);
}

/* PASS */
void ngircd_cmd_pass(char *pass)
{
	send_cmd(NULL, "PASS %s 0210-IRC+ %s|%s:CL", pass, PACKAGE_NAME,
	         PACKAGE_VERSION);
}

/* SERVER name hop descript */
void ngircd_cmd_server(char *servname, int hop, char *descript)
{
	send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/* PONG */
void ngircd_cmd_pong(char *servname, char *who)
{
	send_cmd(servname, "PONG %s", who);
}

void ngircd_cmd_connect(void)
{
	me_server =
	    do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

	ngircd_cmd_pass(RemotePassword);
	ngircd_cmd_server(ServerName, 1, ServerDesc);
}

/* PART */
void ngircd_cmd_part(char *nick, char *chan, char *buf)
{
	if (buf)
	{
		send_cmd(nick, "PART %s :%s", chan, buf);
	}
	else
	{
		send_cmd(nick, "PART %s", chan);
	}
}

/* GLOBOPS */
void ngircd_cmd_global(char *source, char *buf)
{
	send_cmd(source, "GLOBOPS :%s", buf);
}

/* SQUIT */
void ngircd_cmd_squit(char *servname, char *message)
{
	send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* Events */

int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
	{
		return MOD_CONT;
	}
	ngircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);

	/* Little hack since ngircd appears not to send EOB message */
	if (!UplinkSynced)
		update_sync_state(source, SYNC_COMPLETE);
	return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	m_away(source, (ac ? av[0] : NULL));
	return MOD_CONT;
}

int denora_event_topic(char *source, int ac, char **av)
{
	char *v[32];
	char buf[BUFSIZE];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
	v[0] = av[0];               /* channel */
	v[1] = source;              /* setter */
	v[2] = buf;                 /* topic content */
	v[3] = av[1];
	do_topic(4, v);
	return MOD_CONT;
}

int denora_event_squit(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
		return MOD_CONT;
	do_squit(av[0]);
	return MOD_CONT;
}

int denora_event_quit(char *source, int ac, char **av)
{
	if (ac != 1)
		return MOD_CONT;
	do_quit(source, ac, av);
	return MOD_CONT;
}

int denora_event_mode(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (*av[0] == '#' || *av[0] == '&')
	{
		do_cmode(source, ac, av);
	}
	else
	{
		do_umode(source, ac, av);
	}
	return MOD_CONT;
}


int denora_event_kill(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
		return MOD_CONT;

	m_kill(source, av[0], av[1]);
	return MOD_CONT;
}

int denora_event_kick(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 3)
		return MOD_CONT;
	do_kick(source, ac, av);
	return MOD_CONT;
}

int denora_event_join(char *source, int ac, char **av)
{
	char *flags;
	char *x[32];
	char buf[BUFSIZE];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	flags = strchr(av[0], 0x7);
	if (flags)
	{
		*flags = '\0';
		flags++;
	}
	do_join(source, ac, av);
	if (flags)
	{
		x[0] = av[0];
		ircsnprintf(buf, BUFSIZE, "+%s", flags);
		x[1] = sstrdup(buf);
		x[2] = sstrdup(source);
		do_cmode(source, 3, x);
		DenoraFree(x[1]);
		DenoraFree(x[2]);
	}
	return MOD_CONT;
}

int denora_event_motd(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	m_motd(source);
	return MOD_CONT;
}

/* SERVER <servername> <hopcount> <token> <info> für die server */
int denora_event_server(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (!denora->uplink)
	{
		denora->uplink = sstrdup(av[0]);
	}
	if (!stricmp(av[1], "1"))
	{
		*source = '\0';
		do_server(source, av[0], (char *) "0", av[2], av[1]);
	}
	else
	{
		do_server(source, av[0], av[1], av[3], av[2]);
	}
	return MOD_CONT;
}

int denora_event_privmsg(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
		return MOD_CONT;
	m_privmsg(source, av[0], av[1]);
	return MOD_CONT;
}

int denora_event_part(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_part(source, ac, av);
	return MOD_CONT;
}

int denora_event_whois(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (source && ac >= 1)
	{
		m_whois(source, av[0]);
	}
	return MOD_CONT;
}

void ngircd_cmd_notice(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void ngircd_cmd_privmsg(char *source, char *dest, char *buf)
{
	send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void ngircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void ngircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/*
[Aug 11 23:23:32.699696 2005] av[0] = Trystan
[Aug 11 23:23:32.699954 2005] av[1] = 1
[Aug 11 23:23:32.700115 2005] av[2] = ~tslee
[Aug 11 23:23:32.700271 2005] av[3] = c-24-2-101-227.hsd1.ut.comcast.net
[Aug 11 23:23:32.700432 2005] av[4] = 1
[Aug 11 23:23:32.700588 2005] av[5] = +io
[Aug 11 23:23:32.700745 2005] av[6] = Dreams are answers to questions not yet asked
*/

void ngircd_cmd_nick(char *nick, char *name, const char *mode)
{
	send_cmd(ServerName, "NICK %s 1 %s %s 1 %s :%s", nick,
	         ServiceUser, ServiceHost, mode, name);
}

/* QUIT */
void ngircd_cmd_quit(char *source, char *buf)
{
	send_cmd(source, "QUIT :%s", (buf ? buf : "\"\""));
}

void ngircd_cmd_mode(char *source, char *dest, char *buf)
{
	send_cmd(source, "MODE %s %s", dest, buf);
}

void ngircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                         char *modes)
{
	send_cmd(ServerName, "NICK %s 1 %s %s 1 %s :%s", nick,
	         user, host, modes, real);
}

void ngircd_cmd_ping(char *server)
{
	send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

int denora_event_error(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (av[0])
	{
		alog(LOG_ERROR, "ERROR: %s", av[0]);
	}
	return MOD_CONT;
}

void ngircd_cmd_ctcp(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void moduleAddIRCDCmds(void)
{
	pmodule_cmd_nick(ngircd_cmd_nick);
	pmodule_cmd_mode(ngircd_cmd_mode);
	pmodule_cmd_bot_nick(ngircd_cmd_bot_nick);
	pmodule_cmd_notice(ngircd_cmd_notice);
	pmodule_cmd_privmsg(ngircd_cmd_privmsg);
	pmodule_cmd_serv_notice(ngircd_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(ngircd_cmd_serv_privmsg);
	pmodule_cmd_quit(ngircd_cmd_quit);
	pmodule_cmd_pong(ngircd_cmd_pong);
	pmodule_cmd_join(ngircd_cmd_join);
	pmodule_cmd_part(ngircd_cmd_part);
	pmodule_cmd_global(ngircd_cmd_global);
	pmodule_cmd_squit(ngircd_cmd_squit);
	pmodule_cmd_connect(ngircd_cmd_connect);
	pmodule_cmd_ctcp(ngircd_cmd_ctcp);
	pmodule_cmd_version(ngircd_cmd_version);
	pmodule_cmd_stats(ngircd_cmd_stats);
	pmodule_cmd_motd(ngircd_cmd_motd);
	pmodule_cmd_ping(ngircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
	if (denora->protocoldebug)
	{
		protocol_debug(NULL, argc, argv);
	}
	/* Only 1 protocol module may be loaded */
	if (protocolModuleLoaded())
	{
		alog(LOG_NORMAL, langstr(ALOG_MOD_BE_ONLY_ONE));
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("");
	moduleSetType(PROTOCOL);
	pmodule_ircd_version("Next Generation IRC Server");
	pmodule_ircd_cap(myIrcdcap);
	pmodule_ircd_var(myIrcd);
	pmodule_ircd_useTSMode(0);
	IRCDModeInit();
	pmodule_oper_umode(UMODE_o);
	pmodule_irc_var(IRC_NGIRCD);
	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();
	UplinkSynced = 1;
	return MOD_CONT;
}
