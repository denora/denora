/* Needed due to Windows lack of a decent interpreter
 *
 * (c) 2004-2011 Denora Team
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

#include <string.h>
#include <stdio.h>

char *strip(char *str)
{
    char *c;
    if ((c = strchr(str, '\n')))
        *c = 0;
    if ((c = strchr(str, '\r')))
        *c = 0;
    return str;
}

int main(int argc, char *argv[])
{
    FILE *fd;
    FILE *fdout;
    char buf[1024];
    int i = 0;

#ifdef _WIN32
    errno_t err;
#endif

    if (argc < 2)
        exit(1);

    /* Build the index file */
    if (!strcmp(argv[1], "index")) {
#ifndef _WIN32
        fd = fopen("en_us.l", "rb");
        fdout = fopen("index", "wb");
        if (!fd || !fdout) {
            exit(2);
        }
#else
        if ((err = fopen_s(&fd, "en_us.l", "rb")) != 0) {
            exit(2);
        }
        if ((err = fopen_s(&fdout, "index", "wb")) != 0) {
            exit(2);
        }
#endif
        while (fgets(buf, 1023, fd)) {
            if (isupper(*buf))
                fprintf(fdout, "%s", buf);
        }
        fclose(fd);
        fclose(fdout);
    }
    /* Build the language.h file */
    else if (!strcmp(argv[1], "language.h")) {
#ifndef _WIN32
        fd = fopen("index", "r");
        fdout = fopen("language.h", "wb");
        if (!fd || !fdout) {
            exit(2);
        }
#else
        if ((err = fopen_s(&fd, "index", "r")) != 0) {
            exit(2);
        }
        if ((err = fopen_s(&fdout, "language.h", "w")) != 0) {
            exit(2);
        }
#endif
        fprintf(stderr, "Generating language.h... ");

        while (fgets(buf, 1023, fd))
            fprintf(fdout, "#define %-32s %d\n", strip(buf), i++);

        fprintf(fdout, "#define NUM_STRINGS %d\n", i);
        fprintf(stderr, "%d strings\n", i);
        fclose(fd);
        fclose(fdout);
    }
    return 0;

}
