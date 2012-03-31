/* Html Map
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

void html_map(FILE * ptr);
void html_netmap_title(FILE * ptr);
Server *server_map(Server * s, FILE * ptr, int depth);
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
    HTMLTag *h;
    int status;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion("");
    moduleSetType(CORE);

    h = createHTMLtag("!MAP!", html_map);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for !MAP! [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    h = createHTMLtag("!NETMAPTITLE!", html_netmap_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for !NETMAPTITLE! [%d][%s]",
             status, ModuleGetErrStr(status));
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

Server *server_map(Server * s, FILE * ptr, int last)
{
    Server *sl;
    int i;
    int h;
    char buf[256];
    char *tbuf = NULL;
    char prefixbuf[256];
    char *prefix;

    if (s) {
        if (s->hops) {
            buf[0] = '\0';
            for (h = 1; h <= s->hops; h++) {
                if (tbuf) {
                    ircsnprintf(buf, 256, "%s&nbsp", tbuf);
                    if (tbuf) {
                        free(tbuf);
                    }
                    tbuf = sstrdup(buf);
                } else {
                    ircsnprintf(buf, 256, "&nbsp&nbsp&nbsp&nbsp&nbsp");
                    tbuf = sstrdup(buf);
                }
            }
            ircsnprintf(prefixbuf, sizeof(prefixbuf), "%s%s", tbuf,
                        (last ? "`-" : "|-"));
            if (tbuf) {
                free(tbuf);
            }
            prefix = sstrdup(prefixbuf);

        } else {
            prefix = sstrdup("");
        }

        if (!stricmp(ServerName, s->name)) {
            dfprintf(ptr,
                     "<tr><td>%s</td><td>%ld/%ld</td><td>%ld/%ld</td><td>%ld/%ld</td></tr>\n",
                     s->name, (long int) s->ss->currentusers,
                     (long int) s->ss->maxusers,
                     (long int) s->ss->opers,
                     (long int) s->ss->maxopers, (long int) s->ping,
                     (long int) s->ss->highestping);
        } else {
            if (HideUline) {
                if (!s->uline && s->flags != SERVER_JUPED) {
                    dfprintf(ptr,
                             "<tr><td>%s%s</td><td>%ld/%ld</td><td>%ld/%ld</td><td>%ld/%ld</td></tr>\n",
                             prefix, s->name,
                             (long int) s->ss->currentusers,
                             (long int) s->ss->maxusers,
                             (long int) s->ss->opers,
                             (long int) s->ss->maxopers,
                             (long int) s->ping,
                             (long int) s->ss->highestping);
                }
            } else {
                dfprintf(ptr,
                         "<tr><td>%s%s</td><td>%ld/%ld</td><td>%ld/%ld</td><td>%ld/%ld</td></tr>\n",
                         prefix, s->name, (long int) s->ss->currentusers,
                         (long int) s->ss->maxusers,
                         (long int) s->ss->opers,
                         (long int) s->ss->maxopers, (long int) s->ping,
                         (long int) s->ss->highestping);
            }
        }
        free(prefix);

        if (s->slinks_count) {
            for (i = 0; i <= s->slinks_count - 1; i++) {
                sl = server_find(s->slinks[i]);
                if (i == s->slinks_count - 1) {
                    server_map(sl, ptr, 1);
                } else {
                    server_map(sl, ptr, 0);
                }
            }
        }
        return s;
    } else {
        return NULL;
    }
}

/*************************************************************************/

void html_map(FILE * ptr)
{
    dfprintf(ptr,
             "<table border=0><tr><th>Server Name</th><th>Users/Max</th><th>Opers/Max</th><th>Lag/Max</th></tr>");
    server_map(servlist, ptr, 0);
    fprintf(ptr, "</table>");
}

/*************************************************************************/

void html_netmap_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_NETMAP_TITLE));
    }
}
