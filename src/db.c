/*
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

#include "denora.h"

int db_getcurrent_chans(void);

/* last time we cleaned the server table */
static int ServerLastClean = -1;

/* last time we cleaned the user table */
static int UserLastClean = -1;

/*************************************************************************/

/**
 * Load the Stats database from disk
 *
 * @return void - no returend value
 *
 */
void load_stats_db(void)
{
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(statsDB, STATSDB_VERSION, &key, &value);
	int retval = 0;

        if (!dbptr)
        {
                return;                 /* Bang, an error occurred */
        }
        SET_SEGV_LOCATION();

	while (1)
	{
		/* read a new entry and fill key and value with it -Certus */
		retval = new_read_db_entry(&key, &value, dbptr->fptr);

		if (retval == DB_READ_ERROR)
		{
			alog(LOG_NORMAL, langstr(ALOG_DB_ERROR), dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_EOF_ERROR)
		{
			alog(LOG_EXTRADEBUG, langstr(ALOG_DEBUG_DB_OK),
			     dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_READ_BLOCKEND)            /* DB_READ_BLOCKEND */
		{
			/* a channel has completely been read. put any checks in here! */
		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
			{
				continue;
			}
			SET_SEGV_LOCATION();

			if (!stricmp(key, "usermax"))
			{
				stats->users_max = atoi(value);
			}
			else if (!stricmp(key, "usermaxtime"))
			{
				stats->users_max_time = atoi(value);
			}
			else if (!stricmp(key, "channelmax"))
			{
				stats->chans_max = atoi(value);
			}
			else if (!stricmp(key, "channelmaxtime"))
			{
				stats->chans_max_time = atoi(value);
			}
			else if (!stricmp(key, "servermax"))
			{
				stats->servers_max = atoi(value);
			}
			else if (!stricmp(key, "servermaxtime"))
			{
				stats->servers_max_time = atoi(value);
			}
			else if (!stricmp(key, "opermax"))
			{
				stats->opers_max = atoi(value);
				if ((int) stats->opers_max < 0)
				{
					stats->opers_max = 0;
				}
			}
			else if (!stricmp(key, "opermaxtime"))
			{
				stats->opers_max_time = atoi(value);
			}
		}                       /* else */
	}                           /* while */
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
	DenoraDBFile *dbptr = filedb_create(statsDB, STATSDB_VERSION);

	SET_SEGV_LOCATION();
	new_write_db_entry("usermax", dbptr, "%ld", stats->users_max);
	new_write_db_entry("usermaxtime", dbptr, "%ld",
	                   (long int) stats->users_max_time);
	new_write_db_entry("channelmax", dbptr, "%ld", stats->chans_max);
	new_write_db_entry("channelmaxtime", dbptr, "%ld",
	                   (long int) stats->chans_max_time);
	new_write_db_entry("servermax", dbptr, "%ld", stats->servers_max);
	new_write_db_entry("servermaxtime", dbptr, "%ld",
	                   (long int) stats->servers_max_time);
	if ((int) stats->opers_max >= 0)
	{
		new_write_db_entry("opermax", dbptr, "%ld", stats->opers_max);
	}
	else
	{
		new_write_db_entry("opermax", dbptr, "%ld", 0);
	}
	new_write_db_entry("opermaxtime", dbptr, "%ld",
	                   (long int) stats->opers_max_time);
	new_write_db_endofblock(dbptr);

	SET_SEGV_LOCATION();
	filedb_close(dbptr, NULL, NULL);  /* close file */
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
		     rdb_error_msg());
		return;
	}
	SET_SEGV_LOCATION();

	/* Checking for missing tables */
	if (!rdb_check_table(ChanTable))
		tablecount++;
	if (!rdb_check_table(IsOnTable))
		tablecount++;
	if (!rdb_check_table(ServerTable))
		tablecount++;
	if (!rdb_check_table(UserTable))
		tablecount++;
	if (!rdb_check_table(GlineTable))
		tablecount++;
	if (!rdb_check_table(ChanBansTable))
		tablecount++;
	if (!rdb_check_table(CTCPTable))
		tablecount++;
	if (!rdb_check_table(SpamTable))
		tablecount++;
	if (!rdb_check_table(ChanExceptTable))
		tablecount++;
	if (!rdb_check_table(ChanInviteTable))
		tablecount++;
	if (!rdb_check_table(TLDTable))
		tablecount++;
	if (!rdb_check_table(SglineTable))
		tablecount++;
	if (!rdb_check_table(SqlineTable))
		tablecount++;
	if (!rdb_check_table(MaxValueTable))
		tablecount++;
	if (!rdb_check_table(AliasesTable))
		tablecount++;
	if (!rdb_check_table(CStatsTable))
		tablecount++;
	if (!rdb_check_table(UStatsTable))
		tablecount++;
	if (!rdb_check_table(StatsTable))
		tablecount++;
	if (!rdb_check_table(CurrentTable))
		tablecount++;
	if (!rdb_check_table(ChanStatsTable))
		tablecount++;
	if (!rdb_check_table(ServerStatsTable))
		tablecount++;
	if (!rdb_check_table(ChanQuietTable))
		tablecount++;
	if (!rdb_check_table(AdminTable))
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
	rdb_clear_table(ChanTable);
	rdb_clear_table(IsOnTable);
	rdb_clear_table(ServerTable);
	rdb_clear_table(UserTable);
	rdb_clear_table(GlineTable);
	rdb_clear_table(ChanBansTable);
	rdb_clear_table(CTCPTable);
	rdb_clear_table(SpamTable);
	rdb_clear_table(ChanExceptTable);
	rdb_clear_table(ChanInviteTable);
	rdb_clear_table(TLDTable);
	rdb_clear_table(SglineTable);
	rdb_clear_table(SqlineTable);

	e = first_exclude();
	while (e)
	{
		next = next_exclude();
		rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname=\'%s\'",
		          UStatsTable, e->name);
		e = next;
	}
	rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname=\'%s\'", 
		  UStatsTable,s_StatServ);
	if (s_StatServ_alias)
	{
		rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname=\'%s\'",
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
			sqlchan = rdb_escape(LogChannel);
			save_cs_db();
			rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
			          CStatsTable, sqlchan);
			rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
			          UStatsTable, sqlchan);
			free(sqlchan);
		}
	}
	if (!db_getcurrent_chans())
	{
		rdb_query(QUERY_LOW, "INSERT INTO %s VALUES ('chans', 0, 0); ",
		          CurrentTable);
	}

	SET_SEGV_LOCATION();
}

/*************************************************************************/

/* serv should be db_escape'd before call */
/* -1 if server not found, servid else */
int db_getserver(char *serv)
{
	Server *s;
	int servid = -1;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!serv)
	{
		return servid;
	}

	s = server_find(serv);
	if (s && (s->sqlid > 0))
	{
		return s->sqlid;
	}

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return servid;
	}

	rdb_query(QUERY_HIGH, "SELECT servid FROM %s WHERE server=\'%s\'",
	          ServerTable, serv);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			mysql_row = mysql_fetch_row(mysql_res);
			servid = strtol(mysql_row[0], NULL, 10);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	if (s && servid > 0)
	{
		s->sqlid = servid;
	}
	return servid;
}

/*************************************************************************/

/* nick should be db_escape'd before call */
/* -1 if nick not found, nickid else */
int db_getnick(char *nick)
{
	int nickid = -1;
	User *u;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!nick)
	{
		return nickid;
	}

	SET_SEGV_LOCATION();

	u = user_find(nick);
	if (u && (u->sqlid > 0))
	{
		return u->sqlid;
	}

	if (!denora->do_sql)
	{
		return nickid;
	}

	rdb_query(QUERY_HIGH, "SELECT nickid FROM %s WHERE nick=\'%s\'",
	          UserTable, nick);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			mysql_row = mysql_fetch_row(mysql_res);
			nickid = strtol(mysql_row[0], NULL, 10);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	if (u && (nickid > 0))
	{
		u->sqlid = nickid;
	}
	return nickid;
}

/*************************************************************************/

/* nick should be db_escape'd before call */
/* -1 if nick not found, nickid else */
int db_checknick_nt(char *nick)
{
	int nickid = -1;
	User *u;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	MYSQL_RES *mysql_res2;
	char *olduname;
#endif
	char *username, *host, *queryhost;

	SET_SEGV_LOCATION();

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
		username = rdb_escape(u->username);
		host = rdb_escape(u->host);
		queryhost = (myNumToken(host, '.') >= 2) ? strchr(host, '.') : host;
		rdb_query(QUERY_HIGH,
		          "SELECT nickid,nick FROM %s WHERE (username=\'%s\' AND hostname LIKE \'%%%s\' AND online=\'N\') OR nick=\'%s\' ORDER BY connecttime DESC",
		          UserTable, username, queryhost, u->sqlnick);
		free(username);
		free(host);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_res)
		{
			if (mysql_num_rows(mysql_res))
			{
				mysql_row = mysql_fetch_row(mysql_res);
				nickid = strtol(mysql_row[0], NULL, 10);
				if (stricmp(mysql_row[1], u->sqlnick) != 0)
				{
					/* Removing old user to avoid duplicate on update, which will happen shortly */
					rdb_query(QUERY_HIGH,
					          "DELETE FROM %s WHERE nick=\'%s\' ",
					          UserTable, u->sqlnick);
					/* Getting uname from alias table with old nick */
					rdb_query(QUERY_HIGH,
					          "SELECT uname FROM %s WHERE nick=\'%s\' ",
					          AliasesTable, u->sqlnick);
					mysql_res2 = mysql_store_result(mysql);
					if (mysql_res2)
					{
						if (mysql_num_rows(mysql_res2))
						{
							mysql_row = mysql_fetch_row(mysql_res2);
							olduname = rdb_escape(mysql_row[0]);
							/* Adding alias entry with new nick and old uname to avoid creation of new uname */
							rdb_query(QUERY_HIGH,
							          "INSERT INTO %s (nick, uname) VALUES (\'%s\', \'%s\') ON DUPLICATE KEY UPDATE uname=\'%s\'",
							          AliasesTable, u->sqlnick, olduname,
							          olduname);
							free(olduname);
						}
						mysql_free_result(mysql_res2);
					}
				}
				mysql_free_result(mysql_res);
			}
			SET_SEGV_LOCATION();
		}
#endif
	}
	else
	{
		rdb_query(QUERY_HIGH, "SELECT nickid FROM %s WHERE nick=\'%s\'",
		          UserTable, nick);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_res)
		{
			if (mysql_num_rows(mysql_res))
			{
				mysql_row = mysql_fetch_row(mysql_res);
				nickid = strtol(mysql_row[0], NULL, 10);
			}
			SET_SEGV_LOCATION();
			mysql_free_result(mysql_res);
		}
#endif
	}
	if (u && nickid)
	{
		u->sqlid = nickid;
	}
	return nickid;
}

/*************************************************************************/

int db_getservfromnick(char *nick)
{
	int res = 0;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!denora->do_sql)
	{
		return -1;
	}
	SET_SEGV_LOCATION();

	rdb_query(QUERY_HIGH, "SELECT servid FROM %s WHERE nick=\'%s\'",
	          UserTable, nick);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			mysql_row = mysql_fetch_row(mysql_res);
			res = strtol(mysql_row[0], NULL, 10);
		}
		else
		{
			alog(LOG_NONEXISTANT, "nickname not found ! %s", nick);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
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

	SET_SEGV_LOCATION();

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

	if (!u->sqlid && db_getnick(nick) == -1)
	{
		alog(LOG_NONEXISTANT, "Trying to remove nonexistant user %s", nick);
		return;
	}

	SET_SEGV_LOCATION();

	db_removefromchans(u->sqlid);
	if (UserCacheTime)
	{
		sqlreason = rdb_escape(reason);
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
		          UserTable, sqlreason, u->sqlid);
		free(sqlreason);
	}
	else
	{
		rdb_query(QUERY_LOW, "DELETE FROM %s WHERE nickid=%d",
		          UserTable, u->sqlid);
	}


	SET_SEGV_LOCATION();
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
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	char *newnick;
#endif
	char *username, *host, *queryhost;

	if (!nick)
	{
		return;
	}

	u = user_find(nick);

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return;
	}

	if (!nickid)
	{
		alog(LOG_NONEXISTANT, "nickname not found ! %s", nick);
		return;
	}
	SET_SEGV_LOCATION();

	db_removefromchans(nickid);
	if (UserCacheTime)
	{
		sqlreason = rdb_escape(reason);
		if (u)
		{
			username = rdb_escape(u->username);
			host = rdb_escape(u->host);
			queryhost = (myNumToken(host, '.') >= 2) ? strchr(host, '.') : host;
			rdb_query(QUERY_HIGH,
			          "SELECT nick FROM %s WHERE username=\'%s\' AND hostname LIKE \'%%%s\' AND online=\'Y\' AND nick != \'%s\' ORDER BY connecttime DESC",
			          UserTable, username, queryhost, u->sqlnick);
			free(queryhost);
			free(username);
			free(host);
#ifdef USE_MYSQL
			mysql_res = mysql_store_result(mysql);
			if (mysql_res)
			{
				if (mysql_num_rows(mysql_res) > 0)
				{
					/* Getting nickname of user we found */
					mysql_row = mysql_fetch_row(mysql_res);
					newnick = rdb_escape(mysql_row[0]);
					mysql_free_result(mysql_res);

					/* Deleting user as we got one with a similar mask which is still online */
					alog(LOG_DEBUG,
					     "db_removenick_nt(%s): There is %s with similar mask online, so we will delete %s.",
					     nick, newnick, nick);
					rdb_query(QUERY_LOW, "DELETE FROM %s WHERE nickid=%d",
					          UserTable, nickid);

					free(newnick);
				}
				else
				{
					mysql_free_result(mysql_res);
					rdb_query(QUERY_LOW,
					          "UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
					          UserTable, sqlreason, nickid);
				}
			}
			SET_SEGV_LOCATION();
#endif
		}
		else
		{
			rdb_query(QUERY_LOW,
			          "UPDATE %s SET online=\'N\', lastquit=NOW(), lastquitmsg=\'%s\', servid=0 WHERE nickid=%d",
			          UserTable, sqlreason, nickid);
		}
		free(sqlreason);
	}
	else
	{
		rdb_query(QUERY_LOW, "DELETE FROM %s WHERE nickid=%d",
		          UserTable, nickid);
	}

	SET_SEGV_LOCATION();
}

/*************************************************************************/

void db_removefromchans(int nickid)
{
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	char **res;
	int chanid;
#endif

	SET_SEGV_LOCATION();

	if (!denora->do_sql || nickid == -1)
	{
		return;
	}

	rdb_query
	(QUERY_HIGH,
	 "SELECT %s.chanid, channel FROM %s, %s WHERE nickid=%d AND %s.chanid = %s.chanid",
	 IsOnTable, IsOnTable, ChanTable, nickid, ChanTable, IsOnTable);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
#endif
	rdb_query(QUERY_LOW, "DELETE FROM %s WHERE nickid=%d", IsOnTable,
	          nickid);
	SET_SEGV_LOCATION();
#ifdef USE_MYSQL
	if (mysql_res)
	{
		while ((res = mysql_fetch_row(mysql_res)))
		{
			char *chan = rdb_escape(res[1]);
			chanid = db_getchannel(chan);
			rdb_query(QUERY_LOW,
			          "UPDATE %s SET currentusers=currentusers-1 WHERE chanid=%d",
			          ChanTable, chanid);
			if (!ChanHasMode(chan, ircd->persist_char))
				db_checkemptychan(atoi(res[0]));
			free(chan);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
}

/*************************************************************************/

void db_checkemptychan(int chanid)
{
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return;
	}
	rdb_query(QUERY_HIGH, "SELECT chanid FROM %s WHERE chanid=%d",
	          IsOnTable, chanid);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		SET_SEGV_LOCATION();
		if (!mysql_num_rows(mysql_res))
		{
			rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chanid=%d",
			          ChanTable, chanid);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
}

/*************************************************************************/

int db_getcurrent_chans(void)
{
	int retcode = 0;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!denora->do_sql)
	{
		return 0;
	}
	SET_SEGV_LOCATION();

	rdb_query(QUERY_HIGH, "SELECT COUNT(*) FROM %s WHERE type=\'chans\'",
	          CurrentTable);
	SET_SEGV_LOCATION();
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			retcode = atoi(*mysql_fetch_row(mysql_res));
		}
		else
		{
			retcode = -1;
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	return retcode;
}

/*************************************************************************/

int db_getlusers(int type)
{
	int retcode = 0;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!denora->do_sql)
	{
		return 0;
	}
	SET_SEGV_LOCATION();

	switch (type)
	{
		case LUSERS_USERS:
			rdb_query(QUERY_HIGH,
			          "SELECT COUNT(*) FROM %s WHERE mode_li=\'N\'",
			          UserTable);
			break;
		case LUSERS_USERSINV:
			rdb_query(QUERY_HIGH,
			          "SELECT COUNT(*) FROM %s WHERE mode_li=\'Y\'",
			          UserTable);
			break;
		case LUSERS_OPERS:
			rdb_query(QUERY_HIGH,
			          "SELECT COUNT(*) FROM %s WHERE mode_lo=\'Y\'",
			          UserTable);
			break;
		case LUSERS_CHAN:
			rdb_query(QUERY_HIGH, "SELECT COUNT(*) FROM %s", ChanTable);
			break;
		case LUSERS_SERV:
			rdb_query(QUERY_HIGH, "SELECT COUNT(*) FROM %s", ServerTable);
			break;
		case LUSERS_USERSGLOB:
			rdb_query(QUERY_HIGH, "SELECT COUNT(*) FROM %s", UserTable);
			break;
		case LUSERS_USERSMAX:
			rdb_query(QUERY_HIGH, "SELECT val FROM %s WHERE type='users'",
			          MaxValueTable);
			break;
	}
	SET_SEGV_LOCATION();
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			retcode = atoi(*mysql_fetch_row(mysql_res));
		}
		else
		{
			retcode = -1;
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	return retcode;
}

/*************************************************************************/

/* chan should be db_escape'd before call */
int db_getchannel(char *chan)
{
	int chanid = -1;
	Channel *c;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (!chan)
		return chanid;

	SET_SEGV_LOCATION();

	c = findchan(chan);
	if (c && c->sqlid > 0)
	{
		return c->sqlid;
	}

	if (!denora->do_sql)
	{
		return chanid;
	}

	rdb_query(QUERY_HIGH, "SELECT chanid FROM %s WHERE channel=\'%s\'",
	          ChanTable, chan);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			mysql_row = mysql_fetch_row(mysql_res);
			chanid = strtol(mysql_row[0], NULL, 10);
		}
		else
		{
			alog(LOG_NONEXISTANT, "channel not found ! %s", chan);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
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
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	MYSQL_ROW row;
#endif

	if (!denora->do_sql)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	rdb_query(QUERY_HIGH, "SELECT channel FROM %s WHERE chanid=%d",
	          ChanTable, chanid);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (!mysql_num_rows(mysql_res))
		{
			alog(LOG_NONEXISTANT, "channel not found ! %d", chanid);
		}
		else
		{
			row = mysql_fetch_row(mysql_res);
			res = rdb_escape(row[0]);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	return res;
}

/*************************************************************************/

/* chan should be db_escape'd before call */
int db_getchannel_users(char *chan)
{
	int res = 0;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return -1;
	}
	SET_SEGV_LOCATION();

	rdb_query(QUERY_HIGH,
	          "SELECT currentusers FROM %s WHERE channel=\'%s\'",
	          ChanTable, chan);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	SET_SEGV_LOCATION();
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res))
		{
			res = strtol(*mysql_fetch_row(mysql_res), NULL, 10);
		}
		else
		{
			alog(LOG_DEBUG,
			     "debug: unable to find the requested channel %s", chan);
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
	}
#endif
	return res;
}

/*************************************************************************/

/* chan should be db_escape'd before call */
/* chan is created if not exists */
int db_getchancreate(char *chan)
{
	int chanid = -1;
	int created = 0;
	int newcase = 0;
	Channel *c;
	char *channel;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
	MYSQL_ROW row;
#endif

	SET_SEGV_LOCATION();

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
		channel = rdb_escape(chan);
	}


	if (!denora->do_sql)
	{
		return chanid;
	}

	if (!c || c->sqlid < 1)
	{
		rdb_query(QUERY_HIGH, "SELECT chanid, channel FROM %s WHERE channel=\'%s\'",
		          ChanTable, channel);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_res)
		{
			if (mysql_num_rows(mysql_res))
			{
				row = mysql_fetch_row(mysql_res);
				alog(LOG_DEBUG, "debug: RESULT: %s %s", row[0], row[1]);
				chanid = atoi(row[0]);
				if (channel != row[1])
				{
					newcase = 1;
				}
				if (c && chanid > 0)
				{
					c->sqlid = chanid;
				}
			}
			mysql_free_result(mysql_res);
		}
#endif
		SET_SEGV_LOCATION();
		if (chanid < 1)
		{
			rdb_query(QUERY_HIGH, "INSERT INTO %s (channel) VALUES (\'%s\')",
			          ChanTable, channel);
			chanid = rdb_insertid();
			if (c && chanid > 0)
			{
				c->sqlid = chanid;	
			}
			created = 1;
		}
	}

	if (newcase)
	{
		if (!created && chanid != -1)
		{
			/* We update the channel name in case casing has changed */
			rdb_query(QUERY_LOW, "UPDATE %s SET channel=\'%s\' WHERE chanid=%d",
			          ChanTable, channel, chanid);
		}
		rdb_query(QUERY_LOW, "UPDATE %s SET chan=\'%s\' WHERE chan=\'%s\'",
		          CStatsTable, channel, channel);
		rdb_query(QUERY_LOW, "UPDATE %s SET chan=\'%s\' WHERE chan=\'%s\'",
		          UStatsTable, channel, channel);
	}
	SET_SEGV_LOCATION();
	free(channel);
	return chanid;
}

/*************************************************************************/

/* cleanup the server table, removing old entries */
void db_cleanserver()
{
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif
	int curtime = time(NULL);

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return;
	}

	if (ServerLastClean == -1)
	{
		ServerLastClean = curtime;
	}
	SET_SEGV_LOCATION();
	if (curtime > (ServerLastClean + ServerCleanFreq))
	{
		ServerLastClean = curtime;
		rdb_query
		(QUERY_HIGH,
		 "SELECT server FROM %s WHERE online=\'N\' AND lastsplit<FROM_UNIXTIME(%d)",
		 ServerTable, curtime - ServerCacheTime);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_num_rows(mysql_res))
		{
			mysql_free_result(mysql_res);
			rdb_query
			(QUERY_LOW,
			 "DELETE FROM %s WHERE online=\'N\' AND lastsplit<FROM_UNIXTIME(%d)",
			 ServerTable, curtime - ServerCacheTime);
		}
		else
		{
			if (mysql_res)
			{
				mysql_free_result(mysql_res);
			}
		}
#endif
		SET_SEGV_LOCATION();
	}
}

/*************************************************************************/

/* cleanup the user table, removing old entries */
void db_cleanuser()
{
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif
	int curtime = time(NULL);

	if (!denora->do_sql)
	{
		return;
	}

	if (UserLastClean == -1)
	{
		UserLastClean = curtime;
	}
	SET_SEGV_LOCATION();

	if (curtime > (UserLastClean + UserCleanFreq))
	{
		UserLastClean = curtime;
		rdb_query
		(QUERY_HIGH,
		 "SELECT nick FROM %s WHERE online=\'N\' AND lastquit<FROM_UNIXTIME(%d)",
		 UserTable, curtime - UserCacheTime);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_res)
		{
			if (mysql_num_rows(mysql_res))
			{
				rdb_query
				(QUERY_LOW,
				 "DELETE FROM %s WHERE online=\'N\' AND lastquit<FROM_UNIXTIME(%d)",
				 UserTable, curtime - UserCacheTime);
			}
			mysql_free_result(mysql_res);
		}
#endif
		SET_SEGV_LOCATION();
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
	SET_SEGV_LOCATION();

	if (stats->chans > stats->daily_chans)
	{
		stats->daily_chans++;
		stats->daily_chans_time = time(NULL);
	}
	rdb_query(QUERY_LOW,
	          "UPDATE %s SET val=%d, time=%ld WHERE type='chans'",
	          CurrentTable, stats->chans, time(NULL));
	if (diff1)
	{
		rdb_query
		(QUERY_LOW,
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

	SET_SEGV_LOCATION();

	if (!s)
	{
		return;
	}
	s->ss->currentusers++;
	s->ss->totalusersever++;
	stats->users++;
	stats->totalusersever++;

	SET_SEGV_LOCATION();
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
	SET_SEGV_LOCATION();
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
	SET_SEGV_LOCATION();
	if (s->ss->currentusers > s->ss->maxusers)
	{
		s->ss->maxusers = s->ss->currentusers;
		s->ss->maxusertime = time(NULL);
	}
	SET_SEGV_LOCATION();
	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET val=%d, time=%ld WHERE type='users'",
		          CurrentTable, stats->users, time(NULL));
		if (diff2)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET val=%d, time=%ld WHERE type='daily_users'",
			 CurrentTable, stats->daily_users,
			 stats->daily_users_time);
		}
		if (diff1)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='users'",
			 MaxValueTable, stats->users_max,
			 (long int) stats->users_max_time);
		}
		rdb_query
		(QUERY_LOW,
		 "UPDATE %s SET currentusers=%d, maxusers=%d, maxusertime=%d WHERE server='%s'",
		 ServerTable, s->ss->currentusers, s->ss->maxusers,
		 s->ss->maxusertime, s->name);
	}
}

/*************************************************************************/

void del_current_user(Server * s)
{
	SET_SEGV_LOCATION();
	if (!s)
	{
		return;
	}
	s->ss->currentusers--;
	stats->users--;
	SET_SEGV_LOCATION();

	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET val=%d, time=%ld WHERE type='users'",
		          CurrentTable, stats->users, time(NULL));
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET currentusers=%d WHERE server='%s'",
		          ServerTable, s->ss->currentusers, s->name);
	}
}

/*************************************************************************/

/* check if servs > servs_max */
void do_checkservsmax()
{
	int diff1 = 0;

	SET_SEGV_LOCATION();

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
	SET_SEGV_LOCATION();

	if (stats->servers > stats->daily_servers)
	{
		stats->daily_servers++;
		stats->daily_servers_time = time(NULL);
	}
	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET val=%d, time=%ld WHERE type='servers'",
		          CurrentTable, stats->servers, time(NULL));
		if (diff1)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='servers'",
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

	SET_SEGV_LOCATION();

	if (s)
	{
		s->ss->opers++;
	}
	SET_SEGV_LOCATION();

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

	SET_SEGV_LOCATION();

	if (stats->opers > stats->daily_opers)
	{
		stats->daily_opers++;
		stats->daily_opers_time = time(NULL);
	}
	if (!stats->daily_opers_time)
	{
		stats->daily_opers_time = time(NULL);
	}
	SET_SEGV_LOCATION();

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
		rdb_query
		(QUERY_LOW,
		 "UPDATE %s SET opers=%d, maxopers=%ld, maxopertime=%ld WHERE server='%s'",
		 ServerTable, s->ss->opers, s->ss->maxopers,
		 (long int) s->ss->maxopertime, s->name);

	}
	rdb_query(QUERY_LOW,
	          "UPDATE %s SET val=%d, time=%ld WHERE type='opers'",
	          CurrentTable, stats->opers, (long int) time(NULL));
	if (diff1)
	{
		rdb_query
		(QUERY_LOW,
		 "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='opers'",
		 MaxValueTable, stats->opers_max,
		 (long int) stats->opers_max_time);
	}
}

/*************************************************************************/

void del_oper_count(User * u)
{
	SET_SEGV_LOCATION();

	stats->opers--;
	if (u && u->server && u->server->ss)
	{
		u->server->ss->opers--;
		rdb_query
		(QUERY_LOW,
		 "UPDATE %s SET opers=%d, maxopers=%ld, maxopertime=%ld WHERE server='%s'",
		 ServerTable, u->server->ss->opers, u->server->ss->maxopers,
		 (long int) u->server->ss->maxopertime, u->server->name);
	}

	SET_SEGV_LOCATION();

	rdb_query(QUERY_LOW,
	          "UPDATE %s SET val=%d, time=%ld WHERE type='opers'",
	          CurrentTable, stats->opers, time(NULL));
}

/*************************************************************************/

int chans_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

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

	rdb_query(QUERY_HIGH,
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          ChanStatsTable, mytime.tm_mday, month, year);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res) > 0)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 ChanStatsTable, mytime.tm_hour, stats->chans,
			 mytime.tm_mday, month, year);
		}
		else
		{
			rdb_query
			(QUERY_LOW,
			 "INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 ChanStatsTable, mytime.tm_hour, mytime.tm_mday, month,
			 year, stats->chans);
		}
		mysql_free_result(mysql_res);
	}
	SET_SEGV_LOCATION();
#endif
	SET_SEGV_LOCATION();
	return MOD_CONT;
}

/*************************************************************************/

int servers_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

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

	rdb_query(QUERY_HIGH,
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          ServerStatsTable, mytime.tm_mday, month, year);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res) > 0)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 ServerStatsTable, mytime.tm_hour, stats->servers,
			 mytime.tm_mday, month, year);
		}
		else
		{
			rdb_query
			(QUERY_LOW,
			 "INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 ServerStatsTable, mytime.tm_hour, mytime.tm_mday, month,
			 year, stats->servers);
		}
		mysql_free_result(mysql_res);
	}
	SET_SEGV_LOCATION();
#endif
	SET_SEGV_LOCATION();
	return MOD_CONT;
}

/*************************************************************************/

int users_hourly(const char *name)
{
	static struct tm mytime;
	time_t ts;
	int year;
	int month;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

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

	rdb_query(QUERY_HIGH,
	          "SELECT id FROM %s WHERE day=%d and month=%d and year=%d",
	          StatsTable, mytime.tm_mday, month, year);
#ifdef USE_MYSQL
	mysql_res = mysql_store_result(mysql);
	if (mysql_res)
	{
		if (mysql_num_rows(mysql_res) > 0)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET time_%d=%d WHERE day=%d and month=%d and year=%d",
			 StatsTable, mytime.tm_hour, stats->users, mytime.tm_mday,
			 month, year);
		}
		else
		{
			rdb_query
			(QUERY_LOW,
			 "INSERT INTO %s (day, month, year, time_%d) VALUES (%d, %d, %d, %d)",
			 StatsTable, mytime.tm_hour, mytime.tm_mday, month, year,
			 stats->users);
		}
		mysql_free_result(mysql_res);
	}
	SET_SEGV_LOCATION();
#endif
	SET_SEGV_LOCATION();
	return MOD_CONT;
}
