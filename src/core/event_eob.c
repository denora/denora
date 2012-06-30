/* Event Uline
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

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	moduleAddAuthor("Denora");
	moduleAddVersion
	("");
	moduleSetType(CORE);

	hook =
	    createEventHook(EVENT_UPLINK_SYNC_COMPLETE,
	                    denora_event_synccomplete);
	moduleAddEventHook(hook);

#ifdef USE_MYSQL
	if (ircd->syncstate)
	{
		return MOD_CONT;
	}
	else
	{
		return MOD_STOP;
	}
#else
	return MOD_STOP;
#endif
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

int denora_event_synccomplete(__attribute__((unused))int ac, __attribute__((unused))char **av)
{
	TLD *t;
	lnode_t *tn;

	SET_SEGV_LOCATION();

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
