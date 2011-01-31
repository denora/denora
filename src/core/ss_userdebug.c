/* StatServ core functions
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
/*************************************************************************/

#include "denora.h"

static int do_userdebug(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    Command *c;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id$");
    moduleSetType(CORE);

    c = createCommand("USERDEBUG", do_userdebug, is_stats_admin, -1, -1,
                      -1, STAT_HELP_USERDEBUG);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

static int do_userdebug(User * u, int ac, char **av)
{
    User *u2;
    struct tm tm;
    char buf[BUFSIZE];

    notice(s_StatServ, u->nick,
           "============== USER DEBUG ==============");

    if (ac < 1) {
        notice(s_StatServ, u->nick, "No nick given");
        return MOD_CONT;
    }

    if ((u2 = finduser(av[0]))) {
        notice(s_StatServ, u->nick, "User: %s!%s@%s", u2->nick,
               u2->username, u2->host);
        if (ircd->vident && ircd->vhost) {
            if (u2->vident) {
                notice(s_StatServ, u->nick, "Vhost: %s@%s", u2->vident,
                       u2->vhost);
            } else {
                notice(s_StatServ, u->nick, "Vhost: %s", u2->vhost);
            }
        } else {
            notice(s_StatServ, u->nick, "Vhost: %s", u2->vhost);
        }

        notice(s_StatServ, u->nick, "Host Resolves to %s",
               host_resolve(u2->host));

#ifdef _WIN32
        localtime_s(&tm, &u2->my_signon);
#else
        tm = *localtime(&u2->my_signon);
#endif
        strftime(buf, sizeof(buf) - 1, MYTIME, &tm);
        notice(s_StatServ, u->nick, "Sign On At : %s", buf);

#ifdef _WIN32
        localtime_s(&tm, &u2->timestamp);
#else
        tm = *localtime(&u2->timestamp);
#endif
        strftime(buf, sizeof(buf) - 1, MYTIME, &tm);
        notice(s_StatServ, u->nick, "Time Stamp of Nick :  %s", buf);

        notice(s_StatServ, u->nick, "Server : %s", u2->server->name);

    } else {
        notice(s_StatServ, u->nick,
               "%s user was not found in the user struct", av[0]);
    }

    return MOD_CONT;
}
