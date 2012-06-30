
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

#ifdef USE_POSTGRE

/*************************************************************************/

/* Database Global Variables */
PGconn *postgre;                /* Postgre Handler */
PGresult *postgre_res;          /* Postgre Result  */

/*************************************************************************/

void db_postgre_error(int severity, char *msg)
{
	static char buf[512];

	SET_SEGV_LOCATION();

	if (PQerrorMessage(postgre))
	{
		ircsnprintf(buf, sizeof(buf), "PostgreSQL %s %s: %s", msg,
		            severity == SQL_WARNING ? "warning" : "error",
		            PQerrorMessage(postgre));
	}
	else
	{
		ircsnprintf(buf, sizeof(buf), "PostgreSQL %s %s", msg,
		            severity == SQL_WARNING ? "warning" : "error");
	}
	log_perror(buf);
}

/*************************************************************************/

int db_postgre_init()
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
		alog(LOG_NORMAL, langstring(ALOG_SQL_ENABLED), "PostgreSQL");
		alog(LOG_SQLDEBUG, "MySQL: client version %s.",
		     mysql_get_client_info());
	}

	SET_SEGV_LOCATION();

	if ((denora->do_sql) && (!SqlName || !SqlUser))
	{
		denora->do_sql = 0;
		alog(LOG_ERROR, langstring(ALOG_SQL_NOTSET));
		return 0;
	}

	SET_SEGV_LOCATION();

	if (!db_postgre_open())
	{
		denora->do_sql = 0;
		return 0;
	}
	SET_SEGV_LOCATION();

	return 1;
}

/*************************************************************************/

int db_postgre_open()
{
	char buf[BUFSIZE];

	if (!denora->do_sql)
	{
		return 0;
	}

	SET_SEGV_LOCATION();

	if (!SqlPort)
	{
		SqlPort = 3306;
	}

	SET_SEGV_LOCATION();

	ircsnprintf(buf, BUFSIZE - 1, "host=%s user=%s %s%s dbname=%s port=%s",
	            SqlHost, SqlUser, (SqlPass ? "pass=" : ""),
	            (SqlPass ? SqlPass : ""), SqlName, SqlPort);

	postgre = PQconnectdb(buf);

	return 1;

}

/*************************************************************************/

int db_postgre_query(char *sql)
{
	PGresult *result;
	int lcv;

	if (!denora->do_sql)
	{
		return -1;
	}

	alog(LOG_SQLDEBUG, "sql debug: %s", sql);

	result = PQexec(postgre, sql);

	if (result)
	{
		alog(LOG_NORMAL, "Error?? %s", PQresultErrorMessage(result));
		return 1;
	}

	return (0);

}

/*************************************************************************/

char *db_postgre_quote(char *sql)
{
	int slen;
	char *quoted;

	if (!sql)
	{
		return sstrdup("");
	}

	slen = strlen(sql);
	quoted = malloc((1 + (slen * 2)) * sizeof(char));
	PQescapeString(quoted, sql, slen);
	return quoted;
}

/*************************************************************************/

/* I don't like using res here, maybe we can pass it as a param? */
int db_postgre_close()
{
	PQfinish(postgre);
	return 1;
}

/*************************************************************************/

#endif
