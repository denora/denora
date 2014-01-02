/* XMLRPC Channel List
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
#define MODULE_NAME "xmlrpc_channelslist"

int xmlrpc_channellist(deno_socket_t xmlsocket, int ac, char **av);
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
		alog(LOG_NORMAL,   "[%s] XMLRPC not enabled unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	xml = createXMLCommand("denora.channellist", xmlrpc_channellist);
	status = moduleAddXMLRPCcmd(xml);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting xmlrpc message for denora.channellist [%d][%s]", status,
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

int xmlrpc_channellist(deno_socket_t xmlsocket, int ac, char **av)
{
	char buf[BUFSIZE];
	Channel *c, *next;
	char *s = NULL;

	*buf = '\0';

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, ac, av);
	}
	c = firstchan();
	while (c)
	{
		next = nextchan();
		if (SP_HTML)
		{
			if (ChanHasMode(c->name, CMODE_s)
			        || ChanHasMode(c->name, CMODE_p))
			{
				c = next;
				continue;
			}
		}
		if (s)
		{
			ircsnprintf(buf, BUFSIZE,
			            "%s <value><string>%s</string></value>\n\r", s,
			            c->name);
			free(s);
		}
		else
		{
			ircsnprintf(buf, BUFSIZE,
			            "<value><string>%s</string></value>\n\r", c->name);
		}
		s = sstrdup(buf);
		c = next;
	}
	ircsnprintf(buf, BUFSIZE,
	            "<array>\r\n <data>\r\n  %s\r\n </data>\r\n</array>", s);
	xmlrpc_send(xmlsocket, 1, buf);
	return MOD_CONT;
}
