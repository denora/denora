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

int DenoraInit(int argc, char **argv);
void DenoraFini(void);

#ifdef USE_MODULES
int do_modload(User * u, int ac, char **av);
int do_modunload(User * u, int ac, char **av);
int do_modlist(User * u, int ac, char **av);
int do_modinfo(User * u, int ac, char **av);

static int showModuleCmdLoaded(CommandHash * cmdList, char *mod_name,
                               User * u);
static int showModuleMsgLoaded(MessageHash * msgList, char *mod_name,
                               User * u);
#endif

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
	moduleAddVersion
	("");
	moduleSetType(CORE);

#ifdef USE_MODULES
	c = createCommand("MODLOAD", do_modload, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_MODLOAD);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);

	c = createCommand("MODUNLOAD", do_modunload, is_stats_admin, -1, -1,
	                  -1, STAT_HELP_MODUNLOAD);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);

	c = createCommand("MODLIST", do_modlist, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_MODLIST);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);

	c = createCommand("MODINFO", do_modinfo, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_MODINFO);
	moduleAddCommand(STATSERV, c, MOD_UNIQUE);
#endif

	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

#ifdef USE_MODULES

int do_modload(User * u, int ac, char **av)
{
	if (ac < 1)
	{
		syntax_error(s_StatServ, u, "MODLOAD", STAT_MODULE_LOAD_SYNTAX);
		return MOD_CONT;
	}
	if (!queueModuleLoad(av[0], u))
	{
		notice_lang(s_StatServ, u, STAT_MODULE_LOAD_FAIL, av[0]);
	}
	return MOD_CONT;
}

/*************************************************************************/

int do_modunload(User * u, int ac, char **av)
{
	if (ac < 1)
	{
		syntax_error(s_StatServ, u, "MODUNLOAD",
		             STAT_MODULE_UNLOAD_SYNTAX);
		return MOD_CONT;
	}
	if (!queueModuleUnload(av[0], u))
	{
		notice_lang(s_StatServ, u, STAT_MODULE_REMOVE_FAIL, av[0]);
	}
	return MOD_CONT;
}

/*************************************************************************/

int do_modlist(User * u, int ac, char **av)
{
	int idx;
	int count = 0;
	int showCore = 0;
	int showThird = 1;
	int showProto = 1;
	int showSupported = 1;
	int showQA = 1;

	ModuleHash *current = NULL;

	char core[] = "Core";
	char third[] = "3rd";
	char proto[] = "Protocol";
	char supported[] = "Supported";
	char qa[] = "QATested";

	if (ac >= 1)
	{
		if (stricmp(av[0], core) == 0)
		{
			showCore = 1;
			showThird = 0;
			showProto = 0;
			showSupported = 0;
			showQA = 0;
		}
		else if (stricmp(av[0], third) == 0)
		{
			showCore = 0;
			showThird = 1;
			showSupported = 0;
			showQA = 0;
			showProto = 0;
		}
		else if (stricmp(av[0], proto) == 0)
		{
			showCore = 0;
			showThird = 0;
			showProto = 1;
			showSupported = 0;
			showQA = 0;
		}
		else if (stricmp(av[0], supported) == 0)
		{
			showCore = 0;
			showThird = 0;
			showProto = 0;
			showSupported = 1;
			showQA = 0;
		}
		else if (stricmp(av[0], qa) == 0)
		{
			showCore = 0;
			showThird = 0;
			showProto = 0;
			showSupported = 0;
			showQA = 1;
		}
	}

	notice_lang(s_StatServ, u, STAT_MODULE_LIST_HEADER);

	for (idx = 0; idx != MAX_CMD_HASH; idx++)
	{
		for (current = MODULE_HASH[idx]; current; current = current->next)
		{
			switch (current->m->type)
			{
				case CORE:
					if (showCore)
					{
						notice_lang(s_StatServ, u, STAT_MODULE_LIST,
						            current->name, current->m->version, core);
						count++;
					}
					break;
				case THIRD:
					if (showThird)
					{
						notice_lang(s_StatServ, u, STAT_MODULE_LIST,
						            current->name, current->m->version, third);
						count++;
					}
					break;
				case PROTOCOL:
					if (showProto)
					{
						notice_lang(s_StatServ, u, STAT_MODULE_LIST,
						            current->name, current->m->version, proto);
						count++;
					}
					break;
				case SUPPORTED:
					if (showSupported)
					{
						notice_lang(s_StatServ, u, STAT_MODULE_LIST,
						            current->name, current->m->version,
						            supported);
						count++;
					}
					break;
				case QATESTED:
					if (showQA)
					{
						notice_lang(s_StatServ, u, STAT_MODULE_LIST,
						            current->name, current->m->version, qa);
						count++;
					}
					break;
				default:
					/* assume default is unknown and do debug - this should never
					   happen but to make -Wswitch-default happy we have to do something
					 */
					alog(LOG_DEBUG, "unknown current->m->type for %s",
					     current->name);
					break;
			}

		}
	}
	if (count == 0)
	{
		notice_lang(s_StatServ, u, STAT_MODULE_NO_LIST);
	}
	else
	{
		notice_lang(s_StatServ, u, STAT_MODULE_LIST_FOOTER, count);
	}

	return MOD_CONT;
}

/*************************************************************************/

int do_modinfo(User * u, int ac, char **av)
{
	struct tm tm;
	char timebuf[64];
	Module *m;
	int idx = 0;
	int display = 0;

	if (ac < 1)
	{
		syntax_error(s_StatServ, u, "MODINFO", STAT_MODULE_INFO_SYNTAX);
		return MOD_CONT;
	}
	m = findModule(av[0]);
	if (m)
	{
#ifdef _WIN32
		localtime_s(&tm, &m->time);
#else
		tm = *localtime(&m->time);
#endif
		strftime_lang(timebuf, sizeof(timebuf), u,
		              STRFTIME_DATE_TIME_FORMAT, &tm);
		notice_lang(s_StatServ, u, STAT_MODULE_INFO_LIST, m->name,
		            m->version ? m->version : "?",
		            m->author ? m->author : "?", timebuf);
		for (idx = 0; idx < MAX_CMD_HASH; idx++)
		{
			display += showModuleCmdLoaded(STATSERV[idx], m->name, u);
			display += showModuleMsgLoaded(IRCD[idx], m->name, u);

		}
		if (m->type == PROTOCOL)
		{
			display++;
		}
	}
	if (display == 0)
	{
		notice_lang(s_StatServ, u, STAT_MODULE_NO_INFO, av[0]);
	}
	return MOD_CONT;
}

/*************************************************************************/

static int showModuleCmdLoaded(CommandHash * cmdList, char *mod_name,
                               User * u)
{
	Command *c;
	CommandHash *current;
	int display = 0;

	for (current = cmdList; current; current = current->next)
	{
		for (c = current->c; c; c = c->next)
		{
			if ((c->mod_name) && (stricmp(c->mod_name, mod_name) == 0))
			{
				notice_lang(s_StatServ, u, STAT_MODULE_CMD_LIST,
				            c->service, c->name);
				display++;
			}
		}
	}
	return display;
}

/*************************************************************************/

static int showModuleMsgLoaded(MessageHash * msgList, char *mod_name,
                               User * u)
{
	Message *msg;
	MessageHash *mcurrent;
	int display = 0;
	for (mcurrent = msgList; mcurrent; mcurrent = mcurrent->next)
	{
		for (msg = mcurrent->m; msg; msg = msg->next)
		{
			if ((msg->mod_name) && (stricmp(msg->mod_name, mod_name) == 0))
			{
				notice_lang(s_StatServ, u, STAT_MODULE_MSG_LIST,
				            msg->name);
				display++;
			}
		}
	}
	return display;
}

/*************************************************************************/

#endif
