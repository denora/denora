
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

Server *servlist = NULL;
Server *me_server = NULL;
char *uplink;
char *TS6SID;
char p10id[3];
char *TS6UPLINK;
Server *serverlist[1024];
ServStats *servstatlist[1024];
static ServStats *currentss;
static int next_index;
static Server *server_cur;
Server *server_ping(Server * s);
Server *server_uptime(Server * s);
Server *make_server(const char *servername, char *descript,
                    Server * servuplink, char *numeric);
int find_server_link(Server * s, const char *servname);
int UplinkSynced;

/*************************************************************************/

CapabInfo capab_info[] = {
    {"NOQUIT", CAPAB_NOQUIT},
    {"TSMODE", CAPAB_TSMODE},
    {"UNCONNECT", CAPAB_UNCONNECT},
    {"NICKIP", CAPAB_NICKIP},
    {"SSJOIN", CAPAB_NSJOIN},
    {"ZIP", CAPAB_ZIP},
    {"BURST", CAPAB_BURST},
    {"TS5", CAPAB_TS5},
    {"TS3", CAPAB_TS3},
    {"DKEY", CAPAB_DKEY},
    {"PT4", CAPAB_PT4},
    {"SCS", CAPAB_SCS},
    {"QS", CAPAB_QS},
    {"UID", CAPAB_UID},
    {"KNOCK", CAPAB_KNOCK},
    {"CLIENT", CAPAB_CLIENT},
    {"IPV6", CAPAB_IPV6},
    {"SSJ5", CAPAB_SSJ5},
    {"SN2", CAPAB_SN2},
    {"TOK1", CAPAB_TOKEN},
    {"TOKEN", CAPAB_TOKEN},
    {"VHOST", CAPAB_VHOST},
    {"SSJ3", CAPAB_SSJ3},
    {"SJB64", CAPAB_SJB64},
    {"CHANMODES", CAPAB_CHANMODE},
    {"NICKCHARS", CAPAB_NICKCHARS},
    {NULL, 0}
};

/*************************************************************************/

void update_sync_state(char *source, int ac)
{
    Server *s;
    if (!BadPtr(source)) {
        s = server_find(source);
    } else {
        s = server_find(denora->uplink);
    }
    if (s) {
        if (ac == SYNC_COMPLETE) {
            s->sync = 1;
            if (!stricmp(denora->uplink, s->name)) {
                send_event(EVENT_UPLINK_SYNC_COMPLETE, 1, s->name);
                UplinkSynced = 1;
            } else {
                send_event(EVENT_SERVER_SYNC_COMPLETE, 1, s->name);
            }
        }
    }
}

/*************************************************************************/

void load_server_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    ServStats *ss = NULL;
    char *key, *value;
    int retval = 0;

    fill_db_ptr(dbptr, 0, SERVER_VERSION, s_StatServ, ServerDB);

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
            alog(LOG_NORMAL, langstr(ALOG_DB_ERROR), dbptr->filename);
            new_close_db(dbptr->fptr, &key, &value);
            free(dbptr);
            return;
        } else if (retval == DB_EOF_ERROR) {
            alog(LOG_EXTRADEBUG, langstr(ALOG_DEBUG_DB_OK),
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
                ss = make_servstats(value);
            } else if (!stricmp(key, "maxusers")) {
                ss->maxusers = atoi(value);
            } else if (!stricmp(key, "maxusertime")) {
                ss->maxusertime = atoi(value);
            } else if (!stricmp(key, "maxopers")) {
                ss->maxopers = atoi(value);
            } else if (!stricmp(key, "highestping")) {
                ss->highestping = atoi(value);
            } else if (!stricmp(key, "maxpingtime")) {
                ss->maxpingtime = atoi(value);
            } else if (!stricmp(key, "totalusersever")) {
                ss->totalusersever = atoi(value);
            } else if (!stricmp(key, "splits")) {
                ss->splits = atoi(value);
            } else if (!stricmp(key, "split_stats")) {
                ss->split_stats = 1;
            } else if (!stricmp(key, "lastseen")) {
                ss->lastseen = atoi(value);
            } else if (!stricmp(key, "ircopskills")) {
                ss->ircopskills = atoi(value);
            } else if (!stricmp(key, "serverkills")) {
                ss->serverkills = atoi(value);
            } else if (!stricmp(key, "maxopertime")) {
                ss->maxopertime = atoi(value);
            }


        }                       /* else */
    }                           /* while */
}

/*************************************************************************/

void save_server_db(void)
{
    DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
    ServStats *ss;
    int i;

    fill_db_ptr(dbptr, 0, SERVER_VERSION, s_StatServ, ServerDB);

    /* time to backup the old db */
    rename(ServerDB, dbptr->temp_name);

    if (new_open_db_write(dbptr)) {
        rename(dbptr->temp_name, ServerDB);
        free(dbptr);
        return;                 /* Bang, an error occurred */
    }

    for (i = 0; i < 1024; i++) {
        for (ss = servstatlist[i]; ss; ss = ss->next) {
            new_write_db_entry("name", dbptr, "%s", ss->name);
            new_write_db_entry("maxusers", dbptr, "%d", ss->maxusers);
            new_write_db_entry("maxopers", dbptr, "%d", ss->maxopers);
            new_write_db_entry("highestping", dbptr, "%d",
                               ss->highestping);
            new_write_db_entry("maxpingtime", dbptr, "%d",
                               ss->maxpingtime);
            new_write_db_entry("totalusersever", dbptr, "%ld",
                               ss->totalusersever);
            new_write_db_entry("splits", dbptr, "%d", ss->splits);
            new_write_db_entry("split_stats", dbptr, "%ld",
                               ss->split_stats);
            new_write_db_entry("lastseen", dbptr, "%d", ss->lastseen);
            new_write_db_entry("maxusertime", dbptr, "%ld",
                               ss->maxusertime);
            new_write_db_entry("ircopskills", dbptr, "%ld",
                               ss->ircopskills);
            new_write_db_entry("serverkills", dbptr, "%ld",
                               ss->serverkills);
            new_write_db_entry("maxopertime", dbptr, "%ld",
                               ss->maxopertime);
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

void sql_motd_store(Server * s)
{
    char *string, *source;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }
    if (!denora->do_sql) {
        return;
    }
    source = rdb_escape(s->name);
    string = rdb_escape(s->motd);

    rdb_query(QUERY_LOW,
              "UPDATE %s SET motd=\'%s\' WHERE server=\'%s\'",
              ServerTable, string, source);
    free(source);
    free(string);
}

/*************************************************************************/

/**
 * This function makes a new Server structure 
 * @param server Server Name
 * @return Server Struct
 */
ServStats *make_servstats(const char *server)
{
    ServStats *s, **list;

    SET_SEGV_LOCATION();
    s = calloc(sizeof(ServStats), 1);
    if (!server) {
        server = "";
    }
    strlcpy(s->name, server, SERVERMAX);
    list = &servstatlist[STATSSERVERHASH(s->name)];
    s->next = *list;
    if (*list)
        (*list)->prev = s;
    *list = s;
    s->currentusers = 0;
    return s;
}

/*************************************************************************/

/**
 * This function makes a new Server structure 
 * @param servernam Server Name
 * @param descript is the server description
 * @param servuplink is the server struct for the uplink
 * @param numeric is the server numeric
 * @return Server Struct
 */
Server *make_server(const char *servername, char *descript,
                    Server * servuplink, char *numeric)
{
    Server *serv;
    ServStats *ss;

    serv = calloc(sizeof(Server), 1);
    serv->name = sstrdup(servername);
    if (!BadPtr(descript)) {
        serv->desc = sstrdup(descript);
    }
    serv->uplink = servuplink;
    if (numeric) {
        serv->suid = sstrdup(numeric);
    } else {
        serv->suid = NULL;
    }
    serv->sync = -1;
    serv->links = NULL;
    serv->prev = NULL;

    if (!servuplink) {
        serv->hops = 0;
        serv->next = servlist;
        if (servlist) {
            servlist->prev = serv;
        }
        servlist = serv;
    } else {
        serv->hops = servuplink->hops + 1;
        serv->next = servuplink->links;
        if (servuplink->links) {
            servuplink->links->prev = serv;
        }
        servuplink->links = serv;
    }
    ss = findserverstats(servername);
    SET_SEGV_LOCATION();
    if (!ss) {
        serv->ss = make_servstats(servername);
    } else {
        serv->ss = ss;
        serv->ss->split_stats = 0;
    }
    if (!serv->ss->maxusertime) {
        serv->ss->maxusertime = time(NULL);
    }
    SET_SEGV_LOCATION();

    return serv;
}

/*************************************************************************/

Server *do_server(const char *source, char *servername, char *hops,
                  char *descript, char *numeric)
{
    char *uplinkserver;
    char *sqluplinkserver;
    Server *serv;
    char buf[BUFSIZ];
    char mbuf[NET_BUFSIZE];
    Server *servuplink;
    int servid;
    int add = 1;
    uint32 timenow, uptime;
    FILE *f;
    int juped = 0;
    int upservid = 0;

    *buf = '\0';

    SET_SEGV_LOCATION();

    if ((!source || !*source)) {
        alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO), servername);
        if (stricmp(servername, ServerName)) {
            servuplink = NULL;
            uplinkserver = NULL;
        } else {
            uplinkserver = sstrdup(ServerName);
            servuplink = me_server;
        }
    } else {
        servuplink = server_find(source);
        if (!servuplink) {
            uplinkserver = sstrdup(ServerName);
            servuplink = me_server;
            alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO), servername);
        } else {
            uplinkserver = sstrdup(servuplink->name);
            alog(LOG_DEBUG, langstr(ALOG_DEBUG_SERVINTRO_FORM), servername,
                 servuplink->name);
        }
    }

    if (servuplink) {
        ARRAY_EXTEND(servuplink->slinks);
        servuplink->slinks[servuplink->slinks_count - 1] =
            sstrdup(servername);
    }

    SET_SEGV_LOCATION();
    serv = make_server(servername, descript, servuplink, numeric);
    SET_SEGV_LOCATION();

    if (servuplink && JupeMaster) {
        if (!stricmp(servuplink->name, JupeMaster)) {
            serv->flags = SERVER_JUPED;
            juped = 1;
        }
    }

    if (!juped && stricmp(servername, ServerName) && !LargeNet) {
        denora_cmd_version(servername);
        denora_cmd_stats(s_StatServ, "u", servername);
        if (ircd->uline) {
            denora_cmd_stats(s_StatServ, "U", servername);
        }
        if (ircd->spamfilter) {
            denora_cmd_stats(s_StatServ, "f", servername);
        }
        denora_motd(s_StatServ, servername);
    }
    stats->servers++;
    SET_SEGV_LOCATION();

    if (juped) {
        send_event(EVENT_JUPED_SERVER, 1, servername);
    } else {
        send_event(EVENT_SERVER, 1, servername);
    }

    SET_SEGV_LOCATION();
    if (denora->do_sql) {
        servername = rdb_escape(servername);
        descript = rdb_escape(descript);
        sqluplinkserver = rdb_escape(uplinkserver);
        upservid = db_getserver(sqluplinkserver);

        if (ServerCacheTime
            && ((servid = db_checkserver(servername)) != -1)) {
            /* removed resetting connecttime - denora now assumes you've been connected same as 
               it last saw the server if its not flagged as offline
             */
            if (db_checkserver_online(servername)) {
                rdb_query
                    (QUERY_LOW,
                     "UPDATE %s SET server=\'%s\', hops=\'%s\', comment=\'%s\', linkedto=%d, online=\'Y\', maxusers=%d, maxusertime=%d, lastsplit=FROM_UNIXTIME(%ld) WHERE servid=%d",
                     ServerTable, servername, hops, descript, upservid,
                     serv->ss->maxusers, serv->ss->maxusertime,
                     serv->ss->lastseen, servid);
            } else {
                rdb_query
                    (QUERY_LOW,
                     "UPDATE %s SET server=\'%s\', hops=\'%s\', comment=\'%s\', connecttime=NOW(), linkedto=%d, online=\'Y\', maxusers=%d, maxusertime=%d, lastsplit=FROM_UNIXTIME(%ld) WHERE servid=%d",
                     ServerTable, servername, hops, descript, upservid,
                     serv->ss->maxusers, serv->ss->maxusertime,
                     serv->ss->lastseen, servid);
            }
            add = 0;
        }
        if (add) {
            if (KeepServerTable) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (server, hops, comment, linkedto, connecttime, maxusers, maxusertime, lastsplit) VALUES(\'%s\',\'%s\',\'%s\',%d, NOW(), %d, %d, FROM_UNIXTIME(%ld))  ON DUPLICATE KEY UPDATE hops=\'%s\', comment=\'%s\', linkedto=%d, connecttime=NOW(), maxusers=%d, maxusertime=%d, lastsplit=FROM_UNIXTIME(%ld)",
                     ServerTable, servername, hops, descript, upservid,
                     serv->ss->maxusers, serv->ss->maxusertime,
                     serv->ss->lastseen, hops, descript,
                     db_getserver(sqluplinkserver), serv->ss->maxusers,
                     serv->ss->maxusertime, serv->ss->lastseen);
            } else {
                rdb_query
                    (QUERY_LOW,
                     "INSERT INTO %s (server, hops, comment, linkedto, connecttime, maxusers, maxusertime, lastsplit) VALUES(\'%s\',\'%s\',\'%s\',%d, NOW(), %d, %d, FROM_UNIXTIME(%ld))",
                     ServerTable, servername, hops, descript, upservid,
                     serv->ss->maxusers, serv->ss->maxusertime,
                     serv->ss->lastseen);
            }
        }
        free(descript);
        free(sqluplinkserver);
        servid = db_getserver(servername);
        serv->sqlid = servid;

        if (!stricmp(servername, ServerName)) {
            timenow = (long int) time(NULL);
            uptime = timenow - denora->start_time;
            *buf = '\0';
            ircsnprintf(buf, sizeof(buf), "Denora-%s", denora->version);
            serv->version = sstrdup(buf);

            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET uptime=%ld, version=\'%s\' WHERE servid=%d",
                 ServerTable, uptime, serv->version, servid);
            if (MOTDFilename) {
                if ((f = FileOpen(MOTDFilename, FILE_READ)) != NULL) {
                    while (fgets(buf, BUFSIZE - 1, f)) {
                        buf[strlen(buf) - 1] = 0;
                        if (serv->motd) {
                            ircsnprintf(mbuf, NET_BUFSIZE - 1, "%s\n\r%s",
                                        serv->motd, buf);
                            free(serv->motd);
                            serv->motd = sstrdup(mbuf);
                        } else {
                            serv->motd = sstrdup(buf);
                        }
                    }
                    fclose(f);
                    sql_motd_store(serv);
                }
            }
        }
        free(servername);
    }

    SET_SEGV_LOCATION();
    if (ServerCacheTime) {
        db_cleanserver();
    }
    if (uplinkserver) {
        free(uplinkserver);
    }
    SET_SEGV_LOCATION();
    do_checkservsmax();
    return serv;
}

/*************************************************************************/

void sql_uline(char *server)
{
    int id;
    Server *s;

    /* On Ratbox - ulines are also shared so you can
       get *.your.net at times so we ignore any server
       that starts with * */
    if (*server != '*') {
        id = db_getserver(server);
        if (id) {
            rdb_query(QUERY_LOW, "UPDATE %s SET uline=1 WHERE servid=%d",
                      ServerTable, id);
        }
        s = server_find(server);
        if (s) {
            s->uline = 1;
        }
    }
}

/*************************************************************************/

void server_store_pong(char *source, uint32 ts)
{
    Server *s;
    char *serv;
    int servid = 0;

    if (LargeNet) {
        return;
    }

    s = server_find(source);
    if (s) {
        s->ping = ts - s->lastping;
        alog(LOG_DEBUG, langstr(ALOG_DEBUG_PINGTIME), source, s->ping, ts,
             s->lastping);
        if (s->ping >= s->ss->highestping) {
            s->ss->highestping = s->ping;
            s->ss->maxpingtime = time(NULL);
        }
        if (denora->do_sql) {
            serv = rdb_escape(source);
            servid = db_getserver(serv);
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET ping=%d, highestping=%d, maxpingtime=%ld, lastpingtime=%ld WHERE servid=%d",
                 ServerTable, s->ping, s->ss->highestping,
                 (long int) s->ss->maxpingtime, s->lastping, servid);
            free(serv);
        }
    }
}

/*************************************************************************/

void ping_servers(void)
{
    Server *serv;

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }
    serv = server_find(denora->uplink);

    if (serv) {
        server_ping(serv);
    }
}

/*************************************************************************/

Server *server_ping(Server * s)
{
    Server *sl;
    int i;

    if (s) {
        if (s->slinks_count) {
            for (i = 0; i <= s->slinks_count - 1; i++) {
                sl = server_find(s->slinks[i]);
                server_ping(sl);
            }
        }
        if (stricmp(ServerName, s->name)) {
            if (s->flags != SERVER_JUPED) {
                denora_cmd_ping(s->name);
                s->lastping = time(NULL);
            } else {
                s->lastping = 0;
            }
        }
        return s;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Server *server_uptime(Server * s)
{
    Server *sl;
    int i;

    if (s) {
        if (s->slinks_count) {
            for (i = 0; i <= s->slinks_count - 1; i++) {
                sl = server_find(s->slinks[i]);
                server_uptime(sl);
            }
        }
        if (stricmp(ServerName, s->name)) {
            if (s->flags != SERVER_JUPED) {
                denora_cmd_stats(s_StatServ, "u", s->name);
            }
        }
        return s;
    } else {
        return NULL;
    }
}

/*************************************************************************/

void uptime_servers()
{
    Server *s;
    uint32 timenow, uptime;
    int servid = 0;
    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }

    s = server_find(denora->uplink);

    if (s) {
        server_uptime(s);
    }
    timenow = (long int) time(NULL);
    uptime = timenow - denora->start_time;

    servid = db_getserver(ServerName);
    rdb_query(QUERY_LOW, "UPDATE %s SET uptime=%ld WHERE servid=%d",
              ServerTable, uptime, servid);
}

/*************************************************************************/

/**
 * Find a server by name, returns NULL if not found
 * @param s Server struct
 * @param name Server Name
 * @return Server struct
 */
Server *findserver(Server * s, const char *name)
{
    Server *sl;

    if (!name || !*name) {
        return NULL;
    }

    alog(LOG_EXTRADEBUG, "debug: findserver(%s)", name);
    while (s && (stricmp(s->name, name) != 0)) {
        if (s->links) {
            sl = findserver(s->links, name);
            if (sl) {
                s = sl;
            } else {
                s = s->next;
            }
        } else {
            s = s->next;
        }
    }
    alog(LOG_EXTRADEBUG, "debug: findserver(%s) -> %p", name, (void *) s);
    return s;
}

/*************************************************************************/

/**
 * Find a server by name, returns NULL if not found
 * @param name Server Name
 * @return Server struct
 */
ServStats *findserverstats(const char *name)
{
    ServStats *s;

    SET_SEGV_LOCATION();
    if (!name || !*name) {
        alog(LOG_DEBUG, langstr(ALOG_ERR_FINDSERVERSTATS));
        do_backtrace(1);
        return NULL;
    }

    alog(LOG_EXTRADEBUG, "debug: findserverstats(%s) -> 0x%p", name, name);
    s = servstatlist[STATSSERVERHASH(name)];
    SET_SEGV_LOCATION();

    while (s && stricmp(s->name, name) != 0) {
        s = s->next;
    }
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: findserverstats(%s) -> 0x%p", name,
         (void *) s);
    return s;
}


/*************************************************************************/

/**
 * Find a server by name, returns NULL if not found
 * @param s Server struct
 * @param name Server Name
 * @return Server struct
 */
Server *findserver_uid(Server * s, const char *name)
{
    Server *sl;

    if (!name || !*name) {
        return NULL;
    }

    alog(LOG_EXTRADEBUG, "debug: findserver_uid(%s)", name);
    while (s && s->suid && (stricmp(s->suid, name) != 0)) {
        if (s->links) {
            sl = findserver_uid(s->links, name);
            if (sl) {
                s = sl;
            } else {
                s = s->next;
            }
        } else {
            s = s->next;
        }
    }
    alog(LOG_EXTRADEBUG, "debug: findserver_uid(%s) -> %p", name,
         (void *) s);
    return s;
}

/*************************************************************************/

void delete_serverstats(ServStats * serv)
{
    if (!serv) {
        alog(LOG_DEBUG, langstr(ALOG_ERR_DEL_SERVSTATS));
        return;
    }

    alog(LOG_EXTRADEBUG, langstr(ALOG_DEL_SERVSTATS_FOR), serv->name);

    if (serv->prev) {
        serv->prev->next = serv->next;
    } else {
        servstatlist[STATSSERVERHASH(serv->name)] = serv->next;
    }
    if (serv->next) {
        serv->next->prev = serv->prev;
    }

    alog(LOG_DEBUG, langstr(ALOG_DEL_SERVSTATS_DONE));
    free(serv);
}

/*************************************************************************/

/**
 * Remove and free a Server structure. This function is the most complete
 * remove treatment a server can get, as it first quits all clients which
 * still pretend to be on this server, then it walks through all connected
 * servers and disconnects them too. If all mess is cleared, the server
 * itself will be too.
 * @param serv is the Server struct
 * @param quitreason the server quit message
 * @return void
 */
void delete_server(Server * serv, const char *quitreason, int depth)
{
    User *u, *unext;
    Server *s;
    int i, x;

    if (!serv) {
        alog(LOG_DEBUG, langstr(ALOG_ERR_DEL_SERVER));
        return;
    }

    alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_FOR), serv->name);

    if (ircdcap->noquit || ircdcap->qs) {
        alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_NOQUIT));
        u = firstuser();
        while (u) {
            unext = nextuser();
            if (u->server->name == serv->name) {
                if (denora->do_sql)
                    db_removenick(u->sqlnick, (char *) quitreason);
                delete_user(u);
            }
            u = unext;
        }
        free(u);
        alog(LOG_DEBUG, "debug: delete_server() cleared all users");
    } else {
        alog(LOG_DEBUG, "uplink does not support NOQUIT or QS on SQUIT");
        alog(LOG_DEBUG, "ircdcap->noquit %d ircdcap->qs %d",
             ircdcap->noquit, ircdcap->qs);
    }

    if (serv->slinks_count) {
        for (i = 0; i < serv->slinks_count; i++) {
            alog(LOG_DEBUG, "debug: %s has slinks[%d] = %s", serv->name, i,
                 serv->slinks[i]);
        }
        i = 0;
        x = -1;
        ARRAY_FOREACH(i, serv->slinks) {
            s = server_find(serv->slinks[i]);
            if (s) {
                x = find_server_link(serv, s->name);
                delete_server(s, quitreason, depth++);
            } else {
                alog(LOG_DEBUG, "Can not find server %s", serv->slinks[i]);
            }
        }
        i = 0;
        ARRAY_FOREACH(i, serv->slinks) {
            free(serv->slinks[i]);
            ARRAY_REMOVE(serv->slinks, i);
        }
        alog(LOG_DEBUG, "debug: Reached the end of the loop");
    }

    send_event(EVENT_SQUIT, 2, serv->name, quitreason);

    if (denora->do_sql) {
        sql_do_squit(serv->name);
    }
    stats->servers--;
    serv->ss->splits++;
    serv->ss->split_stats = 1;
    serv->ss->lastseen = time(NULL);

    if (servlist->prev)
        servlist->prev->next = servlist->next;
    if (servlist->next)
        servlist->next->prev = servlist->prev;
    if (servlist->uplink) {
        if (servlist->uplink->links == serv)
            servlist->uplink->links = servlist->next;
    }

    free(serv->desc);
    if (serv->version) {
        free(serv->version);
    }
    if (serv->suid) {
        free(serv->suid);
    }
    if (serv->prev)
        serv->prev->next = serv->next;
    if (serv->next)
        serv->next->prev = serv->prev;
    if (serv->uplink) {
        if (serv->uplink->links == serv)
            serv->uplink->links = serv->next;
    }

    if (denora->do_sql) {
        rdb_query(QUERY_LOW,
                  "UPDATE %s SET val=%d, time=%ld WHERE type='servers'",
                  CurrentTable, stats->servers, time(NULL));
    }

    alog(LOG_DEBUG, langstr(ALOG_DEL_SERVER_DONE));
}

/*************************************************************************/

Server *first_server(void)
{
    server_cur = servlist;
    while (server_cur)
        server_cur = next_server();
    return server_cur;
}

/*************************************************************************/

Server *next_server(void)
{
    if (!server_cur)
        return NULL;

    do {
        if (server_cur->links) {
            server_cur = server_cur->links;
        } else if (server_cur->next) {
            server_cur = server_cur->next;
        } else {
            do {
                server_cur = server_cur->uplink;
                if (server_cur && server_cur->next) {
                    server_cur = server_cur->next;
                    break;
                }
            } while (server_cur);
        }
    } while (server_cur);

    return server_cur;
}

/*************************************************************************/

ServStats *first_statsserver(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentss == NULL)
        currentss = servstatlist[next_index++];
    alog(LOG_EXTRADEBUG, "debug: first_statserver() returning %s",
         currentss ? currentss->name : "NULL (end of list)");
    return currentss;
}

/*************************************************************************/

ServStats *next_statserver(void)
{
    SET_SEGV_LOCATION();

    if (currentss)
        currentss = currentss->next;
    if (!currentss && next_index < 1024) {
        while (next_index < 1024 && currentss == NULL)
            currentss = servstatlist[next_index++];
    }
    alog(LOG_EXTRADEBUG, "debug: next_statserver() returning %s",
         currentss ? currentss->name : "NULL (end of list)");
    return currentss;
}

/*************************************************************************/

void sql_do_uptime(char *source, char *uptime)
{
    char *tmp = NULL;
    char *tmp4 = NULL;
    char *days = NULL;
    int32 days_int = 0;
    char *hours = NULL;
    int32 hours_int = 0;
    char *mins = NULL;
    int32 mins_int = 0;
    char *secs = NULL;
    int32 secs_int = 0;
    int32 total = 0;
    Server *s;
    int servid = 0;

    if (LargeNet) {
        return;
    }
    s = server_find(source);

    if (!s) {
        alog(LOG_DEBUG, "Nonexistant server (%s) uptime", source);
        return;
    }

    if (uptime) {
        tmp = myStrGetToken(uptime, ' ', 0);
    }

    if (tmp) {
        /* Thales/NeoStats send more then the rest */
        if (!stricmp(tmp, "GNU") || !stricmp(tmp, "Statistical")) {
            days = myStrGetToken(uptime, ' ', 3);
            tmp4 = myStrGetTokenRemainder(uptime, ' ', 5);
            /* Sentinel - very none standard reply */
            /* :sentinel.nomadirc.net 242 Denora :Statistics up 8 minutes, 53 seconds */
        } else if (!stricmp(tmp, "Statistics")) {
            tmp4 = myStrGetToken(uptime, ' ', 3);
            if (!stricmp(tmp4, "minutes,") || !stricmp(tmp4, "minute,")) {
                mins = myStrGetToken(uptime, ' ', 2);
                secs = myStrGetToken(uptime, ' ', 4);
            } else if (!stricmp(tmp4, "hours,") || !stricmp(tmp4, "hour,")) {
                hours = myStrGetToken(uptime, ' ', 2);
                mins = myStrGetToken(uptime, ' ', 4);
            } else if (!stricmp(tmp4, "days,") || !stricmp(tmp4, "day,")) {
                days = myStrGetToken(uptime, ' ', 2);
                tmp4 = myStrGetTokenRemainder(uptime, ' ', 4);
                hours = myStrGetToken(tmp4, ':', 0);
                mins = myStrGetToken(tmp4, ':', 1);
            } else {
                alog(LOG_DEBUG, "Non-Standard Reply for UPTIME");
                if (tmp4) {
                    free(tmp4);
                }
                free(tmp);
                return;
            }
            tmp4 = NULL;
        } else {
            days = myStrGetToken(uptime, ' ', 2);
            tmp4 = myStrGetTokenRemainder(uptime, ' ', 4);
        }
    }

    if (tmp4) {
        hours = myStrGetToken(tmp4, ':', 0);
        mins = myStrGetToken(tmp4, ':', 1);
        secs = myStrGetToken(tmp4, ':', 2);
    }

    if (mins) {
        mins_int = atol(mins) * 60;
    }
    if (secs) {
        secs_int = atol(secs);
    }
    if (hours) {
        hours_int = atol(hours) * 3600;
    }
    if (days) {
        days_int = atol(days) * 86400;
    }

    total = mins_int + secs_int + hours_int + days_int;
    servid = db_getserver((s ? s->name : source));

    rdb_query(QUERY_LOW, "UPDATE %s SET uptime=%ld WHERE servid=%d",
              ServerTable, total, servid);

    if (s) {
        s->uptime = total;
    }

    free(days);
    free(hours);
    free(mins);
    free(secs);
    if (tmp) {
        free(tmp);
    }
    free(tmp4);
    return;
}

/*************************************************************************/

/* SDESC */
void sql_do_sdesc(char *user, char *msg)
{
    if (!denora->do_sql) {
        return;
    }
    if (LargeNet) {
        return;
    }
    user = rdb_escape(user);
    msg = rdb_escape(msg);
    rdb_query(QUERY_LOW, "UPDATE %s SET comment=\'%s\' WHERE servid=%d",
              ServerTable, msg, db_getservfromnick(user));
    free(user);
    free(msg);
}

/*************************************************************************/

void server_set_desc(char *server, char *msg)
{
    Server *s;
    s = server_find(server);

    if (s) {
        if (s->desc) {
            free(s->desc);
        }
        s->desc = sstrdup(msg);
    }

    sql_do_sdesc(server, msg);
}

/*************************************************************************/

/* Asuka 
   AB 351 BjAAA u2.10.11.07+asuka(1.2.1). Auska.Nomadirc.net :B96AeEFfIKMpSU 

   Viagra
   viagra.nomadirc.net 351 Denora VIAGRA_IRCd 1.3.0 - BETA 7. viagra.nomadirc.net :Ci TS5ow-r[BETA]
                            0       1          2    3  4   5   6                    7

   RageIRCD
   rage2.nomadirc.net 351 Denora :rage(bluemoon)-2.0.0.beta-8(1.956) rage2.nomadirc.net EBTsIFcMO TS5ow-r[BETA]
                           0       1

    IRCNet
    :ircnet.nomadirc.net 351 StatServ 2.11.1p1. ircnet.nomadirc.net 000A :aEFHJKMRTu

    Atheme IRC Services
    :services.nomadirc.net 351 StatServ :atheme-0.2. services.nomadirc.net FljR TS5ow
    :services.nomadirc.net 351 StatServ :Compile time: Thu Jun 16 2005 at 00:21:15 EDT, build-id 525, build 4
    :services.nomadirc.net 351 StatServ :Compiled on: [Linux denora.elite-irc.net 2.6.9 #1 Fri Oct 22 21:07:15 EDT 2004 i686 unknown]
*/
void sql_do_server_version(char *server, int ac, char **av)
{
    char *version = NULL;
    char *sqlversion = NULL;
    char *temp = NULL;
    char buf[100];
    int servid;
    Server *s;
    char *tmp2;
    *buf = '\0';

    SET_SEGV_LOCATION();

    if (LargeNet) {
        return;
    }
    s = server_find(server);
    SET_SEGV_LOCATION();

    if (!s) {
        alog(LOG_DEBUG, "Version for non-existatnt (%s)", server);
        sleep(15);
        return;
    }

    /* Viagra */
    if (ac == 8) {
        ircsnprintf(buf, 100, "%s %s %s %s %s", av[1], av[2], av[3], av[4],
                    av[5]);
        version = sstrdup(buf);
    }

    /* Unreal / P10 */
    if (ac == 4) {
        version = sstrdup(av[1]);
    }

    /* Anope/Thales respond like so */
    /* QuakeIRCD uses 6 */
    if (ac == 5 || ac == 6) {
        if ((denora_get_ircd() == IRC_ULTIMATE3) && (ac == 6)) {
            ircsnprintf(buf, 100, "%s %s %s", av[1], av[2], av[3]);
        } else {
            ircsnprintf(buf, 100, "%s %s", av[1], av[2]);
        }
        version = sstrdup(buf);
    }

    /* SolarStats and NeoStats like this */
    if (ac == 2) {
        tmp2 = myStrGetToken(av[1], ' ', 0);
        temp = sstrdup(tmp2);
        free(tmp2);
        /* if you have NeoStats mods load they to will respond on VERSION saying they are a 
           module */
        if (!stricmp(temp, "MODULE")) {
            free(temp);
            return;
            /* Atheme uses 351 to tell the user what os it was
               built on */
        } else if (!stricmp(temp, "Compile")) {
            free(temp);
            return;
        } else if (!stricmp(temp, "Compiled")) {
            free(temp);
            return;
        } else {
            version = sstrdup(temp);
            free(temp);
        }
    }

    /* inspircd */
    if (ac == 1) {
        version = sstrdup(av[0]);
    }

    if (!version) {
        alog(LOG_DEBUG, langstr(ALOG_DEBUG_UNKNOWN_VERSION_REPLY));
        alog(LOG_DEBUG, "debug: ac count is %d", ac);
        return;
    }

    if (s) {
        s->version = sstrdup(version);
    }
    if (denora->do_sql) {
        sqlversion = rdb_escape(version);
        if (((servid = db_checkserver(s->name)) != -1)) {
            rdb_query(QUERY_LOW,
                      "UPDATE %s SET version=\'%s\' WHERE servid=%d",
                      ServerTable, version, servid);
        }
        free(sqlversion);
    }
    free(version);
}

/*************************************************************************/

/* SQUIT */
void sql_do_squit(char *server)
{
    int servid;

    server = rdb_escape(server);
    if (ServerCacheTime) {
        servid = db_getserver(server);
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET online=\'N\', lastsplit=NOW(),linkedto=NULL WHERE servid=%d",
             ServerTable, servid);
        db_cleanserver();
    } else {
        rdb_query(QUERY_LOW, "DELETE FROM %s WHERE server=\'%s\'",
                  ServerTable, server);
    }
    free(server);
}

/*************************************************************************/

/**
 * Handle removing the server from the Server struct
 * @param servername Name of the server leaving
 * @return void
 */
void do_squit(char *servername)
{
    char buf[BUFSIZE];
    Server *s;
    *buf = '\0';
    s = server_find(servername);
    if (!s) {
        alog(LOG_NONEXISTANT, "SQUIT for nonexistent server (%s)!!",
             servername);
        return;
    }

    ircsnprintf(buf, BUFSIZE - 1, "%s %s", s->name,
                (s->uplink ? s->uplink->name : ""));

    if (ircdcap->unconnect) {
        if (s->uplink && me_server->uplink) {
            if (!stricmp(s->uplink->name, me_server->uplink->name)
                && (denora->capab & ircdcap->unconnect)) {
                alog(LOG_DEBUG, "debug: Sending UNCONNECT SQUIT for %s",
                     s->name);
                denora_cmd_squit(s->name, buf);
            }
        }
    }
    delete_server(s, buf, 0);
}

/*************************************************************************/

/**
 * Handle parsing the CAPAB/PROTOCTL messages
 * @param ac Number of arguments in av
 * @param av Agruments 
 * @return void
 */
void capab_parse(int ac, char **av)
{
    int i;
    int j;
    char *s, *tmp;

    char *temp;

    for (i = 0; i < ac; i++) {
        temp = av[i];

        s = myStrGetToken(temp, '=', 0);
        tmp = myStrGetTokenRemainder(temp, '=', 1);

        if (!s) {
            if (tmp) {
                free(tmp);
            }
            continue;
        }

        for (j = 0; capab_info[j].token; j++) {
            if (stricmp(s, capab_info[j].token) == 0)
                denora->capab |= capab_info[j].flag;
            /* Special cases */
            if ((stricmp(s, "NICKIP") == 0) && !ircd->nickip)
                ircd->nickip = 1;
            if ((stricmp(s, "CHANMODES") == 0) && tmp)
                ircd->chanmodes = sstrdup(tmp);
            if ((stricmp(s, "NICKCHARS") == 0) && tmp)
                ircd->nickchars = sstrdup(tmp);
        }

        if (s)
            free(s);
        if (tmp)
            free(tmp);
    }
}

/*************************************************************************/

Server *server_find(const char *source)
{
    Server *s;
    char unrealbuf[BUFSIZE];
    *unrealbuf = '\0';

    if (ircd->p10) {
        s = findserver_uid(servlist, source);
        if (!s) {
            return findserver(servlist, source);
        } else {
            return s;
        }
    } else if (!stricmp(ServerName, source)) {
        return me_server;
    } else if (ircd->ts6 && UseTS6) {
        s = findserver_uid(servlist, source);
        if (!s) {
            return findserver(servlist, source);
        } else {
            return s;
        }
    } else if (Numeric) {
        if (!myNumToken(source, '.')) {
            ircsnprintf(unrealbuf, BUFSIZE - 1, "%ld",
                        base64dec((char *) source));
            return findserver_uid(servlist, unrealbuf);
        } else {
            return findserver(servlist, source);
        }
    } else {
        return findserver(servlist, source);
    }
    return NULL;
}

/*************************************************************************/

int find_server_link(Server * s, const char *servname)
{
    int i;

    i = 0;
    if (s && s->slinks_count) {
        ARRAY_FOREACH(i, s->slinks) {
            if (stricmp(s->slinks[i], servname) == 0) {
                return i;
            }
        }
    }
    return -1;
}
