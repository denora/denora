/* Nefarious ircu P10 IRCD functions
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
#include "nefarious2.h"

int p10nickcnt = 0;

int DenoraInit(int argc, char **argv)
{
	Directive *dir;

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

	DenoraXMLIRCdConfig("nefarious2.xml");

	moduleAddAuthor("Denora");
	moduleAddVersion(PROTO_VERSION);
	moduleSetType(PROTOCOL);

	pmodule_irc_var(IRC_NEFARIOUS2);

	ModuleChanModeUpdate(CMODE_k, set_key, get_key);
	ModuleChanModeUpdate(CMODE_l, set_limit, get_limit);
	ModuleChanModeUpdate(CMODE_L, set_redirect, get_redirect);
	ModuleChanModeUpdate(CMODE_A, nef2_chan_mode_a_set, nef2_chan_mode_a_get);
	ModuleChanModeUpdate(CMODE_U, nef2_chan_mode_u_set, nef2_chan_mode_u_get);

	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();

	return MOD_CONT;
}


/*************************************************************************/

char *nef2_chan_mode_a_get(Channel * chan)
{

   return chan->akey;
}

void nef2_chan_mode_a_set(Channel * chan, char *value)
{
	struct c_userlist *u;

	if (chan->akey)
	{
		free(chan->akey);
	}
        chan->akey = sstrdup(value);
	if (denora->do_sql)
	{
		sql_query("UPDATE %s SET mode_ua_data='%s' WHERE chanid=%d", ChanTable, value, chan->sqlid);
	}

        u = chan->users;
	if (u)
	{
        	while ((u = u->next))
        	{
				sql_query("UPDATE %s SET oplevel=999 WHERE chanid=%d AND nickid=%d AND mode_lo='Y'", IsOnTable, chan->sqlid, u->user->sqlid);

			}
    }
}


char *nef2_chan_mode_u_get(Channel * chan)
{

   return chan->ukey;
}

void nef2_chan_mode_u_set(Channel * chan, char *value)
{
	if (chan->ukey)
	{
		free(chan->ukey);
	}
        chan->ukey = sstrdup(value);
	if (denora->do_sql)
	{
		sql_query("UPDATE %s SET mode_uu_data='%s' WHERE chanid=%d", ChanTable, value, chan->sqlid);
	}
}

char *nefarious_nickip(char *host)
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
**	    parv[0] = nickname
**      parv[1] = hopcount
**      parv[2] = timestamp
**      parv[3] = username
**      parv[4] = host
**      parv[5] = modes
**      parv[6] = base64 ip
**	    parv[7] = uid
**      parv[8] = info
** NICK - change
**      source  = oldnick
**      parv[0] = new nickname
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
	char *temp;
	char *ipchar;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	temp = sstrdup(source);

	if (ac != 2)
	{
		char *realname, *ip, *nick;
		char *ident, *host, *modes, *modes2;
		const char *uid = "";
		const char *account = "";
		char *fakehost = NULL;
		char *sethost = NULL;
		char *hiddenhost = NULL;
		char *hiddenip = NULL;
		const char *timestamp = "";
		char hhostbuf[255] = "";
		int ishidden = 0, isaccount = 0;

		s = server_find(source);
		*source = '\0';

		realname = sstrdup(av[ac - 1]);
		uid = sstrdup(av[ac - 2]);
		ip = sstrdup(av[ac - 3]);
		nick = sstrdup(av[0]);
		ident = sstrdup(av[3]);
		host = sstrdup(av[4]);
		modes = sstrdup(av[5]);
		modes2 = sstrdup(av[5]);

		if (strpbrk(av[5], "+"))
		{
			int cnt = 6;
			int c = 1;
			char *uaccount = sstrdup("");
			char *acc = NULL;

			while (*modes)
			{
				switch (*modes)
				{
					case 'r':
						isaccount = 1;
						uaccount = av[cnt++];
						for (acc = strtok(uaccount, ":");
						        acc; acc = strtok(NULL, ":"))
						{
							if (c == 1)
								account = sstrdup(acc);
							else if (c == 2)
								timestamp = sstrdup(acc);
							c++;
						}
						c = 1;
						break;
					case 'h':
						sethost = sstrdup(av[cnt++]);
						break;
					case 'f':
						fakehost = sstrdup(av[cnt++]);
						break;
					case 'c':
						hiddenip = sstrdup(av[cnt++]);
						break;
					case 'C':
						hiddenhost = sstrdup(av[cnt++]);
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

		/* do_nick(const char *source, char *nick, char *username, char *host,
		   char *server, char *realname, time_t ts, uint32 svid,
		   uint32 ip, char *vhost, char *uid, int hopcount, char *modes, char *account) */

		ipchar = nefarious_nickip(ip);

		if (ishidden)
		{
			if (hiddenhost)
			{
				ircsnprintf(hhostbuf, sizeof(hhostbuf) + 3, "%s", hiddenhost);
			}
			else if (isaccount)
			{
				ircsnprintf(hhostbuf,
				            sizeof(account) + sizeof(hhostbuf) + 2,
				            "%s%s%s", HiddenPrefix, account, HiddenSuffix);
			}
		}

		user = do_nick(source, nick, ident, host, (s ? s->name : temp),
		               realname, strtoul(av[2], NULL, 10), 0, ipchar,
		               hhostbuf, (char *) uid, strtoul(av[1], NULL, 10),
		               modes, (char *) account);

		if (user)
		{
			if (fakehost || sethost)
			{
				const char *vhost = "";
				if (sethost)
				{
					int h = 1;
					char *uh = NULL;
					const char *vident = "";
					for (uh = strtok(sethost, "@");
					        uh; uh = strtok(NULL, "@"))
					{
						if (h == 1)
							vident = uh;
						else if (h == 2)
							vhost = uh;
						h++;
					}
					h = 1;
					change_user_username(user->nick, (char *) vident);
				}
				change_user_host(user->nick,
				                 sethost ? (char *) vhost : fakehost);
				/* set host as ip */
			}
		}
		free(ipchar);
	}
	else
	{
		/* Nick change */
		user = find_byuid(source);
		do_nick((user ? user->nick : source), av[0], NULL, NULL, NULL,
		        NULL, strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0,
		        NULL, NULL);
	}
	free(temp);
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

int denora_event_eob(char *source, int ac, char **av)
{
	Server *s;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	s = server_find(source);
	if (stricmp(s->name, denora->uplink) == 0)
	{
		send_cmd(NULL, "%s EA", p10id);
	}
	update_sync_state(source, SYNC_COMPLETE);
	return MOD_CONT;
}

int denora_event_account(char *source, int ac, char **av)
{
	Server *s;
	User *u;

	if ((ac < 2) || !source || !(s = server_find(source)))
		return 0;               /* source must be server. */

	u = user_find(av[0]);
	if (!u)
		return 1;               /* A QUIT probably passed the ACCOUNT. */

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

int denora_event_mark(char *source, int ac, char **av)
{
	User *u;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	u = user_find(av[0]);
	if (!u)
		return 1;

	if (!strcmp(av[1], "CVERSION") && strlen(av[2]))
	{
		handle_ctcp_version(u->nick, av[2]);
	}

	return MOD_CONT;
}

int denora_event_swhois(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_swhois(av[0], av[1]);
	return MOD_CONT;
}

int denora_event_setident(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	change_user_username(av[0], av[1]);
	return MOD_CONT;
}

int denora_event_setname(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	change_user_realname(av[0], av[1]);
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
	m = createMessage("ALIST",    denora_event_null);
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
	/* pong */
	m = createMessage("RO",       denora_event_pong);
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
	/* STATS */
	m = createMessage("R",        m_stats);
	addCoreMessage(IRCD,m);
	/* ACCCOUNT */
	m = createMessage("AC",       denora_event_account);
	addCoreMessage(IRCD,m);
	/* KILL */
	m = createMessage("D",        denora_event_kill);
	addCoreMessage(IRCD,m);
	/* GLINE */
	m = createMessage("GL",       denora_event_gline);
	addCoreMessage(IRCD,m);
	/* SHUN */
	m = createMessage("SU",       denora_event_shun);
	addCoreMessage(IRCD,m);
	/* ZLINE */
	m = createMessage("ZL",       denora_event_zline);
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
	m = createMessage("RI",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* SILENCE */
	m = createMessage("U",        denora_event_null);
	addCoreMessage(IRCD,m);
	/* End of Burst Acknowledge */
	m = createMessage("EA",       denora_event_null);
	addCoreMessage(IRCD,m);
	/* FAKEHOST */
	m = createMessage("FA",       denora_event_fakehost);
	addCoreMessage(IRCD,m);
	/* SWHOIS */
	m = createMessage("SW",       denora_event_swhois);
	addCoreMessage(IRCD,m);
	/* MARK */
	m = createMessage("MK",       denora_event_mark);
	addCoreMessage(IRCD,m);
	/* SVSIDENT */
	m = createMessage("SID",      denora_event_setident);
	addCoreMessage(IRCD,m);
	/* SVSINFO */
	m = createMessage("SI",       denora_event_setname);
	addCoreMessage(IRCD,m);
	/* SNO */
	m = createMessage("SNO",      denora_event_null);
	addCoreMessage(IRCD,m);
	/* PRIVS */
	m = createMessage("PRIVS",    denora_event_null);
	addCoreMessage(IRCD,m);
	/* CLEARMODE */
	m = createMessage("CM",       denora_event_clearmode);
	addCoreMessage(IRCD,m);
	/* SPAMFILTER */
	m = createMessage("SF",       denora_event_spamfilter);
	addCoreMessage(IRCD,m);
	/* STATS REPLY */
	m = createMessage("213",      denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("230",      denora_event_null);
	addCoreMessage(IRCD,m);
	m = createMessage("239",      denora_event_spamburst);
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

/* <source> SU <targetservermask> [!]<+/-><identmask>@<hostmask> <duration> <timestamp> :<reason>
 * AVAAC SU * +*@something.fake 5 1214347562 :Some fake shun */
int denora_event_shun(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	p10_gline((char *) "S", source, ac, av);
	return MOD_CONT;
}

/* <source> ZL <targetservermask> [!]<+/-><ipmask> <duration> <timestamp> :<reason>
 * AVAAC ZL * +69.69.69.69 5 1214347639 :Some fake zline */
int denora_event_zline(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	p10_gline((char *) "Z", source, ac, av);
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


void nefarious_cmd_stats(char *sender, const char *letter, char *server)
{
	Uid *ud;
	Server *s;

	ud = find_uid(sender);
	s = server_find(server);

	send_cmd((ud ? ud->uid : sender), "STATS %s :%s", letter,
	         (s ? (s->suid ? s->suid : server) : server));
}

/* PART */
void nefarious_cmd_part(char *nick, char *chan, char *buf)
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
void nefarious_cmd_join(char *user, char *channel, time_t chantime)
{
	Uid *ud;
	char *modes = NULL;

	ud = find_uid(user);

	if (started)
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

void nefarious_cmd_squit(char *servname, char *message)
{
	alog(LOG_PROTOCOL, "Servername %s Message %s", servname, message);
	send_cmd(p10id, "SQ %s :%s", p10id, message);
}

void nefarious_cmd_connect(void)
{

	if (!BadPtr(Numeric))
	{
		inttobase64(p10id, atoi(Numeric), 2);
		me_server =
		    do_server(NULL, ServerName, (char *) "0", ServerDesc, p10id);
		nefarious_cmd_pass(RemotePassword);
		nefarious_cmd_server(ServerName, 1, ServerDesc);
	}
	else
	{
		alog(LOG_NORMAL, "Numeric is required for P10 ircds");
		exit(1);
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

/* PASS */
void nefarious_cmd_pass(char *pass)
{
	send_cmd(NULL, "PASS :%s", pass);
}

void nefarious_cmd_capab()
{
	/* not used by p10 */
}

/* SERVER [SERVERNAME] [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] :[DESCRIPTION] */
/* SERVER irc.undernet.org 1 933022556 947908144 J10 AA]]] :[127.0.0.1] A Undernet Server */
void nefarious_cmd_server(char *servname, int hop, char *descript)
{

	if (SupportOperFlag) {
		send_cmd(NULL, "SERVER %s %d %ld %lu J10 %s]]] +s6o :%s", servname, hop,
		         (long int) denora->start_time, (long int) time(NULL), p10id,
	        	 descript);
	}
	else
	{
		send_cmd(NULL, "SERVER %s %d %ld %lu J10 %s]]] +s6 :%s", servname, hop,
		         (long int) denora->start_time, (long int) time(NULL), p10id,
	        	 descript);
	}
}

/* GLOBOPS */
void nefarious_cmd_global(char *source, char *buf)
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
	{
		protocol_debug(source, ac, av);
	}
	m_away(source, (ac ? av[0] : NULL));
	return MOD_CONT;
}

int denora_event_topic(char *source, int ac, char **av)
{
	char *newav[5];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	if (ac < 4)
		return MOD_CONT;

	newav[0] = av[0];
	newav[1] = av[1];
	newav[2] = av[ac - 2];
	newav[3] = av[ac - 1];
	newav[4] = '\0';

	do_topic(4, newav);
	return MOD_CONT;
}

int denora_event_squit(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
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
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 1)
		return MOD_CONT;

	if (nefarious_parse_lkill(av[0]) == 0)
	{
		do_quit(source, ac, av);
	}
	else
	{
		killer = nefarious_lkill_killer(av[0]);
		msg = nefarious_lkill_msg(av[0]);
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
/* AKAAD M #street -o+b AxC3U *!*@`.users.beirut.com */
/* ABAAA M #test2 +o ABAAB:10 1383565922 */
/* ABAAA M #test +A jobe 1383750513 */
int denora_event_mode(char *source, int ac, char **av)
{
	User *u, *u2;
	Server *s;
	char *sender;
	Channel *c;
	char *sethost = NULL;
	char *uh = NULL;
	const char *vhost = "";
	const char *vident = "";
	int h = 1;
	SQLres *sql_res;
	char **sql_row;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);

	if (ac < 2)
		return MOD_CONT;

	u = find_byuid(source);

	if (!u)
	{
		sender = source;
	}
	else
	{
		sender = u->nick;
	}

	if (*av[0] == '#' || *av[0] == '&')
	{
		do_cmode(source, ac, av);
		if (!strcmp(av[1],"+A"))
		{
			if (denora->do_sql)
			{
				c = findchan(av[0]);
				sql_query("UPDATE %s SET oplevel=0 WHERE chanid=%d AND nickid=%d", IsOnTable, c->sqlid, u->sqlid);
			}
		}
		else if (!strcmp(av[1], "-A"))
		{
			if (denora->do_sql)
			{
				c = findchan(av[0]);
				sql_query("UPDATE %s SET oplevel=NULL WHERE chanid=%d", IsOnTable, c->sqlid);
				sql_query("UPDATE %s SET mode_ua_data='' WHERE chanid=%d", ChanTable, c->sqlid);
			}

		}
		else if (!strcmp(av[1], "+o"))
		{
			if (denora->do_sql)
			{
				u2 = find_byuid(av[2]);
				c = findchan(av[0]);
				sql_query("SELECT oplevel FROM %s WHERE chanid=%d AND nickid=%d", IsOnTable, c->sqlid, u->sqlid);
				sql_res = sql_set_result(sqlcon);
				sql_row = sql_fetch_row(sql_res);
				sql_query("UPDATE %s SET oplevel=%s+1 WHERE chanid = %d AND nickid=%d", IsOnTable, sql_row[0], c->sqlid, u2->sqlid);
				sql_free_result(sql_res);
			}

		}
		else if (!strcmp(av[1], "-o"))
		{
			if (denora->do_sql)
			{
				u2 = find_byuid(av[2]);
				c = findchan(av[0]);
				sql_query("UPDATE %s SET oplevel=NULL WHERE chanid = %d AND nickid=%d", IsOnTable, c->sqlid, u2->sqlid);
			}

		}
		else if (!strcmp(av[1], "-U"))
		{
			if (denora->do_sql)
			{
				c = findchan(av[0]);
				sql_query("UPDATE %s SET mode_uu_data='' WHERE chanid=%d", ChanTable, c->sqlid);
			}

		}
	}

	else
	{
		s = server_find(source);
		if (s)
		{
			sender = av[0];
			do_umode(sender, ac, av);

		}
		else 
		{
			do_umode(sender, ac, av);
			/* Since nefarious sends a parameter with user mode +h, we need this little hack */
			if (ac > 2 && !strcmp(av[1], "+h"))
			{
				sethost = sstrdup(av[2]);
				for (uh = strtok(sethost, "@"); uh; uh = strtok(NULL, "@"))
				{
					if (h == 1)
						vident = uh;
					else if (h == 2)
						vhost = uh;
					h++;
				}
				change_user_username(av[0], (char *) vident);
				change_user_host(av[0], (char *) vhost);
				free(sethost);
			}
		}
	}
	return MOD_CONT;
}

/* 66AAA D AxC5i :defender.beirut.com!Defender (reason) */
int denora_event_kill(char *source, int ac, char **av)
{
	User *u, *k;
	char *msg = NULL;

	if (denora->protocoldebug)
		protocol_debug(source, ac, av);
	if (ac != 2)
		return MOD_CONT;

	msg = strchr(av[1], '(');
	msg[strlen(msg) - 1] = '\0';
	msg++;

	u = find_byuid(source);
	k = find_byuid(av[0]);
	m_kill((u ? u->nick : source), (k ? k->nick : av[0]), msg);
	return MOD_CONT;
}

/* ABAAA K #ircops ABAAC :Trystan` */
/* ABAAA K #testchan ABAAB :test */
int denora_event_kick(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_p10_kick(source, ac, av);
	return MOD_CONT;
}

/* JOIN - is the same syntax */
/* ABAAB J #ircops 1098031328 */
int denora_event_join(char *source, int ac, char **av)
{
	User *u;

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	u = find_byuid(source);

	if (ac != 2)
	{
		return MOD_CONT;
	}

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

	newav[0] = av[0];
	newav[1] = (char *) "+o";
	newav[2] = source;

	do_cmode(source, 3, newav);

	return MOD_CONT;
}

/* BURST joins simlar to SJOIN */
/* AB B #denora 1113091975 +tn ABAAB,ABAAA:o :%*!*@*.aol.com */
/* s  c 0       1          2   3             4 */
/* AB B #test23 1115788230 ABAAB,ABAAA:o */
/* s  c 0       1          2 */
/* AB B #test23 1115788230 :%*!*@*.aol.com */
/* s  c 0       1          2 */
int denora_event_sjoin(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	do_p10_burst(source, ac, av);
	return MOD_CONT;
}

/* ABAAA MO AG */
int denora_event_motd(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	m_motd(source);
	return MOD_CONT;
}

void nefarious_cmd_mode(char *source, char *dest, char *buf)
{
	Channel *c;
	alog(LOG_PROTOCOL, "MODE Source %s, dest %s, buf %s", source, dest,
	     buf);
	if ((c = findchan(dest)))
		send_cmd(p10id, "M %s %s %ld", dest, buf, (long int) c->creation_time);
	else
		send_cmd(p10id, "M %s %s", dest, buf);
}

void nefarious_cmd_notice(char *source, char *dest, char *buf)
{
	User *u;
	Uid *ud;

	u = finduser(dest);
	ud = find_uid(source);

	send_cmd((ud ? ud->uid : source), "O %s :%s", (u ? u->uid : dest),
	         buf);
}

void nefarious_cmd_privmsg(char *source, char *dest, char *buf)
{
	User *u;
	Uid *ud;

	u = finduser(dest);
	ud = find_uid(source);

	send_cmd((ud ? ud->uid : source), "P %s :%s", (u ? u->uid : dest),
	         buf);
}

void nefarious_cmd_serv_notice(char *source, char *dest, char *msg)
{
	send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void nefarious_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void nefarious_cmd_quit(char *source, char *buf)
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
int nefarious_parse_lkill(char *message)
{
	const char *localkillmsg = "Killed (";

	/* is it a Local kill message? */
	if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
	        && (strstr(message, localkillmsg) != NULL))
		return 1;

	return 0;
}

char *nefarious_lkill_killer(char *message)
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
char *nefarious_lkill_msg(char *message)
{
	char *msg = NULL;

	/* Let's get the kill message */
	msg = strchr(message, '(');
	msg++;
	msg = strchr(msg, '(');
	msg[strlen(msg) - 2] = '\0';
	msg++;                      /* removes first character '(' */

	return sstrdup(msg);
}

/* [NUMERIC PREFIX] N [NICK] [HOPCOUNT] [TIMESTAMP] [USERNAME] [HOST] <+modes> [BASE64 IP] [NUMERIC] :[USERINFO] */
/* AF N Client1 1 947957573 User userhost.net +oiwg DAqAoB AFAAA :Generic Client. */
void nefarious_cmd_nick(char *nick, char *name, const char *modes)
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
/* SERVER Auska.Nomadirc.net 1 1098025304 1098036331 J10 ABAP] + :Test Server */
/* SERVER [SERVERNAME] [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] [FLAGS] :[DESCRIPTION] */
/* (AB S trystan.nomadirc.net 2 0 1106520454 P10 ACAP] +h :Test Server) */
int denora_event_server(char *source, int ac, char **av)
{
	Server *s;
	char uplinknum[3];
	*uplinknum = '\0';

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
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
	{
		protocol_debug(source, ac, av);
	}
	u = find_byuid(source);
	id = find_nickuid(av[0]);

	if (ac != 2 || *av[0] == '$' || strlen(source) == 2)
		return MOD_CONT;
	m_privmsg((u ? u->nick : source), (id ? id->nick : av[0]), av[1]);
	return MOD_CONT;
}

/* ABAAA L #ircops */
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
		m_whois(source, av[1]);
	}
	return MOD_CONT;
}

/* AB G !1098031985.558075 services.nomadirc.net 1098031985.558075 */
int denora_event_ping(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (ac < 1)
		return MOD_CONT;
	nefarious_cmd_pong(p10id, av[0]);
	return MOD_CONT;
}

/* PONG */
void nefarious_cmd_pong(char *servname, char *who)
{
	char *t, *s;
	char *p10time = militime_float(NULL);
	uint32 ts, tsnow, value;

	t = myStrGetToken(who, '!', 1);
	s = myStrGetToken(t, '.', 0);
	if (!s)
	{
		ts = 0;
	}
	else
	{
		ts = strtol(s, NULL, 10);
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

void nefarious_cmd_bot_nick(char *nick, char *user, char *host, char *real,
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

void nefarious_cmd_eob(void)
{
	send_cmd(p10id, "EB");
}

void nefarious_cmd_ping(char *server)
{
	/* AB G !1115872042.64217 denora.nomadirc.net 1115872042.64217
	 * [OUT]: AB RI AL ABAAB 1165972073 45741 :<No client start time>
	 * [IN ]: AL RO ScaryNet.Org ABAAB 1165972073 45741 :<No client start time>
	 */

	Uid *ud;
	Server *s;
	struct timeval t;
	ud = find_uid(s_StatServ);
	s = server_find(server);
	gettimeofday(&t, NULL);

	send_cmd(p10id, "RI %s %s %ld %ld :<No client start time>",
	         ((s
	           && s->suid) ? s->suid : server),
	         (ud ? ud->uid : s_StatServ), (long int) t.tv_sec, (long int) t.tv_usec);
}

void nefarious_cmd_ctcp(char *source, char *dest, char *buf)
{
	send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void nefarious_cmd_version(char *server)
{
	Uid *ud;
	Server *s;
	ud = find_uid(s_StatServ);
	s = server_find(server);

	send_cmd((ud ? ud->uid : s_StatServ), "V :%s",
	         (s ? (s->suid ? s->suid : server) : server));
}

void nefarious_cmd_motd(char *sender, char *server)
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
	{
		protocol_debug(source, ac, av);
	}
	if (ac != 2 || *av[0] == '$' || strlen(source) == 2)
	{
		return MOD_CONT;
	}

	user_s = user_find(source);
	if ((*av[0] == '#' || *av[0] == '&') && (user_s))
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
int denora_event_fakehost(char *source, int ac, char **av)
{
	User *ud;
	char *parv[2];

        if (denora->protocoldebug)
        {
                protocol_debug(source, ac, av);
        }

	ud = user_find(av[0]);

	if (ud)
	{
		parv[0] = (char *) ud->nick;
		parv[1] = (char *) "+f";
		do_umode(ud->nick, 2, parv);
	}

	change_user_host(ud->nick, av[1]);
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

/*
 * IBACj SF IBACj + n S 86400 No_reason_really :^$
 *          0     1 2 3 4     5                 6
 */

int denora_event_spamfilter(char *source, int ac, char **av)
{
	User * u;
	char setby[BUFSIZE];
	char setat[10];

	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	u = user_find(av[0]);

	if (!stricmp(av[1], "+") && u)
	{
		/* (char *target, char *action, char *setby, char *expires, char *setat,
		    char *duration, char *reason, char *regex) */
		ircsnprintf(setby, BUFSIZE, "%s!%s@%s", u->nick, u->username, u->vhost ? u->vhost : u->host);
		ircsnprintf(setat, 10, "%ld", (long int) time(NULL));
		sql_do_server_spam_add(av[2], av[3], setby, (char *)"0", setat, av[4], av[ac-2], av[ac-1]);
	}
	else if (!stricmp(av[1], "-"))
	{
		/* (char *target, char *action, char *regex) */
		sql_do_server_spam_remove(av[2], av[3], av[ac-1]);
	}

	return MOD_CONT;
}

int denora_event_spamburst(char *source, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}

	return MOD_CONT;
}

void moduleAddIRCDCmds()
{
	pmodule_cmd_nick(nefarious_cmd_nick);
	pmodule_cmd_mode(nefarious_cmd_mode);
	pmodule_cmd_bot_nick(nefarious_cmd_bot_nick);
	pmodule_cmd_notice(nefarious_cmd_notice);
	pmodule_cmd_privmsg(nefarious_cmd_privmsg);
	pmodule_cmd_serv_notice(nefarious_cmd_serv_notice);
	pmodule_cmd_serv_privmsg(nefarious_cmd_serv_privmsg);
	pmodule_cmd_quit(nefarious_cmd_quit);
	pmodule_cmd_pong(nefarious_cmd_pong);
	pmodule_cmd_join(nefarious_cmd_join);
	pmodule_cmd_part(nefarious_cmd_part);
	pmodule_cmd_global(nefarious_cmd_global);
	pmodule_cmd_squit(nefarious_cmd_squit);
	pmodule_cmd_connect(nefarious_cmd_connect);
	pmodule_cmd_eob(nefarious_cmd_eob);
	pmodule_cmd_ctcp(nefarious_cmd_ctcp);
	pmodule_cmd_version(nefarious_cmd_version);
	pmodule_cmd_stats(nefarious_cmd_stats);
	pmodule_cmd_motd(nefarious_cmd_motd);
	pmodule_cmd_ping(nefarious_cmd_ping);
}

