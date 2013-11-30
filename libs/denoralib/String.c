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
 * toupper:  Like the ANSI functions, but make sure we return an
 *                   int instead of a (signed) char.
 * @param c Char
 * @return int
 */
int toupper(char c)
{
#if defined(__NetBSD__)
	if (islower((unsigned int) c))
#else
	if (islower(c))
#endif
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

#if defined(__NetBSD__)
	if (isupper((unsigned int) c))
#else
	if (isupper(c))
#endif
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
 * Number of tokens in a string
 * @param str String
 * @param dilim Dilimiter
 * @return number of tokens
 */
int myNumToken(const char *str, const char dilim)
{
	int len, idx, counter = 0;
	if (!str)
	{
		return 0;
	}

	len = strlen(str);
	for (idx = 0; idx <= len; idx++)
	{
		if ((str[idx] == dilim))
		{
			counter++;
		}
	}
	return counter;
}

/*************************************************************************/

char *StringDup(const char *src)
{
	char *ret = NULL;

	if (src)
	{
		if ((ret = (char *) malloc(strlen(src) + 1)))
		{
			strcpy(ret, src);
		}
		if (!ret)
#ifndef _WIN32
			raise(SIGUSR1);
#else
			abort();
#endif
	}
	return ret;
}


/*************************************************************************/

/**
 * Change an unsigned string to a signed string, overwriting the original
 * string.
 * @param str is the input string
 * @return output string, same as input string.
 */
char *str_signed(unsigned char *str)
{
	char *nstr;

	nstr = (char *) str;
	while (*str)
	{
		*nstr = (char) *str;
		str++;
		nstr++;
	}

	return nstr;
}

/**
 * strnrepl:  Replace occurrences of `old' with `new' in string `s'.  Stop
 *            replacing if a replacement would cause the string to exceed
 *            `size' bytes (including the null terminator).  Return the
 *            string.
 * @param s String
 * @param size size of s
 * @param old character to replace
 * @param new character to replace with
 * @return updated s
 */
char *strnrepl(char *s, int32 size, const char *old, const char *new)
{
	char *ptr = s;
	int32 left = strlen(s);
	int32 avail = size - (left + 1);
	int32 oldlen = strlen(old);
	int32 newlen = strlen(new);
	int32 diff = newlen - oldlen;

	while (left >= oldlen)
	{
		if (strncmp(ptr, old, oldlen) != 0)
		{
			left--;
			ptr++;
			continue;
		}
		if (diff > avail)
			break;
		if (diff != 0)
			memmove(ptr + oldlen + diff, ptr + oldlen, left + 1 - oldlen);
		strncpy(ptr, new, newlen);
		ptr += newlen;
		left -= oldlen;
	}
	return s;
}

#if !defined(HAVE_STRLCPY)
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size)
	{
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}
#endif