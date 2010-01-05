/* Cron Chanstats
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

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id$");
    moduleSetType(CORE);

    evt = createCronEvent(CRON_MIDNIGHT, chanstats_daily);
    moduleAddCronEvent(evt);
    evt = createCronEvent(CRON_WEEKLY_MONDAY, chanstats_weekly);
    moduleAddCronEvent(evt);
    evt = createCronEvent(CRON_MONTHLY, chanstats_month);
    moduleAddCronEvent(evt);

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
    if (!denora->do_sql) {
        return MOD_CONT;
    }
    if (!name) {
        return MOD_CONT;
    }
    SET_SEGV_LOCATION();
    alog(LOG_NORMAL, langstr(ALOG_RESET_DAILY));
    rdb_query
        (QUERY_LOW,
         "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
         "kicks=0, modes=0, topics=0, wasted=0, "
         "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
         "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
         "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
         "time22=0, time23=0  WHERE type=1;", UStatsTable);
    rdb_query(QUERY_LOW,
              "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
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
    if (!denora->do_sql) {
        return MOD_CONT;
    }
    if (!name) {
        return MOD_CONT;
    }
    SET_SEGV_LOCATION();
    alog(LOG_NORMAL, langstr(ALOG_RESETTING_WEEKLY));
    rdb_query
        (QUERY_LOW,
         "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
         "kicks=0, modes=0, topics=0, wasted=0, "
         "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
         "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
         "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
         "time22=0, time23=0  WHERE type=2;", UStatsTable);
    rdb_query(QUERY_LOW,
              "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
              "kicks=0, modes=0, topics=0, "
              "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
              "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
              "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
              "time22=0, time23=0  WHERE type=2;", CStatsTable);
    rdb_query(QUERY_LOW,
              "DELETE %s.*,%s.* FROM %s,%s WHERE %s.uname = %s.uname AND %s.lastspoke < %i AND %s.ignore = 'N';",
              UStatsTable, AliasesTable, UStatsTable, AliasesTable,
              UStatsTable, AliasesTable, UStatsTable,
              (time(NULL) - ClearInActive), AliasesTable);
    return MOD_CONT;
}

/*************************************************************************/

int chanstats_month(const char *name)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
    ChannelStats *cs;
    char *chan_;
#endif
    time_t tbuf;

    if (!denora->do_sql) {
        return MOD_CONT;
    }
    if (!name) {
        return MOD_CONT;
    }
    SET_SEGV_LOCATION();
    alog(LOG_NORMAL, langstr(ALOG_RESETTING_MONTHLY));
    rdb_query
        (QUERY_LOW,
         "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
         "kicks=0, modes=0, topics=0, wasted=0, "
         "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
         "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
         "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
         "time22=0, time23=0 WHERE type=3;", UStatsTable);
    rdb_query(QUERY_LOW,
              "UPDATE %s SET letters=0, words=0, line=0, actions=0, smileys=0, "
              "kicks=0, modes=0, topics=0, "
              "time0=0, time1=0, time2=0, time3=0, time4=0, time5=0, time6=0, time7=0, "
              "time8=0, time9=0, time10=0, time11=0, time12=0, time13=0, time14=0, "
              "time15=0, time16=0, time17=0, time18=0, time19=0, time20=0, time21=0, "
              "time22=0, time23=0 WHERE type=3;", CStatsTable);

    /* request 109 -  auto-delete channel after not used for 1 month */

    tbuf = (time(NULL) - ClearChanInActive);
    rdb_query
        (QUERY_HIGH,
         "SELECT chan FROM %s WHERE (lastspoke > 0) AND (lastspoke < %i);",
         CStatsTable, tbuf);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_num_rows(mysql_res) > 0) {
        SET_SEGV_LOCATION();
        while ((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
            if ((cs = find_cs(mysql_row[0]))) {
                alog(LOG_DEBUG,
                     "chanstats monthly: channel %s is expired, statserv will leave this chan, all stats are deleted");
                del_cs(cs);     /* make statserv part the chan */
                chan_ = rdb_escape(mysql_row[0]);
                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          CStatsTable, chan_);
                rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                          UStatsTable, chan_);
                free(chan_);
                if (LogChannel) {
                    if (stricmp(LogChannel, mysql_row[0])) {
                        denora_cmd_part(s_StatServ, mysql_row[0],
                                        getstring(NULL,
                                                  STATS_CHANSTATS_PART),
                                        mysql_row[0]);

                    }
                }
            }
        }
        save_cs_db();
    }
#endif
    return MOD_CONT;
}
