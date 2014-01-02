/* Sqlite3 wrapper
 *
 * A quick way for use to handle doing sqlite3 functions
 *
 * (c) 2014 Denora Dev Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 */
#include "denora.h"

/*************************************************************************/


sqlite3 *DenoraOpenSQL(char *dbname)
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

	sqlite3_initialize();

   /* Open database */
   rc = sqlite3_open(dbname, &db);
   if(rc)
   {
	return NULL;

   }
   return db;

}


/*************************************************************************/

/* send an SQL query to the database */
int DenoraExecQuerySQL(sqlite3 *db, const char *fmt, ...)
{
	va_list args;
	char *buf;
	int res;

	va_start(args, fmt);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, fmt, args);
	va_end(args);

	res = DenoraExecQueryDirectSQL(db, buf, NULL);
	return res;
}

/*************************************************************************/

int DenoraSQLQuery(char *dbname, const char *fmt, ...)
{
	va_list args;
	char *buf;
	int res;
	sqlite3 *db;

	va_start(args, fmt);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, fmt, args);
	va_end(args);


	db = DenoraOpenSQL(dbname);
	res = DenoraExecQueryDirectSQL(db, buf, NULL);
	DenoraCloseSQl(db);

	return res;
}


char *DenoraSQLGetField(char *dbname, const char *fmt, ...)
{
	va_list args;
	char *buf;
	sqlite3_stmt *stmt;
	char **sdata;
	sqlite3 *db;
	char *temp;

	va_start(args, fmt);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, fmt, args);
	va_end(args);

	db = DenoraOpenSQL(dbname);
	stmt = DenoraPrepareQuery(db, buf);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);

	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	temp = sstrdup(sdata[0]);
	free(sdata[0]);
	free(sdata);
	return temp;
}

int DenoraSQLGetFieldInt(char *dbname, char *table, char *what, char *wherewhat, char *whereeqaul)
{
	char *buf;
	sqlite3_stmt *stmt;
	char **sdata;
	sqlite3 *db;
	int temp;

	fmt = "SELECT %s FROM %s WHERE %q=%q";

	sqlite3_snprintf(NET_BUFSIZE, buf, fmt, what, table, wherewhat, whereequal);

	db = DenoraOpenSQL(dbname);
	stmt = DenoraPrepareQuery(db, buf);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	temp = atoi(sdata[0]);
	free(sdata[0]);
	free(sdata);
	return temp;
}


int DenoraSQLGetStatsInt(char *dbname, char *table, char *what)
{
	char *buf;
	sqlite3_stmt *stmt;
	char **sdata;
	sqlite3 *db;
	int temp;

	fmt = "SELECT type FROM %s WHERE type=%q";

	sqlite3_snprintf(NET_BUFSIZE, buf, fmt, table, what);

	db = DenoraOpenSQL(dbname);
	stmt = DenoraPrepareQuery(db, buf);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	temp = atoi(sdata[0]);
	free(sdata[0]);
	free(sdata);
	return temp;
}

/*************************************************************************/

int DenoraSQLUpdateChar(char *dbname, char *var, char *data)
{
	char *buf;
	int res;
	sqlite3 *db;

	const char *fmt = "UPDATE %s SET %s=%q";

	sqlite3_snprintf(NET_BUFSIZE, buf, fmt, dbname, var, data);

	db = DenoraOpenSQL(dbname);
	res = DenoraExecQueryDirectSQL(db, buf, NULL);
	DenoraCloseSQl(db);

	return res;
}


/*************************************************************************/

int DenoraSQLUpdateInt(char *dbname, char *table, char *var, uint32 data, char *what, char *equals)
{
	char *buf;
	int res;
	sqlite3 *db;

	const char *fmt = "UPDATE %s SET %s=%ld WHERE %q=%q";

	sqlite3_snprintf(NET_BUFSIZE, buf, fmt, table, var, data, what, equals);

	db = DenoraOpenSQL(dbname);
	res = DenoraExecQueryDirectSQL(db, buf, NULL);
	DenoraCloseSQl(db);

	return res;
}

/*************************************************************************/

int DenoraSQLUpdateStatsFull(char *dbname, char *table, char *var, uint32 data, char *what, char *equals)
{
	char *buf;
	int res;
	sqlite3 *db;

	const char *fmt = "UPDATE %s SET %s=%ld, time=%ld  WHERE %q=%q";

	sqlite3_snprintf(NET_BUFSIZE, buf, fmt, table, var, data, time(NULL), what, equals);

	db = DenoraOpenSQL(dbname);
	res = DenoraExecQueryDirectSQL(db, buf, NULL);
	DenoraCloseSQl(db);

	return res;
}


/*************************************************************************/

int DenoraSQLUpdateStatsInt(char *dbname, char *table, char *var, int updown)
{
	int value;

	value = DenoraSQLGetStatsInt(dbname, table, var);
	if (updown == 1)
	{
		value++;
	}
	else
	{
		value--;
	}
	DenoraSQLUpdateStatsFull(dbname, table, var, value, "type", var);
	return 1;
}


/*************************************************************************/


int DenoraExecQueryDirectSQL(sqlite3 *db, const char *querystring, int callback(void *NotUsed, int argc, char **argv, char **azColName))
{
	char *zErrMsg = 0;
	int rc;

	/* Execute SQL statement */
	rc = sqlite3_exec(db, querystring, NULL, 0, &zErrMsg);
	
	if (denora->do_sql)
	{
		
		sql_query(querystring);
	}
	
	
	if (rc != SQLITE_OK)
	{
		DenoraLib_SetLastError(rc, zErrMsg);
		sqlite3_free(zErrMsg);
		return rc;
	}
	else
	{
	      return rc;
   	}
}

/*************************************************************************/

void DenoraCloseSQl(sqlite3 *db)
{
	if (db) 
	{
		sqlite3_close(NULL);
	}
	sqlite3_shutdown();
}

/*************************************************************************/

sqlite3_stmt *DenoraPrepareQuery(sqlite3 *db, const char *fmt, ...)
{
	va_list args;
	char *buf;
	int res;
	sqlite3_stmt * stmt;

	va_start(args, fmt);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, fmt, args);
	va_end(args);

	res = sqlite3_prepare(db, buf, sizeof(buf) + 1, &stmt, NULL);
	if (res == SQLITE_OK) 
	{
	 	return stmt;
	} 
	else 
	{
		DenoraLib_SetLastError(res, sqlite3_errmsg(db));
		return NULL;
	}
}



sqlite3_stmt *DenoraPrepareStmt(sqlite3 *db, char *buf)
{
	int res;
	sqlite3_stmt * stmt;

	res = sqlite3_prepare(db, buf, sizeof(buf) + 1, &stmt, NULL);
	if (res == SQLITE_OK) 
	{
	 	return stmt;
	} 
	else 
	{
		DenoraLib_SetLastError(res, sqlite3_errmsg(db));
		return NULL;
	}
}

/*************************************************************************/

char *DenoraReturnSQliteValue(sqlite3_stmt *stmt, int column)
{
	char *data;

	switch (sqlite3_column_type(stmt, column)) {
		case SQLITE_INTEGER:
			data = (char*)sqlite3_column_text(stmt, column);
			break;
		case SQLITE_FLOAT:
			data = (char*)sqlite3_column_text(stmt, column);
			break;
		case SQLITE_NULL:
			data = NULL;
			break;
		case SQLITE3_TEXT:
			data = (char*)sqlite3_column_text(stmt, column);
			break;

		case SQLITE_BLOB:
		default:
			data = (char*)sqlite3_column_blob(stmt, column);
	}
	return data;
}

/*************************************************************************/

char ***DenoraSQLFetchArray(sqlite3 *db, char *table, sqlite3_stmt* stmt, int type)
{
	int ret;
	int i, x;
	int arraysize;
	char ***SQLDataArray;
	char *data;
	int rows, cols;

	cols = sqlite3_data_count(stmt);
	rows = DenoraSQLGetNumRows(db, table);

	SQLDataArray = DenoraCallocArray2D(rows, cols);

	for (x = 0; x <= rows; x++)
	{
		ret = sqlite3_step(stmt);
		switch (ret) {
			case SQLITE_ROW:
				for (i = 0; i < sqlite3_data_count(stmt); i++) 
				{
					data = DenoraReturnSQliteValue(stmt, i);
					SQLDataArray[x][i] = StringDup(data);
				}
				break;
			case SQLITE_DONE:
				break;
		}
		
	}
	return SQLDataArray;
}

/*************************************************************************/

char **DenoraSQLFetchRow(sqlite3_stmt *stmt, int type)
{
	int ret;
	int i, x;
	int arraysize;
	char **SQLDataArray;
	char *data;
	int count;

	ret = sqlite3_step(stmt);
	count = sqlite3_data_count(stmt);

	if (count)
	{
		SQLDataArray = DenoraCallocArray(1);
		switch (ret) {
			case SQLITE_ROW:
				for (i = 0; i < sqlite3_data_count(stmt); i++) 
				{
					data = DenoraReturnSQliteValue(stmt, i);
					SQLDataArray[i] = StringDup(data);
				}
				break;
			case SQLITE_DONE:
				break;
		}
		return SQLDataArray;
	}
	return NULL;
}



/*************************************************************************/

/* 
   DenoraSQLGetNumRows(sqlite3 *db, const char *table)

   @param:  db    : pointer to sqlite database stream
   @param:  table : the table which to get the row count of
   @return: int   : returns a integer with the value

   SQLite3 API lacks this functionality, so we have to use a SQL
   statment to get the COUNT(*) and the result is stored in the
   first column of the returned data. We check that sqlite3_step
   returns SQLITE_ROW, and get the column_int().

   Finally we have to clena up after ourselves by calling for
   finalize, this will free the memory set by DenoraPrepareQuery()

*/
int DenoraSQLGetNumRows(sqlite3 *db, const char *table)
{
	int StepReturn;
	int result = 0;
	sqlite3_stmt *stmt;

	stmt = DenoraPrepareQuery(db, "SELECT COUNT(*) from %s", table);
	StepReturn = sqlite3_step(stmt);
	if (StepReturn == SQLITE_ROW)
	{
		result = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return result;
}


int DenoraSQLGetNumRowsFromQuery(char *dbname, const char *fmt, ...)
{
	int StepReturn;
	int result = 0;
	sqlite3_stmt *stmt;
	va_list args;
	char *buf;
	sqlite3 *db;

	va_start(args, fmt);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, fmt, args);
	va_end(args);

	db = DenoraOpenSQL(dbname);
	stmt = DenoraPrepareQuery(db, buf);
	StepReturn = sqlite3_step(stmt);
	if (StepReturn == SQLITE_ROW)
	{
		result = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);
	return result;
}

char **DenoraSQLReturnRow(char *db, const char *query, ...)
{
	va_list args;
	char *buf;
	int res;
	sqlite3 *sqldb;
	char **sdata;
	sqlite3_stmt *stmt;

	va_start(args, query);
	sqlite3_vsnprintf(NET_BUFSIZE, buf, query, args);
	va_end(args);


	sqldb = DenoraOpenSQL(db);
	stmt = DenoraPrepareStmt(sqldb, buf);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(sqldb);

	if (sdata && sdata[0])
	{
		return sdata;
	}
	return NULL;
}

/*************************************************************************/

int DenoraSQLTableExits(char *db, const char *table)
{
	int StepReturn;
	int result = 0;
	sqlite3_stmt *stmt;
	sqlite3 *db;

	sqldb = DenoraOpenSQL(db);
	stmt = DenoraPrepareQuery(sqldb, "SHOW TABLES LIKE '%s';", table);
	StepReturn = sqlite3_step(stmt);
	if (StepReturn == SQLITE_ROW)
	{
		result = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(sqldb);
	return result;
}



/*************************************************************************/

char *SQLfileLoad(char *filename)
{
	FILE    *infile;
	char    *buffer;
	long    numbytes;
 
	/* open an existing file for reading */
	infile = FileOpen(filename, "r");
 
	/* quit if the file does not exist */
	if(infile == NULL)
	    return NULL;
 
	/* Get the number of bytes */
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
 
	/* reset the file position indicator to 
	the beginning of the file */
	fseek(infile, 0L, SEEK_SET);	
 
	/* grab sufficient memory for the 
	buffer to hold the text */
	buffer = (char*)calloc(numbytes, sizeof(char));	
 
	/* memory error */
	if(buffer == NULL)
	    return NULL;
 
	/* copy all the text into the buffer */
	if (fread(buffer, sizeof(char), numbytes, infile))
	{
		fclose(infile);
	}
 
	/* free the memory we used for the buffer */
	return buffer;
}

/*************************************************************************/

