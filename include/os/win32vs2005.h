/* Hacks for Windows Visual Studio 2005
 *
 * (C) 2004-2007 Denora Team
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

#if defined (_MSC_VER)
#pragma warning(disable:4003)
#endif /* _MSC_VER */

#include <sys/timeb.h>
#include <direct.h>
#include <io.h>
#include <winsock2.h>
#include <windows.h>

#define popen			_popen
#define pclose			_pclose
#define ftruncate		_chsize
#define PATH_MAX 		MAX_PATH
#define MAXPATHLEN 		MAX_PATH
#define bzero(buf, size)	memset(buf, 0, size)
#define strcasecmp		stricmp
#define sleep(x)		Sleep(x*1000)

#define snprintf sprintf_s
#define vsnprintf vsprintf_s

char *win32_vs2005_next_token;

#undef strtok
#define strtok(a, b) strtok_s(a, b, &win32_vs2005_next_token)

/*
  Message: 'The POSIX name for this item is deprecated. Instead, 
  use the ISO C++ conformant
*/
#undef stricmp
#define stricmp _stricmp

#undef strnicmp
#define strnicmp _strnicmp

#undef isatty
#define isatty _isatty

#undef close
#define close(f) _close(f)

#undef fileno
#define fileno(f) _fileno(f)

#undef unlink
#define unlink(f) DeleteFile(f)

#undef chmod
#define chmod(f, p) _chmod(f, p)

#undef chdir
#define chdir(d) _chdir(d)

#undef getpid
#define getpid _getpid

#define READ_PERM _S_IWRITE
#define FILE_READ "rb"
#define FILE_WRITE "wb"
#define FILE_APPEND "a"


/* because MS va_start can come in two flavors */
#define ANSI

#define ENOPROTOOPT WSAENOPROTOOPT

#ifndef SM_MEDIACENTER
#define SM_MEDIACENTER 87
#endif

#ifndef SM_TABLETPC
#define SM_TABLETPC 86
#endif

#ifndef SM_SERVERR2
#define SM_SERVERR2 89
#endif

#ifndef SM_STARTER
#define SM_STARTER 88
#endif

#ifndef VER_SUITE_COMPUTE_SERVER
#define VER_SUITE_COMPUTE_SERVER 0x00004000
#endif

#ifndef VER_SUITE_EMBEDDEDNT
#define VER_SUITE_EMBEDDEDNT 0x00000040
#endif

typedef   signed __int16  int16;
typedef   signed __int16  int16_t;
typedef unsigned __int16 uint16;
typedef unsigned __int16 u_int16_t;
typedef   signed __int32  int32;
typedef   signed __int32  int32_t;
typedef unsigned __int32 uint32;
typedef unsigned __int32 u_int32_t;
typedef unsigned __int8 u_int8_t;


