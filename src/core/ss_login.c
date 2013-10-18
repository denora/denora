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

static int do_login(User * u, int ac, char **av);
static int do_logout(User * u, int ac, char **av);
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

	c = createCommand("LOGIN", do_login, is_oper, STAT_HELP_LOGIN, -1, -1,
	                  -1);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting Command for LOGIN [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}
	c = createCommand("LOGOUT", do_logout, NULL, STAT_HELP_LOGOUT, -1, -1,
	                  -1);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting Command for LOGOUT [%d][%s]",
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

/* LOGIN command */
static int do_login(User * u, int ac, char **av)
{
	Dadmin *aadmin;

	/* Required params */
	if (ac < 2)
	{
		syntax_error(s_StatServ, u, "LOGIN", STAT_LOGIN_SYNTAX);
		return MOD_CONT;
	}

	alog(LOG_NORMAL, "%s: %s: LOGIN %s ****", s_StatServ, u->nick, av[0]);

	/* Trying to login again? */
	if (u->admin)
	{
		notice_lang(s_StatServ, u, STAT_ADMIN_ALREADY_LOGGED);
		return MOD_CONT;
	}

	aadmin = find_admin(av[0], u);
	if (!aadmin)
	{
		u->admin = 0;
	}
	else
	{
		if (ValidPassword(av[1], aadmin->passwd))
		{
			u->admin = 1;
			if (aadmin->configfile == 1)
			{
				u->confadmin = 1;
			}
			else
			{
				u->confadmin = 0;
			}
			/* Use the language set in denora.conf */
			if (aadmin->language)
			{
				u->language = aadmin->language;
			}
		}
	}

	/* The user has logged in */
	if (u->admin)
	{
		denora_cmd_global(s_StatServ, langstring(NOW_STATS_ADMIN),
		                  u->nick, av[0]);
		notice_lang(s_StatServ, u, STAT_ADMIN_LOGGED_IN);
	}
	else
	{
		denora_cmd_global(s_StatServ, langstring(STATS_AUTH_FAILED),
		                  u->nick);
		notice_lang(s_StatServ, u, ACCESS_DENIED);
	}

	return MOD_CONT;
}

/*************************************************************************/

/* LOGOUT command */
static int do_logout(User * u, int ac, char **av)
{
	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}
	if (u->admin)
	{
		u->admin = 0;
		u->confadmin = 0;
		denora_cmd_global(s_StatServ, langstring(STATS_ADMIN_LOGOUT),
		                  u->nick);
		notice_lang(s_StatServ, u, STAT_ADMIN_LOGGED_OUT);
	}
	return MOD_CONT;
}

/*************************************************************************/
