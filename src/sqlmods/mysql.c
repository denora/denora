#include "denora.h"
#include "mysql.h"


/* Database Global Variables */
MYSQL *mysql;                   /* Main MySQL Handler */
MYSQL *mysql_thread;		/* MySQL Handler for thread */
MYSQL_FIELD *mysql_fields;      /* MySQL Fields  */
MYSQL_ROW mysql_row;            /* MySQL Row     */

/*************************************************************************/

int DenoraInit(int argc, char **argv)
{
	if (denora->protocoldebug)
	{
		protocol_debug(NULL, argc, argv);
	}
	moduleAddAuthor("Denora");
	moduleAddVersion("2.0");
	moduleSetType(SQLMOD);

	moduleAddIRCDCmds();
	moduleAddIRCDMsgs();
	return MOD_CONT;
}

/*************************************************************************/

void moduleAddSqlFunctions()
{
	SQLModSet_errmsg(db_MySQLErrMsg);
	SQLModSet_FreeResult(db_MySQLFreeResult);
	SQLModSet_Close(db_mysql_close);
	SQLModSet_escape(db_mysql_quote);
	SQLModSet_Open(db_mysql_open);
	SQLModSet_init(db_mysql_init);
	SQLModSet_query(db_mysql_query);
}

/*************************************************************************/

void db_MySQLFreeResult(SQLres *res)
{
		mysql_free_result(res->myres);
}

/*************************************************************************/

char *db_MySQLErrMsg(SQLCon *con)
{
		return (char *) mysql_error(con->mycon);
}

/*************************************************************************/

int db_mysql_close(SQLCon *con)
{
	mysql_close(con->mycon);
	return 1;
}

/*************************************************************************/

char *db_mysql_quote(char *sql)
{
	int slen;
	char *quoted;

	if (!sql)
	{
		return sstrdup("");
	}

	slen = strlen(sql);
	quoted = malloc((1 + (slen * 2)) * sizeof(char));

	mysql_real_escape_string(sqlcon->mycon, quoted, sql, slen);
	return quoted;
}


int db_mysql_open(void)
{
	unsigned int seconds;
	int flags;

	/* If MySQL is disabled, return 0 */
	if (!denora->do_sql)
		return 0;

	SET_SEGV_LOCATION();

	sqlcon->mycon = mysql_init(NULL);
	if (!mysql)
	{
		db_mysql_error(SQL_WARNING, "Unable to create mysql object", sqlcon->mycon);
		return 0;
	}
	if (!SqlPort)
	{
		SqlPort = 3306;
	}

	if (!DisableMySQLOPT)
	{
		mysql_options(sqlcon->mycon, MYSQL_OPT_COMPRESS, 0);
#if MYSQL_VERSION_ID > 40102
		mysql_options(sqlcon->mycon, MYSQL_SET_CHARSET_NAME, "latin1");
#endif
		seconds = SQLPingFreq * 2;
		mysql_options(sqlcon->mycon, MYSQL_OPT_CONNECT_TIMEOUT, (char *) &seconds);
	}

	SET_SEGV_LOCATION();

	if (DisableMySQLOPT)
	{
		flags = 0;
	}
	else
	{
#ifdef CLIENT_IGNORE_SIGPIPE
		flags =
		    CLIENT_COMPRESS | CLIENT_IGNORE_SIGPIPE | CLIENT_INTERACTIVE;
		alog(LOG_DEBUG, "MySQL Client Flag for IGNORE SIGPIPE is set on");
#else
		flags = CLIENT_COMPRESS | CLIENT_INTERACTIVE;
		alog(LOG_DEBUG,
		     "MySQL Client Flag for IGNORE SIGPIPE is set unknown unablet to set");
#endif
	}

	if ((!mysql_real_connect
	        (sqlcon->mycon, SqlHost, SqlUser, (SqlPass ? SqlPass : ""), SqlName,
	         SqlPort, SqlSock ? SqlSock : NULL, flags)))
	{
		log_perror("MySQL Error: Cant connect to MySQL: %s\n",
		           mysql_error(sqlcon->mycon));
		denora->do_sql = 0;
		return 0;
	}
	return 1;
}


/*************************************************************************/

int db_mysql_init(void)
{
	SET_SEGV_LOCATION();

	/* If the host is not defined, assume we don't want MySQL */
	if (!SqlHost)
	{
		denora->do_sql = 0;
		alog(LOG_NORMAL, langstring(ALOG_SQL_DISABLED));
		return 0;
	}
	else
	{
		denora->do_sql = 1;
		alog(LOG_NORMAL, langstring(ALOG_SQL_ENABLED), "MySQL");
		alog(LOG_SQLDEBUG, "MySQL: client version %s.",
		     mysql_get_client_info());
	}

	/* The following configuration options are required.
	 * If missing disable MySQL to avoid any problems.
	 */

	if ((denora->do_sql) && (!SqlName || !SqlUser))
	{
		denora->do_sql = 0;
		alog(LOG_ERROR, langstring(ALOG_SQL_NOTSET));
		return 0;
	}


	if (!db_mysql_open())
	{
		denora->do_sql = 0;
		return 0;
	}
	else
	{
		denora->do_sql = 1;
		alog(LOG_SQLDEBUG, "MySQL: server version %s.",
		     mysql_get_server_info(sqlcon->mycon));
	}

	return 1;
}

int db_mysql_query(char *sql, SQLCon *con)
{
	int result, lcv;
	int pingresult;
	int closesql = 0;
	char *qpass;



	if (!denora->do_sql)
	{
		return 0;
	}

	qpass = sql_hidepass(sql);
	alog(LOG_SQLDEBUG, "sql debug: %s", qpass);
	free(qpass);

	pingresult = mysql_ping(con->mycon);
	if (!pingresult)
	{
		result = mysql_query(con->mycon, sql);
		if (UseThreading)
		{
			alog(LOG_DEBUG, "[con %d/%s] %s", con, result == 0 ? "Accepted" : "Rejected", sql);
		}
	}
	else
	{
		result = pingresult;
	}

	if (result)
	{
		switch (mysql_errno(con->mycon))
		{
			case CR_SERVER_GONE_ERROR:
			case CR_SERVER_LOST:
			case CR_CONN_HOST_ERROR:
			case CR_IPSOCK_ERROR:
			case CR_CONNECTION_ERROR:
			case CR_UNKNOWN_ERROR:
				for (lcv = 0; lcv < SqlRetries; lcv++)
				{
					alog(LOG_NORMAL, "%s, trying to reconnect...",
					     mysql_error(con->mycon));
					if (db_mysql_open())
					{
						alog(LOG_NORMAL, "MySQL connection reestablished");
						result = mysql_query(con->mycon, sql);
						return (result);
					}
					sleep(SqlRetryGap);
				}

				/* If we get here, we could not connect. */
				log_perror("Unable to reconnect to database: %s\n",
				           mysql_error(con->mycon));
				db_mysql_error(SQL_ERROR, "Connect", con);
				alog(LOG_ERROR, "Disabling MYSQL due to problem with server");
				denora->do_sql = 0;
				SQLDisableDueServerLost = 1;
				/* Never reached. */
				break;
			case CR_COMMANDS_OUT_OF_SYNC:
				db_mysql_error(SQL_ERROR, "Commands out of sync", con);
				break;
			case ER_PARSE_ERROR:
				db_mysql_error(SQL_ERROR, "Parser error", con);
				break;
			case ER_WRONG_VALUE_COUNT_ON_ROW:
				db_mysql_error(SQL_ERROR, "Query error", con);
				break;
			case ER_BAD_FIELD_ERROR:
				db_mysql_error(SQL_ERROR, "Unknown field", con);
				break;
			case ER_NO_SUCH_TABLE:
				db_mysql_error(SQL_ERROR, "Table Doesn't exist", con);
				break;
			case ER_DUP_FIELDNAME:
				db_mysql_error(SQL_ERROR, "Duplicate fieldname", con);
				break;
			case ER_DUP_ENTRY:
				db_mysql_error(SQL_ERROR, "Duplicate entry", con);
				alog(LOG_ERROR, "MYSQL query %s", sql);
				break;
			case ER_BAD_NULL_ERROR:
				db_mysql_error(SQL_ERROR, "Column can not be NULL", con);
				break;
			case ER_USER_LIMIT_REACHED:
				db_mysql_error(SQL_ERROR, "Too Many Connections, disabling mysql", con);
				closesql = 1;
				break;
			case ER_GET_ERRNO:
				db_mysql_error(SQL_ERROR, "Mysql returned errno code, disabling mysql", con);
				closesql = 1;
				break;
			case ER_WARN_DATA_OUT_OF_RANGE:
				alog(LOG_ERROR, "MYSQL reported Error Code %d", mysql_errno(con ? mysql_thread : mysql));
				alog(LOG_ERROR, "MYSQL said %s", mysql_error(con ? mysql_thread : mysql));
				alog(LOG_ERROR, "The field is to small, attempt to change the field from INT TO BIGINT");
				alog(LOG_ERROR, "Until this is done Denora can not update this table");
				alog(LOG_ERROR, "Report this to Denora Developers");
				break;
			default:
				alog(LOG_ERROR, "MYSQL reported Error Code %d", mysql_errno(con ? mysql_thread : mysql));
				alog(LOG_ERROR, "MYSQL said %s", mysql_error(con ? mysql_thread : mysql));
				alog(LOG_ERROR, "Report this to Denora Developers");
				alog(LOG_ERROR, "Disabling MYSQL due to this error");
				closesql = 1;
		}
	}

	if (closesql == 1)
	{
		if (con)
		{
			UseThreading = 0;
		}
		denora->do_sql = 0;
		db_mysql_close(con);
		SQLDisableDueServerLost = 1;
		return 0;
	}

	return 1;
}

SQLres *db_mysql_setresult(SQLCon *con)
{
	sqlres->myres = mysql_store_result(con->mycon);
	return sqlres;
}

int db_mysql_num_rows(SQLres *res)
{
	return mysql_num_rows(res->myres);
}

void db_mysql_freeresult(SQLres *res)
{
		mysql_free_result(res->myres);
}

int mysql_check_table(char *table)
{
	int res = 1;
	MYSQL_RES *my_res;

	sql_query("SHOW TABLES LIKE '%s';", table);
	my_res = mysql_store_result(sqlcon->mycon);

	if (my_res)
	{
		if (mysql_num_rows(my_res) == 0)
		{
			res = 0;
		}
		mysql_free_result(my_res);
	}
	else
	{
		res = 0;
	}
	if (!res)
	{
		alog(LOG_DEBUG, "Table %s was not found", table);
	}
	return res;
}


/*************************************************************************/

int mysql_insertid()
{
	return mysql_insert_id(sqlcon->mycon);
}