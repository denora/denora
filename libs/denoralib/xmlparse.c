#include "denoralib.h"

char *GetConfigStartTagName(char *line)
{
	char *linedata;
	linedata = myStrGetToken(line, '<', 1);
	linedata = myStrGetToken(linedata, '>', 0);
	return linedata;
}


char *GetConfigEndTagName(char *line)
{
	char *linedata;
	linedata = myStrGetToken(line, '<', 1);
	linedata = myStrGetToken(linedata, '>', 0);
	linedata = myStrGetToken(linedata, '/', 1);
	return linedata;
}

char *GetOptionTagData(char *line)
{
	char *linedata;
	linedata = myStrGetToken(line, '>', 1);
	linedata = myStrGetToken(linedata, '<', 0);

	return linedata;
}

char *GetOptionTagName(char *line)
{
	char *linedata;
	linedata = myStrGetToken(line, '<', 1);
	linedata = myStrGetToken(linedata, '>', 0);
	return linedata;
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
	int x = 0;
	char **xmldata;

        fp = FileOpen(filename, "r");
        if (fp == NULL)
           return;

	while ((read = getline(&rawline, &len, fp)) != -1) 
	{
		line = strdup(rawline);
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
						c->parser(c->numoptions, xmldata);
						free(xmldata);
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
					xmldata[x++] = strdup(rawline);
				}			
	   		}
		}
       }
}



