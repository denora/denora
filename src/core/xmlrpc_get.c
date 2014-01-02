/* StatServ core functions
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
#define MODULE_NAME "xmlrpc_get"

int xmlrpc_getstat(deno_socket_t xmlsocket, int ac, char **av);
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

	xml = createXMLCommand("denora.getstat", xmlrpc_getstat);
	status = moduleAddXMLRPCcmd(xml);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting xmlrpc message for denora.getstat [%d][%s]", status,
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

int xmlrpc_getstat(deno_socket_t xmlsocket, int ac, char **av)
{
	char buf[BUFSIZE];
	*buf = '\0';

	if (denora->debug >= 2)
	{
		protocol_debug(NULL, ac, av);
	}

	if (!stricmp(av[0], "users"))
	{
		ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->users);
	}
	else if (!stricmp(av[0], "chans"))
	{
		ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->chans);
	}
	else
	{
		ircsnprintf(buf, BUFSIZE, "<i4>%ld</i4>", (long int) stats->users);
	}
	xmlrpc_send(xmlsocket, 1, buf);
	return MOD_CONT;
}
