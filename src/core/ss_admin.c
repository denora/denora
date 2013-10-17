/* StatServ core functions
 *
 * (c) 2004-2012 Denora Team
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
	moduleAddAuthor("Denora");
	moduleAddVersion("");
	moduleSetType(CORE);

	c = createCommand("ADMIN", do_admin, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_ADMIN);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);

	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

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
		}
		else
		{
			a = make_admin(av[1]);
			if (ac <= 3)
			{
				a->hosts[0] = sstrdup("*@*");
				a->language = StatsLanguage;
			}
			else if (ac <= 4)
			{
				a->hosts[0] = sstrdup(av[3]);
				a->language = StatsLanguage;
			}
			else
			{
				a->hosts[0] = sstrdup(av[3]);
				a->language = atoi(av[4]);
				if (a->language < 1 || a->language > NUM_LANGS)
				{
					a->language = StatsLanguage;
				}
			}
			a->passwd = sstrdup(MakePassword(av[2]));
			add_sqladmin(a->name, a->passwd, 0, a->hosts[0], a->language);
			u2 = user_find(av[1]);
			if (u2)
			{
				u2->admin = 1;
			}
			notice_lang(s_StatServ, u, STAT_ADMIN_CREATED, av[1]);
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
				return MOD_CONT;
			}
			free_admin(a);
			u2 = user_find(av[1]);
			if (u2)
			{
				u2->admin = 0;
			}
			del_sqladmin(av[1]);
			notice_lang(s_StatServ, u, STAT_ADMIN_DELETED, av[1]);
		}
		else
		{
			notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
		}
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
				return MOD_CONT;
			}
			free(a->passwd);
			a->passwd = sstrdup(MakePassword(av[2]));
			if (denora->do_sql) 
			{
				crypted = is_crypted(a->passwd);
				rdb_query(QUERY_LOW, "UPDATE %s SET passwd=%s%s%s WHERE uname = '%s'", AdminTable, crypted ? "'" : "MD5('", passwd, crypted ? "'" : "')", a->name);
			}
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
		for (i = 0; i < 1024; i++)
		{
			for (a = adminlists[i]; a; a = a->next)
			{
				notice(s_StatServ, u->nick, "%d %s", disp++, a->name);
			}
		}
		return MOD_CONT;
	}
	else
	{
		syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
	}
	return MOD_CONT;
}
