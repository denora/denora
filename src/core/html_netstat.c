/* Html Netstat
 *
 * (c) 2004-2011 Denora Team
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

void html_netstats(FILE * ptr);
void html_curnetstats_title(FILE * ptr);
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

    h = createHTMLtag("!NETSTATS!", html_netstats);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !NETSTATS! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!CURNETSTATSTITLE!", html_curnetstats_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !CURNETSTATSTITLE! [%d][%s]",
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

void html_netstats(FILE * ptr)
{
/* <tr><td colspan=2>Total Users Ever Connected</td><td colspan=2>197231</td></tr> */

    char timebuf[64];
    struct tm tm;

    SET_SEGV_LOCATION();

    if (ptr) {
        SET_SEGV_LOCATION();
        fprintf(ptr, "%s", getstring(NULL, HTML_NETSTATS_HEADER));
#ifdef _WIN32
        localtime_s(&tm, &stats->users_max_time);
#else
        tm = *localtime(&stats->users_max_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_NETSTATS_USERS), stats->users,
                 stats->users_max, timebuf);
        SET_SEGV_LOCATION();
#ifdef _WIN32
        localtime_s(&tm, &stats->chans_max_time);
#else
        tm = *localtime(&stats->chans_max_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_NETSTATS_CHANS), stats->chans,
                 stats->chans_max, timebuf);
#ifdef _WIN32
        localtime_s(&tm, &stats->opers_max_time);
#else
        tm = *localtime(&stats->opers_max_time);
#endif
        SET_SEGV_LOCATION();
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_NETSTATS_OPERS), stats->opers,
                 stats->opers_max, timebuf);
#ifdef _WIN32
        localtime_s(&tm, &stats->servers_max_time);
#else
        tm = *localtime(&stats->servers_max_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_NETSTATS_SERVERS),
                 stats->servers, stats->servers_max, timebuf);
        dfprintf(ptr, getstring(NULL, HTML_NETSTATS_AWAY), stats->away);
        fprintf(ptr, "%s", getstring(NULL, HTML_NETSTATS_FOOTER));
    }
}

/*************************************************************************/

void html_curnetstats_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_CUR_NETSTATS_TITLE));
    }
}
