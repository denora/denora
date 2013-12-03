/* 
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */

#define FETCH_ARRAY_NUM 1
#define FETCH_ARRAY_ASSOC 2

typedef struct MD5Context MD5_CTX;

/************************************************************************/
/* MD5									*/
/************************************************************************/

struct MD5Context {
	uint32 buf[4];		/* Current digest state/value.		*/
	uint32 bits[2];		/* Number of bits hashed so far.	*/
	unsigned char in[64];	/* Residual input buffer.		*/
};

/************************************************************************/
/* Base64 struct data							*/
/************************************************************************/

struct buffer_st
{
	char *data;
	int length;
	char *ptr;
	int offset;
};

typedef struct config_ config;
struct config_
{
	config *prev, *next;

	char *name;
	int (*parser)(int count, char **lines);
	int numoptions;
};

#define CONFIGHASH(nick)	(((nick)[0]&31)<<5 | ((nick)[1]&31))


/* denoralib.c */
char *DenoraLib_GetLastError(void);
int DenoraLastErrorCode;


/* Denora_SQL */
sqlite3 *DenoraOpenSQL(char *dbname);
void DenoraCloseSQl(sqlite3 *db);
char *SQLfileLoad(char *filename);
int DenoraExecQuerySQL(sqlite3 *db, const char *fmt, ...);
int DenoraExecQueryDirectSQL(sqlite3 *db, const char *querystring, int callback(void *NotUsed, int argc, char **argv, char **azColName));
sqlite3_stmt *DenoraPrepareQuery(sqlite3 *db, const char *fmt, ...);
char ***DenoraSQLFetchArray(sqlite3 *db, char *table, sqlite3_stmt *stmt, int type);

/* Denora_Mem.c */
char ***DenoraCallocArray2D(int x, int y);
char **DenoraCallocArray(int x);

/* Strings */
char *StringDup(const char *src);

config *DenoraXMLConfigFindBlock(char *mask);
config *DenoraXMLConfigBlockCreate(char *newblockname, int (parser)(int ac, char **av), int options);
int DenoraConfigInit(void);
int DenoraParseConnectBlock(int count, char **lines);

#define MAX_CONFIGLIST 1024
config *configlists[MAX_CONFIGLIST];

