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
		if (tag)
		{
			data = GetOptionTagData(lines[i]);
			printf("Config Options %s = %s\n", tag, data);
			free(tag);
			free(data);
		}
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


int DenoraConfigInit(void)
{
	DenoraXMLConfigBlockCreate("connect", DenoraParseConnectBlock, 4);
	DenoraXMLConfigBlockCreate("identity", DenoraParseConnectBlock, 4);

}