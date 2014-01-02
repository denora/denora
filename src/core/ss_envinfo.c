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
#define MODULE_NAME "ss_envinfo"

static int do_envinfo(User * u, int ac, char **av);
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

	c = createCommand("ENVINFO", do_envinfo, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_ENVINFO);
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

/* ENVINFO command */
static int do_envinfo(User * u, int ac, char **av)
{
	int uptime;

	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}

	uptime = time(NULL) - denora->start_time;

	notice_lang(s_StatServ, u, STAT_ENVINFO_HEADER);
	/* version, protocol, uplink */
	notice_lang(s_StatServ, u, STAT_ENVINFO_VERSION,
	            denora->versiondotted);
	notice_lang(s_StatServ, u, STAT_ENVINFO_PROTOCOL,
	            denora->version_protocol);
	notice_lang(s_StatServ, u, STAT_ENVINFO_UPLINK, denora->uplink);
	/* Yes/No responses */
	notice_lang(s_StatServ, u, STAT_ENVINFO_SQL,
	            denora->do_sql ? langstring(SAY_YES) : langstring(SAY_NO));
	notice_lang(s_StatServ, u, STAT_ENVINFO_HTML,
	            denora->do_html ? langstring(SAY_YES) :
	            langstring(SAY_NO));
	notice_lang(s_StatServ, u, STAT_ENVINFO_DEBUG,
	            denora->debug ? langstring(SAY_YES) : langstring(SAY_NO));
	/* How many modules loaded */
	notice_lang(s_StatServ, u, STAT_ENVINFO_MODULESLOADED, moduleCount(0));
	/* Language in use */
	notice_lang(s_StatServ, u, STAT_ENVINFO_LANGUAGE,
	            langstring(LANG_NAME));
	/* Stats uptime information */
	if (uptime / 86400 == 1)
		notice_lang(s_StatServ, u, STATS_UPTIME_1DHMS, uptime / 86400,
		            (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
	else
		notice_lang(s_StatServ, u, STATS_UPTIME_DHMS, uptime / 86400,
		            (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
	/* End of ENVINFO */
	notice_lang(s_StatServ, u, STAT_ENVINFO_FOOTER);

	return MOD_CONT;
}
