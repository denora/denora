#include "denora.h"

SQLres *sqlres;
SQLCon *sqlcon;

typedef struct sql_mod_
{
	void (*freeresult)(SQLres *res);
	char *(*errmsg)(SQLCon *con);
	int  (*close)(SQLCon *con);
	int  (*open)(void);
	int  (*init)(void);
	int  (*query)(SQLCon *con, char *buf);
	SQLres *(*result)(SQLCon *con);
	int  (*numrows)(SQLres *res);
	char *(*escape) (char *ch);
	char **(*fetch_row)(SQLres *res);
	int (*check_table)(char *table);
	int (*ping)(SQLCon *con);
	int (*insertid)(SQLCon *con);
} SQLMod;


SQLMod sqlmod;


void initSQLMod(void)
{
	sqlmod.freeresult = NULL;
	sqlmod.errmsg = NULL;
	sqlmod.close = NULL;
	sqlmod.open = NULL;
	sqlmod.init = NULL;
	sqlmod.query = NULL;
	sqlmod.result = NULL;
    sqlmod.numrows = NULL;
	sqlmod.escape = NULL;
}


/*************************************************************************/

void SQLModSet_FreeResult(void (*func) (SQLres *res))
{
	sqlmod.freeresult = func;
}

/*************************************************************************/

void SQLModSet_init(int (*func) (void))
{
	sqlmod.init = func;
}


/*************************************************************************/

void SQLModSet_Close(int (*func) (SQLCon *con))
{
	sqlmod.close = func;
}

/*************************************************************************/

void SQLFreeResult(SQLres *res)
{
	if (res)
	{
			sqlmod.freeresult(res);
	}
}

/*************************************************************************/

void SQLModSet_errmsg(char *(*func) (SQLCon *con))
{
	sqlmod.errmsg = func;
}


void SQLModSet_escape(char *(*func) (char *ch))
{
	sqlmod.escape = func;
}

void SQLModSet_open(int (*func) (void))
{
	sqlmod.open = func;
}

/*************************************************************************/

char *SQLErrMsg(SQLCon *con)
{
	return sqlmod.errmsg(con);
}

/*************************************************************************/

void SQL_DisplayError(SQLCon *con, int severity, const char *msg)
{
	static char buf[512];

	if (SQLErrMsg(con))
	{
		ircsnprintf(buf, sizeof(buf), "SQL %s %s: %s", msg,
		            severity == SQL_WARNING ? "warning" : "error",
		            SQLErrMsg(con));
	}
	else
	{
		ircsnprintf(buf, sizeof(buf), "SQL %s %s", msg,
		            severity == SQL_WARNING ? "warning" : "error");
	}
	log_perror("%s", buf);
}

/*************************************************************************/

int sql_close(SQLCon *con)
{
	int res;
	res = sqlmod.close(con);
	return res;
}

/* escape the string */
char *sql_escape(char *ch)
{
	char *ret = NULL;
	if (ch)
	{
		return sqlmod.escape(ch);
	}

	return ret;
}


char *sql_hidepass(char *sql)
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
		return sstrdup(sql);
	}

	buf = strstr(buf, ", ('");
	if (!buf)
	{
		return sstrdup(sql);
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

int sql_init()
{
	int res = 0;
	res = sqlmod.init();
	return res;
}



/*************************************************************************/

int sql_direct_query(SQLCon *con, char *query)
{
	int nptrs, j;
	void *buffer[100];
	char **strings;
	

	if (!denora->do_sql)
	{
		return -1;
	}
	nptrs = backtrace(buffer, 6);
	strings = backtrace_symbols(buffer, nptrs);
	if (strings != NULL) {
	for (j = 2; j < nptrs; j++)
			alog(LOG_DEBUG, "[bt %d/%d] %s", j, nptrs, strings[j]);
			free(strings);
	}
	sqlmod.query(con, query);
	return 0;
}

/*************************************************************************/

/* send an SQL query to the database */
int sql_query(const char *fmt, ...)
{
	va_list args;
	char buf[NET_BUFSIZE];      /* should be enough for long queries (don't underestimate :o)) */
	int res;

	if (!denora->do_sql)
	{
		return 0;
	}

	if (BadPtr(fmt))
	{
		return 0;
	}

	va_start(args, fmt);
	ircvsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	res = sql_direct_query(sqlcon, buf);
	return res;
}


/*************************************************************************/

int sql_clear_table(char *table)
{
	static char buf[1024];

	if (KeepUserTable && !stricmp(table, UserTable))
	{
		ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
		            table);
	}
	else if (KeepServerTable && !stricmp(table, ServerTable))
	{
		ircsnprintf(buf, sizeof(buf), "UPDATE %s SET online='N'",
		            table);
	}
	else
	{
		ircsnprintf(buf, sizeof(buf), "TRUNCATE TABLE %s", table);
	}
	return sql_direct_query(sqlcon, buf);
}


SQLres *sql_set_result(SQLCon *con)
{
	return sqlmod.result(con);
}

int sql_num_rows(SQLres *res)
{
	return sqlmod.numrows(res);
}

char **sql_fetch_row(SQLres *res)
{
	return sqlmod.fetch_row(res);
}


void sql_free_result(SQLres *res)
{
		sqlmod.freeresult(res);
}


int sql_check_table(char *table)
{
	return sqlmod.check_table(table);
}

int sql_ping(SQLCon *con)
{
	return sqlmod.ping(con);
}

int sql_insertid(SQLCon *con)
{
	return sqlmod.insertid(con);
}