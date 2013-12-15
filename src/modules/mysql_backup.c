/* MYSQL Backup at midnight
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

int do_sql_backup(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
void do_sql_table_backup(char *table, char *output);

int DenoraInit(int argc, char **argv)
{
	EvtHook *hook;
	SQLres *sql_res;
	int found = 0;
	char **sql_row;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("1.1");
	moduleSetType(THIRD);

	/* Check if we have the FILE privilege if not, bail out */
        sql_query("SHOW GRANTS;");
	sql_res = sql_set_result(sqlcon);
	if (sql_res && sql_num_rows(sql_res))
	{
		while ((sql_row = sql_fetch_row(sql_res)))
		{
			if (strstr(sql_escape(sql_row[0]),"GRANT FILE ON "))
			{
				found = 1;
				break;
			}
		}
		if (found == 0)
		{
			alog(LOG_NORMAL, "You do not have FILE privileges enabled. Disabling module");
			sql_free_result(sql_res);
			return MOD_STOP;
		}
		sql_free_result(sql_res);
	}

	hook = createEventHook(EVENT_DB_BACKUP, do_sql_backup);
	moduleAddEventHook(hook);

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
	int i;
	char output[BUFSIZE];
	char *table[18];

	if (!denora->do_sql)
	{
		alog(LOG_ERROR, "SQL is disabled, backup stopped");
		return MOD_CONT;
	}

	table[0] = UserTable;
	table[1] = ChanBansTable;
	table[2] = IsOnTable;
	table[3] = ServerTable;
	table[4] = GlineTable;
	table[5] = ChanTable;
	table[6] = MaxValueTable;
	table[7] = TLDTable;
	table[8] = CTCPTable;
	table[9] = ChanStatsTable;
	table[10] = ServerStatsTable;
	table[11] = AliasesTable;
	table[12] = CStatsTable;
	table[13] = UStatsTable;
	table[14] = CurrentTable;
	table[15] = StatsTable;
	table[16] = SpamTable;
	table[17] = AdminTable;

	if (!stricmp(argv[0], EVENT_STOP))
	{
		ircsnprintf(output, BUFSIZE, "%s/backups", STATS_DIR);
		alog(LOG_NORMAL, "Backing up MYSQL tables to '%s'", output);

		for (i=0; i<18; i++)
			do_sql_table_backup(table[i], output);

		if (ircd->except)
			do_sql_table_backup(ChanExceptTable, output);

		if (ircd->invitemode)
			do_sql_table_backup(ChanInviteTable, output);

		if (ircd->sgline_table)
			do_sql_table_backup(SglineTable, output);

		if (ircd->sqline_table)
			do_sql_table_backup(SqlineTable, output);

		if (ircd->spamfilter)
			do_sql_table_backup(SpamTable, output);
	}
	return MOD_CONT;
}

void do_sql_table_backup(char *table, char *output)
{
	SQLres *sql_res;
	if (!denora->do_sql)
	{
		alog(LOG_ERROR, "SQL is disabled in the meantime, backup stopped");
		return;
	}
	sql_query("BACKUP TABLE %s TO '%s'", table, output);
	/* catch the result to prevent the "command out of sync" error - DP */
	sql_res = sql_set_result(sqlcon);
	sql_free_result(sql_res);
}
