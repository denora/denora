/*
 * log_ctcpversion module for Denora 1.4+
 * (c) Jobe (jobe@mdbnet.co.uk)
 */

#include "denora.h"
#define AUTHOR "Jobe"
#define VERSION "1.0.1"
#define MYNAME "log_ctcpversion"

#define LOGTARGET "#services"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int my_version(int argc, char **argv);

int DenoraInit(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
    EvtHook *hook = NULL;
    int status;

    hook = createEventHook(EVENT_CTCP_VERSION, my_version);
    status = moduleAddEventHook(hook);
    if (status != MOD_ERR_OK) {
        /* something went wrong say something about */
        alog(LOG_NORMAL, "[%s%s] unable to bind to EVENT_CTCP_VERSION error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
        return MOD_STOP;
    }

    moduleAddAuthor(AUTHOR);
    moduleAddVersion(VERSION);
    return MOD_CONT;
}

void DenoraFini(void)
{
    alog(LOG_NORMAL, "Unloading log_ctcpversion%s", MODULE_EXT);
}

int my_version(int argc, char **argv)
{
    if (argc >= 2) {
	    /* argv[0] = nick, argv[1] = version */
        if (stricmp(ServerName, argv[0])) {
            denora_cmd_privmsg(s_StatServ, sstrdup(LOGTARGET), "\x02VERSION\x02 %s has version %s", argv[0], argv[1]);
        }
    }
    return MOD_CONT;
}
