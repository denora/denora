
/*
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

/*************************************************************************/

/**
 * Send a command to the server.  The two forms here are like
 * printf()/vprintf() and friends.
 * @param source Orgin of the Message (some times NULL)
 * @param fmt Format of the Message
 * @param ... any number of parameters
 * @return void
 */
void send_cmd(const char *source, const char *fmt, ...)
{
	va_list args;

	

	if (fmt && !denora->quitting)
	{
		va_start(args, fmt);
		vsend_cmd(source, fmt, args);
		va_end(args);
	}
}

/*************************************************************************/

/**
 * actually Send a command to the server.
 * @param source Orgin of the Message (some times NULL)
 * @param fmt Format of the Message
 * @param args List of the arguments
 * @return void
 */
void vsend_cmd(const char *source, const char *fmt, va_list args)
{
	char buf[BUFSIZE];
	*buf = '\0';

	
	total_sendmsg++;

	if (!BadPtr(fmt))
	{
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);

		if (source)
		{
			if (ircd->p10)
			{
				sockprintf(servsock, "%s %s\r\n", source, buf);
				eventprintf("%s %s", source, buf);
				alog(LOG_DEBUG, "debug: Sent: %s %s", source, buf);
			}
			else
			{
				sockprintf(servsock, ":%s %s\r\n", source, buf);
				eventprintf(":%s %s", source, buf);
				alog(LOG_DEBUG, "debug: Sent: :%s %s", source, buf);
			}
		}
		else
		{
			sockprintf(servsock, "%s\r\n", buf);
			eventprintf("%s", buf);
			alog(LOG_DEBUG, "debug: Sent: %s", buf);
		}
	}
}

/*************************************************************************/

/**
 * Send a server notice
 * @param source Orgin of the Message
 * @param s Server Struct
 * @param fmt Format of the Message
 * @param ... any number of parameters
 * @return void
 */
void notice_server(char *source, Server * s, char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';

	

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);

		denora_cmd_serv_notice(source, s->name, buf);
	}
}

/*************************************************************************/

/**
 * Send a notice to a user
 * @param source Orgin of the Message
 * @param u User Struct
 * @param fmt Format of the Message
 * @param ... any number of parameters
 * @return void
 */
void notice_user(char *source, User * u, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);

		denora_cmd_notice(source, u->nick, buf);
	}
}

/*************************************************************************/

/**
 * Send a NULL-terminated array of text as NOTICEs.
 * @param source Orgin of the Message
 * @param dest Destination of the Notice
 * @param text Array of text to send
 * @return void
 */
void notice_list(char *source, char *dest, char **text)
{
	while (*text)
	{
		/* Have to kludge around an ircII bug here: if a notice includes
		 * no text, it is ignored, so we replace blank lines by lines
		 * with a single space.
		 */
		if (**text)
		{
			denora_cmd_notice(source, dest, *text);
		}
		else
		{
			denora_cmd_notice(source, dest, " ");
		}
		text++;
	}
}

/*************************************************************************/

/**
 * Send a message in the user's selected language to the user using NOTICE.
 * @param source Orgin of the Message
 * @param dest User Struct
 * @param message is the index number
 * @param ... any number of parameters
 * @return void
 */
void notice_lang(char *source, User * dest, int message, ...)
{
	va_list args;
	char buf[4096];             /* because messages can be really big */
	char *s, *t;
	const char *fmt;
	if (!dest || !message)
	{
		return;
	}
	va_start(args, message);
	
	fmt = getstring(dest, message);
	if (!fmt)
		return;
	memset(buf, 0, 4096);
	
	ircvsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	s = buf;
	while (*s)
	{
		t = s;
		s += strcspn(s, "\n");
		if (*s)
			*s++ = 0;
		denora_cmd_notice(source, dest->nick, "%s", *t ? t : " ");
	}

}

/*************************************************************************/

/**
 * Like notice_lang(), but replace %S by the source.  This is an ugly hack
 * to simplify letting help messages display the name of the pseudoclient
 * that's sending them.
 * @param source Orgin of the Message
 * @param dest User Struct
 * @param message is the integer index number
 * @param ... any number of parameters
 * @return void
 */
void notice_help(char *source, User * dest, int message, ...)
{
	va_list args;
	char buf[4096], buf2[4096], outbuf[BUFSIZE];
	char *s, *t;
	const char *fmt;

	if (!dest || !message)
	{
		return;
	}
	va_start(args, message);
	fmt = getstring(dest, message);
	if (!fmt)
	{
		return;
	}
	strlcpy(buf2, fmt, sizeof(buf2));
	strnrepl(buf2, sizeof(buf2), "%S", "\1\1");
	ircvsnprintf(buf, sizeof(buf), buf2, args);
	va_end(args);
	s = buf;
	while (*s)
	{
		t = s;
		s += strcspn(s, "\n");
		if (*s)
			*s++ = 0;
		strlcpy(outbuf, t, sizeof(outbuf));
		strnrepl(outbuf, sizeof(outbuf), "\1\1", source);
		denora_cmd_notice(source, dest->nick, "%s",
		                  *outbuf ? outbuf : " ");
	}
}

/*************************************************************************/

/**
 * Send a NOTICE from the given source to the given nick.
 * @param source Orgin of the Message
 * @param dest Destination of the Message
 * @param fmt Format of the Message
 * @param ... any number of parameters
 * @return void
 */
void notice(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);

		denora_cmd_notice(source, dest, "%s", buf);
		va_end(args);
	}
}

/*************************************************************************/

/**
 * Send a PRIVMSG from the given source to the given nick.
 * @param source Orgin of the Message
 * @param dest Destination of the Message
 * @param fmt Format of the Message
 * @param ... any number of parameters
 * @return void
 */
void privmsg(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}

	denora_cmd_privmsg(source, dest, "%s", buf);
}

/*************************************************************************/
