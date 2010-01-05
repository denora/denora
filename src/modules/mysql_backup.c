/* MYSQL Backup at midnight
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
    moduleAddAuthor("Denora");
    moduleAddVersion("1.1");
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

    if (!denora->do_sql) {
        alog(LOG_ERROR, "SQL is disabled, backup stopped");
        return MOD_CONT;
    }

    if (!stricmp(argv[0], EVENT_STOP)) {
        ircsnprintf(output, BUFSIZE, "%s/backups", STATS_DIR);
        alog(LOG_NORMAL, "Backing up MYSQL tables to '%s'", output);

#ifdef USE_MYSQL
        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", UserTable, output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanBansTable,
                  output);

        if (ircd->except) {
            dbMySQLPrepareForQuery();
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'",
                      ChanExceptTable, output);
        }

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", IsOnTable, output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ServerTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", GlineTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanTable, output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", MaxValueTable,
                  output);

        if (ircd->invitemode) {
            dbMySQLPrepareForQuery();
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'",
                      ChanInviteTable, output);
        }

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", TLDTable, output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CTCPTable, output);

        if (ircd->sgline_table) {
            dbMySQLPrepareForQuery();
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SglineTable,
                      output);
        }
        if (ircd->sqline_table) {
            dbMySQLPrepareForQuery();
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SqlineTable,
                      output);
        }

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ChanStatsTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", ServerStatsTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", AliasesTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CStatsTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", UStatsTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", CurrentTable,
                  output);

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", StatsTable,
                  output);

        if (ircd->spamfilter) {
            dbMySQLPrepareForQuery();
            rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", SpamTable,
                      output);
        }

        dbMySQLPrepareForQuery();
        rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", AdminTable,
                  output);
#endif
    }
    return MOD_CONT;
}
