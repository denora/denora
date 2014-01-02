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
#define MODULE_NAME "ss_core"

static int do_shutdown(User * u, int ac, char **av);
static int do_restart(User * u, int ac, char **av);
static int do_reload(User * u, int ac, char **av);
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

	c = createCommand("SHUTDOWN", do_shutdown, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_SHUTDOWN);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for LOGIN [%d][%s]",
		     MODULE_NAME, status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	c = createCommand("RESTART", do_restart, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_RESTART);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for LOGIN [%d][%s]",
		     MODULE_NAME, status, ModuleGetErrStr(status));
		return MOD_STOP;
	}
	c = createCommand("RELOAD", do_reload, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_RELOAD);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for LOGIN [%d][%s]",
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

/* SHUTDOWN command. */

static int do_shutdown(User * u, int ac, char **av)
{
	char buf[BUFSIZE];
	

	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}

	if (!u->confadmin)
	{
		notice_lang(s_StatServ, u, PERMISSION_DENIED);
		return MOD_CONT;
	}

	denora->qmsg = calloc(512 + strlen(u->nick), 1);
	if (!denora->qmsg)
	{
		ircsnprintf(denora->qmsg, 512,
		            "SHUTDOWN command received, but out of memory!");
	}
	else
	{
		ircsnprintf(buf, BUFSIZE, "SHUTDOWN command received from %s",
		            u->nick);
		denora->qmsg = sstrdup(buf);
	}


	denora->save_data = 1;
	denora->delayed_quit = 1;
	return MOD_CONT;
}

/*************************************************************************/

/* RESTART command */

static int do_restart(User * u, int ac, char **av)
{
	char buf[BUFSIZE];
	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}

	if (!u->confadmin)
	{
		notice_lang(s_StatServ, u, PERMISSION_DENIED);
		return MOD_CONT;
	}
#ifdef STATS_BIN
	denora->qmsg = calloc(512 + strlen(u->nick), 1);
	if (!denora->qmsg)
	{
		ircsnprintf(denora->qmsg, 512,
		            "RESTART command received, but out of memory!");
	}
	else
	{
		ircsnprintf(buf, BUFSIZE, "RESTART command received from %s",
		            u->nick);
		denora->qmsg = sstrdup(buf);
	}

	do_restart_denora();
#else
	notice_lang(s_StatServ, u, STAT_CANNOT_RESTART);
#endif
	return MOD_CONT;
}

/*************************************************************************/

static int do_reload(User * u, int ac, char **av)
{
	int i;

	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}

	send_event(EVENT_RELOAD, 1, EVENT_START);

	if (!DenoraParseXMLConfig((char *) denora->config))
	{
		denora->qmsg = calloc(512 + strlen(u->nick), 1);
		if (!denora->qmsg)
			ircsnprintf(denora->qmsg, 512,
			            "Error during the reload of the configuration file, but out of memory!");
		else
			denora->qmsg = sstrdup("Error Reading Configuration File");
		denora->quitting = 1;
	}

	send_event(EVENT_RELOAD, 1, EVENT_STOP);

	notice_lang(s_StatServ, u, STAT_RELOAD);
	return MOD_CONT;
}

/*************************************************************************/
