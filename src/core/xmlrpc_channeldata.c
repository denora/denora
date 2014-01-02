/* XMLRPC Channel Data
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
#define MODULE_NAME "xmlrpc_channeldata"

int xmlrpc_channeldata(deno_socket_t xmlsocket, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
	XMLRPCCmd *xml;
	int status;

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (!XMLRPC_Enable)
	{
		alog(LOG_NORMAL,   "[%s] XMLRPC not enabled unloading module", MODULE_NAME, MODULE_VERSION);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	xml = createXMLCommand("denora.channeldata", xmlrpc_channeldata);
	status = moduleAddXMLRPCcmd(xml);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting xmlrpc message for denora.channeldata [%d][%s]", status,
		     ModuleGetErrStr(status));
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

/*
   av[0] = channel name
   av[1] = data to get

   topic, usercount, maxusercount, modes

*/
int xmlrpc_channeldata(deno_socket_t xmlsocket, int ac, char **av)
{
	char buf[BUFSIZE];
	Channel *c;
	char *temp;

	*buf = '\0';

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, ac, av);
	}

	c = findchan(av[0]);
	if (c)
	{
		if (ac >= 2)
		{
			if (!stricmp(av[1], "topic"))
			{
				temp = char_encode(c->topic);
				ircsnprintf(buf, BUFSIZE, "<string>%s</string>", temp);
				free(temp);
				xmlrpc_send(xmlsocket, 1, buf);
			}
			else if (!stricmp(av[1], "usercount"))
			{
				ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>",
				            (long int) c->stats->usercount);
				xmlrpc_send(xmlsocket, 1, buf);
			}
			else if (!stricmp(av[1], "maxusercount"))
			{
				ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>",
				            (long int) c->stats->maxusercount);
				xmlrpc_send(xmlsocket, 1, buf);
			}
			else if (!stricmp(av[1], "modes"))
			{
				temp = chan_get_modes(c, 1);
				xmlrpc_string(buf, temp);
				free(temp);
				xmlrpc_send(xmlsocket, 1, buf);
			}
			else
			{
				xmlrpc_generic_error(xmlsocket, 2, "To few arguments");
			}
		}
		else
		{
			xmlrpc_generic_error(xmlsocket, 2, "To few arguments");
		}
	}
	else
	{
		xmlrpc_generic_error(xmlsocket, 1, "No such channel");
	}
	return MOD_CONT;
}
