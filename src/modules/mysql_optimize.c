/* MYSQL Optimize at midnight
 *
 * (c) 2004-2012 Denora Team
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
	MYSQL_RES *mysql_res;

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

	rdb_query(QUERY_HIGH, "OPTIMIZE TABLE %s", tables);

	/*
	 * We have to catch the result of the OPTIMIZE TABLE query
	 * or we will get an "COMMANDS OUT OF SYNC" error on all
	 * following queries - DP, 2013
	 *
	 * dev.mysql.com says:
	 *	If you get "Commands out of sync; you can't run this command now" in
	 *	your client code, you are calling client functions in the wrong order.
	 *	This can happen, for example, if you are using mysql_use_result()
	 *	and try to execute a new query before you have called mysql_free_result().
	 *	It can also happen if you try to execute two queries that return data
	 *	without calling mysql_use_result() or mysql_store_result() in between.
	 *
	 * this "fix" makes denora wait until the OPTIMIZE TABLE
	 * query is fully processed, on large tables and slow sql servers this could
	 * take a while. a better solution is to use mysql EVENTS.
	 */
	mysql_res = mysql_store_result(mysql);
	mysql_free_result(mysql_res);

	return MOD_CONT;
}
