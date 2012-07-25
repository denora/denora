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

#ifndef _WIN32
int DenoraInit(__attribute__((unused))int argc, __attribute__((unused))char **argv)
#else
int DenoraInit(int argc, char **argv)
#endif
{
	CronEvent *evt;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
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

int db_optimize(__attribute__((unused))const char *name)
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

	rdb_query(QUERY_LOW, "OPTIMIZE TABLE %s", tables);
	return MOD_CONT;
}
