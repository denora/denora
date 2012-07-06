/* HTML Ctcp
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

void html_ctcp_table(FILE * ptr);
void html_top10client_title(FILE * ptr);

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

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	moduleAddAuthor("Denora");
	moduleAddVersion
	("");
	moduleSetType(CORE);

	h = createHTMLtag("!CLIENTSTATS!", html_ctcp_table);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !CLIENTSTATS! [%d][%s]",
		     status, ModuleGetErrStr(status));
	}

	h = createHTMLtag("!TOP10CLIENTVERTITLE!", html_top10client_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TOP10CLIENTVERTITLE! [%d][%s]",
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

void html_ctcp_table(FILE * ptr)
{
	CTCPVerStats *c;
	lnode_t *tn;
	char *tempc;
	int x = 0;

	if (ptr)
	{
		fprintf(ptr, "%s", getstring(NULL, HTML_CTCP_HEADER));
		list_sort(CTCPhead, sortctcp);
		tn = list_first(CTCPhead);
		while (tn)
		{
			c = lnode_get(tn);
			if (c->overall)
			{
				tempc = char_encode(c->version);
				dfprintf(ptr, getstring(NULL, HTML_CTCP_CONTENT),
				         tempc, c->overall);
				DenoraFree(tempc);
				x++;
				if (x > 10)
				{
					break;
				}
			}
			tn = list_next(CTCPhead, tn);
		}
		fprintf(ptr, "%s", getstring(NULL, HTML_CTCP_FOOTER));
	}
}

/*************************************************************************/

void html_top10client_title(FILE * ptr)
{
	SET_SEGV_LOCATION();
	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_TOP10_CLIENT_TITLE));
	}
}
