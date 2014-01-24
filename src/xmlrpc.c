
/* XML RPC
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

XMLRPCCmdHash *XMLRPCCMD[MAX_CMD_HASH];

XMLRPCSet xmlrpc;
int xmlrpc_error_code = 0;
static XMLRPCCmdHash *current;
static int next_index;
char *xmlrpc_user_struct(User * u);

/*************************************************************************/

/**
 * Allow modules to get the last xmlrpc error code
 *
 * @return The xmlrpc error code
 *
 */
int xmlrpc_getlast_error(void)
{
	return (!xmlrpc_error_code ? 0 : xmlrpc_error_code);
}

/*************************************************************************/

/**
 * Main XMLRPC Processing function that handles all the commands
 *
 * @param socket_fd is the socket that was opened for XMLRPC
 * @param buffer is the data gotten from the socket
 * @return void - no returend value
 *
 */
void xmlrpc_process(deno_socket_t socket_fd, char *buffer)
{
	int retVal = 0;
	XMLRPCCmd *xcurrent = NULL;
	XMLRPCCmd *xml;
	char *tmp;
	int ac = 0;
	char **av;
	char *name = NULL;

	SET_SEGV_LOCATION();

	tmp = xmlrpc_parse(buffer);
	if (tmp)
	{
		name = xmlrpc_method(tmp);
		if (name)
		{
			xml = findXMLCommand(XMLRPCCMD, name);
			if (xml)
			{
				ac = xmlrpc_split_buf(tmp, &av);
				if (ac >= 1)
				{
					if (xml->func)
					{
						retVal = xml->func(socket_fd, ac, av);
						if (retVal == MOD_CONT)
						{
							xcurrent = xml->next;
							while (xcurrent && xcurrent->func
							        && retVal == MOD_CONT)
							{
								retVal = xcurrent->func(socket_fd, ac, av);
								xcurrent = xcurrent->next;
							}
						}
						else
						{
							xmlrpc_error_code = -7;
							xmlrpc_generic_error(socket_fd,
							                     xmlrpc_error_code,
							                     "XMLRPC error: First eligible function returned XMLRPC_STOP");
						}
					}
					else
					{
						xmlrpc_error_code = -6;
						xmlrpc_generic_error(socket_fd, xmlrpc_error_code,
						                     "XMLRPC error: Method [%s] has no registered function",
						                     name);
					}
				}
				else
				{
					xmlrpc_error_code = -5;
					xmlrpc_generic_error(socket_fd, xmlrpc_error_code,
					                     "XMLRPC error: No arguments provided");
				}
			}
			else
			{
				xmlrpc_error_code = -4;
				xmlrpc_generic_error(socket_fd, xmlrpc_error_code,
				                     "XMLRPC error: Unknown routine called [%s]",
				                     name);
			}
			if (name)
			{
				free(name);
			}
		}
		else
		{
			xmlrpc_error_code = -3;
			xmlrpc_generic_error(socket_fd, xmlrpc_error_code,
			                     "XMLRPC error: Missing methodRequest or methodName.");
		}
		if (tmp)
		{
			free(tmp);
		}
	}
	else
	{
		xmlrpc_error_code = -2;
		xmlrpc_generic_error(socket_fd, xmlrpc_error_code,
		                     "XMLRPC error: Invalid document end at line 1");
	}
	SET_SEGV_LOCATION();
}

/*************************************************************************/

/**
 * Create a XMLRPC command
 *
 * @param name of the xmlrpc method
 * @param func Function pointer, which should be a function that takes socket id, param count and char array
 * @return XMLRPCCmd structore
 *
 */
XMLRPCCmd *createXMLCommand(const char *name,
                            int (*func) (deno_socket_t fd, int ac,
                                    char **av))
{
	XMLRPCCmd *xml = NULL;
	if (!func)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	if ((xml = malloc(sizeof(XMLRPCCmd))) == NULL)
	{
		fatal("Out of memory!");
	}
	xml->name = sstrdup(name);
	xml->func = func;
	xml->next = NULL;
	xml->mod_name = NULL;
	return xml;
}

/*************************************************************************/

/**
 * Locate the XMLRPC function
 *
 * @param xmlrpctable Table containing the XMLRPC structores
 * @param name is the method name to locate
 * @return XMLRPCCmd structore
 *
 */
XMLRPCCmd *findXMLCommand(XMLRPCCmdHash * xmlrpctable[], const char *name)
{
	int idx;
	XMLRPCCmdHash *xcurrent = NULL;
	if (!xmlrpctable || !name)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	idx = CMD_HASH(name);

	for (xcurrent = xmlrpctable[idx]; xcurrent; xcurrent = xcurrent->next)
	{
		if (stricmp(name, xcurrent->name) == 0)
		{
			return xcurrent->xml;
		}
	}
	return NULL;
}

/*************************************************************************/

/**
 * Add XMLRPC function to the xmlrpc command table
 *
 * Possible return codes are
 * - MOD_ERR_PARAMS : parameters were NULL
 * - MOD_ERR_OK     : everything went okay
 *
 * @param xmlrpctable Table containing the XMLRPC structores
 * @param xml structore to add to the table
 * @return int result code.
 *
 */
int addXMLCommand(XMLRPCCmdHash * xmlrpctable[], XMLRPCCmd * xml)
{
	/* We can assume both param's have been checked by this point.. */
	int idx = 0;
	XMLRPCCmdHash *xcurrent = NULL;
	XMLRPCCmdHash *newHash = NULL;
	XMLRPCCmdHash *lastHash = NULL;

	if (!xmlrpctable || !xml)
	{
		return MOD_ERR_PARAMS;
	}
	SET_SEGV_LOCATION();

	idx = CMD_HASH(xml->name);

	for (xcurrent = xmlrpctable[idx]; xcurrent; xcurrent = xcurrent->next)
	{
		if (stricmp(xml->name, xcurrent->name) == 0)
		{
			xml->next = xcurrent->xml;
			xcurrent->xml = xml;
			alog(LOG_DEBUG,
			     "debug: existing XMLRPC: (0x%p), new msg (0x%p) (%s)",
			     (void *) xml->next, (void *) xml, xml->name);
			return MOD_ERR_OK;
		}
		lastHash = xcurrent;
	}

	if ((newHash = malloc(sizeof(XMLRPCCmdHash))) == NULL)
	{
		fatal("Out of memory");
	}
	newHash->next = NULL;
	newHash->name = sstrdup(xml->name);
	newHash->xml = xml;
	SET_SEGV_LOCATION();

	if (lastHash == NULL)
		xmlrpctable[idx] = newHash;
	else
		lastHash->next = newHash;
	return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Destory a xmlrpc method, freeing its memory.
 *
 * Possible return codes are
 * - MOD_ERR_PARAMS : parameters were NULL
 * - MOD_ERR_OK     : everything went okay
 *
 * @param xml structore to add to the table
 * @return int result code.
 *
 */
int destroyXMLRPCCommand(XMLRPCCmd * xml)
{
	if (!xml)
	{
		return MOD_ERR_PARAMS;
	}
	SET_SEGV_LOCATION();
	if (xml->name)
	{
		free(xml->name);
	}
	xml->func = NULL;
	if (xml->mod_name)
	{
		free(xml->mod_name);
	}
	xml->next = NULL;
	free(xml);
	return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Delete a xmlrpc method, freeing its memory.
 *
 * Possible return codes are
 * - MOD_ERR_PARAMS  : parameters were NULL
 * - MOD_ERR_OK      : everything went okay
 * - MOD_ERR_NOEXIST : method not found
 *
 * @param xmlrpctable Table containing the XMLRPC structores
 * @param xml structore to add to the table
 * @param mod_name is the module name
 * @return int result code.
 *
 */
int delXMLRPCCommand(XMLRPCCmdHash * xmlrpctable[], XMLRPCCmd * xml,
                     char *mod_name)
{
	int idx = 0;
	XMLRPCCmdHash *xcurrent = NULL;
	XMLRPCCmdHash *lastHash = NULL;
	XMLRPCCmd *tail = NULL, *last = NULL;

	if (!xml || !xmlrpctable)
	{
		return MOD_ERR_PARAMS;
	}
	SET_SEGV_LOCATION();

	idx = CMD_HASH(xml->name);

	for (xcurrent = xmlrpctable[idx]; xcurrent; xcurrent = xcurrent->next)
	{
		if (stricmp(xml->name, xcurrent->name) == 0)
		{
			if (!lastHash)
			{
				tail = xcurrent->xml;
				if (tail->next)
				{
					while (tail)
					{
						if (mod_name && tail->mod_name
						        && (stricmp(mod_name, tail->mod_name) == 0))
						{
							if (last)
							{
								last->next = tail->next;
							}
							else
							{
								xcurrent->xml = tail->next;
							}
							return MOD_ERR_OK;
						}
						last = tail;
						tail = tail->next;
					}
				}
				else
				{
					xmlrpctable[idx] = xcurrent->next;
					if (xcurrent->name)
					{
						free(xcurrent->name);
					}
					return MOD_ERR_OK;
				}
			}
			else
			{
				tail = current->xml;
				if (tail->next)
				{
					while (tail)
					{
						if (mod_name && tail->mod_name
						        && (stricmp(mod_name, tail->mod_name) == 0))
						{
							if (last)
							{
								last->next = tail->next;
							}
							else
							{
								xcurrent->xml = tail->next;
							}
							return MOD_ERR_OK;
						}
						last = tail;
						tail = tail->next;
					}
				}
				else
				{
					lastHash->next = xcurrent->next;
					if (xcurrent->name)
					{
						free(xcurrent->name);
					}
					return MOD_ERR_OK;
				}
			}
		}
		lastHash = xcurrent;
	}
	SET_SEGV_LOCATION();

	return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Allow module to delete a xmlrpc method.
 *
 * Possible return codes are
 * - MOD_ERR_UNKNOWN : Unknown module name
 * - MOD_ERR_NOEXIST : method not found
 *
 * @param name of the method to remove
 * @return int result code.
 *
 */
int moduleXMLRPCDel(const char *name)
{
	XMLRPCCmd *xml;
	int status;

	if (!mod_current_module)
	{
		return MOD_ERR_UNKNOWN;
	}
	if (!name)
	{
		return MOD_ERR_PARAMS;
	}
	xml = findXMLCommand(XMLRPCCMD, name);
	if (!xml)
	{
		return MOD_ERR_NOEXIST;
	}
	SET_SEGV_LOCATION();

	status = delXMLRPCCommand(XMLRPCCMD, xml, mod_current_module->name);
	if (denora->debug)
	{
		displayXMLRPCFromHash(xml->name);
	}
	return status;
}

/*************************************************************************/

/**
 * Allow module to add a xmlrpc method.
 *
 * Possible return codes are
 * - MOD_ERR_UNKNOWN : Unknown module name
 * - MOD_ERR_PARAM   : Not enough parameters
 *
 * @param xml is the struct returned from createXMLCommand
 * @return int result code.
 *
 */
int moduleAddXMLRPCcmd(XMLRPCCmd * xml)
{
	int status;

	if (!xml)
	{
		return MOD_ERR_PARAMS;
	}
	SET_SEGV_LOCATION();

	if ((mod_current_module_name) && (!mod_current_module))
	{
		mod_current_module = findModule(mod_current_module_name);
	}

	if (!mod_current_module)
	{
		return MOD_ERR_UNKNOWN;
	}                           /* shouldnt happen */
	xml->core = 0;
	if (!xml->mod_name)
	{
		xml->mod_name = sstrdup(mod_current_module->name);
	}
	SET_SEGV_LOCATION();

	status = addXMLCommand(XMLRPCCMD, xml);
	if (denora->debug)
	{
		displayXMLRPCFromHash(xml->name);
	}
	return status;
}

/*************************************************************************/

/**
 * Allow core to add a xmlrpc method.
 *
 * Possible return codes are
 * - MOD_ERR_PARAM   : Not enough parameters
 *
 * @param xmlrpctable Table containing the XMLRPC structores
 * @param xml is the struct returned from createXMLCommand
 * @return int result code.
 *
 */
int addCoreXMLRPCCmd(XMLRPCCmdHash * xmlrpctable[], XMLRPCCmd * xml)
{
	if (!xmlrpctable || !xml)
	{
		return MOD_ERR_PARAMS;
	}
	SET_SEGV_LOCATION();

	xml->core = 1;
	return addXMLCommand(xmlrpctable, xml);
}

/*************************************************************************/

/**
 * Display information about the XMLRPC method from hash.
 *
 * @param name is the method to display
 * @return void - no returend value
 *
 */
void displayXMLRPCFromHash(char *name)
{
	XMLRPCCmdHash *xcurrent = NULL;
	int idx = 0;
	int found = 0;

	if (!name)
	{
		return;
	}

	idx = CMD_HASH(name);

	alog(LOG_EXTRADEBUG, langstr(ALOG_TRY_TO_DISPLAY), name);
	SET_SEGV_LOCATION();

	for (xcurrent = XMLRPCCMD[idx]; xcurrent; xcurrent = xcurrent->next)
	{
		if (stricmp(name, xcurrent->name) == 0)
		{
			displayXMLRPCcmd(xcurrent->xml);
			found = 1;
		}
	}
	if (!found)
	{
		alog(LOG_EXTRADEBUG, "%s was unable to find %s", PRETTY_FUNCTION,
		     name);
	}
	alog(LOG_EXTRADEBUG, langstr(ALOG_DONE_DISPLAY), name);
	return;
}

/*************************************************************************/

/**
 * Display information about the XMLRPC method from struct.
 *
 * @param xml struct to display data about
 * @return void - no returend value
 *
 */
void displayXMLRPCcmd(XMLRPCCmd * xml)
{
	XMLRPCCmd *msg = NULL;
	int i = 0;

	if (!xml)
	{
		return;
	}

	alog(LOG_EXTRADEBUG, "debug: Displaying message list for %s",
	     xml->name);
	for (msg = xml; msg; msg = msg->next)
	{
		alog(LOG_DEBUG, "%d: 0x%p", ++i, (void *) msg);
	}
	SET_SEGV_LOCATION();

	alog(LOG_EXTRADEBUG, "debug: end");
	return;
}

/*************************************************************************/

/**
 * Register a XMLRPC Method without need to create or add.
 *
 * @param name is the xmlrpc method name
 * @param func is the xmlrpc function
 * @return int result code.
 *
 */
int xmlrpc_register_method(const char *name,
                           int (*func) (deno_socket_t fd, int ac,
                                        char **av))
{
	XMLRPCCmd *xml;
	xml = createXMLCommand(name, func);
	return addXMLCommand(XMLRPCCMD, xml);
}

/*************************************************************************/

/**
 * Write out a valid xmlrpc header
 *
 * @param length is the size in bytes that the xmlrpc message is taking up
 * @return xmlrpc header, note this must be free'd
 *
 */
char *xmlrpc_write_header(int length)
{
	char buf[XMLRPC_BUFSIZE];
	time_t ts;
	char timebuf[64];
	struct tm tm;

	*buf = '\0';

	ts = time(NULL);
#ifdef _WIN32
	localtime_s(&tm, &ts);
#else
	tm = *localtime(&ts);
#endif
	strftime(timebuf, XMLRPC_BUFSIZE - 1, "%Y-%m-%d %H:%M:%S", &tm);

	ircsnprintf(buf, XMLRPC_BUFSIZE,
	            "HTTP/1.1 200 OK\n\rConnection: close\n\r"
	            "Content-Length: %d\n\r" "Content-Type: text/xml\n\r"
	            "Date: %s\n\r" "Server: Denora %s\r\n\r\n", length,
	            timebuf, denora->version);
	return sstrdup(buf);
}

/*************************************************************************/

/**
 * Parse Data down to just the xml without any special characters
 *
 * @param buffer Incoming data buffer
 * @return cleaned up buffer
 */
char *xmlrpc_parse(char *buffer)
{
	char *tmp = NULL;

	/*
	   Okay since the buffer could contain
	   HTTP header information, lets break
	   off at the point that the <?xml?> starts
	 */
	tmp = strstr(buffer, "<?xml");

	/* check if its xml doc */
	if (tmp)
	{
		/* get all the odd characters out of the data */
		return normalizeBuffer(tmp);
	}
	return NULL;
}

/*************************************************************************/

/**
 * Split the xmlrpc message into an array of data that we can pass to functions
 *
 * @param buffer Incoming data buffer
 * @param argv is the array into which we will store the data
 * @return count of array parameters
 */
int xmlrpc_split_buf(char *buffer, char ***argv)
{
	int ac = 0;
	int argvsize = 8;
	char *xdata, *str;
	char *str2 = NULL;
	char *nexttag = NULL;
	char *temp = NULL;
	char *final;
	int tagtype = 0;

	*argv = calloc(sizeof(char *) * argvsize, 1);
	while ((xdata = strstr(buffer, "<value>")))
	{
		if (xdata)
		{
			temp = myStrGetToken(xdata, '<', 2);
			if (temp)
			{
				nexttag = myStrGetToken(temp, '>', 0);
				if (nexttag)
				{
					/* strings */
					if (!stricmp("string", nexttag))
					{
						tagtype = 1;
					}
					else if (!stricmp("base64", nexttag))
					{
						tagtype = 2;
					}
					else
					{
						tagtype = 0;
					}
					free(nexttag);
				}
				free(temp);
			}
			str = myStrGetToken(xdata, '>', 2);
			str2 = myStrGetTokenRemainder(xdata, '>', 2);
			if (str)
			{
				final = myStrGetToken(str, '<', 0);
				if (final)
				{
					if (tagtype == 1)
					{
						(*argv)[ac++] = xmlrpc_decode_string(final);
					}
					else if (tagtype == 2)
					{
						(*argv)[ac++] = xmlrpc_decode64(final);
					}
					else
					{
						(*argv)[ac++] = final;
					}
					free(final);
				}
				free(str);
			}                   /* str */
		}                       /* data */
		buffer = str2;
	}                           /* while */
	if (str2)
	{
		free(str2);
	}
	return ac;
}

/*************************************************************************/

/**
 * Extract the methodname from the xmlrpc message
 *
 * @param buffer Incoming data buffer
 * @return method name
 */
char *xmlrpc_method(char *buffer)
{
	char *xdata;
	char *tmp, *tmp2;
	char *s;

	xdata = stristr(buffer, (char *) "<methodname>");
	if (xdata)
	{
		tmp = myStrGetToken(xdata, '>', 1);
		if (tmp)
		{
			tmp2 = myStrGetToken(tmp, '<', 0);
			free(tmp);
			if (tmp2)
			{
				s = sstrdup(tmp2);
				free(tmp2);
				return s;
			}
		}
	}
	return NULL;
}

/*************************************************************************/

/**
 * Print out a generic xmlrpc error message back to the socket. This is a
 * simple way to write a correct and valid xmlrpc message
 *
 * @param socket_fd is the socket to send to
 * @param code is the error code to send back
 * @param string allows you to define the error in printf() format
 * @param ... mixed args can follow
 * @return void - no returend value
 */
void xmlrpc_generic_error(deno_socket_t socket_fd, int code,
                          const char *string, ...)
{
	char buf[XMLRPC_BUFSIZE], buf2[XMLRPC_BUFSIZE];
	char fbuf[XMLRPC_BUFSIZE];
	char *header;
	char *encoded;
	int len;
	va_list va;

	va_start(va, string);
	ircvsnprintf(fbuf, sizeof(fbuf), string, va);
	va_end(va);

	encoded = char_encode(fbuf);

	ircsnprintf(buf, XMLRPC_BUFSIZE,
	            "<?xml version=\"1.0\"?>\r\n <methodResponse>\n\r  <fault>\n\r   <value>\n\r    <struct>\n\r     <member>\n\r      <name>faultCode</name>\n\r      <value><int>%d</int></value>\n\r     </member>\n\r     <member>\n\r      <name>faultString</name>\n\r      <value><string>%s</string></value>\n\r     </member>\n\r    </struct>\n\r   </value>\r\n  </fault>\r\n </methodResponse>",
	            code, encoded);

	free(encoded);

	len = strlen(buf);

	if (xmlrpc.httpheader)
	{
		header = xmlrpc_write_header(len);
		strlcpy(buf2, header, XMLRPC_BUFSIZE);
		strlcat(buf2, buf, XMLRPC_BUFSIZE);
		len += strlen(header);
		free(header);
		alog(LOG_DEBUG, "XMLRPC Send: %s", buf2);
		buffered_write(socket_fd, buf2, len);
	}
	else
	{
		alog(LOG_DEBUG, "XMLRPC Send: %s", buf);
		buffered_write(socket_fd, buf, len);
	}

	xml_encode = 0;
}

/*************************************************************************/

/**
 * Allow module to set various xmlrpc options
 *
 * valid types are
 * XMLRPC_HTTP_HEADER
 * XMLRPC_ENCODE
 * XMLRPC_INTTAG
 *
 * XMLRPC_HTTP_HEADER can either be set to XMLRPC_ON, XMLRPC_OFF
 * XMLRPC_INTTAG can either be set ot XMLRPC_I4, XMLRPC_INT
 *
 * @param type see comments on how these are defined
 * @param value that the option will be set to
 * @return interget 1 if type was known and 0 if unknown
 *
 */
int xmlrpc_set_options(int type, const char *value)
{
	if (type == XMLRPC_HTTP_HEADER)
	{
		if (!stricmp(value, XMLRPC_ON))
		{
			xmlrpc.httpheader = 1;
		}
		if (!stricmp(value, XMLRPC_OFF))
		{
			xmlrpc.httpheader = 0;
		}
		return 1;
	}
	else if (type == XMLRPC_ENCODE)
	{
		if (value)
		{
			xmlrpc.encode = sstrdup(value);
			return 1;
		}
	}
	else if (type == XMLRPC_INTTAG)
	{
		if (!stricmp(value, XMLRPC_I4))
		{
			xmlrpc.inttagstart = sstrdup("<i4>");
			xmlrpc.inttagend = sstrdup("</i4>");
			return 1;
		}
		if (!stricmp(value, XMLRPC_INT))
		{
			xmlrpc.inttagstart = sstrdup("<int>");
			xmlrpc.inttagend = sstrdup("</int>");
			return 1;
		}
	}
	else
	{
		alog(LOG_DEBUG, "xmlrpc_set_options called with unknown type %d",
		     type);
	}
	return 0;
}

/*************************************************************************/

/**
 * Send back to a socket the xmlrpc data
 *
 * @param socket_fd is the socket to send to
 * @param argc is the argument count that will follow
 * @param ... mixed args can follow
 * @return void - no returend value
 */
void xmlrpc_send(deno_socket_t socket_fd, int argc, ...)
{
	va_list va;
	char *a;
	int idx = 0;
	char *s = NULL;
	int len;
	char buf[XMLRPC_BUFSIZE];
	char buf2[XMLRPC_BUFSIZE];
	char *header;

	*buf = '\0';
	*buf2 = '\0';

	va_start(va, argc);
	for (idx = 0; idx < argc; idx++)
	{
		a = va_arg(va, char *);
		if (!s)
		{
			ircsnprintf(buf, XMLRPC_BUFSIZE,
			            " <param>\r\n  <value>\r\n   %s\r\n  </value>\r\n </param>",
			            a);
			s = sstrdup(buf);
		}
		else
		{
			ircsnprintf(buf, XMLRPC_BUFSIZE,
			            "%s\r\n <param>\r\n  <value>\r\n   %s\r\n  </value>\r\n </param>",
			            s, a);
			free(s);
			s = sstrdup(buf);
		}
	}
	va_end(va);

	if (xmlrpc.encode)
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE,
		            "<?xml version=\"1.0\" encoding=\"%s\" ?>\r\n<methodResponse>\r\n<params>\r\n%s\r\n</params>\r\n</methodResponse>",
		            xmlrpc.encode, s);
	}
	else
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE,
		            "<?xml version=\"1.0\"?>\r\n<methodResponse>\r\n<params>\r\n%s\r\n</params>\r\n</methodResponse>",
		            s);
	}
	len = strlen(buf);

	if (xmlrpc.httpheader)
	{
		header = xmlrpc_write_header(len);
		ircsnprintf(buf2, XMLRPC_BUFSIZE, "%s%s", header, buf);
		free(header);
		len = strlen(buf2);
		buffered_write(socket_fd, buf2, len);
		xmlrpc.httpheader = 1;
	}
	else
	{
		buffered_write(socket_fd, buf, len);
	}
	if (xmlrpc.encode)
	{
		free(xmlrpc.encode);
		xmlrpc.encode = NULL;
	}
	if (s)
	{
		free(s);
	}
}

/*************************************************************************/

/**
 * Print out user struct for xmlrpc
 *
 * @param u is the struct to print
 * @return the struct in xmlrpc data format
 *
 */
char *xmlrpc_user_struct(User * u)
{
	char buf[BUFSIZE];
	*buf = '\0';

	ircsnprintf(buf, BUFSIZE,
	            "<struct>\r\n <member>\r\n  <name>nick</name>\r\r  <value><string>%s</string></value>\r\n </member>\r\n"
	            " <member>\r\n  <name>username</name>\r\n  <value><string>%s</string></value>\r\n </member>\n\r</struct>",
	            u->nick, u->username);
	return sstrdup(buf);
}

/*************************************************************************/

/**
 * Convert time into a valid xmlrpc <dateTime.iso8601>
 *
 * @param buf is the buffer we are going to copy the data into
 * @param t is the time_t struct of the time
 * @return cleaned up buffer
 *
 */
char *xmlrpc_time2date(char *buf, time_t t)
{
	char timebuf[XMLRPC_BUFSIZE];
	struct tm tm;

	*buf = '\0';
	if (t)
	{
#ifdef _WIN32
		localtime_s(&tm, &t);
#else
		tm = *localtime(&t);
#endif
		/* <dateTime.iso8601>20011003T08:53:38</dateTime.iso8601> */
		strftime(timebuf, XMLRPC_BUFSIZE - 1, XMLRPC_DATETIME_FORMAT, &tm);
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%s</%s>",
		            XMLRPC_DATETIME_TAG, timebuf, XMLRPC_DATETIME_TAG);
	}
	else
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s/>", XMLRPC_DATETIME_TAG);
	}
	return buf;
}

/*************************************************************************/

/**
 * Convert integer into a xmlrpc tag data form an integer
 *
 * @param buf is the buffer we are going to copy the data into
 * @param value is the integer to write out
 * @return tag buffer data
 *
 */
char *xmlrpc_integer(char *buf, int value)
{
	*buf = '\0';

	if (!xmlrpc.inttagstart || !xmlrpc.inttagend)
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%d</%s>", XMLRPC_I4_TAG,
		            value, XMLRPC_I4_TAG);
	}
	else
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE, "%s%d%s", xmlrpc.inttagstart,
		            value, xmlrpc.inttagend);
		if (xmlrpc.inttagend)
		{
			free(xmlrpc.inttagend);
			xmlrpc.inttagend = NULL;
		}
		if (xmlrpc.inttagstart)
		{
			free(xmlrpc.inttagstart);
			xmlrpc.inttagstart = NULL;
		}
	}
	return buf;
}

/*************************************************************************/

/**
 * Convert string into a xmlrpc tag data form an string
 *
 * @param buf is the buffer we are going to copy the data into
 * @param value is the string to write out
 * @return tag buffer data
 *
 */
char *xmlrpc_string(char *buf, char *value)
{
	char *encoded;
	*buf = '\0';

	if (value)
	{
		encoded = char_encode(value);
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%s</%s>", XMLRPC_STRING_TAG,
		            encoded, XMLRPC_STRING_TAG);
		free(encoded);
	}
	else
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s/>", XMLRPC_STRING_TAG);
	}
	return buf;
}

/*************************************************************************/

/**
 * Convert boolean into a xmlrpc tag data form an boolean
 *
 * @param buf is the buffer we are going to copy the data into
 * @param value is the boolean to write out
 * @return tag buffer data
 *
 */
char *xmlrpc_boolean(char *buf, int value)
{
	*buf = '\0';
	ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%d</%s>", XMLRPC_BOOLEAN_TAG,
	            (value ? 1 : 0), XMLRPC_BOOLEAN_TAG);
	return buf;
}

/*************************************************************************/

/**
 * Convert string into a xmlrpc tag data form an base64 string
 *
 * @param buf is the buffer we are going to copy the data into
 * @param value is the string to write out as base64
 * @return tag buffer data
 *
 */
char *xmlrpc_base64(char *buf, char *value)
{
	struct buffer_st b64buf;

	if (value)
	{
		base64_encode(&b64buf, value, strlen(value));
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%s</%s>", XMLRPC_BASE64_TAG,
		            b64buf.data, XMLRPC_BASE64_TAG);
		buffer_delete(&b64buf);
	}
	else
	{
		ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s/>", XMLRPC_BASE64_TAG);
	}
	return buf;
}

/*************************************************************************/

/**
 * Decode a xmlrpc string
 *
 * @param value to decode
 * @return decoded data - this needs to be free'd
 *
 */
char *xmlrpc_decode64(char *value)
{
	char *retval;
	struct buffer_st buf;
	base64_decode(&buf, value);
	retval = sstrdup(buf.data);
	buffer_delete(&buf);
	return retval;
}

/*************************************************************************/

/**
 * Convert double into a xmlrpc tag data form an double
 *
 * @param buf is the buffer we are going to copy the data into
 * @param value is the string to write out as double
 * @return tag buffer data
 *
 */
char *xmlrpc_double(char *buf, double value)
{
	*buf = '\0';
	ircsnprintf(buf, XMLRPC_BUFSIZE, "<%s>%g</%s>", XMLRPC_DOUBLE_TAG,
	            value, XMLRPC_DOUBLE_TAG);
	return buf;
}

/*************************************************************************/

/**
 * Convert numerous string into a xmlrpc array format
 *
 * @param argc are the number of arguments
 * @param ... any number of arguments
 *
 * @return array tagged data
 *
 */
char *xmlrpc_array(int argc, ...)
{
	va_list va;
	char *a;
	int idx = 0;
	char *s = NULL;
	int len;
	char buf[XMLRPC_BUFSIZE];

	va_start(va, argc);
	for (idx = 0; idx < argc; idx++)
	{
		a = va_arg(va, char *);
		if (!s)
		{
			ircsnprintf(buf, XMLRPC_BUFSIZE, "   <value>%s</value>", a);
			s = sstrdup(buf);
		}
		else
		{
			ircsnprintf(buf, XMLRPC_BUFSIZE,
			            "%s\r\n     <value>%s</value>", s, a);
			free(s);
			s = sstrdup(buf);
		}
	}
	va_end(va);

	ircsnprintf(buf, XMLRPC_BUFSIZE,
	            "<array>\r\n    <data>\r\n  %s\r\n    </data>\r\n   </array>",
	            s);
	len = strlen(buf);
	if (s)
	{
		free(s);
	}
	return sstrdup(buf);
}

/*************************************************************************/

/**
 * Iterate over all xmlrpc commands.  Return NULL at end of list.
 *
 * @return XMLRPC Command struct
 *
 */
XMLRPCCmd *first_xmlrpccmd(void)
{
	next_index = 0;

	SET_SEGV_LOCATION();

	while (next_index < 1024 && current == NULL)
		current = XMLRPCCMD[next_index++];
	if (current)
	{
		return current->xml;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

XMLRPCCmd *next_xmlrpccmd(void)
{
	SET_SEGV_LOCATION();

	if (current)
		current = current->next;
	if (!current && next_index < 1024)
	{
		while (next_index < 1024 && current == NULL)
			current = XMLRPCCMD[next_index++];
	}
	if (current)
	{
		return current->xml;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

/**
 * Iterate over all xmlrpc hash.  Return NULL at end of list.
 *
 * @return XMLRPC Command Hash struct
 *
 */
XMLRPCCmdHash *first_xmlrpchash(void)
{
	next_index = 0;

	SET_SEGV_LOCATION();

	while (next_index < 1024 && current == NULL)
		current = XMLRPCCMD[next_index++];
	return current;
}

/*************************************************************************/

XMLRPCCmdHash *next_xmlrpchash(void)
{
	SET_SEGV_LOCATION();

	if (current)
		current = current->next;
	if (!current && next_index < 1024)
	{
		while (next_index < 1024 && current == NULL)
			current = XMLRPCCMD[next_index++];
	}
	if (current)
	{
		return current;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

/**
 * Delete entry for XMLRPC in hash
 *
 * @param xh is the xmlrpc hash struct to free
 *
 * @return MOD_ERR_OK or MOD_ERR_PARAMS
 *
 */
int destroyxmlrpchash(XMLRPCCmdHash * xh)
{
	SET_SEGV_LOCATION();

	if (!xh)
	{
		return MOD_ERR_PARAMS;
	}
	if (xh->name)
	{
		free(xh->name);
	}
	xh->xml = NULL;
	xh->next = NULL;
	free(xh);
	return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Decode a string from HTML formatting to readable format
 *
 * @param buf is the html string to convert
 *
 * @return decoded string
 *
 */
char *xmlrpc_decode_string(char *buf)
{
	int count;
	int i;
	char *token, *temp;
	char *temptoken;
	char buf2[12];
	char buf3[12];

	strnrepl(buf, XMLRPC_BUFSIZE, "&gt;", ">");
	strnrepl(buf, XMLRPC_BUFSIZE, "&lt;", "<");
	strnrepl(buf, XMLRPC_BUFSIZE, "&quot;", "\"");
	strnrepl(buf, XMLRPC_BUFSIZE, "&amp;", "&");

	temp = sstrdup(buf);
	count = myNumToken(temp, '&');
	for (i = 1; i <= count; i++)
	{
		token = myStrGetToken(temp, '&', i);
		if (token && *token == '#')
		{
			temptoken = strtok((token + 1), ";");
			ircsnprintf(buf2, 12, "%c", atoi(temptoken));
			ircsnprintf(buf3, 12, "&%s;", token);
			strnrepl(buf, XMLRPC_BUFSIZE, buf3, buf2);
			free(token);
		}
	}
	free(temp);
	return buf;
}

/*************************************************************************/
