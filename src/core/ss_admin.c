/* StatServ core functions
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
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

static int do_admin(User * u, int ac, char **av);
static int do_chgpass(User * u, int ac, char **av);
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
    moduleAddVersion("$Id$");
    moduleSetType(CORE);

    c = createCommand("ADMIN", do_admin, is_stats_admin, -1, -1, -1,
                      STAT_HELP_ADMIN);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    c = createCommand("CHGPASS", do_chgpass, is_stats_admin, -1, -1, -1,
                      STAT_HELP_CHGPASS);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

static int do_chgpass(User * u, int ac, char **av)
{
    Dadmin *a;

    if (denora->protocoldebug) {
        protocol_debug(NULL, ac, av);
    }

    if (ac < 2) {
        syntax_error(s_StatServ, u, "CHGPASS", STAT_CHGPASS_SYNTAX);
        return MOD_CONT;
    }
    a = find_admin_byname(av[1]);
    if (a) {
        if (a->configfile) {
            notice_lang(s_StatServ, u, STAT_CHGPASS_CONFIG, av[1]);
            return MOD_CONT;
        }
        free(a->passwd);
        a->passwd = sstrdup(MakePassword(av[2]));
        notice_lang(s_StatServ, u, STAT_CHGPASS_OK, av[1]);
    } else {
        notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
    }
    return MOD_CONT;
}

/*
  0 1 add | del
  1 2 user
  2 3 pass
  3 4 host
  4 5 language
*/
static int do_admin(User * u, int ac, char **av)
{
    Dadmin *a;
    User *u2;

    if (denora->protocoldebug) {
        protocol_debug(NULL, ac, av);
    }

    if (ac < 2) {
        syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
        return MOD_CONT;
    }

    if (!stricmp(av[0], "ADD")) {
        if (ac < 3) {
            syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
            return MOD_CONT;
        }
        a = find_admin_byname(av[1]);
        if (a) {
            notice_lang(s_StatServ, u, STAT_ADMIN_ALREADY, av[1]);
        } else {
            a = make_admin(av[1]);
            if (ac <= 3) {
                a->hosts[0] = sstrdup("*@*");
                a->language = StatsLanguage;
            } else if (ac <= 4) {
                a->language = StatsLanguage;
            } else {
                a->hosts[0] = sstrdup(av[3]);
                a->language = atoi(av[4]);
                if (a->language < 1 || a->language > NUM_LANGS) {
                    a->language = StatsLanguage;
                }
            }
            a->passwd = sstrdup(MakePassword(av[2]));
            notice_lang(s_StatServ, u, STAT_ADMIN_CREATED, av[1]);
        }
    } else if (!stricmp(av[0], "DEL")) {
        a = find_admin_byname(av[1]);
        if (a) {
            if (a->configfile) {
                notice_lang(s_StatServ, u, STAT_ADMIN_DELETE_CONFIG,
                            av[1]);
                return MOD_CONT;
            }
            free_admin(a);
            u2 = user_find(av[1]);
            if (u2) {
                u2->admin = 0;
            }
            notice_lang(s_StatServ, u, STAT_ADMIN_DELETED, av[1]);
        } else {
            notice_lang(s_StatServ, u, STAT_ADMIN_NOTADMIN, av[1]);
        }
    } else {
        syntax_error(s_StatServ, u, "ADMIN", STAT_ADMIN_SYNTAX);
    }
    return MOD_CONT;
}
