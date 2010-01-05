/* HTML Servdtl
 *
 * (c) 2004-2010 Denora Team
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

void html_server_details(FILE * ptr);
void html_serverdetails_title(FILE * ptr);
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
    moduleAddVersion
        ("$Id$");
    moduleSetType(CORE);

    h = createHTMLtag("!SRVLISTDET!", html_server_details);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !SRVLISTDET! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!SERVERDETAILSTITLE!", html_serverdetails_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !SERVERDETAILSTITLE! [%d][%s]",
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

void html_server_details(FILE * ptr)
{
    ServStats *s, *next;
    Server *serv;
    char timebuf[64];
    struct tm tm;

    SET_SEGV_LOCATION();

    if (ptr) {
        SET_SEGV_LOCATION();
        s = first_statsserver();
        while (s) {
            serv = server_find(s->name);
            next = next_statserver();
            if (serv) {
                if (HideUline
                    && (serv->uline || serv->flags == SERVER_JUPED)) {
                    s = next;
                    continue;
                }
            }
            dfprintf(ptr, getstring(NULL, HTML_SERVER_DETAIL_HEADER),
                     s->name, s->name);
            if (s->split_stats) {
                if (s->lastseen) {
#ifdef MSVS2005
                    localtime_s(&tm, &s->lastseen);
#else
                    tm = *localtime(&s->lastseen);
#endif
                    strftime_lang(timebuf, sizeof(timebuf), NULL,
                                  STRFTIME_DATE_TIME_FORMAT, &tm);
                    dfprintf(ptr,
                             langstring(HTML_SERVER_DETAIL_LAST_SEEN),
                             timebuf);
                }
            } else {
#ifdef MSVS2005
                localtime_s(&tm, &s->maxusertime);
#else
                tm = *localtime(&s->maxusertime);
#endif
                strftime_lang(timebuf, sizeof(timebuf), NULL,
                              STRFTIME_DATE_TIME_FORMAT, &tm);
                dfprintf(ptr, langstring(HTML_SERVER_DETAIL_USERS),
                         s->currentusers, 0, s->maxusers, timebuf);

#ifdef MSVS2005
                localtime_s(&tm, &s->maxopertime);
#else
                tm = *localtime(&s->maxopertime);
#endif
                strftime_lang(timebuf, sizeof(timebuf), NULL,
                              STRFTIME_DATE_TIME_FORMAT, &tm);
                dfprintf(ptr, langstring(HTML_SERVER_DETAIL_OPERS),
                         s->opers, 0, s->maxopers, timebuf);
            }
            dfprintf(ptr, langstring(HTML_SERVER_DETAIL_TOTALUSERSEVER),
                     s->totalusersever);

            dfprintf(ptr, langstring(HTML_SERVER_DETAIL_IRCOPKILLS),
                     s->ircopskills);
            dfprintf(ptr, langstring(HTML_SERVER_DETAIL_SERVERKILLS),
                     s->serverkills);
            if (s->highestping) {
#ifdef MSVS2005
                localtime_s(&tm, &s->maxpingtime);
#else
                tm = *localtime(&s->maxpingtime);
#endif
                strftime_lang(timebuf, sizeof(timebuf), NULL,
                              STRFTIME_DATE_TIME_FORMAT, &tm);
                dfprintf(ptr,
                         langstring(HTML_SERVER_DETAIL_HIGHESTPING),
                         s->highestping, timebuf);
            }
            if (!s->split_stats) {
                if (serv) {
                    dfprintf(ptr,
                             langstring(HTML_SERVER_DETAIL_CURPING),
                             serv->ping);
                }
            }
            dfprintf(ptr, langstring(HTML_SERVER_DETAIL_SPLITS),
                     (s ? s->splits : 0));
            dfprintf(ptr, langstring(HTML_SERVER_DETAIL_FOOTER));

            s = next;
        }
        SET_SEGV_LOCATION();
    }
}

/*************************************************************************/

void html_serverdetails_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_SERVERDETAILS_TITLE));
    }
}
