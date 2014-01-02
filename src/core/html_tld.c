/* HTML TLD
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
#define MODULE_NAME "html_tld"

void html_tld_table(FILE * ptr);
void html_tld_title(FILE * ptr);
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

	h = createHTMLtag("!TLDMAP!", html_tld_table);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TLDMAP! [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	h = createHTMLtag("!TLDTITLE!", html_tld_title);
	status = addHTMLTag(h);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for !TLDTITLE! [%d][%s]",
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

void html_tld_table(FILE * ptr)
{
	char *tempc, *tempcc;
	int i, rows;
	sqlite3_stmt *stmt;
	char ***data;

	if (ptr)
	{
		fprintf(ptr, "%s", langstr(HTML_TLD_HEADER));
		TLDDatabase = DenoraOpenSQL(TLDDB);
		rows = DenoraSQLGetNumRows(TLDDatabase, TLDTable);
		stmt = DenoraPrepareQuery(TLDDatabase, "SELECT * FROM %s ORDER BY overall LIMIT 10", TLDTable);
		data = DenoraSQLFetchArray(TLDDatabase, TLDTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
				tempcc = char_encode(data[i][0]);
				tempc = char_encode(data[i][1]);
				dfprintf(ptr, langstr(HTML_TLD_CONTENT),
				         tempcc, tempc, atoi(data[i][2]), atoi(data[i][3]));
				free(tempcc);
				free(tempc);
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(TLDDatabase);

		fprintf(ptr, "%s", langstr(HTML_TLD_FOOTER));
	}
}

/*************************************************************************/

void html_tld_title(FILE * ptr)
{
	
	if (ptr)
	{
		fprintf(ptr, "%s", langstring(HTML_TLD_TITLE));
	}
}

/*************************************************************************/
