/* Cron Chanstats
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

#define MODULE_VERSION "2.0"
#define MODULE_NAME "cron_chanstats"

int chanstats_month(const char *name);
int chanstats_weekly(const char *name);
int chanstats_daily(const char *name);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
	CronEvent *evt;
	int status;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	evt = createCronEvent(CRON_MIDNIGHT, chanstats_daily);
	status = moduleAddCronEvent(evt);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting cron->chanstats_daily [%d][%s]", status,
		     ModuleGetErrStr(status));
		return MOD_STOP;
	}
	evt = createCronEvent(CRON_WEEKLY_MONDAY, chanstats_weekly);
	status = moduleAddCronEvent(evt);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting cron->chanstats_weekly [%d][%s]", status,
		     ModuleGetErrStr(status));
		return MOD_STOP;
	}

	evt = createCronEvent(CRON_MONTHLY, chanstats_month);
	status = moduleAddCronEvent(evt);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting cron->chanstats_month [%d][%s]", status,
		     ModuleGetErrStr(status));
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

/*************************************************************************/

int chanstats_daily(const char *name)
{
	if (!name)
	{
		return MOD_CONT;
	}
	
	alog(LOG_NORMAL, langstr(ALOG_RESET_DAILY));
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	 "kicks=0, modes=0, topics=0, wasted=0, "
	 "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	 "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	 "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	 "time22=0, time23=0  WHERE type=1;", UStatsTable);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	          "kicks=0, modes=0, topics=0, "
	          "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	          "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	          "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	          "time22=0, time23=0  WHERE type=1;", CStatsTable);
	return MOD_CONT;
}

/*************************************************************************/

int chanstats_weekly(const char *name)
{
	if (!name)
	{
		return MOD_CONT;
	}
	
	alog(LOG_NORMAL, langstr(ALOG_RESETTING_WEEKLY));
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	 "kicks=0, modes=0, topics=0, wasted=0, "
	 "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	 "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	 "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	 "time22=0, time23=0  WHERE type=2;", UStatsTable);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	          "kicks=0, modes=0, topics=0, "
	          "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	          "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	          "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	          "time22=0, time23=0  WHERE type=2;", CStatsTable);
	DenoraSQLQuery(DenoraDB, "DELETE %s.*,%s.* FROM %s,%s WHERE %s.uname = %s.uname AND %s.lastspoke < %i AND %s.ignore = 'N';",
	          UStatsTable, AliasesTable, UStatsTable, AliasesTable,
	          UStatsTable, AliasesTable, UStatsTable,
	          (time(NULL) - ClearInActive), AliasesTable);
	return MOD_CONT;
}

/*************************************************************************/

int chanstats_month(const char *name)
{
	SQLres *sql_res;
	ChannelStats *cs;
	char *chan_;
	time_t tbuf;
	char **sql_row;

	if (!name)
	{
		return MOD_CONT;
	}
	
	alog(LOG_NORMAL, langstr(ALOG_RESETTING_MONTHLY));
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	 "kicks=0, modes=0, topics=0, wasted=0, "
	 "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	 "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	 "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	 "time22=0, time23=0 WHERE type=3;", UStatsTable);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
	          "kicks=0, modes=0, topics=0, "
	          "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
	          "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
	          "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
	          "time22=0, time23=0 WHERE type=3;", CStatsTable);

	/* request 109 -  auto-delete channel after not used for 1 month */

	tbuf = (time(NULL) - ClearChanInActive);
	DenoraSQLQuery(DenoraDB, "SELECT chan FROM %s WHERE (lastspoke > 0) AND (lastspoke < %i);",
	 CStatsTable, tbuf);
	sql_res = sql_set_result(sqlcon);
	if (sql_num_rows(sql_res) > 0)
	{

		while ((sql_row = sql_fetch_row(sql_res)) != NULL)
		{
			if ((cs = find_cs(sql_row[0])))
			{
				alog(LOG_DEBUG,
				     "chanstats monthly: channel %s is expired, statserv will leave this chan, all stats are deleted");
				del_cs(cs);     /* make statserv part the chan */
				chan_ = sql_escape(sql_row[0]);
				DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE chan=\'%s\'",
				          CStatsTable, chan_);
				DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE chan=\'%s\'",
				          UStatsTable, chan_);
				free(chan_);
				if (LogChannel)
				{
					if (stricmp(LogChannel, sql_row[0]))
					{
						denora_cmd_part(s_StatServ, sql_row[0],
						                getstring(NULL,
						                          STATS_CHANSTATS_PART),
						                sql_row[0]);

					}
				}
			}
		}
		sql_free_result(sql_res);
		save_cs_db();
	}
	return MOD_CONT;
}
