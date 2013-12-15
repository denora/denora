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

char *GetConfigStartTagName(char *line)
{
	char *linedata;
	if (line)
	{
		linedata = myStrGetToken(line, '<', 1);
		linedata = myStrGetToken(linedata, '>', 0);
		return linedata;
	}
	return NULL;
}


char *GetConfigEndTagName(char *line)
{
	char *linedata;
	if (line)
	{
		linedata = myStrGetToken(line, '<', 1);
		linedata = myStrGetToken(linedata, '>', 0);
		linedata = myStrGetToken(linedata, '/', 1);
		return linedata;
	}
	return NULL;
}

char *GetOptionTagData(char *line)
{
	char *linedata;

	if (line)
	{
		linedata = myStrGetToken(line, '>', 1);
		linedata = myStrGetToken(linedata, '<', 0);
		return linedata;
	}
	return NULL;
}

char *GetOptionTagName(char *line)
{
	char *linedata;
	int optag;

	if (line)
	{
		optag = IsOptionTag(line);
		if (optag)
		{
			linedata = myStrGetToken(line, '<', 1);
			linedata = myStrGetToken(linedata, '>', 0);
			linedata = myStrGetToken(linedata, '/', 0);
			strnrepl(linedata, BUFSIZE, " ", "");
		}
		else
		{
			linedata = myStrGetToken(line, '<', 1);
			linedata = myStrGetToken(linedata, '>', 0);
		}
		return linedata;
	}
	return NULL;
}

int IsOptionTag(char *line)
{
	int numgt;
	char *linedata = sstrdup(line);
	numgt = myNumToken(linedata, '>');

	if (myNumToken(linedata, '/') && numgt == 1)
	{
		free(linedata);
		return 1;
	}
	free(linedata);
	return 0;
}

void DenoraXMLIRCdConfig(char *file)
{
	char buf[512];

	ircd = malloc(sizeof(IRCDVar));
	ircdcap = malloc(sizeof(IRCDCAPAB));

	DenoraXMLConfigBlockCreate("ircd", DenoraParseProto_IRCdBlock, 5);
	DenoraXMLConfigBlockCreate("services", DenoraParseProto_ServicesBlock, 1);
	DenoraXMLConfigBlockCreate("features", DenoraParseProto_FeaturesBlock, 27);
	DenoraXMLConfigBlockCreate("warning", DenoraParseProto_WarningBlock, 1);
	DenoraXMLConfigBlockCreate("capab", DenoraParseProto_CapabBlock, 24);
	DenoraXMLConfigBlockCreate("usermodes", DenoraParseProto_UserModeBlock, 48);
	DenoraXMLConfigBlockCreate("chanmodes", DenoraParseProto_ChannelModeBlock, 48);
	DenoraXMLConfigBlockCreate("chanbanmodes", DenoraParseProto_ChannelBanModeBlock, 4);
	DenoraXMLConfigBlockCreate("channelfeatures", DenoraParseProto_ChannelFeaturesBlock, 22);
	DenoraXMLConfigBlockCreate("chanusermode", DenoraParseProto_ChannelUserModeBlock, 3);


	snprintf(buf, sizeof(buf), "ircdconfig/%s", file);
	DenoraParseXMLConfig(buf);
}


int DenoraConfigInit(void)
{
	DenoraXMLConfigBlockCreate("connect", DenoraParseConnectBlock, 7);
	DenoraXMLConfigBlockCreate("identity", DenoraParseIdentityBlock, 4);
	DenoraXMLConfigBlockCreate("statserv", DenoraParseStatServBlock, 7);
	DenoraXMLConfigBlockCreate("filenames", DenoraParseFileNamesBlock, 7);
	DenoraXMLConfigBlockCreate("backup", DenoraParseBackUpBlock, 3);
	DenoraXMLConfigBlockCreate("netinfo", DenoraParseNetInfoBlock, 3);
}

void DenoraParseXMLConfig(char *filename)
{

	FILE * fp;
	char * line = NULL;
	char *rawline = NULL;
	char *newline;
        size_t len = 0;
        ssize_t read;
	int numtok;
	int endblock;
	int skipuntil = 0;
	char *tag;
	char *data;
	config *c;
	int startblock = 0;
	int x = 0, res = 0;
	char **xmldata;

	alog(LOG_DEBUG, "Parsing %s", filename);

        fp = FileOpen(filename, "r");
        if (fp == NULL)
           return;

	while ((read = getline(&rawline, &len, fp)) != -1) 
	{
		line = sstrdup(rawline);
		strnrepl(line, BUFSIZE, "\n", "");
		strnrepl(line, BUFSIZE, "\t", "");

		/* skip if its a #  and c++ style */
		if (*line == '#' || *line == '/' && line[1] == '/')
		{
			free(line);
			continue;
		}
		/* xml <! -!> tags are ignored */
		if (*line == '<' && line[1] == '!')
		{
			skipuntil = 1;
			free(line);
		        continue;

	   	}
		if (*line == '-' && line[1] == '!' && line[2] == '>')
		{
			skipuntil = 0;
			free(line);
			continue;
		}
		if (skipuntil) 
		{ 	
			free(line);
			continue; 
		}

		if (!strcmp(line, "<denora>") || !strcmp(line, "</denora>"))
		{
			free(line);
			continue;
		}
		else
		{
			numtok = myNumToken(line, (const char) '<');
			if (numtok == 1)
			{
				endblock = myNumToken(line, (const char) '/');
				if (endblock)
				{
					tag = GetConfigEndTagName(rawline);
					c = DenoraXMLConfigFindBlock(tag);
					startblock = 0;
					if (c)
					{
						res = c->parser(c->numoptions, xmldata);
						if (res == -1)
						{
							alog(LOG_ERROR, "Failed to parse Config File Correctly");
							return;
						}
					}
					free(tag);

					free(line);
					continue;
				}
				else
				{
					tag = GetConfigStartTagName(rawline);
					c = DenoraXMLConfigFindBlock(tag);
					if (c)
					{
						xmldata = DenoraCallocArray(c->numoptions);
					}
					else
					{
						xmldata = DenoraCallocArray(25);
					}
					free(tag);
					startblock = 1;
					x = 0;

					free(line);
					continue;

				}
		   	}
			else
			{
				if (startblock)
				{
			        	strnrepl(rawline, BUFSIZE, "\n", "");
				        strnrepl(rawline, BUFSIZE, "\t", "");
					xmldata[x++] = StringDup(rawline);
				}			
	   		}
		}
       }
}



