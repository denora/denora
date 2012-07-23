
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

#ifdef USE_MYSQL

/*************************************************************************/

/* Database Global Variables */
MYSQL *mysql;                   /* Main MySQL Handler */
MYSQL *mysql_thread;		/* MySQL Handler for thread */
MYSQL_FIELD *mysql_fields;      /* MySQL Fields  */
MYSQL_ROW mysql_row;            /* MySQL Row     */

void db_mysql_error(int severity, const char *msg, int con);
char *db_mysql_hidepass(char *sql);

/*************************************************************************/

void db_mysql_error(int severity, const char *msg, int con)
{
	static char buf[512];

	SET_SEGV_LOCATION();

	if (mysql_error(con ? mysql_thread : mysql))
	{
		ircsnprintf(buf, sizeof(buf), "MySQL %s %s: %s", msg,
		            severity == SQL_WARNING ? "warning" : "error",
		            mysql_error(con ? mysql_thread : mysql));
	}
	else
	{
		ircsnprintf(buf, sizeof(buf), "MySQL %s %s", msg,
		            severity == SQL_WARNING ? "warning" : "error");
	}

	log_perror("%s", buf);

}

/*************************************************************************/

int db_mysql_init(int con)
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

	SET_SEGV_LOCATION();

	/* The following configuration options are required.
	 * If missing disable MySQL to avoid any problems.
	 */

	if ((denora->do_sql) && (!SqlName || !SqlUser))
	{
		denora->do_sql = 0;
		alog(LOG_ERROR, langstring(ALOG_SQL_NOTSET));
		return 0;
	}

	SET_SEGV_LOCATION();

	if (!db_mysql_open(con))
	{
		denora->do_sql = 0;
		return 0;
	}
	else
	{
		denora->do_sql = 1;
		alog(LOG_SQLDEBUG, "MySQL: server version %s.",
		     mysql_get_server_info(con ? mysql_thread : mysql));
	}
	SET_SEGV_LOCATION();

	return 1;
}

/*************************************************************************/

int db_mysql_open(int con)
{
	unsigned int seconds;
	int flags;

	/* If MySQL is disabled, return 0 */
	if (!denora->do_sql)
		return 0;

	SET_SEGV_LOCATION();

	if (con == 0)
	{
		mysql = mysql_init(NULL);
		if (!mysql)
		{
			db_mysql_error(SQL_WARNING, "Unable to create mysql object", con);
			return 0;
		}
	}
	else
	{
		mysql_thread = mysql_init(NULL);
		if (!mysql_thread)
		{
			db_mysql_error(SQL_WARNING, "Unable to create mysql_thread object", con);
			return 0;
		}
	}

	if (!SqlPort)
	{
		SqlPort = 3306;
	}

	if (!DisableMySQLOPT)
	{
		mysql_options(con ? mysql_thread : mysql, MYSQL_OPT_COMPRESS, 0);
#if MYSQL_VERSION_ID > 40102
		mysql_options(con ? mysql_thread : mysql, MYSQL_SET_CHARSET_NAME, "latin1");
		/* mysql_options(con ? mysql_thread : mysql, MYSQL_INIT_COMMAND, "SET NAMES 'utf8';"); */
#endif
		seconds = SQLPingFreq * 2;
		mysql_options(con ? mysql_thread : mysql, MYSQL_OPT_CONNECT_TIMEOUT, (char *) &seconds);
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
	        (con ? mysql_thread : mysql, SqlHost, SqlUser, (SqlPass ? SqlPass : ""), SqlName,
	         SqlPort, SqlSock ? SqlSock : NULL, flags)))
	{
		log_perror("MySQL Error: Cant connect to MySQL: %s\n",
		           mysql_error(con ? mysql_thread : mysql));
		if (con == 0)
			denora->do_sql = 0;
		else
			UseThreading = 0;
		return 0;
	}
	return 1;
}

/*************************************************************************/

int db_mysql_query(char *sql, int con)
{
	int result, lcv;
	int pingresult;
	int closesql = 0;

	if (!denora->do_sql)
	{
		return 0;
	}

	alog(LOG_SQLDEBUG, "sql debug: %s", db_mysql_hidepass(sql));

	pingresult = mysql_ping(con ? mysql_thread : mysql);
	if (!pingresult)
	{
		result = mysql_query(con ? mysql_thread : mysql, sql);
		alog(LOG_DEBUG, "[con %d/%s] %s", con, result == 0 ? "Accepted" : "Rejected", sql);
	}
	else
	{
		result = pingresult;
	}

	if (result)
	{
		switch (mysql_errno(con ? mysql_thread : mysql))
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
					     mysql_error(con ? mysql_thread : mysql));
					if (db_mysql_open(con))
					{
						alog(LOG_NORMAL, "MySQL connection reestablished");
						result = mysql_query(con ? mysql_thread : mysql, sql);
						return (result);
					}
					sleep(SqlRetryGap);
				}

				/* If we get here, we could not connect. */
				log_perror("Unable to reconnect to database: %s\n",
				           mysql_error(con ? mysql_thread : mysql));
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

	mysql_real_escape_string(mysql, quoted, sql, slen);
	return quoted;
}

/*************************************************************************/

char *db_mysql_hidepass(char *sql)
{
	int slen, pos, i;
	char *buf, *hidden;

	if (!sql)
	{
		return sstrdup("");
	}

	buf = strstr(sql, "MD5");
	if (!buf)
	{
		return sql;
	}

	buf = strstr(buf, ", ('");
	if (!buf)
	{
		return sql;
	}

	slen = strlen(sql);
	pos = (slen - strlen(buf)) + 4;

	free(buf);

	hidden = sstrdup(sql);
	for (i = pos; i < slen; i++)
	{
		if (hidden[i] != ')')
		{
			hidden[i] = 'x';
		}
		else
		{
			hidden[i - 1] = sql[i - 1];
			break;
		}
	}

	return hidden;
}

/*************************************************************************/

/* I don't like using res here, maybe we can pass it as a param? */
int db_mysql_close(int con)
{
	if (mysql && con == 0)
	{
		mysql_close(mysql);
#ifdef USE_THREADS
	}
	else if (mysql_thread)
	{
		mysql_close(mysql_thread);
#endif
	}
	return 1;
}

/*************************************************************************/

void dbMySQLPrepareForQuery(int con)
{
	MYSQL_RES *mysql_res;
	mysql_res = mysql_use_result(con ? mysql_thread : mysql);
	if (mysql_res)
	{
		mysql_free_result(mysql_res);
		if (con)
			alog(LOG_DEBUG, "Result from the threaded query");
	}
	else if (con)
		alog(LOG_DEBUG, "No result from the threaded query");
}

#endif
