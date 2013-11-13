/* Hacks for Sun OS
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

/* Some SUN fixs */
/* Solaris specific code, types that do not exist in Solaris'
 * sys/types.h
 **/
#undef u_int8_t
#undef u_int16_t
#undef u_int32_t
#undef u_int_64_t
#define u_int8_t uint8_t
#define u_int16_t uint16_t
#define u_int32_t uint32_t
#define u_int64_t uint64_t

#ifndef INADDR_NONE
#define INADDR_NONE (-1)
#endif

#ifndef HAVE_GETHOSTBYNAME
#include <netdb.h>
#define HAVE_GETHOSTBYNAME 1
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef HAVE_INET_NTOP
#define HAVE_INET_NTOP 1
#endif

#ifndef HAVE_INET_PTON
#define HAVE_INET_PTON 1
#endif



