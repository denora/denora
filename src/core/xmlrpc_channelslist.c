/* XMLRPC Channel List
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

int xmlrpc_channellist(deno_socket_t xmlsocket, int ac, char **av);
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
        ("$Id$");
    moduleSetType(CORE);

    if (!XMLRPC_Enable) {
        return MOD_STOP;
    }

    xml = createXMLCommand("denora.channellist", xmlrpc_channellist);
    moduleAddXMLRPCcmd(xml);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int xmlrpc_channellist(deno_socket_t xmlsocket, int ac, char **av)
{
    char buf[BUFSIZE];
    Channel *c, *next;
    char *s = NULL;

    *buf = '\0';

    if (denora->debug >= 2) {
        protocol_debug(NULL, ac, av);
    }
    c = firstchan();
    while (c) {
        next = nextchan();
        if (SP_HTML) {
            if (ChanHasMode(c->name, CMODE_s)
                || ChanHasMode(c->name, CMODE_p)) {
                c = next;
                continue;
            }
        }
        if (s) {
            ircsnprintf(buf, BUFSIZE,
                        "%s <value><string>%s</string></value>\n\r", s,
                        c->name);
            free(s);
        } else {
            ircsnprintf(buf, BUFSIZE,
                        "<value><string>%s</string></value>\n\r", c->name);
        }
        s = sstrdup(buf);
        c = next;
    }
    ircsnprintf(buf, BUFSIZE,
                "<array>\r\n <data>\r\n  %s\r\n </data>\r\n</array>", s);
    xmlrpc_send(xmlsocket, 1, buf);
    return MOD_CONT;
}
