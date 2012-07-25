/*
 * ircd_ports module for Denora 1.2+
 * (c) Trystan
 */

#include "denora.h"
#define AUTHOR "Trystan"
#define VERSION "1.0.1"
#define MYNAME "ircd_ports"
#define SERVPORTTABLE "serverports"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int my_server(int argc, char **argv);
int get_port(char *source, int ac, char **av);
int my_squit(int argc, char **argv);
void create_table(void);

#ifndef _WIN32
int DenoraInit(__attribute__((unused))int argc, __attribute__((unused))char **argv)
#else
int DenoraInit(int argc, char **argv)
#endif
{
    EvtHook *hook = NULL;
    Message *msg = NULL;
    int status;

    hook = createEventHook(EVENT_SERVER, my_server);
    status = moduleAddEventHook(hook);
    if (status != MOD_ERR_OK) {
        /* something went wrong say something about */
        alog(LOG_NORMAL, "[%s%s] unable to bind to EVENT_SERVER error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
        return MOD_STOP;
    }

    hook = createEventHook(EVENT_SQUIT, my_squit);
    status = moduleAddEventHook(hook);
    if (status != MOD_ERR_OK) {
        /* something went wrong say something about */
        alog(LOG_NORMAL, "[%s%s] unable to bind to EVENT_SQUIT error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
        return MOD_STOP;
    }

    if (denora_get_ircd() == IRC_SOLIDIRCD) {
        msg = createMessage("249", get_port);
        status = moduleAddMessage(msg, MOD_HEAD);
        if (status != MOD_ERR_OK) {
            /* something went wrong say something about */
            alog(LOG_NORMAL, "[%s%s] unable to bind to 249 error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
            return MOD_STOP;
        }
    } else if (denora_get_ircd() == IRC_UNREAL32) {
        msg = createMessage("NOTICE", get_port);
        status = moduleAddMessage(msg, MOD_HEAD);
        if (status != MOD_ERR_OK) {
            /* something went wrong say something about */
            alog(LOG_NORMAL, "[%s%s] unable to bind to NOTICE error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
            return MOD_STOP;
        }
        if (UseTokens) {
            msg = createMessage("B", get_port);
            status = moduleAddMessage(msg, MOD_HEAD);
            if (status != MOD_ERR_OK) {
                /* something went wrong say something about */
                alog(LOG_NORMAL, "[%s%s] unable to bind to NOTICE(B) error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
                return MOD_STOP;
            }
        }
    } else if (denora_get_ircd() == IRC_ASUKA || denora_get_ircd() == IRC_NEFARIOUS  || denora_get_ircd() == IRC_IRCU) {
        msg = createMessage("217", get_port);
        status = moduleAddMessage(msg, MOD_HEAD);
        if (status != MOD_ERR_OK) {
            /* something went wrong say something about */
            alog(LOG_NORMAL, "[%s%s] unable to bind to 217 error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
            return MOD_STOP;
        }
    } else {
        return MOD_STOP;
    }

    create_table();
    rdb_clear_table((char*) SERVPORTTABLE);

    moduleAddAuthor(AUTHOR);
    moduleAddVersion(VERSION);
    return MOD_CONT;
}

void DenoraFini(void)
{
    alog(LOG_NORMAL, "Unloading ircd_port%s", MODULE_EXT);
}

int my_server(int argc, char **argv)
{
    if (argc >= 1) {
        if (stricmp(ServerName, argv[0])) {
            denora_cmd_stats(s_StatServ, "P", argv[0]);
        }
    }
    return MOD_CONT;
}

int my_squit(int argc, char **argv)
{
    if (argc >= 2) {
       rdb_query(QUERY_LOW, "DELETE FROM %s WHERE name=\'%s\'", SERVPORTTABLE, argv[0]);
    }
    return MOD_CONT;
}

void create_table(void)
{
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;

	rdb_query(QUERY_HIGH, "SHOW TABLES LIKE '%s';", SERVPORTTABLE);
	mysql_res = mysql_store_result(mysql);
	if (mysql_num_rows(mysql_res) == 0) {
        	rdb_query(QUERY_LOW, "CREATE TABLE %s ( id mediumint(15) NOT NULL auto_increment, name varchar(255), portnum varchar(255), porttype varchar(255), PRIMARY KEY (id) );", SERVPORTTABLE);
	}
	mysql_free_result(mysql_res);
#endif
}

int get_port(char *source, __attribute__((unused))int ac, char **av)
{
    char *port;
    char *portnum;
    char *type;
    char *temp = NULL;
    Server *s;
    User *u = NULL;

    if (denora_get_ircd() == IRC_SOLIDIRCD) {
        s = server_find(source);
        if (!s) {
            return MOD_CONT;
        }
        port = myStrGetToken(av[1], ' ', 0);
        if (!port) {
            return MOD_CONT;
        }
        if (!stricmp(port, "PORT")) {
            portnum = myStrGetToken(av[1], ' ', 2);
            type = myStrGetTokenRemainder(av[1], ' ', 3);
            rdb_query
                (QUERY_LOW, "INSERT INTO %s (name, portnum, porttype) VALUES(\'%s\',\'%s\',\'%s\')",
                 SERVPORTTABLE, source, portnum, type);
            free(port);
            free(portnum);
            free(type);
        }
    } else if (denora_get_ircd() == IRC_UNREAL32) {
        s = server_find(source);
        if (!s) {
            return MOD_CONT;
        }
        if (!stricmp(av[0], s_StatServ)) {
            port = myStrGetToken(av[1], ' ', 3);
            if (!port) {
                return MOD_CONT;
            }
            temp = myStrGetToken(port, ':', 1);
            portnum = myStrGetToken(temp, ',', 0);
            type = myStrGetTokenRemainder(av[1], ' ', 4);
            rdb_query
                (QUERY_LOW, "INSERT INTO %s (name, portnum, porttype) VALUES(\'%s\',\'%s\',\'%s\')",
                 SERVPORTTABLE, source, portnum, type);
            free(port);
            free(portnum);
            free(type);
        }
    } else if (denora_get_ircd() == IRC_ASUKA || denora_get_ircd() == IRC_NEFARIOUS || denora_get_ircd() == IRC_IRCU) {
        s = server_find(source);
        if (!s) {
            return MOD_CONT;
        }
        u = find_byuid(av[0]);
        if (!u) {
            return MOD_CONT;
        }
        if (!stricmp(u->nick, s_StatServ)) {
            if (!stricmp(av[1], "P")) {
                if (!stricmp(av[4], "C")) {
                    type = sstrdup("Clients");
                } else if (!stricmp(av[4], "S")) {
                    type = sstrdup("Servers");
                } else {
                    type = sstrdup("Unknown");
                }
                rdb_query
                 (QUERY_LOW, "INSERT INTO %s (name, portnum, porttype) VALUES(\'%s\',\'%s\',\'%s\')",
                 SERVPORTTABLE, s->name, av[2], type);
            }
        }
    }
    return MOD_CONT;
}
