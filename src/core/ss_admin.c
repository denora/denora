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

static int do_admin(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);


/*************************************************************************/
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

	c = createCommand("ADMIN", do_admin, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_ADMIN);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting Command for ADMIN [%d][%s]",
		     status, ModuleGetErrStr(status));
		return MOD_STOP;
	}

	return MOD_CONT;
}

/*************************************************************************/

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

/*
  0 1 add | del
  1 2 user
  2 3 pass
  3 4 host
  4 5 language
*/
static int do_admin(User * u, int ac, char **av)
{
	Dadmin *a;
	User *u2;
	int i;
	int disp = 1;
	int crypted = 0;
	int rows;
	sqlite3_stmt * stmt;
	char ***data;
	int language;

	if (denora->protocoldebug)
	{
		protocol_debug(NULL, ac, av);
	}

	if (ac < 1)
	{
		syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
		return MOD_CONT;
	}
	if (!stricmp(av[0], "ADD"))
	{
		if (!u->confadmin)
		{
			notice_lang(s_StatServ, u, PERMISSION_DENIED);
			return MOD_CONT;
		}
		if (ac < 3)
		{
			syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
			return MOD_CONT;
		}
		alog(LOG_NORMAL, "%s: %s: ADMIN ADD %s ****", s_StatServ, u->nick,
		     av[1]);

		a = find_admin_byname(av[1]);
		if (a)
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_ALREADY, av[1]);
			return MOD_CONT;
		}
		else
		{
			if (ac <= 4)
			{
				language = atoi(av[4]);
				if (language < 1 || language > NUM_LANGS)
				{
					language = StatsLanguage;
				}
			} 
			else 
			{
					language = StatsLanguage;
			}
			add_sqladmin(av[1], av[2], 0, (ac <= 3 ? "*@*" : av[3]), language, 0, 1);

			u2 = user_find(av[1]);
			if (u2)
			{
				u2->admin = 1;
			}
			notice_lang(s_StatServ, u, STAT_ADMIN_CREATED, av[1]);
			return MOD_CONT;
		}
	}
	else if (!stricmp(av[0], "DEL"))
	{
		if (!u->confadmin)
		{
			notice_lang(s_StatServ, u, PERMISSION_DENIED);
			return MOD_CONT;
		}
		if (ac < 2)
		{
			syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
			return MOD_CONT;
		}

		alog(LOG_NORMAL, "%s: %s: ADMIN DEL %s", s_StatServ, u->nick,
		     av[1]);

		a = find_admin_byname(av[1]);
		if (a)
		{
			if (a->configfile)
			{
				notice_lang(s_StatServ, u, STAT_ADMIN_DELETE_CONFIG,
				            av[1]);
				free_admin(a);
				return MOD_CONT;
			}
			free_admin(a);
			u2 = user_find(av[1]);
			if (u2)
			{
				u2->admin = 0;
			}
			if (denora->do_sql) 
			{
				del_sqladmin(av[1]);
			}
			notice_lang(s_StatServ, u, STAT_ADMIN_DELETED, av[1]);
		}
		else
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
		}
		return MOD_CONT;
	}
	else if (!stricmp(av[0], "SETPASS"))
	{
		if (!u->confadmin)
		{
			notice_lang(s_StatServ, u, PERMISSION_DENIED);
			return MOD_CONT;
		}
		if (ac < 3)
		{
			syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
			return MOD_CONT;
		}
		alog(LOG_NORMAL, "%s: %s: ADMIN SETPASS %s ****", s_StatServ, u->nick, av[1]);

		a = find_admin_byname(av[1]);
		if (a)
		{
			if (a->configfile)
			{
				notice_lang(s_StatServ, u, STAT_CHGPASS_CONFIG, av[1]);
				free_admin(a);
				return MOD_CONT;
			}
			AdminSetPassword(a, av[2]);
			free_admin(a);
			notice_lang(s_StatServ, u, STAT_CHGPASS_OK, av[1]);
		}
		else
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
		}
		return MOD_CONT;
	}
	else if (!stricmp(av[0], "SHOW"))
	{
		if (ac < 2)
		{
			syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
			return MOD_CONT;
		}
		alog(LOG_NORMAL, "%s: %s: ADMIN SHOW %s", s_StatServ, u->nick, av[1]);

		a = find_admin_byname(av[1]);
		if (a)
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_SHOW, a->name,
			            a->hosts[0], a->language);
			free_admin(a);
		}
		else
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
		}
		return MOD_CONT;
	}
	else if (!stricmp(av[0], "LIST"))
	{
		alog(LOG_NORMAL, "%s: %s: ADMIN LIST", s_StatServ, u->nick);
		AdminDatabase = DenoraOpenSQL(AdminDB);
		rows = DenoraSQLGetNumRows(AdminDatabase, "admin");
		stmt = DenoraPrepareQuery(AdminDatabase, "SELECT * FROM admin");
		data = DenoraSQLFetchArray(AdminDatabase, "admin", stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			notice(s_StatServ, u->nick, "%d %s", disp++, data[0]);
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(AdminDatabase);
		return MOD_CONT;
	}
	else
	{
		syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
	}
	return MOD_CONT;
}
