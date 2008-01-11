/* Ratbox IRCD functions
 *
 * © 2004-2008 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 * last tested with ircd-ratbox-2.1.7(20051026_3-21016)
 * and ircd-ratbox-2.2.0beta(20051223_0-21526)
 *
 */

#include "denora.h"
#include "ratbox.h"

IRCDVar myIrcd[] = {
    {"Ratbox 2.0+",             /* ircd name                 */
     "+o",                      /* StatServ mode             */
     IRCD_DISABLE,              /* Vhost                     */
     IRCD_ENABLE,               /* Supports SGlines          */
     IRCD_ENABLE,               /* sgline sql table          */
     IRCD_DISABLE,              /* Supports SQlines          */
     IRCD_DISABLE,              /* sqline sql table          */
     IRCD_DISABLE,              /* Supports SZlines          */
     IRCD_ENABLE,               /* Has exceptions +e         */
     IRCD_DISABLE,              /* vidents                   */
     IRCD_ENABLE,               /* NICKIP                    */
     IRCD_DISABLE,              /* VHOST ON NICK             */
     IRCD_DISABLE,              /* +f                        */
     IRCD_DISABLE,              /* +j                      */
     IRCD_DISABLE,              /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j                        */
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
     NULL,                      /* user modes                */
     NULL,                      /* channel modes             */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     IRCD_DISABLE,              /* vhost                     */
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
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_DISABLE,              /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_ENABLE,               /* extra warning             */
     IRCD_DISABLE,              /* Report sync state         */
     IRCD_DISABLE               /* Persistent channel mode   */
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
     0, 0, 0}
};


/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_b, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_l, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_u, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();

    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
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

void ratbox_cmd_notice(char *source, char *dest, char *buf)
{
    Uid *ud;
    User *u;

    ud = find_uid(source);
    u = finduser(dest);
    send_cmd((UseTS6 ? (ud ? ud->uid : source) : source),
             "NOTICE %s :%s", (UseTS6 ? (u ? u->uid : dest) : dest), buf);
}

void ratbox_cmd_privmsg(char *source, char *dest, char *buf)
{
    Uid *ud, *ud2;

    ud = find_uid(source);
    ud2 = find_uid(dest);

    send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "PRIVMSG %s :%s",
             (UseTS6 ? (ud2 ? ud2->uid : dest) : dest), buf);
}

void ratbox_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $$%s :%s", dest, msg);
}

void ratbox_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $$%s :%s", dest, msg);
}

void ratbox_cmd_stats(char *sender, const char *letter, char *server)
{
    Uid *ud;
    ud = find_uid(sender);

    send_cmd((ud ? ud->uid : sender), "STATS %s %s", letter, server);
}

void ratbox_cmd_global(char *source, char *buf)
{
    Uid *u;

    if (source) {
        u = find_uid(source);
        if (u) {
            send_cmd((UseTS6 ? u->uid : source), "OPERWALL :%s", buf);
        } else {
            send_cmd((UseTS6 ? TS6SID : ServerName), "OPERWALL :%s", buf);
        }
    } else {
        send_cmd((UseTS6 ? TS6SID : ServerName), "OPERWALL :%s", buf);
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
   av[8] = info

*/
int denora_event_nick(char *source, int ac, char **av)
{
    Server *s;
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (UseTS6) {
        if (ac == 9) {
            s = server_find(source);
            /* Source is always the server */
            *source = '\0';
            ipchar = host_resolve(av[5]);
            user = do_nick(source, av[0], av[4], av[5], s->name, av[8],
                           strtoul(av[2], NULL, 10), 0, ipchar, NULL,
                           av[7], strtoul(av[1], NULL, 10), av[3], NULL);
            free(ipchar);
        } else if (ac == 8) {
            ipchar = host_resolve(av[5]);
            user = do_nick(source, av[0], av[4], av[5], av[6], av[7],
                           strtoul(av[2], NULL, 10), 0, ipchar, NULL, NULL,
                           strtoul(av[1], NULL, 10), av[3], NULL);
            free(ipchar);
        } else {
            user = find_byuid(source);
            do_nick((user ? user->nick : source), av[0], NULL, NULL, NULL,
                    NULL, strtoul(av[1], NULL, 10), 0, 0, NULL, NULL, 0,
                    NULL, NULL);
        }
    } else {
        if (ac != 2) {
            user = do_nick(source, av[0], av[4], av[5], av[6], av[7],
                           strtoul(av[2], NULL, 10), 0, 0, NULL, NULL,
                           strtoul(av[1], NULL, 10), av[3], NULL);
        } else {
            do_nick(source, av[0], NULL, NULL, NULL, NULL,
                    strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0,
                    NULL, NULL);
        }
    }
    return MOD_CONT;
}

int denora_event_topic(char *source, int ac, char **av)
{
    char *temp[5];
    char tbuf[BUFSIZE];

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    ircsnprintf(tbuf, BUFSIZE, "%ld", (long int) time(NULL));

    temp[0] = av[0];
    temp[1] = source;
    temp[2] = tbuf;
    temp[3] = av[1];
    do_topic(4, temp);
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

    if (UseTS6) {
        if (!BadPtr(Numeric)) {
            TS6SID = sstrdup(Numeric);
            UseTSMODE = 1;  /* TMODE */
        } else {
            alog(LOG_NORMAL, "TS6 requires the use of Numeric - exiting");
            exit(1);
        }
    }

    m = createMessage("401",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("402",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
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
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("TB",        denora_event_tburst); addCoreMessage(IRCD,m);
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
    m = createMessage("ENCAP",     denora_event_encap); addCoreMessage(IRCD,m);    
    m = createMessage("SID",       denora_event_sid); addCoreMessage(IRCD,m);
    m = createMessage("KLINE",     denora_event_kline); addCoreMessage(IRCD,m);
    m = createMessage("UNKLINE",   denora_event_unkline); addCoreMessage(IRCD,m);
    m = createMessage("XLINE",     denora_event_xline); addCoreMessage(IRCD,m);
    m = createMessage("UNXLINE",   denora_event_unxline); addCoreMessage(IRCD,m);
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

void ratbox_cmd_join(char *user, char *channel, time_t chantime)
{
    Uid *ud;

    ud = find_uid(user);
    send_cmd(NULL, "SJOIN %ld %s + :%s", (long int) chantime,
             channel, (UseTS6 ? (ud ? ud->uid : user) : user));
}

/*
 * SVINFO
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
void ratbox_cmd_svinfo(void)
{
    send_cmd(NULL, "SVINFO 6 3 0 :%ld", (long int) time(NULL));
}

void ratbox_cmd_svsinfo(void)
{

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
  HUB    - This server is a HUB 
  UID    - Can do UIDs
  ZIP    - Can do ZIPlinks
  ENC    - Can do ENCrypted links 
  KNOCK  -  supports KNOCK 
  TBURST - supports TBURST
  PARA	 - supports invite broadcasting for +p
  ENCAP	 - ?
*/
void ratbox_cmd_capab(void)
{
    send_cmd(NULL,
             "CAPAB :QS EX CHW IE KLN GLN KNOCK TB UNKLN CLUSTER ENCAP");
}

/* PASS */
void ratbox_cmd_pass(char *pass)
{
    if (UseTS6) {
        send_cmd(NULL, "PASS %s TS 6 :%s", pass, TS6SID);
    } else {
        send_cmd(NULL, "PASS %s :TS", pass);
    }
}

/* SERVER name hop descript */
void ratbox_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

void ratbox_cmd_connect(void)
{
    /* Make myself known to myself in the serverlist */
    if (UseTS6 && Numeric) {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, TS6SID);
    } else {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);
    }

    ratbox_cmd_pass(RemotePassword);
    ratbox_cmd_capab();
    ratbox_cmd_server(ServerName, 1, ServerDesc);
    ratbox_cmd_svinfo();
}

void ratbox_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                         char *modes)
{
    char *nicknumbuf = ts6_uid_retrieve();

    if (UseTS6) {
        send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0 %s :%s", nick,
                 (long int) time(NULL), modes, user, host, nicknumbuf,
                 real);

        new_uid(nick, nicknumbuf);
    } else {
        send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
                 (long int) time(NULL), modes, user, host, ServerName,
                 real);
    }
}

void ratbox_cmd_part(char *nick, char *chan, char *buf)
{
    Uid *ud;

    ud = find_uid(nick);

    if (buf) {
        send_cmd((UseTS6 ? ud->uid : nick), "PART %s :%s", chan, buf);
    } else {
        send_cmd((UseTS6 ? ud->uid : nick), "PART %s", chan);
    }
}

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    ratbox_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
    User *u = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (UseTS6) {
        u = find_byuid(source);
    }

    if (!ac) {
        m_away((UseTS6 ? (u ? u->nick : source) : source), NULL);
    } else {
        m_away((UseTS6 ? (u ? u->nick : source) : source), av[0]);
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

void ratbox_cmd_eob(void)
{
    /* doesn't support EOB */
}

void ratbox_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
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
    Uid *ud = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }

    if (UseTS6) {
        u = find_byuid(source);
        ud = find_nickuid(av[0]);
    }
    m_privmsg((UseTS6 ? (u ? u->nick : source) : source),
              (UseTS6 ? (ud ? ud->nick : av[0]) : av[0]), av[1]);
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
        m_whois(source, (UseTS6 ? (ud ? ud->nick : av[0]) : av[0]));
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
        if (UseTS6 && TS6UPLINK) {
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

void ratbox_cmd_mode(char *source, char *dest, char *buf)
{
    Uid *ud;

    if (source) {
        ud = find_uid(source);
        send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "MODE %s %s",
                 dest, buf);
    } else {
        send_cmd(source, "MODE %s %s", dest, buf);
    }
}

void ratbox_cmd_nick(char *nick, char *name, const char *mode)
{
    char *nicknumbuf = ts6_uid_retrieve();
    if (UseTS6) {
        send_cmd(TS6SID, "UID %s 1 %ld %s %s %s 0 %s :%s", nick,
                 (long int) time(NULL), mode, ServiceUser, ServiceHost,
                 nicknumbuf, name);

        new_uid(nick, nicknumbuf);
    } else {
        send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
                 (long int) time(NULL), mode, ServiceUser, ServiceHost,
                 ServerName, name);
    }
}

/* QUIT */
void ratbox_cmd_quit(char *source, char *buf)
{
    Uid *ud;
    ud = find_uid(source);

    if (buf) {
        send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "QUIT :%s",
                 buf);
    } else {
        send_cmd((UseTS6 ? (ud ? ud->uid : source) : source), "QUIT");
    }
}

/* PONG */
void ratbox_cmd_pong(char *servname, char *who)
{
    if (UseTS6) {
        send_cmd(TS6SID, "PONG %s", who);
    } else {
        send_cmd(servname, "PONG %s", who);
    }
}

/* SQUIT */
void ratbox_cmd_squit(char *servname, char *message)
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
        if (UseTS6) {
            u = find_byuid(source);
            u2 = find_byuid(av[0]);
            av[0] = u2->nick;
            do_umode(u->nick, ac, av);
        } else {
            do_umode(source, ac, av);
        }
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
    if (UseTS6) {
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

void ratbox_cmd_version(char *server)
{
    Uid *ud;
    ud = find_uid(s_StatServ);

    send_cmd((ud ? ud->uid : s_StatServ), "VERSION %s", server);
}

void ratbox_cmd_motd(char *sender, char *server)
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

    if (UseTS6) {
        u = find_byuid(source);
        ud = find_nickuid(av[0]);
        m_notice((u ? u->nick : source), (ud ? ud->nick : av[0]), av[1]);
    } else {
        m_notice(source, av[0], av[1]);
    }
    return MOD_CONT;
}

void ratbox_cmd_ctcp(char *source, char *dest, char *buf)
{
    Uid *ud, *nickud;
    ud = find_uid(source);
    nickud = find_nickuid(dest);

    if (UseTS6) {
        send_cmd((ud ? ud->uid : source), "NOTICE %s :\1%s \1",
                 (nickud ? nickud->uid : dest), buf);
    } else {
        send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
    }
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(ratbox_cmd_nick);
    pmodule_cmd_mode(ratbox_cmd_mode);
    pmodule_cmd_bot_nick(ratbox_cmd_bot_nick);
    pmodule_cmd_notice(ratbox_cmd_notice);
    pmodule_cmd_privmsg(ratbox_cmd_privmsg);
    pmodule_cmd_serv_notice(ratbox_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(ratbox_cmd_serv_privmsg);
    pmodule_cmd_quit(ratbox_cmd_quit);
    pmodule_cmd_pong(ratbox_cmd_pong);
    pmodule_cmd_join(ratbox_cmd_join);
    pmodule_cmd_part(ratbox_cmd_part);
    pmodule_cmd_global(ratbox_cmd_global);
    pmodule_cmd_squit(ratbox_cmd_squit);
    pmodule_cmd_connect(ratbox_cmd_connect);
    pmodule_cmd_eob(ratbox_cmd_eob);
    pmodule_cmd_ctcp(ratbox_cmd_ctcp);
    pmodule_cmd_version(ratbox_cmd_version);
    pmodule_cmd_stats(ratbox_cmd_stats);
    pmodule_cmd_motd(ratbox_cmd_motd);
    pmodule_cmd_ping(ratbox_cmd_ping);
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
    moduleAddVersion("$Id$");
    moduleSetType(PROTOCOL);
    pmodule_ircd_version("Ratbox IRCD 2.0+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_RATBOX);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    UplinkSynced = 1;
    return MOD_CONT;
}
