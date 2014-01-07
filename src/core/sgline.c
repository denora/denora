/* SGLINE Bans 
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

void sql_do_sgline(char *mask, char *reason);
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
	
	if (!ircd->sgline)
	{
		alog(LOG_NORMAL,   "[%s] ircd does not support SGLINE command unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	SGline_Parse_Handler(sql_do_sgline);

	
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
 * Parse SGLINE message for SQL
 * Note that the length should indicated where to split the mask into two pieces
 *
 * @param length number of characters that will indicate the split point in mask and reason
 * @param mask this holds both the mask and reason separated by a :
 * @return void - no returend value
 *
 */
void sql_do_sgline(char *length, char *mask)
{
	long int len;               /* length when converted to integer */
	char *reason;               /* reason for sgline                */
	int errsave;
	int res;


	if (!length || !mask)
	{
		return;
	}

	errsave = errno;            /* temp copy the variable so we can reset the error state */
	errno = 0;                  /* make the errno 0 so we don't report wrong errors */

	len = strtol(length, NULL, 10);
	/*
	 * Check error state, to see if we were overflowed
	 */
	if (errno == ERANGE)
	{
		alog(LOG_DEBUG, "%s strtol() set errno to ERANGE possible %s",
		     PRETTY_FUNCTION,
		     (len == LONG_MAX ? "overflow" : "underflow"));
	}
	errno = errsave;

	if (((int) strlen(mask) > len) && (mask[len]) == ':')
	{
		mask[len] = '\0';
		reason = mask + len + 1;
	}
	else
	{
		return;
	}

	res = DenoraSQLGetNumRowsFromQuery(DenoraDB, "SELECT mask FROM %s WHERE mask = \'%q\' LIMIT 1",
	 SglineTable, mask);

	if (res == 0)
	{
			DenoraSQLQuery(DenoraDB, "INSERT INTO %s (mask, reason) values('%s', '%s')",
			          SglineTable, mask, reason);
	}
	else
	{
			DenoraSQLQuery(DenoraDB, "UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
			          SglineTable, reason, mask);
	}

	return;
}
