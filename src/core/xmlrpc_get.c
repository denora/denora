/* StatServ core functions
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: xmlrpc_get.c 606 2006-05-13 11:10:09Z trystan $
 *
 */
/*************************************************************************/

#include "denora.h"

int xmlrpc_getstat(deno_socket_t xmlsocket, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
    XMLRPCCmd *xml;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id: xmlrpc_get.c 606 2006-05-13 11:10:09Z trystan $");
    moduleSetType(CORE);

    if (!XMLRPC_Enable) {
        return MOD_STOP;
    }

    xml = createXMLCommand("denora.getstat", xmlrpc_getstat);
    moduleAddXMLRPCcmd(xml);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int xmlrpc_getstat(deno_socket_t xmlsocket, int ac, char **av)
{
    char buf[BUFSIZE];
    *buf = '\0';

    if (denora->debug >= 2) {
        protocol_debug(NULL, ac, av);
    }

    if (!stricmp(av[0], "users")) {
        ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->users);
    } else if (!stricmp(av[0], "chans")) {
        ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->chans);
    } else {
        ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->users);
    }
    xmlrpc_send(xmlsocket, 1, buf);
    return MOD_CONT;
}
