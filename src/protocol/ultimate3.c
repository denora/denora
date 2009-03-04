/* Ultimate IRCD 3
 *
 * (c) 2004-2009 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 * last tested with UltimateIRCd(Development)-3.1(00).svn($Rev: 897 $)
 *
 */

#include "denora.h"
#include "ultimate3.h"

IRCDVar myIrcd[] = {
    {"UltimateIRCd 3.0.",       /* ircd name                 */
     "+ioS",                    /* StatServ mode             */
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
     IRCD_ENABLE,               /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j Mode                   */
     CMODE_F,                   /* +L Mode                   */
     NULL,                      /* CAPAB Chan Modes          */
     IRCD_DISABLE,              /* We support TOKENS         */
     IRCD_ENABLE,               /* TOKENS are CASE Sensitive */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
     IRCD_DISABLE,              /* +I support                */
     IRCD_DISABLE,              /* SJOIN ban char            */
     IRCD_DISABLE,              /* SJOIN except char         */
     IRCD_DISABLE,              /* SJOIN invite char         */
     UMODE_x,                   /* umode for vhost           */
     IRCD_DISABLE,              /* owner                     */
     IRCD_ENABLE,               /* protect                   */
     IRCD_ENABLE,               /* halfop                    */
     NULL,
     NULL,
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     IRCD_DISABLE,              /* join throttle             */
     IRCD_DISABLE,              /* nick change flood         */
     'x',                       /* vhost                     */
     IRCD_DISABLE,              /* vhost other               */
     'F',                       /* channek linking           */
     IRCD_DISABLE,              /* p10                       */
     IRCD_DISABLE,              /* ts6                       */
     IRCD_ENABLE,               /* numeric                   */
     IRCD_DISABLE,              /* gagged                    */
     IRCD_DISABLE,              /* spamfilter                */
     'b',                       /* ban char                  */
     'e',                       /* except char               */
     IRCD_DISABLE,              /* invite char               */
     IRCD_DISABLE,              /* zip                       */
     IRCD_DISABLE,              /* ssl                       */
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_ENABLE,               /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_DISABLE,              /* extra warning             */
     IRCD_ENABLE,               /* Report sync state         */
     IRCD_DISABLE               /* Persistent channel mode   */
     }
    ,
};

IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     CAPAB_TSMODE,              /* TSMODE       */
     CAPAB_UNCONNECT,           /* UNCONNECT    */
     0,                         /* NICKIP       */
     CAPAB_NSJOIN,              /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
     CAPAB_BURST,               /* BURST        */
     CAPAB_TS5,                 /* TS5          */
     0,                         /* TS3          */
     CAPAB_DKEY,                /* DKEY         */
     0,                         /* PT4          */
     0,                         /* SCS          */
     0,                         /* QS           */
     0,                         /* UID          */
     0,                         /* KNOCK        */
     CAPAB_CLIENT,              /* CLIENT       */
     CAPAB_IPV6,                /* IPV6         */
     CAPAB_SSJ5,                /* SSJ5         */
     0,                         /* SN2          */
     0,                         /* TOKEN        */
     0,                         /* VHOST        */
     0,                         /* SSJ3         */
     0,                         /* NICK2        */
     0,                         /* UMODE2       */
     0,                         /* VL           */
     0,                         /* TLKEXT       */
     CAPAB_DODKEY,              /* DODKEY       */
     CAPAB_DOZIP,               /* DOZIP        */
     0, 0, 0}
};

/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_J, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_N, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_P, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_T, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_W, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_Z, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_j, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_t, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);

    /* Channel Modes */
    CreateChanMode(CMODE_A, NULL, NULL);
    CreateChanMode(CMODE_F, set_redirect, get_redirect);
    CreateChanMode(CMODE_K, NULL, NULL);
    CreateChanMode(CMODE_M, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_Q, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_T, NULL, NULL);
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_k, set_key, get_key);
    CreateChanMode(CMODE_l, set_limit, get_limit);
    CreateChanMode(CMODE_m, NULL, NULL);
    CreateChanMode(CMODE_n, NULL, NULL);
    CreateChanMode(CMODE_p, NULL, NULL);
    CreateChanMode(CMODE_r, NULL, NULL);
    CreateChanMode(CMODE_s, NULL, NULL);
    CreateChanMode(CMODE_t, NULL, NULL);
    CreateChanMode(CMODE_u, NULL, NULL);
    CreateChanMode(CMODE_z, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleSetChanUMode('!', 'a', STATUS_PROTECTED);


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

char *ultimate3_nickip(char *host)
{
    struct in_addr addr;

    addr.s_addr = htonl(strtoul(host, NULL, 0));
    return sstrdup(inet_ntoa(addr));
}

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
    char *ipchar = NULL;
    User *user;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        ipchar = ultimate3_nickip(av[8]);
        user = do_nick(source, av[0], av[4], av[5], av[6], av[9],
                       strtoul(av[2], NULL, 10), strtoul(av[7], NULL,
                                                         0),
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

int denora_event_sethost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_capab(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    capab_parse(ac, av);
    return MOD_CONT;
}

/*
** CLIENT
**      source  = NULL
**	parv[0] = nickname   Trystan
**      parv[1] = hopcount   1
**      parv[2] = timestamp  1090083810
**      parv[3] = modes      +ix
**	parv[4] = modes ?    +
**      parv[5] = username   Trystan
**      parv[6] = hostname   c-24-2-101-227.client.comcast.net
**      parv[7] = vhost      3223f75b.2b32ee69.client.comcast.net
**	parv[8] = server     WhiteRose.No.Eu.Shadow-Realm.org
**      parv[9] = svid       0
**	parv[10] = ip         402810339
** 	parv[11] = info      Dreams are answers to questions not yet asked
*/
int denora_event_client(char *source, int ac, char **av)
{
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        ipchar = ultimate3_nickip(av[10]);
        user = do_nick(source, av[0], av[5], av[6], av[8], av[11],
                       strtoul(av[2], NULL, 10), strtoul(av[9], NULL,
                                                         0),
                       ipchar, av[7], NULL,
                       strtoul(av[1], NULL, 10), av[3], NULL);
        free(ipchar);
    }
    return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("401",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("402",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
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
    m = createMessage("SVSKILL",   denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_svsmode); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    denora_event_sqline); addCoreMessage(IRCD,m);
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
    m = createMessage("SVINFO",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SZLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSGLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSZLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SETHOST",   denora_event_sethost); addCoreMessage(IRCD,m);
    m = createMessage("NETINFO",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GCONNECT",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("NETGLOBAL", denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CHATOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("NETCTRL",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CLIENT",	   denora_event_client); addCoreMessage(IRCD,m);
    m = createMessage("SMODE",	   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",	   denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("BURST",	   denora_event_burst); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SMODE",     denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("EOBURST",   denora_event_eob); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("RESYNCH",   denora_event_null); addCoreMessage(IRCD,m);
}

/* Events */

/* SVSMODE - forwarded */
int denora_event_svsmode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (*av[0] != '#')
        do_svsumode(ac, av);
    else
        do_cmode(source, ac, av);
    return MOD_CONT;
}

void ultimate3_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    ultimate3_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
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

int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!ac) {
       m_away(source, NULL);
    } else {
       m_away(source, av[0]);
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
    if (ac < 2)
        return MOD_CONT;

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
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
    if (ac != 1)
        return MOD_CONT;
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

int denora_event_setname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(source, av[0]);
    return MOD_CONT;
}

int denora_event_sqline(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    sql_do_sqline(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_chgname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_setident(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_username(source, av[0]);
    return MOD_CONT;
}

int denora_event_chgident(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_username(av[0], av[1]);
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
        do_server(source, av[0], av[1], av[2], NULL);
        return MOD_CONT;
    }
    do_server(source, av[0], av[1], av[2], NULL);
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

void ultimate3_cmd_nick(char *nick, char *name, const char *modes)
{
    send_cmd(NULL, "CLIENT %s 1 %ld %s + %s %s * %s 0 0 :%s", nick,
             (long int) time(NULL), modes, ServiceUser, ServiceHost,
             ServerName, name);
}

void ultimate3_cmd_mode(char *source, char *dest, char *buf)
{
    send_cmd(source, "MODE %s %s", dest, buf);
}

void ultimate3_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                        char *modes)
{
    send_cmd(NULL, "CLIENT %s 1 %ld %s + %s %s * %s 0 0 :%s", nick,
             (long int) time(NULL), modes, user, host, ServerName, real);
}

void ultimate3_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void ultimate3_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void ultimate3_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void ultimate3_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void ultimate3_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

/* PROTOCTL */
void ultimate3_cmd_capab(void)
{
    send_cmd(NULL,
             "CAPAB TS5 NOQUIT SSJ5 BURST UNCONNECT TSMODE NICKIP CLIENT");
}

/* PASS */
void ultimate3_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

/* SERVER name hop descript */
/* Unreal 3.2 actually sends some info about itself in the descript area */
void ultimate3_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/* PONG */
void ultimate3_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/* PART */
void ultimate3_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

/* GLOBOPS */
void ultimate3_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

/* SQUIT */
void ultimate3_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* Functions that use serval cmd functions */
/*
 * SVINFO
 *       parv[0] = sender prefix
 *       parv[1] = TS_CURRENT for the server
 *       parv[2] = TS_MIN for the server
 *       parv[3] = server is standalone or connected to non-TS only
 *       parv[4] = server's idea of UTC time
 */
void ultimate3_cmd_svinfo(void)
{
    send_cmd(NULL, "SVINFO 5 3 0 :%ld", (long int) time(NULL));
}

void ultimate3_cmd_burst(void)
{
    send_cmd(NULL, "BURST");
}

void ultimate3_cmd_connect(void)
{
    me_server = do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    ultimate3_cmd_pass(RemotePassword);
    ultimate3_cmd_capab();
    ultimate3_cmd_server(ServerName, 1, ServerDesc);
    ultimate3_cmd_svinfo();
    ultimate3_cmd_burst();
}

int denora_event_svinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

void ultimate3_cmd_eob(void)
{
    send_cmd(NULL, "BURST 0");
}

void ultimate3_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
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

int denora_event_eob(char *source, int ac, char **av)
{
  if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
  }
    update_sync_state(source, SYNC_COMPLETE);
    return MOD_CONT;
}

int denora_event_burst(char *source, int ac, char **av)
{
  if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
  }
    return MOD_CONT;
}

void ultimate3_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
}

void ultimate3_cmd_motd(char *sender, char *server)
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

void ultimate3_cmd_join(char *user, char *channel, time_t chantime)
{
    if (started) {
		send_cmd(ServerName, "SJOIN %ld %s + :%s", (long int) chantime, channel, user);
    } else {
		send_cmd(ServerName, "SJOIN %ld %s +nt :%s", (long int) chantime, channel, user);
    }
}

void ultimate3_cmd_ctcp(char *source, char *dest, char *buf)
{
   send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void moduleAddIRCDCmds() {
    pmodule_cmd_nick(ultimate3_cmd_nick);
    pmodule_cmd_mode(ultimate3_cmd_mode);
    pmodule_cmd_bot_nick(ultimate3_cmd_bot_nick);
    pmodule_cmd_notice(ultimate3_cmd_notice);
    pmodule_cmd_privmsg(ultimate3_cmd_privmsg);
    pmodule_cmd_serv_notice(ultimate3_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(ultimate3_cmd_serv_privmsg);
    pmodule_cmd_quit(ultimate3_cmd_quit);
    pmodule_cmd_pong(ultimate3_cmd_pong);
    pmodule_cmd_join(ultimate3_cmd_join);
    pmodule_cmd_part(ultimate3_cmd_part);
    pmodule_cmd_global(ultimate3_cmd_global);
    pmodule_cmd_squit(ultimate3_cmd_squit);
    pmodule_cmd_connect(ultimate3_cmd_connect);
    pmodule_cmd_eob(ultimate3_cmd_eob);
    pmodule_cmd_ctcp(ultimate3_cmd_ctcp);
    pmodule_cmd_version(ultimate3_cmd_version);
    pmodule_cmd_stats(ultimate3_cmd_stats);
    pmodule_cmd_motd(ultimate3_cmd_motd);
    pmodule_cmd_ping(ultimate3_cmd_ping);
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

    pmodule_ircd_version("UltimateIRCd 3.0.0.a26+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_ULTIMATE3);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    return MOD_CONT;
}

