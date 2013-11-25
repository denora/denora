/* 
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sqlite.h"
#include "denoralib_err.h"

#define FETCH_ARRAY_NUM 1
#define FETCH_ARRAY_ASSOC 2

/* denoralib.c */
E char *DenoraLib_GetLastError(void);
E int DenoraLastErrorCode;


/* FileIO */
E int FileExists(char *fileName);
E int FileRename(char *fileName, char *newfilename);
E FILE *FileOpen(char *file, const char *mode);


/* Denora_SQL */
sqlite3 *DenoraOpenSQL(char *dbname);
void DenoraCloseSQl(sqlite3 *db);
char *SQLfileLoad(char *filename);
int DenoraExecQuerySQL(sqlite3 *db, const char *fmt, ...);
int DenoraExecQueryDirectSQL(sqlite3 *db, const char *querystring, int callback(void *NotUsed, int argc, char **argv, char **azColName));
sqlite3_stmt *DenoraPrepareQuery(sqlite3 *db, const char *fmt, ...);
char **DenoraSQLFetchArray(sqlite3 *db, char *table, sqlite3_stmt* stmt, int type);

/* Denora_Mem.c */
char ***DenoraCallocArray2D(int x, int y);
char **DenoraCallocArray(int x);