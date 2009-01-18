/* Numeric Event 242
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
/*************************************************************************/

#include "denora.h"

int denora_event_242(char *source, int ac, char **av);
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
        ("$Id$");
    moduleSetType(CORE);

    m = createMessage("242", denora_event_242);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 242 [%d][%s]", status,
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

/**
 * Process numeric 242 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_242(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac >= 2) {
        sql_do_uptime(source, av[1]);
    }
    return MOD_CONT;
}
