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
	char *chan, *target, *sqltarget;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif
	ChannelStats *cs;

	if (argc < 3)
		return MOD_CONT;

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	if (stricmp(argv[0], "stats") == 0)
	{
		if (!(u = finduser(argv[1])))
			return MOD_CONT;
		if (argc == 3)
		{
			if (!u->sgroup)
				return MOD_CONT;
			target = u->nick;
			sqltarget = sstrdup(u->sgroup);
		}
		else
		{
			target = strtok(argv[3], " ");
			sqltarget = rdb_escape(target);
			rdb_query(QUERY_HIGH, "SELECT uname FROM %s WHERE nick=\'%s\' ", AliasesTable, sqltarget);
#ifdef USE_MYSQL
			mysql_res = mysql_store_result(mysql);
			if (mysql_res && mysql_num_rows(mysql_res))
			{
				mysql_row = mysql_fetch_row(mysql_res);
				free(sqltarget);
				sqltarget = rdb_escape(mysql_row[0]);
			}
			else
			{
				free(sqltarget);
				return MOD_CONT;
			}
#endif
		}
		chan = rdb_escape(argv[2]);
		cs = find_cs(argv[2]);
		rdb_query(QUERY_HIGH, "SELECT * FROM %s WHERE chan=\'%s\' AND type=0 AND uname=\'%s\';",
		          UStatsTable, chan, sqltarget);
		free(chan);
		free(sqltarget);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_num_rows(mysql_res) > 0)
		{
			SET_SEGV_LOCATION();
			while ((mysql_row = mysql_fetch_row(mysql_res)) != NULL)
			{
				if (cs->flags & CS_NOTICE)
				{
					notice_lang(s_StatServ, u, STATS_USER_CHANNEL, target, argv[2]);
					notice_lang(s_StatServ, u, STATS_MESSAGE_ONE,
					            mysql_row[3], mysql_row[4], mysql_row[5], mysql_row[7], mysql_row[6]);
				}
				else
				{
					denora_cmd_privmsg(s_StatServ, argv[2], getstring(NULL, STATS_USER_CHANNEL),
					                   target, argv[2]);
					denora_cmd_privmsg(s_StatServ, argv[2], getstring(NULL, STATS_MESSAGE_ONE),
					                   mysql_row[3], mysql_row[4], mysql_row[5], mysql_row[7], mysql_row[6]);
				}

			}
		}
		SET_SEGV_LOCATION();
		mysql_free_result(mysql_res);
#endif
	}

	return MOD_CONT;
}
