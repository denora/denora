/* Null Route some Messages
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
/*************************************************************************/

#include "denora.h"

#define MODULE_VERSION "2.0"
#define MODULE_NAME "msg_null"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
	Message *m;
	int status;

	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);

	m = createMessage("005", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 005 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("020", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 020 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("105", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 105 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("216", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 216 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("217", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 217 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("219", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 219 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	/* /stats k */
	m = createMessage("223", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 223 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("244", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 244 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("249", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 249 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("250", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 250 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("263", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 263 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("304", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 304 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("401", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 401 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("402", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 402 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("422", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 422 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("451", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 451 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("461", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 461 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("481", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 481 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("482", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 482 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("503", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 503 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	/* Ithildin Adds some replies */
	m = createMessage("771", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 771 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("773", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 773 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	m = createMessage("774", denora_event_null);
	status = addCoreMessage(IRCD, m);
	if (status != MOD_ERR_OK)
	{
		alog(LOG_NORMAL,
		     "Error Occurred setting message for 774 [%d][%s]", status,
		     ModuleGetErrStr(status));
	}

	return MOD_CONT;

}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

