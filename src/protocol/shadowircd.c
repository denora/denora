/* ShadowIRCD functions
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
 */

#include "denora.h"
#include "shadowircd.h"

/* 6 slot array, 35 possible combinations per slot, exponential. */
int ts6nickcount[6] = { 0, 0, 0, 0, 0, 0 };

IRCDVar myIrcd[] = {
    {"ShadowIRCd 4.0+",         /* ircd name                 */
     "+oiqSK",                  /* StatServ mode             */
     IRCD_ENABLE,               /* Vhost                     */
     IRCD_DISABLE,              /* Supports SGlines          */
     IRCD_DISABLE,              /* sgline sql table          */
     IRCD_ENABLE,               /* Supports SQlines          */
     IRCD_DISABLE,              /* sqline sql table          */
     IRCD_DISABLE,              /* Supports SZlines          */
     IRCD_ENABLE,               /* Has exceptions +e         */
     IRCD_ENABLE,               /* vidents                   */
     IRCD_DISABLE,              /* NICKIP                    */
     IRCD_ENABLE,               /* VHOST ON NICK             */
     IRCD_DISABLE,              /* +f                        */
     IRCD_DISABLE,              /* +j                        */
     IRCD_DISABLE,              /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j mode                   */
     IRCD_DISABLE,              /* +L Mode                   */
     NULL,                      /* CAPAB Chan Modes          */
     IRCD_DISABLE,              /* We support Unreal TOKENS  */
     IRCD_ENABLE,               /* TOKENS are CASE Sensitive */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
     IRCD_ENABLE,               /* +I support                */
     IRCD_DISABLE,              /* SJOIN ban char            */
     IRCD_DISABLE,              /* SJOIN except char         */
     IRCD_DISABLE,              /* SJOIN invite char         */
     UMODE_v,                   /* umode for vhost           */
     IRCD_DISABLE,              /* owner                     */
     IRCD_DISABLE,              /* protect                   */
     IRCD_ENABLE,               /* halfop                    */
     NULL,                      /* user modes                */
     NULL,                      /* Channel modes      */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     'v',                       /* vhost                     */
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
     IRCD_DISABLE,              /* ssl                       */
     IRCD_DISABLE,              /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_DISABLE,              /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_ENABLE,               /* extra warning             */
     IRCD_ENABLE                /* Report sync state         */
     },
};

/* ShadowIRCd does not use CAPAB */
IRCDCAPAB myIrcdcap[] = {
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
     0, 0, 0}
};



/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_C, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_E, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_H, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_b, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_e, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_l, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_u, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_v, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
    CreateChanMode(CMODE_A, NULL, NULL);
    CreateChanMode(CMODE_E, NULL, NULL);
    CreateChanMode(CMODE_F, NULL, NULL);
    CreateChanMode(CMODE_G, NULL, NULL);
    CreateChanMode(CMODE_K, NULL, NULL);
    CreateChanMode(CMODE_L, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_P, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_T, NULL, NULL);
    CreateChanMode(CMODE_V, NULL, NULL);
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
    CreateChanMode(CMODE_z, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleSetChanUMode('!', 'a', STATUS_PROTECTED);

    ModuleUpdateSQLChanMode();

}

void shadowircd_cmd_stats(char *sender, const char *letter, char *server)
{
    Uid *ud;
    ud = find_uid(sender);

    send_cmd((ud ? ud->uid : sender), "STATS %s %s", letter, server);
}

void shadowircd_cmd_notice(char *source, char *dest, char *buf)
{
    Uid *ud;
    User *u;

    ud = find_uid(source);
    u = finduser(dest);
    send_cmd((ud ? ud->uid : source), "NOTICE %s :%s",
             (u ? u->uid : dest), buf);
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

void shadowircd_cmd_global(char *source, char *buf)
{
    Uid *u;

    if (source) {
        u = find_uid(source);
        send_cmd((u ? u->uid : TS6SID), "OPERWALL :%s", buf);
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
   Non TS6

   av[0] = nick
   av[1] = hop
   av[2] = ts
   av[3] = modes
   av[4] = user
   av[5] = host
   av[6] = server
   av[7] = info

   TS6
   av[0] = nick
   av[1] = hop
   av[2] = ts
   av[3] = modes
   av[4] = user
   av[5] = host
   av[6] = IP
   av[7] = UID
   av[8] = vhost
   av[9] = info

do_nick(const char *source, char *nick, char *username, char *host,
              char *server, char *realname, time_t ts, uint32 svid,
              uint32 ip, char *vhost, char *uid, int hopcount, char *modes)

*/
int denora_event_nick(char *source, int ac, char **av)
{
    Server *s;
    char *temp;
    User *user, *u2;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac == 10) {
        ipchar = host_resolve(av[5]);
        s = server_find(source);
        temp = sstrdup(source);
        /* Source is always the server */
        *source = '\0';
        user =
            do_nick(source, av[0], av[4], av[5], (s ? s->name : temp),
                    av[9], strtoul(av[2], NULL, 10), 0, ipchar, av[8],
                    av[7], strtoul(av[1], NULL, 10), av[3], NULL);
        free(temp);
        free(ipchar);
    } else {
        u2 = find_byuid(source);
        do_nick((u2 ? u2->nick : source), av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
                NULL);
    }
    return MOD_CONT;
}

int denora_event_chghost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_topic(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_topic(ac, av);
    return MOD_CONT;
}

int denora_event_tburst(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
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

    TS6SID = sstrdup(Numeric);
    UseTS6 = 1;

    m = createMessage("401",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("402",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("TMODE",     denora_event_tmode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("BMASK",     denora_event_bmask); addCoreMessage(IRCD,m);
    m = createMessage("UID",       denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      denora_event_pass); addCoreMessage(IRCD,m);
    m = createMessage("PING",      denora_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   denora_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("TBURST",    denora_event_tburst); addCoreMessage(IRCD,m);
    m = createMessage("USER",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     denora_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB",     denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     denora_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    denora_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("421",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ENCAP",     denora_event_null); addCoreMessage(IRCD,m);   
    m = createMessage("SID",       denora_event_sid); addCoreMessage(IRCD,m);
    m = createMessage("EOB",       denora_event_eos); addCoreMessage(IRCD,m);
    m = createMessage("TSSYNC",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSCLOAK",  denora_event_chghost); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);

}

/* *INDENT-ON* */

void shadowircd_cmd_join(char *user, char *channel, time_t chantime)
{
    Uid *ud;

    ud = find_uid(user);
    send_cmd(NULL, "SJOIN %ld %s + :%s", (long int) chantime,
             channel, (ud ? ud->uid : user));
}

void shadowircd_cmd_svsinfo(void)
{
    /* not used */
}

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
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
void shadowircd_cmd_svinfo(void)
{
    send_cmd(NULL, "SVINFO 6 3 0 :%ld", (long int) time(NULL));
}

void shadowircd_cmd_capab(void)
{
    /* ShadowIRCd does not use CAPAB */
}

/* PASS */
void shadowircd_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s TS 6 %s", pass, TS6SID);
}

/* SERVER name protocol hop descript */
void shadowircd_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d %d :%s", servname, hop, PROTOCOL_REVISION,
             descript);
}


void shadowircd_cmd_connect(void)
{
    /* Make myself known to myself in the serverlist */
    if (UseTS6) {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, TS6SID);
    } else {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);
    }

    shadowircd_cmd_pass(RemotePassword);
    shadowircd_cmd_capab();
    shadowircd_cmd_server(ServerName, 1, ServerDesc);
    shadowircd_cmd_svinfo();
}

void shadowircd_cmd_bot_nick(char *nick, char *user, char *host,
                             char *real, char *modes)
{
    char nicknumbuf[10];
    ircsnprintf(nicknumbuf, 10, "%sAAAA%c%c", Numeric,
                (ts6nickcount[1] + 'A'), (ts6nickcount[0] + 'A'));
    send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0.0.0.0 %s %s :%s", nick,
             (long int) time(NULL), modes, user, host, nicknumbuf, host,
             real);
    new_uid(nick, nicknumbuf);
    ts6nickcount[0]++;
    if (ts6nickcount[0] > 35) { /* AAAAA9 */
        ts6nickcount[1]++;
        ts6nickcount[0] = 0;    /* AAAABA */
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

void shadowircd_cmd_eob(void)
{
    send_cmd(TS6SID, "EOB");
}

void shadowircd_cmd_ping(char *server)
{
    send_cmd(NULL, "PING :%s", server);
}

int denora_event_join(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1) {
        do_sjoin(source, ac, av);
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

int denora_event_privmsg(char *source, int ac, char **av)
{
    User *u;
    Uid *ud;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }

    u = find_byuid(source);
    ud = find_nickuid(av[0]);

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
        m_whois(source, (ud ? ud->nick : source));
    }
    return MOD_CONT;
}

/* EVENT: SERVER */
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        if (TS6UPLINK) {
            do_server(source, av[0], av[1], av[2], TS6UPLINK);
        } else {
            do_server(source, av[0], av[1], av[2], NULL);
        }
    } else {
        do_server(source, av[0], av[1], av[2], NULL);
    }
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

int denora_event_eos(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    update_sync_state(source, SYNC_COMPLETE);
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
    User *u;
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1) {
        return MOD_CONT;
    }

    u = find_byuid(source);

    do_quit(u->nick, ac, av);
    return MOD_CONT;
}

void shadowircd_cmd_372(char *source, char *msg)
{
    send_cmd(TS6SID, "372 %s :- %s", source, msg);
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

void shadowircd_cmd_nick(char *nick, char *name, const char *mode)
{
    char nicknumbuf[10];
    ircsnprintf(nicknumbuf, 10, "%sAAAAA%c", TS6SID,
                (ts6nickcount[0] + 'A'));
    send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0.0.0.0 %s %s :%s", nick,
             (long int) time(NULL), mode, ServiceUser, ServiceHost,
             nicknumbuf, ServiceHost, name);
    new_uid(nick, nicknumbuf);
    ts6nickcount[0]++;
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
    if (denora->protocoldebug) {
        alog(LOG_PROTOCOL, "PONG: Server Name %s : Who %s", servname, who);
    }
    send_cmd(TS6SID, "PONG %s", who);
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

    if (ac < 2 || *av[0] == '&') {
        return MOD_CONT;
    }

    if (*av[0] == '#') {
        do_cmode(source, ac, av);
    } else {
        u = find_byuid(source);
        u2 = find_byuid(av[0]);
        av[0] = u2->nick;
        do_umode2((u ? u->nick : source), ac, av);
    }
    return MOD_CONT;
}

int denora_event_tmode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (*av[1] == '#' || *av[1] == '&') {
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
    /* Not supported by ShadowIRCd. */
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
    TS6UPLINK = sstrdup(av[3]);
    return MOD_CONT;
}

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

void shadowircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    Uid *ud, *nickud;
    ud = find_uid(source);
    nickud = find_nickuid(dest);

    send_cmd((ud ? ud->uid : source), "NOTICE %s :\1%s \1",
             (nickud ? nickud->uid : dest), buf);
}

void shadowircd_cmd_version(char *server)
{
    Uid *ud;
    ud = find_uid(s_StatServ);

    send_cmd((ud ? ud->uid : s_StatServ), "VERSION %s", server);
}

void shadowircd_cmd_motd(char *sender, char *server)
{
    Uid *ud;
    ud = find_uid(sender);

    send_cmd((ud ? ud->uid : sender), "MOTD %s", server);
}

int denora_event_notice(char *source, int ac, char **av)
{
    User *u = NULL;
    Uid *ud = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }

    u = find_byuid(source);
    ud = find_nickuid(av[0]);
    m_notice((u ? u->nick : source), (ud ? ud->nick : av[0]), av[1]);
    return MOD_CONT;
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

    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id$");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("ShadowIRCd 4.0+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(1);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_SHADOWIRCD);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    return MOD_CONT;
}
