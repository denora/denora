/* StatServ Core module
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
	int i = 1;
	ChannelStats *cs;
	char **sql_row;

	if (argc < 3)
		return MOD_CONT;

	if (!denora->do_sql)
	{
		return MOD_CONT;
	}

	if (stricmp(argv[0], "top") == 0)
	{
		u = finduser(argv[1]);
		chan = sql_escape(argv[2]);
		cs = find_cs(argv[2]);
		sql_query(
			  "SELECT * FROM %s WHERE chan=\'%s\' AND type=0 ORDER BY letters DESC LIMIT 3;",
			  UStatsTable, chan);
		free(chan);
		sql_res = sql_set_result(sqlcon);
		if (sql_num_rows(sql_res) > 0)
		{
			if (cs->flags & CS_NOTICE)
			{
				notice_lang(s_StatServ, u, STATS_TOP_CHANNEL, argv[2]);
			}
			else
			{
				denora_cmd_privmsg(s_StatServ, argv[2],
				                   getstring(NULL,
				                             STATS_TOP_CHANNEL), argv[2]);
			}
			
			while ((sql_row = sql_fetch_row(sql_res)) != NULL)
			{
				if (cs->flags & CS_NOTICE)
				{
					notice_lang(s_StatServ, u, STATS_MESSAGE,
					            i, sql_row[0], sql_row[3],
					            sql_row[4], sql_row[5],
					            sql_row[7], sql_row[6]);
				}
				else
				{
					denora_cmd_privmsg(s_StatServ, argv[2],
					                   getstring(NULL,
					                             STATS_MESSAGE), i,
					                   sql_row[0], sql_row[3],
					                   sql_row[4], sql_row[5],
					                   sql_row[7], sql_row[6]);
				}
				i++;
			}
		}
		
		sql_free_result(sql_res);
	}

	return MOD_CONT;
}
