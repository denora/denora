/* Html Misc
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

void html_version(FILE * ptr);
void html_title(FILE * ptr);
void html_lang(FILE * ptr);
void html_credits(FILE * ptr);
void html_netstats_title(FILE * ptr);
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
    moduleAddVersion("$Id$");
    moduleSetType(CORE);

    h = createHTMLtag("!VERSION!", html_version);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !VERSION! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!TITLE!", html_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TITLE! [%d][%s]",
             status, ModuleGetErrStr(status));
    }
    h = createHTMLtag("!HTMLLANG!", html_lang);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !HTMLLANG! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!CREDITS!", html_credits);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !CREDITS! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!NETSTATSTITLE!", html_netstats_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !NETSTATSTITLE! [%d][%s]",
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

void html_version(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        dfprintf(ptr, langstr(HTML_VERSION), denora->version, ircd->name);
    }
}

/*************************************************************************/

void html_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        dfprintf(ptr, getstring(NULL, HTML_TITLE), NetworkName);
    }
}

/*************************************************************************/

void html_lang(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_LANG));
    }
}

/*************************************************************************/

void html_credits(FILE * ptr)
{
    struct tm tm;
    char timebuf[64];
    time_t ts;
    ts = time(NULL);

#ifdef MSVS2005
    localtime_s(&tm, &ts);
#else
    tm = *localtime(&ts);
#endif
    strftime_lang(timebuf, sizeof(timebuf), NULL,
                  STRFTIME_DATE_TIME_FORMAT, &tm);
    fprintf(ptr, "HTML Last Generated at %s", timebuf);
}

/*************************************************************************/

void html_netstats_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_NETSTATS_TITLE));
    }
}
