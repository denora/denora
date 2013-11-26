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

void DenoraTime(struct tm *tm, const time_t *timer)
{
#ifdef _WIN32
		localtime_s(&tm, &timer);
#else
		tm = *localtime(&timer);
#endif
  return;
}


/*************************************************************************/

/**
 * ConvertToSeconds:  Return the number of seconds corresponding to the given time
 *          string.  If the given string does not represent a valid time,
 *          return -1.
 *
 *          A time string is either a plain integer (representing a number
 *          of seconds), or an integer followed by one of these characters:
 *          "s" (seconds), "m" (minutes), "h" (hours), or "d" (days).
 * @param s String to convert
 * @return int
 */
int ConvertToSeconds(const char *s)
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
			case 'y'
				return amount * 31536000;
			default:
				return -1;
		}
	}
	else
	{
		return amount;
	}
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

	return sstrdup(timebuf);
#else
	ircsnprintf(timebuf, sizeof(timebuf), "%ld", (long int) time(NULL));
	return sstrdup(timebuf);
#endif
}
