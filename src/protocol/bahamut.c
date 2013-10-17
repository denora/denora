/*************************************************************************/
/* Bahamut IRCD Protocol v1.4.x - v1.8.x                                 */
/* (c) 2004-2013 Denora Team                                             */
/* Contact us at info@denorastats.org                                    */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/*                       */
/*                                                                       */
/*************************************************************************/

#include "denora.h"
#include "bahamut.h"

IRCDVar myIrcd[] =
{
	{
		"BahamutIRCd 1.4.*/1.8.*", /* ircd name                 */
		"+io",                     /* StatServ mode             */
		IRCD_DISABLE,              /* Vhost                     */
		IRCD_ENABLE,               /* Supports SGlines          */
		IRCD_ENABLE,               /* sgline sql table          */
		IRCD_ENABLE,               /* Supports SQlines          */
		IRCD_ENABLE,               /* sqline sql table          */
		IRCD_ENABLE,               /* Supports SZlines          */
		IRCD_ENABLE,               /* Has exceptions +e         */
		IRCD_DISABLE,              /* vidents                   */
		IRCD_ENABLE,               /* NICKIP                    */
		IRCD_DISABLE,              /* VHOST ON NICK             */
		IRCD_DISABLE,              /* +f                        */
		IRCD_ENABLE,               /* +j                        */
		IRCD_DISABLE,              /* +L                        */
		IRCD_DISABLE,              /* +f Mode                   */
		CMODE_j,                   /* +j                        */
		IRCD_DISABLE,              /* +L Mode                   */
		NULL,                      /* CAPAB Chan Modes          */
		IRCD_DISABLE,              /* We support TOKENS         */
		IRCD_ENABLE,               /* TOKENS are CASE Sensitive */
		IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
		IRCD_ENABLE,               /* +I support                */
		IRCD_DISABLE,              /* SJOIN ban char            */
		IRCD_DISABLE,              /* SJOIN except char         */
		IRCD_DISABLE,              /* SJOIN invite char         */
		IRCD_DISABLE,              /* umode for vhost           */
		IRCD_DISABLE,              /* owner                     */
		IRCD_DISABLE,              /* protect                   */
		IRCD_DISABLE,              /* halfop                    */
		NULL,                      /* user modes        */
		NULL,                      /* channel modes             */
		IRCD_DISABLE,              /* flood                     */
		'j',                       /* flood other               */
		IRCD_DISABLE,              /* join throttle             */
		IRCD_DISABLE,              /* nick change flood         */
		IRCD_DISABLE,              /* vhost                     */
		IRCD_DISABLE,              /* vhost other               */
		IRCD_DISABLE,              /* Channel forwarding        */
		IRCD_DISABLE,              /* p10                       */
		IRCD_DISABLE,              /* ts6                       */
		IRCD_ENABLE,               /* numeric                   */
		IRCD_DISABLE,              /* gagged                    */
		IRCD_DISABLE,              /* spamfilter                */
		'b',                       /* ban char                  */
		'e',                       /* except char               */
		'I',                       /* invite char               */
		IRCD_DISABLE,              /* zip                       */
		IRCD_DISABLE,              /* ssl                       */
		IRCD_ENABLE,               /* uline                     */
		NULL,                      /* nickchar                  */
		IRCD_ENABLE,               /* svid                      */
		IRCD_DISABLE,              /* hidden oper               */
		IRCD_ENABLE,               /* extra warning             */
		IRCD_ENABLE,               /* Report sync state         */
		IRCD_DISABLE               /* Persistent channel mode   */
	}
	,
};

IRCDCAPAB myIrcdcap[] =
{
	{
		CAPAB_NOQUIT,              /* NOQUIT       */
		CAPAB_TSMODE,              /* TSMODE       */
		CAPAB_UNCONNECT,           /* UNCONNECT    */
		0,                         /* NICKIP       */
		0,                         /* SJOIN        */
		0,                         /* ZIP          */
		CAPAB_BURST,               /* BURST        */
		0,                         /* TS5          */
		0,                         /* TS3          */
		CAPAB_DKEY,                /* DKEY         */
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
		CAPAB_DOZIP,               /* DOZIP        */
		0,
		0,
		0
	}
};

/*************************************************************************/

void IRCDModeInit(void)
{
	ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_F, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_I, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_K, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_X, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_b, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_c, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_e, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_f, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_j, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_k, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_m, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_y, IRCD_ENABLE);
	ModuleUpdateSQLUserMode();
	CreateChanBanMode(CMODE_b, add_ban, del_ban);
	CreateChanBanMode(CMODE_e, add_exception, del_exception);
	CreateChanBanMode(CMODE_I, add_invite, del_invite);

	/* Channel Modes */
	CreateChanMode(CMODE_L, NULL, NULL);
	CreateChanMode(CMODE_M, NULL, NULL);
	CreateChanMode(CMODE_O, NULL, NULL);
	CreateChanMode(CMODE_R, NULL, NULL);
	CreateChanMode(CMODE_c, NULL, NULL);
	CreateChanMode(CMODE_i, NULL, NULL);
	CreateChanMode(CMODE_j, set_flood_alt, get_flood_alt);
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

/*************************************************************************/

void bahamut_cmd_stats(char *sender, const char *letter, char *server)
{
	send_cmd(sender, "STATS %s %s", letter, server);
}

/*************************************************************************/

void bahamut_cmd_mode(char *source, char *dest, char *buf)
{
	if (ircdcap->tsmode)
	{
		if (denora->capab & ircdcap->tsmode)
		{
			send_cmd(source, "MODE %s 0 %s", dest, buf);
		}
		else
		{
			send_cmd(source, "MODE %s %s", dest, buf);
		}
	}
	else
	{
		send_cmd(source, "MODE %s %s", dest, buf);
	}
}

/*************************************************************************/

/* EVENT: SJOIN */
int denora_event_sjoin(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_sjoin(source, ac, av);
	return MOD_CONT;
}

/*************************************************************************/

char *bahamut_nickip(char *host)
{
	struct in_addr addr;

	addr.s_addr = htonl(strtoul(host, NULL, 0));
	return sstrdup(inet_ntoa(addr));
}

/*************************************************************************/

/*
** NICK - new
**      source  = NULL
**	parv[0] = nickname
**      parv[1] = hopcount
**      parv[2] = timestamp
**      parv[3] = modes
**      parv[4] = username
**      parv[5] = hostname
**      parv[6] = server
**	parv[7] = servicestamp
**      parv[8] = IP
**	parv[9] = info
** NICK - change
**      source  = oldnick
**	parv[0] = new nickname
**      parv[1] = hopcount
*/
int denora_event_nick(char *source, int ac, char **av)
{
	User *user = NULL;
	char *ipchar = NULL;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
	{
		if (ac == 10)
		{
			ipchar = bahamut_nickip(av[8]);
			user = do_nick(source, av[0], av[4], av[5], av[6], av[9],
			               strtoul(av[2], NULL, 10), strtoul(av[7], NULL,
			                       0),
			               ipchar, NULL, NULL,
			               strtoul(av[1], NULL, 0), av[3], NULL);
		}
		if (ac == 9)
		{
			ipchar = bahamut_nickip(av[7]);
			user = do_nick(source, av[0], av[4], av[5], av[6], av[8],
			               strtoul(av[2], NULL, 10), 0,
			               ipchar, NULL, NULL, strtoul(av[1], NULL, 0),
			               av[3], NULL);
		}
		if (ipchar)
			free(ipchar);
	}
	else
	{
		do_nick(source, av[0], NULL, NULL, NULL, NULL,
		        strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
		        NULL);
	}
	return MOD_CONT;
}

/*************************************************************************/

/* EVENT : CAPAB */
int denora_event_capab(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	capab_parse(ac, av);
	return MOD_CONT;
}

/*************************************************************************/

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

/*************************************************************************/

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void)
{
	Message *m;

	m = createMessage("436",       denora_event_436);
	addCoreMessage(IRCD,m);
	m = createMessage("AWAY",      denora_event_away);
	addCoreMessage(IRCD,m);
	m = createMessage("INVITE",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("PONG",      denora_event_pong);
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
	m = createMessage("GLOBOPS",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("GNOTICE",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("GOPER",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("RAKILL",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SILENCE",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSKILL",   denora_event_kill);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSMODE",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSNICK",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSNOOP",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SQLINE",    denora_event_sqline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNSQLINE",  denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("CAPAB", 	   denora_event_capab);
	addCoreMessage(IRCD,m);
	m = createMessage("CS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("HS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("MS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("NS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("OS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("RS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SGLINE",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SJOIN",     denora_event_sjoin);
	addCoreMessage(IRCD,m);
	m = createMessage("SS",        denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SVINFO",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SZLINE",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("UNSGLINE",  denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("UNSZLINE",  denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("ERROR",     denora_event_error);
	addCoreMessage(IRCD,m);
	m = createMessage("LUSERSLOCK",denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("BURST",     denora_event_burst);
	addCoreMessage(IRCD,m);
	m = createMessage("ADMIN",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("RESYNCH",   denora_event_null);
	addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

/*************************************************************************/

/* PART */
void bahamut_cmd_part(char *nick, char *chan, char *buf)
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

/*************************************************************************/

/* JOIN - SJOIN */
void bahamut_cmd_join(char *user, char *channel, time_t chantime)
{
	if (started)
	{
		send_cmd(ServerName, "SJOIN %ld %s + :%s", (long int) chantime,
		         channel, user);
	}
	else
	{
		send_cmd(ServerName, "SJOIN %ld %s +nt :%s", (long int) chantime,
		         channel, user);
		if (AutoOp && AutoMode)
			denora_automode(channel);
	}
}

/*************************************************************************/

void bahamut_cmd_burst(void)
{
	send_cmd(NULL, "BURST");
}

/*************************************************************************/

void bahamut_cmd_ping(char *server)
{
	send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

/*************************************************************************/

/* SQUIT */
/*
 *        parv[0] = sender prefix
 *        parv[1] = server name
 *        parv[2] = comment
*/
void bahamut_cmd_squit(char *servname, char *message)
{
	send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/*************************************************************************/

/* PONG */
void bahamut_cmd_pong(char *servname, char *who)
{
	send_cmd(servname, "PONG %s", who);
}

/*************************************************************************/

/*
 * SVINFO
 *       parv[0] = sender prefix
 *       parv[1] = TS_CURRENT for the server
 *       parv[2] = TS_MIN for the server
 *       parv[3] = server is standalone or connected to non-TS only
 *       parv[4] = server's idea of UTC time
 */
void bahamut_cmd_svinfo(void)
{
	send_cmd(NULL, "SVINFO 3 1 0 :%ld", (long int) time(NULL));
}

/*************************************************************************/

/* PASS */
void bahamut_cmd_pass(char *pass)
{
	send_cmd(NULL, "PASS %s :TS", pass);
}

/*************************************************************************/

/* SERVER */
void bahamut_cmd_server(char *servname, int hop, char *descript)
{
	send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/*************************************************************************/

/* CAPAB */
void bahamut_cmd_capab(void)
{
	send_cmd(NULL,
	         "CAPAB SSJOIN NOQUIT BURST UNCONNECT NICKIP TSMODE TS3");
}

/*************************************************************************/

int denora_event_pong(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	server_store_pong(source, time(NULL));
	return MOD_CONT;
}

/*************************************************************************/

void bahamut_cmd_connect(void)
{
	me_server =
	    do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

	bahamut_cmd_pass(RemotePassword);
	bahamut_cmd_capab();
	bahamut_cmd_server(ServerName, 1, ServerDesc);
	bahamut_cmd_svinfo();
	bahamut_cmd_burst();
}

/*************************************************************************/

/* EVENT : SERVER */
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

	do_server(source, av[0], av[1], av[2], NULL);
	return MOD_CONT;
}

/*************************************************************************/

/* EVENT : PRIVMSG */
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

/*************************************************************************/

int denora_event_part(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_part(source, ac, av);
	return MOD_CONT;
}

/*************************************************************************/

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

/*************************************************************************/

int denora_event_topic(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	if (ac != 4)
	{
		return MOD_CONT;
	}
	do_topic(ac, av);
	return MOD_CONT;
}

/*************************************************************************/

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

/*************************************************************************/

int denora_event_quit(char *source, int ac, char **av)
{
	char *killer = NULL;
	char *msg = NULL;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 1)
		return MOD_CONT;

	if (bahamut_parse_lkill(av[0]) == 0)
	{
		do_quit(source, ac, av);
	}
	else
	{
		killer = bahamut_lkill_killer(av[0]);
		msg = bahamut_lkill_msg(av[0]);

		if (killer)
			m_kill(killer, source, msg);
		else
			m_kill(source, source, msg);

		if (msg)
			free(msg);
	}

	return MOD_CONT;
}

/*************************************************************************/

/* EVENT: MODE */
int denora_event_mode(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 2)
		return MOD_CONT;

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

/*************************************************************************/

/* EVENT: KILL */
int denora_event_kill(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
	{
		return MOD_CONT;
	}
	m_kill(source, av[0], av[1]);
	return MOD_CONT;
}

/*************************************************************************/

/* EVENT: KICK */
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

/*************************************************************************/

/* EVENT: JOIN */
int denora_event_join(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_join(source, ac, av);
	return MOD_CONT;
}

/*************************************************************************/

/* EVENT: MOTD */
int denora_event_motd(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	m_motd(source);
	return MOD_CONT;
}

/*************************************************************************/

/* NOTICE */
void bahamut_cmd_notice(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :%s", dest, buf);
}

/*************************************************************************/

void bahamut_cmd_privmsg(char *source, char *dest, char *buf)
{
	send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

/*************************************************************************/

void bahamut_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

/*************************************************************************/

void bahamut_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/*************************************************************************/

/* GLOBOPS */
void bahamut_cmd_global(char *source, char *buf)
{
	send_cmd(source, "GLOBOPS :%s", buf);
}

/*************************************************************************/

void bahamut_cmd_nick(char *nick, char *name, const char *modes)
{
	send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s 0 0 :%s", nick,
	         (long int) time(NULL), modes, ServiceUser, ServiceHost,
	         ServerName, name);
}

/*************************************************************************/

/* QUIT */
void bahamut_cmd_quit(char *source, char *buf)
{
	if (buf)
	{
		send_cmd(source, "QUIT :%s", buf);
	}
	else
	{
		send_cmd(source, "QUIT");
	}
}

int bahamut_parse_lkill(char *message)
{
	const char *localkillmsg = "Local kill by";

	/* is it a Local kill message? */
	if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
	        && (strstr(message, localkillmsg) != NULL))
		return 1;

	return 0;
}

char *bahamut_lkill_killer(char *message)
{
	char *buf, *killer = NULL;

	/* Let's get the killer nickname */
	killer = strchr(message, 'y');
	buf = sstrdup(killer);
	killer = strtok(buf, " ");
	killer = strtok(NULL, " ");

	return killer;
}

char *bahamut_lkill_msg(char *message)
{
	char *buf, *msg = NULL;

	/* Let's get the kill message */
	msg = strchr(message, 'y');
	buf = sstrdup(msg);
	msg = strtok(buf, " ");
	msg = strtok(NULL, " ");
	msg = strtok(NULL, " ");

	return msg;
}

/*************************************************************************/

int denora_event_away(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	m_away(source, (ac ? av[0] : NULL));
	return MOD_CONT;
}

/*************************************************************************/

int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
		return MOD_CONT;
	bahamut_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
	return MOD_CONT;
}

/*************************************************************************/

void bahamut_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                          char *modes)
{
	send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s 0 0 :%s", nick,
	         (long int) time(NULL), modes, user, host, ServerName, real);
}

/*************************************************************************/

int denora_event_error(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac >= 1)
	{
		alog(LOG_ERROR, "ERROR: %s", av[0]);
	}
	return MOD_CONT;
}

/*************************************************************************/

int denora_event_sqline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	sql_do_sqline(av[0], av[1]);
	return MOD_CONT;
}

/*************************************************************************/

void bahamut_cmd_eob(void)
{
	send_cmd(NULL, "BURST 0");
}

/*************************************************************************/

int denora_event_burst(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac)
	{
		update_sync_state(source, SYNC_COMPLETE);
	}
	return MOD_CONT;
}

/*************************************************************************/

void bahamut_cmd_version(char *server)
{
	send_cmd(s_StatServ, "VERSION %s", server);
}

/*************************************************************************/

void bahamut_cmd_ctcp(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

/*************************************************************************/

void bahamut_cmd_motd(char *sender, char *server)
{
	send_cmd(sender, "MOTD %s", server);
}

/*************************************************************************/

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

/*************************************************************************/

void moduleAddIRCDCmds()
{
	pmodule_cmd_nick(bahamut_cmd_nick);
	pmodule_cmd_mode(bahamut_cmd_mode);
	pmodule_cmd_bot_nick(bahamut_cmd_bot_nick);
	pmodule_cmd_notice(bahamut_cmd_notice);
	pmodule_cmd_privmsg(bahamut_cmd_privmsg);
	pmodule_cmd_serv_notice(bahamut_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(bahamut_cmd_serv_privmsg);
	pmodule_cmd_quit(bahamut_cmd_quit);
	pmodule_cmd_pong(bahamut_cmd_pong);
	pmodule_cmd_join(bahamut_cmd_join);
	pmodule_cmd_part(bahamut_cmd_part);
	pmodule_cmd_global(bahamut_cmd_global);
	pmodule_cmd_squit(bahamut_cmd_squit);
	pmodule_cmd_connect(bahamut_cmd_connect);
	pmodule_cmd_eob(bahamut_cmd_eob);
	pmodule_cmd_ctcp(bahamut_cmd_ctcp);
	pmodule_cmd_version(bahamut_cmd_version);
	pmodule_cmd_stats(bahamut_cmd_stats);
	pmodule_cmd_motd(bahamut_cmd_motd);
	pmodule_cmd_ping(bahamut_cmd_ping);
}

/*************************************************************************/

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

	pmodule_ircd_version("BahamutIRCd 1.4.*/1.8.*");
	pmodule_ircd_cap(myIrcdcap);
	pmodule_ircd_var(myIrcd);
	pmodule_ircd_useTSMode(0);

	IRCDModeInit();
	pmodule_oper_umode(UMODE_o);
	pmodule_irc_var(IRC_BAHAMUT);
	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();
	return MOD_CONT;
}
