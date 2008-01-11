/*
 *
 * © 2004-2008 Denora Team
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
#define AUTHOR "Trystan"
#define VERSION "1.0"

int m_do_uptime(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
    Command *c;
    c = createCommand("UPTIME", m_do_uptime, is_oper, -1, -1, -1, -1);
    moduleAddCommand(STATSERV, c, MOD_HEAD);

    if (denora->debug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor(AUTHOR);
    moduleAddVersion(VERSION);
    return MOD_CONT;
}

void DenoraFini(void)
{
    alog(LOG_NORMAL, "Unloading ss_uptime.%s", MODULE_EXT);
}

int m_do_uptime(User * u, int ac, char **av)
{
    time_t uptime = time(NULL) - denora->start_time;
    int days = uptime / 86400, hours = (uptime / 3600) % 24,
        mins = (uptime / 60) % 60, secs = uptime % 60;
    if (denora->protocoldebug) {
        protocol_debug(NULL, ac, av);
    }
    if (days > 1) {
        notice_lang(s_StatServ, u, STATS_UPTIME_DHMS, days, hours, mins,
                    secs);
    } else if (days == 1) {
        notice_lang(s_StatServ, u, STATS_UPTIME_1DHMS,
                    days, hours, mins, secs);
    } else {
        if (hours > 1) {
            if (mins != 1) {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_HMS,
                                hours, mins, secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_HM1S,
                                hours, mins, secs);
                }
            } else {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_H1MS,
                                hours, mins, secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_H1M1S,
                                hours, mins, secs);
                }
            }
        } else if (hours == 1) {
            if (mins != 1) {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1HMS,
                                hours, mins, secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1HM1S,
                                hours, mins, secs);
                }
            } else {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1H1MS,
                                hours, mins, secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1H1M1S,
                                hours, mins, secs);
                }
            }
        } else {
            if (mins != 1) {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_MS, mins,
                                secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_M1S, mins,
                                secs);
                }
            } else {
                if (secs != 1) {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1MS, mins,
                                secs);
                } else {
                    notice_lang(s_StatServ, u, STATS_UPTIME_1M1S, mins,
                                secs);
                }
            }
        }
    }

    return MOD_STOP;
}
