/* HTML Ctcp
 *
 * (c) 2004-2014 Denora Team
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

#define MODULE_VERSION "2.0"
#define MODULE_NAME "html_ctcp"

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

	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (!denora->do_html)
	{
		alog(LOG_NORMAL,   "[%s] HTML output disabled, unloading module", MODULE_NAME);
		return MOD_STOP;
	}
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	h = createHTMLtag("!CLIENTSTATS!", html_ctcp_table);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !CLIENTSTATS! [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	h = createHTMLtag("!TOP10CLIENTVERTITLE!", html_top10client_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TOP10CLIENTVERTITLE! [%d][%s]",
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

/*************************************************************************/

void html_ctcp_table(FILE * ptr)
{
	CTCPVerStats *c;
	char *tempc;
	int rows;
	sqlite3_stmt * stmt;
	char ***data;
	int i;

	if (ptr)
	{
		fprintf(ptr, "%s", getstring(NULL, HTML_CTCP_HEADER));
		CTCPDatabase = DenoraOpenSQL(AdminDB);
		rows = DenoraSQLGetNumRows(CTCPDatabase, "version");
		stmt = DenoraPrepareQuery(CTCPDatabase, "SELECT version, overall FROM %s ORDER BY overall LIMIT 10", CTCPTable);
		data = DenoraSQLFetchArray(CTCPDatabase, CTCPTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
				tempc = char_encode(data[i][0]);
				dfprintf(ptr, getstring(NULL, HTML_CTCP_CONTENT),
				         tempc, data[i][1]);
				free(tempc);
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(CTCPDatabase);
		fprintf(ptr, "%s", getstring(NULL, HTML_CTCP_FOOTER));
	}
}

/*************************************************************************/

void html_top10client_title(FILE * ptr)
{
	
	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_TOP10_CLIENT_TITLE));
	}
}
