/* HTML Top 10 Channel
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

void html_top10chan(FILE * ptr);
void html_top10_chanever_title(FILE * ptr);
void html_popularchan_title(FILE * ptr);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int top10chan_sort(const void *v, const void *v2);

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

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	if (!denora->do_html)
	{
		return MOD_STOP;
	}
	moduleAddAuthor("Denora");
	moduleAddVersion
	("");
	moduleSetType(CORE);

	h = createHTMLtag("!TOP10CHAN!", html_top10chan);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TOP10CHAN! [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	h = createHTMLtag("!TOP10CHANEVERTITLE!", html_top10_chanever_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TOP10CHANEVERTITLE! [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	h = createHTMLtag("!POPULARCHANTITLE!", html_popularchan_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !POPULARCHANTITLE! [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}


	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int top10chan_sort(const void *v, const void *v2)
{
	const StatsChannel *t = (void *) v;
	const StatsChannel *t2 = (void *) v2;
	return (t2->joincounter - t->joincounter);
}

/*************************************************************************/

void html_top10chan(FILE * ptr)
{
	StatsChannel *s;
	lnode_t *tn;
	char *tempc;
	Exclude *e;
	int x = 0;

	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_CHAN_TOP10_HEADER));
		SET_SEGV_LOCATION();
		list_sort(StatsChanhead, top10chan_sort);
		tn = list_first(StatsChanhead);
		while (tn)
		{
			s = lnode_get(tn);
			if (SP_HTML && (s->secret || s->private))
			{
				tn = list_next(StatsChanhead, tn);
				continue;
			}
			if (s)
			{
				if (s->joincounter)
				{
					e = find_exclude(s->name, NULL);
					if (!e)
					{
						tempc = char_encode(s->name);
						if (s->in_use)
						{
							dfprintf(ptr,
							         langstring
							         (HTML_CHAN_TOP10_CONTENT), tempc,
							         s->joincounter);
						}
						else
						{
							dfprintf(ptr,
							         langstring
							         (HTML_CHAN_TOP10_OFFLINE), tempc,
							         s->joincounter);
						}
						free(tempc);
					}
				}
				x++;
				if (x > 10)
				{
					break;
				}
			}
			tn = list_next(StatsChanhead, tn);

		}
		fprintf(ptr, "%s", langstring(HTML_CHAN_TOP10_FOOTER));
		SET_SEGV_LOCATION();
	}
}

/*************************************************************************/

void html_top10_chanever_title(FILE * ptr)
{
	SET_SEGV_LOCATION();
	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_TOP10_CHANEVER_TITLE));
	}
}

/*************************************************************************/

void html_popularchan_title(FILE * ptr)
{
	SET_SEGV_LOCATION();
	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_POPULAR_TITLE));
	}
}
