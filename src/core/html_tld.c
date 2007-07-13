/* HTML TLD
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

E void html_tld_table(FILE * ptr);
E void html_tld_title(FILE * ptr);
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

    h = createHTMLtag("!TLDMAP!", html_tld_table);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TLDMAP! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!TLDTITLE!", html_tld_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TLDTITLE! [%d][%s]",
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

void html_tld_table(FILE * ptr)
{
    TLD *t;
    lnode_t *tn;
    char *tempc, *tempcc;
    int x = 0;

    if (ptr) {
        fprintf(ptr, "%s", langstr(HTML_TLD_HEADER));
        list_sort(Thead, sortusers);
        tn = list_first(Thead);
        while (tn) {
            t = lnode_get(tn);
            if (t->count) {
                tempcc = char_encode(t->countrycode);
                tempc = char_encode(t->country);
                dfprintf(ptr, langstr(HTML_TLD_CONTENT),
                         tempcc, tempc, t->count, t->overall);
                free(tempcc);
                free(tempc);
                x++;
                if (x > 10) {
                    break;
                }
            }
            tn = list_next(Thead, tn);
        }
        fprintf(ptr, "%s", langstr(HTML_TLD_FOOTER));
    }
}

/*************************************************************************/

void html_tld_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_TLD_TITLE));
    }
}

/*************************************************************************/
