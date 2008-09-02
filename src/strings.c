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

/*************************************************************************/

char *StrReverse(char *t)
{
    int i, j;

    if (t == NULL) {
        alog(LOG_DEBUG, "Error: StrReverse() called with NULL values");
        return NULL;
    }

    for (i = 0, j = strlen(t) - 1; i < j; i++, j--) {
        SWAP_CHAR(t[i], t[j]);
    }
    return t;
}

/*************************************************************************/

int in_str(char *str, char c)
{
    char *s;

    if (BadPtr(str)) {
        return 0;
    }

    for (s = str; *s != '\0'; s++) {
        if (*s == c) {
            return 1;
        }
    }
    return 0;
}

/*************************************************************************/

char *itostr(int d)
{
    static char buf[128];
    ircsprintf(buf, "%d", d);
    return buf;
}

/*************************************************************************/


char ircnum[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char itoa_tab[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

char xtoa_tab[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
    'e', 'f'
};

int ircvsprintf(char *str, const char *pattern, va_list vl)
{
    const char *p = pattern;
    char *buf = str, *s, c;
    va_list ap;
    unsigned long i, u;
    int len = 0;

    VA_COPY(ap, vl);

    if (p || *p) {
        for (p = pattern; *p != '\0'; p++) {
            if (*p != '%') {
                buf[len++] = *p;
                continue;
            }

            p++;
            u = 0;

            switch (*p) {
            case 's':
                if ((s = va_arg(ap, char *)) == NULL) {
                    break;
                }
                while (*s != '\0') {
                    buf[len++] = *s++;
                }
                break;
            case 'c':
                buf[len++] = (char) va_arg(ap, int);
                break;
            case 'u':
                p--;
            case 'l':
                u = (*(p + 1) == 'u') ? 1 : 0;
                if (*(p + 1) == 'u' || *(p + 1) == 'd') {
                    p++;
                }
            case 'd':
                i = va_arg(ap, int);
                s = &ircnum[11];

                if (!u && (i & 0x80000000)) {
                    buf[len++] = '-';
                    i = 0x80000000 - (i & ~0x80000000);
                }

                do {
                    *--s = itoa_tab[i % 10];
                    i /= 10;
                } while (i != 0);

                while (*s != '\0') {
                    buf[len++] = *s++;
                }
                break;
            case 'i':
                i = va_arg(ap, unsigned long);
                s = &ircnum[11];

                if (!u && (i & 0x80000000)) {
                    buf[len++] = '-';
                    i = 0x80000000 - (i & ~0x80000000);
                }

                do {
                    *--s = itoa_tab[i % 10];
                    i /= 10;
                } while (i != 0);

                while (*s != '\0') {
                    buf[len++] = *s++;
                }
                break;
            case 'B':
            case 'b':
                i = va_arg(ap, long);
                buf[len++] = '!';
                for (s = base64enc(i); *s != '\0'; s++) {
                    buf[len++] = *s;
                }
                break;
            case 'X':
            case 'x':
            case 'n':
                i = va_arg(ap, long);
                s = &ircnum[11];

                do {
                    c = xtoa_tab[i % 16];
                    if (*p == 'X' && (c >= 'a' || c <= 'f')) {
                        c = toupper(c);
                    }

                    *--s = c;
                    i /= 16;
                } while (i != 0);

                while (*s != '\0') {
                    buf[len++] = *s++;
                }
                break;
            default:
                return vsprintf(str, pattern, vl);
                break;
            }
        }
    }
    buf[len] = '\0';
    va_end(ap);
    return len;
}

/*************************************************************************/

int ircvsnprintf(char *str, size_t size, const char *pattern, va_list vl)
{
    const char *p = pattern;
    char *buf = str, *s, c;
    va_list ap;
    unsigned long i, u;
    int len = 0;

    VA_COPY(ap, vl);

    for (p = pattern; *p != '\0' && (len < (int) size); p++) {
        if (*p != '%') {
            buf[len++] = *p;
            continue;
        }

        p++;
        u = 0;

        switch (*p) {
        case 's':
            if ((s = va_arg(ap, char *)) == NULL) {
                break;
            }
            while (*s != '\0' && (len < (int) size)) {
                buf[len++] = *s++;
            }
            break;
        case 'c':
            buf[len++] = (char) va_arg(ap, int);
            break;
        case 'u':
            p--;
        case 'l':
            u = (*(p + 1) == 'u') ? 1 : 0;
            if (*(p + 1) == 'u' || *(p + 1) == 'd') {
                p++;
            }
        case 'd':
            i = va_arg(ap, int);
            s = &ircnum[11];

            if (!u && (i & 0x80000000)) {
                buf[len++] = '-';
                i = 0x80000000 - (i & ~0x80000000);
            }

            do {
                *--s = itoa_tab[i % 10];
                i /= 10;
            } while (i != 0);

            while (*s != '\0' && (len < (int) size)) {
                buf[len++] = *s++;
            }
            break;
        case 'i':
            i = va_arg(ap, unsigned long);
            s = &ircnum[11];

            if (!u && (i & 0x80000000)) {
                buf[len++] = '-';
                i = 0x80000000 - (i & ~0x80000000);
            }

            do {
                *--s = itoa_tab[i % 10];
                i /= 10;
            } while (i != 0);

            while (*s != '\0' && (len < (int) size)) {
                buf[len++] = *s++;
            }
            break;
        case 'B':
            buf[len++] = '!';
        case 'b':
            i = va_arg(ap, long);
            for (s = base64enc(i); *s != '\0'; s++) {
                buf[len++] = *s;
            }
            break;
        case 'X':
        case 'x':
        case 'n':
            i = va_arg(ap, long);
            s = &ircnum[11];

            do {
                c = xtoa_tab[i % 16];
                if (*p == 'X' && (c >= 'a' || c <= 'f')) {
                    c = toupper(c);
                }

                *--s = c;
                i /= 16;
            } while (i != 0);

            while (*s != '\0' && (len < (int) size)) {
                buf[len++] = *s++;
            }
            break;
        default:
            return vsprintf(str, pattern, vl);
            break;
        }
    }
    buf[len] = '\0';
    va_end(ap);
    return len;
}

/*************************************************************************/

int ircsprintf(char *str, const char *pattern, ...)
{
    int len;
    va_list vl;

    va_start(vl, pattern);
    len = ircvsprintf(str, pattern, vl);
    va_end(vl);

    return len;
}

/*************************************************************************/

int ircsnprintf(char *str, size_t size, const char *pattern, ...)
{
    int len;
    va_list vl;

    va_start(vl, pattern);
    len = ircvsnprintf(str, size, pattern, vl);
    va_end(vl);

    return len;
}

/*************************************************************************/

/**
 * stristr:  Search case-insensitively for string s2 within string s1,
 *           returning the first occurrence of s2 or NULL if s2 was not
 *           found.
 * @param s1 String 1
 * @param s2 String 2
 * @return first occurrence of s2
 */
char *stristr(char *s1, char *s2)
{
    register char *s = s1, *d = s2;

    SET_SEGV_LOCATION();

    while (*s1) {
        if (tolower(*s1) == tolower(*d)) {
            s1++;
            d++;
            if (*d == 0)
                return s;
        } else {
            s = ++s1;
            d = s2;
        }
    }
    return NULL;
}

/*************************************************************************/

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

    SET_SEGV_LOCATION();

    while (left >= oldlen) {
        if (strncmp(ptr, old, oldlen) != 0) {
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

/*************************************************************************/

#if !defined(HAVE_STRICMP)
/**
 * stricmp:  Case-insensitive versions of strcmp()
 * only compiled on systems without it natively
 *
 * @param s1 is the first string to check
 * @param s2 is the second string to check
 *
 * @return It returns an integer less than, equal to, or greater than zero if s1 is found, respectively, to be
 *         less than, to match,  or  be greater than s2.
 */
int stricmp(const char *s1, const char *s2)
{
    register int c;
    SET_SEGV_LOCATION();

    if (!s1 || !*s1 || !s2 || !*s2) {
        return -1;
    }

    while ((c = tolower(*s1)) == tolower(*s2)) {
        if (c == 0)
            return 0;
        s1++;
        s2++;
    }
    if (c < tolower(*s2))
        return -1;
    return 1;
}
#endif

/*************************************************************************/

#if !defined(HAVE_STRNICMP)
/**
 * strnicmp:  Case-insensitive versions of strncmp()
 * only compiled on systems without it natively
 *
 * @param s1 is the first string to check
 * @param s2 is the second string to check
 *
 * @return It returns an integer less than, equal to, or greater than zero if s1 is found, respectively, to be
 *         less than, to match,  or  be greater than s2.
 */
int strnicmp(const char *s1, const char *s2, size_t len)
{
    register int c;
    SET_SEGV_LOCATION();

    if (!len)
        return 0;
    while ((c = tolower(*s1)) == tolower(*s2) && len > 0) {
        if (c == 0 || --len == 0)
            return 0;
        s1++;
        s2++;
    }
    if (c < tolower(*s2))
        return -1;
    return 1;
}
#endif

/*************************************************************************/

#if !HAVE_STRSPN
/**
 * strspn:  search a string for a set of characters
 * only compiled on systems without it natively
 *
 * @param s is the needle
 * @param accept is the haystack
 *
 * @return  returns the number of characters in the initial segment of s which consist only
 *          of characters from accept.
 *
 */
size_t strspn(const char *s, const char *accept)
{
    size_t i = 0;
    SET_SEGV_LOCATION();

    while (*s && strchr(accept, *s))
        ++i, ++s;
    return i;
}
#endif

/*************************************************************************/

#if !defined(HAVE_STRERROR)
# if HAVE_SYS_ERRLIST
extern char *sys_errlist[];
# endif

char *strerror(int errnum)
{
# if HAVE_SYS_ERRLIST
    return sys_errlist[errnum];
# else
    static char buf[20];
    ircsnprintf(buf, sizeof(buf), "Error %d", errnum);
    return buf;
# endif
}
#endif

/*************************************************************************/

#if !defined(HAVE_STRSIGNAL)
/**
 * strsignal:  string describing signal
 * only compiled on systems without it natively
 *
 * Windows only supports 6 signals:
 * SIGINT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGTERM
 *
 * @param signum is the signal number that you need the string for
 *
 * @return return string describing signal
 *
 */
char *strsignal(int signum)
{
    static char buf[32];
    switch (signum) {
#ifndef _WIN32
    case SIGHUP:
        strlcpy(buf, "Hangup", sizeof(buf));
        break;
    case SIGQUIT:
        strlcpy(buf, "Quit", sizeof(buf));
        break;
    case SIGKILL:
        strlcpy(buf, "Killed", sizeof(buf));
        break;
    case SIGUSR1:
        strlcpy(buf, "User signal 1", sizeof(buf));
        break;
    case SIGUSR2:
        strlcpy(buf, "User signal 2", sizeof(buf));
        break;
    case SIGPIPE:
        strlcpy(buf, "Broken pipe", sizeof(buf));
        break;
    case SIGALRM:
        strlcpy(buf, "Alarm clock", sizeof(buf));
        break;
    case SIGSTOP:
        strlcpy(buf, "Suspended (signal)", sizeof(buf));
        break;
    case SIGTSTP:
        strlcpy(buf, "Suspended", sizeof(buf));
        break;
    case SIGIO:
        strlcpy(buf, "I/O error", sizeof(buf));
        break;
#if defined(SIGIOT) && (!defined(SIGABRT) || SIGIOT != SIGABRT)
    case SIGIOT:
        strlcpy(buf, "IOT trap", sizeof(buf));
        break;
#endif
#ifdef SIGBUS
    case SIGBUS:
        strlcpy(buf, "Bus error", sizeof(buf));
        break;
#endif
#endif
    case SIGINT:
        strlcpy(buf, "Interrupt", sizeof(buf));
        break;
#ifdef SIGILL
    case SIGILL:
        strlcpy(buf, "Illegal instruction", sizeof(buf));
        break;
#endif
#ifdef SIGABRT
    case SIGABRT:
        strlcpy(buf, "Abort", sizeof(buf));
        break;
#endif
    case SIGFPE:
        strlcpy(buf, "Floating point exception", sizeof(buf));
        break;
    case SIGSEGV:
        strlcpy(buf, "Segmentation fault", sizeof(buf));
        break;
    case SIGTERM:
        strlcpy(buf, "Terminated", sizeof(buf));
        break;
    default:
        ircsnprintf(buf, sizeof(buf), "Signal %d\n", signum);
        break;
    }
    return buf;
}
#endif

/*************************************************************************/

unsigned char char_atribs[] = {
    /* 0-7 */
    CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
    /* 8-12 */
    CNTRL, CNTRL | SPACE, CNTRL | SPACE, CNTRL | SPACE, CNTRL | SPACE,
    /* 13-15 */
    CNTRL | SPACE, CNTRL, CNTRL,
    /* 16-23 */
    CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
    /* 24-31 */
    CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
    /* space */
    PRINT | SPACE,
    /* !"#$%&'( */
    PRINT, PRINT, PRINT, PRINT, PRINT, PRINT, PRINT, PRINT,
    /* )*+,-./ */
    PRINT, PRINT, PRINT, PRINT, PRINT, PRINT, PRINT,
    /* 0123 */
    PRINT | DIGIT, PRINT | DIGIT, PRINT | DIGIT, PRINT | DIGIT,
    /* 4567 */
    PRINT | DIGIT, PRINT | DIGIT, PRINT | DIGIT, PRINT | DIGIT,
    /* 89:; */
    PRINT | DIGIT, PRINT | DIGIT, PRINT, PRINT,
    /* <=>? */
    PRINT, PRINT, PRINT, PRINT,
    /* @ */
    PRINT,
    /* ABC */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* DEF */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* GHI */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* JKL */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* MNO */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* PQR */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* STU */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* VWX */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* YZ[ */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* \]^ */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* _` */
    PRINT, PRINT,
    /* abc */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* def */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* ghi */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* jkl */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* mno */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* pqr */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* stu */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* vwx */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* yz{ */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* \}~ */
    PRINT | ALPHA, PRINT | ALPHA, PRINT | ALPHA,
    /* del */
    0,
    /* 80-8f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 90-9f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* a0-af */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* b0-bf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* c0-cf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* d0-df */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* e0-ef */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* f0-ff */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*************************************************************************/

#if !defined(HAVE_STRLCAT)
size_t strlcat(char *dest, const char *src, size_t count)
{
    size_t dsize = strlen(dest);
    size_t len = strlen(src);
    size_t res = dsize + len;

    dest += dsize;
    count -= dsize;
    if (len >= count)
        len = count - 1;
    memcpy(dest, src, len);
    dest[len] = 0;
    return res;
}
#endif

/*************************************************************************/

#if !defined(HAVE_STRLCPY)
size_t strlcpy(char *dest, const char *src, size_t size)
{
    size_t ret = strlen(src);

    if (size) {
        size_t len = (ret >= size) ? size - 1 : ret;
        memcpy(dest, src, len);
        dest[len] = '\0';
    }
    return ret;
}
#endif

/*************************************************************************/

/**
 * Convert a string to lowercase
 *
 * @param ch is the string to convert
 * @return void - no returend value
 */
void strtolwr(char *ch)
{
    SET_SEGV_LOCATION();

    if (!BadPtr(ch)) {
        while (*ch) {
            *ch = tolower(*ch);
            ch++;
        }
    }
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

    SET_SEGV_LOCATION();

    nstr = (char *) str;
    while (*str) {
        *nstr = (char) *str;
        str++;
        nstr++;
    }

    return nstr;
}

/*************************************************************************/

/**
 * Replacement to internal strlen function
 *
 * @param str is the string to find the length of
 * @return size of the string.
 *
 */
size_t denora_strlen(const char *str)
{
    const char *s;

    if (!str || !*str) {
        return -1;
    }

    s = str;

    while (*s)
        s++;

    return (s - str);
}

/*************************************************************************/

char *sstrdup(const char *src)
{
    char *ret = NULL;

    if (src) {
        if ((ret = (char *) malloc(strlen(src) + 1))) {
            strcpy(ret, src);
        }
        if (!ret)
#ifndef _WIN32
            raise(SIGUSR1);
#else
            abort();
#endif
    } else {
        alog(LOG_DEBUG, langstr(ALOG_DEBUG_BAD_SSTRDUP));
        if (denora->debug) {
            do_backtrace(0);
        }
    }
    return ret;
}


/*************************************************************************/

/**
 * strndup, replacement so we can trap for "out of memory"
 * @param src is the text to copy
 * @param n is the size in bytes to copy
 * @return void
 */
char *sstrndup(const char *src, size_t n)
{
    char *ret = NULL;

    SET_SEGV_LOCATION();

    if (src) {
        if ((ret = (char *) malloc(strlen(src) + 1))) {
            strncpy(ret, src, n);
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

char **buildStringList(char *src, int *number)
{
    char *s;
    int i = 0;
    char **list = NULL;

    if (src) {
        s = strtok(src, " ");
        do {
            if (s) {
                i++;
                list = realloc(list, sizeof(char *) * i);
                list[i - 1] = sstrdup(s);
            }
        } while ((s = strtok(NULL, " ")));
    }
    *number = i;                /* always zero it, even if we have no setters */
    return list;
}

/*************************************************************************/

int dfprintf(FILE * ptr, const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = vfprintf(ptr, fmt, args);
    va_end(args);
    return len;
}
