/* Network Bans
 *
 * © 2004-2008 Denora Team
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

static int find_gline(const void *v, const void *cc)
{
    const Gline *t = (void *) v;
    return (stricmp(t->host, (char *) cc));
}

static int find_zline(const void *v, const void *cc)
{
    const Zline *t = (void *) v;
    return (stricmp(t->host, (char *) cc));
}

static int find_qline(const void *v, const void *cc)
{
    const Qline *t = (void *) v;
    return (stricmp(t->host, (char *) cc));
}

list_t *Glinehead;
list_t *Qlinehead;
list_t *Zlinehead;

/*************************************************************************/

void init_bans(void)
{
    Gline *g;
    Zline *z;
    Qline *q;
    SET_SEGV_LOCATION();

    Glinehead = list_create(-1);
    g = malloc(sizeof(Gline));
    bzero(g, sizeof(Gline));

    Zlinehead = list_create(-1);
    z = malloc(sizeof(Zline));
    bzero(z, sizeof(Zline));

    Qlinehead = list_create(-1);
    q = malloc(sizeof(Qline));
    bzero(q, sizeof(Qline));

}

/*************************************************************************/

Gline *new_Gline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
    lnode_t *tn;
    Gline *g = NULL;
    SET_SEGV_LOCATION();

    tn = list_find(Glinehead, host, find_gline);
    if (tn) {
        g = lnode_get(tn);
    } else {
        g = malloc(sizeof(Gline));
        g->host = sstrdup(host);
        g->user = sstrdup(user);
        g->setby = sstrdup(setby);
        g->expires = strtoul(expires, NULL, 10);
        g->setat = strtoul(setat, NULL, 10);
        g->reason = sstrdup(reason);
        tn = lnode_create(g);
        list_append(Glinehead, tn);
    }
    return g;
}

/*************************************************************************/

Zline *new_Zline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
    lnode_t *tn;
    Zline *z = NULL;
    SET_SEGV_LOCATION();

    tn = list_find(Zlinehead, host, find_zline);
    if (tn) {
        z = lnode_get(tn);
    } else {
        z = malloc(sizeof(Gline));
        z->host = sstrdup(host);
        z->user = sstrdup(user);
        z->setby = sstrdup(setby);
        z->expires = strtoul(expires, NULL, 10);
        z->setat = strtoul(setat, NULL, 10);
        z->reason = sstrdup(reason);
        tn = lnode_create(z);
        list_append(Zlinehead, tn);
    }
    return z;
}

/*************************************************************************/

Qline *new_Qline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
    lnode_t *tn;
    Qline *q = NULL;
    SET_SEGV_LOCATION();

    tn = list_find(Qlinehead, host, find_qline);
    if (tn) {
        q = lnode_get(tn);
    } else {
        q = malloc(sizeof(Gline));
        q->host = sstrdup(host);
        q->user = sstrdup(user);
        q->setby = sstrdup(setby);
        q->expires = strtoul(expires, NULL, 10);
        q->setat = strtoul(setat, NULL, 10);
        q->reason = sstrdup(reason);
        tn = lnode_create(q);
        list_append(Qlinehead, tn);
    }
    return q;
}

/*************************************************************************/

Gline *findGline(const char *host)
{
    lnode_t *tn;
    Gline *t = NULL;

    tn = list_find(Glinehead, host, find_gline);
    if (tn) {
        t = lnode_get(tn);
        return t;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Zline *findZline(const char *host)
{
    lnode_t *tn;
    Zline *t = NULL;

    tn = list_find(Zlinehead, host, find_zline);
    if (tn) {
        t = lnode_get(tn);
        return t;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Qline *findQline(const char *host)
{
    lnode_t *tn;
    Qline *t = NULL;

    tn = list_find(Qlinehead, host, find_qline);
    if (tn) {
        t = lnode_get(tn);
        return t;
    } else {
        return NULL;
    }
}

/*************************************************************************/

void fini_bans(void)
{
    Gline *g;
    Qline *q;
    Zline *z;
    lnode_t *tn;

    tn = list_first(Glinehead);
    while (tn != NULL) {
        g = lnode_get(tn);
        free(g->user);
        free(g->host);
        free(g->setby);
        free(g->reason);
        free(g);
        tn = list_next(Glinehead, tn);
    }
    list_destroy_nodes(Glinehead);

    tn = list_first(Zlinehead);
    while (tn != NULL) {
        z = lnode_get(tn);
        free(z->user);
        free(z->host);
        free(z->setby);
        free(z->reason);
        free(z);
        tn = list_next(Zlinehead, tn);
    }
    list_destroy_nodes(Zlinehead);

    tn = list_first(Qlinehead);
    while (tn != NULL) {
        q = lnode_get(tn);
        free(q->user);
        free(q->host);
        free(q->setby);
        free(q->reason);
        free(q);
        tn = list_next(Qlinehead, tn);
    }
    list_destroy_nodes(Qlinehead);
}

/*************************************************************************/

/**
 * Converts Unreal spamfilter action letters to text
 *
 * @param val Unreal spamfilter action letter
 * @return The action in readable text format
 *
 */
const char *ban_char_to_action(char *val)
{
    SET_SEGV_LOCATION();

    if (val) {
        switch (*val) {
        case 'k':
            return "kline";
        case 'K':
            return "kill";
        case 'S':
            return "tenpshun";
        case 's':
            return "shun";
        case 'z':
            return "zline";
        case 'g':
            return "gline";
        case 'Z':
            return "gzline";
        case 'b':
            return "block";
        case 'd':
            return "dcc";
        case 'v':
            return "virsuchan";
        case 'w':
            return "warn";
        default:
            return "unknown";
        }
    }
    SET_SEGV_LOCATION();
    return "unknown";
}

/*************************************************************************/

/**
 * Parse a P10 ircd gline message into a format that sql_do_server_bans_* can handle
 *
 * @param source Server that sent the GLINE
 * @param ac Parameter Count
 * @param av Char array in 0 index
 *
 * @return void - no returend value
 */
void p10_gline(char *type, char *source, int ac, char **av)
{
    Server *s = NULL;
    User *u = NULL;
    char buf[BUFSIZE];
    char *user;
    char *host;
    char *address;
    char *setby;
    char expires[12];

    *buf = '\0';
    *expires = '\0';

    SET_SEGV_LOCATION();

    if (ac < 4 || ac > 5) {
        alog(LOG_DEBUG,
             "debug: %s called with %d needed 4 or 5", PRETTY_FUNCTION,
             ac);
        return;
    }

    if (source) {
        s = server_find(source);
        if (!s) {
            u = user_find(source);
        }
    }
    if (s) {
        setby = s->name;
    } else if (u) {
        setby = u->sqlnick;
    } else {
        setby = source;
    }

    SET_SEGV_LOCATION();

    address = (*av[1] == '!') ? myStrGetToken(av[1], '!', 1) : av[1];
    if (*address == '+') {
        address = myStrGetToken(address, '+', 1);
    } else if (*address == '-') {
        address = myStrGetToken(address, '-', 1);
    }

    if (strstr(address, "@")) {
        user = myStrGetToken(address, '@', 0);
        host = myStrGetToken(address, '@', 1);
    } else {
        user = sstrdup("*");
        host = sstrdup(address);
    }

    SET_SEGV_LOCATION();

    if (*av[1] == '+') {
        if (ac == 5) {
            sprintf(expires, "%d", atoi(av[2]) + atoi(av[3]));
            sql_do_server_bans_add(type, user, host, setby, av[3], expires,
                                   av[4]);
        } else if (ac == 4) {
            ircsnprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
            sprintf(expires, "%ld", atoi(av[2]) + (long int) time(NULL));
            sql_do_server_bans_add(type, user, host, setby, buf, expires,
                                   av[3]);
        }
    } else {
        sql_do_server_bans_remove(type, user, host);
    }
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (address) {
        free(address);
    }
    if (user) {
        free(user);
    }
    if (host) {
        free(host);
    }
}

/*************************************************************************/

/**
 * Parse SQLINE message for SQL
 *
 * @param mask The SQLINE mask that has been banned
 * @param reason The reason for the SQLINE ban
 *
 * @return void - no returend value
 */
void sql_do_sqline(char *mask, char *reason)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlmask;
    char *sqlreason;

    SET_SEGV_LOCATION();

    /* Do not execute if
     * SQL is disabled
     * LargeNet is enabled
     * Mask as not passed as a varaible
     * Reason was not passed as a variable
     */
    if (!denora->do_sql || LargeNet || BadPtr(mask) || BadPtr(reason)) {
        return;
    }

    SET_SEGV_LOCATION();

    sqlmask = rdb_escape(mask);
    sqlreason = rdb_escape(reason);

    SET_SEGV_LOCATION();

    rdb_query
        (QUERY_HIGH, "SELECT mask FROM %s WHERE mask = \'%s\';",
         SqlineTable, sqlmask);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {
            rdb_query
                (QUERY_LOW,
                 "INSERT INTO %s (mask, reason) values('%s', '%s')",
                 SqlineTable, sqlmask, sqlreason);
        } else {
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
                      SqlineTable, sqlreason, sqlmask);
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqlmask) {
        free(sqlmask);
    }
    if (sqlreason) {
        free(sqlreason);
    }
}

/*************************************************************************/

/**
 * Parse SGLINE message for SQL
 * Note that the length should indicated where to split the mask into two pieces
 *
 * @param length number of characters that will indicate the split point in mask and reason
 * @param mask this holds both the mask and reason separated by a :
 * @return void - no returend value
 *
 */
void sql_do_sgline(char *length, char *mask)
{
    long int len;               /* length when converted to integer */
    char *reason;               /* reason for sgline                */
    char *sqlmask;              /* sql escaped mask                 */
    char *sqlreason;            /* sql escaped reason               */
    int errsave;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     * 3. Lenght was not passed
     * 4. Mask was not passed
     */
    if (!denora->do_sql || LargeNet || !length || !mask) {
        return;
    }

    SET_SEGV_LOCATION();

    errsave = errno;            /* temp copy the variable so we can reset the error state */
    errno = 0;                  /* make the errno 0 so we don't report wrong errors */

    len = strtol(length, NULL, 10);
    /*
     * Check error state, to see if we were overflowed
     */
    if (errno == ERANGE) {
        alog(LOG_DEBUG, "%s strtol() set errno to ERANGE possible %s",
             PRETTY_FUNCTION,
             (len == LONG_MAX ? "overflow" : "underflow"));
    }
    errno = errsave;

    SET_SEGV_LOCATION();

    if (((int) strlen(mask) > len) && (mask[len]) == ':') {
        mask[len] = '\0';
        reason = mask + len + 1;
    } else {
        return;
    }

    SET_SEGV_LOCATION();

    sqlmask = rdb_escape(mask);
    sqlreason = rdb_escape(reason);

    rdb_query
        (QUERY_HIGH, "SELECT mask FROM %s WHERE mask = \'%s\';",
         SglineTable, sqlmask);

#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {
            rdb_query
                (QUERY_LOW,
                 "INSERT INTO %s (mask, reason) values('%s', '%s')",
                 SglineTable, sqlmask, sqlreason);
        } else {
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
                      SglineTable, sqlreason, sqlmask);
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqlmask) {
        free(sqlmask);
    }
    if (sqlreason) {
        free(sqlreason);
    }
    return;
}

/*************************************************************************/

/**
 * Parse XLINE message for SQL
 *
 * @param geos mask that will be banned by XLINE
 * @param reason is the reason for the XLINE
 * @return void - no returend value
 *
 */
void sql_do_xline(char *geos, char *reason)
{
    char *sqlgeos;
    char *sqlreason;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     */
    if (!denora->do_sql || LargeNet) {
        return;
    }

    SET_SEGV_LOCATION();

    sqlgeos = rdb_escape(geos);
    sqlreason = rdb_escape(reason);

    rdb_query
        (QUERY_HIGH, "SELECT mask FROM %s WHERE mask = \'%s\';",
         SglineTable, sqlgeos);

#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {
            rdb_query
                (QUERY_LOW,
                 "INSERT INTO %s (mask, reason) values('%s', '%s')",
                 SglineTable, sqlgeos, sqlreason);
        } else {
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
                      SglineTable, sqlreason, sqlgeos);
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqlgeos) {
        free(sqlgeos);
    }
    if (sqlreason) {
        free(sqlreason);
    }
    return;
}

/*************************************************************************/

/**
 * Parse UNXLINE message for SQL
 * @param geos mask that will be removed by UNXLINE
 * @return void - no returend value
 */
void sql_do_unxline(char *geos)
{
    char *sqlgeos;

    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     * 3. geos was NULL
     */
    if (!denora->do_sql || LargeNet || !geos) {
        return;
    }
    SET_SEGV_LOCATION();

    sqlgeos = rdb_escape(geos);
    rdb_query(QUERY_LOW, "DELETE FROM %s WHERE mask=\'%s\'", SglineTable,
              sqlgeos);

    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqlgeos) {
        free(sqlgeos);
    }
    return;
}

/*************************************************************************/

/**
 * Unreal passes in TKL the expires as the exact time of the expiration
 * for this reason and the fact they use "type" we do not add the expire + time
 * only on ircds where this is not done
 *
 * @param type If Unreal the type is Gline, Shun, etc..
 * @param user is the user ident that is banned
 * @param host is the user host that is banned
 * @param setby is the person or server that has set the ban
 * @param setat is the unix timestamp of when the ban was set
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param reason is the reason for the ban
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_add(char *type, char *user, char *host,
                            char *setby, char *setat, char *expires,
                            char *reason)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqluser;
    char *sqlhost;
    int checkdur = 0;
    int expire = 0;
    uint32 setattime = 0;
    Gline *g;
    Qline *q;
    Zline *z;


    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     */
    if (!denora->do_sql || LargeNet) {
        return;
    }
    if (!stricmp(type, "G")) {
        g = findGline(host);
        if (g) {
            if (g->setat == (int32) strtoul(setat, NULL, 10)
                && g->expires == (int32) strtoul(expires, NULL, 10)) {
                return;
            }
        } else {
            new_Gline(user, host, setby, setat, expires, reason);
        }
    }

    if (!stricmp(type, "Q")) {
        q = findQline(host);
        if (q) {
            if (q->setat == (int32) strtoul(setat, NULL, 10)
                && q->expires == (int32) strtoul(expires, NULL, 10)) {
                return;
            }
        } else {
            new_Qline(user, host, setby, setat, expires, reason);
        }
    }

    if (!stricmp(type, "Z")) {
        z = findZline(host);
        if (z) {
            if (z->setat == (int32) strtoul(setat, NULL, 10)
                && z->expires == (int32) strtoul(expires, NULL, 10)) {
                return;
            }
        } else {
            new_Zline(user, host, setby, setat, expires, reason);
        }
    }


    SET_SEGV_LOCATION();

    reason = rdb_escape(reason);
    sqluser = rdb_escape(user);
    sqlhost = rdb_escape(host);

    if (type) {
        rdb_query
            (QUERY_HIGH,
             "SELECT id FROM %s WHERE type = \'%s\' and user=\'%s\' and host=\'%s\';",
             GlineTable, type, sqluser, sqlhost);
#ifdef USE_MYSQL
        SET_SEGV_LOCATION();
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (type, user, host, setby, setat, expires, reason) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',%ld,%ld,\'%s\')",
                     GlineTable, type, sqluser, sqlhost, setby,
                     strtoul(setat, NULL, 10), strtoul(expires, NULL, 10),
                     reason);
            } else {
                rdb_query
                    (QUERY_LOW,
                     "UPDATE %s SET setat=%ld, expires=%ld, reason=\'%s\' WHERE type = \'%s\' and user=\'%s\' and host=\'%s\'",
                     GlineTable, strtoul(setat, NULL, 10), strtoul(expires,
                                                                   NULL,
                                                                   10),
                     reason, type, sqluser, sqlhost);
            }
            mysql_free_result(mysql_res);
        }
#endif
    } else {
        SET_SEGV_LOCATION();

        rdb_query(QUERY_HIGH,
                  "SELECT id FROM %s WHERE user=\'%s\' and host=\'%s\';",
                  GlineTable, sqluser, sqlhost);

        checkdur = strtoul(expires, NULL, 10);
        setattime = strtoul(setat, NULL, 10);
        if (checkdur != 0) {
            expire = checkdur + setattime;
        } else {
            expire = checkdur;
        }
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (user, host, setby, setat, expires, reason) VALUES(\'%s\',\'%s\',\'%s\',%ld,%ld,\'%s\')",
                     GlineTable, sqluser, sqlhost, setby, setattime,
                     expire, reason);
            } else {
                rdb_query
                    (QUERY_LOW,
                     "UPDATE %s SET setat=%ld, expires=%ld, reason=\'%s\' WHERE user=\'%s\' and host=\'%s\'",
                     GlineTable, setattime, expire, reason, sqluser,
                     sqlhost);
            }
            mysql_free_result(mysql_res);
        }
#endif
    }
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (reason) {
        free(reason);
    }
    if (sqluser) {
        free(sqluser);
    }
    if (sqlhost) {
        free(sqlhost);
    }
    return;
}

/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages into SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param setby is the person or server that has set the ban
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param setat is the unix timestamp of when the ban was set
 * @param duration is the number of seconds the ban will last
 * @param reason is the reason for the ban
 * @param regex is the regular expression for the spam filter
 *
 * @return void - no returend value
 */
void sql_do_server_spam_add(char *target, char *action,
                            char *setby, char *expires, char *setat,
                            char *duration, char *reason, char *regex)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlreason;
    char *sqlregex;
    SpamFilter *sf;

    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     */
    if (!denora->do_sql || LargeNet) {
        return;
    }
    sf = findSpamFilter(regex);
    if (sf) {
        if (sf->duration == (int32) strtoul(duration, NULL, 10)
            && sf->setat == (int32) strtoul(setat, NULL, 10)) {
            return;
        }
    } else {
        new_SpamFilter(target, action, setby, expires, setat, duration,
                       reason, regex);
    }

    SET_SEGV_LOCATION();

    sqlreason = rdb_escape(reason);
    sqlregex = rdb_escape(regex);

    rdb_query(QUERY_HIGH, "SELECT id FROM %s WHERE regex = \'%s\';",
              SpamTable, sqlregex);

#ifdef USE_MYSQL
    if (strlen(action) == 1) {
        action = sstrdup(ban_char_to_action(action));
    }

    SET_SEGV_LOCATION();

    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {
            rdb_query
                (QUERY_LOW,
                 "INSERT INTO %s (target, action, setby, expires, setat, duration, reason, regex) VALUES(\'%s\',\'%s\',\'%s\',%ld, %ld,%ld, \'%s\', \'%s\')",
                 SpamTable, target, action, setby, strtoul(expires, NULL,
                                                           10),
                 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
                 sqlreason, sqlregex);
        } else {
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET target=\'%s\', action=\'%s\', setby=\'%s\', expires=%ld, setat=%ld, duration=%ld, reason=\'%s\' WHERE regex =\'%s\'",
                 SpamTable, target, action, setby, strtoul(expires, NULL,
                                                           10),
                 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
                 sqlreason, sqlregex);
        }
        mysql_free_result(mysql_res);
    }
#endif
    /*
     * Free the data that was allocated
     */
    SET_SEGV_LOCATION();

    if (sqlreason) {
        free(sqlreason);
    }
    if (sqlregex) {
        free(sqlregex);
    }
    return;
}

/*************************************************************************/

/**
 * Parse GLINE messages and remove from SQL
 *
 * @param type If Unreal the type is Gline, Shun, etc.. this is optional
 * @param user is the user ident that is being unbanned
 * @param host is the user host that is being unbanned
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_remove(char *type, char *user, char *host)
{
    char *sqluser;
    char *sqlhost;

    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     */
    if (!denora->do_sql || LargeNet) {
        return;
    }

    SET_SEGV_LOCATION();

    sqluser = rdb_escape(user);
    sqlhost = rdb_escape(host);

    SET_SEGV_LOCATION();

    if (type) {
        rdb_query
            (QUERY_LOW,
             "DELETE FROM %s WHERE type=\'%s\' and user=\'%s\' and host=\'%s\'",
             GlineTable, type, sqluser, sqlhost);
    } else {
        rdb_query
            (QUERY_LOW, "DELETE FROM %s WHERE user=\'%s\' and host=\'%s\'",
             GlineTable, sqluser, sqlhost);
    }
    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqluser) {
        free(sqluser);
    }
    if (sqlhost) {
        free(sqlhost);
    }
    return;
}

/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages remove from SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param regex is the regular expression that made up the ban
 *
 * @return void - no returend value
 */
void sql_do_server_spam_remove(char *target, char *action, char *regex)
{
    char *sqlregex;             /* local variable to store regex from rdb_escape */
    char *sqlaction;            /* local variable to store action from sstrdup */
    char *sqltarget;            /* local variable to store target from rdb_escape */

    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     * 3. Regex was NULL
     * 4. Action was NULL
     * 5. Target was NULL
     */
    if (!denora->do_sql || LargeNet || !regex || !action || !target) {
        return;
    }

    SET_SEGV_LOCATION();

    sqlaction = sstrdup(ban_char_to_action(action));    /* copy result into local variable */
    sqlregex = rdb_escape(regex);       /* prepare sql escaped string */
    sqltarget = rdb_escape(target);     /* prepare sql escaped string */

    SET_SEGV_LOCATION();

    /*
     * Query the spam table and remove bans that match the following
     * 1. Target
     * 2. Action text
     * 3. Regex
     */
    rdb_query
        (QUERY_LOW,
         "DELETE FROM %s WHERE target=\'%s\' and action=\'%s\' and regex=\'%s'",
         SpamTable, sqltarget, sqlaction, sqlregex);

    SET_SEGV_LOCATION();

    /*
     * Free the data that was allocated
     */
    if (sqlregex) {
        free(sqlregex);
    }
    if (sqlaction) {
        free(sqlaction);
    }
    if (sqltarget) {
        free(sqltarget);
    }
    return;
}

/*************************************************************************/

/**
 * Clean up the GlineTable and SpamTable since some ircds assume we should
 * remove the bans from ourselves without being told.
 *
 * @param name containing when the event was called
 * @return MOD_CONT to indicate event is done
 *
 */
int sql_ban_clean(const char *name)
{
    /*
     * Prevent compiler warnings that this variable is not used
     */
    USE_VAR(name);

    SET_SEGV_LOCATION();

    /*
     * Do not execute if
     * 1. SQL code has been disabled
     * 2. LargeNet is enabled
     */
    if (!denora->do_sql || LargeNet) {
        return MOD_CONT;
    }

    SET_SEGV_LOCATION();

    /*
     * Query the gline table and remove bans that the expiration is
     * 1. Not 0 (ie.. no expire)
     * 2. Less then the current time
     */
    rdb_query(QUERY_LOW,
              "DELETE FROM %s WHERE expires !=0 AND expires < %ld",
              GlineTable, time(NULL));

    /*
     * Check if the ircd supports spamfilter and if so we should clean that
     * up as well
     */
    SET_SEGV_LOCATION();

    if (ircd->spamfilter) {
        /*
         * Query the spamfilter table and remove bans that the expiration is
         * 1. Not 0 (ie.. no expire)
         * 2. Less then the current time
         */
        rdb_query(QUERY_LOW,
                  "DELETE FROM %s WHERE expires !=0 AND expires < %ld",
                  SpamTable, time(NULL));
    }
    SET_SEGV_LOCATION();

    /*
     * return MOD_CONT when we are all done.
     */
    return MOD_CONT;
}

/*************************************************************************/
