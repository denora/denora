/*
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

/* Needed due to Windows lack of a decent interpreter */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

long version_major, version_minor, version_patch, version_build, build;
char *version_extra;
char version[1024];
char version_dotted[1024];

void load_ctrl(FILE *);
long get_value(char *);
char *get_value_str(char *);
char *strip(char *);
void parse_version(FILE *);
void write_version(FILE *);
void parse_line(FILE *, char *);

#ifdef MSVS2005
/* because MS va_start can come in two flavors */
#define ANSI
#define snprintf sprintf_s

/* weeeee hack to make strtok() work as it has */
char *next_token;
#undef strtok
#define strtok(a, b) strtok_s(a, b, &next_token)

#else
#define snprintf _snprintf
#endif

#if !defined(HAVE_STRLCPY)
size_t strlcpy(char *dest, const char *src, size_t size)
#endif

char *denoraStrDup(const char *src);

/*************************************************************************/

char *denoraStrDup(const char *src)
{
    char *ret = NULL;
    if (src) {
        if ((ret = (char *) malloc(strlen(src) + 1))) {
            strlcpy(ret, src, sizeof(ret));
        }
    }
    return ret;
}

/*************************************************************************/

int main()
{
    FILE *fd;
#ifdef MSVS2005
    errno_t err;
#endif

#ifndef MSVS2005
    fd = fopen("version.log", "r");
    if (!fd) {
        fprintf(stderr, "Error: Unable to find control file: version.log\n");
        exit(0);
    }
#else
    if((err  = fopen_s(&fd, "version.log", "r")) !=0 ) {
      fprintf(stderr, "Error: Unable to find control file: version.log\n");
      exit(0);
    }
#endif

    load_ctrl(fd);
    fclose(fd);

    snprintf(version, 1024, "%d.%d.%d%s (%d)", version_major, version_minor,
              version_patch, (version_extra ? version_extra : ""), version_build);

    snprintf(version_dotted, 1024, "%d.%d.%d%s.%d", version_major, version_minor,
              version_patch, (version_extra ? version_extra : ""), version_build);

#ifndef MSVS2005
    fd = fopen("version.h", "r");
    if (!fd) {
#else
    if((err  = fopen_s(&fd, "version.h", "r")) == 0 ) {
#endif
        parse_version(fd);
        fclose(fd);
    } else {
        build = 1;
    }

#ifndef MSVS2005
    fd = fopen("version.h", "w");
    if (!fd) {
#else
    if((err  = fopen_s(&fd, "version.h", "w")) == 0 ) {
#endif
        write_version(fd);
        fclose(fd);
    }
    free(version_extra);
}

/*************************************************************************/

void load_ctrl(FILE * fd)
{
    char buf[512];
    char *var;
    char *tmp;
    
    while (fgets(buf, 511, fd)) {
        strip(buf);

        var = strtok(buf, "=");
        if (!var) {
            continue;
        }
        
        if (!strcmp(var, "VERSION_MAJOR")) {
            version_major = get_value(strtok(NULL, ""));
        } else if (!strcmp(var, "VERSION_MINOR")) {
            version_minor = get_value(strtok(NULL, ""));
        } else if (!strcmp(var, "VERSION_PATCH")) {
            version_patch = get_value(strtok(NULL, ""));
        } else if (!strcmp(var, "VERSION_BUILD")) {
            version_build = get_value(strtok(NULL, ""));
        } else if (!strcmp(var, "VERSION_EXTRA")) {
            tmp = get_value_str(strtok(NULL, ""));
            if (tmp) {
                version_extra = denoraStrDup(tmp);
            }
        }

    }
}

/*************************************************************************/

char *strip(char *str)
{
    char *c;
    if ((c = strchr(str, '\n')))
        *c = 0;
    if ((c = strchr(str, '\r')))
        *c = 0;
    return str;
}

/*************************************************************************/

long get_value(char *string)
{
    return atol(get_value_str(string));
}

/*************************************************************************/

char *get_value_str(char *string)
{
    int len;

    if (*string == '"')
        string++;

    len = strlen(string);

    if (string[len - 1] == '"')
        string[len - 1] = 0;
    if (!*string)
        return NULL;
    return string;
}

/*************************************************************************/

void parse_version(FILE * fd)
{
    char buf[1024];

    while (fgets(buf, 1023, fd)) {
        char *para1;

        strip(buf);
        para1 = strtok(buf, " \t");

        if (!para1)
            continue;

        if (!strcmp(para1, "#define")) {
            char *para2 = strtok(NULL, " \t");

            if (!para2)
                continue;

            if (!strcmp(para2, "BUILD")) {
                char *value = strtok(NULL, "");
                build = get_value(value);
                build++;
                return;
            }
        }
    }
    build = 1;
}

/*************************************************************************/

void write_version(FILE * fd)
{
    FILE *fdin;
    char buf[1024];
    short until_eof = 0;

#ifndef MSVS2005
    fdin = fopen("include\\version.sh", "r");
#else
    fopen_s(&fdin, "include\\version.sh", "r");
#endif

    while (fgets(buf, 1023, fdin)) {
        strip(buf);

        if (until_eof) {
            if (!strcmp(buf, "EOF")) {
                break;
            } else {
                parse_line(fd, buf);
            }
        }
        if (!strcmp(buf, "cat >version.h <<EOF")) {
            until_eof = 1;
        }
    }

}

/*************************************************************************/

void parse_line(FILE * fd, char *line)
{
    char *c;
    for (c = line; *c; c++) {
        /* It's a variable, find out which */
        if (*c == '$') {
            char *var, *varbegin;

            if (*(c + 1)) {
                c++;
            } else {
                continue;
            }
            for (var = varbegin = c; var; var++) {
                if (!isalnum(*var) && *var != '_') {
                    break;
                }
            }
            if (var != varbegin) {
                char tmp = *var;

                *var = 0;
                if (!strcmp(varbegin, "VERSION_MAJOR")) {
                    fprintf(fd, "%d", version_major);
                } else if (!strcmp(varbegin, "VERSION_MINOR")) {
                    fprintf(fd, "%d", version_minor);
                } else if (!strcmp(varbegin, "VERSION_PATCH")) {
                    fprintf(fd, "%d", version_patch);
                } else if (!strcmp(varbegin, "VERSION_EXTRA")) {
                    if (version_extra) {
                        fprintf(fd, "\"%s\"", version_extra);
                    }
                } else if (!strcmp(varbegin, "VERSION_BUILD")) {
                    fprintf(fd, "%d", version_build);
                } else if (!strcmp(varbegin, "BUILD")) {
                    fprintf(fd, "%d\"", build);
                } else if (!strcmp(varbegin, "VERSION")) {
                    fprintf(fd, "%s\"", version);
                } else if (!strcmp(varbegin, "VERSIONDOTTED")) {
                    fprintf(fd, "%s\"", version_dotted);
                }
            }
            c = var;
        } else {
            fputc(*c, fd);
        }
    }
    fprintf(fd, "\n");
}

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
