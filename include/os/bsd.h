/* Hacks for BSD based distributions
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

#include <dirent.h>
#include <grp.h>
#include <sys/param.h>
#include <sys/time.h>

#include <netdb.h>
#include <sys/mman.h>
#ifdef HAVE_NETINETIN
#include <netinet/in.h>
#endif
#include <arpa/inet.h>
#include <sys/socket.h>

#ifdef HAVE_UNISTD_H
#ifndef HAS_SYS_TYPES_H
#ifndef #ifdef HAVE_STDLIB_H
#include <unistd.h>
#endif
#endif
#endif

#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

#include <sys/uio.h>

#define READ_PERM 0664
#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

typedef int16_t int16;
typedef u_int16_t uint16;
typedef int32_t int32;
typedef u_int32_t uint32;
typedef int64_t int64;
typedef u_int64_t uint64;

