/* Cron Daily
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

int users_daily(const char *name);
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
	CronEvent *evt;
	int status;
	
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	moduleAddAuthor("Denora");
	moduleAddVersion
	("");
	moduleSetType(CORE);

	evt = createCronEvent(CRON_MIDNIGHT, users_daily);
	status = moduleAddCronEvent(evt);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting cron->users_daily [%d][%s]", status,
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

int users_daily(const char *name)
{
	SET_SEGV_LOCATION();

	if (name)
	{
		stats->daily_users = stats->users;
		stats->daily_users_time = time(NULL);
		stats->daily_opers = stats->opers;
		stats->daily_opers_time = time(NULL);
		stats->daily_servers = stats->servers;
		stats->daily_servers_time = time(NULL);
		stats->daily_chans = stats->chans;
		stats->daily_chans_time = time(NULL);
	}

	SET_SEGV_LOCATION();

	return MOD_CONT;
}
