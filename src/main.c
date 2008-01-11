/* Stats -- main source file.
 *
 * © 2004-2008 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id$
 *
 */

#include "denora.h"

/******** Global variables! ********/

/* Input buffer - global, so we can dump it if something goes wrong */
char inbuf[BUFSIZE];

/* Socket for talking to server */
int servsock = -1;

/* Parameters */
char **my_av;

/* Set to 1 if we are waiting for input */
int waiting = 0;

/* Set to 1 after we've set everything up */
int started = 0;

int SpeedTest;

/*************************************************************************/

DenoraVar denora[] = {
    {STATS_DIR,                 /* Stats Dir */
     LOG_FILENAME,              /* Log File Name */
     STATS_CONF,                /* Config file Name */
     VERSION_STRING,            /* Version string */
     VERSION_STRING_DOTTED,     /* Version string dotted */
     STATS_BUILD,               /* Version flags OS */
     VER_SQL,                   /* Version flags SQL */
     VER_MODULE,                /* Version flag Module */
     NULL,                      /* Version Protocol */
     __DATE__,                  /* Date Compiled */
     __TIME__,                  /* Time Compiled */
     BUILD,                     /* Build Number */
     NULL,                      /* quit message */
     0,                         /* time started */
     0,                         /* sql code in use */
     0,                         /* sql debug */
     NULL,                      /* uplink server */
     0,                         /* capab of our uplink */
     0,                         /* quitting */
     0,                         /* delayed quit */
     0,                         /* html output */
     0,                         /* socket debug */
     0,                         /* Debug */
     0,                         /* nofork */
     0,                         /* Should we update the databases now? */
     0,                         /* protocol debug */
     }
};

/*************************************************************************/

STATVar stats[] = {
    {0,                         /* users */
     0,                         /* opers */
     0,                         /* channels */
     0,                         /* servers */
     0,                         /* away */
     0,                         /* user max */
     0,                         /* user time */
     0,                         /* oper max */
     0,                         /* oper time */
     0,                         /* chan max */
     0,                         /* chan time */
     0,                         /* server max */
     0,                         /* servers_max_time */
     0,                         /* daily_users */
     0,                         /* daily_users_time */
     0,                         /* daily_opers */
     0,                         /* daily_opers_time */
     0,                         /* daily_servers */
     0,                         /* daily_servers_time */
     0,                         /* daily_chans */
     0,                         /* daily_chans_time */
     0,                         /* totalusersever */
     }
};

/*************************************************************************/

/* process:  Main processing routine.  Takes the string in inbuf (global
 *           variable) and does something appropriate with it. */

void process()
{
    int retVal = 0;
    Message *msgcurrent = NULL;
    char source[64];
    char cmd[64];
    char buf[513];              /* Longest legal IRC command line */
    char bufp10[513];           /* Longest legal IRC command line */
    char *s;
    int ac;                     /* Parameters for the command */
    char **av;
    Message *m;
    char *p10token;
    int p10parse = 1;
    Server *serv;
    int is_server = 0;

    /* zero out the buffers before we do much else */
    *buf = '\0';
    *source = '\0';
    *cmd = '\0';
    *bufp10 = '\0';

    SET_SEGV_LOCATION();

    /* If debugging, log the buffer */
    alog(LOG_DEBUG, "debug: Received: %s", inbuf);

    /* First make a copy of the buffer so we have the original in case we
     * crash - in that case, we want to know what we crashed on. */
    strlcpy(buf, inbuf, sizeof(buf));

    /* Split the buffer into pieces. */
    if (*buf == ':' || *buf == '@') {
        s = strpbrk(buf, " ");
        if (!s)
            return;
        *s = 0;
        while (isspace(*++s));
        strlcpy(source, buf + 1, sizeof(source));
        memmove(buf, s, strlen(s) + 1);
    } else if (ircd->p10) {
        if (!*buf) {
            return;
        }
        strlcpy(bufp10, buf, sizeof(bufp10));
        p10token = myStrGetToken(bufp10, ' ', 0);
        if (!stricmp(p10token, "SERVER")) {
            p10parse = 0;
        }
        if (!stricmp(p10token, "PASS")) {
            p10parse = 0;
        }
        if (!stricmp(p10token, "ERROR")) {
            p10parse = 0;
        }
        if (!stricmp(p10token, "NOTICE")) {
            p10parse = 0;
        }
        if (p10parse) {
            s = strpbrk(buf, " ");
            if (!s) {
                free(p10token);
                return;
            }
            *s = 0;
            while (isspace(*++s));
            strlcpy(source, buf, sizeof(source));
            memmove(buf, s, strlen(s) + 1);
        }
        if (p10token) {
            free(p10token);
        }
    } else {
        *source = 0;
    }
    serv = server_find(source);
    if (serv) {
        is_server = 1;
    }

    if (!is_server) {
        doCleanBuffer((char *) buf);
    }

    s = strpbrk(buf, " ");
    if (s) {
        *s = 0;
        while (isspace(*++s));
    } else
        s = buf + strlen(buf);
    strlcpy(cmd, buf, sizeof(cmd));
    ac = split_buf(s, &av);
    if (mod_current_buffer) {
        free(mod_current_buffer);
    }
    /* fix to moduleGetLastBuffer() bug 296 */
    /* old logic was that since its meant for PRIVMSG that we would get
       the NICK as AV[0] and the rest would be in av[1], however on Bahamut
       based systems when you do /cs it assumes we will translate the command
       to the NICK and thus AV[0] is the message. The new logic is to check
       av[0] to see if its a service nick if so assign mod_current_buffer the
       value from AV[1] else just assign av[0] - TSL */
    /* First check if the ircd proto module overrides this -GD */
    if (!denora_set_mod_current_buffer(ac, av)) {
        if (ac >= 1 && av[0]) {
            if (nickIsServices(av[0])) {
                if (av[1]) {
                    mod_current_buffer = sstrdup(av[1]);
                } else {
                    mod_current_buffer = sstrdup(av[0]);
                }
            } else {
                mod_current_buffer = sstrdup(av[0]);
            }
        } else {
            mod_current_buffer = NULL;
        }
    }
    /* Do something with the message. */
    m = findMessage(IRCD, cmd);
    total_recmsg++;
    SET_SEGV_LOCATION();
    if (m) {
        if (m->func) {
            mod_current_module_name = m->mod_name;
            retVal = m->func(source, ac, av);
            mod_current_module_name = NULL;
            SET_SEGV_LOCATION();
            if (retVal == MOD_CONT) {
                if (m->next) {
                    msgcurrent = m->next;
                    if (msgcurrent) {
                        SET_SEGV_LOCATION();
                        while (msgcurrent && msgcurrent->func
                               && retVal == MOD_CONT) {
                            mod_current_module_name = msgcurrent->mod_name;
                            retVal = msgcurrent->func(source, ac, av);
                            mod_current_module_name = NULL;
                            msgcurrent = msgcurrent->next;
                        }
                    }
                }
            }
        }
    } else {
        alog(LOG_DEBUG, langstr(ALOG_UNKNWON_MSG), inbuf);
    }

    /* Load/unload modules if needed */
    handleModuleOperationQueue();

    /* Free argument list we created */
    free(av);
}

/*************************************************************************/

/* Restarts denora */

void denora_restart(void)
{
    ChannelStats *cs;
    lnode_t *tn;

    SET_SEGV_LOCATION();
    alog(LOG_NORMAL, langstr(RESTARTING));
    if (!denora->qmsg) {
        denora->qmsg = sstrdup(langstr(RESTARTING));
    }
    send_event(EVENT_RESTART, 1, EVENT_START);
    if (started) {
        if (!LargeNet) {
            tn = list_first(CStatshead);
            while (tn != NULL) {
                cs = lnode_get(tn);
                denora_cmd_part(s_StatServ, cs->name, "%s", denora->qmsg);
                tn = list_next(CStatshead, tn);
            }
        }
        if (LogChannel) {
            denora_cmd_part(s_StatServ, LogChannel, "%s", denora->qmsg);
        }
        denora_cmd_squit(ServerName, denora->qmsg);
    }
    disconn(servsock);
    close_log();
#ifdef _WIN32
    chdir("..");
#endif
#ifdef STATS_BIN
    execv(STATS_BIN, my_av);
#endif
    open_log();
    log_perror("%s", langstr(RESTART_FAILED));
    close_log();
}

/*************************************************************************/
/**
 * Added to allow do_restart from statserv access to the static functions without making them
 * fair game to every other function - not exactly ideal :|
 **/
void do_restart_denora(void)
{
    denora_restart();
    save_databases();
    SET_SEGV_LOCATION();
    exit(1);
}

/*************************************************************************/

/* Terminates denora */

void denora_shutdown(void)
{
    ChannelStats *cs;
    lnode_t *tn;

    SET_SEGV_LOCATION();

    send_event(EVENT_SHUTDOWN, 1, EVENT_START);
    save_databases();

    /* we are going down time to tell 
       some internal functions we are dead */
    denora->delayed_quit = 1;

    if (!denora->qmsg) {
        denora->qmsg = sstrdup("Terminating, reason unknown");
    }
    alog(LOG_NORMAL, "%s", denora->qmsg);
    if (started) {
        if (!LargeNet) {
            tn = list_first(CStatshead);
            while (tn != NULL) {
                cs = lnode_get(tn);
                denora_cmd_part(s_StatServ, cs->name, "%s", denora->qmsg);
                tn = list_next(CStatshead, tn);
            }
        }
        if (LogChannel) {
            denora_cmd_part(s_StatServ, LogChannel, "%s", denora->qmsg);
        }
        denora_cmd_squit(ServerName, denora->qmsg);
    }
    if (XMLRPC_Enable) {
        extsock_close();
    }
    disconn(servsock);
    send_event(EVENT_SHUTDOWN, 1, EVENT_STOP);
    if (started) {
        destroy_all();
    }
    if (denora->do_sql) {
        rdb_close();
    }
#ifndef _WIN32
    adns_finish(adns);
#endif
    if (gi) {
        GeoIP_delete(gi);
    }
    close_log();
    lang_destory();
}

/*************************************************************************/

void save_databases()
{
    send_event(EVENT_DB_SAVING, 1, EVENT_START);

    alog(LOG_DEBUG, langstr(SAVING_FFF));
    save_stats_db();
    save_tld_db();
    save_cs_db();
    save_server_db();
    save_ctcp_db();
    save_chan_db();
    save_exclude_db();
    save_admin_db();
    sql_ban_clean(NULL);
    send_event(EVENT_DB_SAVING, 1, EVENT_STOP);
}

/*************************************************************************/

/* Main routine.  (What does it look like? :-) ) */

int main(int ac, char **av)
{
    int i;
    volatile time_t last_update;
    volatile time_t last_htmlupdate;
    volatile time_t last_server_ping;
    volatile time_t last_server_uptime;
    volatile time_t last_sql_ping;
    volatile time_t last_backup_ping;
#ifdef USE_MYSQL
    int result;
#endif
#ifdef MSVS2005
    char errbuf[256];
#else
    char *errbuf;
#endif

    my_av = av;

#ifdef HAVE_DMALLOC
    dmalloc_debug_setup("log-stats,log-non-free,log=denora_dmalloc.%p");
    atexit(dmalloc_shutdown);
#endif

    SET_SEGV_LOCATION();

#ifndef _WIN32
    if (getuid() == ROOT_UID) {
        printf("Error: Cannot run Denora as root, exiting\n\r");
        exit(1);
    }
#endif

#ifndef _WIN32
    adns_init(&adns, adns_if_noenv, 0);
#endif
    gi = GeoIP_new(GEOIP_STANDARD);

    UplinkSynced = 0;
    denora->debug = 0;

    /* Initialization stuff. */
    if ((i = init(ac, av)) != 0) {
        destroy_all();
        return i;
    }
    signal_init();
    init_bans();
    if (ircd->spamfilter) {
        init_spamfilter();
    }

    /* We have a line left over from earlier, so process it first. */
    process();

    last_update = time(NULL);
    last_htmlupdate = time(NULL);
    last_server_ping = time(NULL);
    last_server_uptime = time(NULL);
    last_sql_ping = time(NULL);
    last_backup_ping = time(NULL);

    /* set these globals to NULL on startup, they are mainly for module
       coders */
    xml_encode = NULL;
    xml_doctype = NULL;
    xml_header = NULL;

    started = 1;

    /*** Main loop. ***/

    while (!denora->quitting) {
        time_t t = time(NULL);

        alog(LOG_DEBUGSOCK, "debug: Top of main loop");

        if (denora->delayed_quit)
            break;

        if ((denora->save_data || t - last_update >= UpdateTimeout)) {
            if (denora->delayed_quit) {
                denora_cmd_global(NULL, langstring(SHUTDOWN_SAVING_DB));
            }
            save_databases();

            if (denora->save_data < 0) {
                break;          /* out of main loop */
            }

            denora->save_data = 0;
            last_update = t;
        }

        if (PingFrequency >= 120) {
            if ((t - last_server_ping >= PingFrequency)) {
                if (UplinkSynced) {
                    ping_servers();
                }
                last_server_ping = t;
            }
        }

        if ((t - last_backup_ping >= BackupFreq)) {
            backup_databases();
            last_backup_ping = t;
        }


        if (SQLPingFreq >= 120) {
            if (denora->do_sql && (t - last_sql_ping >= SQLPingFreq)) {
#ifdef USE_MYSQL
                result = mysql_ping(mysql);
                if (result) {
                    if (denora->do_sql) {
                        denora->do_sql = 0;
                        if (result == CR_COMMANDS_OUT_OF_SYNC) {
                            alog(LOG_NORMAL,
                                 "Commands were executed in an improper order.");
                        }
                        if (result == CR_SERVER_GONE_ERROR) {
                            alog(LOG_NORMAL,
                                 "The MySQL server has gone away.");
                        }
                        if (result == CR_UNKNOWN_ERROR) {
                            alog(LOG_NORMAL, "An unknown error occurred");
                        }
                    }
                }
#endif
                last_sql_ping = t;
            } else if (SQLDisableDueServerLost && SQLRetryOnServerLost) {
                if (rdb_init()) {
                    SQLDisableDueServerLost = 0;
                }
            }
        }

        if (HTMLTimeout >= 120) {
            if ((t - last_htmlupdate >= HTMLTimeout)) {
                if (UplinkSynced) {
                    do_html();
                }
                last_htmlupdate = t;
            }
        }

        if (UptimeTimeout >= 120) {
            if ((t - last_server_uptime >= UptimeTimeout)) {
                if (UplinkSynced) {
                    uptime_servers();
                }
                last_server_uptime = t;
            }
        }
        if (XMLRPC_Enable) {
            extsock_process();
        }

        moduleCallBackRun();
        if (UplinkSynced) {
            denora_cron(t);
        }

        waiting = 1;
        i = (long int) sgets2(inbuf, sizeof(inbuf), servsock);
        waiting = 0;
        if ((i > 0) || (i < (-1))) {
            SET_START_TIME();
            process();
            CHECK_END_TIME();
        } else if (i == 0) {
            int errno_save = errno;
            denora->qmsg = calloc(BUFSIZE, 1);
            if (denora->qmsg) {
#ifdef MSVS2005
                strerror_s(errbuf, sizeof(errbuf), errno_save);
#else
                errbuf = strerror(errno_save);
#endif
                ircsnprintf(denora->qmsg, BUFSIZE,
                            "Read error from server: %s (error num: %d)",
                            errbuf, errno_save);
            } else {
                denora->qmsg = sstrdup("Read error from server");
            }
            denora->quitting = 1;
        }
        waiting = -4;
    }


    /* Check for restart instead of exit */
    if (denora->save_data == -2) {
#ifdef STATS_BIN
        denora_restart();
        return 1;
#else
        denora->qmsg =
            sstrdup
            ("Restart attempt failed--STATS_BIN not defined (rerun configure)");
#endif
    }

    /* Disconnect and exit */
    denora_shutdown();
    return 0;
}


/*************************************************************************/

void introduce_user(const char *user)
{
    ChannelStats *cs;
    lnode_t *tn;
    Uid *ud;

    char *modes;
    char nickbuf[BUFSIZE];

    /* Watch out for infinite loops... */
#define LTSIZE 20
    static int lasttimes[LTSIZE];
    if (lasttimes[0] >= time(NULL) - 3)
        fatal("introduce_user() loop detected");
    memmove(lasttimes, lasttimes + 1, sizeof(lasttimes) - sizeof(int));
    lasttimes[LTSIZE - 1] = time(NULL);
#undef LTSIZE

    *nickbuf = '\0';

    SET_SEGV_LOCATION();
    if (!user || stricmp(user, s_StatServ) == 0) {
        denora_cmd_nick(s_StatServ, desc_StatServ, ircd->statservmode);
    }

    if (s_StatServ_alias && !LargeNet) {
        SET_SEGV_LOCATION();
        if (!user || stricmp(user, s_StatServ_alias) == 0) {
            denora_cmd_nick(s_StatServ_alias, desc_StatServ_alias,
                            ircd->statservmode);
        }
    }
    SET_SEGV_LOCATION();
    if (!LargeNet) {
        if (!PartOnEmpty) {
            tn = list_first(CStatshead);
            ud = find_uid(s_StatServ);
            while (tn != NULL) {
                cs = lnode_get(tn);
                denora_cmd_join(s_StatServ, cs->name, time(NULL));
                if (AutoOp && AutoMode) {
                    modes = sstrdup(AutoMode);
                    while (*modes) {
                        switch (*modes) {
                        case '+':
                            break;
                        case '-':
                            break;
                        default:
                            ircsnprintf(nickbuf, BUFSIZE, "%s %s", nickbuf,
                                        ((ircd->p10
                                          && ud) ? ud->uid : s_StatServ));
                        }
                        (void) *modes++;
                    }
                    denora_cmd_mode(ServerName, cs->name, "%s%s", AutoMode,
                                    nickbuf);
                }
                tn = list_next(CStatshead, tn);
            }
        }
    }
}

/*************************************************************************/

/* Set GID if necessary.  Return 0 if successful (or if RUNGROUP not
 * defined), else print an error message to logfile and return -1.
 */

static int set_group(void)
{
#if defined(RUNGROUP) && defined(HAVE_SETGRENT)
    struct group *gr;

    SET_SEGV_LOCATION();

    setgrent();
    while ((gr = getgrent()) != NULL) {
        if (strcmp(gr->gr_name, RUNGROUP) == 0) {
            break;
        }
    }
    endgrent();
    if (gr) {
        setgid(gr->gr_gid);
        return 0;
    } else {
        alog(LOG_ERROR, "Error: Unable to find the group name `%s'",
             RUNGROUP);
        alog(LOG_ERROR,
             "Check your RUNGROUP setting and confirm that the group `%s' exists",
             RUNGROUP);
        return -1;
    }
#else
    SET_SEGV_LOCATION();
#if defined(RUNGROUP)
    alog(LOG_DEBUG,
         "debug: RUNGROUP listed but system does not support setgid()");
#endif
    return 0;
#endif
}

/*************************************************************************/

static int parse_dir_options(int ac, char **av)
{
    int i;
    char *s;

    SET_SEGV_LOCATION();

    for (i = 1; i < ac; i++) {
        s = av[i];
        if (*s == '-') {
            s++;
            if (strcmp(s, "dir") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-dir requires a parameter\n");
                    return -1;
                }
                denora->dir = av[i];
            } else if (strcmp(s, "log") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-log requires a parameter\n");
                    return -1;
                }
                denora->logname = av[i];
            }
        }
    }
    return 0;
}

/*************************************************************************/

/* Parse command-line options.  Return 0 if all went well, -1 for an error
 * with an option, or 1 for -help.
 */

static int parse_options(int ac, char **av)
{
    int i;
    char *s, *t;
    char *extra, *value;

    SET_SEGV_LOCATION();

    for (i = 1; i < ac; i++) {
        s = av[i];
        if (*s == '-') {
            s++;
            value = myStrGetToken(s, '=', 0);
            extra = myStrGetToken(s, '=', 1);

            if (strcmp(value, "remote") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-remote requires hostname[:port]\n");
                    return 0;
                }
                s = av[i];
                t = strchr(s, ':');
                if (t) {
                    *t++ = 0;
                    if (atoi(t) >= 1 || atoi(t) <= 65535)
                        RemotePort = atoi(t);
                    else {
                        fprintf(stderr,
                                "-remote: port number must be a positive integer.  Using default.\n");
                        return 0;
                    }
                }
                if (RemoteServer) {
                    free(RemoteServer);
                }
                RemoteServer = sstrdup(s);
            } else if (strcmp(value, "local") == 0) {
                if (++i >= ac) {
                    fprintf(stderr,
                            "-local requires hostname or [hostname]:[port]\n");
                    return 0;
                }
                s = av[i];
                t = strchr(s, ':');
                if (t) {
                    *t++ = 0;
                    if (atoi(t) >= 1 || atoi(t) <= 65535)
                        LocalPort = atoi(t);
                    else {
                        fprintf(stderr,
                                "-local: port number must be a positive integer or 0.  Using default.\n");
                        return 0;
                    }
                }
                if (LocalHost) {
                    free(LocalHost);
                }
                LocalHost = sstrdup(s);
            } else if (strcmp(value, "name") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-name requires a parameter\n");
                    return 0;
                }
                if (ServerName) {
                    free(ServerName);
                }
                ServerName = sstrdup(av[i]);
            } else if (strcmp(value, "desc") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-desc requires a parameter\n");
                    return 0;
                }
                if (ServerDesc) {
                    free(ServerDesc);
                }
                ServerDesc = sstrdup(av[i]);
            } else if (strcmp(value, "user") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-user requires a parameter\n");
                    return 0;
                }
                if (ServiceUser) {
                    free(ServiceUser);
                }
                ServiceUser = sstrdup(av[i]);
            } else if (strcmp(value, "host") == 0) {
                if (++i >= ac) {
                    fprintf(stderr, "-host requires a parameter\n");
                    return 0;
                }
                if (ServiceHost) {
                    free(ServiceHost);
                }
                ServiceHost = sstrdup(av[i]);
            } else if (strcmp(value, "dir") == 0) {
                /* Handled by parse_dir_options() */
                i++;            /* Skip parameter */
            } else if (strcmp(value, "log") == 0) {
                /* Handled by parse_dir_options(), too */
                i++;            /* Skip parameter */
            } else if (strcmp(value, "debug") == 0) {
                if (extra) {
                    denora->debug = atoi(extra);
                } else {
                    denora->debug++;
                }
            } else if (strcmp(value, "protocoldebug") == 0) {
                denora->protocoldebug++;
            } else if (strcmp(value, "speedtest") == 0) {
                SpeedTest++;
            } else if (strcmp(value, "sql") == 0) {
                denora->sqldebug = 1;
            } else if (strcmp(value, "socketdebug") == 0) {
                denora->socketdebug = 1;
            } else if (strcmp(value, "nofork") == 0) {
                denora->nofork = 1;
#ifdef HAVE_CRYPT
            } else if (strcmp(value, "mkpass") == 0) {
                if (extra) {
                    fprintf(stdout, "Encrypted Password: %s\n",
                            MakePassword(extra));
                } else {
                    fprintf(stdout,
                            "Please use syntax of -mkpass=password\n");
                }
                return 0;
#endif
            } else if (!strcmp(value, "version")) {
                fprintf(stdout,
                        "Denora-%s %s %s%s -- build #%s, compiled %s %s\n",
                        denora->version, denora->flags_os,
                        (denora->flag_mods ? denora->flag_mods : "")
                        , (denora->flag_sql ? denora->flag_sql : ""),
                        denora->build, denora->date, denora->time);
                return 0;
            } else if (!strcmp(value, "help")) {
                fprintf(stdout,
                        "Denora-%s %s %s%s -- build #%s, compiled %s %s\n",
                        denora->version, denora->flags_os,
                        (denora->flag_mods ? denora->flag_mods : "")
                        , (denora->flag_sql ? denora->flag_sql : ""),
                        denora->build, denora->date, denora->time);
                fprintf(stdout,
                        "Denora IRC Stats (http://denorastats.org/)\n");
                fprintf(stdout, "Usage ./stats [options] ...\n");
                fprintf(stdout,
                        "-remote        -remote hostname[:port]\n");
                fprintf(stdout, "-local         -local hostname[:port]\n");
                fprintf(stdout, "-name          -name servername\n");
                fprintf(stdout, "-desc          -desc serverdesc\n");
                fprintf(stdout, "-user          -user serviceuser\n");
                fprintf(stdout, "-host          -host servicehost\n");
                fprintf(stdout, "-debug         -debug\n");
                fprintf(stdout, "-nofork        -nofork\n");
                fprintf(stdout, "-sql           -sql\n");
                fprintf(stdout, "-socketdebug   -socketdebug\n");
                fprintf(stdout, "-version       -version\n");
                fprintf(stdout, "-help          -help\n");
                fprintf(stdout, "-log           -log logfilename\n");
                fprintf(stdout, "-dir           -dir statsdirectory\n");
#ifdef HAVE_CRYPT
                fprintf(stdout, "-mkpass        -mkpass=password\n");
#endif
                fprintf(stdout, "\n");
                fprintf(stdout,
                        "Further support is available from http://denorastats.org/\n");
                fprintf(stdout,
                        "Or visit US on IRC at irc.denorastats.org #denora\n");
                return 0;
            } else {
                fprintf(stderr, "Unknown option -%s\n", value);
                return 0;
            }
            if (extra) {
                free(extra);
            }
            if (value) {
                free(value);
            }
        } else {
            fprintf(stderr, "Non-option arguments not allowed\n");
            return 0;
        }
    }
    return 1;
}

/*************************************************************************/

/* Remove our PID file.  Done at exit. */

void remove_pidfile(void)
{
    if (PIDFilename) {
        remove(PIDFilename);
    }
}

/*************************************************************************/

/* Create our PID file and write the PID to it. */

static void write_pidfile(void)
{
    FILE *pidfile;

    SET_SEGV_LOCATION();

    if ((pidfile = FileOpen(PIDFilename, FILE_WRITE)) != NULL) {
        fprintf(pidfile, "%d\n", (int) getpid());
        fclose(pidfile);
    } else {
        log_perror("Warning: cannot write to PID file %s", PIDFilename);
    }
}

/*************************************************************************/

/* Overall initialization routine.  Returns 0 on success, -1 on failure. */

int init(int ac, char **av)
{
    int i;
    int openlog_failed = 0, openlog_errno = 0;
    int started_from_term = isatty(0) && isatty(1) && isatty(2);
#ifdef MSVS2005
    char errbuf[256];
#else
    char *errbuf;
#endif

    /* Set file creation mask and group ID. */
#if defined(DEFUMASK) && HAVE_UMASK
    umask(DEFUMASK);
#endif
    if (set_group() < 0) {
        return -1;
    }

    SET_SEGV_LOCATION();

    /* Parse command line for -dir option. */
    parse_dir_options(ac, av);

    /* Chdir to Denora data directory. */
    if (chdir(denora->dir) < 0) {
#ifdef MSVS2005
        strerror_s(errbuf, sizeof(errbuf), errno);
#else
        errbuf = strerror(errno);
#endif
        fprintf(stderr, "chdir(%s): %s\n", denora->dir, errbuf);
        return -1;
    }

    /* Open logfile, and complain if we didn't. */
    if (open_log() < 0) {
        openlog_errno = errno;
        if (started_from_term) {
#ifdef MSVS2005
            strerror_s(errbuf, sizeof(errbuf), errno);
#else
            errbuf = strerror(errno);
#endif
            fprintf(stderr, "Warning: unable to open log file %s: %s\n",
                    denora->logname, errbuf);
        } else {
            openlog_failed = 1;
        }
    }

    /* Initialize multi-language support */
    lang_init();
    alog(LOG_DEBUG, "debug: Loaded languages");

    /* call it a hack, need to take a static and turn it global 
       anything else will piss off the compiler.
     */
    initconfsettigs();

    /* Read configuration file; exit if there are problems. */
    if (initconf(denora->config, 0, mainconf) == -1) {
        printf("Server not started\n");
        exit(-1);
    }

    /* Let's merge all the config */
    merge_confs();

    /* Parse all remaining command-line options. */
    if (!parse_options(ac, av)) {
        fprintf(stderr, "Denora not started\n");
        exit(1);
    }
#ifndef _WIN32
    /* Detach ourselves if requested. */
    if (!denora->nofork) {
        if ((i = fork()) < 0) {
            perror("fork()");
            return -1;
        } else if (i != 0) {
            exit(0);
        }
        if (started_from_term) {
            close(0);
            close(1);
            close(2);
        }
        if (setpgid(0, 0) < 0) {
            perror("setpgid()");
            return -1;
        }
    }
#else
    /* Initialize winsocks -- codemastr */
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
            alog(LOG_ERROR, "Failed to initialized WinSock library");
            return -1;
        }
    }
    if (!SupportedWindowsVersion()) {
        alog(LOG_ERROR, "Not supported version of Windows");
        return -1;
    } else {
        denora->flags_os = GetWindowsVersion();
        alog(LOG_NORMAL, "Running: %s", denora->flags_os);
    }

    if (!denora->nofork) {
        alog(LOG_NORMAL, "Launching Denora into the background");
        FreeConsole();
    }
#endif

    /* Since you can rem out whole blocks and segfault 
       denora we run a bad pointer check on variables */
    post_config_check();

    /* setup user modes array now all at 0, during protocol init they
       will be setting these to 1 as needed.
     */
    init_umodes();
    init_cmodes();
    init_csmodes();

    /* Add IRCD Protocol Module; exit if there are errors */
    if (protocol_module_init() != MOD_ERR_OK) {
        return -1;
    }

    if (!denora->version_protocol) {
        denora->version_protocol = sstrdup("Bad No Protocol");
        return -1;
    }

    if (ircd->extrawarning) {
        alog(LOG_NORMAL,
             "======================== WARNING =========================");
        alog(LOG_NORMAL,
             "You have choosen to run a ircd protocl module that is very");
        alog(LOG_NORMAL,
             "developemental, we do not have enough users out there to mark");
        alog(LOG_NORMAL,
             "it as stable, if you are willing to test and allow us");
        alog(LOG_NORMAL,
             "mark it as stable please email dev@denorastats.org");
        alog(LOG_NORMAL,
             "======================== WARNING =========================");
    }
#ifdef HAVE_LIBZ
    if (ircd->zip && UseZIP) {
        if (zip_init(ZIP_DEFAULT_LEVEL)) {
            alog(LOG_DEBUG, "debug: Zip Compression enabled");
        } else {
            alog(LOG_DEBUG, "debug: Error during Zip Compression setup");
            UseZIP = 0;
        }
    }
#endif
    if (UseThreading) {
        QueueEntryInit();
    }
    statserv_int();

    /* Add Core MSG handles */
    moduleAddMsgs();

    CreateUserModeCallBack((char *) "o", MODE_NOPARAM, handle_ircop);

    load_stats_db();
    init_tld();
    load_tld_db();
    InitCStatsList();
    load_cs_db();
    load_server_db();
    init_ctcp();
    load_ctcp_db();
    InitStatsChanList();
    load_chan_db();
    load_exclude_db();
    load_admin_db();

    /* Write our PID to the PID file. */
    write_pidfile();

    /* Announce ourselves to the logfile. */
    alog(LOG_NORMAL,
         "Denora %s (IRCd protocol: %s) starting up (options:%s)",
         denora->version,
         (denora->version_protocol ? denora->
          version_protocol : "None Set"),
         denora->debug ? " debug on" : " debug off");
    denora->start_time = time(NULL);

    setup_cron_event();

    if (!rdb_init()) {
        alog(LOG_ERROR, "Error: Disabling SQL due to errors with SQL");
    }

    if (XMLRPC_Enable) {
        extsock_init(XMLRPC_Host, XMLRPC_Port);
    }

    core_modules_init();

    /* load any custom modules */
    modules_init();

    /* Connect to the remote server */
    servsock = conn(RemoteServer, RemotePort, LocalHost, LocalPort);
    if (servsock < 0) {
        fatal_perror("Can't connect to server");
    }
    alog(LOG_NORMAL, "Connected to Server (%s:%d)", RemoteServer,
         RemotePort);

    send_event(EVENT_CONNECT, 1, EVENT_START);
    denora_cmd_connect();

    sgets2(inbuf, sizeof(inbuf), servsock);
    if (strnicmp(inbuf, "ERROR", 5) == 0) {
        /* Close server socket first to stop wallops, since the other
         * server doesn't want to listen to us anyway */
        disconn(servsock);
        servsock = -1;
        fatal("Remote server returned: %s", inbuf);
    }

    /* Announce a logfile error if there was one */
    if (openlog_failed) {
#ifdef MSVS2005
        strerror_s(errbuf, sizeof(errbuf), openlog_errno);
#else
        errbuf = strerror(openlog_errno);
#endif
        denora_cmd_global(NULL, langstring(CANNOT_OPEN_LOG), errbuf);
    }

    /* Bring in our pseudo-clients */
    introduce_user(NULL);

    /* And hybrid needs Global joined in the logchan */
    if (!BadPtr(LogChannel)) {
        denora_cmd_join(s_StatServ, LogChannel, time(NULL));
    }

    denora_cmd_eob();
    send_event(EVENT_CONNECT, 1, EVENT_STOP);

    /* Dumping stats.db maxvalues to sql */
    if (denora->do_sql) {
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='channels'",
             MaxValueTable, stats->chans_max,
             (long int) stats->chans_max_time);
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='users'",
             MaxValueTable, stats->users_max,
             (long int) stats->users_max_time);
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='servers'",
             MaxValueTable, stats->servers_max,
             (long int) stats->servers_max_time);
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='opers'",
             MaxValueTable, stats->opers_max,
             (long int) stats->opers_max_time);
    }

    /* Dumping all admins (from config file and admin.db) to sql */
    reset_sqladmin();

    /**
      * Load our delayed modules - modules that are planing on making clients need to wait till now
      * where as modules wanting to modify our ircd connection messages need to load earlier :|
      **/
    modules_delayed_init();

    /* Success! */
    return 0;
}

/*************************************************************************/
