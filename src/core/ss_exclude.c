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
#define MODULE_NAME "ss_exclude"

static int do_exclude(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * RFC: defination of a valid nick
 * nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
 * letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
 * digit      =  %x30-39                 ; 0-9
 * special    =  %x5B-60 / %x7B-7D       ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
 **/
#define isvalidnick(c) ( isalnum(c) || ((c) >='\x5B' && (c) <='\x60') || ((c) >='\x7B' && (c) <='\x7D') || (c)=='-' )


/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
	Command *c;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	c = createCommand("EXCLUDE", do_exclude, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_EXCLUDE);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for EXCLUDE [%d][%s]",
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

static int do_exclude(User * u, int ac, char **av)
{
	Exclude *e;
	char *tmp = NULL;
	char *s = NULL;
	char *name = NULL;
	int count = 0, from = 0, to = 0;
	int nnicks = 0, i;
	int disp = 1;
	char *ch = NULL;
	User *u2;
	int rows;
	sqlite3_stmt * stmt;
	char ***data;

	if (ac < 1)
	{
		notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
		return MOD_CONT;
	}

	if (!stricmp(av[0], "ADD"))
	{
		if (ac < 3)
		{
			notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
			return MOD_CONT;
		}
		if (!stricmp(av[1], "user"))
		{
			if (strlen(av[2]) > NICKMAX)
			{
				notice(s_StatServ, u->nick, "Invalid nick length");
				return MOD_CONT;
			}
			if (isdigit(av[2][0]) || av[2][0] == '-')
			{
				notice(s_StatServ, u->nick, "Invalid nick");
				return MOD_CONT;
			}
	
			e = find_exclude(av[2], EXCLUDE_USER);
			if (!e)
			{
				Create_Exclude(av[2], EXCLUDE_USER);
				notice_lang(s_StatServ, u, STAT_EXCLUDE_ADDED, av[2]);
				u2 = user_find(av[2]);
				sql_query( "DELETE FROM %s WHERE lower(`uname`)=lower(\'%q\')",
				          UStatsTable, u2 ? u2->sgroup : av[2]);
				sql_query(
				          "UPDATE `%s` SET `ignore`=\'Y\' WHERE lower(`uname`)=lower(\'%q\')",
				          AliasesTable, u2 ? u2->sgroup : av[2]);
			}
			else
			{
				del_exclude(e);
				notice_lang(s_StatServ, u, STAT_EXCLUDE_ALREADY, av[1]);
			}
		}
		else if (!stricmp(av[1], "server"))
		{

			e = find_exclude(av[1], EXCLUDE_SERVER);
			if (!e)
			{
				Create_Exclude(av[2], EXCLUDE_SERVER);
				notice_lang(s_StatServ, u, STAT_EXCLUDE_ADDED, av[2]);
			}
			else
			{
				del_exclude(e);
				notice_lang(s_StatServ, u, STAT_EXCLUDE_ALREADY, av[2]);
			}
		}
	}
	else if (!stricmp(av[0], "DEL"))
	{
		if (ac < 3)
		{
			notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
			return MOD_CONT;
		}
		if (!stricmp(av[1], "user"))
		{
			e = find_exclude(av[2], EXCLUDE_USER);
			if (e)
			{
				del_exclude(e);
				Delete_Exclude(av[2], EXCLUDE_USER);
				u->cstats = 0;
				notice_lang(s_StatServ, u, STAT_EXCLUDE_DELETED, av[2]);
				u2 = user_find(av[1]);
				sql_query(
				          "UPDATE `%s` SET `ignore`=\'N\' WHERE lower(`uname`)=lower(\'%q\')",
				          AliasesTable, u2 ? u2->sgroup : av[2]);
				for (i = 0; i < 4; i++)
				{
					sql_query("INSERT IGNORE INTO %s SET uname=\'%q\', chan=\'global\', type=%i;",
					 UStatsTable, u2 ? u2->sgroup : av[2], i);
				}
			}
			else
			{
				notice_lang(s_StatServ, u, STAT_EXCLUDE_NOT_FOUND, av[2]);
			}
		}
		else if (!stricmp(av[1], "server"))
		{
			e = find_exclude(av[2], EXCLUDE_SERVER);
			if (e)
			{
				del_exclude(e);
				Delete_Exclude(av[2], EXCLUDE_SERVER);
			}
			else
			{
				notice_lang(s_StatServ, u, STAT_EXCLUDE_NOT_FOUND, av[2]);
			}
		}
	}
	else if (!stricmp(av[0], "LIST"))
	{
		if (ac >= 2)
		{
			if (*av[1] == '#')
			{
				tmp = myStrGetOnlyToken((av[1] + 1), '-', 0);   /* Read FROM out */
				if (!tmp)
				{
					return MOD_CONT;
				}
				for (s = tmp; *s; s++)
				{
					if (!isdigit(*s))
					{
						free(tmp);
						return MOD_CONT;
					}
				}
				from = atoi(tmp);
				free(tmp);
				tmp = myStrGetTokenRemainder(av[1], '-', 1);    /* Read TO out */
				if (!tmp)
				{
					return MOD_CONT;
				}
				for (s = tmp; *s; s++)
				{
					if (!isdigit(*s))
					{
						free(tmp);
						return MOD_CONT;
					}
				}
				to = atoi(tmp);
				free(tmp);
			}
		}

		notice_lang(s_StatServ, u, STAT_EXCLUDE_LIST_HEADER);
		ExcludeDatabase = DenoraOpenSQL(excludeDB);
		rows = DenoraSQLGetNumRows(ExcludeDatabase, ExcludeTable);
		stmt = DenoraPrepareQuery(ExcludeDatabase, "SELECT * FROM %s", ExcludeTable);
		data = DenoraSQLFetchArray(ExcludeDatabase, ExcludeTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			notice(s_StatServ, u->nick, "%d %s", disp++, data[0]);
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(ExcludeDatabase);
		return MOD_CONT;
	}
	else
	{
		notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
		return MOD_CONT;
	}

	return MOD_CONT;
}
