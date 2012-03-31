/* Numeric Event 372/375/376
 *
 * (c) 2004-2012 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * 
 *
 */
/*************************************************************************/

#include "denora.h"

int denora_event_372(char *source, int ac, char **av);
int denora_event_375(char *source, int ac, char **av);
int denora_event_376(char *source, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    Message *m;
    int status;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("");
    moduleSetType(CORE);

    m = createMessage("372", denora_event_372);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for 372 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("375", denora_event_375);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for 375 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("376", denora_event_376);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for 376 [%d][%s]", status,
             ModuleGetErrStr(status));
    }
    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/


int denora_event_376(char *source, int ac, char **av)
{
    Server *s;
    s = server_find(source);
    if (!s) {
        return MOD_CONT;
    }
    sql_motd_store(s);
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Process numeric 372 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_372(char *source, int ac, char **av)
{
    Server *s;
    char buf[NET_BUFSIZE];

    SET_SEGV_LOCATION();
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    s = server_find(source);
    if (!s) {
        return MOD_CONT;
    }

    if (ac >= 2) {
        if (s->motd) {
            ircsnprintf(buf, NET_BUFSIZE - 1, "%s\n\r%s", s->motd, av[1]);
            free(s->motd);
            s->motd = sstrdup(buf);
        } else {
            s->motd = sstrdup(av[1]);
        }
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Process numeric 375 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_375(char *source, int ac, char **av)
{
    SET_SEGV_LOCATION();
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    rdb_query(QUERY_LOW, "UPDATE %s SET motd=\'\' WHERE server=\'%s\'",
              ServerTable, source);
    return MOD_CONT;
}
