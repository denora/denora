/* ScaryNet ircu IRCD functions
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@nomadirc.net
 *
 * (C) 2006 Alexander Maassen
 * Contact me at outsider@scarynet.org
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
#include "scarynet.h"

int p10nickcnt = 0;

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
	moduleAddVersion("2.0");
	moduleSetType(PROTOCOL);

	DenoraXMLIRCdConfig("scarynet.xml");

	pmodule_irc_var(IRC_SCARYNET);
	ModuleChanModeUpdate(CMODE_k, set_key, get_key);  /* Keyed */
	ModuleChanModeUpdate(CMODE_l, set_limit, get_limit);      /* Limit */

	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();

	return MOD_CONT;
}


/*************************************************************************/

char *scarynet_nickip(char *host)
{
	char ipaddr[INET6_ADDRSTRLEN];

	base64toip(host, (char *)&ipaddr);

	alog(LOG_DEBUG, "debug: Decoded base64 %s to %s", host, ipaddr);

	return sstrdup((char *)&ipaddr);
}

/* On Services connect the modes are given */
/* AB N Trystan` 1 1098031638 tslee comcast.net +i AYAmXj ABAAC : real */
/* On user join they are not */
/* AB N Trystan  1 1101443066 tslee comcast.net AYAmXj ABAAB : real */

/*
** NICK - new
**      source  = Ownering Server Prefix
**	parv[0] = nickname
**      parv[1] = hopcount
**      parv[2] = timestamp
**      parv[3] = username
**      parv[4] = host
**      parv[5] = modes
**	parv[6] = account if authed
**      parv[6|7] = base64 ip
**	parv[7|8] = uid
**      parv[8|9] = info
** NICK - change
**      source  = oldnick
**	parv[0] = new nickname
**      parv[1] = timestamp
*/
/*
  AB N Trystan 1 1117327797 tslee c-24-2-101-227.hsd1.ut.comcast.net +i AYAmXj ABAAB :Dreams are answers to questions not yet asked
        0      1  2          2      3                                 4 5       6     7
 */

int denora_event_nick(char *source, int ac, char **av)
{
	User *user;
	Server *s;
	char *ipchar, *modes, *modes2;
	char *account = NULL;
	char hhostbuf[255];
	int ishidden = 0, isaccount = 0;


	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	/* do_nick(const char *source, char *nick, char *username, char *host,
	   char *server, char *realname, time_t ts, uint32 svid,
	   uint32 ip, char *vhost, char *uid, int hopcount, char *modes) */

	if (ac != 2)
	{
		s = server_find(source);

		modes = sstrdup(av[5]);
		modes2 = sstrdup(av[5]);

		if (strpbrk(av[5], "+"))
		{
			while (*modes)
			{
				switch (*modes)
				{
					case 'r':
						isaccount = 1;
						account = sstrdup(av[6]);
						break;
					case 'x':
						ishidden = 1;
						break;
					default:
						break;
				}
				modes++;
			}
			modes = sstrdup(modes2);
		}
		else
			modes = NULL;

		ipchar = scarynet_nickip(av[ac - 3]);

		if (isaccount && ishidden)
		{
			ircsnprintf(hhostbuf, sizeof(av[6]) + sizeof(hhostbuf) + 2,
			            "%s%s%s", HiddenPrefix, av[6], HiddenSuffix);
		}

		user = do_nick("\0", av[0], av[3], av[4], (s ? s->name : source),
		               av[ac - 1], strtoul(av[2], NULL, 10), 0, ipchar,
		               (ishidden && isaccount) ? hhostbuf : NULL, av[ac - 2],
		               strtoul(av[1], NULL, 10), modes, account);

		free(ipchar);
		free(modes);
		free(modes2);
		if (account)
			free(account);
	}
	else
	{
		user = find_byuid(source);
		do_nick((user ? user->nick : source), av[0], NULL, NULL, NULL,
		        NULL, strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0,
		        NULL, NULL);
	}
	return MOD_CONT;
}

int denora_event_436(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 1)
		return MOD_CONT;

	m_nickcoll(av[0]);
	return MOD_CONT;
}

int denora_event_error(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac >= 1)
		alog(LOG_ERROR, "ERROR: %s", av[0]);

	return MOD_CONT;
}

int denora_event_eob(char *source, int ac, char **av)
{
	Server *s;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	s = server_find(source);
	if (stricmp(s->name, denora->uplink) == 0)
		send_cmd(NULL, "%s EA", p10id);

	update_sync_state(source, SYNC_COMPLETE);
	return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void)
{
	Message *m;
	m = createMessage("ERROR",    denora_event_error);
	addCoreMessage(IRCD,m);
	m = createMessage("NOTICE",   denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("SERVER",   denora_event_server);
	addCoreMessage(IRCD,m);
	m = createMessage("PASS",     denora_event_null);
	addCoreMessage(IRCD,m);

	/* let the p10 tokens begin */

	/* end of burst */
	m = createMessage("EB",       denora_event_eob);
	addCoreMessage(IRCD,m);
	/* nick */
	m = createMessage("N",        denora_event_nick);
	addCoreMessage(IRCD,m);
	/* ping */
	m = createMessage("G",        denora_event_ping);
	addCoreMessage(IRCD,m);
	/* MODE */
	m = createMessage("M",        denora_event_mode);
	addCoreMessage(IRCD,m);
	/* OPMODE */
	m = createMessage("OM",       denora_event_mode);
	addCoreMessage(IRCD,m);
	/* CREATE */
	m = createMessage("C",        denora_event_create);
	addCoreMessage(IRCD,m);
	/* JOIN */
	m = createMessage("J",        denora_event_join);
	addCoreMessage(IRCD,m);
	/* QUIT */
	m = createMessage("Q",        denora_event_quit);
	addCoreMessage(IRCD,m);
	/* TOPIC */
	m = createMessage("T",        denora_event_topic);
	addCoreMessage(IRCD,m);
	/* MOTD */
	m = createMessage("MO",       denora_event_motd);
	addCoreMessage(IRCD,m);
	/* KICK */
	m = createMessage("K",        denora_event_kick);
	addCoreMessage(IRCD,m);
	/* AWAY */
	m = createMessage("A",        denora_event_away);
	addCoreMessage(IRCD,m);
	/* ADMIN */
	m = createMessage("AD",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* PART */
	m = createMessage("L",        denora_event_part);
	addCoreMessage(IRCD,m);
	/* BURST */
	m = createMessage("B",        denora_event_sjoin);
	addCoreMessage(IRCD,m);
	/* PRIVMSG */
	m = createMessage("P",        denora_event_privmsg);
	addCoreMessage(IRCD,m);
	/* NOTICE */
	m = createMessage("O",        denora_event_notice);
	addCoreMessage(IRCD,m);
	/* SQUIT */
	m = createMessage("SQ",       denora_event_squit);
	addCoreMessage(IRCD,m);
	/* INVITE */
	m = createMessage("I",        denora_event_null);
	addCoreMessage(IRCD,m);
	/* DESYNCH */
	m = createMessage("DS",       denora_event_error);
	addCoreMessage(IRCD,m);
	/* WALLOP */
	m = createMessage("WA",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* VERSION */
	m = createMessage("V",        m_version);
	addCoreMessage(IRCD,m);
	/* WHOIS */
	m = createMessage("W",        denora_event_whois);
	addCoreMessage(IRCD,m);
	/* SERVER */
	m = createMessage("S",        denora_event_server);
	addCoreMessage(IRCD,m);
	/* PONG */
	m = createMessage("Z",        denora_event_pong);
	addCoreMessage(IRCD,m);
	/* STATS */
	m = createMessage("R",        m_stats);
	addCoreMessage(IRCD,m);
	/* ACCOUNT */
	m = createMessage("AC",       denora_event_account);
	addCoreMessage(IRCD,m);
	/* KILL */
	m = createMessage("D",	      denora_event_kill);
	addCoreMessage(IRCD,m);
	/* GLINE */
	m = createMessage("GL",       denora_event_gline);
	addCoreMessage(IRCD,m);
	/* JUPE */
	m = createMessage("JU",       denora_event_jupe);
	addCoreMessage(IRCD,m);
	/* INFO */
	m = createMessage("F",        denora_event_null);
	addCoreMessage(IRCD,m);
	/* SETTIME */
	m = createMessage("SE",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* TIME */
	m = createMessage("TI",       m_time);
	addCoreMessage(IRCD,m);
	/* TRACE */
	m = createMessage("TR",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* RPING */
	m = createMessage("RI",       denora_event_rping);
	addCoreMessage(IRCD,m);
	/* RPONG */
	m = createMessage("RO",	      denora_event_rpong);
	addCoreMessage(IRCD,m);
	/* End of Burst Acknowledge */
	m = createMessage("EA",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* SILENCE */
	m = createMessage("U",	      denora_event_null);
	addCoreMessage(IRCD,m);
	/* PRIVS */
	m = createMessage("PRIVS",    denora_event_null);
	addCoreMessage(IRCD,m);
	/* CLEARMODE */
	m = createMessage("CM",       denora_event_clearmode);
	addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

/* <source> GL <targetservermask> [!]<+/-><identmask>@<hostmask> <duration> <timestamp> :<reason>
 * AVAAC GL * +*@something.fake 5 1214347529 :Some fake gline */
int denora_event_gline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	p10_gline((char *) "G", source, ac, av);
	return MOD_CONT;
}

/* <source> JU <targetservermask> [!]<+/-><jupedservername> <duration> <timestamp> :<reason>
 * AVAAC JU * +something.fake 5 1214347612 :Some fake jupe */
int denora_event_jupe(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	p10_gline((char *) "J", source, ac, av);
	return MOD_CONT;
}

void scarynet_cmd_stats(char *sender, const char *letter, char *server)
{
	Uid *ud;
	Server *s;

	ud = find_uid(sender);
	s = server_find(server);

	send_cmd((ud ? ud->uid : sender), "STATS %s :%s", letter,
	         (s ? (s->suid ? s->suid : server) : server));
}

/* PART */
void scarynet_cmd_part(char *nick, char *chan, char *buf)
{
	Uid *ud;
	ud = find_uid(nick);
	if (buf)
	{
		send_cmd((ud ? ud->uid : nick), "L %s :%s", chan, buf);
	}
	else
	{
		send_cmd((ud ? ud->uid : nick), "L %s", chan);
	}
}

/* ABAAB J #ircops 1098031328 */
void scarynet_cmd_join(char *user, char *channel, time_t chantime)
{
	Uid *ud;
	char *modes = NULL;

	ud = find_uid(user);

	if (UplinkSynced)
	{
		send_cmd((ud ? ud->uid : user), "J %s %ld", channel,
		         (long int) chantime);
	}
	else
	{
		if (AutoOp && AutoMode)
		{
			modes = sstrdup(AutoMode);
			modes++;            /* since the first char is +, we skip it */
			send_cmd(p10id, "B %s %ld %s:%s", channel,
			         (long int) chantime, (ud ? ud->uid : user), modes);
		}
		else
		{
			send_cmd(p10id, "B %s %ld %s", channel,
			         (long int) chantime, (ud ? ud->uid : user));
		}
	}
}

void scarynet_cmd_squit(char *servname, char *message)
{
	alog(LOG_PROTOCOL, "Servername %s Message %s", servname, message);
	send_cmd(p10id, "SQ %s :%s", p10id, message);
}

void scarynet_cmd_connect(void)
{

	if (!BadPtr(Numeric))
	{
		inttobase64(p10id, atoi(Numeric), 2);
		me_server =
		    do_server(NULL, ServerName, (char *) "0", ServerDesc, p10id);
		scarynet_cmd_pass(RemotePassword);
		scarynet_cmd_server(ServerName, 1, ServerDesc);
	}
	else
	{
		alog(LOG_NORMAL, "Numeric is required for P10 ircds");
		exit(1);
	}
}

/* RPING */
/* AB RI AN ABAAB 1166048441 214764 :<No client start time> */
int denora_event_rping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	send_cmd(av[0], "RO %s %s %s %s :%s", ServerName, av[1], av[2], av[3],
	         av[4]);
	return MOD_CONT;
}

/* RPONG */
/* AN RO Stats.ScaryNet.Org ABAAA 1166048441 214764 :<No client start time> */
int denora_event_rpong(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	server_store_pong(source, time(NULL));
	return MOD_CONT;
}

int denora_event_pong(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	server_store_pong(source, time(NULL));
	return MOD_CONT;
}

/* PASS */
void scarynet_cmd_pass(char *pass)
{
	send_cmd(NULL, "PASS :%s", pass);
}

void scarynet_cmd_capab()
{
	/* not used by p10 */
}

/* SERVER [SERVERNAME]     [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] :[DESCRIPTION] */
/* SERVER irc.undernet.org 1          933022556    947908144   J10        AA]]]             :[127.0.0.1] A Undernet Server */
void scarynet_cmd_server(char *servname, int hop, char *descript)
{
	send_cmd(NULL, "SERVER %s %d %ld %lu J10 %s]]] +s6 :%s", servname, hop,
	         (long int) denora->start_time, (long int) time(NULL), p10id,
	         descript);
}

/* GLOBOPS */
void scarynet_cmd_global(char *source, char *buf)
{
	Uid *id;

	id = find_uid(source);

	send_cmd((id ? id->uid : ServerName), "WA :%s", buf);
}

/* ABAAC A :I go away */
/* ABAAC A */
int denora_event_away(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	m_away(source, (ac ? av[0] : NULL));
	return MOD_CONT;
}

/* Old style AC */
/* ABAAB AC ANAC] :Trystan */

/* AC With login on connect */
/* ABAAC AC ANAC] R :Trystan */
int denora_event_account(char *source, int ac, char **av)
{
	Server *s;
	User *u;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if ((ac < 2) || !source || !(s = server_find(source)))
		return MOD_CONT;

	u = find_byuid(av[0]);
	if (!u)
		return MOD_CONT;

	if (!strcmp(av[1], "R"))    /* Set */
		do_p10account(u, av[2], 0);
	else if (!strcmp(av[1], "M"))       /* Rename */
		do_p10account(u, av[2], 2);
	else if (!strcmp(av[1], "U"))       /* Remove */
		do_p10account(u, NULL, 1);
	else
		do_p10account(u, av[1], 0);     /* For backward compatability */

	return MOD_CONT;
}

/* During Burst (topicburst with lastts enabled) */
/* [SOURCE] T [CHANNEL] [TIMESTAMP] [LASTSET]  :[TOPIC] */
/* AB       T #channel  1160681786  1162869140 :This is a topic */

/* During Burst (topicburst without lastts enabled) */
/* [SOURCE] T [CHANNEL] [TIMESTAMP] :[TOPIC] */
/* AX       T #channel  1154382905 :This is a topic */

/* By user */
/* [SOURCE] T [CHANNEL] [TIMESTAMP] :[TOPIC] */
/* ABAAC    T #channel  1163489401  :This is a topic */

int denora_event_topic(char *source, int ac, char **av)
{
	Server *s;
	User *u;
	char *newav[5];

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 3)
		return MOD_CONT;

	u = find_byuid(source);
	s = server_find(source);

	newav[0] = sstrdup(av[0]); /* channel */
	newav[1] = (u ? (char *)u->nick : (s ? (char *)s->name : (char *)"Unknown")); /* topic setter */
	newav[2] = sstrdup(av[1]); /* ts */
	newav[3] = sstrdup(av[ac - 1]); /* topic */
	newav[4] = '\0';

	do_topic(4, newav);
	free(newav[0]);
	free(newav[3]);
	free(newav[4]);
	return MOD_CONT;
}

int denora_event_squit(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 2)
		return MOD_CONT;

	do_squit(av[0]);
	return MOD_CONT;
}

/* ABAAB Q :Quit */
int denora_event_quit(char *source, int ac, char **av)
{
	char *killer = NULL;
	char *msg = NULL;
	User *u;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac != 1)
		return MOD_CONT;

	if (scarynet_parse_lkill(av[0]) == 0)
	{
		do_quit(source, ac, av);
	}
	else
	{
		killer = scarynet_lkill_killer(av[0]);
		msg = scarynet_lkill_msg(av[0]);
		u = find_byuid(source);

		if (killer)
			m_kill(killer, (u ? u->nick : source), msg);
		else
			m_kill((u ? u->nick : source), (u ? u->nick : source), msg);

		if (msg)
			free(msg);
	}
	return MOD_CONT;
}

/* User modes */
/* ABAAA M Trystan +wg */

/* Channel modes */
/* ABAAA M #ircops +v ABAAB */
int denora_event_mode(char *source, int ac, char **av)
{
	User *u;
	User *v;
	Server *s;
	char *sender;
	char hhostbuf[255];

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 2)
		return MOD_CONT;

	u = find_byuid(source);

	if (!u)
	{
		sender = sstrdup(source);
	}
	else
	{
		sender = sstrdup(u->nick);
	}

	if (*av[0] == '#' || *av[0] == '&')
	{
		do_cmode(source, ac, av);
	}
	else
	{
		s = server_find(source);
		if (s)
			sender = sstrdup(av[0]);
		do_umode(sender, ac, av);
		if (strcmp(av[1], "x") != -1)
		{
			v = user_find(av[0]);
			if (v->account)
			{
				ircsnprintf(hhostbuf,
				            sizeof(v->account) + sizeof(hhostbuf) + 2,
				            "%s%s%s", HiddenPrefix, v->account,
				            HiddenSuffix);
				change_user_host(v->nick, hhostbuf);
			}
		}
	}
	if (sender)
		free(sender);
	return MOD_CONT;
}

int denora_event_kill(char *source, int ac, char **av)
{
	User *u, *k;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac != 2)
		return MOD_CONT;

	u = find_byuid(source);
	k = find_byuid(av[0]);
	m_kill((u ? u->nick : source), (k ? k->nick : av[0]), av[1]);
	return MOD_CONT;
}

/* ABAAA K #ircops ABAAC :Trystan` */
/* ABAAA K #testchan ABAAB :test */
int denora_event_kick(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	do_p10_kick(source, ac, av);
	return MOD_CONT;
}

/* JOIN - is the same syntax */
/* ABAAB J #ircops 1098031328 */
int denora_event_join(char *source, int ac, char **av)
{
	User *u;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac != 2)
		return MOD_CONT;

	u = find_byuid(source);

	do_join((u ? u->nick : source), ac, av);
	return MOD_CONT;
}

/* CREATE - p10 don't join empty channels the CREATE them */
/* ABAAA C #ircops 1098031328 */
int denora_event_create(char *source, int ac, char **av)
{
	char *newav[3];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	do_join(source, ac, av);

	newav[0] = sstrdup(av[0]);
	newav[1] = sstrdup("+o");
	newav[2] = sstrdup(source);

	do_cmode(source, 3, newav);

	free(newav[0]);
	free(newav[1]);
	free(newav[2]);

	return MOD_CONT;
}

/* BURST joins simlar to SJOIN */
/* AB B #denora 1113091975 +tn ABAAB,ABAAA:o :%*!*@*.aol.com */
/* s  c  0        1         2   3              4 */
/* AB B #test23 1115788230 ABAAB,ABAAA:o */
/* s  c 0         1         2 */
int denora_event_sjoin(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	do_p10_burst(source, ac, av);
	return MOD_CONT;
}

/* ABAAA MO AG */
int denora_event_motd(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	m_motd(source);
	return MOD_CONT;
}

void scarynet_cmd_mode(char *source, char *dest, char *buf)
{
	Channel *c;
	alog(LOG_PROTOCOL, "MODE Source %s, dest %s, buf %s", source, dest,
	     buf);
	if ((c = findchan(dest)))
		send_cmd(p10id, "M %s %s %ld", dest, buf, (long int) c->creation_time);
	else
		send_cmd(p10id, "M %s %s", dest, buf);
}

void scarynet_cmd_notice(char *source, char *dest, char *buf)
{
	User *u;
	Uid *ud;

	u = finduser(dest);
	ud = find_uid(source);

	send_cmd((ud ? ud->uid : source), "O %s :%s", (u ? u->uid : dest),
	         buf);
}

void scarynet_cmd_privmsg(char *source, char *dest, char *buf)
{
	User *u;
	Uid *ud;

	u = finduser(dest);
	ud = find_uid(source);

	send_cmd((ud ? ud->uid : source), "P %s :%s", (u ? u->uid : dest),
	         buf);
}

void scarynet_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void scarynet_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void scarynet_cmd_quit(char *source, char *buf)
{
	Uid *ud;
	ud = find_uid(source);

	if (buf)
	{
		send_cmd((ud ? ud->uid : source), "QUIT :%s", buf);
	}
	else
	{
		send_cmd((ud ? ud->uid : source), "QUIT");
	}
}

/* Local kills handling - DrStein */
int scarynet_parse_lkill(char *message)
{
	const char *localkillmsg = "Killed (";

	/* is it a Local kill message? */
	if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
	        && (strstr(message, localkillmsg) != NULL))
		return 1;
	return 0;
}

char *scarynet_lkill_killer(char *message)
{
	char *buf, *killer = NULL;

	/* Let's get the killer nickname */
	buf = sstrdup(message);
	killer = strtok(buf, " ");
	killer = strtok(NULL, " ");
	killer++;
	if (buf)
	{
		free(buf);
	}
	return killer;
}

/* Killed (*.beirut.com (KILL TESTING)) */
char *scarynet_lkill_msg(char *message)
{
	char *msg = NULL;

	/* Let's get the kill message */
	msg = strchr(message, '(');
	msg = strchr(message, '(');
	msg[strlen(msg) - 2] = '\0';
	msg++;                      /* removes first character '(' */

	return msg;
}

/* [NUMERIC PREFIX] N [NICK] [HOPCOUNT] [TIMESTAMP] [USERNAME] [HOST] <+modes> [BASE64 IP] [NUMERIC] :[USERINFO] */
/* AF N Client1 1 947957573 User userhost.net +oiwg DAqAoB AFAAA :Generic Client. */
void scarynet_cmd_nick(char *nick, char *name, const char *modes)
{
	char nicknumbuf[6];
	send_cmd(p10id, "N %s 1 %ld %s %s %s B]AAAB %sAA%c :%s", nick,
	         (long int) time(NULL), ServiceUser, ServiceHost, modes, p10id,
	         (p10nickcnt + 'A'), name);
	ircsnprintf(nicknumbuf, 6, "%sAA%c", p10id, (p10nickcnt + 'A'));
	new_uid(nick, nicknumbuf);
	p10nickcnt++;
}

/* EVENT: SERVER */
/* [NUMERIC] SERVER [SERVERNAME] [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] [FLAGS] :[DESCRIPTION] */
/* [NUMERIC] SERVER Auska.Nomadirc.net 1 1098025304 1098036331 J10 ABAP] + :Test Server */
/* (AB S trystan.nomadirc.net 2 0 1106520454 P10 ACAP] +h :Test Server) */
int denora_event_server(char *source, int ac, char **av)
{
	Server *s;
	char uplinknum[3];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	*uplinknum = '\0';
	strlcpy(uplinknum, av[5], sizeof(uplinknum));
	if (!denora->uplink)
	{
		denora->uplink = sstrdup(av[0]);
	}
	s = server_find(source);
	do_server((s ? s->name : source), av[0], av[1], av[7], uplinknum);
	return MOD_CONT;
}

/* ABAAA P ADAAB :help */
int denora_event_privmsg(char *source, int ac, char **av)
{
	User *u;
	Uid *id;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac != 2 || *av[0] == '$' || strlen(source) == 2)
		return MOD_CONT;

	u = find_byuid(source);
	id = find_nickuid(av[0]);

	m_privmsg((u ? u->nick : source), (id ? id->nick : av[0]), av[1]);
	return MOD_CONT;
}

/* ABAAA L #ircops */
int denora_event_part(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	do_part(source, ac, av);
	return MOD_CONT;
}

int denora_event_whois(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (source && ac >= 1)
		m_whois(source, av[1]);

	return MOD_CONT;
}

/* AB G !1098031985.558075 services.nomadirc.net 1098031985.558075 */
int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 1)
		return MOD_CONT;

	scarynet_cmd_pong(p10id, av[0]);
	return MOD_CONT;
}

/* PONG */
void scarynet_cmd_pong(char *servname, char *who)
{
	char *t, *s;
	uint32 ts, tsnow, value;
	char *p10time = militime_float(NULL);

	t = myStrGetToken(who, '!', 1);
	s = myStrGetToken(t, '.', 0);
	free(t);
	if (!s)
	{
		ts = 0;
	}
	else
	{
		ts = strtol(s, NULL, 10);
		free(s);
	}
	tsnow = time(NULL);
	value = tsnow - ts;
	if (denora->protocoldebug)
	{
		alog(LOG_PROTOCOL, "PONG: Server Name %s : Who %s", servname, who);
	}
	send_cmd(p10id, "Z %s %ld %ld %ld %s", p10id, (long int) ts,
	         (long int) tsnow, (long int) value, p10time);
	free(s);
	free(t);
	free(p10time);
}

void scarynet_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                           char *modes)
{
	char nicknumbuf[6];
	send_cmd(p10id, "N %s 1 %ld %s %s %s B]AAAB %sAA%c :%s", nick,
	         (long int) time(NULL), user, host, modes, p10id,
	         (p10nickcnt + 'A'), real);
	ircsnprintf(nicknumbuf, 6, "%sAA%c", p10id, (p10nickcnt + 'A'));
	new_uid(nick, nicknumbuf);
	p10nickcnt++;
}

void scarynet_cmd_eob(void)
{
	send_cmd(p10id, "EB");
}

void scarynet_cmd_ping(char *server)
{
	Uid *ud;
	Server *s;
	struct timeval t;
	ud = find_uid(s_StatServ);
	s = server_find(server);
	gettimeofday(&t, NULL);
	send_cmd(p10id, "RI %s %s %ld %ld :<No client start time>",
	         ((s && s->suid) ? s->suid : server),
	         (ud ? ud->uid : s_StatServ), (long int) t.tv_sec, (long int) t.tv_usec);
}

void scarynet_cmd_ctcp(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void scarynet_cmd_version(char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(s_StatServ);
	s = server_find(server);

	send_cmd((ud ? ud->uid : s_StatServ), "V :%s",
	         (s ? (s->suid ? s->suid : server) : server));
}

void scarynet_cmd_motd(char *sender, char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(sender);
	s = server_find(server);

	send_cmd((ud ? ud->uid : sender), "MO :%s",
	         (s ? (s->suid ? s->suid : server) : server));

}

int denora_event_notice(char *source, int ac, char **av)
{
	User *user_s = NULL;
	User *user_r = NULL;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac != 2 || *av[0] == '$' || strlen(source) == 2)
		return MOD_CONT;

	user_s = user_find(source);
	if (*av[0] == '#' && user_s)
	{
		m_notice(user_s->nick, av[0], av[1]);
	}
	else if (user_s)
	{
		user_r = user_find(av[0]);
		if (user_r)
		{
			m_notice(user_s->nick, user_r->nick, av[1]);
		}
	}
	return MOD_CONT;
}

int denora_event_clearmode(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	chan_clearmodes(source, ac, av);
	return MOD_CONT;
}

void moduleAddIRCDCmds()
{
	pmodule_cmd_nick(scarynet_cmd_nick);
	pmodule_cmd_mode(scarynet_cmd_mode);
	pmodule_cmd_bot_nick(scarynet_cmd_bot_nick);
	pmodule_cmd_notice(scarynet_cmd_notice);
	pmodule_cmd_privmsg(scarynet_cmd_privmsg);
	pmodule_cmd_serv_notice(scarynet_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(scarynet_cmd_serv_privmsg);
	pmodule_cmd_quit(scarynet_cmd_quit);
	pmodule_cmd_pong(scarynet_cmd_pong);
	pmodule_cmd_join(scarynet_cmd_join);
	pmodule_cmd_part(scarynet_cmd_part);
	pmodule_cmd_global(scarynet_cmd_global);
	pmodule_cmd_squit(scarynet_cmd_squit);
	pmodule_cmd_connect(scarynet_cmd_connect);
	pmodule_cmd_eob(scarynet_cmd_eob);
	pmodule_cmd_ctcp(scarynet_cmd_ctcp);
	pmodule_cmd_version(scarynet_cmd_version);
	pmodule_cmd_stats(scarynet_cmd_stats);
	pmodule_cmd_motd(scarynet_cmd_motd);
	pmodule_cmd_ping(scarynet_cmd_ping);
}


