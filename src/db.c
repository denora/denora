/*
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

#include "denora.h"

int db_getcurrent_chans(void);

/* last time we cleaned the server table */
static int ServerLastClean = -1;

/* last time we cleaned the user table */
static int UserLastClean = -1;

sqlite3* StatsDatabase;

/*************************************************************************/

/**
 * Load the Stats database from disk
 *
 * @return void - no returend value
 *
 */
void load_stats_db(void)
{
	sqlite3_stmt *stmt;
	char **data;

	StatsDatabase = DenoraOpenSQL(statsDB);
	stmt = DenoraPrepareQuery(StatsDatabase, "SELECT * FROM %s", StatsTable);
	data = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (data) {
		stats->users_max = atoi(data[0]);
		stats->users_max_time = atoi(data[1]);
		stats->chans_max = atoi(data[2]);
		stats->chans_max_time = atoi(data[3]);
		stats->servers_max = atoi(data[4]);
		stats->servers_max_time = atoi(data[5]);
		stats->opers_max = atoi(data[6]);
		stats->opers_max_time = atoi(data[7]);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(StatsDatabase);
}

/*************************************************************************/

/**
 * Save the Stats database to disk
 *
 * @return void - no returend value
 *
 */
void save_stats_db(void)
{
	DenoraSQLQuery(statsDB, "UPDATE %s SET users_max=%ld, users_max_time=%ld, chans_max=%ld, chans_max_time=%ld, \
					servers_max=%ld, servers_max_time=%ld, opers_max=%ld, opers_max_time=%ld", StatsTable, stats->users_max,
					stats->users_max_time, stats->chans_max, stats->chans_max_time, stats->servers_max, stats->servers_max_time,
					stats->opers_max, stats->opers_max_time);
}

/*************************************************************************/

/**
 * Connect to SQL Database
 *
 * @return void - no returend value
 *
 */
void db_connect(void)
{
	Exclude *e, *next;
	ChannelStats *cs;
	int tablecount = 0;
	char *sqlchan;

	if (!denora->do_sql)
	{
		alog(LOG_ERROR, langstring(ALOG_FAILED_SQL_CONNECT),
		     SQLErrMsg(sqlcon));
		return;
	}
	

	/* Checking for missing tables */
	if (!sql_check_table(ChanTable))
		tablecount++;
	if (!sql_check_table(IsOnTable))
		tablecount++;
	if (!sql_check_table(ServerTable))
		tablecount++;
	if (!sql_check_table(UserTable))
		tablecount++;
	if (!sql_check_table(GlineTable))
		tablecount++;
	if (!sql_check_table(ChanBansTable))
		tablecount++;
	if (!sql_check_table(CTCPTable))
		tablecount++;
	if (!sql_check_table(SpamTable))
		tablecount++;
	if (!sql_check_table(ChanExceptTable))
		tablecount++;
	if (!sql_check_table(ChanInviteTable))
		tablecount++;
	if (!sql_check_table(TLDTable))
		tablecount++;
	if (!sql_check_table(SglineTable))
		tablecount++;
	if (!sql_check_table(SqlineTable))
		tablecount++;
	if (!sql_check_table(MaxValueTable))
		tablecount++;
	if (!sql_check_table(AliasesTable))
		tablecount++;
	if (!sql_check_table(CStatsTable))
		tablecount++;
	if (!sql_check_table(UStatsTable))
		tablecount++;
	if (!sql_check_table(StatsTable))
		tablecount++;
	if (!sql_check_table(CurrentTable))
		tablecount++;
	if (!sql_check_table(ChanStatsTable))
		tablecount++;
	if (!sql_check_table(ServerStatsTable))
		tablecount++;
	if (!sql_check_table(ChanQuietTable))
		tablecount++;
	if (!sql_check_table(AdminTable))
		tablecount++;

	if (tablecount)
	{
		alog(LOG_ERROR,
		     "Denora is missing %d required sql tables, disabling sql. Run ./mydbgen to update your sql db.",
		     tablecount);
		denora->do_sql = 0;
		return;
	}

	/* Cleaning up the database */
	sql_clear_table(ChanTable);
	sql_clear_table(IsOnTable);
	sql_clear_table(ServerTable);
	sql_clear_table(UserTable);
	sql_clear_table(GlineTable);
	sql_clear_table(ChanBansTable);
	sql_clear_table(CTCPTable);
	sql_clear_table(SpamTable);
	sql_clear_table(ChanExceptTable);
	sql_clear_table(ChanInviteTable);
	sql_clear_table(TLDTable);
	sql_clear_table(SglineTable);
	sql_clear_table(SqlineTable);

	sql_query("DELETE FROM %s WHERE uname=\'%s\'", 
		  UStatsTable,s_StatServ);
	if (s_StatServ_alias)
	{
		sql_query("DELETE FROM %s WHERE uname=\'%s\'",
		          UStatsTable, s_StatServ_alias);
	}

	if (LogChannel)
	{
		if (!(cs = find_cs(LogChannel)))
		{
			if (cs)
			{
				del_cs(cs);
			}
			sqlchan = sql_escape(LogChannel);
			save_cs_db();
			sql_query("DELETE FROM %s WHERE chan=\'%s\'",
			          CStatsTable, sqlchan);
			sql_query("DELETE FROM %s WHERE chan=\'%s\'",
			          UStatsTable, sqlchan);
			free(sqlchan);
		}
	}
	if (!db_getcurrent_chans())
	{
		sql_query("INSERT INTO %s VALUES ('chans', 0, 0); ",
		          CurrentTable);
	}

	
}

/*************************************************************************/

/* -1 if server not found, servid else */
int db_getserver(char *serv)
{
	Server *s;
	char *sqlserv;
	int servid = -1;
	SQLres *sql_res;
	char **sql_row;

	if (!serv)
	{
		return servid;
	}

	s = server_find(serv);
	if (s && (s->sqlid > 0))
	{
		return s->sqlid;
	}

	

	if (!denora->do_sql)
	{
		return servid;
	}

	sqlserv = sql_escape(serv);
	sql_query("SELECT servid FROM %s WHERE server=\'%s\'", ServerTable, sqlserv);
	free(sqlserv);

	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			servid = strtol(sql_row[0], NULL, 10);
		}
		
		sql_free_result(sql_res);
	}
	if (s && servid > 0)
	{
		s->sqlid = servid;
	}
	return servid;
}

/*************************************************************************/

/* -1 if nick not found, nickid else */
int db_getnick(char *nick)
{
	int nickid = -1;
	User *u;
	SQLres *sql_res;
	char **sql_row;

	if (!nick)
	{
		return nickid;
	}

	

	u = user_find(nick);
	if (u && (u->sqlid > 0))
	{
		return u->sqlid;
	}

	if (!denora->do_sql)
	{
		return nickid;
	}

	sql_query("SELECT nickid FROM %s WHERE nick=\'%s\'", UserTable, nick);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			nickid = strtol(sql_row[0], NULL, 10);
		}
		
		sql_free_result(sql_res);
	}
	if (u && (nickid > 0))
	{
		u->sqlid = nickid;
	}
	return nickid;
}

/*************************************************************************/

/* -1 if nick not found, nickid else */
int db_checknick_nt(char *nick)
{
	int nickid = -1;
	User *u;
	SQLres *sql_res;
	SQLres *sql_res2;
	char *olduname;
	char *username, *host, *queryhost, *sqlnick;
	char **sql_row;

	u = user_find(nick);
	if (u && u->sqlid)
	{
		return u->sqlid;
	}

	if (!denora->do_sql)
	{
		return nickid;
	}

	if (u)
	{
		username = sql_escape(u->username);
		host = sql_escape(u->host);
		queryhost = sstrdup((myNumToken(host, '.') >= 2) ? strchr(host, '.') : host);
		sql_query("SELECT nickid,nick FROM %s WHERE (username=\'%s\' AND hostname LIKE \'%%%s\' AND online=\'N\') OR nick=\'%s\' ORDER BY connecttime DESC",
		          UserTable, username, queryhost, u->sqlnick);
		free(username);
		free(host);
		free(queryhost);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res))
			{
				sql_row = sql_fetch_row(sql_res);
				nickid = strtol(sql_row[0], NULL, 10);
				if (stricmp(sql_row[1], u->sqlnick) != 0)
				{
					/* Removing old user to avoid duplicate on update, which will happen shortly */
					sql_query(
					          "DELETE FROM %s WHERE nick=\'%s\' ",
					          UserTable, u->sqlnick);
					/* Getting uname from alias table with old nick */
					sql_query(
					          "SELECT uname FROM %s WHERE nick=\'%s\' ",
					          AliasesTable, u->sqlnick);
					sql_res2 = sql_set_result(sqlcon);
					if (sql_res2)
					{
						if (sql_num_rows(sql_res2))
						{
							sql_row = sql_fetch_row(sql_res2);
							olduname = sql_escape(sql_row[0]);
							/* Adding alias entry with new nick and old uname to avoid creation of new uname */
							sql_query(
							          "INSERT INTO %s (nick, uname) VALUES (\'%s\', \'%s\') ON DUPLICATE KEY UPDATE uname=\'%s\'",
							          AliasesTable, u->sqlnick, olduname,
							          olduname);
							free(olduname);
						}
						sql_free_result(sql_res2);
					}
				}
				sql_free_result(sql_res);
			}
			
		}
	}
	else
	{
		sqlnick = sql_escape(nick);
		sql_query("SELECT nickid FROM %s WHERE nick=\'%s\'",
		          UserTable, sqlnick);
		free(sqlnick);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res))
			{
				sql_row = sql_fetch_row(sql_res);
				nickid = strtol(sql_row[0], NULL, 10);
			}
			
			sql_free_result(sql_res);
		}
	}
	if (u && nickid)
	{
		u->sqlid = nickid;
	}
	return nickid;
}

/*************************************************************************/

/* fix me needs to return server name */
int db_getservfromnick(char *nick)
{
	int res = 0;
	SQLres *sql_res;
	char *sqlnick;
	char **sql_row;

	if (!denora->do_sql)
	{
		return res;
	}
	

	sqlnick = sql_escape(nick);
	sql_query("SELECT servid FROM %s WHERE nick=\'%s\'",
	          UserTable, sqlnick);
	free(sqlnick);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			res = strtol(sql_row[0], NULL, 10);
		}
		else
		{
			alog(LOG_NONEXISTANT, "nickname not found ! %s", nick);
		}
		sql_free_result(sql_res);
	}
	return res;
}

/*************************************************************************/

/**
 * Remove user from the SQL database files
 *
 * @param nick is the user whom needs to be removed
 * @param reason is the reason they left
 *
 * @return void - no returend value
 *
 */

void db_removenick(char *nick, char *reason)
{
	char *sqlreason;
	User *u;

	

	if (!denora->do_sql)
	{
		return;
	}

	u = user_find(nick);
	if (!u)
	{
		alog(LOG_NONEXISTANT, "Trying to remove nonexistent nick %s", nick);
		return;
	}

	if (!u->sqlid && db_getnick(u->sqlnick) == -1)
	{
		alog(LOG_NONEXISTANT, "Trying to remove nonexistant user %s", nick);
		return;
	}

	

	db_removefromchans(u->sqlid);
	if (UserCacheTime)
	{
		sqlreason = sql_escape(reason);
		sql_query(
		          "UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
		          UserTable, sqlreason, u->sqlid);
		free(sqlreason);
	}
	else
	{
		sql_query("DELETE FROM %s WHERE nickid=%d",
		          UserTable, u->sqlid);
	}


	
}

/*************************************************************************/

/**
 * Remove user from the SQL database files using Nick Tracking
 *
 * @param nick is the user whom needs to be removed
 * @param reason is the reason they left
 *
 * @return void - no returend value
 *
 */

void db_removenick_nt(char *nick, char *reason)
{
	char *sqlreason;
	int nickid = db_getnick(nick);
	User *u;
	SQLres *sql_res;
	char *newnick;
	char *username, *host, *queryhost;
	char **sql_row;

	if (!nick)
	{
		return;
	}

	u = user_find(nick);

	if (!denora->do_sql)
	{
		return;
	}

	if (!nickid)
	{
		alog(LOG_NONEXISTANT, "nickname not found ! %s", nick);
		return;
	}
	db_removefromchans(nickid);
	if (UserCacheTime)
	{
		sqlreason = sql_escape(reason);
		if (u)
		{
			username = sql_escape(u->username);
			host = sql_escape(u->host);
			queryhost = sstrdup((myNumToken(host, '.') >= 2) ? strchr(host, '.') : host);
			sql_query("SELECT nick FROM %s WHERE username=\'%s\' AND hostname LIKE \'%%%s\' AND online=\'Y\' AND nick != \'%s\' ORDER BY connecttime DESC",
			          UserTable, username, queryhost, u->sqlnick);
			free(queryhost);
			free(username);
			free(host);
			sql_res = sql_set_result(sqlcon);
			if (sql_res)
			{
				if (sql_num_rows(sql_res) > 0)
				{
					/* Getting nickname of user we found */
					sql_row = sql_fetch_row(sql_res);
					newnick = sql_escape(sql_row[0]);
					sql_free_result(sql_res);

					/* Deleting user as we got one with a similar mask which is still online */
					alog(LOG_DEBUG,
					     "db_removenick_nt(%s): There is %s with similar mask online, so we will delete %s.",
					     nick, newnick, nick);
					sql_query("DELETE FROM %s WHERE nickid=%d",
					          UserTable, nickid);

					free(newnick);
				}
				else
				{
					sql_free_result(sql_res);
					sql_query(
					          "UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
					          UserTable, sqlreason, nickid);
				}
			}
		}
		else
		{
			sql_query("UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
			          UserTable, sqlreason, nickid);
		}
		free(sqlreason);
	}
	else
	{
		sql_query("DELETE FROM %s WHERE nickid=%d",
		          UserTable, nickid);
	}

	
}

/*************************************************************************/

void db_removefromchans(int nickid)
{
	SQLres *sql_res;
	char **res;
	char *chan;
	int chanid;

	if (!denora->do_sql || nickid == -1)
	{
		return;
	}

	sql_query("SELECT %s.chanid, channel FROM %s, %s WHERE nickid=%d AND %s.chanid = %s.chanid",
	 IsOnTable, IsOnTable, ChanTable, nickid, ChanTable, IsOnTable);
	sql_res = sql_set_result(sqlcon);
	sql_query("DELETE FROM %s WHERE nickid=%d", IsOnTable,
	          nickid);
	
	if (sql_res)
	{
		while ((res = sql_fetch_row(sql_res)))
		{
			chan = sql_escape(res[1]);
			chanid = db_getchannel(chan);
			sql_query(
			          "UPDATE %s SET currentusers=currentusers-1 WHERE chanid=%d",
			          ChanTable, chanid);
			if (!ChanHasMode(chan, ircd->persist_char))
				db_checkemptychan(atoi(res[0]));
			free(chan);
		}
		
		sql_free_result(sql_res);
	}
}

/*************************************************************************/

void db_checkemptychan(int chanid)
{
	SQLres *sql_res;

	if (!denora->do_sql)
	{
		return;
	}
	sql_query("SELECT chanid FROM %s WHERE chanid=%d",
	          IsOnTable, chanid);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		
		if (!sql_num_rows(sql_res))
		{
			sql_query("DELETE FROM %s WHERE chanid=%d",
			          ChanTable, chanid);
		}
		sql_free_result(sql_res);
	}
}

/*************************************************************************/

int db_getcurrent_chans(void)
{
	int retcode = 0;
	SQLres *sql_res;
	char **sql_row;

	if (!denora->do_sql)
	{
		return 0;
	}
	sql_query("SELECT COUNT(*) FROM %s WHERE type=\'chans\'",
	          CurrentTable);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			retcode = atoi(sql_row[0]);
		}
		else
		{
			retcode = -1;
		}
		sql_free_result(sql_res);
	}
	return retcode;
}

/*************************************************************************/

int db_getlusers(int type)
{
	int retcode = 0;
	SQLres *sql_res;
	char **sql_row;

	if (!denora->do_sql)
	{
		return 0;
	}
	switch (type)
	{
		case LUSERS_USERS:
			sql_query(
			          "SELECT COUNT(*) FROM %s WHERE mode_li=\'N\'",
			          UserTable);
			break;
		case LUSERS_USERSINV:
			sql_query(
			          "SELECT COUNT(*) FROM %s WHERE mode_li=\'Y\'",
			          UserTable);
			break;
		case LUSERS_OPERS:
			sql_query(
			          "SELECT COUNT(*) FROM %s WHERE mode_lo=\'Y\'",
			          UserTable);
			break;
		case LUSERS_CHAN:
			sql_query("SELECT COUNT(*) FROM %s", ChanTable);
			break;
		case LUSERS_SERV:
			sql_query("SELECT COUNT(*) FROM %s", ServerTable);
			break;
		case LUSERS_USERSGLOB:
			sql_query("SELECT COUNT(*) FROM %s", UserTable);
			break;
		case LUSERS_USERSMAX:
			sql_query("SELECT val FROM %s WHERE type='users'",
			          MaxValueTable);
			break;
	}
	
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			retcode = atoi(sql_row[0]);
		}
		else
		{
			retcode = -1;
		}
		
		sql_free_result(sql_res);
	}
	return retcode;
}

/*************************************************************************/

int db_getchannel(char *chan)
{
	int chanid = -1;
	Channel *c;
	char *sqlchan;
	SQLres *sql_res;
	char **sql_row;

	if (!chan)
		return chanid;

	

	c = findchan(chan);
	if (c && c->sqlid > 0)
	{
		return c->sqlid;
	}

	if (!denora->do_sql)
	{
		return chanid;
	}

	sqlchan = sql_escape(chan);
	sql_query("SELECT chanid FROM %s WHERE channel=\'%s\'",
	          ChanTable, sqlchan);
	free(sqlchan);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			chanid = strtol(sql_row[0], NULL, 10);
		}
		else
		{
			alog(LOG_NONEXISTANT, "channel not found ! %s", chan);
		}
		
		sql_free_result(sql_res);
	}
	if (c && chanid > 0)
	{
		c->sqlid = chanid;
	}
	return chanid;
}

/*************************************************************************/

char *db_getchannel_byid(int chanid)
{
	char *res = NULL;
	SQLres *sql_res;
	char **row;

	if (!denora->do_sql)
	{
		return NULL;
	}
	

	sql_query("SELECT channel FROM %s WHERE chanid=%d",
	          ChanTable, chanid);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (!sql_num_rows(sql_res))
		{
			alog(LOG_NONEXISTANT, "channel not found ! %d", chanid);
		}
		else
		{
			row = sql_fetch_row(sql_res);
			res = sql_escape(row[0]);
		}
		
		sql_free_result(sql_res);
	}
	return res;
}

/*************************************************************************/

int db_getchannel_users(char *chan)
{
	int res = 0;
	char *sqlchan;
	SQLres *sql_res;
	char **sql_row;

	if (!denora->do_sql)
	{
		return -1;
	}
	

	sqlchan = sql_escape(chan);
	sql_query(
	          "SELECT currentusers FROM %s WHERE channel=\'%s\'",
	          ChanTable, sqlchan);
	free(sqlchan);
	sql_res = sql_set_result(sqlcon);
	
	if (sql_res)
	{
		if (sql_num_rows(sql_res))
		{
			sql_row = sql_fetch_row(sql_res);
			res = strtol(sql_row[0], NULL, 10);
		}
		else
		{
			alog(LOG_DEBUG,
			     "debug: unable to find the requested channel %s", chan);
		}
		
		sql_free_result(sql_res);
	}
	return res;
}

/*************************************************************************/

/* chan is created if not exists */
int db_getchancreate(char *chan)
{
	int chanid = -1;
	int created = 0;
	int newcase = 0;
	Channel *c;
	char *channel;
	SQLres *sql_res;
	char **row;

	c = findchan(chan);
	if (c)
	{
		if (c->sqlid)
		{
			return c->sqlid;
		}
		else
		{
			channel = sstrdup(c->sqlchan);
		}
	}
	else
	{
		channel = sql_escape(chan);
	}


	if (!denora->do_sql)
	{
		free(channel);
		return chanid;
	}

	if (!c || c->sqlid < 1)
	{
		sql_query("SELECT chanid, channel FROM %s WHERE channel=\'%s\'",
		          ChanTable, channel);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res))
			{
				row = sql_fetch_row(sql_res);
				alog(LOG_DEBUG, "debug: RESULT: %s %s", row[0], row[1]);
				chanid = atoi(row[0]);
				if (channel != row[1])
				{
					newcase = 1;
				}
			}
			sql_free_result(sql_res);
		}
		if (chanid < 1)
		{
			sql_query("INSERT INTO %s (channel) VALUES (\'%s\')",
			          ChanTable, channel);
			chanid = sql_insertid(sqlcon);
			created = 1;
		}
	}

	if (newcase)
	{
		if (!created && chanid != -1)
		{
			/* We update the channel name in case casing has changed */
			sql_query("UPDATE %s SET channel=\'%s\' WHERE chanid=%d",
			          ChanTable, channel, chanid);
		}
		sql_query("UPDATE %s SET chan=\'%s\' WHERE chan=\'%s\'",
		          CStatsTable, channel, channel);
		sql_query("UPDATE %s SET chan=\'%s\' WHERE chan=\'%s\'",
		          UStatsTable, channel, channel);
	}
	
	free(channel);
	if (c && chanid > 0)
	{
		c->sqlid = chanid;
	}
	return chanid;
}

/*************************************************************************/

/* cleanup the server table, removing old entries */
void db_cleanserver()
{

	SQLres *sql_res;
	int curtime = time(NULL);

	if (!denora->do_sql)
	{
		return;
	}

	if (ServerLastClean == -1)
	{
		ServerLastClean = curtime;
	}
	
	if (curtime > (ServerLastClean + ServerCleanFreq))
	{
		ServerLastClean = curtime;
		sql_query
		(
		 "SELECT server FROM %s WHERE online=\'N\' AND lastsplit<FROM_UNIXTIME(%d)",
		 ServerTable, curtime - ServerCacheTime);

		sql_res = sql_set_result(sqlcon);
		if (sql_num_rows(sql_res))
		{
			sql_free_result(sql_res);
			sql_query
			("DELETE FROM %s WHERE online=\'N\' AND lastsplit<FROM_UNIXTIME(%d)",
			 ServerTable, curtime - ServerCacheTime);
		}
		else
		{
			if (sql_res)
			{
				sql_free_result(sql_res);
			}
		}
	}
}

/*************************************************************************/

/* cleanup the user table, removing old entries */
void db_cleanuser()
{
	SQLres *sql_res;
	int curtime = time(NULL);

	if (!denora->do_sql)
	{
		return;
	}

	if (UserLastClean == -1)
	{
		UserLastClean = curtime;
	}

	if (curtime > (UserLastClean + UserCleanFreq))
	{
		UserLastClean = curtime;
		sql_query
		(
		 "SELECT nick FROM %s WHERE online=\'N\' AND lastquit<FROM_UNIXTIME(%d)",
		 UserTable, curtime - UserCacheTime);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res))
			{
				sql_query
				(
				 "DELETE FROM %s WHERE online=\'N\' AND lastquit<FROM_UNIXTIME(%d)",
				 UserTable, curtime - UserCacheTime);
			}
			sql_free_result(sql_res);
		}
	}
}

/*************************************************************************/

/* check if chans > chans_max */
void do_checkchansmax()
{
	int diff1 = 0;

	if (stats->chans > stats->chans_max)
	{
		stats->chans_max = stats->chans;
		stats->chans_max_time = time(NULL);
		diff1 = 1;
	}
	if (!stats->chans_max_time)
	{
		stats->chans_max_time = time(NULL);
		diff1 = 1;
	}
	

	if (stats->chans > stats->daily_chans)
	{
		stats->daily_chans++;
		stats->daily_chans_time = time(NULL);
	}
	sql_query(
	          "UPDATE %s SET val=%d, time=%ld WHERE type='chans'",
	          CurrentTable, stats->chans, time(NULL));
	if (diff1)
	{
		sql_query
		(
		 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='channels'",
		 MaxValueTable, stats->chans_max,
		 (long int) stats->chans_max_time);
	}
}

/*************************************************************************/

/* check if users > users_max */
void add_current_user(Server * s)
{
	int diff1 = 0;
	int diff2 = 0;

	

	if (!s)
	{
		return;
	}
	s->ss->currentusers++;
	s->ss->totalusersever++;
	stats->users++;
	stats->totalusersever++;

	
	if (stats->users > stats->users_max)
	{
		stats->users_max = stats->users;
		stats->users_max_time = time(NULL);
		diff1 = 1;
	}
	if (!stats->users_max_time)
	{
		stats->users_max_time = time(NULL);
		diff1 = 1;
	}
	
	if (stats->users > stats->daily_users)
	{
		stats->daily_users++;
		stats->daily_users_time = time(NULL);
		diff2 = 1;
	}
	if (!stats->daily_users_time)
	{
		stats->daily_users_time = time(NULL);
		diff2 = 1;
	}
	
	if (s->ss->currentusers > s->ss->maxusers)
	{
		s->ss->maxusers = s->ss->currentusers;
		s->ss->maxusertime = time(NULL);
	}
	
	if (denora->do_sql)
	{
		sql_query(
		          "UPDATE %s SET val=%d, time=%ld WHERE type='users'",
		          CurrentTable, stats->users, time(NULL));
		if (diff2)
		{
			sql_query
			(
			 "UPDATE %s SET val=%d, time=%ld WHERE type='daily_users'",
			 CurrentTable, stats->daily_users,
			 stats->daily_users_time);
		}
		if (diff1)
		{
			sql_query
			(
			 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='users'",
			 MaxValueTable, stats->users_max,
			 (long int) stats->users_max_time);
		}
		sql_query
		(
		 "UPDATE %s SET currentusers=%d, maxusers=%d, maxusertime=%d WHERE server='%s'",
		 ServerTable, s->ss->currentusers, s->ss->maxusers,
		 s->ss->maxusertime, s->name);
	}
}

/*************************************************************************/

void del_current_user(Server * s)
{
	
	if (!s)
	{
		return;
	}
	s->ss->currentusers--;
	stats->users--;
	

	if (denora->do_sql)
	{
		sql_query("UPDATE %s SET val=%d, time=%ld WHERE type='users'",
		          CurrentTable, stats->users, time(NULL));
		sql_query("UPDATE %s SET currentusers=%d WHERE server='%s'",
		          ServerTable, s->ss->currentusers, s->name);
	}
}

/*************************************************************************/

/* check if servs > servs_max */
void do_checkservsmax()
{
	int diff1 = 0;

	

	if (stats->servers > stats->servers_max)
	{
		stats->servers_max = stats->servers;
		stats->servers_max_time = time(NULL);
		diff1 = 1;
	}
	if (!stats->servers_max_time)
	{
		stats->servers_max_time = time(NULL);
		diff1 = 1;
	}
	

	if (stats->servers > stats->daily_servers)
	{
		stats->daily_servers++;
		stats->daily_servers_time = time(NULL);
	}
	if (denora->do_sql)
	{
		sql_query("UPDATE %s SET val=%d, time=%ld WHERE type='servers'",
		          CurrentTable, stats->servers, time(NULL));
		if (diff1)
		{
			sql_query("UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='servers'",
			 MaxValueTable, stats->servers_max,
			 (long int) stats->servers_max_time);
		}
	}
}

/*************************************************************************/

void add_oper_count(User * u)
{
	Server *s;
	int diff1 = 0;

	stats->opers++;
	s = u->server;

	

	if (s)
	{
		s->ss->opers++;
	}
	

	if (stats->opers > stats->opers_max)
	{
		stats->opers_max = stats->opers;
		stats->opers_max_time = time(NULL);
		diff1 = 1;
	}
	if (!stats->opers_max_time)
	{
		stats->opers_max_time = time(NULL);
		diff1 = 1;
	}

	

	if (stats->opers > stats->daily_opers)
	{
		stats->daily_opers++;
		stats->daily_opers_time = time(NULL);
	}
	if (!stats->daily_opers_time)
	{
		stats->daily_opers_time = time(NULL);
	}
	

	if (s)
	{
		if (s->ss->opers > s->ss->maxopers)
		{
			s->ss->maxopers = s->ss->opers;
			s->ss->maxopertime = time(NULL);
		}
		if (!s->ss->maxopertime)
		{
			s->ss->maxopertime = time(NULL);
		}
		sql_query
		(
		 "UPDATE %s SET opers=%d, maxopers=%ld, maxopertime=%ld WHERE server='%s'",
		 ServerTable, s->ss->opers, s->ss->maxopers,
		 (long int) s->ss->maxopertime, s->name);

	}
	sql_query(
	          "UPDATE %s SET val=%d, time=%ld WHERE type='opers'",
	          CurrentTable, stats->opers, (long int) time(NULL));
	if (diff1)
	{
		sql_query
		(
		 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='opers'",
		 MaxValueTable, stats->opers_max,
		 (long int) stats->opers_max_time);
	}
}

/*************************************************************************/

void del_oper_count(User * u)
{

	stats->opers--;
	if (u && u->server && u->server->ss)
	{
		u->server->ss->opers--;
		sql_query
		(
		 "UPDATE %s SET opers=%d, maxopers=%ld, maxopertime=%ld WHERE server='%s'",
		 ServerTable, u->server->ss->opers, u->server->ss->maxopers,
		 (long int) u->server->ss->maxopertime, u->server->name);
	}

	sql_query("UPDATE %s SET val=%d, time=%ld WHERE type='opers'",
	          CurrentTable, stats->opers, time(NULL));
}

/*************************************************************************/

int chans_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
	SQLres *sql_res;

	ts = time(NULL);

#ifdef _WIN32
	localtime_s(&mytime, &ts);
#else
	mytime = *localtime(&ts);
#endif

	year = 1900 + mytime.tm_year;
	month = 1 + mytime.tm_mon;

	if (name)
	{
		alog(LOG_DEBUG, "Executing for %s", name);
	}

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	sql_query(
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          ChanStatsTable, mytime.tm_mday, month, year);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) > 0)
		{
			sql_query
			("UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 ChanStatsTable, mytime.tm_hour, stats->chans,
			 mytime.tm_mday, month, year);
		}
		else
		{
			sql_query
			("INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 ChanStatsTable, mytime.tm_hour, mytime.tm_mday, month,
			 year, stats->chans);
		}
		sql_free_result(sql_res);
	}
	return MOD_CONT;
}

/*************************************************************************/

int servers_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
	SQLres *sql_res;

	ts = time(NULL);
#ifdef _WIN32
	localtime_s(&mytime, &ts);
#else
	mytime = *localtime(&ts);
#endif

	year = 1900 + mytime.tm_year;
	month = 1 + mytime.tm_mon;

	if (name)
	{
		alog(LOG_DEBUG, "Executing for %s", name);
	}

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	sql_query(
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          ServerStatsTable, mytime.tm_mday, month, year);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) > 0)
		{
			sql_query
			("UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 ServerStatsTable, mytime.tm_hour, stats->servers,
			 mytime.tm_mday, month, year);
		}
		else
		{
			sql_query
			("INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 ServerStatsTable, mytime.tm_hour, mytime.tm_mday, month,
			 year, stats->servers);
		}
		sql_free_result(sql_res);
	}
	return MOD_CONT;
}

/*************************************************************************/

int users_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
	SQLres *sql_res;

	ts = time(NULL);
#ifdef _WIN32
	localtime_s(&mytime, &ts);
#else
	mytime = *localtime(&ts);
#endif

	year = 1900 + mytime.tm_year;
	month = 1 + mytime.tm_mon;

	if (name)
	{
		alog(LOG_DEBUG, "Executing for %s", name);
	}

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	sql_query(
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          StatsTable, mytime.tm_mday, month, year);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) > 0)
		{
			sql_query
			(
			 "UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 StatsTable, mytime.tm_hour, stats->users, mytime.tm_mday,
			 month, year);
		}
		else
		{
			sql_query
			(
			 "INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 StatsTable, mytime.tm_hour, mytime.tm_mday, month, year,
			 stats->users);
		}
		sql_free_result(sql_res);
	}
	return MOD_CONT;
}
