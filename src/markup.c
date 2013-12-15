/* HTML and XML Markup code
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

HTMLHash *HTMLHANDLERS[MAX_CMD_HASH];
char *xml_encode;               /* xml encoding */
char *xml_doctype;              /* xml document type */
char *xml_header;               /* xml header data */

/*************************************************************************/

/**
 * Create a file pointer for xml file
 *
 * @param filename is the file name to open for writting
 * @return file pointer
 *
 */
FILE *new_xml(char *filename)
{
	FILE *xmlfile;

	

	if ((xmlfile = FileOpen(filename, FILE_WRITE)) == NULL)
	{
		alog(LOG_ERROR, "Error accessing file: %s", filename);
		return NULL;
	}
	return xmlfile;
}

/*************************************************************************/

/**
 * Write out valid XML header
 *
 * @param ptr is the file pointer to write to
 * @return void - no returend value
 *
 */
void xml_write_header(FILE * ptr)
{
	

	if (ptr)
	{
		if (xml_encode)
		{
			fprintf(ptr, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n\r",
			        xml_encode);
		}
		else
		{
			fprintf(ptr, "<?xml version=\"1.0\" ?>\n\r");
		}
		if (xml_doctype)
		{
			fprintf(ptr, "%s\n\r", xml_doctype);
		}
		
		if (xml_header)
		{
			fprintf(ptr, "<%s>\n\r", xml_header);
		}
		else
		{
			fprintf(ptr, "<denora>\n\r");
		}
	}
	
	xml_encode = 0;
}

/*************************************************************************/

/**
 * Write out valid XML footer
 *
 * @param ptr is the file pointer to write to
 * @return void - no returend value
 *
 */
void xml_write_footer(FILE * ptr)
{
	
	if (ptr)
	{
		if (xml_header)
		{
			fprintf(ptr, "</%s>\n\r", xml_header);
		}
		else
		{
			fprintf(ptr, "</denora>\n\r");
		}
		fclose(ptr);
	}
}

/*************************************************************************/

/**
 * Write out valid XML tag
 *
 * @param ptr is the file pointer to write to
 * @param tag is the xml tag for the data
 * @param data is the xml data
 * @return void - no returend value
 *
 */
void xml_write_tag(FILE * ptr, const char *tag, char *xdata)
{
	char *mydata;

	

	if (ptr && tag)
	{
		if (xdata)
		{
			mydata = char_encode(xdata);
			fprintf(ptr, "<%s>%s</%s>\n\r", tag, mydata, tag);
			free(mydata);
		}
		else
		{
			fprintf(ptr, "<%s />\n\r", tag);
		}
	}
}

/*************************************************************************/

/**
 * Write out XML for moduledata struct
 *
 * @param ptr is the file pointer to write to
 * @param md Module Data struct
 * @return void - no returend value
 *
 */
void xml_moduleData(FILE * ptr, ModuleData ** md)
{
	ModuleData *modcurrent = NULL;
	

	if (ptr)
	{
		if (md)
		{
			fprintf(ptr, "<moduledata>");
			for (modcurrent = *md; modcurrent;
			        modcurrent = modcurrent->next)
			{
				fprintf(ptr, "<%s>", modcurrent->moduleName);
				xml_write_tag(ptr, modcurrent->key, modcurrent->value);
				fprintf(ptr, "</%s>", modcurrent->moduleName);
			}
			fprintf(ptr, "</moduledata>");
		}
		else
		{
			fprintf(ptr, "<moduledata />");
		}
	}
	return;
}

/*************************************************************************/

/**
 * Buffer valid XML tag
 *
 * @param tag is the xml tag for the data
 * @param data is the xml data
 * @return tag this data must be free'd
 *
 */
char *xml_prepare_tag(char *tag, char *xdata)
{
	char *mydata;
	char buf[BUFSIZE];
	*buf = '\0';

	

	if (tag)
	{
		if (xdata)
		{
			mydata = char_encode(xdata);
			ircsnprintf(buf, BUFSIZE, "<%s>%s</%s>\n\r", tag, mydata, tag);
			free(mydata);
		}
		else
		{
			ircsnprintf(buf, BUFSIZE, "<%s />\n\r", tag);
		}
		return sstrdup(buf);
	}
	return sstrdup("");
}

/*************************************************************************/

/**
 * Write out a interger int xml tag format
 *
 * @param ptr is the file pointer
 * @param tag is the tag name to be writen
 * @param data is the data that will fill the block
 * @return void - no returend value
 *
 */
void xml_write_tag_int(FILE * ptr, const char *tag, long int xdata)
{
	
	if (ptr && tag)
	{
		fprintf(ptr, "<%s>%ld</%s>\n\r", tag, (long int) xdata, tag);
	}
}

/*************************************************************************/

/**
 * Write out a block of data starting with the block tag
 *
 * @param ptr is the file pointer
 * @param block is the block name to be writen
 * @param data is the data that will fill the block
 * @return void - no returend value
 *
 */
void xml_write_block(FILE * ptr, char *block, char *xdata)
{
	
	if (ptr && block)
	{
		if (xdata)
		{
			fprintf(ptr, "<%s>\n\r", block);
			fprintf(ptr, "%s", xdata);
			fprintf(ptr, "</%s>\n\r", block);
		}
		else
		{
			fprintf(ptr, "<%s />\n\r", block);
		}
	}
}

/*************************************************************************/

/**
 * Write out the starting tag for a block of data
 *
 * @param ptr is the file pointer
 * @param block is the block name to be writen
 * @return void - no returend value
 *
 */
void xml_write_block_top(FILE * ptr, const char *block)
{
	
	if (ptr && block)
	{
		fprintf(ptr, "<%s>\n\r", block);
	}
}

/*************************************************************************/

/**
 * Write out the closing tag for a block of data
 *
 * @param ptr is the file pointer
 * @param block is the block name to be writen
 * @return void - no returend value
 *
 */
void xml_write_block_bottom(FILE * ptr, const char *block)
{
	
	if (ptr && block)
	{
		fprintf(ptr, "</%s>\n\r", block);
	}
}


/*************************************************************************/

/**
 * Create a file pointer for html file
 *
 * @param filename is the file name to open for reading
 * @return file pointer
 *
 */
FILE *html_open(char *filename)
{
	FILE *htmlfile;
	

	htmlfile = FileOpen(filename, FILE_READ);
	if (htmlfile == NULL)
	{
		alog(LOG_ERROR, "Error accessing file: %s", filename);
		return NULL;
	}
	return htmlfile;
}

/*************************************************************************/

/**
 * Create a file pointer for html file
 *
 * @param filename is the file name to open for writting
 * @return file pointer
 *
 */
FILE *html_open_write(char *filename)
{
	FILE *htmlfile;

	

	if (filename)
	{
		htmlfile = FileOpen(filename, FILE_WRITE);
		if (htmlfile == NULL)
		{
			alog(LOG_ERROR, langstring(ALOG_HTML_ERR_WRITE), filename);
			return NULL;
		}
		return htmlfile;
	}
	return NULL;
}

/*************************************************************************/

/**
 * Close file pointer
 *
 * @param ptr is the file pointer to close
 * @return void - no returend value
 *
 */
void html_close(FILE * ptr)
{
	
	if (ptr)
	{
		fclose(ptr);
	}
}

/*************************************************************************/

HTMLTag *createHTMLtag(const char *word, void (*func) (FILE * ptr))
{
	HTMLTag *h;

	if (!word)
	{
		return NULL;
	}

	

	if ((h = malloc(sizeof(HTMLTag))) == NULL)
	{
		fatal("Out of memory!");
	}
	h->tag = sstrdup(word);     /* Our Name                 */
	h->handler = func;          /* Handle                   */
	return h;                   /* return a nice new module */
}

/*************************************************************************/

int addHTMLTag(HTMLTag * h)
{
	int modindex = 0;
	HTMLHash *hcurrent = NULL;
	HTMLHash *newHash = NULL;
	HTMLHash *lastHash = NULL;

	

	if (!h)
	{
		return MOD_ERR_PARAMS;
	}

	modindex = CMD_HASH(h->tag);

	for (hcurrent = HTMLHANDLERS[modindex]; hcurrent;
	        hcurrent = hcurrent->next)
	{
		if (stricmp(h->tag, hcurrent->tag) == 0)
			return MOD_ERR_EXISTS;
		lastHash = hcurrent;
	}

	if ((newHash = malloc(sizeof(HTMLHash))) == NULL)
	{
		fatal("Out of memory");
	}
	newHash->next = NULL;
	newHash->tag = sstrdup(h->tag);
	newHash->h = h;

	if (lastHash == NULL)
		HTMLHANDLERS[modindex] = newHash;
	else
		lastHash->next = newHash;
	return MOD_ERR_OK;
}

/*************************************************************************/

int destroyHTMLTag(HTMLTag * m)
{
	if (!m)
	{
		return MOD_ERR_PARAMS;
	}

	if (m->tag)
	{
		free(m->tag);
	}
	free(m);
	return MOD_ERR_OK;
}

/*************************************************************************/

int delHTMLTag(HTMLTag * m)
{
	int idx = 0;
	HTMLHash *hcurrent = NULL;
	HTMLHash *lastHash = NULL;

	if (!m)
	{
		return MOD_ERR_PARAMS;
	}

	idx = CMD_HASH(m->tag);

	for (hcurrent = HTMLHANDLERS[idx]; hcurrent; hcurrent = hcurrent->next)
	{
		if (stricmp(m->tag, hcurrent->tag) == 0)
		{
			if (!lastHash)
			{
				HTMLHANDLERS[idx] = hcurrent->next;
			}
			else
			{
				lastHash->next = hcurrent->next;
			}
			destroyHTMLTag(hcurrent->h);
			if (hcurrent->tag)
			{
				free(hcurrent->tag);
			}
			free(hcurrent);
			return MOD_ERR_OK;
		}
		lastHash = hcurrent;
	}
	return MOD_ERR_NOEXIST;
}

/*************************************************************************/

void do_html()
{
	char *buf, *buf1;
	char *bufold, *bufold1;
	char *html_template;
	const char *template;
	char startstr = 0;
	FILE *tpl, *opf;
	HTMLHash *hcurrent = NULL;
	int i, n;
#ifdef _WIN32
	char buffer[_MAX_PATH];
	char win32filename[MAXPATHLEN];

	/* Get the current working directory: */
	if (_getcwd(buffer, _MAX_PATH) == NULL)
	{
		alog(LOG_DEBUG, "debug: Unable to set Current working directory");
	}
#endif

	

#ifndef _WIN32
	template = STATS_DIR "/index.tpl";
#else
	ircsnprintf(win32filename, sizeof(win32filename), "%s\\%s", buffer,
	            "index.tpl");
	template = sstrdup(win32filename);
#endif
	html_template = sstrdup(template);

	if (!denora->do_html)
	{
		free(html_template);
		return;
	}

	if (denora->do_html)
	{
		if (!HTMLFilename)
		{
			alog(LOG_ERROR,
			     "Can't do HTML Writout as html path is not defined");
			free(html_template);
			return;
		}
	}
	else
	{
		free(html_template);
		return;
	}
	

	tpl = html_open(html_template);
	if (!tpl)
	{
		free(html_template);
		return;
	}
	opf = html_open_write(HTMLFilename);
	if (!opf)
	{
		denora->do_html = 0;
		free(html_template);
		return;
	}
	chmod(HTMLFilename, READ_PERM);
	buf = malloc(STARTBUFSIZE * 2);
	bufold = buf;
	buf1 = malloc(STARTBUFSIZE * 2);
	bufold1 = buf1;
	

	while (fgets(buf, STARTBUFSIZE, tpl))
	{

		for (i = 0; i < 1024; i++)
		{
			for (hcurrent = HTMLHANDLERS[i]; hcurrent;
			        hcurrent = hcurrent->next)
			{
				buf1 = strstr(buf, hcurrent->tag);
				if (buf1)
				{
					startstr = strlen(buf) - strlen(buf1);
					n = fwrite(buf, startstr, 1, opf);
					if (n < 0)
					{
						alog(LOG_DEBUG, "Error writting to HTML file");
					}
					hcurrent->h->handler(opf);
					
					buf = buf1 + strlen(hcurrent->tag);
				}
			}
		}
		fputs(buf, opf);
	}

	fclose(tpl);
	fclose(opf);
	
	if (bufold)
	{
		free(bufold);
	}
	if (bufold1)
	{
		free(bufold1);
	}
	if (html_template)
	{
		free(html_template);
	}
}
