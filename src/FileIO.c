/* File I/O functions - basically got tired of the defaults not having the
 * best way to trap for errors or be portable.
 *
 * (c) 2004-2012 Denora Team
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

DENORA_INLINE int FileExists(char *fileName)
{
    struct stat buf;
    int i;
    i = stat(fileName, &buf);
    return (i == 0 ? 1 : 0);
}

/*************************************************************************/

DENORA_INLINE int FileRename(char *fileName, char *newfilename)
{
    if (FileExists(fileName)) {
        if (rename(fileName, newfilename) != 0) {
            alog(LOG_DEBUG, "Rename of %s failed. [%d][%s]", fileName,
                 errno, strerror(errno));
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

/*************************************************************************/

DENORA_INLINE FILE *FileOpen(char *file, const char *mode)
{
    FILE *ptr;
#ifdef _WIN32
    errno_t err;
#endif

#ifdef _WIN32
    if ((err = fopen_s(&ptr, file, mode)) != 0) {
#else
    if ((ptr = fopen(file, mode)) == NULL) {
#endif
        alog(LOG_ERROR, "Error accessing file: %s [%d][%s]", file, errno,
             strerror(errno));
        return NULL;
    }
    return ptr;
}
