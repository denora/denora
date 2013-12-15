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

	if (!denora->do_sql)
	{
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

/**
 * Handle seen fantasy command.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
	User *u;
	char *target, *sqltarget;
	SQLres *sql_res;
	ChannelStats *cs = NULL;
	char **sql_row;

	if (argc < 3)
		return MOD_CONT;

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	if (stricmp(argv[0], "gstats") == 0)
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
			sqltarget = sql_escape(target);
			sql_query("SELECT uname FROM %s WHERE nick=\'%s\' ",
				  AliasesTable, sqltarget);
			free(sqltarget);
			sql_res = sql_set_result(sqlcon);
			if (sql_res && sql_num_rows(sql_res))
			{
				sql_row = sql_fetch_row(sql_res);
				sqltarget = sql_escape(sql_row[0]);
			}
			else
			{
				return MOD_CONT;
			}
		}
		cs = find_cs(argv[2]);
		sql_query(
			  "SELECT * FROM %s WHERE chan=\'global\' AND type=0 AND uname=\'%s\';",
		          UStatsTable, sqltarget);
		free(sqltarget);
		sql_res = sql_set_result(sqlcon);
		if (sql_num_rows(sql_res) > 0)
		{
			
			while ((sql_row = sql_fetch_row(sql_res)) != NULL)
			{
				if (cs->flags & CS_NOTICE)
				{
					notice_lang(s_StatServ, u, STATS_USER_NETWORK,
					            target);
					notice_lang(s_StatServ, u, STATS_MESSAGE_ONE,
					            sql_row[3], sql_row[4],
					            sql_row[5], sql_row[7], sql_row[6]);
				}
				else
				{
					denora_cmd_privmsg(s_StatServ, argv[2],
					                   getstring(NULL,
					                             STATS_USER_NETWORK),
					                   target);
					denora_cmd_privmsg(s_StatServ, argv[2],
					                   getstring(NULL,
					                             STATS_MESSAGE_ONE),
					                   sql_row[3], sql_row[4],
					                   sql_row[5], sql_row[7],
					                   sql_row[6]);
				}

			}
		}
		
		sql_free_result(sql_res);
	}

	return MOD_CONT;
}
