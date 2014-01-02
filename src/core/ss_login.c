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
#define MODULE_NAME "ss_login"

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
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	c = createCommand("LOGIN", do_login, is_oper, STAT_HELP_LOGIN, -1, -1,
	                  -1);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for LOGIN [%d][%s]",
		     MODULE_NAME, status, ModuleGetErrStr(status));
		return MOD_STOP;
	}
	c = createCommand("LOGOUT", do_logout, NULL, STAT_HELP_LOGOUT, -1, -1,
	                  -1);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for LOGOUT [%d][%s]",
		     MODULE_NAME, status, ModuleGetErrStr(status));
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
		DenoraSQLUpdateInt(DenoraDB, UserTable, "admin", 0, "nick", u->nick);
		alog(LOG_NORMAL, "%s Attempted to login but is not a admin", u->nick);
		return MOD_CONT;
	}
	else
	{
		if (ValidPassword(av[1], aadmin->passwd))
		{
			DenoraSQLUpdateInt(DenoraDB, UserTable, "admin", 1, "nick", u->nick);
			if (aadmin->configfile == 1)
			{
				DenoraSQLUpdateInt(DenoraDB, UserTable, "confadmin", 1, "nick", u->nick);
			}
			else
			{
				DenoraSQLUpdateInt(DenoraDB, UserTable, "confadmin", 0, "nick", u->nick);
			}
			/* Use the language set in denora.conf */
			if (aadmin->language)
			{
				DenoraSQLUpdateInt(DenoraDB, UserTable, "language", aadmin->language, "nick", u->nick);
			}
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
		if (aadmin)
		{
			free_admin(aadmin);
		}
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
		DenoraSQLUpdateInt(DenoraDB, UserTable, "admin", 0, "nick", u->nick);
		DenoraSQLUpdateInt(DenoraDB, UserTable, "confadmin", 0, "nick", u->nick);
		denora_cmd_global(s_StatServ, langstring(STATS_ADMIN_LOGOUT),
		                  u->nick);
		notice_lang(s_StatServ, u, STAT_ADMIN_LOGGED_OUT);
	}
	return MOD_CONT;
}

/*************************************************************************/
