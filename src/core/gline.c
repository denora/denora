/* GLINE/TKL Bans 
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
/*************************************************************************/

#include "denora.h"

#define MODULE_VERSION "2.0"
#define MODULE_NAME "gline"

void sql_do_server_bans_add(char *type, char *user, char *host, char *setby, char *setat, char *expires, char *reason);
void sql_do_server_bans_remove(char *type, char *user, char *host);
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
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (DisableNetBans)
	{
		alog(LOG_NORMAL,   "[%s] DisableNetBans enabled unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	Gline_Parse_Add_Handler(sql_do_server_bans_add);
	Gline_Parse_Del_Handler(sql_do_server_bans_remove);
	
	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

/**
 * Unreal passes in TKL the expires as the exact time of the expiration
 * for this reason and the fact they use "type" we do not add the expire + time
 * only on ircds where this is not done
 *
 * @param type If Unreal the type is Gline, Shun, etc..
 * @param user is the user ident that is banned
 * @param host is the user host that is banned
 * @param setby is the person or server that has set the ban
 * @param setat is the unix timestamp of when the ban was set
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param reason is the reason for the ban
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_add(char *type, char *user, char *host,
                            char *setby, char *setat, char *expires,
                            char *reason)
{
	int *res;
	int checkdur = 0;
	int expire = 0;
	uint32 setattime = 0;
	char **data;

	if (DisableNetBans)
	{
		return;
	}

	if (type)
	{
		data = DenoraSQLReturnRow(DenoraDB, "SELECT * FROM %s WHERE type = \'%s\' and user=\'%q\' and host=\'%q\'", GlineTable, type, user, host);
		if (data)
		{
			if (atoi(data[4]) == (int32) strtoul(setat, NULL, 10) && atoi(data[5]) == (int32) strtoul(expires, NULL, 10))
			{
					return;
			}
			else
			{
				DenoraSQLQuery(DenoraDB, "UPDATE %s SET setat=%ld, expires=%ld, reason=\'%q\' WHERE id = \'%d\'",
				         GlineTable, strtoul(setat, NULL, 10), strtoul(expires, NULL, 10), reason, data[0]);
			}
		}
		else
		{
					DenoraSQLQuery(DenoraDB, "INSERT INTO %s (type, user, host, setby, setat, expires, reason) VALUES(\'%q\',\'%q\',\'%q\',\'%s\',%ld,%ld,\'%q\')",
					 GlineTable, type, user, host, setby,
					 strtoul(setat, NULL, 10), strtoul(expires, NULL, 10), reason);
		}
	}
	else
	{
		

		checkdur = strtoul(expires, NULL, 10);
		setattime = strtoul(setat, NULL, 10);
		if (checkdur != 0)
		{
			expire = checkdur + setattime;
		}
		else
		{
			expire = checkdur;
		}

		res = DenoraSQLGetNumRowsFromQuery(char *dbname, "SELECT id FROM %s WHERE user=\'%s\' and host=\'%s\' LIMIT 1", GlineTable, user, host);

		if (res)
		{
			DenoraSQLQuery(DenoraDB, "INSERT INTO %s (user, host, setby, setat, expires, reason) VALUES(\'%q\',\'%q\',\'%s\',%ld,%ld,\'%q\')",
				 GlineTable, user, host, setby, setattime, expire, reason);
		}
		else
		{
			DenoraSQLQuery(DenoraDB, "UPDATE %s SET setat=%ld, expires=%ld, reason=\'%q\' WHERE user=\'%q\' and host=\'%q\'",
			 GlineTable, setattime, expire, reason, user, host);
		}
	}

	return;
}


/*************************************************************************/

/**
 * Parse GLINE messages and remove from SQL
 *
 * @param type If Unreal the type is Gline, Shun, etc.. this is optional
 * @param user is the user ident that is being unbanned
 * @param host is the user host that is being unbanned
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_remove(char *type, char *user, char *host)
{

	if (DisableNetBans)
	{
		return;
	}

	if (type)
	{
		sql_query(
		          "DELETE FROM %s WHERE type=\'%s\' and user=\'%q\' and host=\'%q\'",
		          GlineTable, type, user, host);
	}
	else
	{
		sql_query("DELETE FROM %s WHERE user=\'%q\' and host=\'%q\'",
		          GlineTable, user, host);
	}
	
	return;
}
