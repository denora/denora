/* MYSQL Optimize at midnight
 *
 * (c) 2004-2013 Denora Team
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

int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
	char tables[512] = "\0";

	USE_VAR(argc);
	USE_VAR(argv);

	moduleAddAuthor("Denora");
	moduleAddVersion("1.1");
	moduleSetType(THIRD);

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

	alog(LOG_NORMAL, "Installing MYSQL Optimizing Event, this will run daily on your MySQL Server.");

	rdb_query(QUERY_LOW, "DROP EVENT IF EXISTS `DENORA_EVENT_OPTIMIZE`");

	rdb_query(QUERY_LOW, "CREATE EVENT `DENORA_EVENT_OPTIMIZE` "
				"ON SCHEDULE EVERY 1 DAY STARTS CURRENT_DATE "
				"DO OPTIMIZE TABLE %s", tables);

	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{
	rdb_query(QUERY_LOW,"DROP EVENT IF EXISTS `DENORA_EVENT_OPTIMIZE`");
}

