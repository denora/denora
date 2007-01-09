/* MYSQL Backup at midnight
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
/*************************************************************************/

#include "denora.h"

int do_sql_backup(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
    EvtHook *hook;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Trystan");
    moduleAddVersion("1.0");
    moduleSetType(THIRD);

    hook = createEventHook(EVENT_DB_BACKUP, do_sql_backup);
    moduleAddEventHook(hook);

    if (!denora->do_sql) {
        return MOD_STOP;
    }

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int do_sql_backup(int argc, char **argv)
{
    char output[BUFSIZE];

    if (!stricmp(argv[0], EVENT_STOP)) {
        ircsnprintf(output, BUFSIZE, "%s/backups", STATS_DIR);
        alog(LOG_NORMAL, "Backing up MYSQL tables to '%s'", output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", UserTable, output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanBansTable,
                  output);

        if (ircd->except) {
#ifdef USE_MYSQL
            dbMySQLPrepareForQuery();
#endif
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'",
                      ChanExceptTable, output);
        }
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", IsOnTable, output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ServerTable,
                  output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", GlineTable,
                  output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanTable, output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", MaxValueTable,
                  output);

        if (ircd->invitemode) {
#ifdef USE_MYSQL
            dbMySQLPrepareForQuery();
#endif
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'",
                      ChanInviteTable, output);
        }
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", TLDTable, output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CTCPTable, output);

        if (ircd->sgline_table) {
#ifdef USE_MYSQL
            dbMySQLPrepareForQuery();
#endif
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SglineTable,
                      output);
        }
        if (ircd->sqline_table) {
#ifdef USE_MYSQL
            dbMySQLPrepareForQuery();
#endif
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SqlineTable,
                      output);
        }
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanStatsTable,
                  output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ServerStatsTable,
                  output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", AliasesTable,
                  output);
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CStatsTable,
                  output);
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", UStatsTable,
                  output);
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CurrentTable,
                  output);
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", StatsTable,
                  output);
        if (ircd->spamfilter) {
#ifdef USE_MYSQL
            dbMySQLPrepareForQuery();
#endif
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SpamTable,
                      output);
        }
#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
#endif
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", AdminTable,
                  output);
    }
    return MOD_CONT;
}
