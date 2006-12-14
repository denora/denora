/* XMLRPC Channel Data
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: xmlrpc_channeldata.c 627 2006-07-23 14:05:55Z trystan $
 *
 */
/*************************************************************************/

#include "denora.h"

int xmlrpc_channeldata(deno_socket_t xmlsocket, int ac, char **av);
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
        ("$Id: xmlrpc_channeldata.c 627 2006-07-23 14:05:55Z trystan $");
    moduleSetType(CORE);

    if (!XMLRPC_Enable) {
        return MOD_STOP;
    }

    xml = createXMLCommand("denora.channeldata", xmlrpc_channeldata);
    moduleAddXMLRPCcmd(xml);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*
   av[0] = channel name
   av[1] = data to get

   topic, usercount, maxusercount, modes

*/
int xmlrpc_channeldata(deno_socket_t xmlsocket, int ac, char **av)
{
    char buf[BUFSIZE];
    Channel *c;
    char *temp;

    *buf = '\0';

    if (denora->debug >= 2) {
        protocol_debug(NULL, ac, av);
    }

    c = findchan(av[0]);
    if (c) {
        if (ac >= 2) {
            if (!stricmp(av[1], "topic")) {
                temp = char_encode(c->topic);
                ircsnprintf(buf, BUFSIZE, "<string>%s</string>", temp);
                free(temp);
                xmlrpc_send(xmlsocket, 1, buf);
            } else if (!stricmp(av[1], "usercount")) {
                ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>",
                            (long int) c->stats->usercount);
                xmlrpc_send(xmlsocket, 1, buf);
            } else if (!stricmp(av[1], "maxusercount")) {
                ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>",
                            (long int) c->stats->maxusercount);
                xmlrpc_send(xmlsocket, 1, buf);
            } else if (!stricmp(av[1], "modes")) {
                temp = chan_get_modes(c, 1);
                xmlrpc_string(buf, temp);
                free(temp);
                xmlrpc_send(xmlsocket, 1, buf);
            } else {
                xmlrpc_generic_error(xmlsocket, 2, "To few arguments");
            }
        } else {
            xmlrpc_generic_error(xmlsocket, 2, "To few arguments");
        }
    } else {
        xmlrpc_generic_error(xmlsocket, 1, "No such channel");
    }
    return MOD_CONT;
}
