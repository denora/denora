/*
 *
 * (c) 2004-2012 Denora Team
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

/*************************************************************************/

/**
 * Convert a string into HTML safe text
 *
 * @param s1 is the string to encode
 * @return returns HTML safe text
 *
 */
char *char_encode(char *s1)
{
	int i;
	unsigned char c;
	char buf2[15];
	char buf3[BUFSIZE];
	char buf4[BUFSIZE];
	*buf2 = '\0';
	*buf3 = '\0';
	*buf4 = '\0';

	SET_SEGV_LOCATION();

	if (!s1 || !*s1)
	{
		return sstrdup("");
	}

	if (BadPtr(s1))
	{
		return sstrdup("");
	}

	for (i = 0; i <= (int) strlen(s1) - 1; i++)
	{
		c = s1[i];
		if (c == '&')
		{
			ircsnprintf(buf3, BUFSIZE, "%s%s", buf4, "&amp;");
			ircsnprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '<')
		{
			ircsnprintf(buf3, BUFSIZE, "%s%s", buf4, "&lt;");
			ircsnprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '>')
		{
			ircsnprintf(buf3, BUFSIZE, "%s%s", buf4, "&gt;");
			ircsnprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '"')
		{
			ircsnprintf(buf3, BUFSIZE, "%s%s", buf4, "&quot;");
			ircsnprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else
		{
			ircsnprintf(buf3, BUFSIZE, "%s%c", buf4, c);
			ircsnprintf(buf4, BUFSIZE, "%s", buf3);
		}
	}

	SET_SEGV_LOCATION();

	return sstrdup(buf4);
}

/*************************************************************************/

/**
 * toupper:  Like the ANSI functions, but make sure we return an
 *                   int instead of a (signed) char.
 * @param c Char
 * @return int
 */
int toupper(char c)
{
	SET_SEGV_LOCATION();

	if (islower(c))
	{
		return (unsigned char) c - ('a' - 'A');
	}
	else
	{
		return (unsigned char) c;
	}
}

/*************************************************************************/

/**
 * tolower:  Like the ANSI functions, but make sure we return an
 *                   int instead of a (signed) char.
 * @param c Char
 * @return int
 */
int tolower(char c)
{
	SET_SEGV_LOCATION();

	if (isupper(c))
	{
		return (unsigned char) c + ('a' - 'A');
	}
	else
	{
		return (unsigned char) c;
	}
}

/*************************************************************************/

/**
 * merge_args:  Take an argument count and argument vector and merge them
 *              into a single string in which each argument is separated by
 *              a space.
 * @param argc is the number of Args
 * @param argv Array
 * @return string of the merged array
 */
char *merge_args(int argc, char **argv)
{
	int i;
	static char s[4096];
	char *t;

	SET_SEGV_LOCATION();

	t = s;
	for (i = 0; i < argc; i++)
		t += ircsnprintf(t, sizeof(s) - (t - s), "%s%s", *argv++,
		                 (i < argc - 1) ? " " : "");
	return s;
}

/*************************************************************************/

/**
 * print_args:  Take an argument count and argument vector and merge them
 *              into a single string in which each argument is separated by
 *              a space.
 * @param source is the source of the message
 * @param argc is the Number of Args
 * @param argv Array
 * @return string of the merged array
 */
void protocol_debug(char *source, int argc, char **argv)
{
	int i;
	SET_SEGV_LOCATION();

	if (source)
	{
		alog(LOG_PROTOCOL, "Source %s", source);
	}

	if (argc)
	{
		for (i = 0; i < argc; i++)
		{
			alog(LOG_PROTOCOL, "av[%d] = %s", i, argv[i]);
		}
	}
	else
	{
		alog(LOG_PROTOCOL, "av[0] = NULL");
	}
	return;
}

/*************************************************************************/

/**
 * do_match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 * @param pattern To be matched
 * @param str String in which the pattern is to be matched
 * @param docase Case In/Senstive
 * @return 1 if the string matches the pattern, 0 if not.
 */
static int do_match_wild(const char *pattern, const char *str, int docase)
{
	char c;
	const char *s;

	SET_SEGV_LOCATION();

	if (BadPtr(str) || BadPtr(pattern))
	{
		return 0;
	}

	/* This WILL eventually terminate: either by *pattern == 0, or by a
	 * trailing '*'. */

	for (;;)
	{
		switch (c = *pattern++)
		{
			case 0:
				if (!*str)
					return 1;
				return 0;
			case '?':
				if (!*str)
					return 0;
				str++;
				break;
			case '*':
				if (!*pattern)
					return 1;       /* trailing '*' matches everything else */
				s = str;
				while (*s)
				{
					if ((docase ? (*s == *pattern)
					        : (tolower(*s) == tolower(*pattern)))
					        && do_match_wild(pattern, s, docase))
						return 1;
					s++;
				}
				break;
			default:
				if (docase ? (*str++ != c) : (tolower(*str++) != tolower(c)))
					return 0;
				break;
		}                       /* switch */
	}
}

/*************************************************************************/

/**
 * match_wild:  Case Senstive wild card search
 * @param pattern To be matched
 * @param str String in which the pattern is to be matched
 * @return 1 if the string matches the pattern, 0 if not.
 */
int match_wild(const char *pattern, const char *str)
{
	SET_SEGV_LOCATION();
	return do_match_wild(pattern, str, 1);
}

/*************************************************************************/

/**
 * match_wild:  Case Insenstive wild card search
 * @param pattern To be matched
 * @param str String in which the pattern is to be matched
 * @return 1 if the string matches the pattern, 0 if not.
 */
int match_wild_nocase(const char *pattern, const char *str)
{
	SET_SEGV_LOCATION();
	return do_match_wild(pattern, str, 0);
}

/*************************************************************************/

/**
 * match_usermask:  Match the mask to the users host
 *
 * @param mask To be matched
 * @param user is the user struct to check
 * @return 1 if the string matches the pattern, 0 if not.
 */
int match_usermask(const char *mask, User * user)
{
	char *mask2 = sstrdup(mask);
	char *nick, *username, *host;
	int result;

	if (strchr(mask2, '!'))
	{
		nick = strtok(mask2, "!");
		username = strtok(NULL, "@");
	}
	else
	{
		nick = NULL;
		username = strtok(mask2, "@");
	}
	host = strtok(NULL, "");
	if (!username || !host)
	{
		if (nick)
			free(nick);
		if (mask2)
			free(mask2);
		return 0;
	}
	SET_SEGV_LOCATION();

	if (nick)
	{
		result = match_wild_nocase(nick, user->nick)
		         && match_wild_nocase(username, user->username)
		         && (match_wild_nocase(host, user->host)
		             || match_wild_nocase(host, user->vhost));
		free(nick);
	}
	else
	{
		result = match_wild_nocase(username, user->username)
		         && (match_wild_nocase(host, user->host)
		             || match_wild_nocase(host, user->vhost));
	}
	free(username);
	return result;
}

/*************************************************************************/

/**
 * dotime:  Return the number of seconds corresponding to the given time
 *          string.  If the given string does not represent a valid time,
 *          return -1.
 *
 *          A time string is either a plain integer (representing a number
 *          of seconds), or an integer followed by one of these characters:
 *          "s" (seconds), "m" (minutes), "h" (hours), or "d" (days).
 * @param s String to convert
 * @return int
 */
int dotime(const char *s)
{
	int amount;

	if (BadPtr(s))
	{
		return -1;
	}

	SET_SEGV_LOCATION();

	amount = strtol(s, (char **) &s, 10);
	if (*s)
	{
		switch (*s)
		{
			case 's':
				return amount;
			case 'm':
				return amount * 60;
			case 'h':
				return amount * 3600;
			case 'd':
				return amount * 86400;
			default:
				return -2;
		}
	}
	else
	{
		return amount;
	}
}

/*************************************************************************/

/**
 * Get the token
 * @param str String to search in
 * @param dilim Character to search for
 * @param token_number the token number
 * @return token
 */
char *myStrGetToken(const char *str, const char dilim, int token_number)
{
	int len, idx, counter = 0, start_pos = 0;
	char *substring = NULL;
	if (!str)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	len = strlen(str);
	for (idx = 0; idx <= len; idx++)
	{
		if ((str[idx] == dilim) || (idx == len))
		{
			if (counter == token_number)
			{
				substring = myStrSubString(str, start_pos, idx);
				counter++;
			}
			else
			{
				start_pos = idx + 1;
				counter++;
			}
		}
	}
	return substring;
}

/*************************************************************************/

/**
 * Get the token only
 * @param str String to search in
 * @param dilim Character to search for
 * @param token_number the token number
 * @return token
 */
char *myStrGetOnlyToken(const char *str, const char dilim,
                        int token_number)
{
	int len, idx, counter = 0, start_pos = 0;
	char *substring = NULL;
	if (!str)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	len = strlen(str);
	for (idx = 0; idx <= len; idx++)
	{
		if (str[idx] == dilim)
		{
			if (counter == token_number)
			{
				if (str[idx] == '\r')
					substring = myStrSubString(str, start_pos, idx - 1);
				else
					substring = myStrSubString(str, start_pos, idx);
				counter++;
			}
			else
			{
				start_pos = idx + 1;
				counter++;
			}
		}
	}
	return substring;
}

/*************************************************************************/

/**
 * Get the Remaining tokens
 * @param str String to search in
 * @param dilim Character to search for
 * @param token_number the token number
 * @return token
 */
char *myStrGetTokenRemainder(const char *str, const char dilim,
                             int token_number)
{
	int len, idx, counter = 0, start_pos = 0;
	char *substring = NULL;
	if (!str)
	{
		return NULL;
	}
	len = strlen(str);

	SET_SEGV_LOCATION();

	for (idx = 0; idx <= len; idx++)
	{
		if ((str[idx] == dilim) || (idx == len))
		{
			if (counter == token_number)
			{
				substring = myStrSubString(str, start_pos, len);
				counter++;
			}
			else
			{
				start_pos = idx + 1;
				counter++;
			}
		}
	}
	return substring;
}

/*************************************************************************/

/**
 * Get the string between point A and point B
 * @param src String to search in
 * @param start Point A
 * @param end Point B
 * @return the string in between
 */
char *myStrSubString(const char *src, int start, int end)
{
	char *substring = NULL;
	int len, idx;
	if (!src)
	{
		return NULL;
	}
	SET_SEGV_LOCATION();

	len = strlen(src);
	if (((start >= 0) && (end <= len)) && (end > start))
	{
		substring = (char *) malloc(sizeof(char) * ((end - start) + 1));
		for (idx = 0; idx <= end - start; idx++)
		{
			substring[idx] = src[start + idx];
		}
		substring[end - start] = '\0';
	}
	return substring;
}

/*************************************************************************/

/**
 * Clean up the buffer for extra spaces
 * @param str to clean up
 * @return void
 */
void doCleanBuffer(char *str)
{
	char *in, *out;
	char ch;

	if (!str)
	{
		return;
	}

	SET_SEGV_LOCATION();

	in = str;
	out = str;

	while (issp(ch = *in++));
	if (ch != '\0')
		for (;;)
		{
			*out++ = ch;
			ch = *in++;
			if (ch == '\0')
				break;
			if (!issp(ch))
				continue;
			while (issp(ch = *in++));
			if (ch == '\0')
				break;
			*out++ = ' ';
		}
	*out = ch;                  /* == '\0' */
}

/*************************************************************************/

/**
 * Check if the given nick is a service and if so return 1, or return 0
 *
 * @param nick to check
 * @return boolean
 */
DENORA_INLINE boolean nickIsServices(char *nick)
{
	User *u;

	/* Set the segfault location */
	SET_SEGV_LOCATION();

	/**
	 * Check for a bad pointer that might have been passed to us
	 * this fixes bug #309
	 */
	if (BadPtr(nick))
	{
		alog(LOG_EXTRADEBUG, "nickIsServices was called with NULL values");
		return false;
	}
	else
	{
		/* Find the user */
		u = user_find(nick);
		/* reset the segfault location */
		SET_SEGV_LOCATION();
		/* if u check if they are flagged as a service */
		if (u && u->isservice)
		{
			return true;
		}
	}
	return false;
}

/*************************************************************************/

/**
 * Determine if we need to send the TOKEN
 * @param token1
 * @param token2
 * @return token to send
 */
const char *send_token(const char *token1, const char *token2)
{
	SET_SEGV_LOCATION();

	return (UseTokens && ircd->token && ircdcap->token ? token2 : token1);
}

/*************************************************************************/

/**
 * Number of tokens in a string
 * @param str String
 * @param dilim Dilimiter
 * @return number of tokens
 */
int myNumToken(const char *str, const char dilim)
{
	int len, idx, counter = 0, start_pos = 0;
	if (!str)
	{
		return 0;
	}
	SET_SEGV_LOCATION();

	len = strlen(str);
	for (idx = 0; idx <= len; idx++)
	{
		if ((str[idx] == dilim))
		{
			start_pos = idx + 1;
			counter++;
		}
	}
	return counter;
}

/*************************************************************************/

/**
 * Normalize buffer stripping control characters and colors
 * @param buf A string to be parsed for control and color codes
 * @return A string stripped of control and color codes
 */
char *normalizeBuffer(char *buf)
{
	char *newbuf;
	int i, len, j = 0;

	SET_SEGV_LOCATION();

	if (BadPtr(buf))
	{
		return NULL;
	}

	len = strlen(buf);
	newbuf = (char *) malloc(sizeof(char) * len + 1);

	for (i = 0; i < len; i++)
	{
		switch (buf[i])
		{
				/* ctrl char */
			case 1:
				break;
				/* Bold ctrl char */
			case 2:
				break;
				/* Color ctrl char */
			case 3:
				/* If the next character is a digit, its also removed */
				if (isdigit(buf[i + 1]))
				{
					i++;

					/* not the best way to remove colors
					 * which are two digit but no worse then
					 * how the Unreal does with +S - TSL
					 */
					if (isdigit(buf[i + 1]))
					{
						i++;
					}

					/* Check for background color code
					 * and remove it as well
					 */
					if (buf[i + 1] == ',')
					{
						i++;

						if (isdigit(buf[i + 1]))
						{
							i++;
						}
						/* not the best way to remove colors
						 * which are two digit but no worse then
						 * how the Unreal does with +S - TSL
						 */
						if (isdigit(buf[i + 1]))
						{
							i++;
						}
					}
				}

				break;
				/* tabs char */
			case 9:
				break;
				/* line feed char */
			case 10:
				break;
				/* carrage returns char */
			case 13:
				break;
				/* Reverse ctrl char */
			case 22:
				break;
				/* Underline ctrl char */
			case 31:
				break;
				/* A valid char gets copied into the new buffer */
			default:
				newbuf[j] = buf[i];
				j++;
		}
	}

	/* Terminate the string */
	newbuf[j] = 0;

	return (newbuf);
}

/*************************************************************************/

/** split_buf:  Split a buffer into arguments and store the arguments in an
 *             argument vector pointed to by argv (which will be malloc'd
 *             as necessary); return the argument count.
 *
 * @param buf is the buffer to split apart
 * @param argv is a recursive array into which the parts will be stored
 * @return it the count of the parts
 */
int split_buf(char *buf, char ***argv)
{
	int argvsize = 8;
	int argc;
	char *s;

	SET_SEGV_LOCATION();

	*argv = calloc(sizeof(char *) * argvsize, 1);
	argc = 0;
	while (*buf)
	{
		if (argc == argvsize)
		{
			argvsize += 8;
			*argv = realloc(*argv, sizeof(char *) * argvsize);
		}
		if (*buf == ':')
		{
			(*argv)[argc++] = buf + 1;
			buf = (char *) "";
		}
		else
		{
			s = strpbrk(buf, " ");
			if (s)
			{
				*s++ = 0;
				while (*s == ' ')
					s++;
			}
			else
			{
				s = buf + strlen(buf);
			}
			(*argv)[argc++] = buf;
			buf = s;
		}
	}
	return argc;
}

/*************************************************************************/

/**
 * Generate a P10 ping reply
 * @param start is the ping time we got
 * @return a valid P10 ping reply format
 */
char *militime_float(char *start)
{
	static char timebuf[18];
#if HAVE_GETTIMEOFDAY
	struct timeval tv;
	char *p;
	gettimeofday(&tv, NULL);

	SET_SEGV_LOCATION();

	if (start)
	{
		if ((p = strchr(start, '.')))
		{
			p++;
			ircsnprintf(timebuf, sizeof(timebuf), "%ld",
			            (tv.tv_sec - atoi(start)) * 1000 + (tv.tv_usec -
			                    atoi(p)) /
			            1000);
		}
		else
			strlcpy(timebuf, "0", sizeof(timebuf));
	}
	else
	{
		ircsnprintf(timebuf, sizeof(timebuf), "%ld.%ld", tv.tv_sec,
		            tv.tv_usec);
	}
	SET_SEGV_LOCATION();

	return timebuf;
#else
	ircsnprintf(timebuf, sizeof(timebuf), "%ld", (long int) time(NULL));
	return sstrdup(timebuf);
#endif
}

/*************************************************************************/

/**
 * Make sure all the config options are set that need to be
 * @return void - no returend value
 */
void post_config_check(void)
{
	int error = 0;
	if (BadPtr(ServerName))
	{
		error++;
	}
	if (BadPtr(ServerDesc))
	{
		error++;
	}
	if (BadPtr(ServiceUser))
	{
		error++;
	}
	SET_SEGV_LOCATION();

	if (!ReadTimeout)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_READ_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_READ_ERROR));
		}
		error++;
	}
	if (!WarningTimeout)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_WARNING_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s",
			        langstring(CONFIG_TIMEOUT_WARNING_ERROR));
		}
		error++;
	}
	if (!UpdateTimeout)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_UPDATE_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_UPDATE_ERROR));
		}
		error++;
	}
	if (!PingFrequency)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_PING_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_PING_ERROR));
		}
		error++;
	}
	if (!UptimeTimeout)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_UPTIME_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_UPTIME_ERROR));
		}
		error++;
	}
	if (!HTMLTimeout)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_HTML_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_HTML_ERROR));
		}
		error++;
	}
	if (!SQLPingFreq)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TIMEOUT_SQL_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TIMEOUT_SQL_ERROR));
		}
		error++;
	}
	if (BadPtr(s_StatServ))
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_SS_NAME_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_SS_NAME_ERROR));
		}
		error++;
	}
	if (BadPtr(desc_StatServ))
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_SS_DESC_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_SS_DESC_ERROR));
		}
		error++;
	}
	if (s_StatServ_alias && BadPtr(desc_StatServ_alias))
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_SS_ALIASDESC_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_SS_ALIASDESC_ERROR));
		}
		error++;
	}
	if (s_StatServ_alias)
	{
		if (BadPtr(s_StatServ_alias))
		{
			alog(LOG_ERROR, "%s", langstring(CONFIG_SS_ALIASDESC_ERROR));
			if (!denora->nofork && isatty(2))
			{
				fprintf(stderr, "%s",
				        langstring(CONFIG_SS_ALIASDESC_ERROR));
			}
			error++;
		}
	}
	if (!UserTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_USER_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TABLE_USER_ERROR));
		}
		error++;
	}
	if (!ChanBansTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_CHANBANS_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TABLE_CHANBANS_ERROR));
		}
		error++;
	}
	if (!ChanExceptTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_CHANEXCEPT_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s",
			        langstring(CONFIG_TABLE_CHANEXCEPT_ERROR));
		}
		error++;
	}
	if (!IsOnTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_ISON_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TABLE_ISON_ERROR));
		}
		error++;
	}
	if (!ServerTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_SERVER_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TABLE_SERVER_ERROR));
		}
		error++;
	}
	if (!GlineTable)
	{
		alog(LOG_ERROR, "%s", langstring(CONFIG_TABLE_GLINE_ERROR));
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "%s", langstring(CONFIG_TABLE_GLINE_ERROR));
		}
		error++;
	}
	if (!ChanTable)
	{
		alog(LOG_ERROR, "Lacking chan definition in tables block");
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "Lacking chan definition in tables block");
		}
		error++;
	}
	if (!MaxValueTable)
	{
		alog(LOG_ERROR, "Lacking maxvalue definition in tables block");
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "Lacking maxvalue definition in tables block");
		}
		error++;
	}
	if (!ChanInviteTable)
	{
		ChanInviteTable = sstrdup("chaninvites");
	}
	if (!TLDTable)
	{
		alog(LOG_ERROR, "Lacking tld definition in tables block");
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "Lacking tld definition in tables block");
		}
		error++;
	}
	if (!CTCPTable)
	{
		alog(LOG_ERROR, "Lacking ctcp definition in tables block");
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr, "Lacking ctcp definition in tables block");
		}
		error++;
	}
	if (!SglineTable)
	{
		SglineTable = sstrdup("sgline");
	}
	if (!SqlineTable)
	{
		SqlineTable = sstrdup("sqline");
	}
	if (!AutoMode)
	{
		AutoMode = sstrdup("+o");
	}

	if (error)
	{
		alog(LOG_ERROR,
		     "Numerious configuration options are incorrectly defined, exiting");
		if (!denora->nofork && isatty(2))
		{
			fprintf(stderr,
			        "Numerious configuration options are incorrectly defined, exiting");
		}
		exit(1);
	}
}

/*************************************************************************/

/**
 * Is the user a stats admin
 * @param u is the user struct to check
 * @return 1 if yes, 0 if no
 */
int is_stats_admin(User * u)
{
	SET_SEGV_LOCATION();
	if ((is_oper(u)) && (u->admin == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/

/**
 * Check if the string is a valid server name
 *
 * @param name is the name to check
 * @return 0 if not 1 if is valid
 */
int is_valid_server(char *name)
{
	unsigned char *s;
	int valid = 0;

	SET_SEGV_LOCATION();

	if (BadPtr(name))
	{
		return 0;
	}

	for (s = (unsigned char *) name; *s; s++)
	{
		if (*s < ' ' || *s > '~')
		{
			valid = 0;
			break;
		}
		if (*s == '.')
		{
			valid = 1;
		}
	}
	if (!valid)
	{
		return 0;
	}
	return 1;
}

/*************************************************************************/

int is_crypted(const char *passwd)
{
	const char *const valid_md5chars = "0123456789abcdef";
	int found, i, j;
#ifdef HAVE_CRYPT
	/* Check if the string matches $1$........$...................... */
	if (strlen(passwd) == 34 && passwd[0] == '$' && passwd[1] == '1' && passwd[2] == '$' && passwd[11] == '$')
	{
		return 1;
	}
#endif

	/* Check if string matches a md5 using ^[0-9a-f]{32}$ the non regex fugly way */
	if (strlen(passwd) == 32)
	{
		for (i = 0; i < 32; i++)
		{
			found = 0;
			for (i = 0; i < strlen(valid_md5chars); i++)
			{
				if (passwd[i] == valid_md5chars[i])
				{
					found = 1;
					break;
				}
			}
			if (!found)
				return 0;
		}
		return 1;
        }

	return 0;
}

char *MakePassword(char *plaintext)
{
#ifdef HAVE_CRYPT
	unsigned long seed[2];
	char salt[] = "$1$........";
	const char *const seedchars =
	    "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char *password;
	int i;

	/* Generate a (not very) random seed.
	   You should do it better than this... */
	seed[0] = time(NULL);
	seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

	/* Turn it into printable characters from `seedchars'. */
	for (i = 0; i < 8; i++)
		salt[3 + i] = seedchars[(seed[i / 5] >> (i % 5) * 6) & 0x3f];

	/* Read in the user's password and encrypt it. */
	password = crypt(plaintext, (char *) salt);
	return password;
#endif

	return md5(plaintext);
}

/*************************************************************************/

int ValidPassword(char *plaintext, char *checkvs)
{
	char *result;

#ifdef HAVE_CRYPT
	/* Read in the user's password and encrypt it,
	   passing the expected password in as the salt. */
	result = crypt(plaintext, checkvs);
	if (!BadPtr(result) && !strcmp(result, checkvs))
	{
		free(result);
		return 1;
	}
#endif
	result = md5(plaintext);
	if (!stricmp(result, checkvs))
	{
		free(result);
		return 1;
	}
	if (result)
		free(result);

	if (!strcmp(plaintext, checkvs))
	{
		return 1;
	}
	return 0;
}

/*************************************************************************/

#if 0
void BubbleSortData(temp_data * table, int size, int direction)
{
	int i, dum;
	alog(LOG_DEBUG, "size is %d", size);

	for (dum = 0; dum < size - 1; dum++)
	{
		for (i = 0; i < size - 1; i++)
		{
			alog(LOG_DEBUG, "count[%d] is from count[%d]", table[i].count,
			     table[i + 1].count);

			if (direction == SORT_DESC)
			{
				if (table[i].count < table[i + 1].count)
				{
					SortSwapArray(&table[i], &table[i + 1]);
				}
			}
			else if (direction == SORT_ASC)
			{
				if (table[i].count > table[i + 1].count)
				{
					SortSwapArray(&table[i + 1], &table[i]);
				}
			}
			else
			{
				if (table[i].count < table[i + 1].count)
				{
					SortSwapArray(&table[i], &table[i + 1]);
				}
			}
		}
	}
}

/* swap: interchange two structs */
void SortSwapArray(temp_data * table1, temp_data * table2)
{
	temp_data *temp;
	temp = table1;
	table1 = table2;
	table2 = temp;
}
#endif

/*************************************************************************/

void destroy_all(void)
{
	User *u, *next;
	Message *m, *next2;
	MessageHash *mh, *next3;
	Command *c, *next4;
	CommandHash *ch, *next5;
	CronEvent *cev, *next6;
	CronEventHash *cevh, *next7;
	EvtMessage *emsg, *next8;
	XMLRPCCmd *xml, *next9;
	XMLRPCCmdHash *xmlh, *next10;
	Exclude *e, *next14;
	Uid *uid, *next17;
	ServStats *ss, *next18;
	int i, j;
	char *sqlnick = NULL;

	if (ircd->ts6 && UseTS6)
	{
		if (TS6UPLINK)
			free(TS6UPLINK);
		if (TS6SID)
			free(TS6SID);
	}

	if (mod_current_buffer)
		free(mod_current_buffer);
	if (ircd->chanmodes)
	{
		free(ircd->chanmodes);
		ircd->chanmodes = NULL;
	}
	if (ircd->nickchars)
	{
		free(ircd->nickchars);
		ircd->nickchars = NULL;
	}

	do_squit(ServerName);
	if (denora->uplink)
	{
		do_squit(denora->uplink);
	}
	u = firstuser();
	while (u)
	{
		next = nextuser();
		if (denora->do_sql)
		{
			sqlnick = rdb_escape(u->nick);
			db_removenick(sqlnick, (char *) "Denora shutdown");
			free(sqlnick);
		}
		delete_user(u);
		u = next;
	}

	modules_unload_all();

	alog(LOG_DEBUG, "debug: Clearing Event Messages");
	m = first_message();
	while (m)
	{
		next2 = next_message();
		if (m)
		{
			destroyMessage(m);
		}
		m = next2;
	}
	mh = first_messagehash();
	while (mh)
	{
		next3 = next_messagehash();
		if (mh)
		{
			destroyMessageHash(mh);
		}
		mh = next3;
	}
	cev = first_cronevent();
	while (cev)
	{
		next6 = next_cronevent();
		if (cev)
		{
			destroyCronEvent(cev);
		}
		cev = next6;
	}
	cevh = first_croneventhash();
	while (cevh)
	{
		next7 = next_croneventhash();
		if (cevh)
		{
			destroyCronEventHash(cevh);
		}
		cevh = next7;
	}

	alog(LOG_DEBUG, "debug: Clearing Commands");
	c = first_command();
	while (c)
	{
		next4 = next_command();
		if (c)
		{
			if (destroyCommand(c) == MOD_ERR_UNKNOWN)
			{
				alog(LOG_DEBUG, "debug: could not free command %s",
				     c->name);
			}
		}
		c = next4;
	}
	alog(LOG_DEBUG, "debug: Clearing Commands Hahs");

	ch = first_commandhash();
	while (ch)
	{
		next5 = next_commandhash();
		if (ch)
		{
			destroyCommandHash(ch);
		}
		ch = next5;
	}

	alog(LOG_DEBUG, "debug: Clearing XMLRPC Commands");

	xml = first_xmlrpccmd();
	while (xml)
	{
		next9 = next_xmlrpccmd();
		if (xml)
		{
			destroyXMLRPCCommand(xml);
		}
		xml = next9;
	}
	alog(LOG_DEBUG, "debug: Clearing XMLRPC Hash Commands");

	xmlh = first_xmlrpchash();
	while (xmlh)
	{
		next10 = next_xmlrpchash();
		if (xmlh)
		{
			destroyxmlrpchash(xmlh);
		}
		xmlh = next10;
	}

	alog(LOG_DEBUG, "debug: Clearing Events");

	emsg = first_EvtMessage();
	while (emsg)
	{
		next8 = next_EvtMessage();
		if (emsg)
		{
			destroyEventHandler(emsg);
		}
		emsg = next8;
	}

	alog(LOG_DEBUG, "debug: Clearing Exclude");

	e = first_exclude();
	while (e)
	{
		next14 = next_exclude();
		if (e)
		{
			del_exclude(e);
		}
		e = next14;
	}

	alog(LOG_DEBUG, "debug: Clearing UID");

	uid = uid_first();
	while (e)
	{
		next17 = uid_next();
		if (uid)
		{
			delete_uid(uid);
		}
		uid = next17;
	}

	alog(LOG_DEBUG, "debug: Clearing Stats Chan");
	Fini_StatsChannel();

	alog(LOG_DEBUG, "debug: Clearing Chan Stats");
	Fini_ChannelStats();

	alog(LOG_DEBUG, "debug: Clearing Stats Server");

	ss = first_statsserver();
	while (ss)
	{
		next18 = next_statserver();
		if (ss)
		{
			delete_serverstats(ss);
		}
		ss = next18;
	}

	alog(LOG_DEBUG, "debug: Clearing external sockets");

	for (i = 0; i < SockIPNumber; i++)
	{
		if (ExtSockIPs[i])
			free(ExtSockIPs[i]);
	}

	alog(LOG_DEBUG, "debug: Clearing variables");

	if (denora->qmsg)
		free(denora->qmsg);
	if (denora->version_protocol)
		free(denora->version_protocol);
	if (mod_current_module && mod_current_module->author)
		free(mod_current_module->author);

	if (denora->uplink)
		free(denora->uplink);
	if (UserTable)
		free(UserTable);
	if (ChanBansTable)
		free(ChanBansTable);
	if (ChanExceptTable)
		free(ChanExceptTable);
	if (IsOnTable)
		free(IsOnTable);
	if (ServerTable)
		free(ServerTable);
	if (GlineTable)
		free(GlineTable);
	if (ChanTable)
		free(ChanTable);
	if (MaxValueTable)
		free(MaxValueTable);
	if (ChanInviteTable)
		free(ChanInviteTable);
	if (TLDTable)
		free(TLDTable);
	if (CTCPTable)
		free(CTCPTable);
	if (SqlineTable)
		free(SglineTable);
	if (SqlineTable)
		free(SqlineTable);
	if (AliasesTable)
		free(AliasesTable);
	if (CStatsTable)
		free(CStatsTable);
	if (UStatsTable)
		free(UStatsTable);
	if (StatsTable)
		free(StatsTable);
	if (ChanStatsTable)
		free(ChanStatsTable);
	if (ServerStatsTable)
		free(ServerStatsTable);
	if (SpamTable)
		free(SpamTable);
	if (CurrentTable)
		free(CurrentTable);
	if (NetworkName)
		free(NetworkName);
	if (NickChar)
		free(NickChar);
	if (ChannelDB)
		free(ChannelDB);
	if (statsDB)
		free(statsDB);
	if (SqlHost)
		free(SqlHost);
	if (SqlUser)
		free(SqlUser);
	if (SqlPass)
		free(SqlPass);
	if (LogChannel)
		free(LogChannel);
	if (Smiley)
		free(Smiley);
	if (XMLRPC_Host)
		free(XMLRPC_Host);
	if (SqlName)
		free(SqlName);
	if (SqlSock)
		free(SqlSock);
	if (s_StatServ)
		free(s_StatServ);
	if (desc_StatServ)
		free(desc_StatServ);
	if (ChanStatsTrigger)
		free(ChanStatsTrigger);
	if (s_StatServ_alias)
		free(s_StatServ_alias);
	if (IRCDModule)
		free(IRCDModule);
	if (desc_StatServ_alias)
		free(desc_StatServ_alias);
	if (RemoteServer)
		free(RemoteServer);
	for (j = 0; j < NumExcludeServers; j++)
	{
		if (ExcludeServers[j])
			free(ExcludeServers[j]);
	}
	if (RemotePassword)
		free(RemotePassword);
	if (LocalHost)
		free(LocalHost);
	if(MOTDFilename)
		free(MOTDFilename);
	if (ServerName)
		free(ServerName);
	if (ServerDesc)
		free(ServerDesc);
	if (ServiceUser)
		free(ServiceUser);
	if (NickChar)
		free(NickChar);
	if (HTMLFilename)
		free(HTMLFilename);
	if (GeoIPDBFileName)
		free(GeoIPDBFileName);
	if (excludeDB)
		free(excludeDB);
	if (ctcpDB)
		free(ctcpDB);
	if (ServerDB)
		free(ServerDB);
	if (ChannelStatsDB)
		free(ChannelStatsDB);
	if (TLDDB)
		free(TLDDB);
	if (Numeric)
		free(Numeric);
	remove_pidfile();
	if (PIDFilename)
		free(PIDFilename);
	if (rdb_errmsg)
		free(rdb_errmsg);
	alog(LOG_DEBUG, "debug: Clean up complete");
}
