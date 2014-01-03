/* Spamfilter
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
/*************************************************************************/

#include "denora.h"

#define MODULE_VERSION "2.0"
#define MODULE_NAME "spamfilter"

int denora_event_229(char *source, int ac, char **av);
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
	Message *m;
	int status;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (!ircd->spamfilter)
	{
		alog(LOG_NORMAL,   "[%s] ircd does not support spamfilters unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	/* spam filter */
	m = createMessage("229", denora_event_229);
	status = moduleAddMessage(m, MOD_HEAD);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 229 [%d][%s]", status,
		     ModuleGetErrStr(status));
		return MOD_STOP;
	}
	hook = createEventHook(EVENT_SERVER, denora_event_requestspam);
	status = moduleAddEventHook(hook);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting hook for EVENT_SERVER [%d][%s]", MODULE_NAME, status,
		     ModuleGetErrStr(status));
		return MOD_STOP;
	}
	
	SpamFilter_Add_Handler(sql_do_server_spam_add);
	SpamFilter_Del_Handler(sql_do_server_spam_remove);
	SpamFilter_Find_Handler(findSpamFilter);
	SpamFilter_Fini_Handler(fini_SpamFilter);
	
	
	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

/*
[Dec 13 15:11:09.442539 2005] Source Luna.NomadIrc.Net
[Dec 13 15:11:09.442593 2005] av[0] = Denora
[Dec 13 15:11:09.442647 2005] av[1] = f
[Dec 13 15:11:09.442701 2005] av[2] = p
[Dec 13 15:11:09.442755 2005] av[3] = gline
[Dec 13 15:11:09.442810 2005] av[4] = 0
[Dec 13 15:11:09.442863 2005] av[5] = 781731
[Dec 13 15:11:09.442918 2005] av[6] = 7776000
[Dec 13 15:11:09.442972 2005] av[7] = [SPAMFILTER]_:_Banned_for_spam_/_contact_kline@nomadirc.net
[Dec 13 15:11:09.443029 2005] av[8] = Luna.NomadIrc.Net
[Dec 13 15:11:09.443084 2005] av[9] = .*FOR MATRIX 2 DOWNLOAD.*
*/
/**
 * Process numeric 229 messages
 *
 * @param source is the server that sent the message
 * @param ac is the array count
 * @param av is the array
 *
 * @return return is always MOD_CONT
 *
 */
int denora_event_229(char *source, int ac, char **av)
{
	
	if (denora->protocoldebug)
	{
		protocol_debug(source, ac, av);
	}
	if (!stricmp("f", av[1]))
	{
		sql_do_server_spam_add(av[2], av[3], av[8], av[4], av[5], av[6],
		                       av[7], av[9]);
	}
	return MOD_CONT;
}

/*************************************************************************/

static int denora_event_requestspam(int ac, char **av)
{
	denora_cmd_stats(s_StatServ, ircd->spamfilter, av[0]);
}

/*************************************************************************/

SpamFilter *new_SpamFilter(char *target, char *action,
                            char *setby, char *expires, char *setat,
                            char *duration, char *reason, char *regex)
{
	SpamFilter *sf;

	sf = malloc(sizeof(SpamFilter));
	sf->regex = sstrdup(regex);
	sf->target = sstrdup(target);
	sf->action = sstrdup(action);
	sf->setby = sstrdup(setby);
	sf->expires = sstrdup(expires);
	sf->setat = strtoul(setat, NULL, 10);
	sf->duration = strtoul(duration, NULL, 10);
	sf->reason = sstrdup(reason);
	return sf;
}

/*************************************************************************/

SpamFilter *findSpamFilter(const char *regex)
{
	SpamFilter *a;
	sqlite3_stmt *stmt;
	sqlite3 *db;
	char **data;

	if (!regex)
	{
		return NULL;
	}

	db = DenoraOpenSQL(DenoraDB);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s WHERE regex='%q'", regex);
	data = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (data) {
		a = new_SpamFilter(data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[0]);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	if (stricmp(a->regex, regex) == 0)
	{
		return a;
	}
	return NULL;
}

/*************************************************************************/

void fini_SpamFilter(SpamFilter *sf)
{
	if (sf->regex)
		free(sf->regex);
	if (sf->target)
		free(sf->target);
	if (sf->action)
		free(sf->action);
	if (sf->setby)
		free(sf->setby);
	if (sf->expires)
		free(sf->expires);
	if (sf->reason)
		free(sf->reason);
	free(sf);
}


/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages into SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param setby is the person or server that has set the ban
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param setat is the unix timestamp of when the ban was set
 * @param duration is the number of seconds the ban will last
 * @param reason is the reason for the ban
 * @param regex is the regular expression for the spam filter
 *
 * @return void - no returend value
 */
void sql_do_server_spam_add(char *target, char *action,
                            char *setby, char *expires, char *setat,
                            char *duration, char *reason, char *regex)
{
	SpamFilter *sf;

	sf = findSpamFilter(regex);
	if (sf)
	{
		if (sf->duration != (int32) strtoul(duration, NULL, 10) && sf->setat != (int32) strtoul(setat, NULL, 10))
		{
			DenoraExecQuerySQL(DenoraDB, "UPDATE %s SET regex=\'%q\' target=\'%q\', action=\'%q\', setby=\'%q\', expires=%ld, setat=%ld, duration=%ld, reason=\'%q\'",
				 SpamTable, regex, target, sqlaction, setby, strtoul(expires, NULL, 10),
				 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
				 sqlreason);
		}
		fini_SpamFilter(sf);
		return;
	}
	else
	{
		DenoraExecQuerySQL(DenoraDB, "INSERT INTO %s (target, action, setby, expires, setat, duration, reason, regex) VALUES(\'%s\',\'%s\',\'%s\',%ld, %ld,%ld, \'%s\', \'%s\')",
				SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10),   strtoul(duration, NULL, 10), reason, regex);
	}
	return;
}


/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages remove from SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param regex is the regular expression that made up the ban
 *
 * @return void - no returend value
 */
void sql_do_server_spam_remove(char *target, char *action, char *regex)
{
	if (!regex || !action || !target)
	{
		return;
	}

	DenoraExecQuerySQL(SpamDatabase, "DELETE FROM %s WHERE target=\'%q\' and action=\'%q\' and regex=\'%q\'",
	          SpamTable, target, action, regex);

	return;
}

void spamfilter_del_expired(void)
{
	DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE expires != 0 AND expires < %ld", SpamTable, time(NULL));
}