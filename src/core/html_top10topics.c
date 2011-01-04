/* HTML Top 10 Topics
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

void html_top10_topics_title(FILE * ptr);
void html_top10_topics(FILE * ptr);
void html_mosttopic_title(FILE * ptr);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int top10topic_sort(const void *v, const void *v2);

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

    h = createHTMLtag("!TOP10TOPICS!", html_top10_topics);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TOP10TOPICS! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!TOP10TOPICSTITLE!", html_top10_topics_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TOP10TOPICSTITLE! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!MOSTTOPICTITLE!", html_mosttopic_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !MOSTTOPICTITLE! [%d][%s]",
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

int top10topic_sort(const void *v, const void *v2)
{
    const StatsChannel *t = (void *) v;
    const StatsChannel *t2 = (void *) v2;
    return (t2->topic_count - t->topic_count);
}

/*************************************************************************/

void html_top10_topics(FILE * ptr)
{
    StatsChannel *s;
    lnode_t *tn;
    char *tempc;
    int x = 0;
    Exclude *e;

    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_TOP10_TOPIC_HEADER));
        SET_SEGV_LOCATION();
        list_sort(StatsChanhead, top10topic_sort);
        tn = list_first(StatsChanhead);
        while (tn) {
            s = lnode_get(tn);
            if (SP_HTML && (s->secret || s->private)) {
                tn = list_next(StatsChanhead, tn);
                continue;
            }
            if (s->topic_count) {
                e = find_exclude(s->name, NULL);
                if (!e) {
                    tempc = char_encode(s->name);
                    if (s->in_use) {
                        dfprintf(ptr,
                                 langstring
                                 (HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE),
                                 tempc, s->topic_count);
                    } else {
                        dfprintf(ptr,
                                 langstring
                                 (HTML_CHAN_TOP10_KICKS_CONTENT),
                                 tempc, s->topic_count);
                    }
                    free(tempc);
                    x++;
                    if (x > 10) {
                        break;
                    }
                }
            }
            tn = list_next(StatsChanhead, tn);

        }
        fprintf(ptr, "%s", langstring(HTML_TOP10_TOPIC_FOOTER));
        SET_SEGV_LOCATION();
    }
}

/*************************************************************************/

void html_top10_topics_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_TOP10_TOPIC_TITLE));
    }
}


/*************************************************************************/

void html_mosttopic_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_MOST_TOPIC_TITLE));
    }
}
