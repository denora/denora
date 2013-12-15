#include "denora.h"
#include "postgre.h"

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

void moduleAddSqlFunctions()
{
	SQLModSet_errmsg(db_postgreErrMsg);
	SQLModSet_FreeResult(db_postgre_freeresult);
	SQLModSet_Close(db_postgreClose);
	SQLModSet_escape(db_postgre_quote);
	SQLModSet_Open(db_postgre_open);
	SQLModSet_init(db_postgre_init);
	SQLModSet_query(db_postgre_query);


}

/*************************************************************************/

void db_postgreFreeResult(SQLres *res)
{
		PQclear(res->pgres);
}

/*************************************************************************/


char *db_postgreErrMsg(SQLCon *con)
{
	return PQerrorMessage(con->pgconn);
}

/*************************************************************************/

int db_postgreClose(SQLCon *con)
{
	PQfinish(con->pgconn);
	return 1;
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

	quoted = PQescapeIdentifier(sqlcon->pgconn, sql, slen);
	return quoted;
}


int db_postgre_open(void)
{
	char sql_port[5];


	if (!denora->do_sql)
		return 0;

	SET_SEGV_LOCATION();

	if (!SqlPort)
	{
		SqlPort = 5433;
	}

	/* Postgre expects the port to be a char */
	sprintf(sql_port, "%d", SqlPort);

	sqlcon->pgconn = PQsetdbLogin(SqlHost, sql_port, NULL, NULL, SqlName,  SqlUser, (SqlPass ? SqlPass : ""));

	if (PQstatus(sqlcon->pgconn) != CONNECTION_OK)
	{
		log_perror("PostgreSQL Error: Cant connect to SQL: %s\n",
		           PQerrorMessage(sqlcon->pgconn));
			denora->do_sql = 0;
		return 0;
	}

	return 1;
}


int db_postgre_init(void)
{
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
	}

	if ((denora->do_sql) && (!SqlName || !SqlUser))
	{
		denora->do_sql = 0;
		alog(LOG_ERROR, langstring(ALOG_SQL_NOTSET));
		return 0;
	}

	if (!db_postgre_open())
	{
		denora->do_sql = 0;
		return 0;
	}
	else
	{
		denora->do_sql = 1;
	}

	return 1;
}


int db_postgre_query(SQLCon *con, char *sql)
{
	char *qpass;
	PGresult * result;
	int resstatus;

	if (!denora->do_sql)
	{
		return 0;
	}

	qpass = sql_hidepass(sql);
	alog(LOG_SQLDEBUG, "sql debug: %s", qpass);
	free(qpass);

	result = PQexec(sqlcon->pgconn, sql);

	if (result)
	{
		resstatus = PQresultStatus(result);
		if (resstatus == PGRES_COMMAND_OK || resstatus == PGRES_TUPLES_OK)
		{
			alog(LOG_DEBUG, "[SQL: %s] %s", "Accepted", sql);
			PQclear(result);
			return 1;
		}
		else if (resstatus == PGRES_COPY_OUT || resstatus == PGRES_COPY_IN)
		{
			alog(LOG_DEBUG, "[SQL: %s] %s", "Accepted", sql);
			PQclear(result);
			return 1;
		}
		else if (resstatus == PGRES_EMPTY_QUERY || resstatus == PGRES_BAD_RESPONSE || resstatus == PGRES_FATAL_ERROR)
		{
			alog(LOG_DEBUG, "[SQL: %s] %s", "Rejected/Error", sql);
			PQclear(result);
			return 0;
		}
		else if (resstatus == PGRES_NONFATAL_ERROR)
		{
			alog(LOG_DEBUG, "[SQL: %s] %s", "Rejected/Nonfatal", sql);
			PQclear(result);
			return 1;
		}
	}
	return 1;
}

SQLres *db_postgre_setresult(SQLCon *con)
{
	sqlres->pgres = PQgetResult(con->pgconn);
	return sqlres;
}

int db_postgre_num_rows(SQLres *res)
{
	return PQntuples(res->pgres);
}

void db_postgre_freeresult(SQLres *res)
{
		PQclear(res->pgres);
}

int postgre_check_table(char *table)
{
	int res = 1;
	PGresult *pgres;

	sql_query("SHOW TABLES LIKE '%s';", table);
	pgres = PQgetResult(sqlcon->pgconn);

	if (pgres)
	{
		if (PQntuples(pgres) == 0)
		{
			res = 0;
		}
		PQclear(pgres);
	}
	else
	{
		res = 0;
	}
	if (!pgres)
	{
		alog(LOG_DEBUG, "Table %s was not found", table);
	}
	return res;
}

/* Yeap Postgre doesn't have this one either */
char **PostgreSQLFetchRow(SQLres *res)
{
	int i;
	char **SQLDataArray;
	int pgsql_row;
	int num_fields;

	SQLDataArray = DenoraCallocArray(1);
	pgsql_row = PQntuples(res->pgres);
	for (i = 0, num_fields = PQnfields(res->pgres); i < num_fields; i++) 
	{
			SQLDataArray[i] = StringDup(PQgetvalue(res->pgres, pgsql_row, i));
	}
	if (SQLDataArray[0])
	{
		return SQLDataArray;
	}
	return NULL;
}