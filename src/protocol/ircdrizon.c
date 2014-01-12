/* ircd-rizon 3 IRCD functions
 *
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
 * last tested against hybrid-7.2.3+plexus-3.0.1(20081028_0-520)
 *
 */

#include "denora.h"
#include "ircdrizon.h"

IRCDVar myIrcd[] =
{
	{
		"ircd-rizon-3.0.1+",           /* ircd name                 */
		"+oai",                    /* StatServ mode             */
		IRCD_ENABLE,               /* Vhost                     */
		IRCD_ENABLE,               /* Supports SGlines          */
		IRCD_ENABLE,               /* sgline sql table          */
		IRCD_ENABLE,               /* Supports SQlines          */
		IRCD_ENABLE,               /* sqline sql table          */
		IRCD_DISABLE,              /* Supports SZlines          */
		IRCD_ENABLE,               /* Has exceptions +e         */
		IRCD_DISABLE,              /* vidents                   */
		IRCD_DISABLE,              /* NICKIP                    */
		IRCD_ENABLE,               /* VHOST ON NICK             */
		IRCD_DISABLE,              /* +f                        */
		IRCD_DISABLE,              /* +j                           */
		IRCD_DISABLE,              /* +L                        */
		IRCD_DISABLE,              /* +f Mode                   */
		IRCD_DISABLE,              /* +j                           */
		IRCD_DISABLE,              /* +L Mode                   */
		NULL,                      /* CAPAB Chan Modes          */
		IRCD_DISABLE,              /* We support TOKENS         */
		IRCD_ENABLE,               /* TOKENS are CASE Sensitive */
		IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
		IRCD_ENABLE,               /* +I support                */
		IRCD_DISABLE,              /* SJOIN ban char            */
		IRCD_DISABLE,              /* SJOIN except char         */
		IRCD_DISABLE,              /* SJOIN invite char         */
		UMODE_x,                   /* umode for vhost           */
		IRCD_ENABLE,               /* owner                     */
		IRCD_ENABLE,               /* protect                   */
		IRCD_ENABLE,               /* halfop                    */
		NULL,                      /* User modes                */
		NULL,                      /* Channel Modes             */
		IRCD_DISABLE,              /* flood                     */
		IRCD_DISABLE,              /* flood other               */
		IRCD_DISABLE,              /* join throttle             */
		IRCD_DISABLE,              /* nick change flood         */
		'x',                       /* vhost                     */
		IRCD_DISABLE,              /* vhost other               */
		IRCD_DISABLE,              /* channek linking           */
		IRCD_DISABLE,              /* p10                       */
		IRCD_ENABLE,               /* ts6                       */
		IRCD_ENABLE,               /* numeric ie.. 350 etc      */
		IRCD_DISABLE,              /* channel mode gagged       */
		IRCD_DISABLE,              /* spamfilter                */
		'b',                       /* ban char                  */
		'e',                       /* except char               */
		'I',                       /* invite char               */
		IRCD_DISABLE,              /* zip                       */
		IRCD_ENABLE,               /* ssl                       */
		IRCD_ENABLE,               /* uline                     */
		NULL,                      /* nickchar                  */
		IRCD_ENABLE,               /* svid                      */
		IRCD_DISABLE,              /* hidden oper               */
		IRCD_DISABLE,              /* extra warning             */
		IRCD_ENABLE,               /* Report sync state         */
		CMODE_z                    /* Persistent channel mode   */
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
		CAPAB_ZIP,                 /* ZIP          */
		0,                         /* BURST        */
		CAPAB_TS5,                 /* TS5          */
		0,                         /* TS3          */
		0,                         /* DKEY         */
		0,                         /* PT4          */
		0,                         /* SCS          */
		CAPAB_QS,                  /* QS           */
		CAPAB_UID,                 /* UID          */
		CAPAB_KNOCK,               /* KNOCK        */
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
		0, 0, 0
	}
};

/*************************************************************************/

void IRCDModeInit(void)
{
	ModuleSetUserMode(UMODE_C, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_N, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_W, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_X, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_b, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_c, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_f, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_k, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_l, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_q, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_u, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_y, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_z, IRCD_ENABLE);
	ModuleUpdateSQLUserMode();
	CreateChanBanMode(CMODE_b, add_ban, del_ban);
	CreateChanBanMode(CMODE_e, add_exception, del_exception);
	CreateChanBanMode(CMODE_I, add_invite, del_invite);

	/* Channel Modes */
	CreateChanMode(CMODE_B, NULL, NULL);
	CreateChanMode(CMODE_M, NULL, NULL);
	CreateChanMode(CMODE_N, NULL, NULL);
	CreateChanMode(CMODE_O, NULL, NULL);
	CreateChanMode(CMODE_R, NULL, NULL);
	CreateChanMode(CMODE_S, NULL, NULL);
	CreateChanMode(CMODE_Z, NULL, NULL);
	CreateChanMode(CMODE_c, NULL, NULL);
	CreateChanMode(CMODE_i, NULL, NULL);
	CreateChanMode(CMODE_k, set_key, get_key);
	CreateChanMode(CMODE_l, set_limit, get_limit);
	CreateChanMode(CMODE_m, NULL, NULL);
	CreateChanMode(CMODE_n, NULL, NULL);
	CreateChanMode(CMODE_p, NULL, NULL);
	CreateChanMode(CMODE_s, NULL, NULL);
	CreateChanMode(CMODE_t, NULL, NULL);
	CreateChanMode(CMODE_z, NULL, NULL);

	ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
	ModuleSetChanUMode('+', 'v', STATUS_VOICE);
	ModuleSetChanUMode('@', 'o', STATUS_OP);
	ModuleSetChanUMode('&', 'a', STATUS_PROTECTED);
	ModuleSetChanUMode('~', 'q', STATUS_OWNER);

	ModuleUpdateSQLChanMode();

}


void rizon_cmd_notice(char *source, char *dest, char *buf)
{
	Uid *ud;
	User *u;

	ud = find_uid(source);
	u = finduser(dest);
	send_cmd((UseTS6 ? (ud ? ud->uid : source) : source),
		         "NOTICE %s :%s", (UseTS6 ? (u ? u->uid : dest) : dest), buf);
}

void rizon_cmd_privmsg(char *source, char *dest, char *buf)
{
	Uid *ud, *ud2;

	ud = find_uid(source);
	ud2 = find_uid(dest);

	send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "PRIVMSG %s :%s",
		         (UseTS6 ? (ud2 ? ud2->uid : dest) : dest), buf);
}

void rizon_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $$%s :%s", dest, msg);
}

void rizon_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $$%s :%s", dest, msg);
}

void rizon_cmd_global(char *source, char *buf)
{
	Uid *u;

	if (source)
	{
		u = find_uid(source);
		if (u)
		{
			send_cmd((UseTS6 ? u->uid : source), "OPERWALL :%s", buf);
		}
		else
		{
			send_cmd((UseTS6 ? TS6SID : ServerName), "OPERWALL :%s", buf);
		}
	}
	else
	{
		send_cmd((UseTS6 ? TS6SID : ServerName), "OPERWALL :%s", buf);
	}
}

int denora_event_sjoin(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (!strcmp(av[2], "0"))
		av[2] = sstrdup("+");
	do_sjoin(source, ac, av);
	return MOD_CONT;
}

/*
 do_nick(const char *source, char *nick, char *username, char *host,
              char *server, char *realname, time_t ts, uint32 svid,
              uint32 ip, char *vhost, char *uid, int hopcount, char *modes, char *account)
  NICK Trystan 1 1148214497 +aiow tslee is.my.vhost plexus3.nomadirc.net 0 c-67-186-230-12.hsd1.ut.comcast.net :Dreams are answers to questions not yet asked
       0       1 2           3    4     5           6                    7 8                                    9
  666 UID asdasd 1 1234817435 +ix Nesstest 566C206.B53EDE66.1DF57482.IP 66.63.160.250 666AAAAAD 0 66.63.160.250 :JasonX
          0      1 2           3  4        5                            6             7         8 9              10

[Feb 19 04:24:22.531142 2009] debug: Received: :57CAAEV6D NICK Miu :1235017456
[Feb 19 04:24:22.536789 2009] Source 57CAAEV6D
[Feb 19 04:24:22.537026 2009] av[0] = Miu
[Feb 19 04:24:22.537233 2009] av[1] = 1235017456
[Feb 19 04:24:22.537422 2009] Unknown NICK formatted message please report the following
*/
int denora_event_nick(char *source, int ac, char **av)
{
	Server *s;
	User *user;
	char *temp;
	char *ipchar = NULL;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	temp = sstrdup(source);

	if (UseTS6 && ac == 11)
	{
		s = server_find(source);
		/* Source is always the server */
		*source = '\0';
		user = do_nick(source, av[0], av[4], av[9], s->name, av[10],
		               strtoul(av[2], NULL, 10), 0, av[6], av[5],
		               av[7], strtoul(av[1], NULL, 10), av[3], NULL);
	}
	else if (ac == 10)
	{
		ipchar = host_resolve(av[8]);
		user = do_nick(source, av[0], av[4], av[8], av[6], av[9],
		               strtoul(av[2], NULL, 10),
		               strtoul(av[7], NULL, 0), ipchar, av[5], NULL,
		               strtoul(av[1], NULL, 0), av[3], NULL);
		free(ipchar);
	}
	else if (ac == 2)
	{
		do_nick(source, av[0], NULL, NULL, NULL, NULL,
		        strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0,
		        NULL, NULL);
	}
	else
	{
		alog(LOG_DEBUG,
		     "Unknown NICK formatted message please report the following");
		protocol_debug(temp, ac, av);
	}
	free(temp);
	return MOD_CONT;
}

/* :42XAAAAAB TOPIC #testchan :test test test */
int denora_event_topic(char *source, int ac, char **av)
{
	char *newav[5];
	User *u;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac == 2)
	{
		u = user_find(source);
		if (u)
		{
			newav[0] = sstrdup(av[0]);
			newav[1] = sstrdup(u->nick);
			newav[2] = itostr(time(NULL));
			newav[3] = sstrdup(av[1]);
			do_topic(4, newav);
			free(newav[0]);
			free(newav[1]);
			free(newav[3]);
		}

	}
	else
	{
		do_topic(ac, av);
	}
	return MOD_CONT;
}

/* irc.dev.cccp-project.net TBURST 1218474093 #oper 1212613221 Celestin :Main L2Thorn Channel */
int denora_event_tburst(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);
	if (ac != 5)
		return MOD_CONT;

	av[0] = av[1];
	av[1] = av[3];
	av[3] = av[4];
	do_topic(4, av);
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

	if (UseTS6)
	{
		if (!BadPtr(Numeric))
		{
			TS6SID = sstrdup(Numeric);
			UseTSMODE = 1;  /* TMODE */
		}
		else
		{
			alog(LOG_NORMAL, "TS6 requires the use of Numeric - exiting");
			exit(1);
		}
	}

	m = createMessage("436",       denora_event_436);
	addCoreMessage(IRCD,m);
	m = createMessage("439",       denora_event_null);
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
	m = createMessage("TMODE",     denora_event_tmode);
	addCoreMessage(IRCD,m);
	m = createMessage("MOTD",      denora_event_motd);
	addCoreMessage(IRCD,m);
	m = createMessage("NICK",      denora_event_nick);
	addCoreMessage(IRCD,m);
	m = createMessage("PONG",      denora_event_pong);
	addCoreMessage(IRCD,m);
	m = createMessage("NOTICE",    denora_event_notice);
	addCoreMessage(IRCD,m);
	m = createMessage("PART",      denora_event_part);
	addCoreMessage(IRCD,m);
	m = createMessage("PASS",      denora_event_pass);
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
	m = createMessage("TBURST",    denora_event_tburst);
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
	m = createMessage("SVSMODE",   denora_event_svsmode);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSNOOP",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SQLINE",    denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("UNSQLINE",  denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("CAPAB",     denora_event_capab);
	addCoreMessage(IRCD,m);
	m = createMessage("SJOIN",     denora_event_sjoin);
	addCoreMessage(IRCD,m);
	m = createMessage("SVINFO",    denora_event_svinfo);
	addCoreMessage(IRCD,m);
	m = createMessage("EOB",       denora_event_eob);
	addCoreMessage(IRCD,m);
	m = createMessage("ADMIN",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("ERROR",     denora_event_error);
	addCoreMessage(IRCD,m);
	m = createMessage("SETHOST",   denora_event_sethost);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSID",     denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("KLINE",     denora_event_kline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNKLINE",   denora_event_unkline);
	addCoreMessage(IRCD,m);
	m = createMessage("XLINE",     denora_event_xline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNXLINE",   denora_event_unxline);
	addCoreMessage(IRCD,m);
	m = createMessage("SVSTOPIC",  denora_event_topic);
	addCoreMessage(IRCD,m);
	m = createMessage("ENCAP",     denora_event_encap);
	addCoreMessage(IRCD,m);
	m = createMessage("BMASK",     denora_event_bmask);
	addCoreMessage(IRCD,m);
	m = createMessage("SID",       denora_event_sid);
	addCoreMessage(IRCD,m);
	m = createMessage("KLINE",     denora_event_kline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNKLINE",   denora_event_unkline);
	addCoreMessage(IRCD,m);
	m = createMessage("RKLINE",    denora_event_kline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNRKLINE",  denora_event_unkline);
	addCoreMessage(IRCD,m);
	m = createMessage("XLINE",     denora_event_xline);
	addCoreMessage(IRCD,m);
	m = createMessage("RXLINE",    denora_event_xline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNXLINE",   denora_event_unxline);
	addCoreMessage(IRCD,m);
	m = createMessage("UNRXLINE",  denora_event_unxline);
	addCoreMessage(IRCD,m);
	m = createMessage("UID",       denora_event_nick);
	addCoreMessage(IRCD,m);
	m = createMessage("OPERWALL",  denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("RESV",      denora_event_null);
	addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */


/* :42X BMASK 1106409026 #ircops b :*!*@*.aol.com */
int denora_event_bmask(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_bmask(av);
	return MOD_CONT;
}

int denora_event_pass(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (UseTS6)
	{
		if (av[3])
		{
			TS6UPLINK = sstrdup(av[3]);
		}
	}
	return MOD_CONT;
}

int denora_event_encap(char *source, int ac, char **av)
{
	char *newav[3];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (!stricmp(av[1], "CHGNAME"))
	{
		change_user_realname(av[2], av[3]);
	}
	if (!stricmp(av[1], "CHGHOST"))
	{
		change_user_host(av[2], av[3]);
	}
	if (!stricmp(av[1], "CHGIDENT"))
	{
		change_user_username(av[2], av[3]);
	}
	if (!stricmp(av[1], "XLINE"))
	{
		sql_do_xline(av[3], av[5]);
	}
	if (!stricmp(av[1], "SVSNICK"))
	{
		do_nick(av[2], av[3], NULL, NULL, NULL, NULL,
		        time(NULL), 0, NULL, NULL, NULL, 0, NULL, NULL);
	}
	if (!stricmp(av[1], "SVSMODE"))
	{
		newav[0] = av[2];
		newav[1] = av[4];
		newav[2] = av[3];
		denora_event_svsmode(source, 3, newav);
	}
	return MOD_CONT;
}

/* SVSMODE - forwarded */
int denora_event_svsmode(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (*av[0] != '#')
	{
		do_svsumode(ac, av);
	}
	else
	{
		do_cmode(source, ac, av);
	}
	return MOD_CONT;
}

int denora_event_kline(char *source, int ac, char **av)
{
	char buf[BUFSIZE];
	*buf = '\0';

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
	sql_do_server_bans_add(NULL, av[2], av[3], source, buf, av[1], av[4]);
	return MOD_CONT;
}

int denora_event_unkline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	sql_do_server_bans_remove(NULL, av[1], av[2]);
	return MOD_CONT;
}

int denora_event_xline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	sql_do_xline(av[1], av[3]);
	return MOD_CONT;
}

int denora_event_unxline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	sql_do_unxline(av[1]);
	return MOD_CONT;
}

void rizon_cmd_join(char *user, char *channel, time_t chantime)
{
	Uid *ud;

	ud = find_uid(user);
	send_cmd(NULL, "SJOIN %ld %s + :%s", (long int) chantime,
	         channel, (UseTS6 ? (ud ? ud->uid : user) : user));
	if (AutoOp && AutoMode && LogChannel == channel)
		denora_automode(channel);
}

/*
 * SVINFO
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
void rizon_cmd_svinfo()
{
	if (UseTS6)
	{
		send_cmd(NULL, "SVINFO 6 3 0 :%ld", (long int) time(NULL));
	}
	else
	{
		send_cmd(NULL, "SVINFO 5 5 0 :%ld", (long int) time(NULL));
	}
}

/* CAPAB */
/*
  QS     - Can handle quit storm removal
  EX     - Can do channel +e exemptions
  CHW    - Can do channel wall @#
  LL     - Can do lazy links
  IE     - Can do invite exceptions
  EOB    - Can do EOB message
  KLN    - Can do KLINE message
  GLN    - Can do GLINE message
  HOPS   - can do half ops (+h)
  HUB    - This server is a HUB
  AOPS   - Can do anon ops (+a)
  UID    - Can do UIDs
  ZIP    - Can do ZIPlinks
  ENC    - Can do ENCrypted links
  KNOCK  -  supports KNOCK
  TBURST - supports TBURST
  PARA	 - supports invite broadcasting for +p
  ENCAP	 - ?
*/
void rizon_cmd_capab()
{
	/*send_cmd(NULL, "CAPAB :TBURST TB KNOCK UNKLN KLN GLN ENCAP CHW IE EX CLUSTER EOB LL QS HUB"); */
	send_cmd(NULL,
	         "CAPAB :TBURST TB KNOCK UNKLN KLN GLN CHW IE EX CLUSTER EOB LL QS HUB ENCAP");
}


void rizon_cmd_stats(char *sender, const char *letter, char *server)
{
	Uid *ud;
	ud = find_uid(sender);

	send_cmd((ud ? ud->uid : sender), "STATS %s %s", letter, server);
}

/* PASS */
void rizon_cmd_pass(char *pass)
{
	if (UseTS6)
	{
		send_cmd(NULL, "PASS %s TS 6 :%s", pass, TS6SID);
	}
	else
	{
		send_cmd(NULL, "PASS %s :TS", pass);
	}
}

/* SERVER name hop descript */
void rizon_cmd_server(char *servname, int hop, char *descript)
{
	send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

void rizon_cmd_connect(void)
{
	/* Make myself known to myself in the serverlist */
	if (UseTS6 && Numeric)
	{
		me_server =
		    do_server(NULL, ServerName, (char *) "0", ServerDesc, TS6SID);
	}
	else
	{
		me_server =
		    do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);
	}

	rizon_cmd_pass(RemotePassword);
	rizon_cmd_capab();
	rizon_cmd_server(ServerName, 1, ServerDesc);
	rizon_cmd_svinfo();
}

void rizon_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                        char *modes)
{
	Uid *ud;
	char *genid = uid_gen();

	if (UseTS6)
	{
		ud = find_uid(nick);

		send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0 %s :%s", nick,
		         (long int) time(NULL), modes, user, host, (ud ? ud->uid : genid),
		         real);
		if (!ud)
		{
				new_uid(nick, genid);
		}
	}
	else
	{
		send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
		         (long int) time(NULL), modes, user, host, ServerName,
		         real);
	}
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

void rizon_cmd_part(char *nick, char *chan, char *buf)
{
	Uid *ud;

	ud = find_uid(nick);

	if (buf)
	{
		send_cmd((UseTS6 ? ud->uid : nick), "PART %s :%s", chan, buf);
	}
	else
	{
		send_cmd((UseTS6 ? ud->uid : nick), "PART %s", chan);
	}
}

int denora_event_sethost(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	change_user_host(av[0], av[1]);
	return MOD_CONT;
}

int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
		return MOD_CONT;
	rizon_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
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

/* 97H KILL 69CAABOR8 :geo.rizon.net!geo.rizon.net!GeoServ!GeoServ (4 joins/parts in #DontJoinItsATrap within 6 seconds.) */
int denora_event_kill(char *source, int ac, char **av)
{
	User *u = NULL;
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
		return MOD_CONT;

	u = find_byuid(av[0]);
	m_kill(source, u ? u->nick : av[0], av[1]);
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

int denora_event_eob(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	update_sync_state(source, SYNC_COMPLETE);
	return MOD_CONT;
}

void rizon_cmd_eob(void)
{
	send_cmd(UseTS6 ? TS6SID : ServerName, "EOB");
}

void rizon_cmd_ping(char *server)
{
	send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

int denora_event_join(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	do_sjoin(source, ac > 1 ? 2 : ac, av);
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

int denora_event_privmsg(char *source, int ac, char **av)
{
	User *u = NULL;
	User *ud = NULL;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
	{
		return MOD_CONT;
	}

	if (UseTS6)
	{
		u = find_byuid(source);
		ud = find_byuid(av[0]);
	}
	m_privmsg((UseTS6 ? (u ? u->nick : source) : source),
		          (UseTS6 ? (ud ? ud->nick : av[0]) : av[0]), av[1]);
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
	Uid *ud;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	if (source && ac >= 1)
	{
		ud = find_nickuid(av[0]);
		m_whois(source, (UseTS6 ? (ud ? ud->nick : av[0]) : av[0]));
	}
	return MOD_CONT;
}

/* EVENT: SERVER */
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
	do_server(source, av[0], av[1], av[2], (!stricmp(av[1], "1") && UseTS6 && TS6UPLINK ? TS6UPLINK : NULL));
	return MOD_CONT;
}

int denora_event_sid(char *source, int ac, char **av)
{
	Server *s;

	/* :42X SID trystan.nomadirc.net 2 43X :ircd-ratbox test server */

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	s = server_find(source);

	do_server(s->name, av[0], av[1], av[3], av[2]);
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
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 1)
		return MOD_CONT;
	do_quit(source, ac, av);
	return MOD_CONT;
}

void rizon_cmd_mode(char *source, char *dest, char *buf)
{
	Uid *ud;

	if (source)
	{
		ud = find_uid(source);
		send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "MODE %s %s",
			         dest, buf);
	}
	else
{
		send_cmd(source, "MODE %s %s", dest, buf);
	}
}

void rizon_cmd_nick(char *nick, char *name, const char *mode)
{
	char *ipaddr;
	char *genid = uid_gen();
	Uid *ud;

	if (UseTS6)
	{
		ipaddr = host_resolve(ServiceHost);
		ud = find_uid(nick);
		send_cmd(TS6SID, "UID %s 1 %ld %s %s %s %s %s 0 %s :%s", nick,
		         (long int) time(NULL), mode, ServiceUser, ServiceHost,
		         ipaddr, (ud ? ud->uid : genid), ServiceHost, name);
		if (!ud)
		{
				new_uid(nick, genid);
		}
		free(ipaddr);
	}
	else
	{
		send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
		         (long int) time(NULL), mode, ServiceUser, ServiceHost,
		         ServerName, name);
	}
}

/* QUIT */
void rizon_cmd_quit(char *source, char *buf)
{
	Uid *ud;
	ud = find_uid(source);

	if (buf)
	{
		send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "QUIT :%s",
			         buf);
	}
	else
{
		send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "QUIT");
	}
}

/* PONG */
void rizon_cmd_pong(char *servname, char *who)
{
	if (UseTS6)
	{
		send_cmd(TS6SID, "PONG %s", who);
	}
	else
	{
		send_cmd(servname, "PONG %s", who);
	}
}

/* SQUIT */
void rizon_cmd_squit(char *servname, char *message)
{
	send_cmd(ServerName, "SQUIT %s :%s", servname, message);
}

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
		Server *s;
		s = server_find(source);

		if (s && *av[0])
		{
			do_umode(av[0], ac, av);
		}
		else
		{
			do_umode(source, ac, av);
		}
	}
	return MOD_CONT;
}

/* 00HAAAAAJ TMODE 1218474093 #oper +a 669AAAAAB */
int denora_event_tmode(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac > 2 && (*av[1] == '#' || *av[1] == '&'))
	{
		ac--;
		av++;
		do_cmode(source, ac, av);
	}
	return MOD_CONT;
}

/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	capab_parse(ac, av);
	return MOD_CONT;
}

/*
 * SVINFO
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
int denora_event_svinfo(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	/* currently not used but removes the message : unknown message from server */
	return MOD_CONT;
}

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

void rizon_cmd_version(char *server)
{
	Uid *ud;
	ud = find_uid(s_StatServ);

	send_cmd((ud ? ud->uid : s_StatServ), "VERSION %s", server);
}

void rizon_cmd_motd(char *sender, char *server)
{
	Uid *ud;
	ud = find_uid(sender);

	send_cmd((ud ? ud->uid : sender), "MOTD %s", server);
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

void rizon_cmd_ctcp(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void moduleAddIRCDCmds()
{
	pmodule_cmd_nick(rizon_cmd_nick);
	pmodule_cmd_mode(rizon_cmd_mode);
	pmodule_cmd_bot_nick(rizon_cmd_bot_nick);
	pmodule_cmd_notice(rizon_cmd_notice);
	pmodule_cmd_privmsg(rizon_cmd_privmsg);
	pmodule_cmd_serv_notice(rizon_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(rizon_cmd_serv_privmsg);
	pmodule_cmd_quit(rizon_cmd_quit);
	pmodule_cmd_pong(rizon_cmd_pong);
	pmodule_cmd_join(rizon_cmd_join);
	pmodule_cmd_part(rizon_cmd_part);
	pmodule_cmd_global(rizon_cmd_global);
	pmodule_cmd_squit(rizon_cmd_squit);
	pmodule_cmd_connect(rizon_cmd_connect);
	pmodule_cmd_eob(rizon_cmd_eob);
	pmodule_cmd_ctcp(rizon_cmd_ctcp);
	pmodule_cmd_version(rizon_cmd_version);
	pmodule_cmd_stats(rizon_cmd_stats);
	pmodule_cmd_motd(rizon_cmd_motd);
	pmodule_cmd_ping(rizon_cmd_ping);
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

	pmodule_ircd_version("ircd-rizon 3.0+");
	pmodule_ircd_cap(myIrcdcap);
	pmodule_ircd_var(myIrcd);
	pmodule_ircd_useTSMode(0);
	pmodule_irc_var(IRC_IRCDRIZON);
	IRCDModeInit();
	pmodule_oper_umode(UMODE_o);
	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();
	return MOD_CONT;
}
