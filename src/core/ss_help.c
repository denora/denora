/* StatServ core functions
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

static int do_help(User * u, int ac, char **av);
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
	moduleAddAuthor("Denora");
	moduleAddVersion("");
	moduleSetType(CORE);

	c = createCommand("HELP", do_help, NULL, -1, -1, -1, -1);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting Command for HELP [%d][%s]",
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

/* HELP command. */

static int do_help(User * u, int ac, char **av)
{
	

	if (ac < 1)
	{
		notice_help(s_StatServ, u, STAT_HELP);
		if (is_stats_admin(u))
		{
			notice_help(s_StatServ, u, STAT_HELP_ADMIN_CMD);
		}
		moduleDisplayHelp(1, u);
	}
	else
	{
		if (!BadChar(av[0]))
		{
			mod_help_cmd(s_StatServ, u, STATSERV, av[0]);
		}
		else
		{
			notice_lang(s_StatServ, u, NO_HELP_AVAILABLE, av[0]);
		}
	}
	return MOD_CONT;
}

/*************************************************************************/
