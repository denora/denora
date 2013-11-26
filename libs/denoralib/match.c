/* Library of common denora code, built as a library
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
#include "denoralib.h"

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
	if (mask2)
		free(mask2);

	free(username);
	return result;
}

/*************************************************************************/

/*
 * Copyright © 1997 The Open Group
 *
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * return 1 for match, 0 for no match
 */
int regex_match(const char *string, char *pattern)
{
	int status;
	regex_t re;
	int     rc;
	char    buffer[100];

	if (!string || !pattern)
	{
	        return 0;
	}

	if (0 != (rc = regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB))) 
	{
		regerror(rc, &re, buffer, 100);
		DenoraLib_SetLastError(rc, buffer);
        	return 0;      /* report error */
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	if (status != 0) 
	{
		regerror(rc, &re, buffer, 100);
		DenoraLib_SetLastError(rc, buffer);
        	return 0;      /* report error */
	}
	regfree(&re);
	return 1;
}
