/* Event Uline
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
/*************************************************************************/

#include "denora.h"

int denora_event_246(char *source, int ac, char **av);
int denora_event_248(char *source, int ac, char **av);
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

    /* 246 is SolidIRC's U:line */
    m = createMessage("246", denora_event_246);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 246 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    /* Most everyone else uses 248 */
    m = createMessage("248", denora_event_248);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 248 [%d][%s]", status,
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
 * Process numeric 246 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_246(char *source, int ac, char **av)
{
    SET_SEGV_LOCATION();

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac >= 3) {
        if (denora_get_ircd() == IRC_SOLIDIRCD) {
            sql_uline(av[4]);
        } else if (denora_get_ircd() == IRC_VIAGRA) {
            sql_uline(av[2]);
        } else if (denora_get_ircd() == IRC_BAHAMUT) {
            sql_uline(av[4]);
        } else if (denora_get_ircd() == IRC_FQIRCD) {
            sql_uline(av[4]);
        } else if (denora_get_ircd() == IRC_ULTIMATE3) {
            sql_uline(av[2]);
        } else if (denora_get_ircd() == IRC_LIQUIDIRCD) {
            sql_uline(av[2]);
        }
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Process numeric 248 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_248(char *source, int ac, char **av)
{
    SET_SEGV_LOCATION();

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac >= 3) {
        sql_uline(av[2]);
    }
    return MOD_CONT;
}
