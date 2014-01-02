/*
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
#define CONF_TABS

void flatconfig_error(int linenum, const char *message, ...);
int parse_directive(Directive * d, char *dir, int ac, char *av[MAXPARAMS],
                    int linenum, int reload, char *s);

/*************************************************************************/

/* notes on config.c
 * The parser works on two primary depths - blocks and tokens:
 *
 * block {
 *      token value;
 *      token "string value";
 *      token 123;               # int value
 *      token;                   # nonvar token
 * };
 *
 * It can also parse non-token blocks:
 *
 * block (
 *      "string string string";
 *      "string blah";
 * };
 *
 * Blocks are defined by tconftab (in confparse.h)
 * Tokens are defined by sconftab (^^^^^^^^^^^^^^)
 *
 * Each block must have a function that takes the values collected
 * and checks them against the requirements.  These functions are also
 * handy for getting variables out of the array that they are stored in.
 *
 * The array variables are placed in (an array of cVar structs) contains
 * all values for the the block, and the corrisponding sconftab item.
 */

/* these are our global lists of ACTIVE conf entries */


/* v2 globals */
char *DenoraDB; 							/* SQLite3 database file */
int DisableTLD;								/* Do not use the Top Level Domain feature in Denora */


/* v1.5  need to check and move this up after they are worked on correctly */


Conf_Modules *modules = NULL;
char *RemoteServer;
int RemotePort;
char *RemotePassword;
char *IRCDModule;
char *ENCModule;
char *SQLModule;
char *QuitPrefix;
char *ExcludeTable;
char *spamDB;

char *LocalHost;
int LocalPort;

int NumExcludeServers;
char **ExcludeServers;

char *ServerName;
char *ServerDesc;
char *ServiceUser;
char *ServiceHost;
static char *temp_userhost;

char *LogChannel;
char *NetworkName;
const char *HiddenPrefix;
char *HiddenSuffix;

char *s_StatServ;
char *desc_StatServ;

char *s_StatServ_alias;
char *desc_StatServ_alias;

char *PIDFilename;
char *MOTDFilename;
char *HTMLFilename;

int ReadTimeout;
int WarningTimeout;
int KeepLogs;
int NoLogs = 0;
char *ChanStatsTrigger;
int KeepBackups;
int DumpCore;
int UseTokens;
int StatsLanguage = 0;

int HideStatsO;
int HideUline;

int ModulesNumber;
int ModulesDelayedNumber;

char *ChannelDB;
char *ctcpDB;
char *ServerDB;
char *ChannelStatsDB;
char *excludeDB;
char *statsDB;
char *UserDB;

char *SqlHost;
char *SqlUser;
char *SqlPass;
char *SqlName;
int SqlPort;
char *SqlSock;
int SqlRetries = 0;
int SqlRetryGap = 0;
int sqltype = 0;
int DisableMySQLOPT = 0;

char *Numeric;
int LargeNet;
int UserStatsRegistered = 0;
int UserStatsExcludeBots = 0;

int UserCacheTime;
int UserCleanFreq;
int ServerCacheTime;
int ServerCleanFreq;
int UpdateTimeout;
int HTMLTimeout;
int UptimeTimeout;
int PingFrequency;
int SQLPingFreq;
int ClearInActive;
int ClearChanInActive;

int HidePasswords;
char *Smiley;

char *UserTable;
char *ChanBansTable;
char *ChanExceptTable;
char *ChanQuietTable;
char *IsOnTable;
char *ServerTable;
char *GlineTable;
char *ChanTable;
char *MaxValueTable;
char *ChanInviteTable;
char *TLDTable;
char *CTCPTable;
char *SglineTable;
char *SqlineTable;
char *ChanStatsTable;
char *ServerStatsTable;
char *AdminTable;

char *AliasesTable;
char *CStatsTable;
char *UStatsTable;
char *CurrentTable;
char *StatsTable;
char *SpamTable;

int SP_HTML;

int UseTS6;
int UseZIP;
char *NickChar;
int CTCPUsers;
int CTCPUsersEOB;

int CSDefFlag = 0;
int BackupFreq;
int KeepBackupsFor;

int AutoOp;
char *AutoMode;

int SockIPNumber;
char **ExtSockIPs;
int XMLRPC_Enable;
char *XMLRPC_Host;
int XMLRPC_Port;

char *JupeMaster;

/* this set of lists is used for loading and rehashing the config file */

Conf_Modules *new_modules = NULL;

int SQLDisableDueServerLost;
int SQLRetryOnServerLost;

char *StatsPage;
int PartOnEmpty;

tConf *mainconf;

int KeepUserTable;
int KeepServerTable;


/*************************************************************************/

/* Used if RELOAD fails */
void clear_newconfs()
{
	int i;

	if (new_modules)
	{
		for (i = 0; new_modules->autoload[i]; i++)
			free(new_modules->autoload[i]);
		for (i = 0; new_modules->delayed[i]; i++)
			free(new_modules->delayed[i]);
		free(new_modules);
		new_modules = NULL;
	}
	return;
}

/*************************************************************************/

static void free_vars(cVar * vars[])
{
	int i = 0;

	while (vars[i])
	{
		if (vars[i]->value)
			free(vars[i]->value);
		free(vars[i]);
		i++;
	}
}

/*************************************************************************/

static const char *current_file = "unknown";
static int quote = 0;

void confparse_error(const char *problem, int line)
{
	alog(LOG_ERROR, langstr(CONFIG_ERROR_BASE), problem, line,
	     current_file);
	if (!denora->nofork && isatty(2))
	{
		dfprintf(stderr, langstr(CONFIG_ERROR_BASE), problem, line,
		         current_file);
		fprintf(stderr, "\n");
	}
	return;
}

/*************************************************************************/

/* check_quote
 * this routine skips over any ignored items inside our file
 */
static char *check_quote(char *cur)
{
	if (quote)
	{
		while ((cur = strchr(cur, '*')))
			if (*(++cur) == '/')
			{
				cur++;
				quote = 0;
				break;
			}
		if (!cur)
			return cur;
	}
	while ((*cur == ' ') || (*cur == '\t'))
		cur++;
	/* now we've hit something .. check for single line quotes */
	if (!*cur || *cur == '#' || *cur == '\n' ||
	        (*cur == '/' && *(cur + 1) == '/'))
		return NULL;
	/* check for multiple line quotes */
	if ((*cur == '/') && (*(cur + 1) == '*'))
	{
		cur += 2;
		quote = 1;
		while ((cur = strchr(cur, '*')))
			if (*(++cur) == '/')
			{
				cur++;
				quote = 0;
				break;
			}
		if (!cur)
			return cur;
		else
			return check_quote(cur);
	}
	return cur;
}

/*************************************************************************/

static char *parse_block(tConf * conftab, tConf * block, char *cur,
                         FILE * file, int *lnum)
{
	char *tok, *var, *var2;
	char line[LINE_MAX];
	tConf *b2 = NULL;
	sConf *item = NULL;
	sConf *subconftab = block->subtok;
	cVar *vars[MAX_VALUES] = { 0 };
	int vnum = 0, tlnum = 0, clear = 0, done = 0, skip = 0;

	if ((subconftab) && (subconftab->flag == SCONFF_STRING))
	{
		/* this subtype only takes freeform variables
		 * dont bother looking for tokens
		 */
		int i = 0;
		while (!BadPtr(cur) || ((fgets(line, LINE_MAX, file) != NULL) &&
		                        (*lnum)++ && (cur = line)))
		{
			cur = check_quote(cur);
			if (BadPtr(cur))
				continue;
			if (clear)
			{
				if (*cur != ';')
				{
					confparse_error("Missing semicolon", *lnum);
					free_vars(vars);
					return NULL;
				}
				else
					cur++;
				clear = 0;
				cur = check_quote(cur);
				if (BadPtr(cur))
					continue;
			}
			if (done)
			{
				if (*cur != ';')
				{
					confparse_error("Missing block end semicolon", *lnum);
					free_vars(vars);
					return NULL;
				}
				else
					cur++;
				if (((*block->func) (vars, *lnum)) == -1)
				{
					free_vars(vars);
					return NULL;
				}
				if (BadPtr(cur))
					*cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
				free_vars(vars);
				return cur;
			}
			cur = check_quote(cur);
			if (BadPtr(cur))
				continue;
			if (*cur == '}')
			{
				done = 1;
				cur++;
				cur = check_quote(cur);
				if (BadPtr(cur))
					continue;
				if (*cur != ';')
				{
					confparse_error("Missing block end semicolon", *lnum);
					free_vars(vars);
					return NULL;
				}
				else
					cur++;
				if (((*block->func) (vars, *lnum)) == -1)
				{
					free_vars(vars);
					return NULL;
				}
				if (BadPtr(cur))
					*cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
				free_vars(vars);
				return cur;
			}
			vars[vnum] = (cVar *) malloc(sizeof(cVar));
			memset((char *) vars[vnum], '\0', sizeof(cVar));
			vars[vnum]->loaded = 1;
			vars[vnum]->type = NULL;
			tok = cur;
			if (*cur == '"')
			{
				i = 1;
				cur++;
			}
			var = cur;
			if (i == 1)
			{
				while (!BadPtr(cur) && (*cur != '"'))
					cur++;
				if (BadPtr(cur))
				{
					confparse_error("Cant find closequote", *lnum);
					free_vars(vars);
					return NULL;
				}
				*cur = '\0';
				cur++;
				while (!BadPtr(cur) && (*cur != ';'))
					cur++;
			}
			else
			{
				while (!BadPtr(cur) && (*cur != ';'))
				{
					if (*cur == ' ')
					{
						*cur = '\0';
						if (vars[vnum]->loaded == 1)
						{
							vars[vnum]->value = sstrdup(var);
							vars[vnum]->loaded = 2;
						}
					}
					else if (vars[vnum]->loaded == 2)
					{
						confparse_error("Junk after value", *lnum);
						free_vars(vars);
						return NULL;
					}
					cur++;
				}
			}
			tlnum = *lnum;
			if (BadPtr(cur))
			{
				clear = 1;
				continue;
			}
			*cur = '\0';
			cur++;
			if (vars[vnum]->loaded == 1)
				vars[vnum]->value = sstrdup(var);
			vars[vnum]->loaded = 3;
			vnum++;
		}
		confparse_error("Unexpected EOF: Syntax Error", tlnum);
		free_vars(vars);
		return NULL;
	}

	while (!BadPtr(cur)
	        || ((fgets(line, LINE_MAX, file) != NULL) && (*lnum)++
	            && (cur = line)))
	{
		cur = check_quote(cur);
		if (BadPtr(cur))
			continue;
		if (clear)
		{
			/* if we're looking for a closing semicolon, check for it first
			 * if we cant find it, ignore it and hope for the best
			 */
			if (*cur != ';')
			{
				confparse_error("Missing semicolon ", *lnum);
				free_vars(vars);
				return NULL;
			}
			else
				cur++;
			clear = 0;
			if (vars[vnum])
			{
				vars[vnum]->loaded = 3;
				vnum++;
			}
			item = NULL;
			cur = check_quote(cur);
			if (BadPtr(cur))
				continue;
		}
		if (done)
		{
			/* we've found the end of our block, now we're looking for the
			 * closing semicolon.  if we cant find it, ignore it and
			 * hope for the best
			 */
			if (*cur != ';')
			{
				confparse_error("Missing block end semicolon", *lnum);
				free_vars(vars);
				return NULL;
			}
			else
				cur++;
			if (((*block->func) (vars, *lnum)) == -1)
			{
				free_vars(vars);
				return NULL;
			}
			if (BadPtr(cur))
				*cur = '#';     /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
			free_vars(vars);
			return cur;
		}
		if (b2 && b2->tok)
		{
			/* we've identified a nested block in a previous loop.
			 * we didnt get an openquote yet, so look for that.
			 * we must find this.  keep looking til we do.
			 */
			if (*cur != '{')
			{
				confparse_error("Junk after nested block token", *lnum);
				free_vars(vars);
				return NULL;
			}
			cur++;
			cur = check_quote(cur);
			cur = parse_block(conftab, b2, cur, file, lnum);
			b2 = NULL;
			continue;
		}
		if (!item || !item->tok)
		{
			/* if we dont already have a specific token we're working on
			 * find one here.
			 */
			cur = check_quote(cur);
			if (BadPtr(cur))
				continue;
			tok = cur;
			tlnum = *lnum;
			if (*cur == '}')
			{
				/* if we've got a closebracket, then we've hit the end
				 * of our block.
				 */
				done = 1;
				cur++;
				cur = check_quote(cur);
				if (BadPtr(cur))
					continue;
				if (*cur != ';')
				{
					confparse_error("Missing block end semicolon", *lnum);
					free_vars(vars);
					return NULL;
				}
				else
					cur++;
				if (((*block->func) (vars, *lnum)) == -1)
				{
					free_vars(vars);
					return NULL;
				}
				if (BadPtr(cur))
					*cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
				free_vars(vars);
				return cur;

			}
			/* our token ends where whitespace or a semicolon begins */
			while (!BadPtr(cur) && ((*cur != ' ') && (*cur != ';') &&
			                        (*cur != '\t') && (*cur != '\n')
			                        && (*cur != '\r')))
				cur++;
			if (BadPtr(cur))
			{
				confparse_error("Unterminated token", *lnum);
				free_vars(vars);
				return NULL;
			}
			else
			{
				if (*cur == ';')
					skip = 1;
				*cur = '\0';
			}
			cur++;
			if (block->nest)
			{
				/* we allow nested stuff inside here, so check for it. */
				for (b2 = conftab; b2->tok; b2++)
					if (!stricmp(b2->tok, tok))
						break;
				if (b2 && b2->tok)
					if (!(block->nest & b2->flag))
						b2 = NULL;
				if (b2 && b2->tok)
				{
					/* recurse through the block we found */
					tlnum = *lnum;
					cur = check_quote(cur);
					if (BadPtr(cur))
						continue;
					if (*cur != '{')
					{
						confparse_error("Junk after nested block name",
						                *lnum);
						free_vars(vars);
						return NULL;
					}
					cur++;
					cur = check_quote(cur);
					cur = parse_block(conftab, b2, cur, file, lnum);
					if (!cur)
					{
						free_vars(vars);
						return NULL;
					}
					b2 = NULL;
					continue;
				}
			}
			if (BadPtr(tok))
			{
				continue;
			}

			/* find our token */
			for (item = subconftab; item && item->tok; item++)
				if (!stricmp(item->tok, tok))
					break;
			if (!item->tok)
			{
				confparse_error("Unknown token", *lnum);
				alog(LOG_DEBUG, "debug: error token value = %s", tok);
				free_vars(vars);
				return NULL;
			}
			/* create our variable */
			vars[vnum] = (cVar *) malloc(sizeof(cVar));
			memset((char *) vars[vnum], '\0', sizeof(cVar));
			vars[vnum]->type = item;
			vars[vnum]->loaded = 1;
		}
		if (item->var & VARTYPE_NONE)
		{
			/* we dont need to grab a variable for this type
			 * just look for the closing semicolon, and move on */
			vars[vnum]->loaded = 2;
			if (!skip)
			{
				/* we've already gotten our semicolon back
				 * at the end of our token.  dont look for it. */
				cur = check_quote(cur);
				while (!BadPtr(cur) && (*cur != ';'))
					cur++;
				if (BadPtr(cur))
				{
					clear = 1;
					continue;
				}
				cur++;
			}
			skip = 0;
			vars[vnum]->loaded = 3;
			vnum++;
			item = NULL;
			continue;
		}
		if (item->var & VARTYPE_STRING)
		{
			/* we're looking for a string here, so we require
			 * quotes around the string...
			 */
			cur = check_quote(cur);
			while (!BadPtr(cur) && (*cur != '"'))
				cur++;
			if (BadPtr(cur))
				continue;
			cur++;
			var = cur;
			while (!BadPtr(cur) && (*cur != '"'))
				cur++;
			if (BadPtr(cur))
			{
				confparse_error("Unterminated quote", *lnum);
				free_vars(vars);
				return NULL;
			}
			*cur = '\0';
			cur++;
			vars[vnum]->value = sstrdup(var);
			vars[vnum]->loaded = 2;
			while (!BadPtr(cur) && (*cur != ';'))
				cur++;
			if (BadPtr(cur))
			{
				clear = 1;
				continue;
			}
			cur++;
			vars[vnum]->loaded = 3;
			vnum++;
			item = NULL;
			continue;
		}
		if (item->var & VARTYPE_INT)
		{
			cur = check_quote(cur);
			var = cur;
			while (!BadPtr(cur) && ((*cur != ';') && (*cur != '\t') &&
			                        (*cur != '\n') && (*cur != ' ')
			                        && (*cur != '\r')))
				cur++;
			if (BadPtr(cur))
			{
				clear = 1;
				continue;
			}
			if (*cur != ';')
				clear = 1;
			*cur = '\0';
			cur++;
			var2 = var;
			while (*var)
			{
				if (IsDigit(*var))
					var++;
				else
				{
					confparse_error("Expecting integer value", *lnum);
					free_vars(vars);
					return NULL;
				}
			}
			if (!item)
				continue;
			var = var2;
			vars[vnum]->value = sstrdup(var);
			vars[vnum]->loaded = 3;
			vnum++;
			item = NULL;
			continue;
		}
		if (item->var & VARTYPE_NAME)
		{
			cur = check_quote(cur);
			if (!BadPtr(cur) && (*cur == '"'))
				cur++;
			var = cur;
			while (!BadPtr(cur) && (*cur != ';'))
			{
				if ((*cur == ' ') || (*cur == '"') || (*cur == '\t'))
				{
					*cur = '\0';
					if (vars[vnum]->loaded == 1)
					{
						vars[vnum]->value = sstrdup(var);
						vars[vnum]->loaded = 2;
					}
				}
				cur++;
			}
			if (BadPtr(cur))
			{
				clear = 1;
				continue;
			}
			*cur = '\0';
			cur++;
			if (vars[vnum]->loaded == 1)
				vars[vnum]->value = sstrdup(var);
			vars[vnum]->loaded = 3;
			vnum++;
			item = NULL;
			continue;
		}
		confparse_error("Unexpected EOF:  Syntax Error", tlnum);
		free_vars(vars);
		return NULL;
	}
	confparse_error("Unexpected EOF:  Syntax Error", tlnum);
	free_vars(vars);
	return NULL;
}

/*************************************************************************/

/* Called on startup, check the config file */

int initconf(const char *filename, int reload, tConf * conftab)
{
	int lnum = 0, blnum = 0, clear = 0;
	char line[LINE_MAX];
	char *cur = NULL;
	char *tok;
	tConf *block = NULL;
	FILE *file;
	const char *strlang;
	current_file = filename;

	if (!(file = FileOpen((char *) filename, FILE_READ)))
	{
		strlang = langstr(CONFIG_ERROR_OPENING);
		if (!reload)
		{
			log_perror(strlang, filename);
		}
		else
		{
			alog(LOG_ERROR, langstr(CONFIG_ERROR_OPENING), filename);
		}
		return -1;
	}

	while (!BadPtr(cur)
	        || ((fgets(line, LINE_MAX, file) != NULL) && ++lnum
	            && (cur = line)))
	{
		cur = check_quote(cur);
		if (BadPtr(cur))
			continue;
		/* now, we should be ok to get that token.. */
		if (!block)
		{
			tok = cur;
			while ((*cur != ' ') && (*cur != '\n') && (*cur != '{')
			        && (*cur != '\r'))
				cur++;          /* find the whitespace following the token */
			if (*cur == '{')
				clear = 1;
			*cur = '\0';
			cur++;
			if (!stricmp("INCLUDE", tok))
			{
				/* this is an include - find pull out the file name
				 * and parse this file now
				 */
				char *var;
				cur = check_quote(cur);
				if ((*cur == '"') || *cur == '<')
					cur++;
				var = cur;
				while ((*cur != ' ') && (*cur != '"') && (*cur != '>') &&
				        (*cur != '\n') && (*cur != ';') && (*cur != '\t')
				        && (*cur != '\r'))
					cur++;
				if (BadPtr(cur))
				{
					confparse_error("Bad include line", lnum);
					return -1;
				}
				*cur = '\0';
				cur++;
				if (initconf(var, reload, conftab) == -1)
					return -1;
				current_file = filename;        /* reset */
				continue;
			}
			if (BadPtr(tok))
				continue;

			for (block = conftab; block->tok; block++)
			{
				if (!BadPtr(block->tok) && !stricmp(block->tok, tok))
					break;
			}
			if (!block->tok)
			{
				alog(LOG_DEBUG, "debug: token is %s", tok);
				confparse_error("Unknown block type", lnum);
				printf("tok %s %d\r\n", tok, *tok);
				return -1;
			}
			blnum = lnum;
		}
		cur = check_quote(cur);
		if (BadPtr(cur))
			continue;
		if ((*cur == '{') || clear)
			cur++;
		else
		{
			confparse_error("Junk after block name", lnum);
			return -1;
		}
		if ((cur = parse_block(conftab, block, cur, file, &lnum)) == NULL)
		{
			return -1;
		}
		clear = 0;
		block = NULL;
		continue;
	}
	if (clear)
	{
		confparse_error("Unexpected EOF:  Syntax error", blnum);
		return -1;
	}
	return 1;
}

/*************************************************************************/

Directive *ModuleCreateConfigDirective(const char *name, int type, int flags, void *ptr)
{
	Directive *a;

	a = calloc(sizeof(Directive), 1);
	a->name = sstrdup(name);
	a->params->type = type;
	a->params->flags = flags;
	a->params->ptr = ptr;
	return a;
}

/*************************************************************************/
/**
 * Deal with modules who want to lookup config directives!
 * @param h The Directive to lookup in the config file
 * @return 1 on success, 0 on error
 **/
int moduleGetConfigDirective(char *configfile, Directive * d)
{
	FILE *config;
	char *dir = NULL;
	char buf[1024];
	int linenum = 0;
	int ac = 0;
	char *av[MAXPARAMS];
	char *s = NULL;
	char *t;
	int retval = 1;

	config = FileOpen(configfile, FILE_READ);
	if (!config)
	{
		alog(LOG_NORMAL, "Can't open %s", configfile);
		return 0;
	}
	while (fgets(buf, sizeof(buf), config))
	{
		linenum++;
		if (*buf == '#' || *buf == '\r' || *buf == '\n')
			continue;
		dir = myStrGetOnlyToken(buf, '\t', 0);
		if (dir)
		{
			s = myStrGetTokenRemainder(buf, '\t', 1);
		}
		else
		{
			dir = myStrGetOnlyToken(buf, ' ', 0);
			if (dir || (dir = myStrGetOnlyToken(buf, '\n', 0)))
			{
				s = myStrGetTokenRemainder(buf, ' ', 1);
			}
			else
			{
				continue;
			}
		}

		if (stricmp(dir, d->name) == 0)
		{
			if (s)
			{
				while (isspace(*s))
					s++;
				while (*s)
				{
					if (ac >= MAXPARAMS)
					{
						alog(LOG_ERROR,
						     "module error: too many config. params");
						break;
					}
					t = s;
					if (*s == '"')
					{
						t++;
						s++;
						while (*s && *s != '"')
						{
							if (*s == '\\' && s[1] != 0)
								s++;
							s++;
						}
						if (!*s)
							alog(LOG_ERROR,
							     "module error: Warning: unterminated double-quoted string");
						else
							*s++ = 0;
					}
					else
					{
						s += strcspn(s, " \t\r\n");
						if (*s)
							*s++ = 0;
					}
					av[ac++] = t;
					while (isspace(*s))
						s++;
				}
			}
			retval = parse_directive(d, dir, ac, av, linenum, 0, s);
		}
	}
	if (dir)
		free(dir);
	if (s)
		free(s);

	fclose(config);
	return retval;
}

/*************************************************************************/

/* Print an error message to the log (and the console, if open). */

void flatconfig_error(int linenum, const char *message, ...)
{
	char buf[4096];
	va_list args;

	va_start(args, message);
	ircvsnprintf(buf, sizeof(buf), message, args);
	va_end(args);



	if (linenum)
		alog(LOG_DEBUG, "%d : %s", linenum, buf);
	else
		alog(LOG_DEBUG, "%s", buf);
}

/*************************************************************************/

/* Parse a configuration line.  Return 1 on success; otherwise, print an
 * appropriate error message and return 0.  Destroys the buffer by side
 * effect.
 */

int parse_directive(Directive * d, char *dir, int ac, char *av[MAXPARAMS],
                    int linenum, int reload, char *s)
{
	int retval = 1;
	int i;
	long val;
	int coptind;

	if (stricmp(dir, d->name) != 0)
		return 1;
	coptind = 0;
	for (i = 0; i < MAXPARAMS && d->params[i].type != PARAM_NONE; i++)
	{
		if (reload && !(d->params[i].flags & PARAM_RELOAD))
			continue;

		if (d->params[i].type == PARAM_SET)
		{
			*(int *) d->params[i].ptr = 1;
			continue;
		}

		if (coptind >= ac)
		{
			if (!(d->params[i].flags & PARAM_OPTIONAL))
			{
				flatconfig_error(linenum, "Not enough parameters for `%s'",
				                 d->name);
				retval = 0;
			}
			break;
		}
		switch (d->params[i].type)
		{
			case PARAM_INT:
				val = strtol(av[coptind++], &s, 0);
				if (*s)
				{
					flatconfig_error(linenum,
					                 "%s: Expected an integer for parameter %d",
					                 d->name, coptind);
					retval = 0;
					break;
				}
				*(int *) d->params[i].ptr = val;
				break;
			case PARAM_POSINT:
				val = strtol(av[coptind++], &s, 0);
				if (*s || val <= 0)
				{
					flatconfig_error(linenum,
					                 "%s: Expected a positive integer for parameter %d",
					                 d->name, coptind);
					retval = 0;
					break;
				}
				if (errno == ERANGE && val == LONG_MAX)
				{
					/* well the true top off is 2,147,483,647 but lets not give them the real top */
					flatconfig_error(linenum,
					                 "%s: paramter %d is to large, reduce this value (0 to 2,147,483,646)",
					                 d->name, coptind);
				}
				*(int *) d->params[i].ptr = val;
				break;
			case PARAM_PORT:
				val = strtol(av[coptind++], &s, 0);
				if (*s)
				{
					flatconfig_error(linenum,
					                 "%s: Expected a port number for parameter %d",
					                 d->name, coptind);
					retval = 0;
					break;
				}
				if (val < 1 || val > 65535)
				{
					flatconfig_error(linenum,
					                 "Port numbers must be in the range 1..65535");
					retval = 0;
					break;
				}
				*(int *) d->params[i].ptr = val;
				break;
			case PARAM_STRING:
				*(char **) d->params[i].ptr = sstrdup(av[coptind++]);
				if (!d->params[i].ptr)
				{
					flatconfig_error(linenum, "%s: Out of memory", d->name);
					return 0;
				}
				break;
			case PARAM_TIME:
				val = dotime(av[coptind++]);
				if (val < 0)
				{
					flatconfig_error(linenum,
					                 "%s: Expected a time value for parameter %d",
					                 d->name, coptind);
					retval = 0;
					break;
				}
				*(int *) d->params[i].ptr = val;
				break;
			default:
				flatconfig_error(linenum, "%s: Unknown type %d for param %d",
				                 d->name, d->params[i].type, i + 1);
				retval = 0;         /* don't bother continuing--something's bizarre */
				break;
		}
	}
	return retval;;
}

/*************************************************************************/
