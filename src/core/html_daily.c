/* Html Daily
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

void html_dailystats_title(FILE * ptr);
void html_dailystats(FILE * ptr);
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
        ("");
    moduleSetType(CORE);

    h = createHTMLtag("!DAILYSTATS!", html_dailystats);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for !DAILYSTATS! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!DAILYSTATSTITLE!", html_dailystats_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurred setting message for !DAILYSTATSTITLE! [%d][%s]",
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

void html_dailystats(FILE * ptr)
{
    char timebuf[64];
    struct tm tm;

/*
<tr><td colspan="2">Total Users Connected:</td>
<td colspan="2"> 14839</td></tr>
*/

    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", getstring(NULL, HTML_DAILY_HEADER));
#ifdef _WIN32
        localtime_s(&tm, &stats->daily_users_time);
#else
        tm = *localtime(&stats->daily_users_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_DAILY_USER), stats->daily_users,
                 timebuf);
        dfprintf(ptr, getstring(NULL, HTML_DAILY_CHAN),
                 stats->daily_chans);
#ifdef _WIN32
        localtime_s(&tm, &stats->daily_opers_time);
#else
        tm = *localtime(&stats->daily_opers_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_DAILY_OPERS),
                 stats->daily_opers, timebuf);
#ifdef _WIN32
        localtime_s(&tm, &stats->daily_servers_time);
#else
        tm = *localtime(&stats->daily_servers_time);
#endif
        strftime_lang(timebuf, sizeof(timebuf), NULL,
                      STRFTIME_DATE_TIME_FORMAT, &tm);
        dfprintf(ptr, getstring(NULL, HTML_DAILY_SERVERS),
                 stats->daily_servers, timebuf);
        fprintf(ptr, "%s", getstring(NULL, HTML_DAILY_RESET_AT));
        fprintf(ptr, "%s", getstring(NULL, HTML_DAILY_FOOTER));
    }
}

/*************************************************************************/

void html_dailystats_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_DAILY_TITLE));
    }
}
