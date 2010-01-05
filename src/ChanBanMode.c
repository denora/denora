/* Channel Modes - read MODES for complete details on how Denora
 * handles modes.
 *
 * (c) 2004-2010 Denora Team
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

ChanBanModeHash *CHANBANMODEHANDLERS[MAX_CMD_HASH];

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
ChanBanMode *FindChanBanMode(char *name)
{
    int idx;
    ChanBanModeHash *privcurrent = NULL;

    if (!name) {
        return NULL;
    }
    if (!denora_cmode(*name)) {
        alog(LOG_DEBUG, "Mode %s %c not found", name, *name);
        return NULL;
    }

    idx = CMD_HASH(name);

    SET_SEGV_LOCATION();

    for (privcurrent = CHANBANMODEHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (strcmp(name, privcurrent->mode) == 0) {
            return privcurrent->cbm;
        }
    }
    return NULL;
}

/*************************************************************************/

ChanBanMode *CreateChanBanMode(int mode,
                               void (*addmask) (Channel * chan,
                                                char *mask),
                               void (*delmask) (Channel * chan,
                                                char *mask))
{
    ChanBanMode *m;
    char modebuf[15];

    if (!mode) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    ircsnprintf(modebuf, sizeof(modebuf), "%c", mode);

    if ((m = malloc(sizeof(ChanBanMode))) == NULL) {
        fatal("Out of memory!");
    }
    m->mode = sstrdup(modebuf); /* Our Name                 */
    m->addmask = addmask;       /* Handle                   */
    m->delmask = delmask;
    addChanBanMode(m);
    ModuleSetChanMode(mode, IRCD_ENABLE);
    return m;                   /* return a nice new module */
}

/*************************************************************************/

int addChanBanMode(ChanBanMode * m)
{
    int modindex = 0;
    ChanBanModeHash *privcurrent = NULL;
    ChanBanModeHash *newHash = NULL;
    ChanBanModeHash *lastHash = NULL;

    SET_SEGV_LOCATION();

    modindex = CMD_HASH(m->mode);

    for (privcurrent = CHANBANMODEHANDLERS[modindex]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->mode, privcurrent->mode) == 0)
            return MOD_ERR_EXISTS;
        lastHash = privcurrent;
    }

    if ((newHash = malloc(sizeof(ChanBanModeHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->mode = sstrdup(m->mode);
    newHash->cbm = m;

    if (lastHash == NULL)
        CHANBANMODEHANDLERS[modindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyChanBanMode(ChanBanMode * m)
{
    if (!m) {
        return MOD_ERR_PARAMS;
    }

    if (m->mode) {
        free(m->mode);
    }

    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

int delChanBanMode(ChanBanMode * m)
{
    int idx = 0;
    ChanBanModeHash *privcurrent = NULL;
    ChanBanModeHash *lastHash = NULL;

    if (!m) {
        return MOD_ERR_PARAMS;
    }

    idx = CMD_HASH(m->mode);

    for (privcurrent = CHANBANMODEHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->mode, privcurrent->mode) == 0) {
            if (!lastHash) {
                CHANBANMODEHANDLERS[idx] = privcurrent->next;
            } else {
                lastHash->next = privcurrent->next;
            }
            destroyChanBanMode(privcurrent->cbm);
            free(privcurrent->mode);
            free(privcurrent);
            return MOD_ERR_OK;
        }
        lastHash = privcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

void sql_channel_ban(int type, Channel * c, char *mask)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlchan;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }
    if (mask) {
        mask = rdb_escape(mask);
    }
    if (c && c->name) {
        sqlchan = rdb_escape(c->name);
    } else {
        return;
    }

    if (type == 1) {
        rdb_query
            (QUERY_HIGH,
             "SELECT bans FROM %s WHERE chan = \'%s\' and bans = \'%s\';",
             ChanBansTable, sqlchan, mask);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (chan, bans) VALUES (\'%s\', \'%s\')",
                     ChanBansTable, sqlchan, mask);
            }
            mysql_free_result(mysql_res);
        }
#endif
    } else if (type == 2) {
        if (c->sqlchan) {
            rdb_query(QUERY_LOW,
                      "DELETE FROM %s WHERE chan=\'%s\' AND bans=\'%s\'",
                      ChanBansTable, sqlchan, mask);
        }
    } else {
        rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                  ChanBansTable, sqlchan);
    }
    SET_SEGV_LOCATION();
    if (mask) {
        free(mask);
    }
    if (sqlchan) {
        free(sqlchan);
    }
}

/*************************************************************************/

void sql_channel_quiet(int type, Channel * c, char *mask)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlchan;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }
    if (mask) {
        mask = rdb_escape(mask);
    }
    if (c && c->name) {
        sqlchan = rdb_escape(c->name);
    } else {
        return;
    }

    if (type == 1) {
        rdb_query
            (QUERY_HIGH,
             "SELECT bans FROM %s WHERE chan = \'%s\' and bans = \'%s\';",
             ChanQuietTable, sqlchan, mask);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (chan, bans) VALUES (\'%s\', \'%s\')",
                     ChanQuietTable, sqlchan, mask);
            }
            mysql_free_result(mysql_res);
        }
#endif
    } else if (type == 2) {
        if (c->sqlchan) {
            rdb_query(QUERY_LOW,
                      "DELETE FROM %s WHERE chan=\'%s\' AND bans=\'%s\'",
                      ChanQuietTable, sqlchan, mask);
        }
    } else {
        rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                  ChanQuietTable, sqlchan);
    }
    SET_SEGV_LOCATION();
    if (mask) {
        free(mask);
    }
    if (sqlchan) {
        free(sqlchan);
    }
}

/*************************************************************************/

void sql_channel_exception(int type, Channel * c, char *mask)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlchan;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }

    if (mask) {
        mask = rdb_escape(mask);
    }
    if (c && c->name) {
        sqlchan = rdb_escape(c->name);
    } else {
        return;
    }

    if (type == 1) {
        rdb_query
            (QUERY_HIGH,
             "SELECT mask FROM %s WHERE chan = \'%s\' and mask = \'%s\';",
             ChanExceptTable, c->sqlchan, mask);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT IGNORE INTO %s (chan, mask) VALUES (\'%s\', \'%s\')",
                     ChanExceptTable, sqlchan, mask);
            }
            mysql_free_result(mysql_res);
        }
#endif
    } else if (type == 2) {
        rdb_query(QUERY_LOW,
                  "DELETE FROM %s WHERE chan=\'%s\' AND mask=\'%s\'",
                  ChanExceptTable, sqlchan, mask);
    } else {
        rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                  ChanExceptTable, sqlchan);
    }
    SET_SEGV_LOCATION();
    if (mask) {
        free(mask);
    }
    if (sqlchan) {
        free(sqlchan);
    }
}

/*************************************************************************/

void sql_channel_invite(int type, Channel * c, char *mask)
{
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *sqlchan = NULL;
    char *sqlmask = NULL;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }

    if (mask) {
        sqlmask = rdb_escape(mask);
    }
    if (c && c->name) {
        sqlchan = rdb_escape(c->name);
    } else {
        return;
    }

    if (type == 1) {
        rdb_query
            (QUERY_HIGH,
             "SELECT mask FROM %s WHERE chan = \'%s\' and mask = \'%s\';",
             ChanInviteTable, sqlchan, sqlmask);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT IGNORE INTO %s (chan, mask) VALUES (\'%s\', \'%s\')",
                     ChanInviteTable, sqlchan, sqlmask);
            }
            mysql_free_result(mysql_res);
        }
#endif
    } else if (type == 2) {
        rdb_query(QUERY_LOW,
                  "DELETE FROM %s WHERE chan=\'%s\' AND mask=\'%s\'",
                  ChanInviteTable, sqlchan, sqlmask);
    } else {
        rdb_query(QUERY_LOW, "DELETE FROM %s WHERE chan=\'%s\'",
                  ChanInviteTable, sqlchan);
    }
    SET_SEGV_LOCATION();
    if (mask && sqlmask) {
        free(sqlmask);
    }
    if (sqlchan) {
        free(sqlchan);
    }
}

/*************************************************************************/

void add_ban(Channel * chan, char *mask)
{
    SET_SEGV_LOCATION();

    /* check for NULL values otherwise we will segfault */
    if (!chan || !mask) {
        alog(LOG_DEBUG, langstr(ALOG_ADD_BAN_BAD));
        return;
    }

    if (chan->bancount >= chan->bansize) {
        chan->bansize += 8;
        chan->bans = realloc(chan->bans, sizeof(char *) * chan->bansize);
    }
    chan->bans[chan->bancount++] = sstrdup(mask);

    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, langstr(ALOG_ADD_BAN_ADDED), mask, chan->name);

    if (denora->do_sql) {
        sql_channel_ban(ADD, chan, mask);
    }
}

/*************************************************************************/

void add_exception(Channel * chan, char *mask)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->exceptcount >= chan->exceptsize) {
        chan->exceptsize += 8;
        chan->excepts =
            realloc(chan->excepts, sizeof(char *) * chan->exceptsize);
    }
    chan->excepts[chan->exceptcount++] = sstrdup(mask);

    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, langstr(ALOG_ADD_EXCEPTION_ADDED), mask, chan->name);

    if (denora->do_sql) {
        sql_channel_exception(ADD, chan, mask);
    }
}

/*************************************************************************/

void add_invite(Channel * chan, char *mask)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->invitecount >= chan->invitesize) {
        chan->invitesize += 8;
        chan->invite =
            realloc(chan->invite, sizeof(char *) * chan->invitesize);
    }
    chan->invite[chan->invitecount++] = sstrdup(mask);

    alog(LOG_DEBUG, langstr(ALOG_ADD_INVITE_ADDED), mask, chan->name);
    SET_SEGV_LOCATION();

    if (denora->do_sql) {
        sql_channel_invite(ADD, chan, mask);
    }
}

/*************************************************************************/

void add_quiet(Channel * chan, char *mask)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->quietcount >= chan->quietsize) {
        chan->quietsize += 8;
        chan->quiet =
            realloc(chan->quiet, sizeof(char *) * chan->quietsize);
    }
    chan->quiet[chan->quietcount++] = sstrdup(mask);

    alog(LOG_DEBUG, langstr(ALOG_ADD_INVITE_ADDED), mask, chan->name);
    SET_SEGV_LOCATION();

    if (denora->do_sql) {
        sql_channel_quiet(ADD, chan, mask);
    }
}

/*************************************************************************/

void del_ban(Channel * chan, char *mask)
{
    char **s;
    int i = 0;

    SET_SEGV_LOCATION();

    /* Sanity check as it seems some IRCD will just send -b without a mask */
    if (!chan || !mask) {
        return;
    }

    s = chan->bans;

    while (i < chan->bancount && strcmp(*s, mask) != 0) {
        i++;
        s++;
    }

    if (i < chan->bancount) {
        chan->bancount--;
        if (i < chan->bancount) {
            memmove(s, s + 1, sizeof(char *) * (chan->bancount - i));
        }
        alog(LOG_DEBUG, langstr(ALOG_BAN_DEL_CHAN), mask, chan->name);
    }
    SET_SEGV_LOCATION();

    if (denora->do_sql) {
        sql_channel_ban(DEL, chan, mask);
    }
}

/*************************************************************************/

void del_exception(Channel * chan, char *mask)
{
    int i;
    int reset = 0;

    SET_SEGV_LOCATION();

    /* Sanity check as it seems some IRCD will just send -e without a mask */
    if (!chan || !mask) {
        return;
    }

    for (i = 0; i < chan->exceptcount; i++) {
        if ((!reset) && (stricmp(chan->excepts[i], mask) == 0)) {
            free(chan->excepts[i]);
            reset = 1;
        }
        if (reset) {
            chan->excepts[i] =
                (i == chan->exceptcount) ? NULL : chan->excepts[i + 1];
        }
    }

    if (reset) {
        chan->exceptcount--;
    }
    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, langstr(ALOG_EXCEPTION_DEL_CHAN), mask, chan->name);

    if (denora->do_sql) {
        sql_channel_exception(DEL, chan, mask);
    }
}

/*************************************************************************/

void del_invite(Channel * chan, char *mask)
{
    int i;
    int reset = 0;

    /* Sanity check as it seems some IRCD will just send -I without a mask */
    if (!chan || !mask) {
        return;
    }

    SET_SEGV_LOCATION();

    for (i = 0; i < chan->invitecount; i++) {
        if ((!reset) && (stricmp(chan->invite[i], mask) == 0)) {
            free(chan->invite[i]);
            reset = 1;
        }
        if (reset) {
            chan->invite[i] =
                ((i == chan->invitecount) ? NULL : chan->invite[i + 1]);
        }
    }

    if (reset) {
        chan->invitecount--;
    }

    alog(LOG_DEBUG, langstr(ALOG_INVITE_DEL_CHAN), mask, chan->name);
    SET_SEGV_LOCATION();
    if (denora->do_sql) {
        sql_channel_invite(DEL, chan, mask);
    }
}

/*************************************************************************/

void del_quiet(Channel * chan, char *mask)
{
    int i;
    int reset = 0;

    /* Sanity check as it seems some IRCD will just send -I without a mask */
    if (!mask) {
        return;
    }

    SET_SEGV_LOCATION();

    for (i = 0; i < chan->quietcount; i++) {
        if ((!reset) && (stricmp(chan->quiet[i], mask) == 0)) {
            free(chan->quiet[i]);
            reset = 1;
        }
        if (reset) {
            chan->quiet[i] =
                ((i == chan->quietcount) ? NULL : chan->quiet[i + 1]);
        }
    }

    if (reset) {
        chan->quietcount--;
    }

    alog(LOG_DEBUG, langstr(ALOG_INVITE_DEL_CHAN), mask, chan->name);
    SET_SEGV_LOCATION();
    if (denora->do_sql) {
        sql_channel_quiet(DEL, chan, mask);
    }
}
