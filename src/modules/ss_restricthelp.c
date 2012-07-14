/*
 * ss_restricthelp module for Denora 1.4+
 * (c) Jobe (jobe@mdbnet.co.uk)
 */

#include "denora.h"
#define AUTHOR "Jobe"
#define VERSION "1.0.1"
#define MYNAME "ss_restricthelp"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int do_help(User * u, int argc, char **argv);

int DenoraInit(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
  Command *c;
  c = createCommand("HELP", do_help, is_oper, -1, -1, -1, -1);
  moduleAddCommand(STATSERV, c, MOD_HEAD);

  moduleAddAuthor(AUTHOR);
  moduleAddVersion(VERSION);
  return MOD_CONT;
}

void DenoraFini(void)
{
  alog(LOG_NORMAL, "Unloading ss_restricthelp%s", MODULE_EXT);
}

int do_help(__attribute__((unused))User * u, __attribute__((unused))int argc, __attribute__((unused))char **argv)
{
  return MOD_CONT;
}
