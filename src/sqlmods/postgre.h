#ifdef HAVE_POSTGRE_H

void moduleAddSqlFunctions();
void db_postgreFreeResult(SQLres *res);
char *db_postgreErrMsg(SQLCon *con);
int db_postgreClose(SQLCon *con);
char *db_postgre_quote(char *sql);
int db_postgre_open(void);
int db_postgre_init(void);
int db_postgre_query(SQLCon *con, char *sql);
SQLres *db_postgre_setresult(SQLCon *con);
int db_postgre_num_rows(SQLres *res);
void db_postgre_freeresult(SQLres *res);
int postgre_check_table(char *table);
char **PostgreSQLFetchRow(SQLres *res);



#endif