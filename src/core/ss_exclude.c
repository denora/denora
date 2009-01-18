/* StatServ core functions
 *
 * (c) 2004-2009 Denora Team
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

static int do_exclude(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * RFC: defination of a valid nick
 * nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
 * letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
 * digit      =  %x30-39                 ; 0-9
 * special    =  %x5B-60 / %x7B-7D       ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
 **/
#define isvalidnick(c) ( isalnum(c) || ((c) >='\x5B' && (c) <='\x60') || ((c) >='\x7B' && (c) <='\x7D') || (c)=='-' )


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

    c = createCommand("EXCLUDE", do_exclude, is_stats_admin, -1, -1, -1,
                      STAT_HELP_EXCLUDE);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

static int do_exclude(User * u, int ac, char **av)
{
    Exclude *e;
    char *tmp = NULL;
    char *s = NULL;
    int count = 0, from = 0, to = 0;
    int nnicks = 0, i;
    int disp = 1;
    char *name;
    char *ch = NULL;
    User *u2;

    if (ac < 1) {
        notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
        return MOD_CONT;
    }

    if (!stricmp(av[0], "ADD")) {
        if (ac < 2) {
            notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
            return MOD_CONT;
        }
        if (strlen(av[1]) > NICKMAX) {
            notice(s_StatServ, u->nick, "Invalid nick length");
            return MOD_CONT;
        }
        if (isdigit(av[1][0]) || av[1][0] == '-') {
            notice(s_StatServ, u->nick, "Invalid nick");
            return MOD_CONT;
        }
        for (ch = av[1]; *ch && (ch - av[1]) < NICKMAX; ch++) {
            if (!isvalidnick(*ch)) {
                notice(s_StatServ, u->nick, "Invalid nick");
                return MOD_CONT;
            }
        }
        e = find_exclude(av[1], NULL);
        if (!e) {
            make_exclude(av[1]);
            notice_lang(s_StatServ, u, STAT_EXCLUDE_ADDED, av[1]);
            name = rdb_escape(av[1]);
            u2 = user_find(av[1]);
            rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname=\'%s\'",
                      UStatsTable, u2 ? u2->sgroup : name);
            rdb_query(QUERY_LOW,
                      "UPDATE `%s` SET `ignore`=\'Y\' WHERE `uname`=\'%s\'",
                      AliasesTable, u2 ? u2->sgroup : name);
            free(name);
        } else {
            notice_lang(s_StatServ, u, STAT_EXCLUDE_ALREADY, av[1]);
        }
    } else if (!stricmp(av[0], "DEL")) {
        if (ac < 2) {
            notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
            return MOD_CONT;
        }
        e = find_exclude(av[1], NULL);
        if (e) {
            del_exclude(e);
            u->cstats = 0;
            notice_lang(s_StatServ, u, STAT_EXCLUDE_DELETED, av[1]);
            name = rdb_escape(av[1]);
            u2 = user_find(av[1]);
            rdb_query(QUERY_LOW,
                      "UPDATE `%s` SET `ignore`=\'N\' WHERE `uname`=\'%s\'",
                      AliasesTable, u2 ? u2->sgroup : name);
            for (i = 0; i < 4; i++) {
                rdb_query
                    (QUERY_LOW,
                     "INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'global\', type=%i;",
                     UStatsTable, u2 ? u2->sgroup : name, i);
            }
            free(name);
        } else {
            notice_lang(s_StatServ, u, STAT_EXCLUDE_NOT_FOUND, av[1]);
        }
    } else if (!stricmp(av[0], "LIST")) {
        if (ac >= 2) {
            if (*av[1] == '#') {
                tmp = myStrGetOnlyToken((av[1] + 1), '-', 0);   /* Read FROM out */
                if (!tmp) {
                    return MOD_CONT;
                }
                for (s = tmp; *s; s++) {
                    if (!isdigit(*s)) {
                        free(tmp);
                        return MOD_CONT;
                    }
                }
                from = atoi(tmp);
                free(tmp);
                tmp = myStrGetTokenRemainder(av[1], '-', 1);    /* Read TO out */
                if (!tmp) {
                    return MOD_CONT;
                }
                for (s = tmp; *s; s++) {
                    if (!isdigit(*s)) {
                        free(tmp);
                        return MOD_CONT;
                    }
                }
                to = atoi(tmp);
                free(tmp);
            }
        }

        notice_lang(s_StatServ, u, STAT_EXCLUDE_LIST_HEADER);

        for (i = 0; i < 1024; i++) {
            for (e = exlists[i]; e; e = e->next) {
                if ((count + 1 >= from) && (count + 1 <= to)) {
                    notice(s_StatServ, u->nick, "%d %s", disp++, e->name);
                } else if (((from == 0) && (to == 0)) && (++nnicks <= 50)) {
                    notice(s_StatServ, u->nick, "%d %s", disp++, e->name);
                }
                count++;
            }
        }
    } else {
        notice_lang(s_StatServ, u, STAT_EXCLUDE_SYNTAX);
        return MOD_CONT;
    }

    return MOD_CONT;
}
