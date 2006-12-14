
/*
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
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

User *userlist[1024];
Uid *uidlist[1024];
static Uid *ucurrent;
static User *current;
static int next_index;
User *new_user(const char *nick);
GeoIP *gi;

/*************************************************************************/

/**
 * SQL handle the user mode changes
 *
 * @param u is the user struct for which the modes changed
 * @param modes is the modes to add/remove
 *
 * @return void - no returend value
 *
 */
void sql_do_usermodes(User * u, char *modes)
{
    int atleastone = 0;
    char db[MAX_SQL_BUF];
    char tmp[14] = "mode_XX=\'X\', ";
    char buf[1000];
    int nickid = 0;

    SET_SEGV_LOCATION();
    nickid = db_getnick(u->sqlnick);

    if (!nickid) {
        return;
    }

    if (!denora->do_sql) {
        return;
    }

    *db = '\0';
    *buf = '\0';

    SET_SEGV_LOCATION();

    if (strlen(modes) == 1 && (*modes == '+' || *modes == '-')) {
        alog(LOG_DEBUG, "Received just +/- as the users mode");
        return;
    }

    ircsnprintf(buf, sizeof(buf), "UPDATE %s SET ", UserTable);
    strlcpy(db, buf, sizeof(db));
    while (*modes) {
        switch (*modes) {
        case '+':
            tmp[9] = 'Y';
            break;
        case '-':
            tmp[9] = 'N';
            break;
        default:
            if (!strchr(ircd->usermodes, *modes)) {
                alog(LOG_DEBUG, langstr(ALOG_UKNOWN_USER_MODE), *modes,
                     inbuf);
            } else {
                atleastone = 1;
                tmp[5] = ((*modes >= 'a') ? 'l' : 'u');
                tmp[6] = tolower(*modes);
                /* strcat(db, tmp); */
                strlcat(db, tmp, sizeof(db));
            }
            break;
        }
        modes++;
    }
    SET_SEGV_LOCATION();

    ircsnprintf(&db[strlen(db) - 2], sizeof(db), " WHERE nickid=%d",
                nickid);
    if (atleastone) {
        rdb_query(QUERY_LOW, db);
    } else {
        alog(LOG_DEBUG,
             "There were no modes or atleastone (%d) was still 0",
             atleastone);
        alog(LOG_DEBUG, "buffer = %s", db);
    }
}

/*************************************************************************/

/**
 * Reset all user modes to N for the given nick ID
 */

void sql_reset_usermodes(int nickid, char *nickname)
{
    char *modes = "qwertzuiopasdfghjklyxcvbnmQWERTZUIOPASDFGHJKLYXCVBNM";
    char db[MAX_SQL_BUF];
    char tmp[14] = "mode_XX=\'X\', ";
    char buf[1000];

    *db = '\0';
    *buf = '\0';

    ircsnprintf(buf, sizeof(buf), "UPDATE %s SET ", UserTable);
    strlcpy(db, buf, sizeof(db));
    while (*modes) {
        tmp[9] = 'N';
        if (strchr(ircd->usermodes, *modes)) {
            tmp[5] = ((*modes >= 'a') ? 'l' : 'u');
            tmp[6] = tolower(*modes);
            strlcat(db, tmp, sizeof(db));
        }
        modes++;
    }
    SET_SEGV_LOCATION();

    if (nickid == 0) {
        ircsnprintf(&db[strlen(db) - 2], sizeof(db), " WHERE nick='%s'",
                    nickname);
    } else {
        ircsnprintf(&db[strlen(db) - 2], sizeof(db), " WHERE nickid=%d",
                    nickid);
    }
    rdb_query(QUERY_LOW, db);
}

/*************************************************************************/

/**
 * Store the SWHOIS message
 *
 * @param user is the nick of the user changing SWHOIS for
 * @param msg is the swhois message
 *
 * @return void - no returend value
 *
 */
void do_swhois(char *user, char *msg)
{
    int nickid;
    User *u;
    char *sqlmsg = NULL;

    SET_SEGV_LOCATION();

    if (!msg || !*msg) {
        alog(LOG_ERROR,
             "SWHOIS message was NULL, please run debug to get more info");
        return;
    }

    /* Find the user struct for the given user */
    u = user_find(user);
    if (!u) {
        alog(LOG_NONEXISTANT, langstr(ALOG_SWHOIS_ERROR), user);
        return;                 /* Can't find them exit */
    }

    /* Check if current set, if so we should free it before using it */
    if (u->swhois) {
        free(u->swhois);
        u->swhois = NULL;
    }
    u->swhois = sstrdup(msg);

    SET_SEGV_LOCATION();

    if (denora->do_sql) {
        sqlmsg = rdb_escape(msg);
        nickid = db_getnick_unsure(u->sqlnick);
        if (nickid == -1) {
            alog(LOG_NONEXISTANT, langstr(ALOG_SWHOIS_ERROR), user);
        } else {
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET swhois=\'%s\' WHERE nickid=%d",
                      UserTable, sqlmsg, nickid);
        }
        free(sqlmsg);
    }

    SET_SEGV_LOCATION();
    return;
}

/*************************************************************************/

/**
 * Update SQL for the new nick
 *
 * @param u is the user struct from which we will pull the information
 *
 * @return void - no returend value
 *
 */
void sql_do_nick(User * u)
{
    int servid;
    int add = 1;
    int nickid;
    char *username, *host, *server, *realname;
    char *vhost = NULL;
    char *countryname, *countrycode;

    SET_SEGV_LOCATION();

    username = rdb_escape(u->username);
    host = rdb_escape(u->host);
    server = rdb_escape(u->server->name);
    if (ircd->vhost) {
        vhost = rdb_escape(u->vhost);
    }
    realname = rdb_escape(u->realname);
    servid = db_getserver(server);

    countryname = rdb_escape(u->country_name);
    countrycode = rdb_escape(u->country_code);

    SET_SEGV_LOCATION();

    if (NickTracking == 0) {
        nickid = db_checknick(u->sqlnick);
    } else if (LargeNet || !UplinkSynced) {
        nickid = db_checknick(u->sqlnick);
    } else {
        nickid = db_checknick_nt(u->sqlnick);
    }

    if (UserCacheTime && (nickid != -1)) {
        SET_SEGV_LOCATION();
        if (ircd->vhost) {
            rdb_query
                (QUERY_HIGH,
                 "UPDATE %s SET nick=\'%s\', hopcount=%d, nickip=\'%s\', countrycode=\'%s\', country=\'%s\', realname=\'%s\', hostname=\'%s\', hiddenhostname=\"%s\", username=\'%s\', connecttime=FROM_UNIXTIME(%d), servid=%d, server=\'%s\', lastquit=NULL, online=\'Y\', away=\'N\', awaymsg=\'\' WHERE nickid=%d",
                 UserTable, u->sqlnick, u->hopcount, u->ip, countrycode,
                 countryname, realname, host, vhost, username,
                 u->timestamp, servid, server, nickid);
        } else {
            rdb_query
                (QUERY_HIGH,
                 "UPDATE %s SET nick=\'%s\', hopcount=%d, nickip=\'%s\', countrycode=\'%s\', country=\'%s\', realname=\'%s\', hostname=\'%s\', username=\'%s\', connecttime=FROM_UNIXTIME(%d), servid=%d, server=\'%s\', lastquit=NULL, online=\'Y\', away=\'N\', awaymsg=\'\' WHERE nickid=%d",
                 UserTable, u->sqlnick, u->hopcount, u->ip, countrycode,
                 countryname, realname, host, username, u->timestamp,
                 servid, server, nickid);
        }
        sql_reset_usermodes(nickid, NULL);
        add = 0;
    }
    if (add) {
        SET_SEGV_LOCATION();
        if (KeepUserTable) {
            if (ircd->vhost) {
                rdb_query
                    (QUERY_HIGH,
                     "INSERT INTO %s (nick, hopcount, nickip, realname, hostname, hiddenhostname, username, connecttime, servid, server, countrycode, country) VALUES(\'%s\',%d,\'%s\', \'%s\',\'%s\',\'%s\',\'%s\',FROM_UNIXTIME(%d),%d, \'%s\', \'%s\', \'%s\') ON DUPLICATE KEY UPDATE nick=\'%s\', hopcount=%d, nickip=\'%s\', realname=\'%s\', hostname=\'%s\', hiddenhostname=\'%s\', username=\'%s\', connecttime=FROM_UNIXTIME(%d), servid=%d, server=\'%s\', countrycode=\'%s\', country=\'%s\', lastquit=NULL, online=\'Y\', away=\'N\', awaymsg=\'\'",
                     UserTable, u->sqlnick, u->hopcount, u->ip, realname,
                     host, vhost, username, u->timestamp, servid, server,
                     countrycode, countryname, u->sqlnick, u->hopcount,
                     u->ip, realname, host, vhost, username, u->timestamp,
                     servid, server, countrycode, countryname);
            } else {
                rdb_query
                    (QUERY_HIGH,
                     "INSERT INTO %s (nick, hopcount, nickip, realname, hostname, username, connecttime, servid, server, countrycode, country) VALUES(\'%s\',%d,\'%s\', \'%s\',\'%s\',\'%s\',FROM_UNIXTIME(%d),%d, \'%s\', \'%s\', \'%s\') ON DUPLICATE KEY UPDATE nick=\'%s\', hopcount=%d, nickip=\'%s\', realname=\'%s\', hostname=\'%s\', username=\'%s\', connecttime=FROM_UNIXTIME(%d), servid=%d, server=\'%s\', countrycode=\'%s\', country=\'%s\', lastquit=NULL, online=\'Y\', away=\'N\', awaymsg=\'\'",
                     UserTable, u->sqlnick, u->hopcount, u->ip, realname,
                     host, username, u->timestamp, servid, server,
                     countrycode, countryname, u->sqlnick, u->hopcount,
                     u->ip, realname, host, username, u->timestamp, servid,
                     server, countrycode, countryname);
            }
            sql_reset_usermodes(0, u->sqlnick);
        } else {
            if (ircd->vhost) {
                rdb_query
                    (QUERY_HIGH,
                     "INSERT INTO %s (nick, hopcount, nickip, realname, hostname, hiddenhostname, username, connecttime, servid, server, countrycode, country) VALUES(\'%s\',%d,\'%s\', \'%s\',\'%s\',\'%s\',\'%s\',FROM_UNIXTIME(%d),%d, \'%s\', \'%s\', \'%s\')",
                     UserTable, u->sqlnick, u->hopcount, u->ip, realname,
                     host, vhost, username, u->timestamp, servid, server,
                     countrycode, countryname);
            } else {
                rdb_query
                    (QUERY_HIGH,
                     "INSERT INTO %s (nick, hopcount, nickip, realname, hostname, username, connecttime, servid, server, countrycode, country) VALUES(\'%s\',%d,\'%s\', \'%s\',\'%s\',\'%s\',FROM_UNIXTIME(%d),%d, \'%s\', \'%s\', \'%s\')",
                     UserTable, u->sqlnick, u->hopcount, u->ip, realname,
                     host, username, u->timestamp, servid, server,
                     countrycode, countryname);
            }
        }
    }
    SET_SEGV_LOCATION();

    nickid = db_getnick(u->sqlnick);
    if (nickid) {
        u->sqlid = nickid;
    }

    if (UserCacheTime) {
        db_cleanuser();
    }
    SET_SEGV_LOCATION();

    free(username);
    free(host);
    free(server);
    SET_SEGV_LOCATION();

    if (ircd->vhost) {
        free(vhost);
    }
    free(realname);
    free(countrycode);
    free(countryname);
    SET_SEGV_LOCATION();
}

/*************************************************************************/

/**
 * Update SQL when the nick has been changed
 *
 * @param newnick is the nick being changed to
 * @param oldnick is the nick being changed from
 *
 * @return void - no returend value
 *
 */
void sql_do_nick_chg(char *newnick, char *oldnick)
{
    int nickid;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif

    SET_SEGV_LOCATION();

    if (!denora->do_sql) {
        return;
    }

    /* NULL parameter checks */
    if (!newnick || !*newnick || !oldnick || !*oldnick) {
        return;
    }

    newnick = rdb_escape(newnick);
    oldnick = rdb_escape(oldnick);
    /* the target nickname might already exist if caching is enabled */
    if (UserCacheTime && (strcasecmp(newnick, oldnick))
        && ((nickid = db_checknick(newnick)) != -1)) {
        /* In this case, we don't keep a record of the old nick. It would be :
         * - technically difficult, because we'd have to make a copy of the record
         * - dangerous, because it would provide an easy way to fill up the DB */
        rdb_query(QUERY_HIGH, "DELETE from %s WHERE nickid=%d", UserTable,
                  nickid);
    }
    rdb_query
        (QUERY_HIGH, "SELECT nick FROM %s WHERE nick = \"%s\";", UserTable,
         newnick);
    SET_SEGV_LOCATION();

#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {
            /* we update the nickname */
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET nick=\'%s\' WHERE nick=\'%s\'",
                      UserTable, newnick, oldnick);
        } else {
            rdb_query(QUERY_LOW, "DELETE from %s WHERE nick=\'%s\'",
                      UserTable, newnick);
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET nick=\'%s\' WHERE nick=\'%s\'",
                      UserTable, newnick, oldnick);
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();
    /* we update the alias record */
    rdb_query(QUERY_LOW,
              "UPDATE IGNORE %s SET nick=\'%s\' WHERE nick=\'%s\'",
              AliasesTable, newnick, oldnick);
    free(newnick);
    free(oldnick);
}

/*************************************************************************/

/**
 * Allocate a new User structure, fill in basic values, link it to the
 * overall list, and return it.  Always successful.
 *
 * @param nick is nickname of the user to create the struct for
 *
 * @return User struct
 *
 */
User *new_user(const char *nick)
{
    User *user, **list;

    SET_SEGV_LOCATION();
    user = calloc(sizeof(User), 1);
    if (!nick || !*nick) {
        nick = "";
    }
    strlcpy(user->nick, nick, NICKMAX);
    list = &userlist[USERHASH(user->nick)];
    user->next = *list;
    if (*list) {
        (*list)->prev = user;
    }
    *list = user;
    return user;
}

/*************************************************************************/

/**
 * Change the nickname of a user, and move pointers as necessary.
 *
 * @param user is the old user struct
 * @param nick is nickname of the user to create the struct for
 *
 * @return void - no returend value
 *
 */
static void change_user_nick(User * user, const char *nick)
{
    User **list;

    /* Sanity check to make sure we don't segfault */
    if (!user || !nick || !*nick) {
        return;
    }

    SET_SEGV_LOCATION();

    if (user->prev) {
        user->prev->next = user->next;
    } else {
        userlist[USERHASH(user->nick)] = user->next;
    }
    if (user->next) {
        user->next->prev = user->prev;
    }
    user->nick[1] = 0;          /* paranoia for zero-length nicks */
    strlcpy(user->nick, nick, NICKMAX);
    list = &userlist[USERHASH(user->nick)];
    user->next = *list;
    user->prev = NULL;
    if (*list) {
        (*list)->prev = user;
    }
    *list = user;
}

/*************************************************************************/

/**
 * Update internal and SQL on CHGHOST or SETHOST like commands
 *
 * @param source is the person we are setting the host for
 * @param host is the new virtual host
 *
 * @return void - no returend value
 *
 */
void change_user_host(char *source, char *host)
{
    User *user;
    char *msg;
    int userid;

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_VHOST_NONEXISTANT_USER),
             source);
        return;
    }

    SET_SEGV_LOCATION();

    if (user->vhost) {
        free(user->vhost);
        user->vhost = NULL;
    }
    user->vhost = sstrdup(host);

    alog(LOG_DEBUG, langstr(ALOG_VHOST_FOR), user->nick, host);

    if (denora->do_sql) {
        msg = rdb_escape(user->vhost);
        SET_SEGV_LOCATION();
        userid = db_getnick(user->sqlnick);

        if (ircd->vhostchar && ircd->vhostchar2) {
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET mode_l%c=\'Y\', mode_l%c=\'Y\', hiddenhostname=\'%s\' WHERE nickid=%d",
                 UserTable, ircd->vhostchar, ircd->vhostchar2, msg,
                 userid);
        } else if (ircd->vhostchar && !ircd->vhostchar2) {
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET mode_l%c=\'Y\', hiddenhostname=\'%s\' WHERE nickid=%d",
                 UserTable, ircd->vhostchar, msg, userid);
        } else {
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET hiddenhostname=\'%s\' WHERE nickid=%d",
                 UserTable, msg, userid);
        }
        free(msg);
    }
    SET_SEGV_LOCATION();
}

/*************************************************************************/

/**
 * Update internal and SQL on CHGNAME or SETNAME like commands
 *
 * @param source is the person we are setting the host for
 * @param host is the new virtual realname
 *
 * @return void - no returend value
 *
 */
void change_user_realname(char *source, char *realname)
{
    User *user;
    char *msg;
    int userid;

    SET_SEGV_LOCATION();

    if (!realname || !*realname || !source || !*source) {
        return;
    }

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_CHGNAME_NONEXISTANT_USER),
             source);
        return;
    }

    SET_SEGV_LOCATION();

    if (user->realname) {
        free(user->realname);
        user->realname = NULL;
    }
    user->realname = sstrdup(realname);

    alog(LOG_DEBUG, langstr(ALOG_REALNAME_FOR), user->nick, realname);

    if (denora->do_sql) {
        msg = rdb_escape(user->realname);
        SET_SEGV_LOCATION();
        userid = db_getnick(user->sqlnick);
        rdb_query
            (QUERY_LOW, "UPDATE %s SET realname=\'%s\' WHERE nickid=%d",
             UserTable, msg, userid);
        free(msg);
    }
    SET_SEGV_LOCATION();
}

/*************************************************************************/

/**
 * Update internal and SQL on SETIDENT or CHGIDENT like commands
 *
 * @param source is the person we are setting the host for
 * @param host is the new virtual ident
 *
 * @return void - no returend value
 *
 */
void change_user_username(char *source, char *username)
{
    User *user;
    char *msg;
    int userid;

    SET_SEGV_LOCATION();

    if (!username || !*username || !source || !*source) {
        return;
    }

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_CHGIDENT_NONEXISTANT_USER),
             source);
        return;
    }

    if (user->username) {
        free(user->username);
        user->username = NULL;
    }
    user->username = sstrdup(username);

    alog(LOG_DEBUG, langstr(ALOG_USERNAME_FOR), user->nick, username);

    if (denora->do_sql) {
        msg = rdb_escape(user->username);
        userid = db_getnick(user->sqlnick);
        SET_SEGV_LOCATION();
        rdb_query
            (QUERY_LOW, "UPDATE %s SET username=\'%s\' WHERE nickid=%d",
             UserTable, msg, userid);
        free(msg);
    }
}

/*************************************************************************/

/**
 * Delete user from the hash
 *
 * @param user is the user struct to remove
 *
 * @return void - no returend value
 *
 */
void delete_user(User * user)
{
    struct u_chanlist *c, *c2;
    PrivMsg *p;

    SET_SEGV_LOCATION();

    if (!user) {
        return;
    }
    send_event(EVENT_DELETE_USER, 1, user->nick);
    del_current_user(user->server);

    alog(LOG_EXTRADEBUG, "debug: delete_user() called for %s", user->nick);
    if (is_oper(user)) {
        del_oper_count(user);
    }

    if (user->isservice) {
        p = findPrivMsg(user->nick);
        delPrivMsg(p);
    }

    if (user->country_code) {
        tld_update(user->country_code);
    }
    alog(LOG_EXTRADEBUG, "debug: delete_user(): free user data");
    free(user->username);
    free(user->host);
    if (user->vhost) {
        free(user->vhost);
    }
    free(user->ip);
    if (CTCPUsers) {
        if (user->ctcp) {
            ctcp_update(user->ctcp);
            free(user->ctcp);
        }
    }
    if (user->isaway && user->awaymsg) {
        free(user->awaymsg);
    }

    free(user->country_code);
    free(user->country_name);
    if (user->sgroup) {
        free(user->sgroup);
    }
    if (user->swhois) {
        free(user->swhois);
    }
    free(user->realname);
    if (denora->do_sql) {
        free(user->sqlnick);
    }
    alog(LOG_EXTRADEBUG, "debug: delete_user(): remove from channels");
    c = user->chans;
    while (c) {
        c2 = c->next;
        chan_deluser(user, c->chan);
        free(c);
        c = c2;
    }
    alog(LOG_EXTRADEBUG, "debug: delete_user(): free founder data");

    moduleCleanStruct(&user->moduleData);

    alog(LOG_EXTRADEBUG, "debug: delete_user(): delete from list");
    if (user->prev) {
        user->prev->next = user->next;
    } else {
        userlist[USERHASH(user->nick)] = user->next;
    }
    if (user->next) {
        user->next->prev = user->prev;
    }
    alog(LOG_EXTRADEBUG, "debug: delete_user(): free user structure");
    free(user);
    alog(LOG_EXTRADEBUG, "debug: delete_user() done");
}

/*************************************************************************/

/**
 * Find a user by nick.  Return NULL if user could not be found.
 *
 * @param nick is the nickname to locate the struct of
 *
 * @return User struct
 *
 */
User *finduser(const char *nick)
{
    User *user;

    SET_SEGV_LOCATION();

    if (!nick || !*nick) {
        alog(LOG_DEBUG, "Error: finduser() called with NULL values");
        do_backtrace(0);
        return NULL;
    }

    alog(LOG_EXTRADEBUG, "debug: finduser(%p)", nick);
    user = userlist[USERHASH(nick)];
    while (user && stricmp(user->nick, nick) != 0) {
        user = user->next;
    }
    alog(LOG_EXTRADEBUG, "debug: finduser(%s) -> 0x%p", nick,
         (void *) user);
    return user;
}

/*************************************************************************/

/**
 * Iterate over all users in the user list.  Return NULL at end of list.
 *
 * @return User struct
 *
 */
User *firstuser(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL) {
        current = userlist[next_index++];
    }
    alog(LOG_EXTRADEBUG, "debug: firstuser() returning %s",
         current ? current->nick : "NULL (end of list)");
    if (current) {
        return current;
    } else {
        return NULL;
    }
}

/*************************************************************************/

/**
 * Move to the next user in the list
 *
 * @return User struct
 *
 */
User *nextuser(void)
{
    SET_SEGV_LOCATION();

    if (current) {
        current = current->next;
    }
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL) {
            current = userlist[next_index++];
        }
    }
    alog(LOG_EXTRADEBUG, "debug: nextuser() returning %s",
         current ? current->nick : "NULL (end of list)");
    return current;
}

/*************************************************************************/

User *find_byuid(const char *uid)
{
    User *u, *next;

    u = first_uid();
    while (u) {
        next = next_uid();
        if (u && u->uid) {
            if (!stricmp(uid, u->uid)) {
                return u;
            }
        }
        u = next;
    }
    return NULL;
}

/*************************************************************************/

/**
 * Iterate over all user ids in the user list.  Return NULL at end of list.
 *
 * @return User struct
 *
 */
User *first_uid(void)
{
    next_index = 0;
    while (next_index < 1024 && current == NULL) {
        current = userlist[next_index++];
    }
    alog(LOG_EXTRADEBUG, "debug: first_uid() returning %s %s",
         current ? current->nick : "NULL (end of list)",
         current ? current->uid : "");
    return current;
}

/*************************************************************************/

/**
 * Move to the next user id in the list
 *
 * @return User struct
 *
 */
User *next_uid(void)
{
    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL) {
            current = userlist[next_index++];
        }
    }
    alog(LOG_EXTRADEBUG, "debug: next_uid() returning %s %s",
         current ? current->nick : "NULL (end of list)",
         current ? current->uid : "");
    return current;
}

/*************************************************************************/

Uid *uid_first(void)
{
    next_index = 0;
    while (next_index < 1024 && ucurrent == NULL) {
        ucurrent = uidlist[next_index++];
    }
    alog(LOG_EXTRADEBUG, "debug: uid_first() returning %s %s",
         ucurrent ? ucurrent->nick : "NULL (end of list)",
         ucurrent ? ucurrent->uid : "");
    return ucurrent;
}

/*************************************************************************/

Uid *uid_next(void)
{
    if (ucurrent)
        ucurrent = ucurrent->next;
    if (!ucurrent && next_index < 1024) {
        while (next_index < 1024 && ucurrent == NULL)
            ucurrent = uidlist[next_index++];
    }
    alog(LOG_EXTRADEBUG, "debug: uid_next() returning %s %s",
         ucurrent ? ucurrent->nick : "NULL (end of list)",
         ucurrent ? ucurrent->uid : "");
    return ucurrent;
}

/*************************************************************************/

Uid *new_uid(const char *nick, char *uid)
{
    Uid *u, **list;

    u = calloc(sizeof(Uid), 1);
    if (!nick || !uid) {
        return NULL;
    }
    strlcpy(u->nick, nick, NICKMAX);
    list = &uidlist[UIDHASH(u->nick)];
    u->next = *list;
    if (*list)
        (*list)->prev = u;
    *list = u;
    u->uid = sstrdup(uid);
    return u;
}

/*************************************************************************/

void delete_uid(Uid * u)
{
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: delete_uid() called");

    free(u->uid);

    alog(LOG_EXTRADEBUG, "debug: delete_uid(): delete from list");

    SET_SEGV_LOCATION();
    if (u->prev) {
        u->prev->next = u->next;
    } else {
        uidlist[UIDHASH(u->nick)] = u->next;
    }
    if (u->next) {
        u->next->prev = u->prev;
    }
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: delete_uid(): free user structure");

    free(u);
    alog(LOG_EXTRADEBUG, "debug: delete_uid() done");
}

/*************************************************************************/

/**
 * Find the uid struct for the given user nick
 *
 * @param nick is the user to look for
 *
 * @return uid struct
 *
 */
Uid *find_uid(const char *nick)
{
    Uid *u;
    int i;

    if (!nick || !*nick) {
        return NULL;
    }

    for (i = 0; i < 1024; i++) {
        for (u = uidlist[i]; u; u = u->next) {
            if (u->nick) {
                if (!stricmp(nick, u->nick)) {
                    return u;
                }
            }
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Find the uid struct for the given uid
 *
 * @param uid is the user id to look for
 *
 * @return uid struct
 *
 */
Uid *find_nickuid(const char *uid)
{
    Uid *u;
    int i;

    if (!uid || !*uid) {
        return NULL;
    }

    for (i = 0; i < 1024; i++) {
        for (u = uidlist[i]; u; u = u->next) {
            if (u->uid) {
                if (!stricmp(uid, u->uid)) {
                    return u;
                }
            }
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Handle NICK commands from the ircd
 *
 * @param source is the orgin of the mssage
 * @param nick is the nick to handle (if change this is the new nick)
 * @param username is the ident
 * @param host is the host
 * @param server is the server the user is on
 * @param realname is the realname (geos) of the user
 * @param ts is the time stamp for the event (either change or connect time)
 * @param svid is the service time stamp if any
 * @param ip if nickip the ip in int32 fashion
 * @param vhost if vhost on nick it is passed now
 * @param uid if the ircd uses uid for the nick it is passed now
 * @param hopcount is the hop count
 * @param modes is the user modes if they are sent on nick
 *
 * @return void - no returend value
 *
 */
User *do_nick(const char *source, char *nick, char *username, char *host,
              char *server, char *realname, time_t ts, uint32 svid,
              char *ipchar, char *vhost, char *uid, int hopcount,
              char *modes)
{

    User *user;
    char *newav[5];
    Server *s;
    const char *country_code;
    const char *country_name;
    TLD *tld;

    SET_SEGV_LOCATION();

    if (!*source) {
        if (!LargeNet) {
            country_code = GeoIP_country_code_by_name(gi, host);
            country_name = GeoIP_country_name_by_name(gi, host);
            if (!country_name) {
                if (host && !stricmp("localhost", host)) {
                    country_name = "localhost";
                } else {
                    country_name = "Unknown";
                }
            }
            if (!country_code) {
                if (host && !stricmp("localhost", host)) {
                    country_code = "local";
                } else {
                    country_code = "??";
                }
            }
        } else {
            country_name = "Unknown";
            country_code = "??";
        }
        s = server_find(server);
        /* Allocate User structure and fill it in. */
        user = new_user(nick);
        user->username = (username ? sstrdup(username) : NULL);
        user->host = (host ? sstrdup(host) : NULL);
        user->server = (s ? s : me_server);
        add_current_user(user->server);
        user->realname = sstrdup((realname ? realname : ""));
        user->timestamp = ts;
        user->my_signon = time(NULL);
        user->vhost = (vhost ? sstrdup(vhost) : sstrdup(host));
        user->uid = (uid ? sstrdup(uid) : NULL);
        user->admin = 0;        /* 0 by default, winner, eh? */
        user->hopcount = hopcount;
        user->ip = sstrdup(ipchar);
        user->cstats = 0;
        user->sgroup = NULL;
        user->language = StatsLanguage;
        user->country_code = sstrdup(country_code);
        user->country_name = sstrdup(country_name);
        if (!LargeNet) {
            tld = do_tld(user->country_name, user->country_code);
            if (denora->do_sql) {
                if (ircd->syncstate || UplinkSynced) {
                    sql_do_tld(UPDATE, user->country_code,
                               user->country_name, tld->count,
                               tld->overall);
                }
            }
        }
        user->svid = (svid == (uint32) ts ? svid : 1);
        user->sqlnick = rdb_escape(user->nick);
        send_event(EVENT_NEWNICK, 1, user->nick);
        SET_SEGV_LOCATION();
        if (denora->do_sql) {
            sql_do_nick(user);
            if (modes) {
                sql_do_usermodes(user, modes);
            }
        }
        if (!LargeNet) {
            if (CTCPUsers && stricmp(s_StatServ, user->nick)) {
                if (CTCPUsersEOB) {
                    if (user->server->sync) {
                        denora_cmd_privmsg(s_StatServ, user->nick,
                                           "\1VERSION\1");
                        send_event(EVENT_SENT_CTCP_VERSION, 1, user->nick);
                    }
                } else {
                    denora_cmd_privmsg(s_StatServ, user->nick,
                                       "\1VERSION\1");
                    send_event(EVENT_SENT_CTCP_VERSION, 1, user->nick);
                }
            }
        }
        if (modes) {
            newav[0] = sstrdup(modes);
            denora_set_umode(user, 1, newav);
            free(newav[0]);
        }
    } else {
        user = user_find(source);
        if (!user) {
            return NULL;
        }
        user->timestamp = ts;

        if (stricmp(nick, user->nick)) {
            sql_do_nick_chg(nick, user->nick);
            change_user_nick(user, nick);
            alog(LOG_DEBUG, "debug: %s has changed nicks to %s", source,
                 nick);
            send_event(EVENT_CHANGE_NICK, 2, source, nick);
            free(user->sqlnick);
            user->sqlnick = rdb_escape(user->nick);
        } else {
            alog(LOG_DEBUG, "compare says its the same");
            user->my_signon = time(NULL);
        }

    }
    SET_SEGV_LOCATION();
    return user;
}

/*************************************************************************/

/**
 * Handle UMODE messages from the ircd
 *
 * @param source is the user whom just changed modes
 * @param ac is the array count
 * @param av is the array
 *
 * @return void - no returend value
 *
 */
void do_umode(const char *source, int ac, char **av)
{
    User *user;

    SET_SEGV_LOCATION();

    if (!source || !*source) {
        return;
    }

    if (ac < 1) {
        return;
    }

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_UMODE_NONEXISTANT),
             av[1], source, merge_args(ac, av));
        return;
    }

    denora_set_umode(user, ac - 1, &av[1]);
    if (denora->do_sql) {
        sql_do_usermodes(user, av[1]);
    }
}

/*************************************************************************/

/**
 * Handle SVSMODE messages from the ircd
 *
 * @param ac is the array count
 * @param av is the array
 *
 * @return void - no returend value
 *
 */
void do_svsumode(int ac, char **av)
{
    User *user;

    SET_SEGV_LOCATION();

    if (ac < 2) {
        return;
    }

    user = user_find(av[0]);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_UMODE_NONEXISTANT),
             av[1], av[0], merge_args(ac, av));
        return;
    }

    if (ircdcap->tsmode) {
        if (denora->capab & ircdcap->tsmode || UseTSMODE) {
            denora_set_umode(user, ac - 1, &av[2]);
            if (denora->do_sql) {
                sql_do_usermodes(user, av[2]);
            }
        } else {
            denora_set_umode(user, ac - 1, &av[1]);
            if (denora->do_sql) {
                sql_do_usermodes(user, av[1]);
            }
        }
    } else {
        denora_set_umode(user, ac - 1, &av[1]);
        if (denora->do_sql) {
            sql_do_usermodes(user, av[1]);
        }
    }
}

/*************************************************************************/

/**
 * Handle UMODE2 messages from the ircd
 *
 * @param source is the user whom just changed modes
 * @param ac is the array count
 * @param av is the array
 *
 * @return void - no returend value
 *
 */
void do_umode2(const char *source, int ac, char **av)
{
    User *user;

    SET_SEGV_LOCATION();

    if (!source || !*source) {
        return;
    }

    if (ac < 1) {
        return;
    }

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, langstr(ALOG_UMODE_NONEXISTANT),
             av[0], source, merge_args(ac, av));
        return;
    }
    SET_SEGV_LOCATION();

    denora_set_umode(user, ac, &av[0]);
    if (denora->do_sql) {
        sql_do_usermodes(user, av[0]);
    }
}

/*************************************************************************/

/**
 * Handle QUIT messages from the ircd
 *
 * @param nick is the user whom just quit
 * @param msg is the reason they quit
 *
 * @return void - no returend value
 *
 */
void do_quit(const char *source, int ac, char **av)
{
    User *user;

    SET_SEGV_LOCATION();

    if (!source || !*source) {
        return;
    }

    if (ac < 1) {
        return;
    }

    user = user_find(source);
    if (!user) {
        alog(LOG_NONEXISTANT, "user: QUIT from nonexistent user %s: %s",
             source, merge_args(ac, av));
        return;
    }
    alog(LOG_DEBUG, "debug: %s quits (%s)", user->nick,
         (!BadPtr(av[0]) ? av[0] : "Quit"));

    if (denora->do_sql) {
        if (NickTracking == 0) {
            db_removenick(user->sqlnick,
                          (!BadPtr(av[0]) ? av[0] : (char *) "Quit"));
        } else if (LargeNet || !UplinkSynced) {
            db_removenick(user->sqlnick,
                          (!BadPtr(av[0]) ? av[0] : (char *) "Quit"));
        } else {
            db_removenick_nt(user->sqlnick,
                             (!BadPtr(av[0]) ? av[0] : (char *) "Quit"));
        }
        if (UserCacheTime) {
            db_cleanuser();
        }
    }
    send_event(EVENT_USER_LOGOFF, 2, user->nick,
               (!BadPtr(av[0]) ? av[0] : "Quit"));
    delete_user(user);
}

/*************************************************************************/

/**
 * Handle KILL messages from the ircd
 *
 * @param nick is the user whom just got killed
 * @param msg is the reason they got killed
 *
 * @return void - no returend value
 *
 */
void do_kill(char *nick, char *msg)
{
    User *user;

    if (!nick || !*nick || !msg || !*msg) {
        return;
    }

    user = user_find(nick);

    SET_SEGV_LOCATION();

    if (!user) {
        alog(LOG_NONEXISTANT, "debug: KILL of nonexistent nick: %s", nick);
        return;
    }
    alog(LOG_DEBUG, "debug: %s killed: %s", nick, msg);
    send_event(EVENT_USER_LOGOFF, 2, user->nick, msg);
    delete_user(user);
}

/*************************************************************************/

/**
 * Check if the given user is a ircop +o
 *
 * @param user is the user struct to check
 *
 * @return 1 if is oper, 0 if they are not
 *
 */
int is_oper(User * user)
{
    SET_SEGV_LOCATION();

    if (user) {
        SET_SEGV_LOCATION();
        return (UserHasMode(user->nick, denora_get_oper_mode()));
    } else {
        SET_SEGV_LOCATION();
        return 0;
    }
}

/*************************************************************************/

/**
 * Get the vhost for the user, if set else return the host, on ircds without
 * vhost this returns the host
 *
 * @param u user to get the vhost for
 *
 * @return vhost
 */
char *common_get_vhost(User * u)
{
    SET_SEGV_LOCATION();

    if (!u) {
        return NULL;
    }
    if (ircd->vhostmode) {
        if (u->mode & ircd->vhostmode) {
            return u->vhost;
        } else {
            return u->host;
        }
    } else {
        return u->host;
    }
}

/*************************************************************************/

/**
 * Get the vident for the user, if set else return the ident, on ircds without
 * vident this returns the ident
 *
 * @param u user to get info the vident for
 *
 * @return vident
 */
char *common_get_vident(User * u)
{
    SET_SEGV_LOCATION();

    if (!u) {
        return NULL;
    }
    if (ircd->vhostmode) {
        if (u->mode & ircd->vhostmode) {
            if (u->vident) {
                return u->vident;
            } else {
                return u->username;
            }
        } else {
            return u->username;
        }
    } else {
        return u->username;
    }
}

/*************************************************************************/

User *user_find(const char *source)
{
    User *user = NULL;

    if (!source) {
        return NULL;
    }

    if (ircd->p10) {
        user = find_byuid(source);
        if (!user) {
            return finduser(source);
        } else {
            return user;
        }
    } else if (UseTS6 && ircd->ts6) {
        user = find_byuid(source);
        if (!user) {
            return finduser(source);
        } else {
            return user;
        }
    } else {
        return finduser(source);
    }
    return NULL;
}

/*************************************************************************/
