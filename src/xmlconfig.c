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

int DenoraParseConnectBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
					alog(LOG_ERROR, langstring(CONFIG_PORT_ERROR));
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
					alog(LOG_ERROR, langstring(CONFIG_PORT_ERROR));
					return -1;
				}
			}
			else if (!strcmp(tag, "bindhost") || !strcmp(tag,"LocalHost"))
			{
				LocalHost = sstrdup(data);
			}
			else
			{
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!RemoteServer)
	{
		alog(LOG_ERROR, langstring(CONFIG_HOSTNAME_ERROR));
		return -1;
	}
	if (BadPtr(IRCDModule))
	{
		alog(LOG_ERROR, langstring(CONFIG_PROTOCOL_ERROR));
		return -1;
	}
	if (!QuitPrefix)
		QuitPrefix = sstrdup("Quit:");
	if (!RemotePort)
	{
		alog(LOG_ERROR, langstring(CONFIG_PORT_ERROR_NOTDEF));
		return -1;
	}
	if (BadPtr(RemotePassword))
	{
		alog(LOG_ERROR, langstring(CONFIG_PASSWD_ERROR));
		return -1;
	}
	return 1;
}

/*************************************************************************/


int DenoraParseIdentityBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
					alog(LOG_ERROR, "Invalid server name");
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
				char *s;
				char *temp_userhost = sstrdup(data);
				/* Strip the user and host from user token */
				if (!(s = strchr(temp_userhost, '@')))
				{
					free(temp_userhost);
					alog(LOG_ERROR, "Missing `@' for Stats User");
					return -1;
				}
				*s++ = 0;
				ServiceUser = temp_userhost;
				ServiceHost = s;
			}

			else if(!strcmp(tag, "StatsLanguage"))
			{
				StatsLanguage = strtol(data, NULL, 10);
				if (StatsLanguage < 1 || StatsLanguage > NUM_LANGS)
				{
					StatsLanguage = 1;
					alog(LOG_ERROR, langstring(CONFIG_INVALID_LANG));
					return -1;
				}
			}
			else
			{
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	if (BadPtr(ServerName))
	{
		alog(LOG_ERROR, langstring(CONFIG_ID_NAME_ERROR));
		return -1;
	}
	if (BadPtr(ServerDesc) || !ServerDesc)
	{
		alog(LOG_ERROR, langstring(CONFIG_ID_DESC_ERROR));
		return -1;
	}
	if (BadPtr(ServiceUser))
	{
		alog(LOG_ERROR, langstring(CONFIG_ID_USER_ERROR));
		return -1;
	}
	if (BadPtr(ServiceHost))
	{
		alog(LOG_ERROR, langstring(CONFIG_ID_HOST_ERROR));
		return -1;
	}
	if (!StatsLanguage)
	{
		alog(LOG_ERROR, langstring(CONFIG_ID_LANG_ERROR));
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


int DenoraParseStatServBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
					alog(LOG_ERROR, langstring(CONFIG_SS_TOLONG));
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
					alog(LOG_ERROR, langstring(CONFIG_SS_TOLONG));
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}

	if (BadPtr(s_StatServ))
	{
		alog(LOG_ERROR, langstring(CONFIG_SS_NAME_ERROR));
		return -1;
	}
	if (BadPtr(desc_StatServ))
	{
		alog(LOG_ERROR, langstring(CONFIG_SS_DESC_ERROR));
		return -1;
	}
	if (s_StatServ_alias && BadPtr(desc_StatServ_alias))
	{
		alog(LOG_ERROR, langstring(CONFIG_SS_ALIASDESC_ERROR));
		return -1;
	}
	if (s_StatServ_alias)
	{
		if (BadPtr(s_StatServ_alias))
		{
			alog(LOG_ERROR, langstring(CONFIG_SS_ALIASDESC_ERROR));
			return -1;
		}
	}

	return 1;
}

/*************************************************************************/


int DenoraParseFileNamesBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (!PIDFilename)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_PID_ERROR));
		return -1;
	}
	if (HTMLFilename)
	{
		denora->do_html = 1;
	}
	if (!ChannelDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_CHANDB_ERROR));
		return -1;
	}
	if (!ctcpDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_CTCP_ERROR));
		return -1;
	}
	if (!ServerDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_SERVER_ERROR));
		return -1;
	}
	if (!ChannelStatsDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_CHANSTATS_ERROR));
		return -1;
	}
	if (!TLDDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_TLD_ERROR));
		return -1;
	}
	if (!excludeDB)
	{
		alog(LOG_ERROR, langstring(CONFIG_FILENAME_EXCLUDE_ERROR));
		return -1;
	}
	if (!statsDB)
	{
		alog(LOG_ERROR, "statsDB is not defined");
		return -1;
	}
	if (!AdminDB)
	{
		alog(LOG_ERROR, "AdminDB is not defined");
		return -1;
	}

	return 1;
}

/*************************************************************************/

int DenoraParseBackUpBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (KeepBackups && !KeepBackupsFor)
	{
		alog(LOG_ERROR, "KeepBackupsFor is not defined");
		return -1;
	}
	if (KeepBackups && !BackupFreq)
	{
		alog(LOG_ERROR, "BackupFreq is not defined");
		return -1;
	}
	return 1;
}


/*************************************************************************/

int DenoraParseNetInfoBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;
	int optag;

	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	if (BadPtr(NetworkName))
	{
		alog(LOG_ERROR, langstring(CONFIG_NETINFO_NAME_ERROR));
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


int DenoraParseProto_IRCdBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	return 1;
}

/*************************************************************************/


int DenoraParseProto_CapabBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

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
			alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
		}
		free(tag);
		free(data);
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_ChannelUserModeBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;
	char *symbol;
	char *mode;
	int flag;


	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}
	}
	alog(LOG_DEBUG, "Mode %s %d, Symbolw %s, Flag %d", mode, *mode, symbol, flag);

	ModuleSetChanUMode(*mode, *symbol, flag);

	return 1;
}

/*************************************************************************/


int DenoraParseProto_ServicesBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (!strcmp(tag, "modes"))
		{
			ircd->servicesmode = sstrdup(data);
		}
		free(tag);
		free(data);
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_FeaturesBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);

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
			ircd->spamfilter = atoi(data);
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
			alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
		}
		free(tag);
		free(data);
	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_UserModeBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i < count; i++)
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


int DenoraParseProto_ChannelModeBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i < count; i++)
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


int DenoraParseProto_ChannelBanModeBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;
	int res;

	for (i = 0; i < count; i++)
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
				alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
			}
			free(tag);
			free(data);
		}

	}
	return 1;
}


/*************************************************************************/


int DenoraParseProto_WarningBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (!strcmp(tag, "extra"))
		{
			ircd->extrawarning = XmlConfigSetFeature(data);
		}
		else
		{
			alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
		}
		free(tag);
		free(data);
	}
}


/*************************************************************************/


int DenoraParseProto_ChannelFeaturesBlock(int count, char **lines)
{
	int i;
	char *tag;
	char *data;

	for (i = 0; i < count; i++)
	{
		tag = GetOptionTagName(lines[i]);
		data = GetOptionTagData(lines[i]);
		if (!tag)
		{
			return -1;
		}


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
			alog(LOG_DEBUG,"Unknown tag %s and Data %s", tag, data);
		}
		free(tag);
		free(data);


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

config *DenoraXMLConfigBlockCreate(char *newblockname, int (parser)(int ac, char **av), int options)
{
	config *c;
	c = calloc(sizeof(config), 1);
	c->name = strdup(newblockname);
	insert_config(c);
	c->parser = parser;
	c->numoptions = (options ? options : 25);
	return c;
}


