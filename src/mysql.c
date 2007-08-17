
/*
 *
 * (C) 2004-2007 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id$
 *
 */
#include "denora.h"

#ifdef USE_MYSQL

/*************************************************************************/

/* Database Global Variables */
MYSQL *mysql;                   /* MySQL Handler */
MYSQL_FIELD *mysql_fields;      /* MySQL Fields  */
MYSQL_ROW mysql_row;            /* MySQL Row     */

void db_mysql_error(int severity, const char *msg);
char *db_mysql_hidepass(char *sql);

/*************************************************************************/

void db_mysql_error(int severity, const char *msg)
{
    static char buf[512];

    SET_SEGV_LOCATION();

    if (mysql_error(mysql)) {
        ircsnprintf(buf, sizeof(buf), "MySQL %s %s: %s", msg,
                    severity == SQL_WARNING ? "warning" : "error",
                    mysql_error(mysql));
    } else {
        ircsnprintf(buf, sizeof(buf), "MySQL %s %s", msg,
                    severity == SQL_WARNING ? "warning" : "error");
    }

    log_perror("%s", buf);

}

/*************************************************************************/

int db_mysql_init()
{

    SET_SEGV_LOCATION();

    /* If the host is not defined, assume we don't want MySQL */
    if (!SqlHost) {
        denora->do_sql = 0;
        alog(LOG_NORMAL, langstring(ALOG_SQL_DISABLED));
        return 0;
    } else {
        denora->do_sql = 1;
        alog(LOG_NORMAL, langstring(ALOG_SQL_ENABLED), "MySQL");
        alog(LOG_SQLDEBUG, "MySQL: client version %s.",
             mysql_get_client_info());
    }

    SET_SEGV_LOCATION();

    /* The following configuration options are required.
     * If missing disable MySQL to avoid any problems.   
     */

    if ((denora->do_sql) && (!SqlName || !SqlUser)) {
        denora->do_sql = 0;
        alog(LOG_ERROR, langstring(ALOG_SQL_NOTSET));
        return 0;
    }

    SET_SEGV_LOCATION();

    if (!db_mysql_open()) {
        denora->do_sql = 0;
        return 0;
    } else {
        denora->do_sql = 1;
        alog(LOG_SQLDEBUG, "MySQL: server version %s.",
             mysql_get_server_info(mysql));
    }
    SET_SEGV_LOCATION();

    return 1;
}

/*************************************************************************/

int db_mysql_open()
{
    unsigned int seconds;
    int flags;

    /* If MySQL is disabled, return 0 */
    if (!denora->do_sql)
        return 0;

    SET_SEGV_LOCATION();

    mysql = mysql_init(NULL);
    if (mysql == NULL)
        db_mysql_error(SQL_WARNING, "Unable to create mysql object");

    if (!SqlPort) {
        SqlPort = 3306;
    }

    if (!DisableMySQLOPT) {
        mysql_options(mysql, MYSQL_OPT_COMPRESS, 0);
#if MYSQL_VERSION_ID > 40102
        mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "latin1");
#endif
        seconds = SQLPingFreq * 2;
        mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (char *) &seconds);
    }

    SET_SEGV_LOCATION();

    if (DisableMySQLOPT) {
        flags = 0;
    } else {
#ifdef CLIENT_IGNORE_SIGPIPE
        flags =
            CLIENT_COMPRESS | CLIENT_IGNORE_SIGPIPE | CLIENT_INTERACTIVE;
        alog(LOG_DEBUG, "MySQL Client Flag for IGNORE SIGPIPE is set on");
#else
        flags = CLIENT_COMPRESS | CLIENT_INTERACTIVE;
        alog(LOG_DEBUG,
             "MySQL Client Flag for IGNORE SIGPIPE is set unknown unablet to set");
#endif
    }

    if (SqlSock) {
        if ((!mysql_real_connect
             (mysql, SqlHost, SqlUser, (SqlPass ? SqlPass : ""), SqlName,
              SqlPort, SqlSock, flags))) {
            log_perror("MySQL Error: Cant connect to MySQL: %s\n",
                       mysql_error(mysql));
            denora->do_sql = 0;
            return 0;
        }
    } else {
        if ((!mysql_real_connect
             (mysql, SqlHost, SqlUser, (SqlPass ? SqlPass : ""), SqlName,
              SqlPort, NULL, flags))) {
            log_perror("MySQL Error: Cant connect to MySQL: %s\n",
                       mysql_error(mysql));
            denora->do_sql = 0;
            return 0;
        }
    }
    return 1;
}

/*************************************************************************/

int db_mysql_query(char *sql)
{
    int result, lcv;
    int pingresult;

    if (!denora->do_sql) {
        return 0;
    }

    alog(LOG_SQLDEBUG, "sql debug: %s", db_mysql_hidepass(sql));

    pingresult = mysql_ping(mysql);
    if (!pingresult) {
        result = mysql_query(mysql, sql);
    } else {
        result = pingresult;
    }

    if (result) {
        switch (mysql_errno(mysql)) {
        case CR_SERVER_GONE_ERROR:
        case CR_SERVER_LOST:
        case CR_CONN_HOST_ERROR:
        case CR_IPSOCK_ERROR:
        case CR_CONNECTION_ERROR:
        case CR_UNKNOWN_ERROR:
            for (lcv = 0; lcv < SqlRetries; lcv++) {
                if (db_mysql_open()) {
                    result = mysql_query(mysql, sql);
                    return (result);
                }
                sleep(SqlRetryGap);
            }

            /* If we get here, we could not connect. */
            log_perror("Unable to reconnect to database: %s\n",
                       mysql_error(mysql));
            db_mysql_error(SQL_ERROR, "connect");
            alog(LOG_NORMAL, "Disabling MYSQL due to problem with server");
            denora->do_sql = 0;
            SQLDisableDueServerLost = 1;
            /* Never reached. */
            break;
        case CR_COMMANDS_OUT_OF_SYNC:
            db_mysql_error(SQL_ERROR, "Commands out of sync");
            break;
        case ER_PARSE_ERROR:
            db_mysql_error(SQL_ERROR, "parser error");
            break;
        case ER_WRONG_VALUE_COUNT_ON_ROW:
            db_mysql_error(SQL_ERROR, "query error");
            break;
        case ER_BAD_FIELD_ERROR:
            db_mysql_error(SQL_ERROR, "Unknown field");
            break;
        case ER_NO_SUCH_TABLE:
            db_mysql_error(SQL_ERROR, "Table Doesn't exist");
            break;
        case ER_DUP_FIELDNAME:
            break;
        case ER_DUP_ENTRY:
            alog(LOG_ERROR, "MYSQL said %s", mysql_error(mysql));
            alog(LOG_ERROR, "MYSQL query %s", sql);
            break;
        case ER_BAD_NULL_ERROR:
            db_mysql_error(SQL_ERROR,
                           "Column can not be NULL!! - check statement");
            break;
        case ER_USER_LIMIT_REACHED:
            db_mysql_error(SQL_ERROR,
                           "To Many Connections, closing denora mysql connection");
            denora->do_sql = 0;
            db_mysql_close();
            SQLDisableDueServerLost = 1;
            break;
        case ER_GET_ERRNO:
            db_mysql_error(SQL_ERROR,
                           "mysql returned errno code: disabling MYSQL");
            denora->do_sql = 0;
            db_mysql_close();
            break;
        default:
            alog(LOG_ERROR, "MYSQL reported Error Code %d",
                 mysql_errno(mysql));
            alog(LOG_ERROR, "MYSQL said %s", mysql_error(mysql));
            alog(LOG_ERROR, "Report this to Denora Developers");
            alog(LOG_ERROR, "Disabling MYSQL due to this error");
            denora->do_sql = 0;
            db_mysql_close();
            SQLDisableDueServerLost = 1;
            return 0;
        }
    }

    return 1;

}

/*************************************************************************/

char *db_mysql_quote(char *sql)
{
    int slen;
    char *quoted;

    if (!sql) {
        return sstrdup("");
    }

    slen = strlen(sql);
    quoted = malloc((1 + (slen * 2)) * sizeof(char));

    mysql_real_escape_string(mysql, quoted, sql, slen);
    return quoted;

}

/*************************************************************************/

char *db_mysql_hidepass(char *sql)
{
    int slen, pos, i;
    char *buf, *hidden;

    if (!sql) {
        return sstrdup("");
    }

    buf = strstr(sql, "MD5");
    if (!buf) {
        return sql;
    }

    buf = strstr(buf, ", ('");
    if (!buf) {
        return sql;
    }

    slen = strlen(sql);
    pos = (slen - strlen(buf)) + 4;

    hidden = sstrdup(sql);
    for (i = pos; i < slen; i++) {
        if (hidden[i] != ')') {
            hidden[i] = 'x';
        } else {
            hidden[i - 1] = sql[i - 1];
            break;
        }
    }

    return hidden;
}

/*************************************************************************/

/* I don't like using res here, maybe we can pass it as a param? */
int db_mysql_close()
{
    mysql_close(mysql);
    return 1;
}

/*************************************************************************/

void dbMySQLPrepareForQuery(void)
{
    MYSQL_RES *mysql_res;
    mysql_res = mysql_use_result(mysql);
    if (mysql_res) {
        mysql_free_result(mysql_res);
    }
}




#endif
