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

/* The list of lists of messages. */
char **langtexts[NUM_LANGS];

/* Order in which languages should be displayed: (alphabetical) */
static int langorder[NUM_LANGS] =
{
	LANG_EN_US,                 /* English (US) */
	LANG_ES,                    /* Spanish */
	LANG_DE,                    /* German */
	LANG_FR,                    /* French */
	LANG_NL,                    /* Dutch */
	LANG_IT,                    /* Italian */
	LANG_RU,                    /* Russian */
	LANG_PT_BR,                 /* Portuguese */
};

/*************************************************************************/

/* Load a language file. */

static int lang_read_int32(int32 * ptr, FILE * f)
{
	int a = fgetc(f);
	int b = fgetc(f);
	int c = fgetc(f);
	int d = fgetc(f);
	if (a == EOF || b == EOF || c == EOF || d == EOF)
		return -1;
	*ptr = a << 24 | b << 16 | c << 8 | d;
	return 0;
}

/*************************************************************************/

static void load_lang(int langindex, const char *filename)
{
	char buf[256];
	FILE *f;
	int32 num, i;

	alog(LOG_DEBUG, "debug: Loading language %d from file `languages/%s'",
	     langindex, filename);
	ircsnprintf(buf, sizeof(buf), "languages/%s", filename);
	if (!(f = FileOpen(buf, FILE_READ)))
	{
		log_perror("Failed to load language %d (%s)", langindex, filename);
		return;
	}
	else if (lang_read_int32(&num, f) < 0)
	{
		alog(LOG_ERROR,
		     "Failed to read number of strings for language %d (%s)",
		     langindex, filename);
		return;
	}
	else if (num != NUM_STRINGS)
	{
		alog(LOG_ERROR, "Warning: Bad number of strings (%d, wanted %d) "
		     "for language %d (%s)", num, NUM_STRINGS, langindex,
		     filename);
	}
	langtexts[langindex] = calloc(sizeof(char *), NUM_STRINGS);
	if (num > NUM_STRINGS)
	{
		num = NUM_STRINGS;
	}
	for (i = 0; i < num; i++)
	{
		int32 pos, len;
		fseek(f, i * 8 + 4, SEEK_SET);
		if (lang_read_int32(&pos, f) < 0 || lang_read_int32(&len, f) < 0)
		{
			alog(LOG_ERROR,
			     "Failed to read entry %d in language %d (%s) TOC", i,
			     langindex, filename);
			while (--i >= 0)
			{
				free(langtexts[langindex][i]);
			}
			free(langtexts[langindex]);
			langtexts[langindex] = NULL;
			return;
		}
		if (len == 0)
		{
			langtexts[langindex][i] = NULL;
		}
		else if (len >= 65536)
		{
			alog(LOG_ERROR,
			     "Entry %d in language %d (%s) is too long (over 64k)--"
			     "corrupt TOC?", i, langindex, filename);
			while (--i >= 0)
			{
				free(langtexts[langindex][i]);
			}
			free(langtexts[langindex]);
			langtexts[langindex] = NULL;
			return;
		}
		else if (len < 0)
		{
			alog(LOG_ERROR,
			     "Entry %d in language %d (%s) has negative length--"
			     "corrupt TOC?", i, langindex, filename);
			while (--i >= 0)
			{
				free(langtexts[langindex][i]);
			}
			free(langtexts[langindex]);
			langtexts[langindex] = NULL;
			return;
		}
		else
		{
			langtexts[langindex][i] = calloc(len + 1, 1);
			fseek(f, pos, SEEK_SET);
			if ((int) fread(langtexts[langindex][i], 1, len, f) != len)
			{
				alog(LOG_ERROR,
				     "Failed to read string %d in language %d (%s)", i,
				     langindex, filename);
				while (--i >= 0)
				{
					free(langtexts[langindex][i]);
				}
				free(langtexts[langindex]);
				langtexts[langindex] = NULL;
				return;
			}
			langtexts[langindex][i][len] = 0;
		}
	}
	fclose(f);
}

/*************************************************************************/

/* Initialize list of lists. */

void lang_init()
{
	int i, j;

	load_lang(LANG_EN_US, "en_us");
	load_lang(LANG_ES, "es");
	load_lang(LANG_DE, "de");
	load_lang(LANG_FR, "fr");
	load_lang(LANG_NL, "nl");
	load_lang(LANG_IT, "it");
	load_lang(LANG_RU, "ru");
	load_lang(LANG_PT_BR, "pt_br");

	SET_SEGV_LOCATION();

	for (i = 0; i < NUM_LANGS; i++)
	{
		if (langtexts[langorder[i]] != NULL)
		{
			SET_SEGV_LOCATION();
			for (j = 0; j < NUM_STRINGS; j++)
			{
				if (!langtexts[langorder[i]][j])
				{
					langtexts[langorder[i]][j] =
					    langtexts[DEF_LANGUAGE][j];
				}
				if (!langtexts[langorder[i]][j])
				{
					langtexts[langorder[i]][j] = langtexts[LANG_EN_US][j];
				}
			}
		}
	}
	SET_SEGV_LOCATION();

	if (!langtexts[DEF_LANGUAGE])
		fatal("Unable to load default language");
	for (i = 0; i < NUM_LANGS; i++)
	{
		if (!langtexts[i])
			langtexts[i] = langtexts[DEF_LANGUAGE];
	}
}

/*************************************************************************/

void lang_destory(void)
{
	int i;
	int j;

	for (i = 0; i < NUM_LANGS; i++)
	{
		for (j = 0; j < NUM_STRINGS; j++)
		{
			if (langtexts[i][j])
			{
				free(langtexts[i][j]);
			}
		}
	}

}

/*************************************************************************/

/* Format a string in a strftime()-like way, but heed the user's language
 * setting for month and day names.  The string stored in the buffer will
 * always be null-terminated, even if the actual string was longer than the
 * buffer size.
 * Assumption: No month or day name has a length (including trailing null)
 * greater than BUFSIZE.
 */

int strftime_lang(char *buf, int size, User * u, int format, struct tm *tm)
{
	int language = u ? u->language : StatsLanguage;
	char tmpbuf[BUFSIZE], buf2[BUFSIZE];
	char *s;
	int i, ret;

	*buf2 = '\0';
	*tmpbuf = '\0';

	if (!tm)
	{
		return 0;
	}

	strlcpy(tmpbuf, langtexts[language][format], sizeof(tmpbuf));
	if ((s = langtexts[language][STRFTIME_DAYS_SHORT]) != NULL)
	{
		for (i = 0; i < tm->tm_wday; i++)
			s += strcspn(s, "\n") + 1;
		i = strcspn(s, "\n");
		strncpy(buf2, s, i);
		buf2[i] = 0;
		strnrepl(tmpbuf, sizeof(tmpbuf), "%a", buf2);
	}
	if ((s = langtexts[language][STRFTIME_DAYS_LONG]) != NULL)
	{
		for (i = 0; i < tm->tm_wday; i++)
			s += strcspn(s, "\n") + 1;
		i = strcspn(s, "\n");
		strncpy(buf2, s, i);
		buf2[i] = 0;
		strnrepl(tmpbuf, sizeof(tmpbuf), "%A", buf2);
	}
	if ((s = langtexts[language][STRFTIME_MONTHS_SHORT]) != NULL)
	{
		for (i = 0; i < tm->tm_mon; i++)
			s += strcspn(s, "\n") + 1;
		i = strcspn(s, "\n");
		strncpy(buf2, s, i);
		buf2[i] = 0;
		strnrepl(tmpbuf, sizeof(tmpbuf), "%b", buf2);
	}
	if ((s = langtexts[language][STRFTIME_MONTHS_LONG]) != NULL)
	{
		for (i = 0; i < tm->tm_mon; i++)
			s += strcspn(s, "\n") + 1;
		i = strcspn(s, "\n");
		strncpy(buf2, s, i);
		buf2[i] = 0;
		strnrepl(tmpbuf, sizeof(tmpbuf), "%B", buf2);
	}
	ret = strftime(buf, size, tmpbuf, tm);
	if (ret == size)
		buf[size - 1] = 0;
	return ret;
}

/*************************************************************************/

/* Send a syntax-error message to the user. */
void syntax_error(char *service, User * u, const char *command, int msgnum)
{
	const char *str;

	if (!u)
	{
		return;
	}

	str = getstring(u, msgnum);
	notice_lang(service, u, SYNTAX_ERROR, str);
	notice_lang(service, u, MORE_INFO, service, command);
}

/*************************************************************************/
