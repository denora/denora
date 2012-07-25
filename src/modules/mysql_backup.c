/* MYSQL Backup at midnight
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

int do_sql_backup(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
void do_mysql_backup(char *table, char *output);

int DenoraInit(int argc, char **argv)
{
	EvtHook *hook;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	int found = 0;
#endif

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("1.1");
	moduleSetType(THIRD);

#ifdef USE_MYSQL
	/* Check if we have the FILE privilege if not, bail out */
        rdb_query(QUERY_HIGH, "SHOW GRANTS;");
	mysql_res = mysql_store_result(mysql);
	if (mysql_res && mysql_num_rows(mysql_res))
	{
		while ((mysql_row = mysql_fetch_row(mysql_res)))
		{
			if (strstr(rdb_escape(mysql_row[0]),"GRANT FILE ON "))
			{
				found = 1;
				break;
			}
		}
		if (found == 0)
		{
			alog(LOG_NORMAL, "You do not have FILE privileges enabled. Disabling module");
			mysql_free_result(mysql_res);
			return MOD_STOP;
		}
		mysql_free_result(mysql_res);
	}
#endif

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

#ifndef _WIN32
int do_sql_backup(__attribute__((unused))int argc, char **argv)
#else
int do_sql_backup(int argc, char **argv)
#endif
{
	int i;
	char output[BUFSIZE];
	char *table[18];

	if (!denora->do_sql)
	{
		alog(LOG_ERROR, "SQL is disabled, backup stopped");
		return MOD_CONT;
	}

#ifdef USE_MYSQL
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
			do_mysql_backup(table[i], output);

		if (ircd->except)
			do_mysql_backup(ChanExceptTable, output);

		if (ircd->invitemode)
			do_mysql_backup(ChanInviteTable, output);

		if (ircd->sgline_table)
			do_mysql_backup(SglineTable, output);

		if (ircd->sqline_table)
			do_mysql_backup(SqlineTable, output);

		if (ircd->spamfilter)
			do_mysql_backup(SpamTable, output);
	}
#endif
	return MOD_CONT;
}

#ifdef USE_MYSQL
void do_mysql_backup(char *table, char *output)
{
	if (!denora->do_sql)
	{
		alog(LOG_ERROR, "SQL is disabled in the meantime, backup stopped");
		return;
	}
	rdb_query(QUERY_LOW, "BACKUP TABLE %s TO '%s'", table, output);
}
#endif
