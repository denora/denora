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

/**
 * Convert a string into HTML safe text
 *
 * @param s1 is the string to encode
 * @return returns HTML safe text
 *
 */
char *HTML_StringEncode(char *s1)
{
	int i;
	unsigned char c;
	char buf2[15];
	char buf3[BUFSIZE];
	char buf4[BUFSIZE];
	*buf2 = '\0';
	*buf3 = '\0';
	*buf4 = '\0';

	if (!s1 || !*s1)
	{
		return StringDup("");
	}

	if (BadPtr(s1))
	{
		return StringDup("");
	}

	for (i = 0; i <= (int) strlen(s1) - 1; i++)
	{
		c = s1[i];
		if (c == '&')
		{
			snprintf(buf3, BUFSIZE, "%s%s", buf4, "&amp;");
			snprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '<')
		{
			snprintf(buf3, BUFSIZE, "%s%s", buf4, "&lt;");
			snprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '>')
		{
			snprintf(buf3, BUFSIZE, "%s%s", buf4, "&gt;");
			snprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else if (c == '"')
		{
			snprintf(buf3, BUFSIZE, "%s%s", buf4, "&quot;");
			snprintf(buf4, BUFSIZE, "%s", buf3);
		}
		else
		{
			snprintf(buf3, BUFSIZE, "%s%c", buf4, c);
			snprintf(buf4, BUFSIZE, "%s", buf3);
		}
	}

	return StringDup(buf4);
}

