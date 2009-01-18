/*
 *
 * (c) 2004-2009 Denora Team
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

Exclude *exlists[1024];
static Exclude *current;
static int next_index;

/*************************************************************************/

void insert_exclude(Exclude * e)
{
    int exindex = EXCLUDEHASH(e->name);

    e->prev = NULL;
    e->next = exlists[exindex];
    if (e->next)
        e->next->prev = e;
    exlists[exindex] = e;
}

/*************************************************************************/

Exclude *find_exclude(char *mask, char *server)
{
    Exclude *e;

    SET_SEGV_LOCATION();

    if (!mask)
        return NULL;
    for (e = exlists[EXCLUDEHASH(mask)]; e; e = e->next) {
        if (stricmp(e->name, mask) == 0) {
            return e;
        }
        if (NumExcludeServers && server) {
            if (isExcludedServer(server)) {
                return e;
            }
        }
    }
    return NULL;
}

/*************************************************************************/

DENORA_INLINE boolean is_excluded(User * u)
{
    Exclude *e;
    e = find_exclude(u->nick, u->server->name);
    return (e ? true : false);
}

/*************************************************************************/

DENORA_INLINE boolean is_excludedserv(Server * server)
{
    if (server && NumExcludeServers && isExcludedServer(server->name)) {
        return true;
    }

    return false;
}

/*************************************************************************/

DENORA_INLINE int isExcludedServer(char *name)
{
    int j;

    for (j = 0; j < NumExcludeServers; j++) {
        if (stricmp(ExcludeServers[j], name) == 0) {
            return 1;
        }
    }
    return 0;
}

/*************************************************************************/

/* Load/save data files. */

void load_exclude_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    Exclude *e = NULL;
    char *key, *value;
    int retval = 0;

    fill_db_ptr(dbptr, 0, EXCLUDE_VERSION, s_StatServ, excludeDB);

    /* let's remove existing temp files here, because we only load dbs on startup */
    remove(dbptr->temp_name);

    /* Open the db, fill the rest of dbptr and allocate memory for key and value */
    if (new_open_db_read(dbptr, &key, &value)) {
        free(dbptr);
        return;                 /* Bang, an error occurred */
    }

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
            /* a exclude entry has completely been read. put any checks in here! */
        } else {
            /* DB_READ_SUCCESS */

            if (!*value || !*key)
                continue;

            if (!stricmp(key, "name")) {
                e = make_exclude(value);
            } else if (!stricmp(key, "flag")) {
                e->flag = atoi(value);
                if (!e->flag) {
                    e->flag |= EXCLUDE_USER;
                }
            }
        }                       /* else */
    }                           /* while */
}

/*************************************************************************/

Exclude *make_exclude(char *mask)
{
    Exclude *e;
    User *u;

    e = calloc(sizeof(Exclude), 1);
    e->name = sstrdup(mask);
    e->flag = 1;
    insert_exclude(e);

    u = user_find(mask);
    if (u) {
        u->cstats = 0;
    }
    return e;
}

/*************************************************************************/

void save_exclude_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    Exclude *e;
    int i;

    fill_db_ptr(dbptr, 0, EXCLUDE_VERSION, s_StatServ, excludeDB);

    /* time to backup the old db */
    rename(excludeDB, dbptr->temp_name);

    if (new_open_db_write(dbptr)) {
        rename(dbptr->temp_name, excludeDB);
        free(dbptr);
        return;                 /* Bang, an error occurred */
    }

    for (i = 0; i < 1024; i++) {
        for (e = exlists[i]; e; e = e->next) {
            new_write_db_entry("name", dbptr, "%s", e->name);
            new_write_db_entry("flag", dbptr, "%d", e->flag);
            new_write_db_endofblock(dbptr);
        }
    }

    if (dbptr) {
        new_close_db(dbptr->fptr, NULL, NULL);  /* close file */
        remove(dbptr->temp_name);       /* saved successfully, no need to keep the old one */
        free(dbptr);            /* free the db struct */
    }
}

/*************************************************************************/

int del_exclude(Exclude * e)
{
    SET_SEGV_LOCATION();

    SET_SEGV_LOCATION();

    if (e) {
        if (e->prev) {
            e->prev->next = e->next;
        } else {
            exlists[EXCLUDEHASH(e->name)] = e->next;
        }
        if (e->next) {
            e->next->prev = e->prev;
        }
        if (e->name) {
            SET_SEGV_LOCATION();
            free(e->name);
        }
        free(e);
        return 1;
    }
    return 0;
}

/*************************************************************************/

Exclude *first_exclude(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL) {
        current = exlists[next_index++];
    }
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: first_exclude() returning %s",
         current ? current->name : "NULL (end of list)");
    return current;
}

/*************************************************************************/

Exclude *next_exclude(void)
{
    SET_SEGV_LOCATION();

    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL) {
            current = exlists[next_index++];
        }
    }
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: next_exclude() returning %s",
         current ? current->name : "NULL (end of list)");
    return current;
}
