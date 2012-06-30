/*
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

int do_fantasy(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the hook, and tell Denora about it.
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

	hook = createEventHook(EVENT_FANTASY, do_fantasy);
	moduleAddEventHook(hook);

	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/**
 * Handle seen fantasy command.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
	User *u;
	if (argc < 3)
		return MOD_CONT;

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	if (stricmp(argv[0], "help") == 0)
	{
		u = finduser(argv[1]);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_1);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_2, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_3, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_4, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_5, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_6, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_7, ChanStatsTrigger);
		notice_lang(s_StatServ, u, STATS_HELP_CHANNEL_8, ChanStatsTrigger);
	}

	return MOD_CONT;
}
