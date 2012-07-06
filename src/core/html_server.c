/* HTML Server
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

void html_server_table(FILE * ptr);
void html_serverlist_title(FILE * ptr);

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

	h = createHTMLtag("!SRVLIST!", html_server_table);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !SRVLIST! [%d][%s]",
		     status, ModuleGetErrStr(status));
	}

	h = createHTMLtag("!SERVERLISTTILE!", html_serverlist_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !SERVERLISTTILE! [%d][%s]",
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

void html_server_table(FILE * ptr)
{
	ServStats *s, *next;
	Server *serv;
	char *tempc;
	SET_SEGV_LOCATION();

	if (ptr)
	{
		fprintf(ptr, "%s", getstring(NULL, HTML_SERVER_HEADER));
		SET_SEGV_LOCATION();
		s = first_statsserver();
		while (s)
		{
			next = next_statserver();
			serv = server_find(s->name);
			if (serv)
			{
				if (HideUline
				        && (serv->uline || serv->flags == SERVER_JUPED))
				{
					s = next;
					continue;
				}
			}
			tempc = char_encode(s->name);
			if (s->split_stats)
			{
				dfprintf(ptr, getstring(NULL, HTML_SERVER_CONTENT),
				         s->name, tempc);
			}
			else
			{
				dfprintf(ptr,
				         getstring(NULL, HTML_SERVER_CONTENT_ONLINE),
				         s->name, tempc);
			}
			DenoraFree(tempc);
			s = next;
		}
		fprintf(ptr, "%s", getstring(NULL, HTML_SERVER_FOOTER));
		SET_SEGV_LOCATION();
	}
}

/*************************************************************************/

void html_serverlist_title(FILE * ptr)
{
	SET_SEGV_LOCATION();
	if (ptr)
	{
		fprintf(ptr, "%s", langstr(HTML_SERVERLIST_TITLE));
	}
}
