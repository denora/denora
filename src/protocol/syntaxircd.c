/* ShadowIRCd 6.0+ functions
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 *
 */

#include "denora.h"
#include "shadowircd.h"

IRCDVar myIrcd[] = {
	{"syntaxIRCd 1.0+",		/* ircd name			*/
	 "+i",				/* StatServ mode		*/
	 IRCD_ENABLE,			/* Vhost			*/
	 IRCD_ENABLE,			/* Supports SGlines		*/
	 IRCD_ENABLE,			/* sgline sql table		*/
	 IRCD_DISABLE,			/* Supports SQlines		*/
	 IRCD_DISABLE,			/* sqline sql table		*/
	 IRCD_DISABLE,			/* Supports SZlines		*/
	 IRCD_ENABLE,			/* Has exceptions +e		*/
	 IRCD_DISABLE,			/* vidents			*/
	 IRCD_ENABLE,			/* NICKIP			*/
	 IRCD_ENABLE,			/* VHOST ON NICK		*/
	 IRCD_DISABLE,			/* +f				*/
	 IRCD_ENABLE,			/* +j				*/
	 IRCD_DISABLE,			/* +L				*/
	 IRCD_DISABLE,			/* +f Mode			*/
	 CMODE_j,			/* +j Mode			*/
	 CMODE_f,			/* +L Mode			*/
	 NULL,				/* CAPAB Chan Modes		*/
	 IRCD_DISABLE,			/* We support TOKENS		*/
	 IRCD_ENABLE,			/* TOKENS are CASE Sensitive	*/
	 IRCD_DISABLE,			/* TIME STAMPS are BASE64	*/
	 IRCD_ENABLE,			/* +I support			*/
	 IRCD_DISABLE,			/* SJOIN ban char		*/
	 IRCD_DISABLE,			/* SJOIN except char		*/
	 IRCD_DISABLE,			/* SJOIN invite char		*/
	 UMODE_x,			/* umode for vhost		*/
	 IRCD_ENABLE,			/* owner			*/
	 IRCD_ENABLE,			/* protect			*/
	 IRCD_ENABLE,			/* halfop			*/
	 NULL,				/* user modes			*/
	 NULL,				/* channel modes		*/
	 IRCD_DISABLE,			/* flood			*/
	 'j',				/* flood other			*/
	 'J',				/* join throttle		*/
	 IRCD_DISABLE,			/* nick change flood		*/
	 'x',				/* vhost			*/
	 IRCD_DISABLE,			/* vhost other			*/
	 'f',				/* Channel forwarding		*/
	 IRCD_DISABLE,			/* p10				*/
	 IRCD_ENABLE,			/* ts6				*/
	 IRCD_ENABLE,			/* numeric			*/
	 IRCD_DISABLE,			/* gagged			*/
	 IRCD_DISABLE,			/* spamfilter			*/
	 'b',				/* ban char			*/
	 'e',				/* except char			*/
	 'I',				/* invite char			*/
	 IRCD_DISABLE,			/* zip				*/
	 IRCD_ENABLE,			/* ssl				*/
	 IRCD_ENABLE,			/* uline			*/
	 NULL,				/* nickchar			*/
	 IRCD_DISABLE,			/* svid				*/
	 IRCD_DISABLE,			/* hidden oper			*/
	 IRCD_ENABLE,			/* extra warning		*/
	 IRCD_DISABLE,			/* Report sync state		*/
	 CMODE_P,			/* Persistent channel mode   	*/
	}
};

IRCDCAPAB myIrcdcap[] = {
	{
	 0,				/* NOQUIT	*/
	 0,				/* TSMODE	*/
	 0,				/* UNCONNECT	*/
	 0,				/* NICKIP	*/
	 0,				/* SJOIN	*/
	 CAPAB_ZIP,			/* ZIP		*/
	 0,				/* BURST	*/
	 CAPAB_TS5,			/* TS5		*/
	 0,				/* TS3		*/
	 0,				/* DKEY		*/
	 0,				/* PT4		*/
	 0,				/* SCS		*/
	 CAPAB_QS,			/* QS		*/
	 CAPAB_UID,			/* UID		*/
	 CAPAB_KNOCK,			/* KNOCK	*/
	 0,				/* CLIENT	*/
	 0,				/* IPV6		*/
	 0,				/* SSJ5		*/
	 0,				/* SN2		*/
	 0,				/* TOKEN	*/
	 0,				/* VHOST	*/
	 0,				/* SSJ3		*/
	 0,				/* NICK2	*/
	 0,				/* UMODE2	*/
	 0,				/* VL		*/
	 0,				/* TLKEXT	*/
	 0,				/* DODKEY	*/
	 0,				/* DOZIP	*/
	 0, 0, 0}
};

/*************************************************************************/

void IRCDModeInit(void)
{
	/* User Modes*/
	ModuleSetUserMode(UMODE_B, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_C, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_Q, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_V, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_Z, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_l, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
	ModuleSetUserMode(UMODE_z, IRCD_ENABLE);
	ModuleUpdateSQLUserMode();

	/* Channel Modes with args */
	CreateChanBanMode(CMODE_b, add_ban, del_ban);
	CreateChanBanMode(CMODE_e, add_exception, del_exception);
	CreateChanBanMode(CMODE_I, add_invite, del_invite);

	/* Channel Modes */
	CreateChanMode(CMODE_A, NULL, NULL);
	CreateChanMode(CMODE_C, NULL, NULL);
	CreateChanMode(CMODE_D, NULL, NULL);
	CreateChanMode(CMODE_E, NULL, NULL);
	CreateChanMode(CMODE_F, NULL, NULL);
	CreateChanMode(CMODE_G, NULL, NULL);
	CreateChanMode(CMODE_J, NULL, NULL);
	CreateChanMode(CMODE_K, NULL, NULL);
	CreateChanMode(CMODE_L, NULL, NULL);
	CreateChanMode(CMODE_M, NULL, NULL);
	CreateChanMode(CMODE_O, NULL, NULL);
	CreateChanMode(CMODE_P, NULL, NULL);
	CreateChanMode(CMODE_Q, NULL, NULL);
	CreateChanMode(CMODE_S, NULL, NULL);
	CreateChanMode(CMODE_T, NULL, NULL);
	CreateChanMode(CMODE_c, NULL, NULL);
	CreateChanMode(CMODE_d, NULL, NULL);
	CreateChanMode(CMODE_f, set_redirect, get_redirect);
	CreateChanMode(CMODE_g, NULL, NULL);
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
	CreateChanMode(CMODE_z, NULL, NULL);

	ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
	ModuleSetChanUMode('+', 'v', STATUS_VOICE);
	ModuleSetChanUMode('@', 'o', STATUS_OP);
	ModuleSetChanUMode('&', 'a', STATUS_PROTECTED);
	ModuleSetChanUMode('~', 'q', STATUS_OWNER);
	ModuleUpdateSQLChanMode();
}

void shadowircd_cmd_notice(char *source, char *dest, char *buf)
{
	Uid *ud;
	User *u;

	ud = find_uid(source);
	u = finduser(dest);
	send_cmd((ud ? ud->uid : source),
			 "NOTICE %s :%s", (u ? u->uid : dest), buf);
}

void shadowircd_cmd_privmsg(char *source, char *dest, char *buf)
{
	Uid *ud, *ud2;

	ud = find_uid(source);
	ud2 = find_uid(dest);

	send_cmd((ud ? ud->uid : source), "PRIVMSG %s :%s",
			 (ud2 ? ud2->uid : dest), buf);
}

void shadowircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $$%s :%s", dest, msg);
}

void shadowircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $$%s :%s", dest, msg);
}

void shadowircd_cmd_stats(char *sender, const char *letter, char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(sender);
	s = server_find(server);

	send_cmd((ud ? ud->uid : sender), "STATS %s %s", letter,
			 (s ? (s->suid ? s->suid : server) : server));
}

void shadowircd_cmd_global(char *source, char *buf)
{
	Uid *u;

	if (source) {
		u = find_uid(source);
		if (u) {
			send_cmd((u ? u->uid : source), "OPERWALL :%s", buf);
		} else {
			send_cmd(TS6SID, "OPERWALL :%s", buf);
		}
	} else {
		send_cmd(TS6SID, "OPERWALL :%s", buf);
	}
}

int denora_event_sjoin(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	do_sjoin(source, ac, av);
	return MOD_CONT;
}

/*
   TS6

   av[0] = nick
   av[1] = hop
   av[2] = ts
   av[3] = modes
   av[4] = user
   av[5] = host
   av[6] = IP
   av[7] = UID
   av[8] = info

   NON TS6

   av[0] = nick
   av[1] = hop
   av[2] = ts
   av[3] = modes
   av[4] = user
   av[5] = host
   av[6] = server
   av[7] = real

*/
int denora_event_nick(char *source, int ac, char **av)
{
	Server *s;
	User *user;
	char *ipchar = NULL;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	/* nenolod said non-TS6 was never possible he lied */
	if (ac == 8) {
		ipchar = host_resolve(av[5]);
		user = do_nick(source, av[0], av[4], av[5], av[6], av[7],
					   strtoul(av[2], NULL, 10), 0, ipchar, NULL, av[7],
					   strtoul(av[1], NULL, 10), av[3], NULL);
		if (user) {
			denora_set_umode(user, 1, &av[3]);
		}
		free(ipchar);

	} else if (ac == 9) {
		s = server_find(source);
		/* Source is always the server */
		*source = '\0';
		ipchar = host_resolve(av[5]);
		user = do_nick(source, av[0], av[4], av[5], s->name, av[8],
					   strtoul(av[2], NULL, 10), 0, ipchar, NULL, av[7],
					   strtoul(av[1], NULL, 10), av[3], NULL);
		free(ipchar);
	} else {
		user = find_byuid(source);
		do_nick((user ? user->nick : source), av[0], NULL, NULL, NULL,
				NULL, strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0,
				NULL, NULL);
	}
	return MOD_CONT;
}

/*
   TS6
   av[0] = nick
   av[1] = hop
   av[2] = ts
   av[3] = modes
   av[4] = user
   av[5] = vhost
   av[6] = IP
   av[7] = UID
   ac[8] = host or *
   ac[9] = services login
   av[10] = info

*/

int denora_event_euid(char *source, int ac, char **av)
{
	Server *s;
	User *user;
	time_t ts;

	if (UseTS6 && ac == 11) {
		s = findserver_uid(servlist, source);
		/* Source is always the server */
		*source = '\0';
		ts = strtoul(av[2], NULL, 10);
		user =
			do_nick(source, av[0], av[4],
					!strcmp(av[8], "*") ? av[5] : av[8], s->name, av[10],
					ts, !stricmp(av[0], av[9]) ? ts : 0, av[6], av[5],
					av[7], atoi(av[1]), av[3], !strcmp(av[9], "*") ? av[9] : NULL);
		if (user) {
			denora_set_umode(user, 1, &av[3]);
		}
	}
	return MOD_CONT;
}

/* :42XAAAAAB TOPIC #testchan :test test test */
int denora_event_topic(char *source, int ac, char **av)
{
	char *newav[5];
	User *u;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac == 2) {
		u = user_find(source);
		if (u) {
			newav[0] = sstrdup(av[0]);
			newav[1] = sstrdup(u->nick);
			newav[2] = itostr(time(NULL));
			newav[3] = sstrdup(av[1]);
			do_topic(4, newav);
			free(newav[0]);
			free(newav[1]);
			free(newav[3]);
		}

	} else {
		do_topic(ac, av);
	}
	return MOD_CONT;
}

int denora_event_tburst(char *source, int ac, char **av)
{
	char *scratch;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 4)
		return MOD_CONT;

	scratch = av[2];
	av[2] = av[1];
	av[1] = scratch;
	do_topic(4, av);

	return MOD_CONT;
}

int denora_event_436(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
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

		if (!BadPtr(Numeric)) {
			if (strlen(Numeric) == 3) {
				TS6SID = sstrdup(Numeric);
				UseTSMODE = 1;  /* TMODE */
			} else {
				alog(LOG_NORMAL, "Incorrect size of TS6 Numeric - exiting");
				exit(1);
			}
		} else {
			alog(LOG_NORMAL, "TS6 requires the use of Numeric - exiting");
			exit(1);
		}

	m = createMessage("401",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("402",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("436",	denora_event_436); addCoreMessage(IRCD,m);
	m = createMessage("AWAY",	denora_event_away); addCoreMessage(IRCD,m);
	m = createMessage("INVITE",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("JOIN",	denora_event_join); addCoreMessage(IRCD,m);
	m = createMessage("KICK",	denora_event_kick); addCoreMessage(IRCD,m);
	m = createMessage("KILL",	denora_event_kill); addCoreMessage(IRCD,m);
	m = createMessage("MODE",	denora_event_mode); addCoreMessage(IRCD,m);
	m = createMessage("TMODE",	denora_event_tmode); addCoreMessage(IRCD,m);
	m = createMessage("MOTD",	denora_event_motd); addCoreMessage(IRCD,m);
	m = createMessage("NICK",	denora_event_nick); addCoreMessage(IRCD,m);
	m = createMessage("BMASK",	denora_event_bmask); addCoreMessage(IRCD,m);
	m = createMessage("UID",	denora_event_nick); addCoreMessage(IRCD,m);
	m = createMessage("NOTICE",	denora_event_notice); addCoreMessage(IRCD,m);
	m = createMessage("PART",	denora_event_part); addCoreMessage(IRCD,m);
	m = createMessage("PASS",	denora_event_pass); addCoreMessage(IRCD,m);
	m = createMessage("PING",	denora_event_ping); addCoreMessage(IRCD,m);
	m = createMessage("PRIVMSG",	denora_event_privmsg); addCoreMessage(IRCD,m);
	m = createMessage("PONG",	denora_event_pong); addCoreMessage(IRCD,m);
	m = createMessage("QUIT",	denora_event_quit); addCoreMessage(IRCD,m);
	m = createMessage("SERVER",	denora_event_server); addCoreMessage(IRCD,m);
	m = createMessage("SQUIT",	denora_event_squit); addCoreMessage(IRCD,m);
	m = createMessage("TOPIC",	denora_event_topic); addCoreMessage(IRCD,m);
	m = createMessage("TB",		denora_event_tburst); addCoreMessage(IRCD,m);
	m = createMessage("USER",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("WALLOPS",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("OPERWALL",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("WHOIS",	denora_event_whois); addCoreMessage(IRCD,m);
	m = createMessage("SVSMODE",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("SVSNICK",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("CAPAB",	denora_event_capab); addCoreMessage(IRCD,m);
	m = createMessage("SJOIN",	denora_event_sjoin); addCoreMessage(IRCD,m);
	m = createMessage("SVINFO",	denora_event_svinfo); addCoreMessage(IRCD,m);
	m = createMessage("ADMIN",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("ERROR",	denora_event_error); addCoreMessage(IRCD,m);
	m = createMessage("421",	denora_event_null); addCoreMessage(IRCD,m);
	m = createMessage("ENCAP",	denora_event_encap); addCoreMessage(IRCD,m);
	m = createMessage("SID",	denora_event_sid); addCoreMessage(IRCD,m);
	m = createMessage("KLINE",	denora_event_kline); addCoreMessage(IRCD,m);
	m = createMessage("UNKLINE",	denora_event_unkline); addCoreMessage(IRCD,m);
	m = createMessage("XLINE",	denora_event_xline); addCoreMessage(IRCD,m);
	m = createMessage("UNXLINE",	denora_event_unxline); addCoreMessage(IRCD,m);
	m = createMessage("EUID",	denora_event_euid); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

int denora_event_encap(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (!stricmp(av[1], "XLINE")) {
		sql_do_xline(av[3], av[5]);
	}
	if (!stricmp(av[1], "CHGHOST")) {
		change_user_host(av[2], av[3]);
	}
	return MOD_CONT;
}

int denora_event_kline(char *source, int ac, char **av)
{
	char buf[BUFSIZE];
	*buf = '\0';

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
	sql_do_server_bans_add(NULL, av[2], av[3], source, buf, av[1], av[4]);
	return MOD_CONT;
}

int denora_event_unkline(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	sql_do_server_bans_remove(NULL, av[1], av[2]);
	return MOD_CONT;
}

int denora_event_xline(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	sql_do_xline(av[1], av[3]);
	return MOD_CONT;
}

int denora_event_unxline(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	sql_do_unxline(av[1]);
	return MOD_CONT;
}

void shadowircd_cmd_join(char *user, char *channel, time_t chantime)
{
	Uid *ud;

	ud = find_uid(user);
	send_cmd(NULL, "SJOIN %ld %s + :%s", (long int) chantime,
			 channel, (ud ? ud->uid : user));
	if (AutoOp && AutoMode && LogChannel == channel)
		denora_automode(channel);
}

/*
 * SVINFO
 *	  parv[0] = sender prefix
 *	  parv[1] = TS_CURRENT for the server
 *	  parv[2] = TS_MIN for the server
 *	  parv[3] = server is standalone or connected to non-TS only
 *	  parv[4] = server's idea of UTC time
 */
void shadowircd_cmd_svinfo(void)
{
	send_cmd(NULL, "SVINFO 6 6 0 :%ld", (long int) time(NULL));
}

void shadowircd_cmd_svsinfo(void)
{

}

/* CAPAB */
/*
  QS		- Can handle quit storm removal
  EX		- Can do channel +e exemptions
  CHW		- Can do channel wall @#
  LL		- Can do lazy links
  IE		- Can do invite exceptions
  EOB		- Can do EOB message
  KLN		- Can do KLINE message
  GLN		- Can do GLINE message
  HOPS		- Supports halfops
  AOPS		- Supports anonymous ops
  HUB		- This server is a HUB
  UID		- Can do UIDs
  ZIP		- Can do ZIPlinks
  ENC		- Can do ENCrypted links
  KNOCK		- Supports KNOCK
  TBURST	- Supports TBURST
  PARA		- Supports invite broadcasting for +p
  ENCAP		- Supports message encapsulation
  SERVICES	- Supports services-oriented TS6 extensions
  EUID		- Supports EUID and non-ENCAP CHGHOST
*/
void shadowircd_cmd_capab(void)
{
	send_cmd(NULL,
			 "CAPAB :QS EX CHW IE KLN GLN HOPS AOPS KNOCK TB UNKLN CLUSTER ENCAP SERVICES EUID");
}

/* PASS */
void shadowircd_cmd_pass(char *pass)
{
	send_cmd(NULL, "PASS %s TS 6 :%s", pass, TS6SID);
}

/* SERVER name hop descript */
void shadowircd_cmd_server(char *servname, int hop, char *descript)
{
	send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

void shadowircd_cmd_connect(void)
{
	/* Make myself known to myself in the serverlist */
	me_server =
		do_server(NULL, ServerName, (char *) "0", ServerDesc, TS6SID);

	shadowircd_cmd_pass(RemotePassword);
	shadowircd_cmd_capab();
	shadowircd_cmd_server(ServerName, 1, ServerDesc);
	shadowircd_cmd_svinfo();
}

void shadowircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
							char *modes)
{
	char *genid = uid_gen();
	Uid *ud;

	ud = find_uid(nick);
	send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0 %s :%s", nick,
			 (long int) time(NULL), modes, user, host, (ud ? ud->uid : genid), real);
	if (!ud)
	{
			new_uid(nick, genid);
	}
}

void shadowircd_cmd_part(char *nick, char *chan, char *buf)
{
	Uid *ud;

	ud = find_uid(nick);

	if (buf) {
		send_cmd((ud ? ud->uid : nick), "PART %s :%s", chan, buf);
	} else {
		send_cmd((ud ? ud->uid : nick), "PART %s", chan);
	}
}

int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
		return MOD_CONT;
	shadowircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);

	/* Little hack since ShadowIRCd appears not to send EOB message */
	if (!UplinkSynced)
		update_sync_state(source, SYNC_COMPLETE);
	return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
	User *u = NULL;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	u = find_byuid(source);

	if (!ac) {
		m_away((u ? u->nick : source), NULL);
	} else {
		m_away((u ? u->nick : source), av[0]);
	}
	return MOD_CONT;
}

int denora_event_kill(char *source, int ac, char **av)
{
	User *u = NULL;

	if (denora->protocoldebug) {
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
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 3)
		return MOD_CONT;
	do_kick(source, ac, av);
	return MOD_CONT;
}

void shadowircd_cmd_eob(void)
{
	/* doesn't support EOB */
}

void shadowircd_cmd_ping(char *server)
{
	Server *s, *mys;
	s = server_find(server);
	mys = server_find(ServerName);
	send_cmd((mys ? (mys->suid ? mys->suid : ServerName) : ServerName),
			 "PING %s :%s",
			 (mys ? (mys->suid ? mys->suid : ServerName) : ServerName),
			 (s ? (s->suid ? s->suid : server) : server));
}


int denora_event_join(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 1) {
		/* ignore cmodes in JOIN as per TS6 v8 */
		do_sjoin(source, ac > 2 ? 2 : ac, av);
		return MOD_CONT;
	} else {
		do_join(source, ac, av);
	}
	return MOD_CONT;
}

int denora_event_motd(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	m_motd(source);
	return MOD_CONT;
}

int denora_event_pong(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	server_store_pong(source, time(NULL));
	return MOD_CONT;
}

int denora_event_privmsg(char *source, int ac, char **av)
{
	User *u = NULL;
	User *ud = NULL;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 2) {
		return MOD_CONT;
	}

	u = find_byuid(source);
	ud = find_byuid(av[0]);
	m_privmsg((u ? u->nick : source), (ud ? ud->nick : av[0]), av[1]);
	return MOD_CONT;
}

int denora_event_part(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	do_part(source, ac, av);
	return MOD_CONT;
}

int denora_event_whois(char *source, int ac, char **av)
{
	Uid *ud;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (source && ac >= 1) {
		ud = find_nickuid(av[0]);
		m_whois(source, (ud ? ud->nick : av[0]));
	}
	return MOD_CONT;
}

/* EVENT: SERVER */
int denora_event_server(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (!denora->uplink) {
		denora->uplink = sstrdup(av[0]);
	}
	do_server(source, av[0], av[1], av[2], (!stricmp(av[1], "1") ? TS6UPLINK : NULL));
	return MOD_CONT;
}

int denora_event_sid(char *source, int ac, char **av)
{
	Server *s;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	/* :42X SID trystan.nomadirc.net 2 43X :ircd-ratbox test server */

	s = server_find(source);

	do_server(s->name, av[0], av[1], av[3], av[2]);
	return MOD_CONT;
}

int denora_event_squit(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 2)
		return MOD_CONT;
	do_squit(av[0]);
	return MOD_CONT;
}

int denora_event_quit(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 1) {
		return MOD_CONT;
	}

	do_quit(source, ac, av);
	return MOD_CONT;
}

void shadowircd_cmd_mode(char *source, char *dest, char *buf)
{
	Uid *ud;

	if (source) {
		ud = find_uid(source);
		send_cmd((ud ? ud->uid : source), "MODE %s %s", dest, buf);
	} else {
		send_cmd(source, "MODE %s %s", dest, buf);
	}
}

void shadowircd_cmd_tmode(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	USE_VAR(source);

	*buf = '\0';

	if (fmt) {
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	if (!*buf) {
		return;
	}

	send_cmd(NULL, "MODE %s %s", dest, buf);
}

void shadowircd_cmd_nick(char *nick, char *name, const char *mode)
{
	char *genid = uid_gen();
	Uid *ud;

	ud = find_uid(nick);

	send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0 %s :%s", nick,
			 (long int) time(NULL), mode, ServiceUser, ServiceHost,
			 (ud ? ud->uid : genid), name);
	if (!ud)
	{
			new_uid(nick, genid);
	}

}

/* QUIT */
void shadowircd_cmd_quit(char *source, char *buf)
{
	Uid *ud;
	ud = find_uid(source);

	if (buf) {
		send_cmd((ud ? ud->uid : source), "QUIT :%s", buf);
	} else {
		send_cmd((ud ? ud->uid : source), "QUIT");
	}
}

/* PONG */
void shadowircd_cmd_pong(char *servname, char *who)
{
	send_cmd((UseTS6 ? TS6SID : servname), "PONG %s", who);
}

/* SQUIT */
void shadowircd_cmd_squit(char *servname, char *message)
{
	send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

int denora_event_mode(char *source, int ac, char **av)
{
	User *u, *u2;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac < 2) {
		return MOD_CONT;
	}

	if (*av[0] == '#' || *av[0] == '&') {
		do_cmode(source, ac, av);
	} else {
		u = find_byuid(source);
		u2 = find_byuid(av[0]);
		av[0] = u2->nick;
		do_umode(u->nick, ac, av);
	}
	return MOD_CONT;
}

int denora_event_tmode(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac > 2 && (*av[1] == '#' || *av[1] == '&')) {
		/* do_cmode() doesn't like a timestamp as the first
		 * parameter, so we do this since it wants a string vector --nenolod
		 */
		ac--;
		av++;
		do_cmode(source, ac, av);
	}
	return MOD_CONT;
}

/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	capab_parse(ac, av);
	return MOD_CONT;
}

/*
 * SVINFO
 *	  parv[0] = sender prefix
 *	  parv[1] = TS_CURRENT for the server
 *	  parv[2] = TS_MIN for the server
 *	  parv[3] = server is standalone or connected to non-TS only
 *	  parv[4] = server's idea of UTC time
 */
int denora_event_svinfo(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	/* currently not used but removes the message : unknown message from server */
	return MOD_CONT;
}

int denora_event_pass(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac >= 4) {
		TS6UPLINK = sstrdup(av[3]);
	}
	return MOD_CONT;
}

/* :42X BMASK 1106409026 #ircops b :*!*@*.aol.com */
int denora_event_bmask(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	do_bmask(av);
	return MOD_CONT;
}

int denora_event_error(char *source, int ac, char **av)
{
	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac >= 1) {
		alog(LOG_ERROR, "ERROR: %s", av[0]);
	}
	return MOD_CONT;
}

void shadowircd_cmd_version(char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(s_StatServ);
	s = server_find(server);

	send_cmd((ud ? ud->uid : s_StatServ), "VERSION %s",
			 (s ? (s->suid ? s->suid : server) : server));
}

void shadowircd_cmd_motd(char *sender, char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(sender);
	s = server_find(server);

	send_cmd((ud ? ud->uid : sender), "MOTD %s",
			 (s ? (s->suid ? s->suid : server) : server));
}

int denora_event_notice(char *source, int ac, char **av)
{
	User *u = NULL;
	User *ud = NULL;

	if (denora->protocoldebug) {
		protocol_debug(source, ac, av);
	}
	if (ac != 2) {
		return MOD_CONT;
	}

	u = find_byuid(source);
	ud = find_byuid(av[0]);
	m_notice((u ? u->nick : source), (ud ? ud->nick : av[0]), av[1]);
	return MOD_CONT;
}

void shadowircd_cmd_ctcp(char *source, char *dest, char *buf)
{
	Uid *ud, *nickud;
	ud = find_uid(source);
	nickud = find_nickuid(dest);

	send_cmd((ud ? ud->uid : source), "NOTICE %s :\1%s \1",
			 (nickud ? nickud->uid : dest), buf);
}

void moduleAddIRCDCmds()
{
	pmodule_cmd_nick(shadowircd_cmd_nick);
	pmodule_cmd_mode(shadowircd_cmd_mode);
	pmodule_cmd_bot_nick(shadowircd_cmd_bot_nick);
	pmodule_cmd_notice(shadowircd_cmd_notice);
	pmodule_cmd_privmsg(shadowircd_cmd_privmsg);
	pmodule_cmd_serv_notice(shadowircd_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(shadowircd_cmd_serv_privmsg);
	pmodule_cmd_quit(shadowircd_cmd_quit);
	pmodule_cmd_pong(shadowircd_cmd_pong);
	pmodule_cmd_join(shadowircd_cmd_join);
	pmodule_cmd_part(shadowircd_cmd_part);
	pmodule_cmd_global(shadowircd_cmd_global);
	pmodule_cmd_squit(shadowircd_cmd_squit);
	pmodule_cmd_connect(shadowircd_cmd_connect);
	pmodule_cmd_eob(shadowircd_cmd_eob);
	pmodule_cmd_ctcp(shadowircd_cmd_ctcp);
	pmodule_cmd_version(shadowircd_cmd_version);
	pmodule_cmd_stats(shadowircd_cmd_stats);
	pmodule_cmd_motd(shadowircd_cmd_motd);
	pmodule_cmd_ping(shadowircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
	if (denora->protocoldebug) {
		protocol_debug(NULL, argc, argv);
	}
	/* Only 1 protocol module may be loaded */
	if (protocolModuleLoaded()) {
		alog(LOG_NORMAL, langstr(ALOG_MOD_BE_ONLY_ONE));
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("1.0");
	moduleSetType(PROTOCOL);
	pmodule_ircd_version("syntaxIRCd 1.0+");
	pmodule_ircd_cap(myIrcdcap);
	pmodule_ircd_var(myIrcd);
	pmodule_ircd_useTSMode(0);
	IRCDModeInit();
	pmodule_oper_umode(UMODE_o);
	pmodule_irc_var(IRC_SHADOWIRCD);
	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();
	UseTS6 = 1;
	UplinkSynced = 1;
	return MOD_CONT;
}
