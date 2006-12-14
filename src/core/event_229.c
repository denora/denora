/* Numeric Event 229
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
/*************************************************************************/

#include "denora.h"

int denora_event_229(char *source, int ac, char **av);
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
    moduleAddVersion("$Id$");
    moduleSetType(CORE);

    /* spam filter */
    m = createMessage("229", denora_event_229);
    status = moduleAddMessage(m, MOD_HEAD);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 229 [%d][%s]", status,
             ModuleGetErrStr(status));
    }
    if (ircd->spamfilter) {
        return MOD_CONT;
    } else {
        return MOD_STOP;
    }
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

/*
[Dec 13 15:11:09.442539 2005] Source Luna.NomadIrc.Net
[Dec 13 15:11:09.442593 2005] av[0] = Denora
[Dec 13 15:11:09.442647 2005] av[1] = f
[Dec 13 15:11:09.442701 2005] av[2] = p
[Dec 13 15:11:09.442755 2005] av[3] = gline
[Dec 13 15:11:09.442810 2005] av[4] = 0
[Dec 13 15:11:09.442863 2005] av[5] = 781731
[Dec 13 15:11:09.442918 2005] av[6] = 7776000
[Dec 13 15:11:09.442972 2005] av[7] = [SPAMFILTER]_:_Banned_for_spam_/_contact_kline@nomadirc.net
[Dec 13 15:11:09.443029 2005] av[8] = Luna.NomadIrc.Net
[Dec 13 15:11:09.443084 2005] av[9] = .*FOR MATRIX 2 DOWNLOAD.*
*/
/**
 * Process numeric 229 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_229(char *source, int ac, char **av)
{
    SET_SEGV_LOCATION();
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp("f", av[1])) {
        sql_do_server_spam_add(av[2], av[3], av[8], av[4], av[5], av[6],
                               av[7], av[9]);
    }
    return MOD_CONT;
}
