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
	char *chan;
	SQLres *sql_res;
	int peak = 0;
	ChannelStats *cs;
	char **sql_row;

	if (argc < 3)
		return MOD_CONT;

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	if (stricmp(argv[0], "peak") == 0)
	{
		u = finduser(argv[1]);
		chan = sql_escape(argv[2]);
		cs = find_cs(argv[2]);
		strtolwr(chan);
		sql_query(
		          "SELECT maxusers FROM %s WHERE channel=\'%s\';",
		          ChanTable, chan);
		alog(LOG_DEBUG, "!peak: Searching for %s", chan);
		free(chan);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res))
			{
				sql_row = sql_fetch_row(sql_res);
				peak = atoi(sql_row[0]);
				alog(LOG_DEBUG, "!peak: peak number is %d", peak);
				if (cs->flags & CS_NOTICE)
				{
					notice_lang(s_StatServ, u, STATS_CHANNEL_PEAK, peak);
				}
				else
				{
					denora_cmd_privmsg(s_StatServ, argv[2],
					                   getstring(NULL,
					                             STATS_CHANNEL_PEAK),
					                   peak);
				}
			}
			
			alog(LOG_DEBUG, "!peak: number of rows found to be 0");
			sql_free_result(sql_res);
		}
		alog(LOG_DEBUG, "!peak: the resource was NULL");
		return MOD_CONT;
	}

	return MOD_CONT;
}
