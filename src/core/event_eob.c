/* Event Uline
 *
 * (c) 2004-2013 Denora Team
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

int denora_event_synccomplete(int ac, char **av);
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

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	if (!ircd->syncstate)
	{
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("");
	moduleSetType(CORE);

	hook =
	    createEventHook(EVENT_UPLINK_SYNC_COMPLETE,
	                    denora_event_synccomplete);
	status = moduleAddEventHook(hook);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for EVENT_UPLINK_SYNC_COMPLETE [%d][%s]", status,
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

int denora_event_synccomplete(int ac, char **av)
{
	TLD *t;
	lnode_t *tn;

	USE_VAR(ac);
	USE_VAR(av);

	

	tn = list_first(Thead);
	while (tn != NULL)
	{
		t = lnode_get(tn);
		sql_do_tld(UPDATE, t->countrycode, t->country, t->count,
		           t->overall);
		tn = list_next(Thead, tn);
	}
	return MOD_CONT;
}

/*************************************************************************/
