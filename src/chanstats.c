/*
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

static void make_stats(User * u, char *receiver, char *msg);
static uint32 countsmileys(char *text);
static int check_db(User * u, Channel * c);
static char *newsplit(char **rest);
list_t *CStatshead;

int CompareCStats(const void *v, const void *cc)
{
    const ChannelStats *cs = (void *) v;
    return (stricmp(cs->name, (char *) cc));
}

/*************************************************************************/

void InitCStatsList(void)
{
    ChannelStats *cs;
    SET_SEGV_LOCATION();

    CStatshead = list_create(-1);
    cs = malloc(sizeof(ChannelStats));
    bzero(cs, sizeof(ChannelStats));
}

/*************************************************************************/

/**
 * Locate ChannelStats struct by the mask
 *
 * @param mask is the ChannelStats to locate
 * @return ChannelStats stats struct
 *
 */
ChannelStats *find_cs(char *mask)
{
    lnode_t *tn;
    ChannelStats *c = NULL;

    tn = list_find(CStatshead, mask, CompareCStats);
    if (tn) {
        c = lnode_get(tn);
        return c;
    } else {
        return NULL;
    }
}

/*************************************************************************/

ChannelStats *makecs(char *mask)
{
    lnode_t *tn;
    ChannelStats *c = NULL;
    SET_SEGV_LOCATION();

    tn = list_find(CStatshead, mask, CompareCStats);
    if (tn) {
        c = lnode_get(tn);
    } else {
        c = malloc(sizeof(ChannelStats));
        c->name = sstrdup(mask);
        c->flags = 0;
        c->timeadded = 0;
        tn = lnode_create(c);
        list_append(CStatshead, tn);
    }
    return c;
}

/*************************************************************************/

int del_cs(ChannelStats * cs)
{
    lnode_t *tn;
    ChannelStats *c = NULL;
    SET_SEGV_LOCATION();

    if (cs) {
        tn = list_find(CStatshead, cs->name, CompareCStats);
        if (tn) {
            c = lnode_get(tn);
            lnode_destroy(ListDelete(CStatshead, tn));
            free(c);
        }
    }
    return 0;
}

/*************************************************************************/

void Fini_ChannelStats(void)
{
    ChannelStats *t;
    lnode_t *tn;

    tn = list_first(CStatshead);
    while (tn != NULL) {
        t = lnode_get(tn);
        free(t);
        tn = list_next(CStatshead, tn);
    }
    list_destroy_nodes(CStatshead);
}

/*************************************************************************/

/**
 * Load the ChannelStats database from disk
 *
 * @return void - no returend value
 *
 */
void load_cs_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    ChannelStats *cs = NULL;
    char *key, *value;
    int retval = 0;

    fill_db_ptr(dbptr, 0, CS_VERSION, s_StatServ, ChannelStatsDB);
    SET_SEGV_LOCATION();

    /* let's remove existing temp files here, because we only load dbs on startup */
    remove(dbptr->temp_name);

    /* Open the db, fill the rest of dbptr and allocate memory for key and value */
    if (new_open_db_read(dbptr, &key, &value)) {
        free(dbptr);
        return;                 /* Bang, an error occurred */
    }
    SET_SEGV_LOCATION();

    while (1) {
        /* read a new entry and fill key and value with it -Certus */
        retval = new_read_db_entry(&key, &value, dbptr->fptr);

        if (retval == DB_READ_ERROR) {
            alog(LOG_NORMAL, "WARNING! DB_READ_ERROR in %s",
                 dbptr->filename);
            new_close_db(dbptr->fptr, &key, &value);
            free(dbptr);
            return;
        } else if (retval == DB_EOF_ERROR) {
            alog(LOG_EXTRADEBUG, "debug: %s read successfully",
                 dbptr->filename);
            new_close_db(dbptr->fptr, &key, &value);
            free(dbptr);
            return;
        } else if (retval == DB_READ_BLOCKEND) {        /* DB_READ_BLOCKEND */
            /* a ctcp entry has completely been read. put any checks in here! */
        } else {
            /* DB_READ_SUCCESS */

            if (!*value || !*key)
                continue;

            if (!stricmp(key, "name")) {
                cs = makecs(value);
            } else if (!stricmp(key, "flags")) {
                cs->flags = atoi(value);
            } else if (!stricmp(key, "timeadded")) {
                cs->timeadded = atoi(value);
            }
        }                       /* else */
    }                           /* while */
}

/*************************************************************************/

void save_cs_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    ChannelStats *cs;
    lnode_t *tn;

    fill_db_ptr(dbptr, 0, CS_VERSION, s_StatServ, ChannelStatsDB);
    SET_SEGV_LOCATION();

    /* time to backup the old db */
    rename(ChannelStatsDB, dbptr->temp_name);

    if (new_open_db_write(dbptr)) {
        rename(dbptr->temp_name, ChannelStatsDB);
        free(dbptr);
        return;                 /* Bang, an error occurred */
    }

    tn = list_first(CStatshead);
    while (tn != NULL) {
        cs = lnode_get(tn);
        new_write_db_entry("name", dbptr, "%s", cs->name);
        new_write_db_entry("flags", dbptr, "%d", cs->flags);
        new_write_db_entry("timeadded", dbptr, "%ld", cs->timeadded);
        new_write_db_endofblock(dbptr);
        tn = list_next(CStatshead, tn);
    }

    SET_SEGV_LOCATION();

    if (dbptr) {
        new_close_db(dbptr->fptr, NULL, NULL);  /* close file */
        remove(dbptr->temp_name);       /* saved successfully, no need to keep the old one */
        free(dbptr);            /* free the db struct */
    }
}

/*****************************************************************************************/

void do_cstats(User * u, char *receiver, char *msg)
{
    char *command, *buf = NULL;
    ChannelStats *cs;

    SET_SEGV_LOCATION();

    send_event(EVENT_CHAN_PRIVMSG, 3, u->nick, receiver, msg);

    if (!(cs = find_cs(receiver))) {
        /* no error message if you place statserv in the logchan which is the same as
           services logchan it will throw fatal errors */
        return;
    }

    if (cs && cs->flags & CS_FANTASY) {
        buf = sstrdup(msg);
        command = strtok(buf, " ");
        SET_SEGV_LOCATION();

        if (command && (*command == *ChanStatsTrigger)) {
            char *params = strtok(NULL, "");
            /* Strip off the fantasy character */
            command++;
            if (params) {
                send_event(EVENT_FANTASY, 4, command, u->nick,
                           receiver, params);
            } else {
                send_event(EVENT_FANTASY, 3, command, u->nick, receiver);
            }
        }
    }
    SET_SEGV_LOCATION();
    make_stats(u, receiver, msg);
    free(buf);
}

/******************************************************************************************/

static void make_stats(User * u, char *receiver, char *msg)
{
    uint32 letters = 0, words = 1, action = 0, smileys = 0;
    int i = 0, hour;
    char *buf;
    Channel *c;

    SET_SEGV_LOCATION();

    if (!denora->do_sql) {
        return;
    }

    if (!u) {
        return;
    }

    if (!stricmp(u->nick, s_StatServ)) {
        return;
    }

    c = findchan(receiver);
    check_db(u, c);
    SET_SEGV_LOCATION();
    buf = normalizeBuffer(msg); /* remove control letters from message */
    if (BadPtr(buf)) {
        return;
    }
    letters = strlen(buf);      /* count letters */
    for (i = 0; i < letters; i++) {     /* count words */
        if ((buf[i] == ' ') && (buf[i + 1] != ' '))
            words++;
    }
    smileys = countsmileys(buf);        /* count smileys */
    free(buf);
    /* is a action ? */
    if (strncmp("\01ACTION", msg, 7) == 0) {    /* is a action ? */
        action = 1;
        letters = letters - 7;  /* remove from the counted letters */
        words--;                /* remove from the counted words */
    }
    words = words - smileys;    /* do not count smileys as words */
    SET_SEGV_LOCATION();
    hour = get_hour();

    /* update user SQL */
    /* update user */
    if (u->cstats != 2) {       /* check for ignore */
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET letters=letters+%u, words=words+%u, line=line+1, "
             "actions=actions+%u, smileys=smileys+%u, lastspoke=%ld, time%i=time%i+1 "
             "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
             UStatsTable, letters, words, action, smileys,
             (long int) time(NULL), hour, hour, u->sgroup, c->sqlchan);
    }
    SET_SEGV_LOCATION();

/* update chan */
    rdb_query
        (QUERY_LOW,
         "UPDATE %s SET letters=letters+%u, words=words+%u, line=line+1, "
         "actions=actions+%u, smileys=smileys+%u, lastspoke=%ld, time%i=time%i+1 WHERE chan=\'%s\';",
         CStatsTable, letters, words, action, smileys,
         (long int) time(NULL), hour, hour, c->sqlchan);
}

/*************************************************************************/

void count_kicks(User * kicker, User * kicked, Channel * c)
{
#ifdef USE_MYSQL
    if (!c) {
        return;
    }
    if (!find_cs(c->name)) {
        return;
    }

    SET_SEGV_LOCATION();

    if (kicker && !is_excluded(kicker)) {
        check_db(kicker, c);
        if (kicker->cstats != 2) {      /* check for ignore */
            rdb_query
                (QUERY_LOW, "UPDATE %s SET kicks=kicks+1 "
                 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
                 UStatsTable, kicker->sgroup, c->sqlchan);
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET kicks=kicks+1 WHERE chan=\'%s\';",
                 CStatsTable, c->sqlchan);
        }
    }

    SET_SEGV_LOCATION();

    if (kicked && !is_excluded(kicked)) {
        check_db(kicked, c);
        if (kicked->cstats != 2) {      /* check for ignore */
            rdb_query
                (QUERY_LOW, "UPDATE %s SET kicked=kicked+1 "
                 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
                 UStatsTable, kicked->sgroup, c->sqlchan);
        }
    }
#endif
}

/*************************************************************************/

void count_topics(User * u, Channel * c)
{
    if (!denora->do_sql) {
        return;
    }

    if (!find_cs(c->name)) {
        return;
    }
    SET_SEGV_LOCATION();

    /*
     * need to exclude them from being counted if it is from a user whom is to be excluded
     */
    if (is_excluded(u)) {
        return;
    }

    check_db(u, c);

    SET_SEGV_LOCATION();

    if (u->cstats != 2) {       /* check for ignore */
        rdb_query
            (QUERY_LOW, "UPDATE %s SET topics=topics+1 "
             "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
             UStatsTable, u->sgroup, c->sqlchan);
    }
    rdb_query
        (QUERY_LOW, "UPDATE %s SET topics=topics+1 WHERE chan=\'%s\';",
         CStatsTable, c->sqlchan);
    SET_SEGV_LOCATION();
}

/*************************************************************************/

void count_modes(User * u, Channel * c)
{
    if (!denora->do_sql) {
        return;
    }

    if (!find_cs(c->name)) {
        return;
    }
    SET_SEGV_LOCATION();

    /*
     * need to exclude them from being counted if it is from a user whom is to be excluded
     */
    if (is_excluded(u)) {
        return;
    }

    check_db(u, c);
    SET_SEGV_LOCATION();
    if (u->cstats != 2) {       /* check for ignore */
        rdb_query
            (QUERY_LOW, "UPDATE %s SET modes=modes+1 "
             "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
             UStatsTable, u->sgroup, c->sqlchan);
    }
    rdb_query
        (QUERY_LOW, "UPDATE %s SET modes=modes+1 WHERE chan=\'%s\';",
         CStatsTable, c->sqlchan);
    SET_SEGV_LOCATION();
}

/*************************************************************************/

static int check_db(User * u, Channel * c)
{
#ifdef USE_MYSQL
    int i;
    MYSQL_RES *mysql_res;
#endif
    SET_SEGV_LOCATION();
#ifndef USE_MYSQL
    return 0;
#else

    /* Check if user has +r */
    if (UserStatsRegistered == 1) {
        if (!UserHasMode(u->nick, UMODE_r)) {
            /* User is not +r so he gets ignored */
            u->cstats = 2;
        } else if (UserHasMode(u->nick, UMODE_r) && (u->cstats == 2)) {
            /* User is +r but is set to ignore, so reset. */
            u->cstats = 0;
        }
    }

    /* get alias from db */
    if (u->cstats == 0) {
        rdb_query(QUERY_HIGH,
                  "SELECT uname,`ignore` FROM %s WHERE uname=\'%s\';",
                  AliasesTable, u->sqlnick);
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) > 0) {        /* exists this nick already in the database? */
                mysql_row = mysql_fetch_row(mysql_res);
                if (u->sgroup) {
                    free(u->sgroup);
                }
                u->sgroup = rdb_escape(mysql_row[0]);
                if (!stricmp(mysql_row[1], "Y")) {
                    u->cstats = 2;
                } else {
                    u->cstats = 1;
                }
            } else {            /* num_rows = 0 */
                /* create alias and global */
                rdb_query(QUERY_LOW,
                          "INSERT INTO %s (nick, uname) VALUES (\'%s\', \'%s\') ON DUPLICATE KEY UPDATE uname=\'%s\'",
                          AliasesTable, u->sqlnick, u->sqlnick,
                          u->sqlnick);
                for (i = 0; i < 4; i++) {
                    rdb_query
                        (QUERY_LOW,
                         "INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'global\', type=%i;",
                         UStatsTable, u->sqlnick, i);
                }
                u->cstats = 1;
                free(u->sgroup);
                u->sgroup = sstrdup(u->sqlnick);
            }
            mysql_free_result(mysql_res);
        }
    }

    SET_SEGV_LOCATION();
    if (c->cstats == 0) {
        SET_SEGV_LOCATION();
        rdb_query(QUERY_HIGH,
                  "SELECT chan FROM %s WHERE chan=\'%s\' AND type=0;",
                  CStatsTable, c->sqlchan);
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) > 0) {        /* exists this chan already in the database? */
                c->cstats = 1;
            } else {            /* num_rows = 0 */
                /* create total, daily, weekly, monthly */
                for (i = 0; i < 4; i++) {
                    rdb_query(QUERY_LOW,
                              "INSERT INTO %s SET chan=\'%s\', type=%d;",
                              CStatsTable, c->sqlchan, i);
                }
                c->cstats = 1;
            }                   /* num_rows */
            mysql_free_result(mysql_res);
        }
    }                           /* c->cstats */
    SET_SEGV_LOCATION();
    if (u->cstats != 2) {
        rdb_query
            (QUERY_HIGH,
             "SELECT uname FROM %s WHERE uname=\'%s\' AND chan=\'%s\';",
             UStatsTable, u->sgroup, c->sqlchan);
        mysql_res = mysql_store_result(mysql);
        if (mysql_res) {
            if (mysql_num_rows(mysql_res) == 0) {
                /* create entries for total, daily, weekly, monthly, */
                for (i = 0; i < 4; i++) {
                    rdb_query
                        (QUERY_LOW,
                         "INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'%s\', type=%d, firstadded=%ld;",
                         UStatsTable, u->sgroup, c->sqlchan, i,
                         (long int) time(NULL));
                }
            }
            mysql_free_result(mysql_res);
        }
    }
    SET_SEGV_LOCATION();
    return 1;
#endif
}

/*************************************************************************/

/* transfer all stats from user2 to user1 and delete user2 */

void sumuser(User * u, char *user1, char *user2)
{
    User *u2;
#ifdef USE_MYSQL
    int i;
    MYSQL_RES *mysql_res_chans;
    MYSQL_RES *mysql_res;
    MYSQL_ROW mysql_row_chans;
    char *chan_;
#endif
    char *user1_, *user2_;

    if (!denora->do_sql) {
        return;
    }

    user1_ = rdb_escape(user1);
    user2_ = rdb_escape(user2);
    SET_SEGV_LOCATION();

    rdb_query
        (QUERY_HIGH,
         "SELECT uname FROM %s WHERE uname = \'%s\';",
         UStatsTable, user1_);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {   /* exists this nick already in the database? */
            mysql_free_result(mysql_res);
            notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user1);
            goto end;
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();
    rdb_query
        (QUERY_HIGH,
         "SELECT uname FROM %s WHERE uname = \'%s\';",
         UStatsTable, user2_);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_num_rows(mysql_res) == 0) {       /* exists this nick already in the database? */
        mysql_free_result(mysql_res);
        notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user2);
        goto end;
    }
    mysql_free_result(mysql_res);
#else
    goto end;
#endif
    /* get all chans of user2 */
    rdb_query(QUERY_HIGH,
              "SELECT chan FROM %s WHERE uname = \'%s\' AND type = 0;",
              UStatsTable, user2_);
#ifdef USE_MYSQL
    mysql_res_chans = mysql_store_result(mysql);
    /* for each chan: */
    /* read total, daily, weekly and  monthly stats and add them to user1 */
    while ((mysql_row_chans = mysql_fetch_row(mysql_res_chans)) != NULL) {
        chan_ = rdb_escape(mysql_row_chans[0]);
        /* read total, daily, weekly and  monthly stats and add them to user1 */
        for (i = 0; i < 4; i++) {
            rdb_query
                (QUERY_HIGH,
                 "SELECT letters, words,line,actions,smileys,kicks,kicked,modes,topics,wasted,time0,time1,time2,time3,time4,time5,time6,time7,time8,time9,time10,time11,time12,time13,time14,time15,time16,time17,time18,time19,time20,time21,time22,time23 FROM %s WHERE uname = \'%s\' AND chan = \'%s\' AND type = %i;",
                 UStatsTable, user2_, chan_, i);
            mysql_res = mysql_store_result(mysql);
            mysql_row = mysql_fetch_row(mysql_res);
            rdb_query(QUERY_LOW, "UPDATE %s SET letters=letters+%s, words=words+%s, line=line+%s, actions=actions+%s, smileys=smileys+%s, kicks=kicks+%s, kicked=kicked+%s, modes=modes+%s, topics=topics+%s, wasted=wasted+%s WHERE uname=\'%s\' AND chan=\'%s\' AND type=%i;", UStatsTable, mysql_row[0],     /* letters */
                      mysql_row[1],     /* words */
                      mysql_row[2],     /* lines */
                      mysql_row[3],     /* actions */
                      mysql_row[4],     /* smileys */
                      mysql_row[5],     /* kicks */
                      mysql_row[6],     /* kicked */
                      mysql_row[7],     /* modes */
                      mysql_row[8],     /* topics */
                      mysql_row[9],     /* wasted */
                      user1_, chan_, i);
            rdb_query(QUERY_LOW, "UPDATE %s SET time0=time0+%s, time1=time1+%s, time2=time2+%s, time3=time3+%s, time4=time4+%s, time5=time5+%s, time6=time6+%s, time7=time7+%s, time8=time8+%s, time9=time9+%s, time10=time10+%s, time11=time11+%s, time12=time12+%s, time13=time13+%s, time14=time14+%s, time15=time15+%s, time16=time16+%s, time17=time17+%s, time18=time18+%s, time19=time19+%s, time20=time20+%s, time21=time21+%s, time22=time22+%s, time23=time23+%s WHERE uname=\'%s\' AND chan=\'%s\' AND type=%i;", UStatsTable, mysql_row[10],        /* time0 */
                      mysql_row[11],    /* time1 */
                      mysql_row[12],    /* time2 */
                      mysql_row[13],    /* time3 */
                      mysql_row[14],    /* time4 */
                      mysql_row[15],    /* time5 */
                      mysql_row[16],    /* time6 */
                      mysql_row[17],    /* time7 */
                      mysql_row[18],    /* time8 */
                      mysql_row[19],    /* time9 */
                      mysql_row[20],    /* time10 */
                      mysql_row[21],    /* time11 */
                      mysql_row[22],    /* time12 */
                      mysql_row[23],    /* time13 */
                      mysql_row[24],    /* time14 */
                      mysql_row[25],    /* time15 */
                      mysql_row[26],    /* time16 */
                      mysql_row[27],    /* time17 */
                      mysql_row[28],    /* time18 */
                      mysql_row[29],    /* time19 */
                      mysql_row[30],    /* time20 */
                      mysql_row[31],    /* time21 */
                      mysql_row[32],    /* time22 */
                      mysql_row[33],    /* time23 */
                      user1_, chan_, i);
            mysql_free_result(mysql_res);
        }
        free(chan_);
    }                           /* while */
    SET_SEGV_LOCATION();

    mysql_free_result(mysql_res_chans);
#endif
    /* delete stats of user2 from ustats */
    rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname = \'%s\';",
              UStatsTable, user2_);
    /* change aliases */
    rdb_query(QUERY_LOW,
              "UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
              AliasesTable, user1_, user2_);
    while ((u2 = finduser_by_sgroup(user2, user2_))) {
        free(u2->sgroup);
        u2->sgroup = sstrdup(user1_);
    }
    notice_lang(s_StatServ, u, STATS_CHANSTATS_SUMUSER_DONE, user2, user1);
    alog(LOG_NORMAL, "Merged stats user %s with %s", user2, user1);

  end:
    SET_SEGV_LOCATION();
    free(user1_);
    free(user2_);
}

/*************************************************************************/

/* rename stats user1 to user2 */

void renameuser(User * u, char *user1, char *user2)
{
    User *u2;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    char *user1_, *user2_;

    if (!denora->do_sql) {
        return;
    }

    user1_ = rdb_escape(user1);
    user2_ = rdb_escape(user2);
    SET_SEGV_LOCATION();

    rdb_query
        (QUERY_HIGH,
         "SELECT uname FROM %s WHERE uname = \'%s\';",
         UStatsTable, user1_);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_res) {
        if (mysql_num_rows(mysql_res) == 0) {   /* exists this nick already in the database? */
            mysql_free_result(mysql_res);
            notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user1);
            goto end;
        }
        mysql_free_result(mysql_res);
    }
#endif
    SET_SEGV_LOCATION();
    rdb_query
        (QUERY_HIGH,
         "SELECT uname FROM %s WHERE uname = \'%s\';",
         UStatsTable, user2_);
#ifdef USE_MYSQL
    mysql_res = mysql_store_result(mysql);
    if (mysql_num_rows(mysql_res) != 0) {       /* exists this nick already in the database? */
        mysql_free_result(mysql_res);
        notice_lang(s_StatServ, u, STATS_RENAMEUSER_EXISTING, user2);
        goto end;
    }
    mysql_free_result(mysql_res);
#else
    goto end;
#endif

    /* renames user1 to user2 */
    rdb_query(QUERY_HIGH,
              "UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
              UStatsTable, user2_, user1_);
    /* updates aliases */
    rdb_query(QUERY_HIGH,
              "UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
              AliasesTable, user2_, user1_);
    while ((u2 = finduser_by_sgroup(user1, user1_))) {
        free(u2->sgroup);
        u2->sgroup = sstrdup(user2_);
    }
    notice_lang(s_StatServ, u, STATS_CHANSTATS_RENAMEUSER_DONE, user1,
                user2);

  end:
    SET_SEGV_LOCATION();
    free(user1_);
    free(user2_);
}

/*************************************************************************/

/* Find a user by nick.  Return NULL if user could not be found. */

User *finduser_by_sgroup(char *nick, char *sgroup)
{
    User *user;

    SET_SEGV_LOCATION();

    if (BadPtr(sgroup)) {
        alog(LOG_DEBUG, langstr(ALOG_DEBUG_BAD_FINDSGROUP));
        return NULL;
    }

    user = userlist[NICKHASH(nick)];
    while (user && user->sgroup && (stricmp(user->sgroup, sgroup) != 0))
        user = user->next;
    return user;
}

/*************************************************************************/

static uint32 countsmileys(char *text)
{
    char buf[1024], *pbuf, *smiley, *p;
    uint32 ismileys = 0;

    ircsnprintf(buf, sizeof(buf), "%s", Smiley);
    pbuf = buf;
    SET_SEGV_LOCATION();

    while (strlen(pbuf) > 0) {
        smiley = newsplit(&pbuf);
        p = strstr(text, smiley);
        while (p) {
            ismileys++;
            p += strlen(smiley);
            p = strstr(p, smiley);
        }
    }
    SET_SEGV_LOCATION();
    return ismileys;
}

/*************************************************************************/

static char *newsplit(char **rest)
{
    register char *o, *r;

    SET_SEGV_LOCATION();

    if (!rest)
        return *rest = NULL;
    o = *rest;
    while (*o == ' ')
        o++;
    r = o;
    while (*o && (*o != ' '))
        o++;
    if (*o)
        *o++ = 0;
    *rest = o;
    return r;
}

/*************************************************************************/

int get_hour(void)
{
    time_t now;
    static struct tm t;

    SET_SEGV_LOCATION();
    now = time(NULL);
#ifdef MSVS2005
    localtime_s(&t, &now);
#else
    t = *localtime(&now);
#endif
    return t.tm_hour;
}

/*************************************************************************/
