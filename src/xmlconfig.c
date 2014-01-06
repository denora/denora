/*
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
#include "denora.h"

/*
[09:53] <~Trystan> so before I start compiling this newest commit, to enable/disable feature in xml configs would people perfer, "yes/no" or "enable/disable" (this is what we do not in ircd struct)
[10:15] <@DukePyrolator> Use yes/no, but make it accept enable/disable and true/false and 0/1 and on/off

	if none of the above return -1
*/
int XmlConfigSetFeature(char *x) 
{
	if (!strcmp(x, "yes") || !strcmp(x, "enable") || !strcmp(x, "true") || !strcmp(x, "1"))
	{
		return 1;
	}

	if (!strcmp(x, "no") || !strcmp(x, "disable") || !strcmp(x, "false") || !strcmp(x, "0"))
	{
		return 0;
	}
	return -1;
}


#define XmlConfigSetMode(x) (XmlConfigSetFeature(x) == -1 ? 1 : 0)



config *DenoraXMLConfigFindBlock(char *mask)
{
	config *c;

	if (!mask)
	{
		return NULL;
	}
	for (c = configlists[CONFIGHASH(mask)]; c; c = c->next)
	{
		if (strcmp(c->name, mask) == 0)
		{
			return c;
		}
	}
	return NULL;
}

int DenoraParseConnectBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
			
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "hostname") || !strcmp(tag,"RemoteServer"))
			{
				RemoteServer = sstrdup(data);
			}
			else if(!strcmp(tag, "port") || !strcmp(tag, "RemotePort"))
			{
				RemotePort = atoi(data);
				if (RemotePort < 1 || RemotePort > 65535)
				{
					DenoraXMLDebug(langstring(CONFIG_PORT_ERROR));
					return -1;
				}
			}
			else if(!strcmp(tag, "passwd") || !strcmp(tag, "RemotePassword"))
			{
				RemotePassword = sstrdup(data);
			}
			else if(!strcmp(tag, "protocol") || !strcmp(tag, "IRCDModule"))
			{
				IRCDModule = sstrdup(data);
			}
			else if(!strcmp(tag, "QuitPrefix"))
			{
				QuitPrefix = sstrdup(data);;
			}
			else if (!strcmp(tag, "bindhost") || !strcmp(tag,"LocalHost"))
			{
				LocalHost = sstrdup(data);
			}
			else if(!strcmp(tag, "bindport") || !strcmp(tag, "LocalPort"))
			{
				LocalPort = atoi(data);
				if (LocalPort < 1 || LocalPort > 65535)
				{
					DenoraXMLDebug(langstring(CONFIG_PORT_ERROR));
					return -1;
				}
			}
			else if (!strcmp(tag, "bindhost") || !strcmp(tag,"LocalHost"))
			{
				LocalHost = sstrdup(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!RemoteServer)
	{
		DenoraXMLDebug(langstring(CONFIG_HOSTNAME_ERROR));
		return -1;
	}
	if (BadPtr(IRCDModule))
	{
		DenoraXMLDebug(langstring(CONFIG_PROTOCOL_ERROR));
		return -1;
	}
	if (!QuitPrefix)
		QuitPrefix = sstrdup("Quit:");
	if (!RemotePort)
	{
		DenoraXMLDebug(langstring(CONFIG_PORT_ERROR_NOTDEF));
		return -1;
	}
	if (BadPtr(RemotePassword))
	{
		DenoraXMLDebug(langstring(CONFIG_PASSWD_ERROR));
		return -1;
	}
	return 1;
}

/*************************************************************************/


int DenoraParseIdentityBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "ServerName"))
			{
				/* validate server name */
				if (!is_valid_server(data))
				{
					printf("1\n");
					DenoraXMLDebug("Invalid server name");
					return -1;
				}
				ServerName = sstrdup(data);
			}
			else if(!strcmp(tag, "ServerDesc"))
			{
				ServerDesc = sstrdup(data);
			}
			else if(!strcmp(tag, "ServiceUser"))
			{
				ServiceUser = sstrdup(data);
			}
			else if(!strcmp(tag, "ServiceHost"))
			{
				ServiceHost = sstrdup(data);
			}

			else if(!strcmp(tag, "StatsLanguage"))
			{
				StatsLanguage = strtol(data, NULL, 10);
				if (StatsLanguage < 1 || StatsLanguage > NUM_LANGS)
				{
					StatsLanguage = 1;
					DenoraXMLDebug(langstring(CONFIG_INVALID_LANG));
					printf("3\n");
					return -1;
				}
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	if (BadPtr(ServerName))
	{
		DenoraXMLDebug(langstring(CONFIG_ID_NAME_ERROR));
					printf("2\n");
		return -1;
	}
	if (BadPtr(ServerDesc) || !ServerDesc)
	{
		DenoraXMLDebug(langstring(CONFIG_ID_DESC_ERROR));
					printf("4\n");
		return -1;
	}
	if (BadPtr(ServiceUser))
	{
		DenoraXMLDebug(langstring(CONFIG_ID_USER_ERROR));
					printf("5\n");
		return -1;
	}
	if (BadPtr(ServiceHost))
	{
		DenoraXMLDebug(langstring(CONFIG_ID_HOST_ERROR));
					printf("6\n");
		return -1;
	}
	if (!StatsLanguage)
	{
		DenoraXMLDebug(langstring(CONFIG_ID_LANG_ERROR));
					printf("7\n");
		return -1;
	}
	if (StatsLanguage)
	{
		/* Now we reduce StatsLanguage by 1 */
		StatsLanguage--;
	}


	return 1;
}

/*************************************************************************/


int DenoraParseStatServBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "StatServNick"))
			{
				s_StatServ = sstrdup(data);
				if (strlen(s_StatServ) > NICKMAX)
				{
					DenoraXMLDebug(langstring(CONFIG_SS_TOLONG));
					return -1;
				}
			}
			else if(!strcmp(tag, "StatServDesc"))
			{
				desc_StatServ = sstrdup(data);
			}
			else if (!strcmp(tag, "StatServNickAlias"))
			{
				s_StatServ_alias = sstrdup(data);
				if (strlen(s_StatServ_alias) > NICKMAX)
				{
					DenoraXMLDebug(langstring(CONFIG_SS_TOLONG));
					return -1;
				}
			}
			else if(!strcmp(tag, "StatServDescAlias"))
			{
				desc_StatServ_alias = sstrdup(data);
			}
			else if(!strcmp(tag, "AutoOp"))
			{
				AutoOp = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "AutoMode"))
			{
				AutoMode = sstrdup(data);
			}
			else if(!strcmp(tag, "PartOnEmpty"))
			{
				PartOnEmpty = XmlConfigSetFeature(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	if (BadPtr(s_StatServ))
	{
		DenoraXMLDebug(langstring(CONFIG_SS_NAME_ERROR));
		return -1;
	}
	if (BadPtr(desc_StatServ))
	{
		DenoraXMLDebug(langstring(CONFIG_SS_DESC_ERROR));
		return -1;
	}
	if (s_StatServ_alias && BadPtr(desc_StatServ_alias))
	{
		DenoraXMLDebug(langstring(CONFIG_SS_ALIASDESC_ERROR));
		return -1;
	}
	if (s_StatServ_alias)
	{
		if (BadPtr(s_StatServ_alias))
		{
			DenoraXMLDebug(langstring(CONFIG_SS_ALIASDESC_ERROR));
			return -1;
		}
	}

	return 1;
}

/*************************************************************************/


int DenoraParseFileNamesBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "PIDFilename"))
			{
				PIDFilename = sstrdup(data);
			}
			else if(!strcmp(tag, "MOTDFilename"))
			{
				MOTDFilename = sstrdup(data);
			}
			else if(!strcmp(tag, "HTMLFilename"))
			{
				HTMLFilename = sstrdup(data);
			}
			else if(!strcmp(tag, "ChannelDB"))
			{
				ChannelDB = sstrdup(data);
			}
			else if(!strcmp(tag, "statsDB"))
			{
				statsDB = sstrdup(data);
			}
			else if(!strcmp(tag, "ctcpDB"))
			{
				ctcpDB = sstrdup(data);
			}
			else if(!strcmp(tag, "AdminDB"))
			{
				AdminDB = sstrdup(data);
			}
			else if(!strcmp(tag, "ServerDB"))
			{
				ServerDB = sstrdup(data);
			}
			else if(!strcmp(tag, "ChannelStatsDB"))
			{
				ChannelStatsDB = sstrdup(data);
			}
			else if(!strcmp(tag, "TLDDB"))
			{
				TLDDB = sstrdup(data);
			}
			else if(!strcmp(tag, "excludeDB"))
			{
				excludeDB = sstrdup(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!PIDFilename)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_PID_ERROR));
		return -1;
	}
	if (HTMLFilename)
	{
		denora->do_html = 1;
	}
	if (!ChannelDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_CHANDB_ERROR));
		return -1;
	}
	if (!ctcpDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_CTCP_ERROR));
		return -1;
	}
	if (!ServerDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_SERVER_ERROR));
		return -1;
	}
	if (!ChannelStatsDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_CHANSTATS_ERROR));
		return -1;
	}
	if (!TLDDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_TLD_ERROR));
		return -1;
	}
	if (!excludeDB)
	{
		DenoraXMLDebug(langstring(CONFIG_FILENAME_EXCLUDE_ERROR));
		return -1;
	}
	if (!statsDB)
	{
		DenoraXMLDebug("statsDB is not defined");
		return -1;
	}
	if (!AdminDB)
	{
		DenoraXMLDebug("AdminDB is not defined");
		return -1;
	}

	return 1;
}

/*************************************************************************/

int DenoraParseBackUpBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "KeepBackups"))
			{
				KeepBackups = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "KeepBackupsFor"))
			{
				KeepBackupsFor = dotime(data);
			}
			else if(!strcmp(tag, "BackupFreq"))
			{
				BackupFreq = dotime(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (KeepBackups && !KeepBackupsFor)
	{
		DenoraXMLDebug("KeepBackupsFor is not defined");
		return -1;
	}
	if (KeepBackups && !BackupFreq)
	{
		DenoraXMLDebug("BackupFreq is not defined");
		return -1;
	}
	return 1;
}


/*************************************************************************/

int DenoraParseTimeOutBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag && data)
		{
			if (!strcmp(tag, "ReadTimeout"))
			{
				ReadTimeout = dotime(data);
				if (ReadTimeout < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "WarningTimeout"))
			{
				WarningTimeout = dotime(data);
				if (WarningTimeout < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "UpdateTimeout"))
			{
				UpdateTimeout = dotime(data);
				if (UpdateTimeout < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "HTMLTimeout"))
			{
				HTMLTimeout = dotime(data);
				if (HTMLTimeout < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
				if (HTMLTimeout <= 120 )
				{
					DenoraXMLDebug("HTMLTimeout must be greater then 2m");
					return -1;
				}
			}
			else if(!strcmp(tag, "UptimeTimeout"))
			{
				UptimeTimeout = dotime(data);
				if (UptimeTimeout < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "PingFrequency"))
			{
				PingFrequency = dotime(data);
				if (PingFrequency < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
				if (PingFrequency <= 120 )
				{
					DenoraXMLDebug("PingFrequency must be greater then 2m");
					return -1;
				}

			}
			else if(!strcmp(tag, "SQLPingFreq"))
			{
				SQLPingFreq = dotime(data);
				if (SQLPingFreq < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "ClearInActive"))
			{
				ClearInActive = dotime(data);
				if (ClearInActive < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "ClearChanInActive"))
			{
				ClearChanInActive = dotime(data);
				if (ClearChanInActive < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "UserCacheTime"))
			{
				UserCacheTime = dotime(data);
				if (UserCacheTime < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "UserCleanFreq"))
			{
				UserCleanFreq = dotime(data);
				if (UserCleanFreq < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "ServerCacheTime"))
			{
				ServerCacheTime = dotime(data);
				if (ServerCacheTime < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}
			else if(!strcmp(tag, "ServerCleanFreq"))
			{
				ServerCleanFreq = dotime(data);
				if (ServerCleanFreq < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_TIME));
					return -1;
				}
			}

			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!ReadTimeout)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_READ_ERROR));
		return -1;
	}
	if (!WarningTimeout)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_WARNING_ERROR));
		return -1;
	}
	if (!UpdateTimeout)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_UPDATE_ERROR));
		return -1;
	}
	if (!PingFrequency)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_PING_ERROR));
		return -1;
	}
	if (!UptimeTimeout)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_UPTIME_ERROR));
		return -1;
	}
	if (!HTMLTimeout)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_HTML_ERROR));
		return -1;
	}
	if (!SQLPingFreq)
	{
		DenoraXMLDebug(langstring(CONFIG_TIMEOUT_SQL_ERROR));
		return -1;
	}
	if (!ClearInActive)
	{
		ClearInActive = dotime("30d");
	}
	if (!ClearChanInActive)
	{
		ClearChanInActive = dotime("30d");
	}
	return 1;
}

/*************************************************************************/

int DenoraParseOptionBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		optag = IsOptionTag(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "KeepLogs"))
			{
				KeepLogs = atoi(data);
				if (KeepLogs < 0)
				{
					DenoraXMLDebug(langstring(CONFIG_ERROR_POSTIVE_VALUE));
					return -1;
				}
			}
			else if(!strcmp(tag, "LogChannel"))
			{
				LogChannel = sstrdup(data);
			}
			else if(!strcmp(tag, "ENCModule"))
			{
				ENCModule = sstrdup(data);
			}
			else if(!strcmp(tag, "Smiley"))
			{
				Smiley = sstrdup(data);
			}
			else if(!strcmp(tag, "DumpCore"))
			{
				if (optag)
				{
					DumpCore = 1;
				}
				else
				{
					DumpCore = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "SQLQueryBT"))
			{
				if (optag)
				{
					SQLQueryBT = 1;
				}
				else
				{
					SQLQueryBT = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "BackTrace"))
			{
				if (optag)
				{
					BackTrace = 1;
				}
				else
				{
					BackTrace = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "HideStatsO"))
			{
				if (optag)
				{
					HideStatsO = 1;
				}
				else
				{
					HideStatsO = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "HideUline"))
			{
				if (optag)
				{
					HideUline = 1;
				}
				else
				{
					HideUline = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "UseTokens"))
			{
				if (optag)
				{
					UseTokens = 1;
				}
				else
				{
					UseTokens = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "NoLogs"))
			{
				if (optag)
				{
					NoLogs = 1;
				}
				else
				{
					NoLogs = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "ChanStatsTrigger"))
			{
				if (strlen(data) == 1)
				{
					ChanStatsTrigger = sstrdup(data);
				}
				else
				{
					DenoraXMLDebug("trigger can only be 1 character longer");
				}
			}
			else if(!strcmp(tag, "UseTS6"))
			{
				if (optag)
				{
					UseTS6 = 1;
				}
				else
				{
					UseTS6 = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "UseZIP"))
			{
				if (optag)
				{
					UseZIP = 1;
				}
				else
				{
					UseZIP = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "JupeMaster"))
			{
				JupeMaster = sstrdup(data);
			}
			else if(!strcmp(tag, "CSDefFlag"))
			{
				if (!stricmp(data, "on"))
				{
					CSDefFlag = 1;
				}
				else if (!stricmp(data, "notice"))
				{
					CSDefFlag = 2;
				}
				else
				{
					CSDefFlag = 0;
				}
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!Smiley)
	{
		Smiley = sstrdup(SMILEYS);
	}
	if (!ChanStatsTrigger)
	{
		ChanStatsTrigger = sstrdup("!");
	}

	if (!ENCModule)
	{
		ENCModule = sstrdup("enc_none");
	}

	return 1;
}

/*************************************************************************/

int DenoraParseAdminBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag, language;
	char *attrib;
	char *value, *name, *passwd, *hostname;
	int  encrypttype;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (tag)
		{
			if (!strcmp(tag, "name"))
			{
				name = sstrdup(data);
			}
			else if(!strcmp(tag, "passwd"))
			{
				passwd = sstrdup(data);
				attrib = GetAtrribTag(lines[i]);
				value = GetAtrribValue(lines[i]);
				if (!stricmp(attrib,"encode"))
				{
					if (!stricmp(value,"plain"))
					{
						encrypttype = 1;
					}
					else if (!stricmp(value,"crypt"))
					{
						encrypttype = 1;
					}
					else if (!stricmp(value,"md5"))
					{
						encrypttype = 1;
					}
					else
					{
						encrypttype = 1;
					}
					free(attrib);
					free(value);
				}
			}
			else if(!strcmp(tag, "hostname"))
			{
				hostname = sstrdup(data);
			}
			else if(!strcmp(tag, "language"))
			{
				language = atoi(data);
				if (language < 1 || language > NUM_LANGS)
				{
					DenoraXMLDebug(langstring(CONFIG_INVALID_LANG));
					return -1;
				}
				language--;
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!name)
	{
		DenoraXMLDebug(langstring(CONFIG_ADMIN_NAME_ERROR));
		return -1;
	}
	if (!hostname)
	{
		DenoraXMLDebug(langstring(CONFIG_ADMIN_HOST_ERROR));
		return -1;
	}
	if (!passwd)
	{
		
		DenoraXMLDebug(langstring(CONFIG_ADMIN_PASS_ERROR));
		return -1;
	}
	//add_sqladmin(name, passwd, 0, hostname, language, 1, encrypttype);
	free(name);
	free(passwd);
	free(hostname);
	return 1;
}

/*************************************************************************/

int DenoraParseSQLBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		optag = IsOptionTag(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "Module"))
			{
				if (!stricmp(data, "mysql"))
				{
					sqltype = SQL_MYSQL;
					SQLModule = sstrdup(data);
				}
				if (!stricmp(data, "postgre"))
				{
					sqltype = SQL_PGSQL;
					SQLModule = sstrdup(data);
				}
			}
			else if(!strcmp(tag, "SqlHost"))
			{
				SqlHost = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlUser"))
			{
				SqlUser = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlPass"))
			{
				SqlPass = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlName"))
			{
				SqlName = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlSock"))
			{
				SqlSock = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlPort"))
			{
				SqlPort = atoi(data);
				if (SqlPort < 1 || SqlPort > 65535)
				{
					printf("%s", langstring(CONFIG_PORT_ERROR));
					return -1;
				}
			}
			else if(!strcmp(tag, "SqlRetries"))
			{
				SqlRetries = atoi(data);
				if (SqlRetries < 0)
				{
					printf("%s", langstring(CONFIG_ERROR_POSTIVE_VALUE));
					return -1;
				}
			}
			else if(!strcmp(tag, "SqlRetryGap"))
			{
				SqlRetryGap = atoi(data);
				if (SqlRetryGap < 0)
				{
					printf("%s", langstring(CONFIG_ERROR_POSTIVE_VALUE));
					return -1;
				}
			}
			else if(!strcmp(tag, "SQLRetryOnServerLost"))
			{
				if (optag)
				{
					SQLRetryOnServerLost = 1;
				}
				else
				{
					SQLRetryOnServerLost = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "DisableMySQLOPT"))
			{
				if (optag)
				{
					DisableMySQLOPT = 1;
				}
				else
				{
					DisableMySQLOPT = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "KeepUserTable"))
			{
				if (optag)
				{
					KeepUserTable = 1;
				}
				else
				{
					KeepUserTable = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "KeepServerTable"))
			{
				if (optag)
				{
					KeepServerTable = 1;
				}
				else
				{
					KeepServerTable = XmlConfigSetFeature(data);
				}
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!SqlHost)
	{
		denora->do_sql = 0;
	}
	if (SqlHost && !SqlUser)
	{
		printf("%s", langstring(CONFIG_SQL_USER_ERROR));
		return -1;
	}
	if (SqlHost && !SqlName)
	{
		printf("%s", langstring(CONFIG_SQL_NAME_ERROR));
		return -1;
	}
	if (DisableMySQLOPT)
	{
		alog(LOG_NORMAL,
		     "=============================================================");
		alog(LOG_NORMAL,
		     "=== MySQL Optimization Disabled                           ===");
		alog(LOG_NORMAL,
		     "You have choosen to run Denora without MySQL optimization");
		alog(LOG_NORMAL,
		     "Should only run it like this if you are having problems with");
		alog(LOG_NORMAL,
		     "MySQL and zlib being out of sync with each other");
		alog(LOG_NORMAL,
		     "=============================================================");
	}

	return 1;
}

/*************************************************************************/

int DenoraParseSQLTableBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		optag = IsOptionTag(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "UserTable"))
			{
				UserTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanBansTable"))
			{
				ChanBansTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanExceptTable"))
			{
				ChanExceptTable = sstrdup(data);
			}
			else if(!strcmp(tag, "IsOnTable"))
			{
				IsOnTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ServerTable"))
			{
				ServerTable = sstrdup(data);
			}
			else if(!strcmp(tag, "GlineTable"))
			{
				GlineTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanTable"))
			{
				ChanTable = sstrdup(data);
			}
			else if(!strcmp(tag, "MaxValueTable"))
			{
				MaxValueTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanInviteTable"))
			{
				ChanInviteTable = sstrdup(data);
			}
			else if(!strcmp(tag, "TLDTable"))
			{
				TLDTable = sstrdup(data);
			}
			else if(!strcmp(tag, "CTCPTable"))
			{
				CTCPTable = sstrdup(data);
			}
			else if(!strcmp(tag, "SglineTable"))
			{
				SglineTable = sstrdup(data);
			}
			else if(!strcmp(tag, "SqlineTable"))
			{
				SqlineTable = sstrdup(data);
			}
			else if(!strcmp(tag, "AliasesTable"))
			{
				AliasesTable = sstrdup(data);
			}
			else if(!strcmp(tag, "CStatsTable"))
			{
				CStatsTable = sstrdup(data);
			}
			else if(!strcmp(tag, "UStatsTable"))
			{
				UStatsTable = sstrdup(data);
			}
			else if(!strcmp(tag, "StatsTable"))
			{
				StatsTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanStatsTable"))
			{
				ChanStatsTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ServerStatsTable"))
			{
				ServerStatsTable = sstrdup(data);
			}
			else if(!strcmp(tag, "SpamTable"))
			{
				SpamTable = sstrdup(data);
			}
			else if(!strcmp(tag, "CurrentTable"))
			{
				CurrentTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ChanQuietTable"))
			{
				ChanQuietTable = sstrdup(data);
			}
			else if(!strcmp(tag, "AdminTable"))
			{
				AdminTable = sstrdup(data);
			}
			else if(!strcmp(tag, "ExcludeTable"))
			{
				ExcludeTable = sstrdup(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	if (!UserTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_USER_ERROR));
		return -1;
	}
	if (!ChanBansTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_CHANBANS_ERROR));
		return -1;
	}
	if (!ChanExceptTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_CHANEXCEPT_ERROR));
		return -1;
	}
	if (!IsOnTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_ISON_ERROR));
		return -1;
	}
	if (!ServerTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_SERVER_ERROR));
		return -1;
	}
	if (!GlineTable)
	{
		DenoraXMLDebug(langstring(CONFIG_TABLE_GLINE_ERROR));
		return -1;
	}
	if (!ChanTable)
	{
		DenoraXMLDebug("Lacking chan definition in tables block");
		return -1;
	}
	if (!MaxValueTable)
	{
		DenoraXMLDebug("Lacking maxvalue definition in tables block");
		return -1;
	}
	if (!ChanInviteTable)
	{
		ChanInviteTable = sstrdup("chaninvites");
		return -1;
	}
	if (!TLDTable)
	{
		DenoraXMLDebug("Lacking tld definition in tables block");
		return -1;
	}
	if (!CTCPTable)
	{
		DenoraXMLDebug("Lacking ctcp definition in tables block");
		return -1;
	}
	if (!ChanStatsTable)
	{
		DenoraXMLDebug("Lacking chanstats definition in tables block");
		return -1;
	}
	if (!ServerStatsTable)
	{
		DenoraXMLDebug("Lacking servstats definition in tables block");
		return -1;
	}
	if (!SglineTable)
	{
		SglineTable = sstrdup("sgline");
	}
	if (!SqlineTable)
	{
		SqlineTable = sstrdup("sqline");
	}
	if (!AliasesTable)
	{
		AliasesTable = sstrdup("aliases");
	}
	if (!CStatsTable)
	{
		CStatsTable = sstrdup("cstats");
	}
	if (!UStatsTable)
	{
		UStatsTable = sstrdup("ustats");
	}
	if (!StatsTable)
	{
		StatsTable = sstrdup("stats");
	}
	if (!SpamTable)
	{
		SpamTable = sstrdup("spamfilters");
	}
	if (!CurrentTable)
	{
		CurrentTable = sstrdup("current");
	}
	if (!ChanQuietTable)
	{
		ChanQuietTable = sstrdup("chanquiet");
	}
	if (!AdminTable)
	{
		AdminTable = sstrdup("admin");
	}


	return 1;
}

/*************************************************************************/

int DenoraParseNetInfoBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		optag = IsOptionTag(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "NetworkName"))
			{
				NetworkName = sstrdup(data);
			}
			else if(!strcmp(tag, "Numeric"))
			{
				Numeric = sstrdup(data);
			}
			else if(!strcmp(tag, "UserStatsRegistered"))
			{
				if (optag)
				{
					UserStatsRegistered = 1;
				}
				else
				{
					UserStatsRegistered = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "UserStatsExcludeBots"))
			{
				if (optag)
				{
					UserStatsExcludeBots = 1;
				}
				else
				{
					UserStatsExcludeBots = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "SP_HTML"))
			{
				if (optag)
				{
					SP_HTML = 1;
				}
				else
				{
					SP_HTML = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "ExcludeServers"))
			{
				ExcludeServers = buildStringList(data, &NumExcludeServers);
			}
			else if(!strcmp(tag, "StatsPage"))
			{
				StatsPage = sstrdup(data);
			}
			else if(!strcmp(tag, "NickChar"))
			{
				NickChar = sstrdup(data);
			}
			else if(!strcmp(tag, "HiddenPrefix"))
			{
				HiddenPrefix = sstrdup(data);
			}
			else if(!strcmp(tag, "HiddenSuffix"))
			{
				HiddenSuffix = sstrdup(data);
			}

			else if(!strcmp(tag, "CTCPUsers"))
			{
				if (optag)
				{
					CTCPUsers = 1;
				}
				else
				{
					CTCPUsers = XmlConfigSetFeature(data);
				}
			}
			else if(!strcmp(tag, "CTCPUsersEOB"))
			{
				if (optag)
				{
					CTCPUsersEOB = 1;
				}
				else
				{
					CTCPUsersEOB = XmlConfigSetFeature(data);
				}
			}


			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (BadPtr(NetworkName))
	{
		DenoraXMLDebug(langstring(CONFIG_NETINFO_NAME_ERROR));
		return -1;
	}
	if (!HiddenPrefix)
	{
		HiddenPrefix = "";
	}
	if (!HiddenSuffix)
	{
		HiddenSuffix = sstrdup(".users.mynet.tld");
	}
	return 1;
}

/*************************************************************************/


int DenoraParseModuleBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	Conf_Modules *x = new_modules;
	int c = 0, ac = 0, oc = 0;

	/* we dont free here because we do it if we fail */

	if (!x)
	{
		x = (Conf_Modules *) malloc(sizeof(Conf_Modules));
		memset((char *) x, '\0', sizeof(Conf_Modules));
		new_modules = x;
	}
	else
	{
		DenoraXMLDebug("Multiple module blocks in config file");
		return -1;
	}
	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "autoload"))
			{
				if ((ac + 1) > 128)
				{
					DenoraXMLDebug("Excessive autoloading modules (max 128)");
					return -1;
				}
				x->autoload[ac] = sstrdup(data);
				ac++;
			}
			else if(!strcmp(tag, "delayed"))
			{
				if ((oc + 1) > 128)
				{
					DenoraXMLDebug(langstring(CONFIG_DELAYMOD_T0MANY));
					return -1;
				}
				x->delayed[oc] = sstrdup(data);
				oc++;
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	ModulesNumber = ac;
	ModulesDelayedNumber = oc;
	if (modules)
	{
		for (i = 0; modules->autoload[i]; i++)
			free(modules->autoload[i]);
		for (i = 0; modules->delayed[i]; i++)
			free(modules->delayed[i]);
		free(modules);
	}
	modules = new_modules;
	new_modules = NULL;
	return 1;
}



/*************************************************************************/


int DenoraParseProto_IRCdBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "name"))
			{
				ircd->name = sstrdup(data);
				denora->version_protocol = sstrdup(data);
			}
			else if(!strcmp(tag, "p10"))
			{
				ircd->p10 = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "ts6"))
			{
				ircd->ts6 = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "usetsmode"))
			{
				UseTSMODE = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "supportopmode"))
			{
				SupportOperFlag = XmlConfigSetFeature(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	return 1;
}

/*************************************************************************/

int DenoraParseXMLRPCBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		optag = IsOptionTag(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "XMLRPC_Host"))
			{
				XMLRPC_Host = sstrdup(data);
			}
			else if(!strcmp(tag, "XMLRPC_Port"))
			{
				XMLRPC_Port = atoi(data);
			}
			else if(!strcmp(tag, "ExtSockIPs"))
			{
				SockIPNumber++;
				ExtSockIPs =
				    realloc(ExtSockIPs, sizeof(char *) * SockIPNumber);
				ExtSockIPs[SockIPNumber - 1] = sstrdup(data);
			}
			else if(!strcmp(tag, "XMLRPC_Enable"))
			{
				if (optag)
				{
					XMLRPC_Enable = 1;
				}
				else
				{
					XMLRPC_Enable = XmlConfigSetFeature(data);
				}
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (XMLRPC_Enable)
	{
		if (!XMLRPC_Host)
		{
			if (LocalHost)
			{
				XMLRPC_Host = sstrdup(LocalHost);
			}
			else
			{
				DenoraXMLDebug("XMLRPC enabled by no host defined");
				return -1;
			}
		}
	}
	return 1;
}

/*************************************************************************/


int DenoraParseProto_CapabBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
	
			if (!strcmp(tag, "noquit"))
			{
				ircdcap->noquit = (XmlConfigSetFeature(data) == 1 ? CAPAB_NOQUIT : 0);
			}
			else if(!strcmp(tag, "tsmode"))
			{
				ircdcap->tsmode = (XmlConfigSetFeature(data) == 1 ? CAPAB_TSMODE : 0);
			}
			else if(!strcmp(tag, "unconnect"))
			{
				ircdcap->unconnect = (XmlConfigSetFeature(data) == 1 ? CAPAB_UNCONNECT : 0);
			}
			else if(!strcmp(tag, "nickip"))
			{
				ircdcap->nickip = (XmlConfigSetFeature(data) == 1 ? CAPAB_NICKIP : 0);
			}
			else if(!strcmp(tag, "nsjoin"))
			{
				ircdcap->nsjoin = (XmlConfigSetFeature(data) == 1 ? CAPAB_NSJOIN : 0);
			}
			else if(!strcmp(tag, "zip"))
			{
				ircdcap->zip = (XmlConfigSetFeature(data) == 1 ? CAPAB_ZIP : 0);
			}
			else if(!strcmp(tag, "burst"))
			{
				ircdcap->burst = (XmlConfigSetFeature(data) == 1 ? CAPAB_BURST : 0);
			}
			else if(!strcmp(tag, "ts5"))
			{
				ircdcap->ts5 = (XmlConfigSetFeature(data) == 1 ? CAPAB_TS5 : 0);
			}
			else if(!strcmp(tag, "ts3"))
			{
				ircdcap->ts3 = (XmlConfigSetFeature(data) == 1 ? CAPAB_TS3 : 0);
			}
			else if(!strcmp(tag, "dkey"))
			{
				ircdcap->dkey = (XmlConfigSetFeature(data) == 1 ? CAPAB_DKEY : 0);
			}
			else if(!strcmp(tag, "pt4"))
			{
				ircdcap->pt4 = (XmlConfigSetFeature(data) == 1 ? CAPAB_PT4 : 0);
			}
			else if(!strcmp(tag, "scs"))
			{
				ircdcap->scs = (XmlConfigSetFeature(data) == 1 ? CAPAB_SCS : 0);
			}
			else if(!strcmp(tag, "qs"))
			{
				ircdcap->qs = (XmlConfigSetFeature(data) == 1 ? CAPAB_QS : 0);
			}
			else if(!strcmp(tag, "uid"))
			{
				ircdcap->uid = (XmlConfigSetFeature(data) == 1 ? CAPAB_UID : 0);
			}
			else if(!strcmp(tag, "knock"))
			{
				ircdcap->knock = (XmlConfigSetFeature(data) == 1 ? CAPAB_KNOCK : 0);
			}
			else if(!strcmp(tag, "client"))
			{
				ircdcap->client = (XmlConfigSetFeature(data) == 1 ? CAPAB_CLIENT : 0);
			}
			else if(!strcmp(tag, "ipv6"))
			{
				ircdcap->ipv6 = (XmlConfigSetFeature(data) == 1 ? CAPAB_IPV6 : 0);
			}
			else if(!strcmp(tag, "ssj5"))
			{
				ircdcap->ssj5 = (XmlConfigSetFeature(data) == 1 ? CAPAB_SSJ5 : 0);
			}
			else if(!strcmp(tag, "sn2"))
			{
				ircdcap->sn2 = (XmlConfigSetFeature(data) == 1 ? CAPAB_SN2 : 0);
			}
			else if(!strcmp(tag, "token"))
			{
				ircdcap->token = (XmlConfigSetFeature(data) == 1 ? CAPAB_TOKEN : 0);
			}
			else if(!strcmp(tag, "vhost"))
			{
				ircdcap->vhost = (XmlConfigSetFeature(data) == 1 ? CAPAB_VHOST : 0);
			}
			else if(!strcmp(tag, "ssj3"))
			{
				ircdcap->ssj3 = (XmlConfigSetFeature(data) == 1 ? CAPAB_SSJ3 : 0);
			}
			else if(!strcmp(tag, "nick2"))
			{
				ircdcap->nick2 = (XmlConfigSetFeature(data) == 1 ? CAPAB_NICK2 : 0);;
			}
			else if(!strcmp(tag, "umode2"))
			{
				ircdcap->umode2 = (XmlConfigSetFeature(data) == 1 ? CAPAB_UMODE2 : 0);
			}
			else if(!strcmp(tag, "vl"))
			{
				ircdcap->vl = (XmlConfigSetFeature(data) == 1 ? CAPAB_VL : 0);
			}
			else if(!strcmp(tag, "tlkext"))
			{
				ircdcap->tlkext = (XmlConfigSetFeature(data) == 1 ? CAPAB_TLKEXT : 0);
			}
			else if(!strcmp(tag, "dodkey"))
			{
				ircdcap->dodkey = (XmlConfigSetFeature(data) == 1 ? CAPAB_DODKEY : 0);
			}
			else if(!strcmp(tag, "dozip"))
			{
				ircdcap->dozip = (XmlConfigSetFeature(data) == 1 ? CAPAB_DOZIP : 0);
			}
			else if(!strcmp(tag, "chanmodes"))
			{
				ircdcap->chanmodes = (XmlConfigSetFeature(data) == 1 ? CAPAB_CHANMODE : 0);
			}
			else if(!strcmp(tag, "sjb64"))
			{
				ircdcap->sjb64 = (XmlConfigSetFeature(data) == 1 ? CAPAB_SJB64 : 0);
			}
			else if(!strcmp(tag, "nickchars"))
			{
				ircdcap->nickchars = (XmlConfigSetFeature(data) == 1 ? CAPAB_NICKCHARS : 0);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_ChannelUserModeBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	char *symbol;
	char *mode;
	int flag;


	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (tag)
		{
			if (!strcmp(tag, "mode"))
			{
				mode = sstrdup(data);
			}
			else if(!strcmp(tag, "symbol"))
			{
				symbol = sstrdup(data);
			}
			else if(!strcmp(tag, "flag"))
			{
				flag = ReturnChanUModeFromToken(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	ModuleSetChanUMode(*mode, *symbol, flag);

	return 1;
}

/*************************************************************************/


int DenoraParseProto_ServicesBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (tag)
		{
			if (!strcmp(tag, "modes"))
			{
				ircd->servicesmode = sstrdup(data);
			}
			free(tag);
			free(data);
		}
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_FeaturesBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (tag)
		{
			if (!strcmp(tag, "vhost"))
			{
				ircd->vhost = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "vident"))
			{
				ircd->vident = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "vhostmode"))
			{
				if (XmlConfigSetMode(data))
				{
					ircd->vhostmode = ReturnModeFromToken(data);
				}
			}
			else if(!strcmp(tag, "vhost_on_nick"))
			{
				ircd->nickvhost = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "opermode"))
			{
				ircd_modes.user_oper = ReturnModeFromToken(data);
			}
			else if(!strcmp(tag, "sgline"))
			{
				ircd->sgline = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "sqline"))
			{
				ircd->sqline = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "szline"))
			{
				ircd->szline = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "sgline_sql"))
			{
				ircd->sgline_table = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "sqline_sql"))
			{
				ircd->sqline_table = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "szline_sql"))
			{
				ircd->szline_table = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "nickip"))
			{
				ircd->nickip = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "ziplink"))
			{
				ircd->zip = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "ssllink"))
			{
				ircd->ssl = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "exceptions"))
			{
				ircd->except = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "tokens"))
			{
				ircd->token = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "casetokens"))
			{
				ircd->tokencaseless = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "timestamp64"))
			{
				ircd->sjb64 = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "invitemode"))
			{
				ircd->invitemode = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "uline"))
			{
				ircd->uline = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "svid"))
			{
				ircd->has_svid = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "hiddenop"))
			{
				ircd->hideoper = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "syncstate"))
			{
				ircd->syncstate = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "numerics"))
			{
				ircd->numerics = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "spamfilterchar"))
			{
				ircd->spamfilter = sstrdup(data);
			}
			else if(!strcmp(tag, "vhost_char"))
			{
				ircd->vhostchar = atoi(data);
			}
			else if(!strcmp(tag, "vhost_char_other"))
			{
				ircd->vhostchar2 = atoi(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_UserModeBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

		if (!tag) 
		{
			break;
		}

		res = ReturnModeFromToken(tag);
		if (!res)
		{
			break;
		}

		if (res && XmlConfigSetFeature(data))
		{
				ModuleSetUserMode(res, IRCD_ENABLE);
		}
		free(tag);
		free(data);

	}
	ModuleUpdateSQLUserMode();
	return 1;
}


/*************************************************************************/


int DenoraParseProto_ChannelModeBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		res = ReturnModeFromToken(tag);

		if (res && XmlConfigSetFeature(data))
		{
				CreateChanMode(res, NULL, NULL);
		}
		free(tag);
		free(data);

	}
	ModuleUpdateSQLChanMode();
	return 1;
}


/*************************************************************************/


int DenoraParseProto_ChannelBanModeBlock(char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		res = ReturnModeFromToken(tag);

		if (tag)
		{
			if (!strcmp(tag, "ban"))
			{
				CreateChanBanMode(res, add_ban, del_ban);
			}
			else if(!strcmp(tag, "exception"))
			{
				CreateChanBanMode(res, add_exception, del_exception);
			}
			else if(!strcmp(tag, "invite"))
			{
				CreateChanBanMode(res, add_invite, del_invite);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}

	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_WarningBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (tag)
		{
			if (!strcmp(tag, "extra"))
			{
				ircd->extrawarning = XmlConfigSetFeature(data);
			}
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
		}
		free(tag);
		free(data);
	}
}


/*************************************************************************/


int DenoraParseProto_ChannelFeaturesBlock(char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i <= SizeOfArray(lines); i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (tag)
		{
			if (!strcmp(tag, "owner"))
			{
				ircd->owner = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "protect"))
			{
				ircd->protect = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "halfop"))
			{
				ircd->halfop = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "floodmode"))
			{
				ircd->fmode = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "floodmodeother"))
			{
				ircd->jmode = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "chanlink"))
			{
				ircd->Lmode = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "floodmode_char"))
			{
				ircd->floodchar = atoi(data);
			}
			else if(!strcmp(tag, "floodmodeother_char"))
			{
				ircd->floodchar_alternative = atoi(data);
			}
			else if(!strcmp(tag, "chanlink_char"))
			{
				ircd->chanforward = atoi(data);
			}
			else if(!strcmp(tag, "jointhrottle"))
			{
				ircd->jointhrottle = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "gagged"))
			{
				ircd->gagged = XmlConfigSetFeature(data);
			}
			else if(!strcmp(tag, "sjoin_ban_char"))
			{
				ircd->sjoinbanchar = atoi(data);
			}
			else if(!strcmp(tag, "sjoin_except_char"))
			{
				ircd->sjoinexchar = atoi(data);
			}
			else if(!strcmp(tag, "sjoin_invite_char"))
			{
				ircd->sjoinivchar = atoi(data);
			}
			else if(!strcmp(tag, "ban_char"))
			{
				ircd->ban_char = atoi(data);
			}
			else if(!strcmp(tag, "except_char"))
			{
				ircd->except_char = atoi(data);
			}
			else if(!strcmp(tag, "invite_char"))
			{
				ircd->invite_char = atoi(data);
			}
			else if(!strcmp(tag, "nickchgflood"))
			{
				ircd->nickchgfloodchar = atoi(data);
			}
			else if(!strcmp(tag, "persistchan"))
			{
				if (XmlConfigSetMode(data))
				{
					ircd->persist_char = ReturnModeFromToken(data);
				}
			}
			else if(!strcmp(tag, "chan_fmode"))
			{
				if (XmlConfigSetMode(data))
				{
					ircd->chan_fmode = ReturnModeFromToken(data);
				}
			}
			else if(!strcmp(tag, "chan_jmode"))
			{
				if (XmlConfigSetMode(data))
				{
					ircd->chan_jmode = ReturnModeFromToken(data);
				}
			}
			else if(!strcmp(tag, "chan_lmode"))
			{
				if (XmlConfigSetMode(data))
				{
					ircd->chan_lmode = ReturnModeFromToken(data);
				}
			}
	
			else
			{
				DenoraXMLDebug("Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
}


/*************************************************************************/

void insert_config(config * c)
{
	int configindex = CONFIGHASH(c->name);

	c->prev = NULL;
	c->next = configlists[configindex];
	if (c->next)
	{
		c->next->prev = c;
	}
	configlists[configindex] = c;

	return;
}

config *DenoraXMLConfigBlockCreate(char *newblockname, int (parser)(char **av))
{
	config *c;
	c = calloc(sizeof(config), 1);
	c->name = strdup(newblockname);
	insert_config(c);
	c->parser = parser;
	return c;
}


