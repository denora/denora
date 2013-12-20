#ifdef USE_MYSQL
/**** mysql.c ****/

MYSQL       *mysql;
MYSQL_FIELD *mysql_fields;
MYSQL_ROW   mysql_row;


char *db_mysql_quote(char *sql);
void dbMySQLPrepareForQuery(int con);
void moduleAddSqlFunctions();
void db_MySQLFreeResult(SQLres *res);
char *db_MySQLErrMsg(SQLCon *con);
int db_mysql_close(SQLCon *con);
int db_mysql_open(void);
int db_mysql_init(void);
int db_mysql_query(SQLCon *con, char *sql);
SQLres *db_mysql_setresult(SQLCon *con);
int db_mysql_num_rows(SQLres *res);
void db_mysql_freeresult(SQLres *res);
int mysql_check_table(char *table);
int mysql_insertid();

#endif