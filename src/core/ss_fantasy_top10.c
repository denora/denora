/*
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: ss_fantasy_top10.c 610 2006-06-06 18:32:06Z trystan $
 *
 */

/*************************************************************************/

#include "denora.h"

int do_fantasy(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the hook, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    EvtHook *hook;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id: ss_fantasy_top10.c 610 2006-06-06 18:32:06Z trystan $");
    moduleSetType(CORE);

    hook = createEventHook(EVENT_FANTASY, do_fantasy);
    moduleAddEventHook(hook);

#ifdef USE_MYSQL
    return MOD_CONT;
#else
    return MOD_STOP;
#endif
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/**
 * Handle seen fantasy command.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
    User *u;
    char *chan;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
    int i = 1;
#endif
    ChannelStats *cs;

    if (argc < 3) {
        return MOD_CONT;
    }

    if (!denora->do_sql) {
        return MOD_CONT;
    }

    if (stricmp(argv[0], "top10") == 0) {
        u = finduser(argv[1]);
        chan = rdb_escape(argv[2]);
        cs = find_cs(argv[2]);
        rdb_query
            (QUERY_HIGH,
             "SELECT * FROM %s WHERE chan=\'%s\' AND type=0 ORDER BY letters DESC LIMIT 10;",
             UStatsTable, chan);
        free(chan);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_num_rows(mysql_res) > 0) {
            if (cs->flags & CS_NOTICE) {
                notice_lang(s_StatServ, u, STATS_TOP10_CHANNEL, argv[2]);
            } else {
                denora_cmd_privmsg(s_StatServ, argv[2],
                                   getstring(NULL,
                                             STATS_TOP10_CHANNEL),
                                   argv[2]);
            }
            SET_SEGV_LOCATION();
            while ((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
                if (cs->flags & CS_NOTICE) {
                    notice_lang(s_StatServ, u, STATS_MESSAGE,
                                i, mysql_row[0], mysql_row[3],
                                mysql_row[4], mysql_row[5],
                                mysql_row[7], mysql_row[6]);
                } else {
                    denora_cmd_privmsg(s_StatServ, argv[2],
                                       getstring(NULL,
                                                 STATS_MESSAGE), i,
                                       mysql_row[0], mysql_row[3],
                                       mysql_row[4], mysql_row[5],
                                       mysql_row[7], mysql_row[6]);
                }
                i++;
            }
        }
        SET_SEGV_LOCATION();
        mysql_free_result(mysql_res);
#endif
    }

    return MOD_CONT;
}
