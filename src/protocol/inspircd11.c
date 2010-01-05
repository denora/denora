
/* InspIRCd 1.1 functions
 *
 * (C) 2005-2006 Craig Edwards <brain@inspircd.org>
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
 * last tested with InspIRCd 1.1.15
 *
 */

/*************************************************************************/

#include "denora.h"
#include "inspircd11.h"

IRCDVar myIrcd[] = {
    {"InspIRCd 1.1.x",          /* ircd name                    */
     "+ioS",                    /* StatServ mode                */
     IRCD_ENABLE,               /* Vhost                        */
     IRCD_DISABLE,              /* Supports SGlines             */
     IRCD_DISABLE,              /* sgline sql table             */
     IRCD_DISABLE,              /* Supports SQlines             */
     IRCD_DISABLE,              /* sqline sql table             */
     IRCD_DISABLE,              /* Supports SZlines             */
     IRCD_ENABLE,               /* Has exceptions +e            */
     IRCD_ENABLE,               /* vidents                      */
     IRCD_ENABLE,               /* NICKIP                       */
     IRCD_ENABLE,               /* VHOST ON NICK                */
     IRCD_ENABLE,               /* +f                           */
     IRCD_ENABLE,               /* +j                           */
     IRCD_ENABLE,               /* +L                           */
     CMODE_f,                   /* +f Mode                      */
     CMODE_j,                   /* +j Mode                      */
     CMODE_L,                   /* +L Mode                      */
     NULL,                      /* CAPAB Chan Modes             */
     IRCD_DISABLE,              /* We support Unreal TOKENS     */
     IRCD_DISABLE,              /* TOKENS are CASE Sensitive    */
     IRCD_ENABLE,               /* TIME STAMPS are BASE64       */
     IRCD_ENABLE,               /* +I support                   */
     IRCD_DISABLE,              /* SJOIN ban char               */
     IRCD_DISABLE,              /* SJOIN except char            */
     IRCD_DISABLE,              /* SJOIN invite char            */
     UMODE_x,                   /* umode for vhost              */
     IRCD_ENABLE,               /* channel owner                */
     IRCD_ENABLE,               /* channel mode protect         */
     IRCD_ENABLE,               /* channel mode halfop          */
     NULL,
     NULL,
     'f',                       /* flood                        */
     'j',                       /* flood other                  */
     'J',                       /* join throttle                */
     IRCD_DISABLE,              /* nick change flood            */
     'x',                       /* vhost                        */
     IRCD_DISABLE,              /* vhost other                  */
     'L',                       /* channek linking              */
     IRCD_DISABLE,              /* p10 protocol                 */
     IRCD_DISABLE,              /* ts6 protocol                 */
     IRCD_DISABLE,              /* numeric ie.. 350 etc         */
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
     IRCD_ENABLE,               /* hidden oper                  */
     IRCD_DISABLE,              /* extra warning                */
     IRCD_ENABLE,               /* Report sync state            */
     IRCD_DISABLE               /* Persistent channel mode   */
     }
    ,
};


IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     0,                         /* TSMODE       */
     1,                         /* UNCONNECT    */
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
     CAPAB_SSJ3,                /* SSJ3         */
     CAPAB_NICK2,               /* NICK2        */
     0,                         /* UMODE2       */
     CAPAB_VL,                  /* VL           */
     CAPAB_TLKEXT,              /* TLKEXT       */
     0,                         /* DODKEY       */
     0,                         /* DOZIP        */
     0,
     0,
     0,
     }
};


/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_B, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_H, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_I, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_Q, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_W, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_c, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_F, NULL, NULL);
    CreateChanMode(CMODE_G, NULL, NULL);
    CreateChanMode(CMODE_J, set_rejoinlock, get_rejoinlock);
    CreateChanMode(CMODE_K, NULL, NULL);
    CreateChanMode(CMODE_L, set_redirect, get_redirect);
    CreateChanMode(CMODE_M, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_P, NULL, NULL);
    CreateChanMode(CMODE_Q, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_T, NULL, NULL);
    CreateChanMode(CMODE_V, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
    CreateChanMode(CMODE_f, set_flood, get_flood);
    CreateChanMode(CMODE_g, NULL, NULL);
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_j, set_flood_alt, get_flood_alt);
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
    ModuleSetChanUMode('&', 'a', STATUS_PROTECTED);
    ModuleSetChanUMode('~', 'q', STATUS_OWNER);

    ModuleUpdateSQLChanMode();

}


/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("ADDLINE",   denora_event_addline); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("BURST",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB",     denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("CHGHOST",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CHGIDENT",  denora_event_chgident); addCoreMessage(IRCD,m);
    m = createMessage("CHGNAME",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ELINE",     denora_event_eline); addCoreMessage(IRCD,m);
    m = createMessage("ENDBURST",  denora_event_eob); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("FHOST",     denora_event_fhost); addCoreMessage(IRCD,m);
    m = createMessage("FJOIN",     denora_event_fjoin); addCoreMessage(IRCD,m);
    m = createMessage("FMODE",     denora_event_fmode); addCoreMessage(IRCD,m);
    m = createMessage("FNAME",     denora_event_fname); addCoreMessage(IRCD,m);
    m = createMessage("FTOPIC",    denora_event_ftopic); addCoreMessage(IRCD,m);
    m = createMessage("GLINE",     denora_event_gline); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("IDLE",      denora_event_idle); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("METADATA",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MODENOTICE",denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("OPERNOTICE",denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("OPERQUIT",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("OPERTYPE",  denora_event_opertype); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("PING",      denora_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   denora_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("PUSH",      denora_event_push); addCoreMessage(IRCD,m);
    m = createMessage("QLINE",     denora_event_qline); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SAJOIN",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SAMODE",    denora_event_svsmode); addCoreMessage(IRCD,m);
    m = createMessage("SANICK",    denora_event_sanick); addCoreMessage(IRCD,m);
    m = createMessage("SAPART",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SETHOST",   denora_event_fhost); addCoreMessage(IRCD,m);
    m = createMessage("SETIDENT",  denora_event_setident); addCoreMessage(IRCD,m);
    m = createMessage("SETNAME",   denora_event_fname); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SNONOTICE", denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("RSQUIT",    denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("SVSHOLD",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSJOIN",   denora_event_svsjoin); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_svsmode); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("VERSION",   denora_event_version); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     denora_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("ZLINE",     denora_event_zline); addCoreMessage(IRCD,m);

}

/* *INDENT-ON* */

int denora_event_push(char *source, int ac, char **av)
{
/* Thanks to w00t from the inspircd team for helping me to write this function
 * debug: Received: :rock.musichat.net PUSH TestBOT ::rock.musichat.net 242 TestBOT :Server up 1 days, 07:47:54
 */
    Server *s;
    char *num;
    char buf[NET_BUFSIZE];

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    num = myStrGetToken(av[1], ' ', 1);
    av[1] = myStrGetTokenRemainder(av[1], ' ', 3);

    if (!num || !av[1]) {
        alog(LOG_ERROR,
             "ERROR: Something wicked while handling the PUSH message (truncated message)");
        return MOD_CONT;
    }

    if (!strcmp(num, "375")) {
        rdb_query(QUERY_LOW, "UPDATE %s SET motd=\'\' WHERE server=\'%s\'",
                  ServerTable, source);
    } else if (!strcmp(num, "372")) {
        s = server_find(source);
        if (!s) {
            return MOD_CONT;
        }
        av[1]++;
        if (ac >= 2) {
            if (s->motd) {
                ircsnprintf(buf, NET_BUFSIZE - 1, "%s\n\r%s", s->motd,
                            av[1]);
                free(s->motd);
                s->motd = sstrdup(buf);
            } else {
                s->motd = sstrdup(av[1]);
            }
        }
    } else if (!strcmp(num, "376")) {
        s = server_find(source);
        if (!s) {
            return MOD_CONT;
        }
        sql_motd_store(s);
    } else if (!strcmp(num, "242")) {
        av[1]++;
        sql_do_uptime(source, av[1]);
    } else if (!strcmp(num, "248")) {
        av[2] = myStrGetTokenRemainder(av[1], ' ', 1);
        av[1] = myStrGetToken(av[1], ' ', 1);   /* possible memleak at this location */
        sql_uline(av[2]);
    }
    return MOD_CONT;
}

int denora_event_eob(char *source, int ac, char **av)
{
    update_sync_state(denora->uplink, SYNC_COMPLETE);
    return MOD_CONT;
}

int denora_event_sanick(char *source, int ac, char **av)
{
    do_nick(av[0], av[1], NULL, NULL, NULL,
            NULL, (int) time(NULL), 0, NULL, NULL, NULL, 0, NULL, NULL);
    return MOD_CONT;
}

int denora_event_svsmode(char *source, int ac, char **av)
{
    denora_event_mode(av[0], 2, av);
    return MOD_CONT;
}

/*
[Dec 30 16:03:09.300934 2005] Source inspircd.nomadirc.net
[Dec 30 16:03:09.300990 2005] av[0] = Q
[Dec 30 16:03:09.301046 2005] av[1] = ChanServ
[Dec 30 16:03:09.301102 2005] av[2] = <Config>
[Dec 30 16:03:09.301158 2005] av[3] = 1135968312
[Dec 30 16:03:09.301214 2005] av[4] = 0
[Dec 30 16:03:09.301270 2005] av[5] = Reserved For Services

*/
int denora_event_addline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur, timeset;
    char buf[BUFSIZE];
    checkdur = atoi(av[4]);
    timeset = atoi(av[3]);

    if (checkdur != 0) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) (checkdur + timeset));
    } else {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) checkdur);
    }

    if (!stricmp(av[0], "G") || !stricmp(av[0], "E")) {
        user = myStrGetToken(av[1], '@', 0);
        host = myStrGetToken(av[1], '@', 1);
        sql_do_server_bans_add(av[0], user, host, av[2], av[3], buf,
                               av[5]);
        if (user)
            free(user);
        if (host)
            free(host);
    } else if (!stricmp(av[0], "Q")) {
        sql_do_server_bans_add(av[0], (char *) "*", av[1], av[2], av[3],
                               buf, av[5]);
    } else {
        sql_do_server_bans_add(av[0], (char *) "", av[1], av[2], av[3],
                               buf, av[5]);
    }
    return MOD_CONT;
}

/* :<source nickname> GLINE <user@host> <duration> :<reason> */

int denora_event_gline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    user = myStrGetToken(av[0], '@', 0);
    host = myStrGetToken(av[0], '@', 1);

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "G", user, host, source, buf, buf2,
                               av[2]);
    } else {
        sql_do_server_bans_remove((char *) "G", user, host);
    }
    return MOD_CONT;
}

int denora_event_eline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    user = myStrGetToken(av[0], '@', 0);
    host = myStrGetToken(av[0], '@', 1);

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "E", user, host, source, buf, buf2,
                               av[2]);
    } else {
        sql_do_server_bans_remove((char *) "E", user, host);
    }
    if (user)
        free(user);
    if (host)
        free(host);
    return MOD_CONT;
}

int denora_event_zline(char *source, int ac, char **av)
{
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "Z", (char *) "", av[0], source,
                               buf, buf2, av[2]);
    } else {
        sql_do_server_bans_remove((char *) "Z", (char *) "", av[0]);
    }
    return MOD_CONT;
}

int denora_event_qline(char *source, int ac, char **av)
{
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "Q", (char *) "*", av[0], source,
                               buf, buf2, av[2]);
    } else {
        sql_do_server_bans_remove((char *) "Q", (char *) "*", av[0]);
    }
    return MOD_CONT;
}

int denora_event_ftopic(char *source, int ac, char **av)
{
    /* :source FTOPIC channel ts setby :topic */
    char *temp;
    if (ac < 4)
        return MOD_CONT;
    temp = av[1];               /* temp now holds ts */
    av[1] = av[2];              /* av[1] now holds set by */
    av[2] = temp;               /* av[2] now holds ts */
    do_topic(ac, av);
    return MOD_CONT;
}

int denora_event_version(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    av[0] = strtok(av[0], " ");
    sql_do_server_version(source, ac, av);
    return MOD_CONT;
}

int denora_event_opertype(char *source, int ac, char **av)
{
    /* opertype is equivalent to mode +o because servers
       dont do this directly */
    char *newav[2];
    newav[0] = source;
    newav[1] = (char *) "+o";
    return denora_event_mode(source, 2, newav);
}

int has_globopsmod = 0;
/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
    int argc = 5;
    char *argv[5];

    if (strcasecmp(av[0], "START") == 0) {
        /* reset CAPAB */
        has_globopsmod = 0;
    } else if (strcasecmp(av[0], "MODULES") == 0
               && strstr(av[1], "m_globops.so")) {
        has_globopsmod = 1;
    } else if (strcasecmp(av[0], "END") == 0) {
        if (has_globopsmod == 0) {
            send_cmd(NULL,
                     "ERROR :m_globops.so is not loaded. This is required by Denora");
            denora->qmsg =
                sstrdup
                ("Remote server does not have the m_globops.so module loaded, and this is required.");
            denora->quitting = 1;
            return MOD_STOP;
        }

        /* Generate a fake capabs parsing call so things like NOQUIT work
         * fine. It's ugly, but it works....
         */
        argv[0] = sstrdup("NOQUIT");
        argv[1] = sstrdup("SSJ3");
        argv[2] = sstrdup("NICK2");
        argv[3] = sstrdup("VL");
        argv[4] = sstrdup("TLKEXT");

        capab_parse(argc, argv);

        free(argv[0]);
        free(argv[1]);
        free(argv[2]);
        free(argv[3]);
        free(argv[4]);
    }
    return MOD_CONT;
}

void inspircd_cmd_nick(char *nick, char *name, const char *modes)
{
    /* :test.chatspike.net NICK 1133519355 Brain synapse.brainbox.winbot.co.uk netadmin.chatspike.net ~brain +xwsioS 10.0.0.2 :Craig Edwards */
    send_cmd(ServerName, "NICK %ld %s %s %s %s +%s 0.0.0.0 :%s",
             (long int) time(NULL), nick, ServiceHost, ServiceHost,
             ServiceUser, modes, name);
    send_cmd(nick, "OPERTYPE Service");
}

void inspircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                           char *modes)
{
    send_cmd(ServerName, "NICK %ld %s %s %s %s +%s 0.0.0.0 :%s",
             (long int) time(NULL), nick, host, host, user, modes, real);
    send_cmd(nick, "OPERTYPE Bot");
}

void inspircd_cmd_notice(char *source, char *dest, char *buf)
{
    if (!buf) {
        return;
    }
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void inspircd_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

void inspircd_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void inspircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void inspircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void inspircd_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT :Leaving");
    }
}

/* A Local kill message is received by Denora like this:
 * :Hal9001 QUIT :Killed (Hal9000 (DEATH TO HIP HOP!!!))
*/
int inspircd11_parse_lkill(char *message)
{
    const char *localkillmsg = "Killed (";

    /* is it a Local kill message? */
    if ((strncmp(message, QuitPrefix, strlen(QuitPrefix)) != 0)
        && (strstr(message, localkillmsg) != NULL))
        return 1;

    return 0;
}

char *inspircd11_lkill_killer(char *message)
{
    char *buf, *killer = NULL;

    /* Let's get the killer nickname */
    buf = sstrdup(message);
    killer = strtok(buf, " ");
    killer = strtok(NULL, " ");
    killer++;

    return killer;
}

/* Killed (Hal9000 (DEATH TO HIP HOP!!!)) */
char *inspircd11_lkill_msg(char *message)
{
    char *msg = NULL;

    /* Let's get the kill message */
    msg = strchr(message, '(');
    msg = strchr(message, '(');
    msg[strlen(msg) - 2] = '\0';
    msg++;                      /* removes first character '(' */

    return msg;
}

/* PROTOCTL */
void inspircd_cmd_capab()
{
}

static char currentpass[1024];

/* PASS */
void inspircd_cmd_pass(char *pass)
{
    strncpy(currentpass, pass, 1024);
}

/* SERVER name hop descript */
void inspircd_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(ServerName, "SERVER %s %s %d :%s", servname, currentpass,
             hop + 1, descript);
}

/* PONG */
void inspircd_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/* JOIN */
void inspircd_cmd_join(char *user, char *channel, time_t chantime)
{
    alog(LOG_PROTOCOL, "User %s joins %s at %ld", user, channel,
         (long int) chantime);
    send_cmd(user, "JOIN %s", channel);
}

/* PART */
void inspircd_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s :Leaving", chan);
    }
}

/* GLOBOPS */
void inspircd_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

/* SQUIT */
void inspircd_cmd_squit(char *servname, char *message)
{
    send_cmd(ServerName, "SQUIT %s :%s", servname, message);
}

void inspircd_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    inspircd_cmd_capab();
    send_cmd(NULL, "SERVER %s %s %d :%s", ServerName, RemotePassword, 0,
             ServerDesc);
    send_cmd(NULL, "BURST");
    send_cmd(ServerName,
             "VERSION :Denora-%s %s :%s -- build #%s, compiled %s %s",
             denora->version, ServerName, ircd->name, denora->build,
             denora->date, denora->time);
}

/* Events */
int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1) {
        return MOD_CONT;
    }
    inspircd_cmd_pong(ac > 1 ? av[0] : ServerName, source);
    return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
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

/* Normal RFC style topic: :source TOPIC chan :topic */
int denora_event_topic(char *source, int ac, char **av)
{
    char *newav[127];

    if (ac < 2) {
        return MOD_CONT;
    }

    newav[0] = sstrdup(av[0]);
    newav[1] = sstrdup(source);
    newav[2] = itostr(time(NULL));
    newav[3] = sstrdup(av[1]);
    do_topic(4, newav);
    if (newav[0]) {
        free(newav[0]);
    }
    if (newav[1]) {
        free(newav[1]);
    }
    if (newav[3]) {
        free(newav[3]);
    }
    return MOD_CONT;
}

/* :source SQUIT dest :reason */
int denora_event_squit(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    do_squit(av[0]);
    return MOD_CONT;
}

int denora_event_quit(char *source, int ac, char **av)
{
    char *killer = NULL;
    char *msg = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 1)
        return MOD_CONT;

    if (inspircd11_parse_lkill(av[0]) == 0) {
        do_quit(source, ac, av);
    } else {
        killer = inspircd11_lkill_killer(av[0]);
        msg = inspircd11_lkill_msg(av[0]);

        if (killer)
            m_kill(killer, source, msg);
        else
            m_kill(source, source, msg);
    }

    return MOD_CONT;
}

/*
 * :irc.mynet.org MODE #opers +bb *!user@spy.com *!person@watching.us
 * :<source server or nickname> MODE <target> <modes and parameters>
 */

int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 2)
        return MOD_CONT;
    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(av[0], ac, av);
    } else {
        do_umode(av[0], ac, av);
    }

    return MOD_CONT;
}

/*
 * :irc.mynet.org FMODE #opers 115432135 +bb *!user@spy.com *!person@watching.us
 * :<source server or nickname> FMODE <target> <timestamp> <modes and parameters>
 */
int denora_event_fmode(char *source, int ac, char **av)
{
    char *newav[127];
    int i = 0;
    Channel *c;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 3)
        return MOD_CONT;

    /* Checking the TS for validity to avoid desyncs */
    c = findchan(av[0]);
    if (c) {
        if (c->creation_time > strtol(av[1], NULL, 10)) {
            alog(LOG_DEBUG,
                 "DEBUG: av[1] %d < c->creation_time %d, c->creation_time lowered to %d",
                 av[1], c->creation_time, av[1]);
            c->creation_time = strtol(av[1], NULL, 10);
        } else if (c->creation_time < strtol(av[1], NULL, 10)) {
            alog(LOG_DEBUG,
                 "DEBUG: av[1] %d > c->creation_time %d, FMODE ignored.",
                 av[1], c->creation_time);
            return MOD_CONT;
        }
    } else {
        alog(LOG_DEBUG, "DEBUG: got FMODE for non-existing channel %s",
             av[0]);
        return MOD_CONT;
    }

    /* We need to remove the timestamp, which is av[1] */
    newav[0] = av[0];
    newav[1] = av[2];
    for (i = 2; i < (ac - 1); i++) {
        newav[i] = av[i + 1];
    }

    if (*newav[0] == '#' || *newav[0] == '&') {
        do_cmode(source, (ac - 1), newav);
    } else {
        do_umode(source, (ac - 1), newav);
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
    if (ac != 2)
        return MOD_CONT;
    do_join(source, ac, av);
    return MOD_CONT;
}

int denora_event_svsjoin(char *source, int ac, char **av)
{
    char *newav[127];

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
        return MOD_CONT;

    newav[0] = av[1];
    do_join(av[0], 1, newav);
    return MOD_CONT;
}

/* We implement this, but for now, InspIRCd doesnt
 * support remote MOTD.
 */

int denora_event_motd(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_motd(source);
    return MOD_CONT;
}

/* FHOST and FNAME are equivalent to SETHOST and SETNAME,
 * CHGHOST and CHGNAME generate FHOST and FNAME commands
 * and are therefore redundant
 */

int denora_event_fname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(source, av[0]);
    return MOD_CONT;
}

int denora_event_fhost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(source, av[0]);
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

    if (ac != 2)
        return MOD_CONT;

    change_user_username(av[0], av[1]);
    return MOD_CONT;
}

/* FJOIN is like SJOIN, but not quite. It doesnt sync
 * simple-modes, or bans/excepts.
 */
int denora_event_fjoin(char *source, int ac, char **av)
{
    char *newav[127];
    char people[1024];
    int i = 0;
    char *userv[256];
    int userc = 0;
    int nlen = 0;
    char prefixandnick[51];

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 3)
        return MOD_CONT;

    newav[0] = av[1];           /* Timestamp */
    newav[1] = av[0];           /* Channel */
    newav[2] = (char *) "+";    /* Mode */
    newav[3] = people;          /* Nickname */

    *people = '\0';

    /*
     * We need to remove the comma and ignore unknown modes.
     * This code is based on work by w00t for atheme.
     */
    userc = sjtoken(av[ac - 1], ' ', userv);

    /* loop over all the users in this fjoin */
    for (i = 0; i < userc; i++) {
        nlen = 0;

        alog(LOG_DEBUG, "denora_event_fjoin(): processing user: %s",
             userv[i]);

        for (; *userv[i]; userv[i]++) {
            /* does this char match a known prefix? */
            if (csmodes[(int) *userv[i]]) {
                prefixandnick[nlen++] = *userv[i];
                continue;
            }

            /* have we reached the end of the prefixes? */
            if (*userv[i] == ',') {
                /* yup, skip over the comma */
                userv[i]++;

                /* create nick with prefixes */
                strlcpy(prefixandnick + nlen, userv[i],
                        sizeof(prefixandnick) - nlen);
                /* add the user */
                strncat(people, prefixandnick, sizeof(people) - 1);
                strncat(people, " ", sizeof(people) - 1);

                /* break out of this loop, which will move us to the next user */
                break;
            }
        }
    }

    do_sjoin(source, 4, newav);

    return MOD_CONT;
}

int denora_event_nick(char *source, int ac, char **av)
{
    User *user;
    char *ptr;
    char buf[BUFSIZE];
    char *ptr2 = buf;

    if (denora->protocoldebug)
        protocol_debug(source, ac, av);

    if (ac != 1) {
        if (ac == 8) {
            int svid = 0;
            int ts = strtoul(av[0], NULL, 10);
            if (strchr(av[5], 'r') != NULL)
                svid = ts;

            /* Here we should check if av[5] contains +o, and if so remove it,
             * as this will be handled by OPERTYPE */
            ptr = av[5];
            while (ptr && *ptr) {
                if (*ptr != 'o') {
                    /* not o, add it to the clean list */
                    *ptr2 = *ptr;
                    ptr2++;
                }
                /* increment original */
                ptr++;
            }
            *ptr2 = '\0';
            av[5] = (!strcmp(buf, "++")) ? NULL : buf;

            user = do_nick("", av[1],   /* nick */
                           av[4],       /* username */
                           av[2],       /* realhost */
                           source,      /* server */
                           av[7],       /* realname */
                           ts, svid, av[6], av[3], NULL, 1, av[5], NULL);
        }
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                0, 0, NULL, NULL, NULL, 0, NULL, NULL);
    }

    return MOD_CONT;
}


/* EVENT: SERVER */
/*
 av[0] = inspircd.nomadirc.net
 av[1] = linkpass
 av[2] = 0
 av[3] = Waddle World
*/
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[2], "0")) {
        denora->uplink = sstrdup(av[0]);
    }
    do_server(source, av[0], av[2], av[3], NULL);
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
    if (source && ac >= 1) {
        m_whois(source, av[0]);
    }
    return MOD_CONT;
}

/* In InspIRCd, instead of sending back the entire WHOIS reply
 * as numerics on remote WHOIS, the remote server sends back
 * a reply to the IDLE command, which returns the users signon
 * and idle times, as these are the only different pieces of
 * information given between remote and local whois
 */
int denora_event_idle(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac == 1) {
        send_cmd(av[0], "IDLE %s %ld 0", source, (long int) time(NULL));
    }

    return MOD_CONT;
}

void inspircd_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

void inspircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void inspircd_cmd_version(char *server)
{
    /* TODO: InspIRCd sends you all servers version strings as they burst.
     * These can be cached, rather than having to request them.
     */
}

void inspircd_cmd_motd(char *sender, char *server)
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

void inspircd_cmd_mode(char *source, char *dest, char *buf)
{
    Channel *c;
    if (!buf) {
        return;
    }

    c = findchan(dest);
    send_cmd(source ? source : s_StatServ, "FMODE %s %u %s", dest,
             (unsigned int) ((c) ? c->creation_time : time(NULL)), buf);
}

void inspircd_cmd_eob(void)
{
    send_cmd(NULL, "ENDBURST");
}

int sjtoken(char *message, char delimiter, char **parv)
{
    char *next;
    unsigned count;

    if (!message)
        return 0;

    /* now we take the beginning of the message and find all the spaces...
     * set them to \0 and use 'next' to go through the string
     */
    next = message;

    /* eat any additional delimiters */
    while (*next == delimiter)
        next++;

    parv[0] = next;
    count = 1;

    while (*next) {
        /* this is fine here, since we don't have a :delimited
         * parameter like tokenize
         */

        if (count == 256) {
            /* we've reached our limit */
            alog(LOG_DEBUG, "sjtokenize(): reached param limit");
            return count;
        }

        if (*next == delimiter) {
            *next = '\0';
            next++;
            /* eat any additional delimiters */
            while (*next == delimiter)
                next++;
            /* if it's the end of the string, it's simply
             ** an extra space at the end.  here we break.
             */
            if (*next == '\0')
                break;

            /* if it happens to be a stray \r, break too */
            if (*next == '\r')
                break;

            parv[count] = next;
            count++;
        } else
            next++;
    }

    return count;
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_eob(inspircd_cmd_eob);
    pmodule_cmd_nick(inspircd_cmd_nick);
    pmodule_cmd_mode(inspircd_cmd_mode);
    pmodule_cmd_bot_nick(inspircd_cmd_bot_nick);
    pmodule_cmd_notice(inspircd_cmd_notice);
    pmodule_cmd_privmsg(inspircd_cmd_privmsg);
    pmodule_cmd_serv_notice(inspircd_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(inspircd_cmd_serv_privmsg);
    pmodule_cmd_quit(inspircd_cmd_quit);
    pmodule_cmd_pong(inspircd_cmd_pong);
    pmodule_cmd_join(inspircd_cmd_join);
    pmodule_cmd_part(inspircd_cmd_part);
    pmodule_cmd_global(inspircd_cmd_global);
    pmodule_cmd_squit(inspircd_cmd_squit);
    pmodule_cmd_connect(inspircd_cmd_connect);
    pmodule_cmd_ctcp(inspircd_cmd_ctcp);
    pmodule_cmd_version(inspircd_cmd_version);
    pmodule_cmd_stats(inspircd_cmd_stats);
    pmodule_cmd_motd(inspircd_cmd_motd);
    pmodule_cmd_ping(inspircd_cmd_ping);
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
    moduleAddVersion
        ("$Id$");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("InspIRCd 1.1.x");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    pmodule_irc_var(IRC_INSPIRCD11);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}
