/* ircu P10 IRCD functions
 *
 * (c) 2004-2010 Denora Team
 * Contact us at info@denorastats.org
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
#include "ircu.h"

int p10nickcnt = 0;

IRCDVar myIrcd[] = {
    {"IRCu 2.10.11+",           /* ircd name                 */
     "+iok",                    /* StatServ mode             */
     IRCD_ENABLE,               /* Vhost                     */
     IRCD_DISABLE,              /* Supports SGlines          */
     IRCD_DISABLE,              /* sgline sql table          */
     IRCD_ENABLE,               /* Supports SQlines          */
     IRCD_DISABLE,              /* sqline sql table          */
     IRCD_DISABLE,              /* Supports SZlines          */
     IRCD_DISABLE,              /* Has exceptions +e         */
     IRCD_DISABLE,              /* vidents                   */
     IRCD_ENABLE,               /* NICKIP                    */
     IRCD_DISABLE,              /* VHOST ON NICK             */
     IRCD_DISABLE,              /* +f                        */
     IRCD_DISABLE,              /* +j                        */
     IRCD_DISABLE,              /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j Mode                   */
     IRCD_DISABLE,              /* +L Mode                   */
     NULL,                      /* CAPAB Chan Modes          */
     IRCD_DISABLE,              /* We support TOKENS         */
     IRCD_DISABLE,              /* TOKENS are CASE Sensitive */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
     IRCD_DISABLE,              /* +I support                */
     IRCD_DISABLE,              /* SJOIN ban char            */
     IRCD_DISABLE,              /* SJOIN except char         */
     IRCD_DISABLE,              /* SJOIN invite char         */
     IRCD_DISABLE,              /* umode for vhost           */
     IRCD_DISABLE,              /* owner                     */
     IRCD_DISABLE,              /* protect                   */
     IRCD_DISABLE,              /* halfop                    */
     NULL,                      /* User modes                */
     NULL,                      /* Channel modes             */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     IRCD_DISABLE,              /* join throttle             */
     IRCD_DISABLE,              /* nick change flood         */
     'x',                       /* vhost                     */
     IRCD_DISABLE,              /* vhost other               */
     IRCD_DISABLE,              /* channel linking           */
     IRCD_ENABLE,               /* p10                       */
     IRCD_DISABLE,              /* TS6                       */
     IRCD_ENABLE,               /* numeric ie.. 350 etc      */
     IRCD_DISABLE,              /* channel mode gagged       */
     IRCD_DISABLE,              /* spamfilter                */
     'b',                       /* ban char                  */
     IRCD_DISABLE,              /* except char               */
     IRCD_DISABLE,              /* invite char               */
     IRCD_DISABLE,              /* zip                       */
     IRCD_ENABLE,               /* ssl                       */
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_DISABLE,              /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_DISABLE,              /* extra warning             */
     IRCD_ENABLE,               /* Report sync state         */
     IRCD_DISABLE               /* Persistent channel mode   */
     },
};

IRCDCAPAB myIrcdcap[] = {
    {
     1,                         /* NOQUIT       */
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
     0,                         /* CHANMODES    */
     0,                         /* sjb64        */
     0,                         /* nickchar     */
     }
};

/*************************************************************************/

void IRCDModeInit(void)
{
    /* User Modes */
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);    /* Deaf Mode */
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);    /* Server debug messages */
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);    /* */
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);    /* Invisible */
    ModuleSetUserMode(UMODE_k, IRCD_ENABLE);    /* Service */
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);    /* Local Operator */
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);    /* Registered */
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);    /* Server Notices */
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);    /* Wallops */
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);    /* Hidden Host */
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);    /* Global Operator */
    ModuleUpdateSQLUserMode();

    /* Channel List Modes */
    CreateChanBanMode(CMODE_b, add_ban, del_ban);       /* Ban */

    /* Channel Modes */
    CreateChanMode(CMODE_D, NULL, NULL);        /* Delayed Join */
    CreateChanMode(CMODE_d, NULL, NULL);        /* Delayed Join */
    CreateChanMode(CMODE_A, NULL, NULL);        /* Channel Admin Pass */
    CreateChanMode(CMODE_U, NULL, NULL);        /* Channel User Pass */
    CreateChanMode(CMODE_i, NULL, NULL);        /* Invite Only */
    CreateChanMode(CMODE_k, set_key, get_key);  /* Keyed */
    CreateChanMode(CMODE_l, set_limit, get_limit);      /* Invite Only */
    CreateChanMode(CMODE_m, NULL, NULL);        /* Moderated */
    CreateChanMode(CMODE_n, NULL, NULL);        /* No external messages */
    CreateChanMode(CMODE_p, NULL, NULL);        /* Private */
    CreateChanMode(CMODE_r, NULL, NULL);        /* Registered Only */
    CreateChanMode(CMODE_s, NULL, NULL);        /* Secret */
    CreateChanMode(CMODE_t, NULL, NULL);        /* Topic only changeable by ops */
    CreateChanMode(CMODE_u, NULL, NULL);        /* No Quitmessages */

    /* Channel User Modes */
    ModuleSetChanUMode('+', 'v', STATUS_VOICE); /* Voice */
    ModuleSetChanUMode('@', 'o', STATUS_OP);    /* Operator */
    ModuleUpdateSQLChanMode();
}

char *ircu_nickip(char *host)
{
    struct in_addr addr;
    int decoded;
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
    char *temp;
    char *ipchar;

    if (denora->protocoldebug)
        protocol_debug(source, ac, av);

    /* do_nick(const char *source, char *nick, char *username, char *host,
       char *server, char *realname, time_t ts, uint32 svid,
       uint32 ip, char *vhost, char *uid, int hopcount, char *modes) */

    temp = sstrdup(source);

    if (ac != 2) {
        char *realname, *ip, *nick;
        char *ident, *host, *modes, *modes2;
        const char *uid = "";
        const char *account = "";
        const char *timestamp = "";
        char hhostbuf[255];
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
        timestamp = sstrdup(av[2]);

        if (strpbrk(av[5], "+")) {
            while (*modes) {
                switch (*modes) {
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
        } else
            modes = NULL;

        ipchar = ircu_nickip(ip);

        if (isaccount && ishidden) {
            ircsnprintf(hhostbuf, sizeof(av[6]) + sizeof(hhostbuf) + 2,
                        "%s%s%s", HiddenPrefix, av[6], HiddenSuffix);
        }

        user = do_nick(source, nick, ident, host, (s ? s->name : temp),
                       realname, strtoul(timestamp, NULL, 10), 0, ipchar,
                       (ishidden
                        && isaccount) ? hhostbuf : NULL, (char *) uid,
                       strtoul(av[1], NULL, 10), modes, (char *) account);

        free(ipchar);
    } else {
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
void moduleAddIRCDMsgs(void) {
    Message *m;
    m = createMessage("ERROR",    denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",   denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("PASS",     denora_event_null); addCoreMessage(IRCD,m);

    /* let the p10 tokens begin */

    /* end of burst */
    m = createMessage("EB",       denora_event_eob); addCoreMessage(IRCD,m);
    /* nick */
    m = createMessage("N",        denora_event_nick); addCoreMessage(IRCD,m);
    /* ping */
    m = createMessage("G",        denora_event_ping); addCoreMessage(IRCD,m);
    /* MODE */
    m = createMessage("M",        denora_event_mode); addCoreMessage(IRCD,m);
    /* OPMODE */
    m = createMessage("OM",       denora_event_mode); addCoreMessage(IRCD,m);
    /* CREATE */
    m = createMessage("C",        denora_event_create); addCoreMessage(IRCD,m);
    /* JOIN */
    m = createMessage("J",        denora_event_join); addCoreMessage(IRCD,m);
    /* QUIT */
    m = createMessage("Q",        denora_event_quit); addCoreMessage(IRCD,m);
    /* TOPIC */
    m = createMessage("T",        denora_event_topic); addCoreMessage(IRCD,m);
    /* MOTD */
    m = createMessage("MO",       denora_event_motd); addCoreMessage(IRCD,m);
    /* KICK */
    m = createMessage("K",        denora_event_kick); addCoreMessage(IRCD,m);
    /* AWAY */
    m = createMessage("A",        denora_event_away); addCoreMessage(IRCD,m);
    /* ADMIN */
    m = createMessage("AD",       denora_event_null); addCoreMessage(IRCD,m);
    /* PART */
    m = createMessage("L",        denora_event_part); addCoreMessage(IRCD,m);
    /* BURST */
    m = createMessage("B",        denora_event_sjoin); addCoreMessage(IRCD,m);
    /* PRIVMSG */
    m = createMessage("P",        denora_event_privmsg); addCoreMessage(IRCD,m);
    /* NOTICE */
    m = createMessage("O",        denora_event_notice); addCoreMessage(IRCD,m);
    /* SQUIT */
    m = createMessage("SQ",       denora_event_squit); addCoreMessage(IRCD,m);
    /* INVITE */
    m = createMessage("I",        denora_event_null); addCoreMessage(IRCD,m);
    /* DESYNCH */
    m = createMessage("DS",       denora_event_error); addCoreMessage(IRCD,m);
    /* WALLOP */
    m = createMessage("WA",       denora_event_null); addCoreMessage(IRCD,m);
    /* VERSION */
    m = createMessage("V",        m_version); addCoreMessage(IRCD,m);
    /* WHOIS */
    m = createMessage("W",        denora_event_whois); addCoreMessage(IRCD,m);
    /* SERVER */
    m = createMessage("S",        denora_event_server); addCoreMessage(IRCD,m);
    /* PONG */
    m = createMessage("Z",        denora_event_pong); addCoreMessage(IRCD,m);
    /* STATS */
    m = createMessage("R",        m_stats); addCoreMessage(IRCD,m);
    /* ACCOUNT */
    m = createMessage("AC",       denora_event_account); addCoreMessage(IRCD,m);
    /* KILL */
    m = createMessage("D",	  denora_event_kill); addCoreMessage(IRCD,m);
    /* GLINE */
    m = createMessage("GL",       denora_event_gline); addCoreMessage(IRCD,m);
    /* JUPE */
    m = createMessage("JU",       denora_event_jupe); addCoreMessage(IRCD,m);
    /* INFO */
    m = createMessage("F",        denora_event_null); addCoreMessage(IRCD,m);
    /* SETTIME */
    m = createMessage("SE",       denora_event_null); addCoreMessage(IRCD,m);
    /* TIME */
    m = createMessage("TI",       m_time); addCoreMessage(IRCD,m);
    /* TRACE */
    m = createMessage("TR",       denora_event_null); addCoreMessage(IRCD,m);
    /* RPING */
    m = createMessage("RI",       denora_event_rping); addCoreMessage(IRCD,m);
    /* RPONG */
    m = createMessage("RO",	  denora_event_rpong); addCoreMessage(IRCD,m);
    /* End of Burst Acknowledge */
    m = createMessage("EA",       denora_event_null); addCoreMessage(IRCD,m);
    /* SILENCE */
    m = createMessage("U",	  denora_event_null); addCoreMessage(IRCD,m);
    /* PRIVS */
    m = createMessage("PRIVS",    denora_event_null); addCoreMessage(IRCD,m);
    /* CLEARMODE */
    m = createMessage("CM",       denora_event_clearmode); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

/* <source> GL <targetservermask> [!]<+/-><identmask>@<hostmask> <duration> <timestamp> :<reason>
 * AVAAC GL * +*@something.fake 5 1214347529 :Some fake gline */
int denora_event_gline(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    p10_gline((char *) "G", source, ac, av);
    return MOD_CONT;
}

/* <source> JU <targetservermask> [!]<+/-><jupedservername> <duration> <timestamp> :<reason>
 * AVAAC JU * +something.fake 5 1214347612 :Some fake jupe */
int denora_event_jupe(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    p10_gline((char *) "J", source, ac, av);
    return MOD_CONT;
}

void ircu_cmd_stats(char *sender, const char *letter, char *server)
{
    Uid *ud;
    Server *s;

    ud = find_uid(sender);
    s = server_find(server);

    send_cmd((ud ? ud->uid : sender), "STATS %s :%s", letter,
             (s ? (s->suid ? s->suid : server) : server));
}

/* PART */
void ircu_cmd_part(char *nick, char *chan, char *buf)
{
    Uid *ud;
    ud = find_uid(nick);
    if (buf) {
        send_cmd((ud ? ud->uid : nick), "L %s :%s", chan, buf);
    } else {
        send_cmd((ud ? ud->uid : nick), "L %s", chan);
    }
}

/* ABAAB J #ircops 1098031328 */
void ircu_cmd_join(char *user, char *channel, time_t chantime)
{
    Uid *ud;
    char *modes = NULL;

    ud = find_uid(user);

    if (started) {
        send_cmd((ud ? ud->uid : user), "J %s %ld", channel,
                 (long int) chantime);
    } else {
        if (AutoOp && AutoMode) {
            modes = sstrdup(AutoMode);
            modes++;            /* since the first char is +, we skip it */
            send_cmd(p10id, "B %s %ld %s:%s", channel,
                     (long int) chantime, (ud ? ud->uid : user), modes);
        } else {
            send_cmd(p10id, "B %s %ld %s", channel,
                     (long int) chantime, (ud ? ud->uid : user));
        }
    }
}

void ircu_cmd_squit(char *servname, char *message)
{
    alog(LOG_PROTOCOL, "Servername %s Message %s", servname, message);
    send_cmd(p10id, "SQ %s :%s", p10id, message);
}

void ircu_cmd_connect(void)
{

    if (!BadPtr(Numeric)) {
        inttobase64(p10id, atoi(Numeric), 2);
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, p10id);
        ircu_cmd_pass(RemotePassword);
        ircu_cmd_server(ServerName, 1, ServerDesc);
    } else {
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
void ircu_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS :%s", pass);
}

void ircu_cmd_capab()
{
    /* not used by p10 */
}

/* SERVER [SERVERNAME]     [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] :[DESCRIPTION] */
/* SERVER irc.undernet.org 1          933022556    947908144   J10        AA]]]             :[127.0.0.1] A Undernet Server */
void ircu_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d %ld %lu J10 %s]]] +s6 :%s", servname, hop,
             (long int) denora->start_time, (long int) time(NULL), p10id,
             descript);
}

/* GLOBOPS */
void ircu_cmd_global(char *source, char *buf)
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
    char *newav[5];

    if (denora->protocoldebug)
        protocol_debug(source, ac, av);

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

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
        return MOD_CONT;

    if (ircu_parse_lkill(av[0]) == 0) {
        do_quit(source, ac, av);
    } else {
        killer = ircu_lkill_killer(av[0]);
        msg = ircu_lkill_msg(av[0]);
        u = find_byuid(source);

        if (killer)
            m_kill(killer, (u ? u->nick : source), msg);
        else
            m_kill((u ? u->nick : source), (u ? u->nick : source), msg);
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

    if (!u) {
        sender = source;
    } else {
        sender = u->nick;
    }

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        s = server_find(source);
        if (s)
            sender = av[0];
        do_umode(sender, ac, av);
        if (strcmp(av[1], "x") != -1) {
            v = user_find(av[0]);
            if (v->account) {
                ircsnprintf(hhostbuf,
                            sizeof(v->account) + sizeof(hhostbuf) + 2,
                            "%s%s%s", HiddenPrefix, v->account,
                            HiddenSuffix);
                change_user_host(v->nick, hhostbuf);
            }
        }
    }
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

    if (denora->protocoldebug) {
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

void ircu_cmd_mode(char *source, char *dest, char *buf)
{
    Channel *c;
    alog(LOG_PROTOCOL, "MODE Source %s, dest %s, buf %s", source, dest,
         buf);
    if ((c = findchan(dest)))
        send_cmd(p10id, "M %s %s %ld", dest, buf, (long int) c->creation_time);
    else
        send_cmd(p10id, "M %s %s", dest, buf);
}

void ircu_cmd_notice(char *source, char *dest, char *buf)
{
    User *u;
    Uid *ud;

    u = finduser(dest);
    ud = find_uid(source);

    send_cmd((ud ? ud->uid : source), "O %s :%s", (u ? u->uid : dest),
             buf);
}

void ircu_cmd_privmsg(char *source, char *dest, char *buf)
{
    User *u;
    Uid *ud;

    u = finduser(dest);
    ud = find_uid(source);

    send_cmd((ud ? ud->uid : source), "P %s :%s", (u ? u->uid : dest),
             buf);
}

void ircu_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void ircu_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void ircu_cmd_quit(char *source, char *buf)
{
    Uid *ud;
    ud = find_uid(source);

    if (buf) {
        send_cmd((ud ? ud->uid : source), "QUIT :%s", buf);
    } else {
        send_cmd((ud ? ud->uid : source), "QUIT");
    }
}

/* Local kills handling - DrStein */
int ircu_parse_lkill(char *message)
{
    const char *localkillmsg = "Killed (";

    /* is it a Local kill message? */
    if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
        && (strstr(message, localkillmsg) != NULL))
        return 1;

    return 0;
}

char *ircu_lkill_killer(char *message)
{
    char *buf, *killer = NULL;

    /* Let's get the killer nickname */
    buf = sstrdup(message);
    killer = strtok(buf, " ");
    killer = strtok(NULL, " ");
    killer++;

    return killer;
}

/* Killed (*.beirut.com (KILL TESTING)) */
char *ircu_lkill_msg(char *message)
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
void ircu_cmd_nick(char *nick, char *name, const char *modes)
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

    if (denora->protocoldebug)
        protocol_debug(source, ac, av);

    *uplinknum = '\0';
    strlcpy(uplinknum, av[5], sizeof(uplinknum));

    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        do_server(source, av[0], av[1], av[7], uplinknum);
    } else {
        s = server_find(source);
        do_server((s ? s->name : source), av[0], av[1], av[7], uplinknum);
    }
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

    ircu_cmd_pong(p10id, av[0]);
    return MOD_CONT;
}

/* PONG */
void ircu_cmd_pong(char *servname, char *who)
{
    char *t, *s;
    uint32 ts, tsnow, value;
    t = myStrGetToken(who, '!', 1);
    s = myStrGetToken(t, '.', 0);
    if (!s) {
        ts = 0;
    } else {
        ts = strtol(s, NULL, 10);
    }
    tsnow = time(NULL);
    value = tsnow - ts;
    if (denora->protocoldebug) {
        alog(LOG_PROTOCOL, "PONG: Server Name %s : Who %s", servname, who);
    }
    send_cmd(p10id, "Z %s %ld %ld %ld %s", p10id, (long int) ts,
             (long int) tsnow, (long int) value, militime_float(NULL));
    if (s) {
        free(s);
    }
    if (t) {
        free(t);
    }
}

void ircu_cmd_bot_nick(char *nick, char *user, char *host, char *real,
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

void ircu_cmd_eob(void)
{
    send_cmd(p10id, "EB");
}

void ircu_cmd_ping(char *server)
{
    Uid *ud;
    Server *s;
    struct timeval t;
    ud = find_uid(s_StatServ);
    s = server_find(server);
    gettimeofday(&t, NULL);
    send_cmd(p10id, "RI %s %s %ld %ld :<No client start time>",
             ((s && s->suid) ? s->suid : server),
             (ud ? ud->uid : s_StatServ), t.tv_sec, t.tv_usec);
}

void ircu_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void ircu_cmd_version(char *server)
{
    Uid *ud;
    Server *s;

    ud = find_uid(s_StatServ);
    s = server_find(server);

    send_cmd((ud ? ud->uid : s_StatServ), "V :%s",
             (s ? (s->suid ? s->suid : server) : server));
}

void ircu_cmd_motd(char *sender, char *server)
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
    if (*av[0] == '#' && user_s) {
        m_notice(user_s->nick, av[0], av[1]);
    } else if (user_s) {
        user_r = user_find(av[0]);
        if (user_r) {
            m_notice(user_s->nick, user_r->nick, av[1]);
        }
    }
    return MOD_CONT;
}

int denora_event_clearmode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    chan_clearmodes(source, ac, av);
    return MOD_CONT;
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(ircu_cmd_nick);
    pmodule_cmd_mode(ircu_cmd_mode);
    pmodule_cmd_bot_nick(ircu_cmd_bot_nick);
    pmodule_cmd_notice(ircu_cmd_notice);
    pmodule_cmd_privmsg(ircu_cmd_privmsg);
    pmodule_cmd_serv_notice(ircu_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(ircu_cmd_serv_privmsg);
    pmodule_cmd_quit(ircu_cmd_quit);
    pmodule_cmd_pong(ircu_cmd_pong);
    pmodule_cmd_join(ircu_cmd_join);
    pmodule_cmd_part(ircu_cmd_part);
    pmodule_cmd_global(ircu_cmd_global);
    pmodule_cmd_squit(ircu_cmd_squit);
    pmodule_cmd_connect(ircu_cmd_connect);
    pmodule_cmd_eob(ircu_cmd_eob);
    pmodule_cmd_ctcp(ircu_cmd_ctcp);
    pmodule_cmd_version(ircu_cmd_version);
    pmodule_cmd_stats(ircu_cmd_stats);
    pmodule_cmd_motd(ircu_cmd_motd);
    pmodule_cmd_ping(ircu_cmd_ping);
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

    pmodule_ircd_version("IRCu 2.10.11+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    pmodule_irc_var(IRC_IRCU);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}
