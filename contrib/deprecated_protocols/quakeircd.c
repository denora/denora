/* QuakeIRCD
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

/*************************************************************************/

#include "denora.h"
#include "quakeircd.h"

IRCDVar myIrcd[] = {
    {"QuakeIRCD 2.0 Ruby",      /* ircd name */
     "+ioS",                    /* StatServ mode             */
     IRCD_ENABLE,               /* Vhost                     */
     IRCD_ENABLE,               /* Supports SGlines          */
     IRCD_DISABLE,              /* sgline sql table          */
     IRCD_ENABLE,               /* Supports SQlines          */
     IRCD_DISABLE,              /* sqline sql table          */
     IRCD_ENABLE,               /* Supports SZlines          */
     IRCD_ENABLE,               /* Has exceptions +e         */
     IRCD_ENABLE,               /* vidents                   */
     IRCD_DISABLE,              /* NICKIP                    */
     IRCD_ENABLE,               /* VHOST ON NICK             */
     IRCD_ENABLE,               /* +f                        */
     IRCD_DISABLE,              /* +j                        */
     IRCD_ENABLE,               /* +L                        */
     CMODE_f,                   /* +f Mode                   */
     IRCD_DISABLE,              /* +j                        */
     CMODE_L,                   /* +L Mode                   */
     NULL,                      /* CAPAB Chan Modes          */
     IRCD_ENABLE,               /* We support Unreal TOKENS  */
     IRCD_DISABLE,              /* TOKENS are CASE Sensitive */
     IRCD_ENABLE,               /* TIME STAMPS are BASE64    */
     IRCD_ENABLE,               /* +I support                */
     '&',                       /* SJOIN ban char            */
     '\"',                      /* SJOIN except char         */
     '\'',                      /* SJOIN Invite char         */
     UMODE_x,                   /* umode for vhost           */
     IRCD_ENABLE,               /* owner                     */
     IRCD_ENABLE,               /* protect                   */
     IRCD_ENABLE,               /* halfop                    */
     NULL,
     NULL,
     'f',                       /* flood                     */
     'j',                       /* flood other               */
     'x',                       /* vhost                     */
     't',                       /* vhost other               */
     'L',                       /* channek linking           */
     IRCD_DISABLE,              /* p10                       */
     IRCD_DISABLE,              /* ts6                       */
     IRCD_ENABLE,               /* numeric                   */
     IRCD_ENABLE,               /* gagged                    */
     IRCD_DISABLE,              /* spamfilter                */
     'b',                       /* ban char                  */
     'e',                       /* except char               */
     'I',                       /* invite char               */
     IRCD_ENABLE,               /* zip                       */
     IRCD_DISABLE,              /* ssl                       */
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_ENABLE,               /* svid                      */
     IRCD_ENABLE,               /* hidden oper               */
     IRCD_DISABLE,              /* extra warning             */
     IRCD_ENABLE                /* Report sync state         */
     }
    ,
};

IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     0,                         /* TSMODE       */
     0,                         /* UNCONNECT    */
     CAPAB_NICKIP,              /* NICKIP       */
     0,                         /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
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
     CAPAB_TOKEN,               /* TOKEN        */
     0,                         /* VHOST        */
     CAPAB_SSJ3,                /* SSJ3         */
     CAPAB_NICK2,               /* NICK2        */
     CAPAB_UMODE2,              /* UMODE2       */
     CAPAB_VL,                  /* VL           */
     CAPAB_TLKEXT,              /* TLKEXT       */
     0,                         /* DODKEY       */
     0,                         /* DOZIP        */
     CAPAB_CHANMODE,            /* CHANMODE             */
     CAPAB_SJB64,
     0,
     }
};

/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_B, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_C, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_H, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_I, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_N, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_O, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_T, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_V, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_W, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_f, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_q, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_t, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_v, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_z, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
    CreateChanMode(CMODE_A, NULL, NULL);
    CreateChanMode(CMODE_B, NULL, NULL);
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_G, NULL, NULL);
    CreateChanMode(CMODE_K, NULL, NULL);
    CreateChanMode(CMODE_L, set_redirect, get_redirect);
    CreateChanMode(CMODE_M, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_Q, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_T, NULL, NULL);
    CreateChanMode(CMODE_V, NULL, NULL);
    CreateChanMode(CMODE_X, NULL, NULL);
    CreateChanMode(CMODE_Y, NULL, NULL);
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
    CreateChanMode(CMODE_u, NULL, NULL);
    CreateChanMode(CMODE_z, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleSetChanUMode('~', 'a', STATUS_PROTECTED);
    ModuleSetChanUMode('*', 'q', STATUS_OWNER);
    ModuleSetChanUMode('!', 'g', STATUS_SHUNNED);

    ModuleUpdateSQLChanMode();

}


void quakeircd_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
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

void quakeircd_cmd_nick(char *nick, char *name, const char *modes)
{
    send_cmd(NULL, "%s %s 1 %ld %s %s %s 0 %s %s%s :%s",
             send_token("NICK", "&"), nick, (long int) time(NULL),
             ServiceUser, ServiceHost, ServerName, modes, ServiceHost,
             (ircd->nickip ? " *" : " "), name);
}

void quakeircd_cmd_mode(char *source, char *dest, char *buf)
{
    send_cmd(source, "%s %s %s", send_token("MODE", "G"), dest, buf);
}

void quakeircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                            char *modes)
{
    send_cmd(NULL, "%s %s 1 %ld %s %s %s 0 %s %s%s :%s",
             send_token("NICK", "&"), nick, (long int) time(NULL),
             user, host, ServerName, modes, host,
             (ircd->nickip ? " *" : " "), real);
}

void quakeircd_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "%s %s :%s", send_token("NOTICE", "B"), dest, buf);
}

void quakeircd_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "%s %s :%s", send_token("PRIVMSG", "!"), dest, buf);
}

void quakeircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "%s $%s :%s", send_token("NOTICE", "B"), dest, msg);
}

void quakeircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "%s $%s :%s", send_token("PRIVMSG", "!"), dest, msg);
}

void quakeircd_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

/* QUIT */
void quakeircd_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "%s :%s", send_token("QUIT", ","), buf);
    } else {
        send_cmd(source, "%s", send_token("QUIT", ","));
    }
}

/* PROTOCTL */
/*
   NICKv2 = Nick Version 2
   VHP    = Sends hidden host 
   UMODE2 = sends UMODE2 on user modes
   NICKIP = Sends IP on NICK
   TOKEN  = Use tokens to talk
   SJ3    = Supports SJOIN
   NOQUIT = No Quit
   TKLEXT = Extended TKL we don't use it but best to have it
   SJB64  = Base64 encoded time stamps
   VL     = Version Info
   NS     = Numeric Server

*/
void quakeircd_cmd_capab()
{
    send_cmd(NULL,
             "PROTOCTL NICKv2 VHP UMODE2 NICKIP%sSJOIN SJOIN2 SJ3 NOQUIT TKLEXT SJB64%s",
             (UseTokens ? " TOKEN " : " "), (Numeric ? " VL NS" : " "));
}

/* PASS */
void quakeircd_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS :%s", pass);
}

/* SERVER name hop descript */
/* Unreal 3.2 actually sends some info about itself in the descript area */
void quakeircd_cmd_server(char *servname, int hop, char *descript)
{
    if (Numeric) {
        send_cmd(NULL, "SERVER %s %d :U0-*-%s %s", servname, hop, Numeric,
                 descript);
    } else {
        send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
    }
}

/* PONG */
void quakeircd_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "%s %s", send_token("PONG", "9"), who);
}

/* JOIN - SJOIN */
void quakeircd_cmd_join(char *user, char *channel, time_t chantime)
{
    send_cmd(ServerName, "%s !%s %s :%s",
             send_token("SJOIN", "~"), base64enc((long int) chantime),
             channel, user);
}

/* PART */
void quakeircd_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "%s %s :%s", send_token("PART", "D"), chan, buf);
    } else {
        send_cmd(nick, "%s %s", send_token("PART", "D"), chan);
    }
}

/* GLOBOPS */
void quakeircd_cmd_global(char *source, char *buf)
{
    send_cmd(source, "%s :%s", send_token("GLOBOPS", "]"), buf);
}

/* SQUIT */
void quakeircd_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "%s %s :%s", send_token("SQUIT", "-"), servname,
             message);
}

/* Events */

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    quakeircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

void quakeircd_cmd_netinfo(int ac, char **av)
{
    if (ac) {
        send_cmd(NULL, "%s %ld %ld %d %s 0 0 0 :%s",
                 send_token("NETINFO", "AO"), (long int) stats->users_max,
                 (long int) time(NULL), atoi(av[2]), av[3], av[7]);
    }
}

/* netinfo
 *  argv[0] = max global count
 *  argv[1] = time of end sync
 *  argv[2] = unreal protocol using (numeric)
 *  argv[3] = cloak-crc (> u2302)
 *  argv[4] = free(**)
 *  argv[5] = free(**)
 *  argv[6] = free(**)
 *  argv[7] = ircnet
 */
int denora_event_netinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    quakeircd_cmd_netinfo(ac, av);
    return MOD_CONT;
}


/* TKL
 *           add:      remove:    spamfilter:    spamfilter+TKLEXT  sqline:
 * parv[ 1]: +         -          +/-            +                  +/-
 * parv[ 2]: type      type       type           type               type
 * parv[ 3]: user      user       target         target             hold
 * parv[ 4]: host      host       action         action             host
 * parv[ 5]: setby     removedby  (un)setby      setby              setby
 * parv[ 6]: expire_at            expire_at (0)  expire_at (0)      expire_at
 * parv[ 7]: set_at               set_at         set_at             set_at
 * parv[ 8]: reason               regex          tkl duration       reason
 * parv[ 9]:                                     tkl reason [A]        
 * parv[10]:                                     regex              
 *
*/
int denora_event_tkl(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[0], "+")) {
        sql_do_server_bans_add(av[1], av[2], av[3], av[4], av[6], av[5],
                               av[7]);
    }

    if (!stricmp(av[0], "-")) {
        sql_do_server_bans_remove(av[1], av[2], av[3]);
    }
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

/*
** away
**      parv[0] = sender prefix
**      parv[1] = away message
*/
int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_away(source, (ac ? av[0] : NULL));
    return MOD_CONT;
}

/*
** m_topic
**	parv[0] = sender prefix
**	parv[1] = topic text
**
**	For servers using TS: 
**	parv[0] = sender prefix
**	parv[1] = channel name
**	parv[2] = topic nickname
**	parv[3] = topic time
**	parv[4] = topic text
*/
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

/* Unreal sends USER modes with this */
/*
    umode2
    parv[0] - sender
    parv[1] - modes to change
*/
int denora_event_umode2(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;

    do_umode2(source, ac, av);
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
    Server *s;
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 3) {
        return MOD_CONT;
    }
    if (Numeric) {
        s = server_find(source);
        do_kick((s ? s->name : source), ac, av);
    } else {
        do_kick(source, ac, av);
    }
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

void quakeircd_cmd_motd(char *sender, char *server)
{
    send_cmd(sender, "MOTD %s", server);
}

int denora_event_setname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(source, av[0]);
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

int denora_event_sethost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(source, av[0]);
    return MOD_CONT;
}

/* SVSMODE - forwarded */
int denora_event_svsmode(char *source, int ac, char **av)
{
    char *modes = av[1];
    char *tbuf = NULL;
    char buf[256];
    char modebuf[256];
    char *newav[4];
    int add = 0;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (*av[0] != '#') {
        /* yes Unreal sucks - they over lap the user mode of d go figure, want to bitch that this
           cause Denora to be slow see the Unreal people and tell them having two modes that do
           two different things is fucked and thank Jobe1986 for pointing out Unreals fuck up
         */
        while (*modes) {
            switch (*modes) {
            case '+':
                add = 1;
                break;
            case '-':
                add = 0;
                break;
            case 'd':
                /* Must ignore it otherwise Jobe's bug will not be fixed */
                if (ac == 3) {
                    break;
                } else {
                    if (tbuf) {
                        ircsnprintf(buf, sizeof(buf), "%s%c", tbuf,
                                    *modes);
                        free(tbuf);
                        tbuf = sstrdup(buf);
                    } else {
                        ircsnprintf(buf, sizeof(buf), "%c", *modes);
                        tbuf = sstrdup(buf);
                    }
                }
            default:
                if (tbuf) {
                    ircsnprintf(buf, sizeof(buf), "%s%c", tbuf, *modes);
                    free(tbuf);
                    tbuf = sstrdup(buf);
                } else {
                    ircsnprintf(buf, sizeof(buf), "%c", *modes);
                    tbuf = sstrdup(buf);
                }
                break;
            }
            (void) *modes++;
        }
        if (tbuf) {
            ircsnprintf(modebuf, sizeof(modebuf), "%s%s",
                        (add ? "+" : "-"), tbuf);
            free(tbuf);
            newav[0] = av[0];
            newav[1] = sstrdup(modebuf);
            newav[2] = av[2];
            do_svsumode(ac, newav);
            if (newav[1]) {
                free(newav[1]);
            }
        }
    } else {
        do_cmode(source, ac, av);
    }
    return MOD_CONT;
}

char *quakeircd_nickip(char *host)
{
    struct in_addr addr;
    uint32 ip;
    ip = ntohl(decode_ip(host));
    addr.s_addr = htonl(ip);
    return sstrdup(inet_ntoa(addr));
}

/*
** NICK - new
**      source  = NULL
**	parv[0] = nickname
**      parv[1] = hopcount
**      parv[2] = timestamp
**      parv[3] = username
**      parv[4] = hostname
**      parv[5] = servername
**  if NICK version 1:
**      parv[6] = servicestamp
**	parv[7] = info
**  if NICK version 2:
**	parv[6] = servicestamp
**      parv[7] = umodes
**	parv[8] = virthost, * if none
**	parv[9] = info
**  if NICKIP:
**      parv[9] = ip
**      parv[10] = info
** 
** NICK - change 
**      source  = oldnick
**	parv[0] = new nickname
**      parv[1] = hopcount
*/
/*
 do_nick(const char *source, char *nick, char *username, char *host,
              char *server, char *realname, time_t ts, uint32 svid,
              uint32 ip, char *vhost, char *uid, int hopcount, char *modes)
*/
int denora_event_nick(char *source, int ac, char **av)
{
    User *user;
    Server *s;
    char buf[BUFSIZE];
    uint32 ts;
    char *ipchar = NULL;
    *buf = '\0';

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 2) {
        if (Numeric) {
            ircsnprintf(buf, BUFSIZE - 1, "%ld", base64dec(av[5]));
            s = server_find(buf);
            ts = base64dec_ts(av[2]);
        } else {
            s = server_find(av[5]);
            ts = strtoul(av[2], NULL, 10);
        }
        if (ac == 7) {
            /* 
               <codemastr> that was a bug that is now fixed in 3.2.1
               <codemastr> in  some instances it would use the non-nickv2 format
               <codemastr> it's sent when a nick collision occurs 
               - so we have to leave it around for now -TSL
             */
            ipchar = host_resolve(av[4]);
            do_nick(source, av[0], av[3], av[4], (s ? s->name : av[5]),
                    av[6], ts, 0, 0, NULL, NULL, strtoul(av[1], NULL, 10),
                    NULL, NULL);
            free(ipchar);
        } else if (ac == 11) {
            ipchar = quakeircd_nickip(av[9]);
            user =
                do_nick(source, av[0], av[3], av[4], (s ? s->name : av[5]),
                        av[10], ts, strtoul(av[6], NULL, 0),
                        ipchar, av[8], NULL, strtoul(av[1], NULL, 10),
                        av[7], NULL);
            free(ipchar);

        } else {
            /* NON NICKIP */
            ipchar = host_resolve(av[4]);
            user =
                do_nick(source, av[0], av[3], av[4], (s ? s->name : av[5]),
                        av[9], ts, strtoul(av[6], NULL, 0), 0, av[8], NULL,
                        strtoul(av[1], NULL, 10), av[7], NULL);
            free(ipchar);
        }
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
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

/* EVENT: SERVER */
int denora_event_server(char *source, int ac, char **av)
{
    char *desc;
    char *vl;
    char *numeric;
    Server *s;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        vl = myStrGetToken(av[2], ' ', 0);
        numeric = myStrGetToken(vl, '-', 2);
        desc = myStrGetTokenRemainder(av[2], ' ', 1);
        do_server(source, av[0], av[1], desc, numeric);
        free(vl);
        free(desc);
        free(numeric);
    } else {
        if (Numeric) {
            s = server_find(source);
            do_server((s ? s->name : source), av[0], av[1], av[3], av[2]);
        } else {
            do_server(source, av[0], av[1], av[2], NULL);
        }
    }

    return MOD_CONT;
}

int denora_event_privmsg(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }
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

int denora_event_error(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (av[0]) {
        alog(LOG_ERROR, "ERROR: %s", av[0]);
    }
    return MOD_CONT;
}

int denora_event_swhois(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_swhois(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_sdesc(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac >= 1) {
        server_set_desc(source, av[0]);
    }
    return MOD_CONT;
}

/*
   On Services connect there is
   SJOIN !11LkOb #ircops +nt :@Trystan &*!*@*.aol.com "*@*.home.com

   av[0] = time stamp (base64)
   av[1] = channel
   av[2] = modes
   av[3] = users + bans + exceptions

   On Channel Creation or a User joins an existing
   Luna.NomadIrc.Net SJOIN !11LkW9 #akill :@Trystan
   Luna.NomadIrc.Net SJOIN !11LkW9 #akill :Trystan`

   av[0] = time stamp (base64)
   av[1] = channel
   av[2] = users

*/
int denora_event_sjoin(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_sjoin(source, ac, av);
    return MOD_CONT;
}

void quakeircd_cmd_eob(void)
{
    send_cmd(ServerName, "%s", send_token("EOS", "ES"));
}

void quakeircd_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

void quakeircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "%s %s :\1%s \1", send_token("NOTICE", "B"), dest,
             buf);
}

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
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

void quakeircd_cmd_connect(void)
{
    if (Numeric) {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, Numeric);
    } else {
        me_server =
            do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);
    }

    quakeircd_cmd_capab();
    quakeircd_cmd_pass(RemotePassword);
    quakeircd_cmd_server(ServerName, 1, ServerDesc);
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("6",        denora_event_away); addCoreMessage(IRCD,m);
    }
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("*",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("C",        denora_event_join); addCoreMessage(IRCD,m);
    }
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("H",        denora_event_kick); addCoreMessage(IRCD,m);
    }
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage(".",        denora_event_kill); addCoreMessage(IRCD,m);
    }
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("G",        denora_event_mode); addCoreMessage(IRCD,m);
    }
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("F",        denora_event_motd); addCoreMessage(IRCD,m);
    }
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("&",        denora_event_nick); addCoreMessage(IRCD,m);
    }
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("B",        denora_event_notice); addCoreMessage(IRCD,m);
    }
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("D",        denora_event_part); addCoreMessage(IRCD,m);
    }
    m = createMessage("PING",      denora_event_ping); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("8",        denora_event_ping); addCoreMessage(IRCD,m);
    }
    m = createMessage("PRIVMSG",   denora_event_privmsg); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("!",        denora_event_privmsg); addCoreMessage(IRCD,m);
    }
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage(",",        denora_event_quit); addCoreMessage(IRCD,m);
    }
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("'",        denora_event_server); addCoreMessage(IRCD,m);
    }
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("-",        denora_event_squit); addCoreMessage(IRCD,m);
    }
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage(")",        denora_event_topic); addCoreMessage(IRCD,m);
    }
    m = createMessage("USER",      denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("%",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("WALLOPS",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("=",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("WHOIS",     denora_event_whois); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("#",        denora_event_whois); addCoreMessage(IRCD,m);
    }
    m = createMessage("AKILL",     denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("V",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("GLOBOPS",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("]",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("GNOTICE",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("Z",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("GOPER",     denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("[",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("RAKILL",    denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("Y",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SILENCE",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("U",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SVSKILL",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("h",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SVSMODE",   denora_event_svsmode); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("n",        denora_event_svsmode); addCoreMessage(IRCD,m);
    }
    m = createMessage("SVS2MODE",   denora_event_svs2mode); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("v",       denora_event_svs2mode); addCoreMessage(IRCD,m);
    }
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("e",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SVSNOOP",   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("f",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SQLINE",    denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("c",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("UNSQLINE",  denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("d",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("PROTOCTL",  denora_event_capab); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("_",        denora_event_capab); addCoreMessage(IRCD,m);
    }
    m = createMessage("CHGHOST",   denora_event_chghost); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AL",        denora_event_chghost); addCoreMessage(IRCD,m);
    }
    m = createMessage("CHGIDENT",  denora_event_chgident); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AZ",        denora_event_chgident); addCoreMessage(IRCD,m);
    }
    m = createMessage("CHGNAME",   denora_event_chgname); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("BK",        denora_event_chgname); addCoreMessage(IRCD,m);
    }
    m = createMessage("NETINFO",   denora_event_netinfo); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AO",       denora_event_netinfo); addCoreMessage(IRCD,m);
    }
    m = createMessage("SETHOST",   denora_event_sethost); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AA",        denora_event_sethost); addCoreMessage(IRCD,m);
    }
    m = createMessage("SETIDENT",  denora_event_setident); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AD",        denora_event_setident); addCoreMessage(IRCD,m);
    }
    m = createMessage("SETNAME",   denora_event_setname); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AE",        denora_event_setname); addCoreMessage(IRCD,m);
    }
    m = createMessage("TKL", 	   denora_event_tkl); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("BD",       denora_event_tkl); addCoreMessage(IRCD,m);
    }
    m = createMessage("EOS", 	   denora_event_eos); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("ES",       denora_event_eos); addCoreMessage(IRCD,m);
    }
    m = createMessage("PASS", 	   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("<",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("ERROR", 	   denora_event_error); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("5",        denora_event_error); addCoreMessage(IRCD,m);
    }
    m = createMessage("SMO", 	   denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("AU",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("UMODE2",    denora_event_umode2); addCoreMessage(IRCD,m);
    if (UseTokens) {
     m = createMessage("|",        denora_event_umode2); addCoreMessage(IRCD,m);
    }
    m = createMessage("SWHOIS",    denora_event_swhois); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("BA",        denora_event_swhois); addCoreMessage(IRCD,m);
    }
    m = createMessage("SJOIN",      denora_event_sjoin); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("~",        denora_event_sjoin); addCoreMessage(IRCD,m);
    }
    m = createMessage("REHASH",     denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("O",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("ADMIN",      denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("@",        denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("CREDITS",    denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AJ",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SDESC",      denora_event_sdesc); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AG",       denora_event_sdesc); addCoreMessage(IRCD,m);
    }
    m = createMessage("HTM",        denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("BH",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("HELP",        denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("4",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("TRACE",        denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("b",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("LAG",        denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AF",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("RPING",        denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AM",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SENDSNO",    denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("Ss",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("SENDUMODE",  denora_event_null); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("AP",       denora_event_null); addCoreMessage(IRCD,m);
    }
    m = createMessage("PONG",       denora_event_pong); addCoreMessage(IRCD,m);
    if (UseTokens) {
      m = createMessage("9",        denora_event_pong); addCoreMessage(IRCD,m);
    }
    /* The none token version of these is in messages.c */
    if (UseTokens) {
     m = createMessage("2",         m_stats); addCoreMessage(IRCD,m);
     m = createMessage(">",         m_time); addCoreMessage(IRCD,m);
     m = createMessage("+",         m_version); addCoreMessage(IRCD,m);
    }
}

int denora_event_svs2mode(char *source, int ac, char **av)
{
    char *modes = av[1];
	char *tbuf = NULL;
	char buf[256];
	char modebuf[256];
	char *newav[4];
	int add = 0;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 2) {
        return MOD_CONT;
    }
	 /* yes Unreal sucks - they over lap the user mode of d go figure, want to bitch that this
        cause Denora to be slow see the Unreal people and tell them having two modes that do
        two different things is fucked and thank Jobe1986 for pointing out Unreals fuck up
     */
     while (*modes) {
        switch (*modes) {
        case '+':
            add = 1;
            break;
        case '-':
            add = 0;
            break;
		case 'd':
			/* Must ignore it otherwise Jobe's bug will not be fixed */
			if (ac == 3) {
				break;
			} else {
				if (tbuf) {
					ircsnprintf(buf, sizeof(buf), "%s%c", tbuf, *modes);
					free(tbuf);
					tbuf = sstrdup(buf);
				} else {
					ircsnprintf(buf, sizeof(buf), "%c", *modes);
					tbuf = sstrdup(buf);
				}
			}
		default:
			if (tbuf) {
				ircsnprintf(buf, sizeof(buf), "%s%c", tbuf, *modes);
				free(tbuf);
				tbuf = sstrdup(buf);
			} else {
				ircsnprintf(buf, sizeof(buf), "%c", *modes);
				tbuf = sstrdup(buf);
			}
			break;
		}
		(void) *modes++;
	}
	if (tbuf) {
		ircsnprintf(modebuf, sizeof(modebuf), "%s%s", (add ? "+" : "-"), tbuf);
		free(tbuf);
	   	newav[0] = av[0];
   		newav[1] = sstrdup(modebuf);
	   	newav[2] = av[2];
       	do_svsumode(ac, newav);
		if (newav[1]) {
		   	free(newav[1]);
		}
	}
    return MOD_CONT;
}

/* *INDENT-ON* */

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(quakeircd_cmd_nick);
    pmodule_cmd_mode(quakeircd_cmd_mode);
    pmodule_cmd_bot_nick(quakeircd_cmd_bot_nick);
    pmodule_cmd_notice(quakeircd_cmd_notice);
    pmodule_cmd_privmsg(quakeircd_cmd_privmsg);
    pmodule_cmd_serv_notice(quakeircd_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(quakeircd_cmd_serv_privmsg);
    pmodule_cmd_quit(quakeircd_cmd_quit);
    pmodule_cmd_pong(quakeircd_cmd_pong);
    pmodule_cmd_join(quakeircd_cmd_join);
    pmodule_cmd_part(quakeircd_cmd_part);
    pmodule_cmd_global(quakeircd_cmd_global);
    pmodule_cmd_squit(quakeircd_cmd_squit);
    pmodule_cmd_connect(quakeircd_cmd_connect);
    pmodule_cmd_eob(quakeircd_cmd_eob);
    pmodule_cmd_ctcp(quakeircd_cmd_ctcp);
    pmodule_cmd_version(quakeircd_cmd_version);
    pmodule_cmd_stats(quakeircd_cmd_stats);
    pmodule_cmd_motd(quakeircd_cmd_motd);
    pmodule_cmd_ping(quakeircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion("");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("QuakeIRCD");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_irc_var(IRC_QUAKEIRCD);
    pmodule_ircd_useTSMode(0);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}
