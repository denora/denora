/* 
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */


#include "sysconf.h"		/* Include auto generated header which  */
				/* describes the users system this file */
				/* is created by running configure	*/
#include "defines.h"

#ifdef __sun			/* If the system reports as Sun OS	*/
#include "os/sun.h"		/* include hacks for the Sun OS		*/
#endif				/* end if				*/

#include <errno.h>		/* Include system error numbers header	*/
#include <stdio.h>		/* Include standard I/O header		*/
#include <stddef.h>		/* Include standard defines header	*/
#include <stdarg.h>		/* Include standard arguements header	*/

#ifdef HAVE_STDLIB_H		/* If system has standard library	*/
#include <stdlib.h>		/* Include standard library header	*/
#endif				/* end if				*/

#ifdef HAVE_STRING_H		/* If system has string library		*/
#include <string.h>		/* Include string library header	*/
#endif				/* end if				*/

#if HAVE_STRINGS_H		/* If system has strings library	*/
#include <strings.h>		/* Include strigns library header	*/
#endif				/* end if				*/

#include <time.h>		/* Include time library header		*/
#include <limits.h>		/* Include limits header		*/

#ifdef HAS_SYS_TYPES_H		/* If system has sys/types header	*/
#include <sys/types.h>		/* Include sys/types header		*/
#endif				/* end if				*/

#include <sys/stat.h>		/* Include sys/stats header		*/
#include <fcntl.h>		/* Include file control header		*/
#include <ctype.h>		/* Include C data types header		*/
#include <assert.h>		/* Include assert header		*/

#ifdef HAVE_STDINT_H		/* If system has standard integer	*/
#include <stdint.h>		/* include standard integer header	*/
#endif				/* end if				*/

#if HAVE_SYS_SELECT_H		/* If system has select header		*/
#include <sys/select.h>		/* Include sys/select header		*/
#endif				/* end if				*/

#ifdef HAVE_SIGNAL_H		/* If system has signal header		*/
#ifndef __OpenBSD__
#include <signal.h>		/* Include signal header		*/
#endif
#endif				/* end if				*/

#ifdef HAVE_POLL_H		/* If system has poll header		*/
#include <poll.h>		/* Include poll header			*/
#endif				/* end if				*/

#ifdef HAVE_REGEX_H
#ifndef _WIN32
 #include <regex.h>
#endif
#endif

/************************************************************************/
/* Include the header for modules as long as we can and its not win32	*/
/************************************************************************/

#if defined(USE_MODULES) && !defined(_WIN32)
#include <dlfcn.h>
#endif

/*************************************************************************/
/* Following includes build info for the given Operating System	  */
/*************************************************************************/

#ifdef _WIN32
#include "os/win32.h"
#else
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__NetBSD__)
#include "os/bsd.h"
#else
#if defined(__CYGWIN__)
#include "os/cygwin.h"
#else
#include "os/linux.h"
#endif
#endif
#endif
#ifdef _AIX
#include "os/aix.h"
#endif

/************************************************************************/
/* If system can use threading include headers for threading		*/
/************************************************************************/

#ifdef USE_THREADS
#ifndef _WIN32			/* Non Win32				*/
#include <pthread.h>		/* Include pthread header		*/
#endif				/* end if				*/
#include "threads.h"		/* our common threading header		*/
#endif

/************************************************************************/
/* Include the zlib header file if the system can has it		*/
/************************************************************************/

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

#include "sqlite3.h"
#include "zip.h"
#include "denoralib_err.h"

/************************************************************************/
/* Remove standard C functions and replace with our own functions	*/
/************************************************************************/

#undef tolower
#undef toupper
#define tolower tolower_
#define toupper toupper_

#ifndef _WIN32
#if defined(__CYGWIN__)
#define E __declspec(dllexport)
#else
#define E extern
#endif
#else
#ifndef MODULE_COMPILE
#define E extern __declspec(dllexport)
#else
#define E extern __declspec(dllimport)
#endif
#endif

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

/* Strings */
char *StringDup(const char *src);
char *str_signed(unsigned char *str);
char *myStrGetToken(const char *str, const char dilim, int token_number);
char *myStrGetOnlyToken(const char *str, const char dilim, int token_number);
char *myStrGetTokenRemainder(const char *str, const char dilim, int token_number);
char *myStrSubString(const char *src, int start, int end);
int myNumToken(const char *str, const char dilim);
char *strnrepl(char *s, int32 size, const char *old, const char *new);

config *DenoraXMLConfigFindBlock(char *mask);
int DenoraParseConnectBlock(int count, char **lines);
config *DenoraXMLConfigBlockCreate(char *newblockname, int (parser)(int ac, char **av), int options);
int DenoraConfigInit(void);
int DenoraParseConnectBlock(int count, char **lines);

#define MAX_CONFIGLIST 1024
config *configlists[MAX_CONFIGLIST];
