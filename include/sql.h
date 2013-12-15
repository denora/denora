/************************************************************************/
/* MySQL headers that we want to include and defines that older		*/
/* versions do not have.						*/
/* Note that MYSQL_HEADER_PREFIX should be trapped from	sysconf.h	*/
/************************************************************************/

#ifdef USE_MYSQL
#ifdef MYSQL_HEADER_PREFIX
#include <mysql/mysql_version.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>
#else
#include <mysql_version.h>
#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>
#endif
#ifndef ER_USER_LIMIT_REACHED
#define ER_USER_LIMIT_REACHED 1226
#endif
#endif

#ifdef HAVE_POSTGRE_H
#include <postgresql/libpq-fe.h>
#endif

typedef struct {
#ifdef USE_MYSQL
	MYSQL_RES *myres;
#endif
#ifdef HAVE_POSTGRE_H
	PGresult *pgres;
#endif
	int res;
} SQLres;


typedef struct {
#ifdef USE_MYSQL
	MYSQL *mycon;
#endif
#ifdef HAVE_POSTGRE_H
	PGconn *pgconn;
#endif
	int con;
} SQLCon;