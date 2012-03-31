/* Rage IRCD functions
 *
 * (C) 2004-2007 Denora Team
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
#include "rageircd.h"

IRCDVar myIrcd[] = {
    {"RageIRCd 2.0.*",          /* ircd name                 */
     "+di",                     /* StatServ mode             */
     IRCD_ENABLE,               /* Vhost                     */
     IRCD_ENABLE,               /* Supports SGlines          */
     IRCD_ENABLE,               /* sgline sql table          */
     IRCD_ENABLE,               /* Supports SQlines          */
     IRCD_ENABLE,               /* sqline sql table          */
     IRCD_ENABLE,               /* Supports SZlines          */
     IRCD_ENABLE,               /* Has exceptions +e         */
     IRCD_DISABLE,              /* vidents                   */
     IRCD_ENABLE,               /* NICKIP                    */
     IRCD_ENABLE,               /* VHOST ON NICK             */
     IRCD_DISABLE,              /* +f                        */
     IRCD_DISABLE,              /* +j                        */
     IRCD_DISABLE,              /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j Mode                   */
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
     IRCD_DISABLE,              /* owner                     */
     IRCD_ENABLE,               /* protect                   */
     IRCD_ENABLE,               /* halfop                    */
     NULL,                      /* User modes                */
     NULL,                      /* Channel modes             */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     'x',                       /* vhost                     */
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
     IRCD_ENABLE                /* Report sync state         */
     }
    ,
};

IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     CAPAB_TSMODE,              /* TSMODE       */
     CAPAB_UNCONNECT,           /* UNCONNECT    */
     0,                         /* NICKIP       */
     0,                         /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
     CAPAB_BURST,               /* BURST        */
     0,                         /* TS5          */
     0,                         /* TS3          */
     CAPAB_DKEY,                /* DKEY         */
     0,                         /* PT4          */
     0,                         /* SCS          */
     0,                         /* QS           */
     CAPAB_UID,                 /* UID          */
     0,                         /* KNOCK        */
     0,                         /* CLIENT       */
     0,                         /* IPV6         */
     0,                         /* SSJ5         */
     CAPAB_SN2,                 /* SN2          */
     CAPAB_TOKEN,               /* TOKEN        */
     CAPAB_VHOST,               /* VHOST        */
     CAPAB_SSJ3,                /* SSJ3         */
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
    ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_F, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_N, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
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
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_y, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_z, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
    CreateChanMode(CMODE_A, NULL, NULL);
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_M, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_Q, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
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
    ModuleSetChanUMode('*', 'a', STATUS_PROTECTED);

    ModuleUpdateSQLChanMode();

}

int denora_event_sjoin(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_sjoin(source, ac, av);
    return MOD_CONT;
}

char *rage_nickip(char *host)
{
    struct in_addr addr;

    addr.s_addr = htonl(strtoul(host, NULL, 0));
    return sstrdup(inet_ntoa(addr));
}

int denora_event_nick(char *source, int ac, char **av)
{
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        ipchar = rage_nickip(av[8]);
        user = do_nick(source, av[0], av[4], av[5], av[6], av[9],
                       strtoul(av[2], NULL, 10), strtoul(av[7], NULL, 0),
                       ipchar, NULL, NULL, strtoul(av[1], NULL, 0), av[3],
                       NULL);
        free(ipchar);
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
                NULL);
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

void rageircd_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

int denora_event_vhost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

/*
** SNICK
**      source  = NULL       
**	parv[0] = nickname     Trystan
**      parv[1] = timestamp    1090113640
**      parv[2] = hops         1
**      parv[3] = username     Trystan
**	parv[4] = host         c-24-2-101-227.client.comcast.net
**      parv[5] = IP           402810339
**      parv[6] = vhost        mynet-27CCA80D.client.comcast.net
**      parv[7] = server       rage2.nomadirc.net
**	parv[8] = servicestamp 0
**      parv[9] = modes      +ix
** 	parv[10] = info      Dreams are answers to questions not yet asked
*/

int denora_event_snick(char *source, int ac, char **av)
{
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 2) {
        ipchar = rage_nickip(av[5]);
        user = do_nick(source, av[0], av[3], av[4], av[7], av[10],
                       strtoul(av[1], NULL, 10), strtoul(av[8], NULL, 0),
                       ipchar, av[6], NULL, strtoul(av[2],
                                                    NULL, 0), av[9], NULL);
        free(ipchar);
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
    
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
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
    m = createMessage("GOPER",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RAKILL",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB", 	   denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("CS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("HS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("MS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("NS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("OS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SGLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     denora_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("SS",        denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    denora_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("SZLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSGLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSZLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SNICK",	   denora_event_snick); addCoreMessage(IRCD,m);
    m = createMessage("VHOST",	   denora_event_vhost); addCoreMessage(IRCD,m);
    m = createMessage("MYID",	   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GNOTICE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("BURST",     denora_event_burst); addCoreMessage(IRCD,m);
    m = createMessage("NBURST",    denora_event_nburst); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RESYNCH",   denora_event_null); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

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

int denora_event_burst(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    return MOD_CONT;
}

int denora_event_nburst(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    update_sync_state(source, SYNC_COMPLETE);
    return MOD_CONT;
}

/* PART */
void rageircd_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

void rageircd_cmd_join(char *user, char *channel, time_t chantime)
{
    if (started) {
        send_cmd(ServerName, "SJOIN %ld %s + :%s", (long int) chantime,
                 channel, user);
    } else {
        send_cmd(ServerName, "SJOIN %ld %s +nt :%s", (long int) chantime,
                 channel, user);
    }
}

void rageircd_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* PONG */
void rageircd_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

void rageircd_cmd_svinfo(void)
{
    send_cmd(NULL, "SVINFO 5 3 0 %ld bluemoon 0", (long int) time(NULL));
}

void rageircd_cmd_capab(void)
{
    /*  CAPAB BURST UNCONNECT ZIP SSJ3 SN2 VHOST SUID TOK1 TSMODE */
    send_cmd(NULL, "CAPAB BURST UNCONNECT SSJ3 SN2 VHOST TSMODE NBURST");
}

void rageircd_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/* PASS */
void rageircd_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

void rageircd_cmd_burst(void)
{
    send_cmd(NULL, "BURST");
}

void rageircd_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    rageircd_cmd_pass(RemotePassword);
    rageircd_cmd_capab();
    if (Numeric) {
        send_cmd(NULL, "MYID !%s", Numeric);
    }
    rageircd_cmd_server(ServerName, 1, ServerDesc);
    rageircd_cmd_svinfo();
    rageircd_cmd_burst();
}

void rageircd_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
}

void rageircd_cmd_motd(char *sender, char *server)
{
    send_cmd(sender, "MOTD %s", server);
}

/* GLOBOPS */
void rageircd_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

void rageircd_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void rageircd_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void rageircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void rageircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_away(source, (ac ? av[0] : NULL));
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

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    rageircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

void rageircd_cmd_mode(char *source, char *dest, char *buf)
{
    if (ircdcap->tsmode) {
        if (denora->capab & ircdcap->tsmode || UseTSMODE) {
            send_cmd(source, "MODE %s 0 %s", dest, buf);
        } else {
            send_cmd(source, "MODE %s %s", dest, buf);
        }
    } else {
        send_cmd(source, "MODE %s %s", dest, buf);
    }
}

void rageircd_cmd_nick(char *nick, char *name, const char *modes)
{
    send_cmd(NULL, "SNICK %s %ld 1 %s %s 0 * %s 0 %s :%s", nick,
             (long int) time(NULL), ServiceUser, ServiceHost, ServerName,
             modes, name);
}

/* QUIT */
void rageircd_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

void rageircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                           char *modes)
{
    send_cmd(NULL, "SNICK %s %ld 1 %s %s 0 * %s 0 %s :%s", nick,
             (long int) time(NULL), user, host, ServerName, modes, real);
}

int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        do_server(source, av[0], av[1], av[2], NULL);
        return MOD_CONT;
    }
    do_server(source, av[0], av[1], av[2], NULL);
    return MOD_CONT;
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

int denora_event_topic(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 4)
        return MOD_CONT;
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

int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 2) {
        return MOD_CONT;
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
    if (ac != 1) {
        return MOD_CONT;
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

/*
 * SVINFO
 *	parv[0] = sender prefix
 *
 *	if (parc == 2)
 *		parv[1] = ZIP (compression initialisation)
 *
 *	if (parc > 2)
 *		parv[1] = TS_CURRENT
 *		parv[2] = TS_MIN
 *		parv[3] = standalone or connected to non-TS (unused)
 *		parv[4] = UTC time
 *		parv[5] = ircd codename
 *		parv[6] = masking keys 
 */
int denora_event_svinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

void rageircd_cmd_eob(void)
{
    send_cmd(NULL, "BURST 0");
}

void rageircd_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

void rageircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(rageircd_cmd_nick);
    pmodule_cmd_mode(rageircd_cmd_mode);
    pmodule_cmd_bot_nick(rageircd_cmd_bot_nick);
    pmodule_cmd_notice(rageircd_cmd_notice);
    pmodule_cmd_privmsg(rageircd_cmd_privmsg);
    pmodule_cmd_serv_notice(rageircd_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(rageircd_cmd_serv_privmsg);
    pmodule_cmd_quit(rageircd_cmd_quit);
    pmodule_cmd_pong(rageircd_cmd_pong);
    pmodule_cmd_join(rageircd_cmd_join);
    pmodule_cmd_part(rageircd_cmd_part);
    pmodule_cmd_global(rageircd_cmd_global);
    pmodule_cmd_squit(rageircd_cmd_squit);
    pmodule_cmd_connect(rageircd_cmd_connect);
    pmodule_cmd_eob(rageircd_cmd_eob);
    pmodule_cmd_ctcp(rageircd_cmd_ctcp);
    pmodule_cmd_version(rageircd_cmd_version);
    pmodule_cmd_stats(rageircd_cmd_stats);
    pmodule_cmd_motd(rageircd_cmd_motd);
    pmodule_cmd_ping(rageircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion("");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("RageIRCd 2.0.x");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(1);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_RAGEIRCD);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    return MOD_CONT;
}
