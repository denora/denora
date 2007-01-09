/* PTLink IRCD functions
 *
 * (C) 2004-2007 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 * last tested against Hybrid6/PTlink6.19.4 (20050827_0)
 *
 */

#include "denora.h"
#include "ptlink.h"

IRCDVar myIrcd[] = {
    {"PTlink 6.15.*+",          /* ircd name                    */
     "+io",                     /* StatServ mode                */
     IRCD_ENABLE,               /* Vhost                        */
     IRCD_ENABLE,               /* Supports SGlines             */
     IRCD_DISABLE,              /* sgline sql table             */
     IRCD_ENABLE,               /* Supports SQlines             */
     IRCD_DISABLE,              /* sqline sql table             */
     IRCD_ENABLE,               /* Supports SZlines             */
     IRCD_ENABLE,               /* Has exceptions +e            */
     IRCD_ENABLE,               /* vidents                      */
     IRCD_DISABLE,              /* NICKIP                       */
     IRCD_ENABLE,               /* VHOST ON NICK                */
     IRCD_ENABLE,               /* +f                           */
     IRCD_DISABLE,              /* +j                           */
     IRCD_DISABLE,              /* +L                           */
     CMODE_f,                   /* +f Mode                      */
     IRCD_DISABLE,              /* +j                           */
     IRCD_DISABLE,              /* +L Mode                      */
     NULL,                      /* CAPAB Chan Modes             */
     IRCD_DISABLE,              /* We support TOKENS            */
     IRCD_ENABLE,               /* TOKENS are CASE Sensitive    */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64       */
     IRCD_DISABLE,              /* +I support                   */
     IRCD_DISABLE,              /* SJOIN ban char               */
     IRCD_DISABLE,              /* SJOIN except char            */
     IRCD_DISABLE,              /* SJOIN invite char            */
     UMODE_o,                   /* umode for vhost              */
     IRCD_DISABLE,              /* owner                        */
     IRCD_ENABLE,               /* protect                      */
     IRCD_ENABLE,               /* halfop                       */
     NULL,                      /* user modes                   */
     NULL,                      /* channel modes                */
     'f',                       /* flood                        */
     IRCD_DISABLE,              /* flood other                  */
     'o',                       /* vhost                        */
     IRCD_DISABLE,              /* vhost other                  */
     IRCD_DISABLE,              /* channek linking              */
     IRCD_DISABLE,              /* p10                          */
     IRCD_DISABLE,              /* ts6                          */
     IRCD_ENABLE,               /* numeric ie.. 350 etc         */
     IRCD_DISABLE,              /* channel mode gagged          */
     IRCD_DISABLE,              /* spamfilter                   */
     'b',                       /* ban char                     */
     'e',                       /* except char                  */
     'I',                       /* invite char                  */
     IRCD_DISABLE,              /* zip                          */
     IRCD_DISABLE,              /* ssl                          */
     IRCD_ENABLE,               /* uline                        */
     NULL,                      /* nickchar                     */
     IRCD_DISABLE,              /* svid                         */
     IRCD_DISABLE,              /* hidden oper                  */
     IRCD_ENABLE,               /* extra warning             */
     IRCD_DISABLE               /* Report sync state         */
     }
    ,
};

IRCDCAPAB myIrcdcap[] = {
    {
     0,                         /* NOQUIT       */
     0,                         /* TSMODE       */
     0,                         /* UNCONNECT    */
     0,                         /* NICKIP       */
     0,                         /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
     0,                         /* BURST        */
     0,                         /* TS5          */
     0,                         /* TS3          */
     0,                         /* DKEY         */
     CAPAB_PT4,                 /* PT4          */
     CAPAB_SCS,                 /* SCS          */
     CAPAB_QS,                  /* QS           */
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
     0, 0, 0}
};


/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_B, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_H, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_N, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_T, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_v, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_y, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_z, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);

    /* Channel Modes */
    CreateChanMode(CMODE_A, NULL, NULL);
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
    CreateChanMode(CMODE_f, set_flood, get_flood);
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_k, set_key, get_key);
    CreateChanMode(CMODE_l, set_limit, get_limit);
    CreateChanMode(CMODE_m, NULL, NULL);
    CreateChanMode(CMODE_n, NULL, NULL);
    CreateChanMode(CMODE_p, NULL, NULL);
    CreateChanMode(CMODE_r, NULL, NULL);
    CreateChanMode(CMODE_s, NULL, NULL);
    CreateChanMode(CMODE_t, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleSetChanUMode('.', 'a', STATUS_PROTECTED);

    ModuleUpdateSQLChanMode();

}

/*
  :%s SJOIN %lu %s %s %s :%s
	parv[0] = sender
	parv[1] = channel TS (channel creation time)
	parv[2] = channel
	parv[3] = modes + n arguments (key and/or limit) 
	... [n]  = if(key and/or limit) mode arguments
	parv[4+n] = flags+nick list (all in one parameter)
	NOTE: ignore channel modes if we already have the channel with a gr
*/
int denora_event_sjoin(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_sjoin(source, ac, av);
    return MOD_CONT;
}

/*
  :%s NEWMASK %s
	parv[0] = sender
	parv[1] = new mask (if no '@', hostname is assumed)
*/
int denora_event_newmask(char *source, int ac, char **av)
{
    char *newhost;
    char *newuser;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1) {
        return MOD_CONT;
    }
    newuser = myStrGetOnlyToken(av[0], '@', 0);
    if (newuser) {
        newhost = myStrGetTokenRemainder(av[0], '@', 1);
        change_user_username(source, newuser);
        free(newuser);
    } else {
        newhost = av[0];
    }

    if (*newhost == '@')
        newhost++;

    if (newhost) {
        change_user_host(source, newhost);
    }

    return MOD_CONT;
}

/*
 NICK %s %d %lu %s %s %s %s %s :%s
	parv[0] = nickname
	parv[1] = hopcount 
	parv[2] = nick TS (nick introduction time)
	parv[3] = umodes
    parv[4] = username
    parv[5] = hostname
    parv[6] = spoofed hostname
    parv[7] = server
    parv[8] = nick info
*/
/*
 Change NICK
	parv[0] = old nick
	parv[1] = new nick
	parv[2] = TS (timestamp from user's server when nick changed was received)
*/
/*
 NICK xpto 2 561264 +rw irc num.myisp.pt mask.myisp.pt uc.ptlink.net :Just me
       0   1  2      3   4   5            6              7             8

*/
int denora_event_nick(char *source, int ac, char **av)
{
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 2) {
        ipchar = host_resolve(av[5]);
        user = do_nick(source, av[0], av[4], av[5], av[7], av[8],
                       strtoul(av[2], NULL, 10), 0, ipchar, av[6], NULL,
                       strtoul(av[1], NULL, 10), av[3]);
        free(ipchar);
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL);
    }
    return MOD_CONT;
}

/*
  :%s SERVER %s %d %s :%s
	parv[0] = server from where the server was introduced to us 
  	parv[1] = server name
	parv[2] = hop count (1 wen are directly connected)
	parv[3] = server version
	parv[4] = server description
*/
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        do_server(source, av[0], av[1], av[3], NULL);
    } else {
        do_server(source, av[0], av[1], av[3], NULL);
    }
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
void moduleAddIRCDMsgs(void) {
    Message *m;
    
    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("PING",      denora_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   denora_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("USER",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     denora_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("AKILL",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GNOTICE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GOPER",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RAKILL",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("NEWMASK",   denora_event_newmask); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB", 	   denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    denora_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("SVSINFO",   denora_event_svsinfo); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     denora_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("NJOIN",     denora_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("NNICK",     denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("ZLINE",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNZLINE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CHGHOST",   denora_event_chghost); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

int denora_event_chghost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_svsinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    return MOD_CONT;
}

int denora_event_svinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
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

void ptlink_cmd_join(char *user, char *channel, time_t chantime)
{
    send_cmd(ServerName, "SJOIN %ld %s + :%s", (long int) chantime,
             channel, user);
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

void ptlink_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* PONG */
void ptlink_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

void ptlink_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

/*
  SVINFO %d %d
	parv[0] = server name
	parv[1] = current supported protocol version
	parv[2] = minimum supported protocol version

  See the ptlink.h for information on PTLINK_TS_CURRENT, and
  PTLINK_TS_MIN
*/
void ptlink_cmd_svinfo(void)
{
#if defined(PTLINK_TS_CURRENT) && defined(PTLINK_TS_MIN)
    send_cmd(NULL, "SVINFO %d %d %lu", PTLINK_TS_CURRENT, PTLINK_TS_MIN,
             (unsigned long int) time(NULL));
#else
    /* hardwired if the defs some how go missing */
    send_cmd(NULL, "SVINFO 6 3 %lu", (unsigned long int) time(NULL));
#endif
}

/*
  SVSINFO %lu %d
  	parv[0] = sender (server name)
	parv[1] = local services data TS
	parv[1] = max global users
*/
void ptlink_cmd_svsinfo(void)
{
    send_cmd(NULL, "SVSINFO %lu %d", (unsigned long int) time(NULL),
             stats->users_max);
}

/*
  PASS %s :TS
	parv[1] = connection password 
	(TS indicates this is server uses TS protocol and SVINFO will be sent 
	for protocol compatibility checking)
*/
void ptlink_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

/*
  CAPAB :%s
	parv[1] = capability list 
*/
void ptlink_cmd_capab(void)
{
    send_cmd(NULL, "CAPAB :QS PTS4");
}

void ptlink_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d Denora.Stats%s :%s", servname, hop,
             denora->versiondotted, descript);
}

void ptlink_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    ptlink_cmd_pass(RemotePassword);
    ptlink_cmd_capab();
    ptlink_cmd_server(ServerName, 1, ServerDesc);
    ptlink_cmd_svinfo();
    ptlink_cmd_svsinfo();
}

int denora_event_privmsg(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2)
        return MOD_CONT;
    m_privmsg(source, av[0], av[1]);
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

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
    return MOD_CONT;
}

int denora_event_whois(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (source && ac >= 1) {
        m_whois(source, av[0]);
    }
    return MOD_CONT;
}

/*
  :%s TOPIC %s %s %lu :%s
	parv[0] = sender prefix
	parv[1] = channel
	parv[2] = topic nick
    parv[3] = topic time
    parv[4] = topic text
*/
int denora_event_topic(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_topic(ac, av);
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
    if (ac != 1)
        return MOD_CONT;
    do_quit(source, ac, av);
    return MOD_CONT;
}

/*
  :%s MODE %s :%s
	parv[0] = sender
	parv[1] = target nick (==sender)
	parv[2] = mode change string
*/
int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
    return MOD_CONT;
}

int denora_event_kill(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2)
        return MOD_CONT;

    m_kill(source, av[0], av[1]);
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

int denora_event_join(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_join(source, ac, av);
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

void ptlink_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void ptlink_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void ptlink_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void ptlink_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* GLOBOPS */
void ptlink_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

void ptlink_cmd_mode(char *source, char *dest, char *buf)
{
    send_cmd(source, "MODE %s %s", dest, buf);
}

/*
 NICK %s %d %lu %s %s %s %s %s :%s
	parv[1] = nickname
	parv[2] = hopcount 
	parv[3] = nick TS (nick introduction time)
	parv[4] = umodes
    parv[5] = username
    parv[6] = hostname
    parv[7] = spoofed hostname
    parv[8] = server
    parv[9] = nick info
*/
void ptlink_cmd_nick(char *nick, char *name, const char *mode)
{
    send_cmd(NULL, "NICK %s 1 %lu %s %s %s %s %s :%s", nick,
             (unsigned long int) time(NULL), mode, ServiceUser,
             ServiceHost, ServiceHost, ServerName, name);
}

/* QUIT */
void ptlink_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

void ptlink_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    ptlink_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_away(source, (ac ? av[0] : NULL));
    return MOD_CONT;
}

void ptlink_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                         char *modes)
{
    send_cmd(NULL, "NICK %s 1 %lu %s %s %s %s %s :%s", nick,
             (unsigned long int) time(NULL), modes, user, host, host,
             ServerName, real);
}

void ptlink_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
}

void ptlink_cmd_motd(char *sender, char *server)
{
    send_cmd(sender, "MOTD %s", server);
}

int denora_event_notice(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }
    m_notice(source, av[0], av[1]);
    return MOD_CONT;
}

void ptlink_cmd_eob(void)
{
    /* not supported  */
}

void ptlink_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

void ptlink_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(ptlink_cmd_nick);
    pmodule_cmd_mode(ptlink_cmd_mode);
    pmodule_cmd_bot_nick(ptlink_cmd_bot_nick);
    pmodule_cmd_notice(ptlink_cmd_notice);
    pmodule_cmd_privmsg(ptlink_cmd_privmsg);
    pmodule_cmd_serv_notice(ptlink_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(ptlink_cmd_serv_privmsg);
    pmodule_cmd_quit(ptlink_cmd_quit);
    pmodule_cmd_pong(ptlink_cmd_pong);
    pmodule_cmd_join(ptlink_cmd_join);
    pmodule_cmd_part(ptlink_cmd_part);
    pmodule_cmd_global(ptlink_cmd_global);
    pmodule_cmd_squit(ptlink_cmd_squit);
    pmodule_cmd_connect(ptlink_cmd_connect);
    pmodule_cmd_eob(ptlink_cmd_eob);
    pmodule_cmd_ctcp(ptlink_cmd_ctcp);
    pmodule_cmd_version(ptlink_cmd_version);
    pmodule_cmd_stats(ptlink_cmd_stats);
    pmodule_cmd_motd(ptlink_cmd_motd);
    pmodule_cmd_ping(ptlink_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion("$Id$");
    moduleSetType(PROTOCOL);


    pmodule_ircd_version("PTlink 6.15.*+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    pmodule_irc_var(IRC_PTLINK);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    UplinkSynced = 1;
    return MOD_CONT;
}
