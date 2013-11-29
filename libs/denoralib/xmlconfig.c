#include "denoralib.h"

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

	exit(EXIT_SUCCESS);
}