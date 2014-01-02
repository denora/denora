/* MYSQL Optimize at midnight
 *
 * (c) 2004-2014 Denora Team
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

int db_optimize(const char *name);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
	CronEvent *evt;

	USE_VAR(argc);
	USE_VAR(argv);

	moduleAddAuthor("Denora");
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
	SQLres *sql_res;

	USE_VAR(name);

	alog(LOG_NORMAL, "Optimizing MYSQL tables");

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

	sql_query("OPTIMIZE TABLE %s", tables);

	/*
	 * We have to catch the result of the OPTIMIZE TABLE query
	 * or we will get an "COMMANDS OUT OF SYNC" error on all
	 * following queries - DP, 2014
	 *
	 * dev.mysql.com says:
	 *	If you get "Commands out of sync; you can't run this command now" in
	 *	your client code, you are calling client functions in the wrong order.
	 *	This can happen, for example, if you are using sql_use_result()
	 *	and try to execute a new query before you have called sql_free_result().
	 *	It can also happen if you try to execute two queries that return data
	 *	without calling sql_use_result() or sql_set_result() in between.
	 *
	 * this "fix" makes denora wait until the OPTIMIZE TABLE
	 * query is fully processed, on large tables and slow sql servers this could
	 * take a while. a better solution is to use mysql EVENTS.
	 */
	sql_res = sql_set_result(sqlcon);
	sql_free_result(sql_res);
	return MOD_CONT;
}
