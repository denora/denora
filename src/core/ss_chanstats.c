/* StatServ core functions
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
 */
/*************************************************************************/

#include "denora.h"

static int do_chanstats(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    Command *c;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id$");
    moduleSetType(CORE);

    c = createCommand("CHANSTATS", do_chanstats, is_stats_admin, -1, -1,
                      -1, STAT_HELP_CHANSTATS);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

static int do_chanstats(User * u, int ac, char **av)
{
    char *cmd1 = NULL;
    char *cmd2 = NULL;
    char *cmd3 = NULL;
    char *cmd4 = NULL;
    lnode_t *tn;

    ChannelStats *cs;
    Channel *c;
    int i;
    int x = 0;
    char *sqlchan;
    Uid *ud;

    char *modes;
    char nickbuf[BUFSIZE];
    *nickbuf = '\0';

    if (ac >= 1) {
        cmd1 = av[0];
    }
    if (ac >= 2) {
        cmd2 = av[1];
    }
    if (ac >= 3) {
        cmd3 = av[2];
    }
    if (ac >= 4) {
        cmd4 = av[3];
    }

    if (cmd1) {
        /* Since ChanStats does require SQL code... */
        if (!denora->do_sql) {
            notice_lang(s_StatServ, u, SQL_DISABLED);
            return MOD_CONT;
        }

        if (!stricmp("ADD", cmd1)) {
            if (LargeNet) {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_LARGENET);
                return MOD_CONT;
            }
            if (!cmd2) {
                syntax_error(s_StatServ, u, "CHANSTATS",
                             STAT_CHANSTATS_ADD_SYNTAX);
                return MOD_CONT;
            }
            if (BadChanName(cmd2)) {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_NOT_VALID_CHAN);
                return MOD_CONT;
            }
            if (strlen(cmd2) > CHANMAX) {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_NOT_VALID_CHAN);
                return MOD_CONT;
            }
            if (LogChannel) {
                if (!stricmp(cmd2, LogChannel)) {
                    notice_lang(s_StatServ, u, STAT_CHANSTATS_LOGCHAN);
                    return MOD_CONT;
                }
            }
            if (!(cs = find_cs(cmd2))) {
                cs = makecs(cmd2);
                save_cs_db();
                sqlchan = rdb_escape(cmd2);
                if (CSDefFlag) {
                    if (CSDefFlag == 1) {
                        cs->flags |= CS_FANTASY;
                        cs->flags &= ~CS_NOTICE;
                    } else if (CSDefFlag == 2) {
                        cs->flags |= CS_FANTASY;
                        cs->flags |= CS_NOTICE;
                    }
                }

                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          CStatsTable, sqlchan);
                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          UStatsTable, sqlchan);
                cs->timeadded = time(NULL);
                for (i = 0; i < 4; i++) {
                    rdb_query
                        (QUERY_LOW,
                         "INSERT INTO %s SET chan=\'%s\', type=%i, timeadded=%ld;",
                         CStatsTable, sqlchan, i, cs->timeadded);
                }
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_ADDED,
                            cmd2);

                ud = find_uid(s_StatServ);
                if (PartOnEmpty) {
                    c = findchan(cmd2);
                    if (c) {
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
                                    ircsnprintf(nickbuf, BUFSIZE, "%s %s",
                                                nickbuf,
                                                ((ircd->p10
                                                  && ud) ? ud->uid :
                                                 s_StatServ));
                                }
                                (void) *modes++;
                            }
                            denora_cmd_mode(ServerName, cs->name, "%s%s",
                                            AutoMode, nickbuf);
                        }
                    }
                } else {
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
                                ircsnprintf(nickbuf, BUFSIZE, "%s %s",
                                            nickbuf,
                                            ((ircd->p10
                                              && ud) ? ud->uid :
                                             s_StatServ));
                            }
                            (void) *modes++;
                        }
                        denora_cmd_mode(ServerName, cs->name, "%s%s",
                                        AutoMode, nickbuf);
                    }
                }
                free(sqlchan);
            } else {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_ALREADY,
                            cmd2);
            }
        } else if (!stricmp("DEL", cmd1)) {
            if (!cmd2) {
                syntax_error(s_StatServ, u, "CHANSTATS",
                             STAT_CHANSTATS_DEL_SYNTAX);
                return MOD_CONT;
            }
            sqlchan = rdb_escape(cmd2);
            if ((cs = find_cs(cmd2))) {
                del_cs(cs);
                save_cs_db();
                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          CStatsTable, sqlchan);
                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          UStatsTable, sqlchan);
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_DEL, cmd2);
                if (LogChannel) {
                    if (stricmp(LogChannel, cmd2)) {
                        denora_cmd_part(s_StatServ, cmd2,
                                        getstring(NULL,
                                                  STATS_CHANSTATS_PART),
                                        cmd2);
                    }
                } else {
                    denora_cmd_part(s_StatServ, cmd2,
                                    getstring(NULL, STATS_CHANSTATS_PART),
                                    cmd2);
                }
            } else {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_NOTFOUND,
                            cmd2);
            }
            free(sqlchan);
        } else if (!stricmp("LIST", cmd1)) {
            tn = list_first(CStatshead);
            while (tn != NULL) {
                cs = lnode_get(tn);
                notice(s_StatServ, u->nick, "%d %s", x, cs->name);
                x++;
                tn = list_next(CStatshead, tn);
            }
            if (!x) {
                notice_lang(s_StatServ, u, STATS_CHANSTATS_NOCHANNELS);
            }
        } else if (!stricmp("SUMUSER", cmd1)) {
            if (!cmd3) {
                syntax_error(s_StatServ, u, "CHANSTATS",
                             STATS_CHANSTATS_SUMUSER_SYNTAX);
                return MOD_CONT;
            }
            sumuser(u, cmd2, cmd3);
        } else if (!stricmp("RENAME", cmd1)) {
            if (!cmd3) {
                syntax_error(s_StatServ, u, "CHANSTATS",
                             STATS_CHANSTATS_RENAMEUSER_SYNTAX);
                return MOD_CONT;
            }
            renameuser(u, cmd2, cmd3);
        } else if (!stricmp("SET", cmd1)) {
            if (!cmd3) {
                syntax_error(s_StatServ, u, "CHANSTATS",
                             STATS_CHANSTATS_SET_SYNTAX);
                return MOD_CONT;
            }
            if (!(cs = find_cs(cmd2))) {
                notice_lang(s_StatServ, u, STAT_CHANSTATS_CHAN_NOTFOUND,
                            cmd2);
                return MOD_CONT;
            }
            if (!stricmp("FANTASY", cmd3)) {
                if (!cmd4) {
                    syntax_error(s_StatServ, u, "CHANSTATS",
                                 STATS_CHANSTATS_SET_SYNTAX);
                    return MOD_CONT;
                }
                if (!stricmp("ON", cmd4)) {
                    cs->flags |= CS_FANTASY;
                    cs->flags &= ~CS_NOTICE;
                    notice_lang(s_StatServ, u, STATS_CHANSTATS_FANTASY_ON,
                                cmd2);
                } else if (!stricmp("OFF", cmd4)) {
                    cs->flags &= ~CS_FANTASY;
                    cs->flags &= ~CS_NOTICE;
                    notice_lang(s_StatServ, u, STATS_CHANSTATS_FANTASY_OFF,
                                cmd2);
                } else if (!stricmp("NOTICE", cmd4)) {
                    cs->flags |= CS_FANTASY;
                    cs->flags |= CS_NOTICE;
                    notice_lang(s_StatServ, u,
                                STATS_CHANSTATS_FANTASY_NOTICE, cmd2);
                } else {
                    syntax_error(s_StatServ, u, "CHANSTATS",
                                 STATS_CHANSTATS_SET_SYNTAX);
                }
            }                   /* fantasy */
        } else {
            syntax_error(s_StatServ, u, "CHANSTATS",
                         STAT_CHANSTATS_SYNTAX);
        }
    } else {
        syntax_error(s_StatServ, u, "CHANSTATS", STAT_CHANSTATS_SYNTAX);
    }
    return MOD_CONT;
}
