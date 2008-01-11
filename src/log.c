/*
 *
 * © 2004-2008 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id$
 *
 */

#include "denora.h"

FILE *logfile;
static int curday = 0;

static int get_logname(char *name, int count, struct tm *tm);
char *log_gettimestamp(void);

/*************************************************************************/

static int get_logname(char *name, int count, struct tm *tm)
{
    char timestamp[32];
    time_t t;

    if (!tm) {
        time(&t);
        tm = localtime(&t);
    }

    strftime(timestamp, sizeof(timestamp), "%Y%m%d", tm);
    ircsnprintf(name, count, "logs/%s.%s", denora->logname, timestamp);
    curday = tm->tm_yday;

    return 1;
}

/*************************************************************************/

static void remove_log(void)
{
    time_t t;
    struct tm tm;

    char name[PATH_MAX];

    if (!KeepLogs)
        return;

    time(&t);
    t -= (60 * 60 * 24 * KeepLogs);
    tm = *localtime(&t);
    if (!get_logname(name, sizeof(name), &tm))
        return;
    unlink(name);
}

/*************************************************************************/

static void checkday(void)
{
    time_t t;
    struct tm tm;

    time(&t);
    tm = *localtime(&t);

    if (curday != tm.tm_yday) {
        close_log();
        remove_log();
        open_log();
    }
}

/*************************************************************************/

/* Open the log file.  Return -1 if the log file could not be opened, else
 * return 0. */

int open_log(void)
{
    char name[PATH_MAX];
    if (logfile)
        return 0;

    if (!get_logname(name, sizeof(name), NULL))
        return 0;

    logfile = FileOpen(name, FILE_APPEND);

    if (logfile)
        setvbuf(logfile, NULL, _IONBF, BUFSIZE);

    return logfile != NULL ? 0 : -1;
}

/*************************************************************************/

/* Close the log file. */

void close_log(void)
{
    if (!logfile)
        return;
    fclose(logfile);
    logfile = NULL;
}


/*************************************************************************/

/* added cause this is used over and over in the code */
char *log_gettimestamp(void)
{
    time_t t;
    struct tm tm;
    static char tbuf[256];

    time(&t);
#ifdef MSVS2005
    localtime_s(&tm, &t);
#else
    tm = *localtime(&t);
#endif
#if HAVE_GETTIMEOFDAY
    if (denora->debug) {
        char *s;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S", &tm);
        s = tbuf + strlen(tbuf);
        s += ircsnprintf(s, sizeof(tbuf) - (s - tbuf), ".%06d",
                         (int) tv.tv_usec);
        strftime(s, sizeof(tbuf) - (s - tbuf) - 1, " %Y]", &tm);
    } else {
#endif
        strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S %Y]", &tm);
#if HAVE_GETTIMEOFDAY
    }
#endif
    return tbuf;
}

/*************************************************************************/

/* Log stuff to the log file with a datestamp.  Note that errno is
 * preserved by this routine and log_perror().
 */

void alog(int type, const char *fmt, ...)
{
    va_list args;
    int errno_save = errno;
    char str[BUFSIZE];
    char *buf;
    *str = '\0';

    if (!fmt) {
        return;
    }

    if ((type == LOG_DEBUG || type == LOG_EXTRADEBUG
         || type == LOG_NONEXISTANT) && !denora->debug) {
        return;
    }
    if (type == LOG_SQLDEBUG && !denora->sqldebug) {
        return;
    }
    if (LOG_EXTRADEBUG == type && denora->debug <= 1) {
        return;
    }
    if ((type == LOG_DEBUGSOCK || type == LOG_ADNS)
        && !denora->socketdebug) {
        return;
    }

    checkday();

    va_start(args, fmt);
    ircvsnprintf(str, sizeof(str), fmt, args);
    va_end(args);

    buf = log_gettimestamp();

    if (!NoLogs && logfile) {
        fprintf(logfile, "%s %s\n", buf, str);
    }

    if (denora->nofork) {
        fprintf(stderr, "%s %s\n", buf, str);
    }

    if (!BadPtr(LogChannel) && !denora->debug && findchan(LogChannel)) {
        if (type != LOG_SQLDEBUG || type != LOG_DEBUGSOCK) {
            privmsg(s_StatServ, LogChannel, "%s", str);
        }
    }
    errno = errno_save;
}

/*************************************************************************/

/* Like alog(), but tack a ": " and a system error message (as returned by
 * strerror()) onto the end.
 */

void log_perror(const char *fmt, ...)
{
    va_list args;
    int errno_save = errno;
    char str[BUFSIZE];
    char *buf;
#ifdef MSVS2005
    char errbuf[256];
#else
    char *errbuf;
#endif

#ifdef MSVS2005
    strerror_s(errbuf, sizeof(errbuf), errno_save);
#else
    errbuf = strerror(errno_save);
#endif

    checkday();

    if (!fmt) {
        return;
    }
    buf = log_gettimestamp();

    va_start(args, fmt);
    ircvsnprintf(str, sizeof(str), fmt, args);
    va_end(args);

    if (!NoLogs && logfile) {
        fprintf(logfile, "%s %s : %s\n", buf, str, errbuf);
    }
    if (denora->nofork) {
        fprintf(stderr, "%s %s : %s\n", buf, str, errbuf);
    }
    errno = errno_save;
}


/*************************************************************************/

/* We've hit something we can't recover from.  Let people know what
 * happened, then go down.
 */

void fatal(const char *fmt, ...)
{
    va_list args;
    char *buf;
    char buf2[4096];
    int errno_save = errno;
#ifdef MSVS2005
    char errbuf[256];
#else
    char *errbuf;
#endif

#ifdef MSVS2005
    strerror_s(errbuf, sizeof(errbuf), errno_save);
#else
    errbuf = strerror(errno_save);
#endif

    checkday();

    buf = log_gettimestamp();

    va_start(args, fmt);
    ircvsnprintf(buf2, sizeof(buf2), fmt, args);
    va_end(args);

    if (!NoLogs && logfile) {
        fprintf(logfile, "%sFATAL: %s\n", buf, buf2);
    }
    if (denora->nofork) {
        fprintf(stderr, "%sFATAL: %s\n", buf, buf2);
    }
    if (servsock >= 0)
        denora_cmd_global(NULL, langstring(GLOBAL_FATAL_ERROR), buf2,
                          errbuf);


    exit(1);
}

/*************************************************************************/

/* Same thing, but do it like perror(). */

void fatal_perror(const char *fmt, ...)
{
    va_list args;
    char *buf, buf2[4096];
    int errno_save = errno;
#ifdef MSVS2005
    char errbuf[256];
#else
    char *errbuf;
#endif

#ifdef MSVS2005
    strerror_s(errbuf, sizeof(errbuf), errno_save);
#else
    errbuf = strerror(errno_save);
#endif

    checkday();

    buf = log_gettimestamp();

    va_start(args, fmt);
    ircvsnprintf(buf2, sizeof(buf2), fmt, args);
    va_end(args);
    if (!NoLogs && logfile)
        fprintf(logfile, "%sFATAL: %s: %s\n", buf, buf2, errbuf);

    if (stderr)
        fprintf(stderr, "%sFATAL: %s: %s\n", buf, buf2, errbuf);

    if (servsock >= 0)
        denora_cmd_global(NULL, langstring(GLOBAL_FATAL_ERROR), buf2,
                          errbuf);
    exit(1);
}

/*************************************************************************/
