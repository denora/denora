/* MYSQL Optimize at midnight
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

int db_optimize(const char *name);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
    CronEvent *evt;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Trystan");
    moduleAddVersion("1.1");
    moduleSetType(THIRD);

    evt = createCronEvent(CRON_MIDNIGHT, db_optimize);
    addCronEvent(CRONEVENT, evt);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int db_optimize(const char *name)
{
    char tables[512] = "\0";

    alog(LOG_NORMAL, "Optimzing MYSQL tables");

    sprintf(tables,
            "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s",
            UserTable, ChanBansTable, IsOnTable, ServerTable, GlineTable,
            ChanTable, MaxValueTable, TLDTable, CTCPTable, ChanStatsTable,
            ServerStatsTable, AliasesTable, CStatsTable, UStatsTable,
            CurrentTable, StatsTable, AdminTable);
    if (ircd->except)
        sprintf(tables, "%s, %s", tables, ChanExceptTable);
    if (ircd->invitemode)
        sprintf(tables, "%s, %s", tables, ChanInviteTable);
    if (ircd->sgline_table)
        sprintf(tables, "%s, %s", tables, SglineTable);
    if (ircd->sqline_table)
        sprintf(tables, "%s, %s", tables, SqlineTable);
    if (ircd->spamfilter)
        sprintf(tables, "%s, %s", tables, SpamTable);

#ifdef USE_MYSQL
    dbMySQLPrepareForQuery();
#endif
    rdb_query(QUERY_LOW, "OPTIMIZE TABLE %s", tables);
#ifdef USE_MYSQL
    dbMySQLPrepareForQuery();
#endif

    return MOD_CONT;
}
