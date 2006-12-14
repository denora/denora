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
 * $Id: ss_stats.c 580 2006-03-17 19:39:34Z trystan $
 *
 */
/*************************************************************************/

#include "denora.h"

static int do_stats(User * u, int ac, char **av);
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
    moduleAddVersion("$Id: ss_stats.c 580 2006-03-17 19:39:34Z trystan $");
    moduleSetType(CORE);

    c = createCommand("STATS", do_stats, is_oper, -1, -1, -1,
                      STAT_HELP_STATS);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/* STATS command. */

static int do_stats(User * u, int ac, char **av)
{
    char *what = NULL;

    if (ac >= 1) {
        what = av[0];
    }

    SET_SEGV_LOCATION();

    if (!what) {
        notice_lang(s_StatServ, u, STAT_STATS_SERVER_COUNT, NetworkName,
                    stats->servers, stats->servers_max);
        notice_user(s_StatServ, u, " ");
        notice_lang(s_StatServ, u, STAT_STATS_CHAN_COUNT, NetworkName,
                    stats->chans, stats->chans_max);
        notice_user(s_StatServ, u, " ");
        notice_lang(s_StatServ, u, STAT_STATS_USER_COUNT, NetworkName,
                    stats->users, stats->users_max);
        if (StatsPage) {
            notice_lang(s_StatServ, u, STAT_STATS_PAGE, StatsPage);
        }
    } else if (!stricmp(what, "SERVERS")) {
        notice_lang(s_StatServ, u, STAT_STATS_SERVER_COUNT, NetworkName,
                    stats->servers, stats->servers_max);
    } else if (!stricmp(what, "CHANNELS")) {
        notice_lang(s_StatServ, u, STAT_STATS_CHAN_COUNT, NetworkName,
                    stats->chans, stats->chans_max);
    } else if (!stricmp(what, "USERS")) {
        notice_lang(s_StatServ, u, STAT_STATS_USER_COUNT, NetworkName,
                    stats->users, stats->users_max);
    } else {
        syntax_error(s_StatServ, u, "STATS", STAT_STATS_SYNTAX);
    }

    return MOD_CONT;
}
