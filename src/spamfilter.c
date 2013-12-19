
/*
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

#include "denora.h"

sqlite3 *SpamDatabase;

/*************************************************************************/

/**
 * Create a new entry in the TLD structs
 *
 * @param country is the two letter code for the country
 * @return TLD struct
 *
 */
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
	char **data;

	if (!regex)
	{
		return NULL;
	}

	SpamDatabase = DenoraOpenSQL(spamDB);
	stmt = DenoraPrepareQuery(SpamDatabase, "SELECT * FROM %s WHERE regex='%q'", regex);
	data = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (data) {
		a = new_SpamFilter(data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[0]);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(SpamDatabase);

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
	SQLres *sql_res;
	int sqlid;
	SpamFilter *sf;
	char *sqlaction, *sqlreason, *sqlregex;
	char **sql_row;
	char **data;
	

	sf = findSpamFilter(regex);
	if (sf)
	{
		if (sf->duration == (int32) strtoul(duration, NULL, 10)
		        && sf->setat == (int32) strtoul(setat, NULL, 10))
		{
			return;
		}
	}
	else
	{
		new_SpamFilter(target, action, setby, expires, setat, duration,
		               reason, regex);
	}

	if (sf)
	{
		DenoraExecQuerySQL(SpamDatabase, "UPDATE %s SET regex=\'%q\' target=\'%q\', action=\'%q\', setby=\'%q\', expires=%ld, setat=%ld, duration=%ld, reason=\'%q\'",
			 SpamTable, regex, target, sqlaction, setby, strtoul(expires, NULL, 10),
			 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
			 sqlreason);

		if (denora->do_sql)
		{
			sql_query(
				 "UPDATE %s SET target=\'%s\', action=\'%q\', setby=\'%s\', expires=%ld, setat=%ld, duration=%ld, reason=\'%q\' WHERE id=\'%d\'",
				 SpamTable, target, action, setby, strtoul(expires, NULL, 10),
				 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
			 	reason, sf->sqlid);
		}
		return;
	}
	else
	{
		DenoraExecQuerySQL(SpamDatabase, "INSERT INTO %s (target, action, setby, expires, setat, duration, reason, regex) VALUES(\'%s\',\'%s\',\'%s\',%ld, %ld,%ld, \'%s\', \'%s\')",
				SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10),   strtoul(duration, NULL, 10), reason, regex);


		if (denora->do_sql)
		{
			sql_query("INSERT INTO %s (target, action, setby, expires, setat, duration, reason, regex) VALUES(\'%s\',\'%s\',\'%s\',%ld, %ld,%ld, \'%s\', \'%s\') \
				ON DUPLICATE KEY UPDATE target=%q, action=%q, setby=%q, expires=%ld, setat=%ld, duration=%ld, reason=%q, regex=%q",
				SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10), 
		          strtoul(duration, NULL, 10), reason, regex, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10), 
		          strtoul(duration, NULL, 10), reason, regex);
		}
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

	if (denora->do_sql)
	{
		/*
		 * Query the spam table and remove bans that match the following
	 	* 1. Target
	 	* 2. Action text
	 	* 3. Regex
	 	*/
		sql_query("DELETE FROM %s WHERE target=\'%q\' and action=\'%q\' and regex=\'%q\'",
	          SpamTable, target, action, regex);
		return;
	}

	


	

	return;
}
