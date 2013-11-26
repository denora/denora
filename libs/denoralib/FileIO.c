/* File I/O functions - basically got tired of the defaults not having the
 * best way to trap for errors or be portable.
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

int FileExists(char *fileName)
{
	struct stat buf;
	int i;

	i = stat(fileName, &buf);
	if (i)
	{
		DenoraLib_SetLastError(errno, strerror(errno));
	}
	return (i == 0 ? 1 : 0);
}

/*************************************************************************/

int FileRename(char *fileName, char *newfilename)
{
	if (FileExists(fileName))
	{
		if (rename(fileName, newfilename) != 0)
		{
			DenoraLib_SetLastError(errno, strerror(errno));
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/

FILE *FileOpen(char *file, const char *mode)
{
	FILE *ptr;
#ifdef _WIN32
	errno_t err;
#endif

#ifdef _WIN32
	if ((err = fopen_s(&ptr, file, mode)) != 0)
	{
#else
	if ((ptr = fopen(file, mode)) == NULL)
	{
#endif
		DenoraLib_SetLastError(errno, strerror(errno));
		return NULL;
	}
	return ptr;
}
