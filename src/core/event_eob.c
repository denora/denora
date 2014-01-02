/* Event Uline
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
#define MODULE_NAME "event_eob"

static int denora_event_synccomplete(int ac, char **av);
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
	EvtHook *hook;
	int status;

	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	if (!ircd->syncstate)
	{
		alog(LOG_NORMAL,   "[%s] IRCd Does not support reporting sync state disabling ", MODULE_NAME);
		return MOD_STOP;
	}
	
	if (DisableTLD)
	{
		alog(LOG_NORMAL,   "[%s] TLD feature is turned off, module being unloaded", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	hook =
	    createEventHook(EVENT_UPLINK_SYNC_COMPLETE,
	                    denora_event_synccomplete);
	status = moduleAddEventHook(hook);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting hook for EVENT_UPLINK_SYNC_COMPLETE [%d][%s]", MODULE_NAME, status,
		     ModuleGetErrStr(status));
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

static int denora_event_synccomplete(int ac, char **av)
{
	int i, rows;
	sqlite3_stmt *stmt;
	sqlite3 *db;
	char ***data;

	USE_VAR(ac);
	USE_VAR(av);

	db = DenoraOpenSQL(DenoraDB);
	rows = DenoraSQLGetNumRows(db, TLDTable);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", TLDTable);
	data = DenoraSQLFetchArray(db, TLDTable, stmt, FETCH_ARRAY_NUM);
	for (i = 0; i < rows; i++)
	{
			sql_do_tld(UPDATE, data[i][0], data[i][1], atoi(data[i][2]), atoi(data[i][3]));
	}
	free(data);
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);
	return MOD_CONT;
}

/*************************************************************************/
