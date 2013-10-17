/* Hacks for AIX
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

/* Some AIX boxes seem to have bogus includes that don't have these
 * prototypes. */
extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, size_t);
# if 0  /* These break on some AIX boxes (4.3.1 reported). */
extern int gettimeofday(struct timeval *, struct timezone *);
extern int socket(int, int, int);
extern int bind(int, struct sockaddr *, int);
extern int connect(int, struct sockaddr *, int);
extern int shutdown(int, int);
# endif
# undef FD_ZERO
# define FD_ZERO(p) memset((p), 0, sizeof(*(p)))
#undef int16
#undef int32
# define int16 builtin_int16
# define int32 builtin_int32




