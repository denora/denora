/* StatServ core functions
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
#define MODULE_NAME "ss_status"

static int do_status(User * u, int ac, char **av);
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
	Command *c;
	int status;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);	
	moduleSetType(CORE);

	c = createCommand("STATUS", do_status, is_oper, -1, -1, -1,
	                  STAT_HELP_STATUS);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting Command for STATUS [%d][%s]",
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

/* STATUS command. */

static int do_status(User * u, int ac, char **av)
{
	char *what = NULL;

	if (ac >= 1)
	{
		what = av[0];
	}

	if (!what)
	{
		notice_lang(s_StatServ, u, STAT_STATUS_SERVER_COUNT, NetworkName,
		            stats->servers, stats->servers_max);
		notice_user(s_StatServ, u, " ");
		notice_lang(s_StatServ, u, STAT_STATUS_CHAN_COUNT, NetworkName,
		            stats->chans, stats->chans_max);
		notice_user(s_StatServ, u, " ");
		notice_lang(s_StatServ, u, STAT_STATUS_USER_COUNT, NetworkName,
		            stats->users, stats->users_max);
		if (StatsPage)
		{
			notice_user(s_StatServ, u, " ");
			notice_lang(s_StatServ, u, STAT_STATUS_PAGE, StatsPage);
		}
	}
	else if (!stricmp(what, "SERVERS"))
	{
		notice_lang(s_StatServ, u, STAT_STATUS_SERVER_COUNT, NetworkName,
		            stats->servers, stats->servers_max);
	}
	else if (!stricmp(what, "CHANNELS"))
	{
		notice_lang(s_StatServ, u, STAT_STATUS_CHAN_COUNT, NetworkName,
		            stats->chans, stats->chans_max);
	}
	else if (!stricmp(what, "USERS"))
	{
		notice_lang(s_StatServ, u, STAT_STATUS_USER_COUNT, NetworkName,
		            stats->users, stats->users_max);
	}
	else
	{
		syntax_error(s_StatServ, u, "STATUS", STAT_STATUS_SYNTAX);
	}

	return MOD_CONT;
}
