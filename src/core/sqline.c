/* SQLINE Bans 
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
#define MODULE_NAME "sqline"

void sql_do_sqline(char *mask, char *reason);
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
	
	if (!ircd->sqline)
	{
		alog(LOG_NORMAL,   "[%s] ircd does not support SQLINE command unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	SQline_Parse_Handler(sql_do_sqline);

	
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
 * Parse SQLINE message for SQL
 *
 * @param mask The SQLINE mask that has been banned
 * @param reason The reason for the SQLINE ban
 *
 * @return void - no returend value
 */
void sql_do_sqline(char *mask, char *reason)
{
	int res;

	if (BadPtr(mask) || BadPtr(reason))
	{
		return;
	}

	res = DenoraSQLGetNumRowsFromQuery(DenoraDB, "SELECT mask FROM %s WHERE mask = \'%q\' LIMIT 1", SqlineTable, mask);
	if (res == 0)
	{
		DenoraSQLQuery(DenoraDB, "INSERT INTO %s (mask, reason) values('%q', '%q')", SqlineTable, mask, reason);
	}
	else
	{
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET reason=\'%q\' WHERE mask=\'%s\'", SqlineTable, reason, mask);
	}

	return;
}

