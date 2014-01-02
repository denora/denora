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
#define MODULE_NAME "ss_export"

static void xml_export_channels(char *file);
static void xml_export_users(char *file);
static void xml_export_tld(char *file);
static void xml_export_all(char *file);
static void xml_export_stats(char *file);
static void xml_export_servers(char *file);
static void xml_export_ctcp(char *file);

static int do_export(User * u, int ac, char **av);
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
	int status;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	c = createCommand("EXPORT", do_export, is_stats_admin, -1, -1, -1,
	                  STAT_HELP_EXPORT);
	status = moduleAddCommand(STATSERV, c, MOD_UNIQUE);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "[%s] Error Occurred setting Command for EXPORT [%d][%s]",
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

/*************************************************************************/

static int do_export(User * u, int ac, char **av)
{
	char *filename = NULL;

	if (ac < 1)
	{
		syntax_error(s_StatServ, u, "EXPORT", STATS_EXPORT_SYNTAX);
		return MOD_CONT;
	}
	if (!stricmp("channels", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("channels.xml"));
		xml_export_channels(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_CHANNELS, filename);
	}
	else if (!stricmp("users", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("users.xml"));
		xml_export_users(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_USERS, filename);
	}
	else if (!stricmp("tld", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("tld.xml"));
		xml_export_tld(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_TLD, filename);
	}
	else if (!stricmp("servers", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("servers.xml"));
		xml_export_servers(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_SERVERS, filename);
	}
	else if (!stricmp("stats", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("stats.xml"));
		xml_export_stats(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_STATS, filename);
	}
	else if (!stricmp("all", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("denora.xml"));
		xml_export_all(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_ALL, filename);
	}
	else if (!stricmp("ctcp", av[0]))
	{
		filename = (ac == 2 ? sstrdup(av[1]) : sstrdup("ctcp.xml"));
		xml_export_ctcp(filename);
		notice_lang(s_StatServ, u, STATS_EXPORT_ALL, filename);
	}
	else
	{
		syntax_error(s_StatServ, u, "EXPORT", STATS_EXPORT_SYNTAX);
	}
	if (filename)
		free(filename);
	return MOD_CONT;
}

/*************************************************************************/

/**
 * Export channel struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_channels(char *file)
{
	FILE *ptr;
	Channel *c, *next;
	int i;
	char buf[BUFSIZE];
	struct c_userlist *cu;
	int32 count;
	int ping = 0;
	int counttoping = 1;
	char *temp;
	char **bans;

	*buf = '\0';

	ptr = new_xml(file);

	if (ptr)
	{
		xml_write_header(ptr);

		xml_write_block_top(ptr, "channels");
		c = firstchan();
		while (c)
		{
			next = nextchan();
			if (ping)
			{
				denora_cmd_pong(ServerName, ServerName);
				ping = 0;
			}
			xml_write_block_top(ptr, "chans");
			xml_write_tag(ptr, "name", c->name);

			xml_write_tag(ptr, "topic", c->topic);
			xml_write_tag(ptr, "topicsetter", c->topic_setter);
			xml_write_tag_int(ptr, "creation_time",
			                  (long int) c->creation_time);
			xml_write_tag_int(ptr, "topic_time", (long int) c->topic_time);
			xml_write_tag_int(ptr, "topic_count", c->stats->topic_count);
			xml_write_tag_int(ptr, "limit", c->limit);
			xml_write_tag_int(ptr, "usercount",
			                  (long int) c->stats->usercount);
			xml_write_tag_int(ptr, "maxusercount",
			                  (long int) c->stats->maxusercount);
			xml_write_tag_int(ptr, "kickcount",
			                  (long int) c->stats->kickcount);
			xml_write_tag_int(ptr, "joincounter",
			                  (long int) c->stats->joincounter);
			temp = chan_get_modes(c, 1);
			xml_write_tag(ptr, "mode", temp);
			free(temp);

			if (c->limit)
			{
				xml_write_tag_int(ptr, "limit", 1);
			}

			if (c->key)
			{
				xml_write_tag_int(ptr, "key", 1);
			}

			if (ircd->Lmode && c->redirect)
			{
				xml_write_tag(ptr, "redirect", c->redirect);
			}
			if (ircd->fmode && c->flood)
			{
				xml_write_tag(ptr, "flood", c->flood);
			}

			xml_write_block_top(ptr, "inchannel");
			for (cu = c->users; cu; cu = cu->next)
			{
				xml_write_tag(ptr, "user", cu->user->nick);
			}
			xml_write_block_bottom(ptr, "inchannel");

			if (c->bancount)
			{
				xml_write_tag_int(ptr, "bancount", c->bancount);
				xml_write_block_top(ptr, "bans");
				count = c->bancount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->bans[i]);
					xml_write_tag(ptr, "ban", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "bans");
			}

			if (ircd->except && c->exceptcount)
			{
				xml_write_tag_int(ptr, "exceptcount", c->exceptcount);
				xml_write_block_top(ptr, "exceptions");
				count = c->exceptcount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->excepts[i]);
					xml_write_tag(ptr, "except", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "exceptions");
			}

			if (ircd->invitemode && c->invitecount)
			{
				xml_write_tag_int(ptr, "invitecount", c->invitecount);
				xml_write_block_top(ptr, "invites");
				count = c->invitecount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->invite[i]);
					xml_write_tag(ptr, "invite", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "invites");
			}


			xml_write_block_bottom(ptr, "chans");
			c = next;
			if (counttoping == 10)
			{
				ping = 1;
				counttoping = 1;
			}
			else
			{
				counttoping++;
			}
		}
		xml_write_block_bottom(ptr, "channels");
		xml_write_footer(ptr);
	}
}

/*************************************************************************/

/**
 * Export tld struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_tld(char *file)
{
	FILE *ptr;
	int i, rows;
	sqlite3_stmt *stmt;
	char ***data;
	sqlite3 *db;

	ptr = new_xml(file);

	if (ptr)
	{
		xml_write_header(ptr);
		xml_write_block_top(ptr, "tld");
		db = DenoraOpenSQL(DenoraDB);
		rows = DenoraSQLGetNumRows(db, TLDTable);
		stmt = DenoraPrepareQuery(db, "SELECT * FROM %s ORDER BY overall", TLDTable);
		data = DenoraSQLFetchArray(db, TLDTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			xml_write_block_top(ptr, "domain");
			xml_write_tag(ptr, "countrycode", data[i][0]);
			xml_write_tag(ptr, "country", data[i][1]);
			xml_write_tag_int(ptr, "current", atoi(data[i][2]));
			xml_write_tag_int(ptr, "overall", atoi(data[i][3]));
			xml_write_block_bottom(ptr, "domain");
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(db);
		xml_write_block_bottom(ptr, "tld");
		xml_write_footer(ptr);
	}
}

/*************************************************************************/

/**
 * Export ctcp struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_ctcp(char *file)
{
	FILE *ptr;
	CTCPVerStats *c;
	int rows;
	sqlite3_stmt * stmt;
	sqlite3 *db;
	char ***data;
	int i;

	ptr = new_xml(file);

	if (ptr)
	{
		xml_write_header(ptr);

		xml_write_block_top(ptr, "ctcp");
		db = DenoraOpenSQL(DenoraDB);
		rows = DenoraSQLGetNumRows(db, "version");
		stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", CTCPTable);
		data = DenoraSQLFetchArray(db, CTCPTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			xml_write_block_top(ptr, "client");
			xml_write_tag(ptr, "version", data[i][0]);
			xml_write_tag_int(ptr, "current", atoi(data[i][1]));
			xml_write_tag_int(ptr, "overall", atoi(data[i][2]));
			xml_write_block_bottom(ptr, "client");
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(db);
		xml_write_block_bottom(ptr, "ctcp");
		xml_write_footer(ptr);
	}
}

/*************************************************************************/

/**
 * Export server struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_servers(char *file)
{
	FILE *ptr;
	Server *s, *next;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rows, i;
	char ***data;
	
	ptr = new_xml(file);

	if (ptr)
	{
		xml_write_header(ptr);

		xml_write_block_top(ptr, "servers");
		
		db = DenoraOpenSQL(DenoraDB);
		rows = DenoraSQLGetNumRows(db, "server");
		stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", ServerTable);
		data = DenoraSQLFetchArray(db, ServerTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			if (HideUline && atoi(data[i][0]))
			{
				continue;
			}
			denora_cmd_pong(ServerName, ServerName);
			xml_write_block_top(ptr, "server");
			xml_write_tag(ptr, "name", data[i][0]);
			xml_write_tag_int(ptr, "hops", atoi(data[i][6]));
			xml_write_tag(ptr, "desc", data[i][1]);
			xml_write_tag_int(ptr, "flags", atoi(data[i][7]));

			if (atoi(data[i][7]) & SERVER_ISME)
			{
				xml_write_tag_int(ptr, "FLAG_ISME", 1);
			}
			else
			{
				xml_write_tag_int(ptr, "FLAG_ISME", 0);
			}
			if (atoi(data[i][7]) & SERVER_JUPED)
			{
				xml_write_tag_int(ptr, "FLAG_JUPED", 1);
			}
			else
			{
				xml_write_tag_int(ptr, "FLAG_JUPED", 0);
			}
			xml_write_tag_int(ptr, "synced", data[i][2]);
			if (data[i][2])
			{
				xml_write_tag(ptr, "suid", data[i][2]);
			}
			if (data[i][3])
			{
				xml_write_tag(ptr, "version", data[i][3]);
			}
			if (data[i][4])
			{
				xml_write_tag_int(ptr, "uptime", data[i][4]);
			}
			if (data[i][5])
			{
				xml_write_tag(ptr, "uplink", data[i][5]);
			}
			xml_write_tag_int(ptr, "uline", atoi(data[i][0]));

			xml_write_block_bottom(ptr, "server");
		}
		xml_write_block_bottom(ptr, "servers");
		xml_write_footer(ptr);
	}
}

/*************************************************************************/

/**
 * Export user struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_users(char *file)
{
	FILE *ptr;
	User *u, *next;
	int i;
	char buf[BUFSIZE];
	struct u_chanlist *uc;

	*buf = '\0';

	ptr = new_xml(file);


	if (ptr)
	{
		xml_write_header(ptr);

		xml_write_block_top(ptr, "users");
		u = firstuser();
		
		while (u)
		{
			next = nextuser();
			denora_cmd_pong(ServerName, ServerName);
			
			xml_write_block_top(ptr, "user");
			xml_write_tag(ptr, "name", u->nick);
			xml_write_tag(ptr, "username", u->username);
			xml_write_tag(ptr, "host", u->host);
			if (ircd->vhost)
			{
				xml_write_tag(ptr, "vhost", u->vhost);
			}
			if (ircd->vident && u->vident)
			{
				xml_write_tag(ptr, "vident", u->vident);
			}
			xml_write_tag(ptr, "realname", u->realname);
			xml_write_tag(ptr, "server", u->server->name);
			if (u->ip)
			{
				xml_write_tag(ptr, "ip", u->ip);
			}
			xml_write_tag(ptr, "country_code", u->country_code);
			xml_write_tag(ptr, "country_name", u->country_name);
			xml_write_tag_int(ptr, "timestamp", (long int) u->timestamp);
			xml_write_tag_int(ptr, "my_signon", (long int) u->my_signon);
			xml_write_tag_int(ptr, "svid", u->svid);
			xml_write_tag_int(ptr, "mode", u->mode);
			xml_write_tag_int(ptr, "language", u->language);
			xml_write_tag_int(ptr, "isaway", (long int) u->isaway);
			if (u->isaway && u->awaymsg)
			{
				xml_write_tag(ptr, "awaymsg", u->awaymsg);
			}
			xml_write_tag_int(ptr, "admin", u->admin);
			for (i = 0; i < 128; i++)
			{
				if (u->mode & umodes[i])
				{
					ircsnprintf(buf, BUFSIZE - 1, "mode_%c", (int) i);
					xml_write_tag_int(ptr, buf, 1);
					*buf = '\0';
				}
			}
			xml_write_block_top(ptr, "inchannels");
			for (uc = u->chans; uc; uc = uc->next)
			{
				xml_write_tag(ptr, "channel", uc->chan->name);
			}
			xml_write_block_bottom(ptr, "inchannels");
			xml_moduleData(ptr, &u->moduleData);
			xml_write_block_bottom(ptr, "user");
			u = next;
		}
		xml_write_block_bottom(ptr, "users");
		xml_write_footer(ptr);
	}
}

/*************************************************************************/

/**
 * Export stats struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_stats(char *file)
{
	FILE *ptr;
	ptr = new_xml(file);

	if (ptr)
	{
		xml_write_header(ptr);
		xml_write_block_top(ptr, "stats");
		xml_write_tag_int(ptr, "users_max", stats->users_max);
		xml_write_tag_int(ptr, "users", stats->users);
		xml_write_tag_int(ptr, "chans_max", stats->chans_max);
		xml_write_tag_int(ptr, "chans", stats->chans);
		xml_write_tag_int(ptr, "servs_max", stats->servers_max);
		xml_write_tag_int(ptr, "servs", stats->servers);
		xml_write_block_bottom(ptr, "stats");
		xml_write_footer(ptr);
	}

}

/*************************************************************************/

/**
 * Export all stats to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_all(char *file)
{
	FILE *ptr;
	User *u, *next;
	Channel *c, *cnext;
	int i, count;
	char buf[BUFSIZE];
	struct u_chanlist *uc;
	struct c_userlist *cu;
	char **bans;
	int ping = 0;
	int counttoping = 1;
	char *temp;
	int rows;
	sqlite3_stmt * stmt;
	sqlite3 *db;
	char ***data;

	ptr = new_xml(file);

	

	if (ptr)
	{
		xml_write_header(ptr);

		xml_write_block_top(ptr, "stats");
		xml_write_tag_int(ptr, "users_max", stats->users_max);
		xml_write_tag_int(ptr, "users", stats->users);
		xml_write_tag_int(ptr, "chans_max", stats->chans_max);
		xml_write_tag_int(ptr, "chans", stats->chans);
		xml_write_tag_int(ptr, "servs_max", stats->servers_max);
		xml_write_tag_int(ptr, "servs", stats->servers);
		xml_write_block_bottom(ptr, "stats");

		denora_cmd_pong(ServerName, ServerName);

		xml_write_block_top(ptr, "users");
		u = firstuser();
		
		while (u)
		{
			next = nextuser();
			if (ping)
			{
				denora_cmd_pong(ServerName, ServerName);
				ping = 0;
			}
			
			xml_write_block_top(ptr, "user");
			xml_write_tag(ptr, "name", u->nick);
			xml_write_tag(ptr, "username", u->username);
			xml_write_tag(ptr, "host", u->host);
			if (ircd->vhost)
			{
				xml_write_tag(ptr, "vhost", u->vhost);
			}
			if (ircd->vident && u->vident)
			{
				xml_write_tag(ptr, "vident", u->vident);
			}
			xml_write_tag(ptr, "realname", u->realname);
			xml_write_tag(ptr, "server", u->server->name);
			if (u->ip)
			{
				xml_write_tag(ptr, "ip", u->ip);
			}
			xml_write_tag(ptr, "country_code", u->country_code);
			xml_write_tag(ptr, "country_name", u->country_name);
			xml_write_tag_int(ptr, "timestamp", (long int) u->timestamp);
			xml_write_tag_int(ptr, "my_signon", (long int) u->my_signon);
			xml_write_tag_int(ptr, "svid", u->svid);
			xml_write_tag_int(ptr, "mode", u->mode);
			xml_write_tag_int(ptr, "language", u->language);
			xml_write_tag_int(ptr, "isaway", (long int) u->isaway);
			if (u->isaway && u->awaymsg)
			{
				xml_write_tag(ptr, "awaymsg", u->awaymsg);
			}
			xml_write_tag_int(ptr, "admin", u->admin);
			for (i = 0; i < 128; i++)
			{
				if (u->mode & umodes[i])
				{
					ircsnprintf(buf, BUFSIZE - 1, "mode_%c", (int) i);
					xml_write_tag_int(ptr, buf, 1);
					*buf = '\0';
				}
			}
			xml_write_block_top(ptr, "inchannels");
			for (uc = u->chans; uc; uc = uc->next)
			{
				xml_write_tag(ptr, "channel", uc->chan->name);
			}
			xml_write_block_bottom(ptr, "inchannels");
			xml_moduleData(ptr, &u->moduleData);
			xml_write_block_bottom(ptr, "user");
			u = next;
			if (counttoping == 10)
			{
				ping = 1;
				counttoping = 1;
			}
			else
			{
				counttoping++;
			}
		}
		xml_write_block_bottom(ptr, "users");

		xml_write_block_top(ptr, "servers");
		db = DenoraOpenSQL(DenoraDB);
		rows = DenoraSQLGetNumRows(db, "server");
		stmt = DenoraPrepareQuery(db, "SELECT * FROM %s", ServerTable);
		data = DenoraSQLFetchArray(db, ServerTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			if (HideUline && atoi(data[i][0]))
			{
				continue;
			}
			denora_cmd_pong(ServerName, ServerName);
			xml_write_block_top(ptr, "server");
			xml_write_tag(ptr, "name", data[i][0]);
			xml_write_tag_int(ptr, "hops", atoi(data[i][6]));
			xml_write_tag(ptr, "desc", data[i][1]);
			xml_write_tag_int(ptr, "flags", atoi(data[i][7]));

			if (atoi(data[i][7]) & SERVER_ISME)
			{
				xml_write_tag_int(ptr, "FLAG_ISME", 1);
			}
			else
			{
				xml_write_tag_int(ptr, "FLAG_ISME", 0);
			}
			if (atoi(data[i][7]) & SERVER_JUPED)
			{
				xml_write_tag_int(ptr, "FLAG_JUPED", 1);
			}
			else
			{
				xml_write_tag_int(ptr, "FLAG_JUPED", 0);
			}
			xml_write_tag_int(ptr, "synced", data[i][2]);
			if (data[i][2])
			{
				xml_write_tag(ptr, "suid", data[i][2]);
			}
			if (data[i][3])
			{
				xml_write_tag(ptr, "version", data[i][3]);
			}
			if (data[i][4])
			{
				xml_write_tag_int(ptr, "uptime", data[i][4]);
			}
			if (data[i][5])
			{
				xml_write_tag(ptr, "uplink", data[i][5]);
			}
			xml_write_tag_int(ptr, "uline", atoi(data[i][0]));
			xml_write_block_top(ptr, "serverstats");
			xml_write_tag_int(ptr, "currentusers", data[i][12]));
			xml_write_tag_int(ptr, "maxusers", data[i][13]);
			xml_write_block_bottom(ptr, "serverstats");


			xml_write_block_bottom(ptr, "server");
		}
		xml_write_block_bottom(ptr, "servers");

		xml_write_block_top(ptr, "tld");
		db = DenoraOpenSQL(DenoraDB);
		rows = DenoraSQLGetNumRows(db, TLDTable);
		stmt = DenoraPrepareQuery(db, "SELECT * FROM %s ORDER BY overall LIMIT 10", TLDTable);
		data = DenoraSQLFetchArray(db, TLDTable, stmt, FETCH_ARRAY_NUM);
		for (i = 0; i < rows; i++)
		{
			xml_write_block_top(ptr, "domain");
			xml_write_tag(ptr, "countrycode", data[i][0]);
			xml_write_tag(ptr, "country", data[i][1]);
			xml_write_tag_int(ptr, "current", atoi(data[i][2]));
			xml_write_tag_int(ptr, "overall", atoi(data[i][3]));
			xml_write_block_bottom(ptr, "domain");
		}
		free(data);
		sqlite3_finalize(stmt);
		DenoraCloseSQl(db);
		xml_write_block_bottom(ptr, "tld");


		xml_write_block_top(ptr, "channels");
		c = firstchan();
		while (c)
		{
			cnext = nextchan();
			if (ping)
			{
				denora_cmd_pong(ServerName, ServerName);
				ping = 0;
			}
			xml_write_block_top(ptr, "chans");
			xml_write_tag(ptr, "name", c->name);

			xml_write_tag(ptr, "topic", c->topic);
			xml_write_tag(ptr, "topicsetter", c->topic_setter);
			xml_write_tag_int(ptr, "creation_time",
			                  (long int) c->creation_time);
			xml_write_tag_int(ptr, "topic_time", (long int) c->topic_time);
			xml_write_tag_int(ptr, "topic_count", c->stats->topic_count);
			xml_write_tag_int(ptr, "limit", c->limit);
			xml_write_tag_int(ptr, "usercount",
			                  (long int) c->stats->usercount);
			xml_write_tag_int(ptr, "maxusercount",
			                  (long int) c->stats->maxusercount);
			xml_write_tag_int(ptr, "kickcount",
			                  (long int) c->stats->kickcount);
			xml_write_tag_int(ptr, "joincounter",
			                  (long int) c->stats->joincounter);
			temp = chan_get_modes(c, 1);
			xml_write_tag(ptr, "mode", temp);
			free(temp);

			if (c->limit)
			{
				xml_write_tag_int(ptr, "limit", 1);
			}

			if (c->key)
			{
				xml_write_tag_int(ptr, "key", 1);
			}

			if (ircd->Lmode && c->redirect)
			{
				xml_write_tag(ptr, "redirect", c->redirect);
			}
			if (ircd->fmode && c->flood)
			{
				xml_write_tag(ptr, "flood", c->flood);
			}

			xml_write_block_top(ptr, "inchannel");
			for (cu = c->users; cu; cu = cu->next)
			{
				xml_write_tag(ptr, "user", cu->user->nick);
			}
			xml_write_block_bottom(ptr, "inchannel");

			if (c->bancount)
			{
				xml_write_tag_int(ptr, "bancount", c->bancount);
				xml_write_block_top(ptr, "bans");
				count = c->bancount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->bans[i]);
					xml_write_tag(ptr, "ban", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "bans");
			}

			if (ircd->except && c->exceptcount)
			{
				xml_write_tag_int(ptr, "exceptcount", c->exceptcount);
				xml_write_block_top(ptr, "exceptions");
				count = c->exceptcount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->excepts[i]);
					xml_write_tag(ptr, "except", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "exceptions");
			}

			if (ircd->invitemode && c->invitecount)
			{
				xml_write_tag_int(ptr, "invitecount", c->invitecount);
				xml_write_block_top(ptr, "invites");
				count = c->invitecount;
				bans = calloc(sizeof(char *) * count, 1);
				for (i = 0; i < count; i++)
				{
					bans[i] = sstrdup(c->invite[i]);
					xml_write_tag(ptr, "invite", bans[i]);
				}
				free(bans);
				xml_write_block_bottom(ptr, "invites");
			}
			xml_write_block_bottom(ptr, "chans");
			c = cnext;
			if (counttoping == 10)
			{
				ping = 1;
				counttoping = 1;
			}
			else
			{
				counttoping++;
			}
		}
		xml_write_block_bottom(ptr, "channels");

		xml_write_footer(ptr);
	}
}

/*************************************************************************/
