/* XLINE Bans 
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
/*************************************************************************/

#include "denora.h"

#define MODULE_VERSION "2.0"
#define MODULE_NAME "sgline"

void sql_do_xline(char *geos, char *reason);
void sql_do_unxline(char *geos);

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
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (!ircd->xline)
	{
		alog(LOG_NORMAL,   "[%s] ircd does not support XLINE/UNXLINE command unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	xline_add_Handler(sql_do_xline);
	xline_del_Handler(sql_do_unxline);


	
	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

/**
 * Parse XLINE message for SQL
 *
 * @param geos mask that will be banned by XLINE
 * @param reason is the reason for the XLINE
 * @return void - no returend value
 *
 */
void sql_do_xline(char *geos, char *reason)
{
	int res;

	res = DenoraSQLGetNumRowsFromQuery(DenoraDB, "SELECT mask FROM %s WHERE mask = \'%q\' LIMIT 1",
	 SglineTable, geos);

	if (res == 0)
	{
		DenoraSQLQuery(DenoraDB, "INSERT INTO %s (mask, reason) values('%q', '%q')",
			          SglineTable, geos, reason);
	}
	else
	{
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET reason=\'%q\' WHERE mask=\'%q\'",
			          SglineTable, reason, geos);
	}
	return;
}

/*************************************************************************/

/**
 * Parse UNXLINE message for SQL
 * @param geos mask that will be removed by UNXLINE
 * @return void - no returend value
 */
void sql_do_unxline(char *geos)
{
	if (!geos)
	{
		return;
	}

	DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE mask=\'%s\'",
		  SglineTable, geos);

	return;
}

