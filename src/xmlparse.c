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
	int numgt = myNumToken(line, '>');
	int eqtag = myNumToken(line, '=');

	if (line)
	{
		optag = IsOptionTag(line);
		if (optag)
		{
			linedata = myStrGetToken(line, '<', 1);
			linedata = myStrGetToken(linedata, '>', 0);
			linedata = myStrGetToken(linedata, '/', 0);
		}
		else if (eqtag && numgt == 2)
		{
			linedata = myStrGetToken(line, '<', 1);
			linedata = myStrGetToken(linedata, '>', 0);
			linedata = myStrGetToken(linedata, '=', 0);
			linedata = myStrGetToken(linedata, ' ', 0);
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
	char *linedata;


	if (line)
	{
	  linedata = sstrdup(line);
  	  numgt = myNumToken(linedata, '>');
   	  if (myNumToken(linedata, '/') && numgt == 1)
	 {
		free(linedata);
		return 1;
	 }
	 free(linedata);
	}
	return 0;
}

char *GetAtrribValue(char *line)
{
	int numgt;
	char *linedata = sstrdup(line);
	char *attrib;
	numgt = myNumToken(linedata, '>');

	if (myNumToken(linedata, '=') && numgt == 2)
	{
		linedata = myStrGetToken(line, '<', 1);
		linedata = myStrGetToken(linedata, '>', 0);
		linedata = myStrGetToken(linedata, ' ', 1);
		attrib = myStrGetToken(linedata, '=', 1);
		free(linedata);
		return attrib;
	}
	free(linedata);
	return NULL;
}

char *GetAtrribTag(char *line)
{
	int numgt;
	char *linedata = sstrdup(line);
	char *attrib;
	numgt = myNumToken(linedata, '>');

	if (myNumToken(linedata, '=') && numgt == 2)
	{
		linedata = myStrGetToken(line, '<', 1);
		linedata = myStrGetToken(linedata, '>', 0);
		linedata = myStrGetToken(linedata, ' ', 1);
		attrib = myStrGetToken(linedata, '=', 0);
		free(linedata);
		return attrib;
	}
	free(linedata);
	return NULL;
}


void DenoraXMLIRCdConfig(char *file)
{
	char buf[512];

	ircd = malloc(sizeof(IRCDVar));
	ircdcap = malloc(sizeof(IRCDCAPAB));

	DenoraXMLConfigBlockCreate("ircd", DenoraParseProto_IRCdBlock);
	DenoraXMLConfigBlockCreate("services", DenoraParseProto_ServicesBlock);
	DenoraXMLConfigBlockCreate("features", DenoraParseProto_FeaturesBlock);
	DenoraXMLConfigBlockCreate("warning", DenoraParseProto_WarningBlock);
	DenoraXMLConfigBlockCreate("capab", DenoraParseProto_CapabBlock);
	DenoraXMLConfigBlockCreate("usermodes", DenoraParseProto_UserModeBlock);
	DenoraXMLConfigBlockCreate("chanmodes", DenoraParseProto_ChannelModeBlock);
	DenoraXMLConfigBlockCreate("chanbanmodes", DenoraParseProto_ChannelBanModeBlock);
	DenoraXMLConfigBlockCreate("channelfeatures", DenoraParseProto_ChannelFeaturesBlock);
	DenoraXMLConfigBlockCreate("chanusermode", DenoraParseProto_ChannelUserModeBlock);


	snprintf(buf, sizeof(buf), "ircdconfig/%s", file);
	/* Read configuration file; exit if there are problems. */
	if (!DenoraParseXMLConfig(buf))
	{
		printf("Error while parsing %s\n", buf);
		exit(-1);
	}
}


void DenoraConfigInit(void)
{
	DenoraXMLConfigBlockCreate("connect", DenoraParseConnectBlock);
	DenoraXMLConfigBlockCreate("identity", DenoraParseIdentityBlock);
	DenoraXMLConfigBlockCreate("statserv", DenoraParseStatServBlock);
	DenoraXMLConfigBlockCreate("filenames", DenoraParseFileNamesBlock);
	DenoraXMLConfigBlockCreate("backup", DenoraParseBackUpBlock);
	DenoraXMLConfigBlockCreate("netinfo", DenoraParseNetInfoBlock);
	DenoraXMLConfigBlockCreate("timeout", DenoraParseTimeOutBlock);
	DenoraXMLConfigBlockCreate("options", DenoraParseOptionBlock);
	DenoraXMLConfigBlockCreate("admin", DenoraParseAdminBlock);
	DenoraXMLConfigBlockCreate("sql", DenoraParseSQLBlock);
	DenoraXMLConfigBlockCreate("sqltables", DenoraParseSQLTableBlock);
	DenoraXMLConfigBlockCreate("modules", DenoraParseModuleBlock);
	DenoraXMLConfigBlockCreate("xmlrcp", DenoraParseXMLRPCBlock);

}

void denoraxmlcheck(int ac, char **av)
{
	//todo
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return sstrdup(str);

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return sstrdup(str);
}

void DenoraXMLDebug(const char *fmt, ...)
{
	va_list args;
	char str[BUFSIZE];
	*str = '\0';

	va_start(args, fmt);
	ircvsnprintf(str, sizeof(str), fmt, args);
	va_end(args);

	if (denora->debug)
	{
		alog(LOG_DEBUG, "%s", str);
	}
	fprintf(stderr, "%s\n", str);
}

int DenoraParseXMLConfig(char *filename)
{
	FILE * fp;
	char * line = NULL;
	char *rawline = NULL;
	char *newline;
	char *linenew;
        size_t len = 0;
        ssize_t myread;
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
	{
		printf("file pointer is null\n");
        return 0;
	}

	while ((myread = getline(&rawline, &len, fp)) != -1) 
	{
		strnrepl(rawline, BUFSIZE, "\n", "");
		strnrepl(rawline, BUFSIZE, "\t", "");
		line = trimwhitespace(rawline);

//		printf("Line: %s\n", line);

		/* skip if its a #  and c++ style */
		if (*line == '#' || *line == '/' && line[1] == '/')
		{
			free(line);
			continue;
		}
		/* xml <! -!> tags are ignored */
		if (*line == '<' && line[1] == '!' && line[2] == '-')
		{
			skipuntil = 1;
			free(line);
		        continue;

	   	}
		if ((*line == '-' && line[1] == '!' && line[2] == '>') || (*line == '-' && line[1] == '-' && line[2] == '!' && line[3] == '>')) 
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
				if (endblock && startblock)
				{
					tag = GetConfigEndTagName(rawline);
					c = DenoraXMLConfigFindBlock(tag);
					startblock = 0;
					if (c)
					{
						res = c->parser(xmldata);
						if (res == -1)
						{
							DenoraXMLDebug("Failed to parse Config File Correctly");
							DenoraXMLDebug("Parser for %s returned %d", c->name, res);
							return 0;
						}
					}
					free(tag);

					free(line);
					continue;
				}
				else
				{
					xmldata = DenoraCallocArray(100);
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
	alog(LOG_DEBUG, "[%d][%s]", errno,  strerror(errno));

	return 1;
}



