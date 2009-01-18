/*************************************************************************/
/* Asuka P10 ircd Protocol                                               */
/* (c) 2004-2009 Denora Team                                             */
/* Contact us at info@denorastats.org                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/* $Id$                      */
/*                                                                       */
/*************************************************************************/

#include "denora.h"
#include "asuka.h"

int p10nickcnt = 0;

IRCDVar myIrcd[] = {
    {"Asuka 1.2.1",             /* ircd name                 */
     "+iok",                    /* StatServ mode             */
     IRCD_DISABLE,              /* Vhost                     */
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
     IRCD_DISABLE,              /* +j                        */
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
     NULL,                      /* user modes                */
     NULL,                      /* channel modes             */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     IRCD_DISABLE,              /* join throttle             */
     IRCD_DISABLE,              /* vhost                     */
     IRCD_DISABLE,              /* vhost other               */
     IRCD_DISABLE,              /* Channel forwarding        */
     IRCD_ENABLE,               /* p10                       */
     IRCD_DISABLE,              /* ts6                       */
     IRCD_ENABLE,               /* numeric                   */
     IRCD_ENABLE,               /* gagged                    */
     IRCD_DISABLE,              /* spamfilter                */
     'b',                       /* ban char                  */
     IRCD_DISABLE,              /* except char               */
     IRCD_DISABLE,              /* invite char               */
     IRCD_DISABLE,              /* zip                       */
     IRCD_DISABLE,              /* ssl                       */
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_DISABLE,              /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_ENABLE,               /* extra warning             */
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

char *asuka_nickip(char *host)
{
    struct in_addr addr;
    int decoded;

    decoded = base64toIP(host);
    addr.s_addr = ntohl(decoded);
    return sstrdup(inet_ntoa(addr));
}

/*************************************************************************/

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
**      parv[6] = base64 ip
**	    parv[7] = uid
**      parv[8] = info
** NICK - change
**      source  = oldnick
**	parv[0] = new nickname
**      parv[1] = timestamp
*/
/* AB N Trystan 1 1116184299 tslee  24.2.101.227 +oiwgr trystan AYAmXj ABAAA :Dreams are answers to questions not yet asked */
/*         0    1  2          3      4            5        6    7      8       9 */
/* AK N OpServ  2 1116184708 OpServ srvx.net     +oik           AAAAAA AKAAA :Oper Service Bot */
/*         0    1  2          3      4            5            6      7       8 */

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

        ipchar = asuka_nickip(ip);

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

/*************************************************************************/

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

/*************************************************************************/

int denora_event_eob(char *source, int ac, char **av)
{
    Server *s;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    s = server_find(source);
    if (stricmp(s->name, denora->uplink) == 0) {
        send_cmd(NULL, "%s EA", p10id);
    }
    update_sync_state(source, SYNC_COMPLETE);
    return MOD_CONT;
}

/*************************************************************************/

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

/*************************************************************************/

void asuka_cmd_stats(char *sender, const char *letter, char *server)
{
    Uid *ud;
    Server *s;

    ud = find_uid(sender);
    s = server_find(server);

    send_cmd((ud ? ud->uid : sender), "R %s :%s", letter,
             (s ? (s->suid ? s->suid : server) : server));
}

/*************************************************************************/

/* PART */
/* ABAAA L #ircops */
void asuka_cmd_part(char *nick, char *chan, char *buf)
{
    Uid *ud;
    ud = find_uid(nick);
    if (buf) {
        send_cmd((ud ? ud->uid : nick), "L %s :%s", chan, buf);
    } else {
        send_cmd((ud ? ud->uid : nick), "L %s", chan);
    }
}

/*************************************************************************/

/* ABAAB J #ircops 1098031328 */
/* AB B #opserv 1116184708 +smtin AKAAA:o */
void asuka_cmd_join(char *user, char *channel, time_t chantime)
{
    Uid *ud;

    ud = find_uid(user);

    if (started) {
        send_cmd((ud ? ud->uid : user), "J %s %ld", channel,
                 (long int) chantime);
    } else {
        send_cmd(p10id, "B %s %ld %s:o", channel,
                 (long int) time(NULL), (ud ? ud->uid : user));
    }
}

/*************************************************************************/

void asuka_cmd_squit(char *servname, char *message)
{
    alog(LOG_PROTOCOL, "Servername %s Message %s", servname, message);
    send_cmd(p10id, "SQ %s :%s", p10id, message);
}

/*************************************************************************/

void asuka_cmd_connect(void)
{
    if (!BadPtr(Numeric)) {
        inttobase64(p10id, atoi(Numeric), 2);
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, p10id);
        asuka_cmd_pass(RemotePassword);
        asuka_cmd_server(ServerName, 1, ServerDesc);
    } else {
        alog(LOG_NORMAL, "Numeric is required for P10 ircds");
        exit(1);
    }
}

/*************************************************************************/

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
    return MOD_CONT;
}

/*************************************************************************/

/* PASS */
void asuka_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS :%s", pass);
}

/*************************************************************************/

/* SERVER [SERVERNAME] [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] :[DESCRIPTION] */
/* SERVER irc.undernet.org 1 933022556 947908144 J10 AA]]] :[127.0.0.1] A Undernet Server */
void asuka_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d %ld %lu J10 %s]]] +s :%s", servname, hop,
             (long int) denora->start_time, (long int) time(NULL), p10id,
             descript);
}

/*************************************************************************/

/* GLOBOPS */
void asuka_cmd_global(char *source, char *buf)
{
    Uid *id;

    id = find_uid(source);

    send_cmd((id ? id->uid : ServerName), "WA :%s", buf);
}

/*************************************************************************/

/* ABAAC A :I go away */
/* ABAAC A */
int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_away(source, (ac ? av[0] : NULL));
    return MOD_CONT;
}

/*************************************************************************/

int denora_event_topic(char *source, int ac, char **av)
{
    User *u;
    Server *s;
    char *newav[5];

    if (denora->protocoldebug)
        protocol_debug(source, ac, av);

    if (ac < 4)
        return MOD_CONT;

    u = user_find(source);
    if (!u)
        s = server_find(source);

    newav[0] = av[0];

    if (u)
        newav[1] = u->nick;
    else if (s)
        newav[1] = s->name;
    else
        newav[1] = source;

    newav[2] = av[ac - 2];
    newav[3] = av[ac - 1];
    newav[4] = '\0';

    do_topic(4, newav);
    return MOD_CONT;
}

/*************************************************************************/

int denora_event_squit(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 2) {
        return MOD_CONT;
    }
    do_squit(av[0]);
    return MOD_CONT;
}

/*************************************************************************/

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

    if (asuka_parse_lkill(av[0]) == 0) {
        do_quit(source, ac, av);
    } else {
        killer = asuka_lkill_killer(av[0]);
        msg = asuka_lkill_msg(av[0]);
        u = find_byuid(source);

        if (killer)
            m_kill(killer, (u ? u->nick : source), msg);
        else
            m_kill((u ? u->nick : source), (u ? u->nick : source), msg);
    }

    return MOD_CONT;
}

/*************************************************************************/

/* User modes */
/* ABAAA M Trystan +wg */

/* Channel modes */
/* ABAAA M #ircops +v ABAAB */
int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 2) {
        alog(LOG_DEBUG,
             "Unknown MODE formatted message please report the following");
        protocol_debug(source, ac, av);
        return MOD_CONT;
    }

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
    return MOD_CONT;
}

/*************************************************************************/

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

/*************************************************************************/

/* ABAAA K #ircops ABAAC :Trystan` */
int denora_event_kick(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    do_p10_kick(source, ac, av);
    return MOD_CONT;
}

/*************************************************************************/

/* JOIN - is the same syntax */
/* ABAAB J #ircops 1098031328 */
int denora_event_join(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_join(source, ac, av);
    return MOD_CONT;
}

/*************************************************************************/

/* CREATE - p10 don't join empty channels the CREATE them */
/* ABAAA C #ircops 1098031328 */
int denora_event_create(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_join(source, ac, av);
    return MOD_CONT;
}

/*************************************************************************/

/* BURST joins simlar to SJOIN */
/* AB B #denora 1113091975 +tn ABAAB,ABAAA:o :%*!*@*.aol.com */
/* s  c  0        1         2   3              4 */
/* AB B #test23 1115788230 ABAAB,ABAAA:o */
/* s  c 0         1         2 */
int denora_event_sjoin(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_p10_burst(source, ac, av);
    return MOD_CONT;
}

/*************************************************************************/

/* ABAAA MO AG */
int denora_event_motd(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_motd(source);
    return MOD_CONT;
}

/*************************************************************************/

void asuka_cmd_mode(char *source, char *dest, char *buf)
{
    alog(LOG_PROTOCOL, "MODE Source %s, dest %s, buf %s", source, dest,
         buf);
    send_cmd(p10id, "M %s %s %ld", dest, buf, (long int) time(NULL));
}

/*************************************************************************/

void asuka_cmd_notice(char *source, char *dest, char *buf)
{
    User *u;
    Uid *ud;

    u = finduser(dest);
    ud = find_uid(source);

    send_cmd((ud ? ud->uid : source), "O %s :%s", (u ? u->uid : dest),
             buf);
}

/*************************************************************************/

void asuka_cmd_privmsg(char *source, char *dest, char *buf)
{
    User *u;
    Uid *ud;

    u = finduser(dest);
    ud = find_uid(source);

    send_cmd((ud ? ud->uid : source), "P %s :%s", (u ? u->uid : dest),
             buf);
}

/*************************************************************************/

void asuka_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "O $%s :%s", dest, msg);
}

/*************************************************************************/

void asuka_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "P $%s :%s", dest, msg);
}

/*************************************************************************/

/* QUIT */
void asuka_cmd_quit(char *source, char *buf)
{
    Uid *ud;
    ud = find_uid(source);

    if (buf) {
        send_cmd((ud ? ud->uid : source), "Q :%s", buf);
    } else {
        send_cmd((ud ? ud->uid : source), "Q");
    }
}

/* Local kills handling - DrStein */
int asuka_parse_lkill(char *message)
{
    const char *localkillmsg = "Killed (";

    /* is it a Local kill message? */
    if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
        && (strstr(message, localkillmsg) != NULL))
        return 1;

    return 0;
}

char *asuka_lkill_killer(char *message)
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
char *asuka_lkill_msg(char *message)
{
    char *msg = NULL;

    /* Let's get the kill message */
    msg = strchr(message, '(');
    msg = strchr(message, '(');
    msg[strlen(msg) - 2] = '\0';
    msg++;                      /* removes first character '(' */

    return msg;
}

/*************************************************************************/

/* [NUMERIC PREFIX] N [NICK] [HOPCOUNT] [TIMESTAMP] [USERNAME] [HOST] <+modes> [BASE64 IP] [NUMERIC] :[USERINFO] */
/* AF N Client1 1 947957573 User userhost.net +oiwg DAqAoB AFAAA :Generic Client. */
void asuka_cmd_nick(char *nick, char *name, const char *modes)
{
    char nicknumbuf[6];
    send_cmd(p10id, "N %s 1 %ld %s %s %s B]AAAB %sAA%c :%s", nick,
             (long int) time(NULL), ServiceUser, ServiceHost, modes, p10id,
             (p10nickcnt + 'A'), name);
    ircsnprintf(nicknumbuf, 6, "%sAA%c", p10id, (p10nickcnt + 'A'));
    new_uid(nick, nicknumbuf);
    p10nickcnt++;
}

/*************************************************************************/

/* EVENT: SERVER */
/* SERVER Auska.Nomadirc.net 1 1098025304 1098036331 J10 ABAP] + :Test Server */
/* SERVER [SERVERNAME] [HOPCOUNT] [START TIME] [LINK TIME] [PROTOCOL] [NUMERIC/MAXCONN] [FLAGS] :[DESCRIPTION] */
/* (AB S trystan.nomadirc.net 2 0 1106520454 P10 ACAP] +h :Test Server) */
int denora_event_server(char *source, int ac, char **av)
{
    Server *s;
    char uplinknum[3];
    *uplinknum = '\0';

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
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

/*************************************************************************/

/* ABAAA P ADAAB :help */
int denora_event_privmsg(char *source, int ac, char **av)
{
    Uid *id;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    id = find_nickuid(av[0]);

    if (ac != 2)
        return MOD_CONT;
    m_privmsg(source, (id ? id->nick : av[0]), av[1]);
    return MOD_CONT;
}

/*************************************************************************/

/* ABAAA L #ircops */
int denora_event_part(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_part(source, ac, av);
    return MOD_CONT;
}

/*************************************************************************/

int denora_event_whois(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (source && ac >= 1) {
        m_whois(source, av[1]);
    }
    return MOD_CONT;
}

/*************************************************************************/

/* AB G !1098031985.558075 services.nomadirc.net 1098031985.558075 */
int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 1)
        return MOD_CONT;

    asuka_cmd_pong(p10id, av[0]);
    return MOD_CONT;
}

/*************************************************************************/

/* PONG */
/* AB Z AB 1116103763 1116103763 0 1116103763.867010 */
void asuka_cmd_pong(char *servname, char *who)
{
    char *t, *s;
    uint32 ts, tsnow, value;

    SET_SEGV_LOCATION();

    t = myStrGetToken(who, '!', 1);
    s = myStrGetToken(t, '.', 0);
    if (!s) {
        ts = 0;
        alog(LOG_NORMAL,
             "This is an error, you should report this as a problem");
        alog(LOG_NORMAL, "Server Name %s : Who %s", servname, who);
    } else {
        ts = strtol(s, NULL, 10);
    }
    tsnow = time(NULL);
    value = tsnow - ts;

    SET_SEGV_LOCATION();

    if (denora->protocoldebug) {
        alog(LOG_PROTOCOL, "PONG: Server Name %s : Who %s", servname, who);
    }
    send_cmd(p10id, "Z %s %ld %ld %ld %s", p10id, (long int) ts,
             (long int) tsnow, (long int) value, militime_float(NULL));

    SET_SEGV_LOCATION();

    if (s) {
        free(s);
    }
    if (t) {
        free(t);
    }
}

/*************************************************************************/

void asuka_cmd_bot_nick(char *nick, char *user, char *host, char *real,
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

/*************************************************************************/

void asuka_cmd_eob(void)
{
    send_cmd(p10id, "EB");
}

/*************************************************************************/

void asuka_cmd_ping(char *server)
{
    /* AB G !1115872042.64217 denora.nomadirc.net 1115872042.64217 */
    send_cmd(p10id, "G !%s %s %s", militime_float(NULL), server,
             militime_float(NULL));
}

/*************************************************************************/

void asuka_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "O %s :\1%s \1", dest, buf);
}

/*************************************************************************/

void asuka_cmd_version(char *server)
{
    Uid *ud;
    Server *s;
    ud = find_uid(s_StatServ);
    s = server_find(server);

    send_cmd((ud ? ud->uid : s_StatServ), "V :%s",
             (s ? (s->suid ? s->suid : server) : server));
}

/*************************************************************************/

void asuka_cmd_motd(char *sender, char *server)
{
    Uid *ud;
    Server *s;
    ud = find_uid(sender);
    s = server_find(server);

    send_cmd((ud ? ud->uid : sender), "MO :%s",
             (s ? (s->suid ? s->suid : server) : server));
}

/*************************************************************************/
int denora_event_notice(char *source, int ac, char **av)
{
    User *user_s = NULL;
    User *user_r = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 2 || *av[0] == '$' || strlen(source) == 2) {
        return MOD_CONT;
    }

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

/*************************************************************************/

void moduleAddIRCDMsgs(void)
{
    Message *m;
    int status;

    m = createMessage("ERROR", denora_event_error);
    addCoreMessage(IRCD, m);

    m = createMessage("NOTICE", denora_event_null);
    addCoreMessage(IRCD, m);

    m = createMessage("SERVER", denora_event_server);
    addCoreMessage(IRCD, m);

    m = createMessage("PASS", denora_event_null);
    addCoreMessage(IRCD, m);

    /* let the p10 tokens begin */

    /* end of burst */
    m = createMessage("EB", denora_event_eob);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_DEBUG, "debug: unable to bind to EB error [%d]", status);
    }

    /* nick */
    m = createMessage("N", denora_event_nick);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_DEBUG, "debug: unable to bind to N error [%d]", status);
    }

    /* ping */
    m = createMessage("G", denora_event_ping);
    addCoreMessage(IRCD, m);

    /* pong */
    m = createMessage("Z", denora_event_null);
    addCoreMessage(IRCD, m);

    /* MODE */
    m = createMessage("M", denora_event_mode);
    addCoreMessage(IRCD, m);

    /* CREATE */
    m = createMessage("C", denora_event_create);
    addCoreMessage(IRCD, m);

    /* JOIN */
    m = createMessage("J", denora_event_join);
    addCoreMessage(IRCD, m);

    /* QUIT */
    m = createMessage("Q", denora_event_quit);
    addCoreMessage(IRCD, m);

    /* TOPIC */
    m = createMessage("T", denora_event_topic);
    addCoreMessage(IRCD, m);

    /* MOTD */
    m = createMessage("MO", denora_event_motd);
    addCoreMessage(IRCD, m);

    /* KICK */
    m = createMessage("K", denora_event_kick);
    addCoreMessage(IRCD, m);

    /* AWAY */
    m = createMessage("A", denora_event_away);
    addCoreMessage(IRCD, m);

    /* ADMIN */
    m = createMessage("AD", denora_event_null);
    addCoreMessage(IRCD, m);

    /* PART */
    m = createMessage("L", denora_event_part);
    addCoreMessage(IRCD, m);

    /* BURST */
    m = createMessage("B", denora_event_sjoin);
    addCoreMessage(IRCD, m);

    /* PRIVMSG */
    m = createMessage("P", denora_event_privmsg);
    addCoreMessage(IRCD, m);

    /* NOTICE */
    m = createMessage("O", denora_event_notice);
    addCoreMessage(IRCD, m);

    /* SQUIT */
    m = createMessage("SQ", denora_event_squit);
    addCoreMessage(IRCD, m);

    /* INVITE */
    m = createMessage("I", denora_event_null);
    addCoreMessage(IRCD, m);

    /* DESYNCH */
    m = createMessage("DS", denora_event_error);
    addCoreMessage(IRCD, m);

    /* WALLOP */
    m = createMessage("WA", denora_event_null);
    addCoreMessage(IRCD, m);

    /* VERSION */
    m = createMessage("V", m_version);
    addCoreMessage(IRCD, m);

    /* WHOIS */
    m = createMessage("W", denora_event_whois);
    addCoreMessage(IRCD, m);

    /* SERVER */
    m = createMessage("S", denora_event_server);
    addCoreMessage(IRCD, m);

    /* PONG */
    m = createMessage("Z", denora_event_pong);
    addCoreMessage(IRCD, m);

    /* STATS */
    m = createMessage("R", m_stats);
    addCoreMessage(IRCD, m);

    /* ACCCOUNT */
    m = createMessage("AC", denora_event_null);
    addCoreMessage(IRCD, m);

    /* GLINE */
    m = createMessage("GL", denora_event_gline);
    addCoreMessage(IRCD, m);

    /* GLINE */
    m = createMessage("JU", denora_event_jupe);
    addCoreMessage(IRCD, m);

    /* INFO */
    m = createMessage("F", denora_event_null);
    addCoreMessage(IRCD, m);

    /* SETTIME */
    m = createMessage("SE", denora_event_null);
    addCoreMessage(IRCD, m);

    /* TIME */
    m = createMessage("TI", m_time);
    addCoreMessage(IRCD, m);

    /* TRACE */
    m = createMessage("TR", denora_event_null);
    addCoreMessage(IRCD, m);

    /* RPING */
    m = createMessage("RI", denora_event_null);
    addCoreMessage(IRCD, m);

    /* End of Burst Acknowledge */
    m = createMessage("EA", denora_event_null);
    addCoreMessage(IRCD, m);
}

/*************************************************************************/

void moduleAddIRCDCmds(void)
{
    pmodule_cmd_nick(asuka_cmd_nick);
    pmodule_cmd_mode(asuka_cmd_mode);
    pmodule_cmd_bot_nick(asuka_cmd_bot_nick);
    pmodule_cmd_notice(asuka_cmd_notice);
    pmodule_cmd_privmsg(asuka_cmd_privmsg);
    pmodule_cmd_serv_notice(asuka_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(asuka_cmd_serv_privmsg);
    pmodule_cmd_quit(asuka_cmd_quit);
    pmodule_cmd_pong(asuka_cmd_pong);
    pmodule_cmd_join(asuka_cmd_join);
    pmodule_cmd_part(asuka_cmd_part);
    pmodule_cmd_global(asuka_cmd_global);
    pmodule_cmd_squit(asuka_cmd_squit);
    pmodule_cmd_connect(asuka_cmd_connect);
    pmodule_cmd_eob(asuka_cmd_eob);
    pmodule_cmd_ctcp(asuka_cmd_ctcp);
    pmodule_cmd_version(asuka_cmd_version);
    pmodule_cmd_stats(asuka_cmd_stats);
    pmodule_cmd_motd(asuka_cmd_motd);
    pmodule_cmd_ping(asuka_cmd_ping);
}

/*************************************************************************/

void IRCDModeInit(void)
{
    /* User Modes
     * o - oper
     * O - local op
     * i - invisible
     * w - receive wallops
     * s - servnotice
     * d - deaf
     * k - chserv
     * g - debug
     * r - account
     * x - hiddenhost
     * X - xtraop
     * n - no chan
     * I - noidle
     * R - accountonly
     * h - sethost
     */
    ModuleSetUserMode(UMODE_I, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_X, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_k, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();

    /* Channel Modes
     * p - private
     * s - secret
     * m - moderated
     * t - topic limit
     * i - invite only
     * n - no privmsgs
     * r - reg only
     * D - deljoins
     * d - wasdeljoin
     * c - no colour
     * C - no ctcp
     * N - no notice
     * u - no quit parts
     */

    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_D, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
    CreateChanMode(CMODE_d, NULL, NULL);
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
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleUpdateSQLChanMode();
}

/*************************************************************************/

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
    /* Actual module code stuff */
    moduleAddAuthor("Denora");
    moduleAddVersion("$Id$");
    moduleSetType(PROTOCOL);
    /* protocol version and name that we support */
    pmodule_ircd_version("Asuka 1.2.1");
    /* set the flag of what ircd */
    pmodule_irc_var(IRC_ASUKA);

    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);

    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}

/*************************************************************************/

void DenoraFini(void)
{
}
