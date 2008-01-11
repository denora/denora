/* StatServ core functions
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
/*************************************************************************/

#include "denora.h"

void xml_export_channels(char *file);
void xml_export_users(char *file);
void xml_export_tld(char *file);
void xml_export_all(char *file);
void xml_export_stats(char *file);
void xml_export_servers(char *file);
void xml_export_ctcp(char *file);

static int do_export(User * u, int ac, char **av);
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

    c = createCommand("EXPORT", do_export, is_stats_admin, -1, -1, -1,
                      STAT_HELP_EXPORT);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

static int do_export(User * u, int ac, char **av)
{
    char *filename = NULL;

    if (ac < 1) {
        syntax_error(s_StatServ, u, "EXPORT", STATS_EXPORT_SYNTAX);
        return MOD_CONT;
    }
    if (!stricmp("channels", av[0])) {
        filename = sstrdup("channels.xml");
        xml_export_channels(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_CHANNELS, "channels.xml");
    } else if (!stricmp("users", av[0])) {
        filename = sstrdup("users.xml");
        xml_export_users(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_USERS, "users.xml");
    } else if (!stricmp("tld", av[0])) {
        filename = sstrdup("tld.xml");
        xml_export_tld(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_TLD, "tld.xml");
    } else if (!stricmp("servers", av[0])) {
        filename = sstrdup("servers.xml");
        xml_export_servers(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_SERVERS, "servers.xml");
    } else if (!stricmp("stats", av[0])) {
        filename = sstrdup("stats.xml");
        xml_export_stats(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_STATS, "stats.xml");
    } else if (!stricmp("all", av[0])) {
        filename = sstrdup("denora.xml");
        xml_export_all(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_ALL, "denora.xml");
    } else if (!stricmp("ctcp", av[0])) {
        filename = sstrdup("ctcp.xml");
        xml_export_ctcp(filename);
        notice_lang(s_StatServ, u, STATS_EXPORT_ALL, "ctcp.xml");
    } else {
        syntax_error(s_StatServ, u, "EXPORT", STATS_EXPORT_SYNTAX);
    }
    free(filename);
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Export channel struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_channels(char *file)
{
    FILE *ptr;
    Channel *c, *next;
    int i;
    char buf[BUFSIZE];
    struct c_userlist *cu;
    int32 count;
    int ping = 0;
    int counttoping = 1;
    char *temp;
    char **bans;

    *buf = '\0';

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);

        xml_write_block_top(ptr, "channels");
        c = firstchan();
        while (c) {
            next = nextchan();
            if (ping) {
                denora_cmd_pong(ServerName, ServerName);
                ping = 0;
            }
            xml_write_block_top(ptr, "chans");
            xml_write_tag(ptr, "name", c->name);

            xml_write_tag(ptr, "topic", c->topic);
            xml_write_tag(ptr, "topicsetter", c->topic_setter);
            xml_write_tag_int(ptr, "creation_time",
                              (long int) c->creation_time);
            xml_write_tag_int(ptr, "topic_time", (long int) c->topic_time);
            xml_write_tag_int(ptr, "topic_count", c->stats->topic_count);
            xml_write_tag_int(ptr, "limit", c->limit);
            xml_write_tag_int(ptr, "usercount",
                              (long int) c->stats->usercount);
            xml_write_tag_int(ptr, "maxusercount",
                              (long int) c->stats->maxusercount);
            xml_write_tag_int(ptr, "kickcount",
                              (long int) c->stats->kickcount);
            xml_write_tag_int(ptr, "joincounter",
                              (long int) c->stats->joincounter);
            temp = chan_get_modes(c, 1);
            xml_write_tag(ptr, "mode", temp);
            free(temp);

            if (c->limit) {
                xml_write_tag_int(ptr, "limit", 1);
            }

            if (c->key) {
                xml_write_tag_int(ptr, "key", 1);
            }

            if (ircd->Lmode && c->redirect) {
                xml_write_tag(ptr, "redirect", c->redirect);
            }
            if (ircd->fmode && c->flood) {
                xml_write_tag(ptr, "flood", c->flood);
            }

            xml_write_block_top(ptr, "inchannel");
            for (cu = c->users; cu; cu = cu->next) {
                xml_write_tag(ptr, "user", cu->user->nick);
            }
            xml_write_block_bottom(ptr, "inchannel");

            if (c->bancount) {
                xml_write_tag_int(ptr, "bancount", c->bancount);
                xml_write_block_top(ptr, "bans");
                count = c->bancount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->bans[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "ban", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "bans");
            }

            if (ircd->except && c->exceptcount) {
                xml_write_tag_int(ptr, "exceptcount", c->exceptcount);
                xml_write_block_top(ptr, "exceptions");
                count = c->exceptcount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->excepts[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "except", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "exceptions");
            }

            if (ircd->invitemode && c->invitecount) {
                xml_write_tag_int(ptr, "invitecount", c->invitecount);
                xml_write_block_top(ptr, "invites");
                count = c->invitecount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->invite[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "invite", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "invites");
            }


            xml_write_block_bottom(ptr, "chans");
            c = next;
            if (counttoping == 10) {
                ping = 1;
                counttoping = 1;
            } else {
                counttoping++;
            }
        }
        xml_write_block_bottom(ptr, "channels");
        xml_write_footer(ptr);
    }
}

/*************************************************************************/

/**
 * Export tld struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_tld(char *file)
{
    FILE *ptr;
    TLD *t;
    lnode_t *tn;

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);
        xml_write_block_top(ptr, "tld");
        list_sort(Thead, sortusers);
        tn = list_first(Thead);
        while (tn) {
            t = lnode_get(tn);
            denora_cmd_pong(ServerName, ServerName);
            xml_write_block_top(ptr, "domain");
            xml_write_tag(ptr, "countrycode", t->countrycode);
            xml_write_tag(ptr, "country", t->country);
            xml_write_tag_int(ptr, "current", t->count);
            xml_write_tag_int(ptr, "overall", t->overall);
            xml_write_block_bottom(ptr, "domain");
            tn = list_next(Thead, tn);
        }
        xml_write_block_bottom(ptr, "tld");
        xml_write_footer(ptr);
    }
}

/*************************************************************************/

/**
 * Export ctcp struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_ctcp(char *file)
{
    FILE *ptr;
    CTCPVerStats *c;
    lnode_t *tn;

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);

        xml_write_block_top(ptr, "ctcp");

        tn = list_first(CTCPhead);
        while (tn) {
            c = lnode_get(tn);
            xml_write_block_top(ptr, "client");
            xml_write_tag(ptr, "version", c->version);
            xml_write_tag_int(ptr, "current", c->count);
            xml_write_tag_int(ptr, "overall", c->overall);
            xml_write_block_bottom(ptr, "client");
            tn = list_next(CTCPhead, tn);
        }
        xml_write_block_bottom(ptr, "ctcp");
        xml_write_footer(ptr);
    }
}

/*************************************************************************/

/**
 * Export server struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_servers(char *file)
{
    FILE *ptr;
    Server *s, *next;

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);

        xml_write_block_top(ptr, "servers");
        s = servlist->links;
        while (s) {
            next = s->next;
            if (HideUline && s->uline) {
                continue;
            }
            denora_cmd_pong(ServerName, ServerName);
            xml_write_block_top(ptr, "server");
            xml_write_tag(ptr, "name", s->name);
            xml_write_tag_int(ptr, "hops", s->hops);
            xml_write_tag(ptr, "desc", s->desc);
            xml_write_tag_int(ptr, "flags", s->flags);

            if (s->flags & SERVER_ISME) {
                xml_write_tag_int(ptr, "FLAG_ISME", 1);
            } else {
                xml_write_tag_int(ptr, "FLAG_ISME", 0);
            }
            if (s->flags & SERVER_JUPED) {
                xml_write_tag_int(ptr, "FLAG_JUPED", 1);
            } else {
                xml_write_tag_int(ptr, "FLAG_JUPED", 0);
            }
            xml_write_tag_int(ptr, "synced", s->sync);
            if (s->suid) {
                xml_write_tag(ptr, "suid", s->suid);
            }
            if (s->version) {
                xml_write_tag(ptr, "version", s->version);
            }
            if (s->uptime) {
                xml_write_tag_int(ptr, "uptime", s->uptime);
            }
            if (s->uplink) {
                xml_write_tag(ptr, "uplink", s->uplink->name);
            }
            xml_write_tag_int(ptr, "uline", s->uline);

            xml_write_block_bottom(ptr, "server");
            s = next;
        }
        xml_write_block_bottom(ptr, "servers");
        xml_write_footer(ptr);
    }
}

/*************************************************************************/

/**
 * Export user struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_users(char *file)
{
    FILE *ptr;
    User *u, *next;
    int i;
    char buf[BUFSIZE];
    struct u_chanlist *uc;

    *buf = '\0';

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);

        xml_write_block_top(ptr, "users");
        u = firstuser();
        SET_SEGV_LOCATION();
        while (u) {
            next = nextuser();
            denora_cmd_pong(ServerName, ServerName);
            SET_SEGV_LOCATION();
            xml_write_block_top(ptr, "user");
            xml_write_tag(ptr, "name", u->nick);
            xml_write_tag(ptr, "username", u->username);
            xml_write_tag(ptr, "host", u->host);
            if (ircd->vhost) {
                xml_write_tag(ptr, "vhost", u->vhost);
            }
            if (ircd->vident && u->vident) {
                xml_write_tag(ptr, "vident", u->vident);
            }
            xml_write_tag(ptr, "realname", u->realname);
            xml_write_tag(ptr, "server", u->server->name);
            if (u->ip) {
                xml_write_tag(ptr, "ip", u->ip);
            }
            xml_write_tag(ptr, "country_code", u->country_code);
            xml_write_tag(ptr, "country_name", u->country_name);
            xml_write_tag_int(ptr, "timestamp", (long int) u->timestamp);
            xml_write_tag_int(ptr, "my_signon", (long int) u->my_signon);
            xml_write_tag_int(ptr, "svid", u->svid);
            xml_write_tag_int(ptr, "mode", u->mode);
            xml_write_tag_int(ptr, "language", u->language);
            xml_write_tag_int(ptr, "isaway", (long int) u->isaway);
            if (u->isaway && u->awaymsg) {
                xml_write_tag(ptr, "awaymsg", u->awaymsg);
            }
            xml_write_tag_int(ptr, "admin", u->admin);
            for (i = 0; i < 128; i++) {
                if (u->mode & umodes[i]) {
                    ircsnprintf(buf, BUFSIZE - 1, "mode_%c", (int) i);
                    xml_write_tag_int(ptr, buf, 1);
                    *buf = '\0';
                }
            }
            xml_write_block_top(ptr, "inchannels");
            for (uc = u->chans; uc; uc = uc->next) {
                xml_write_tag(ptr, "channel", uc->chan->name);
            }
            xml_write_block_bottom(ptr, "inchannels");
            xml_moduleData(ptr, &u->moduleData);
            xml_write_block_bottom(ptr, "user");
            u = next;
        }
        xml_write_block_bottom(ptr, "users");
        xml_write_footer(ptr);
    }
}

/*************************************************************************/

/**
 * Export stats struct to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_stats(char *file)
{
    FILE *ptr;
    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);
        xml_write_block_top(ptr, "stats");
        xml_write_tag_int(ptr, "users_max", stats->users_max);
        xml_write_tag_int(ptr, "users", stats->users);
        xml_write_tag_int(ptr, "chans_max", stats->chans_max);
        xml_write_tag_int(ptr, "chans", stats->chans);
        xml_write_tag_int(ptr, "servs_max", stats->servers_max);
        xml_write_tag_int(ptr, "servs", stats->servers);
        xml_write_block_bottom(ptr, "stats");
        xml_write_footer(ptr);
    }

}

/*************************************************************************/

/**
 * Export all stats to xml file
 *
 * @param file is the name that will be used
 * @return void - no returend value
 *
 */
void xml_export_all(char *file)
{
    FILE *ptr;
    User *u, *next;
    Server *s, *snext;
    TLD *t;
    lnode_t *tn;
    Channel *c, *cnext;
    int i, count;
    char buf[BUFSIZE];
    struct u_chanlist *uc;
    struct c_userlist *cu;
    char **bans;
    int ping = 0;
    int counttoping = 1;
    char *temp;

    ptr = new_xml(file);

    SET_SEGV_LOCATION();

    if (ptr) {
        xml_write_header(ptr);

        xml_write_block_top(ptr, "stats");
        xml_write_tag_int(ptr, "users_max", stats->users_max);
        xml_write_tag_int(ptr, "users", stats->users);
        xml_write_tag_int(ptr, "chans_max", stats->chans_max);
        xml_write_tag_int(ptr, "chans", stats->chans);
        xml_write_tag_int(ptr, "servs_max", stats->servers_max);
        xml_write_tag_int(ptr, "servs", stats->servers);
        xml_write_block_bottom(ptr, "stats");

        denora_cmd_pong(ServerName, ServerName);

        xml_write_block_top(ptr, "users");
        u = firstuser();
        SET_SEGV_LOCATION();
        while (u) {
            next = nextuser();
            if (ping) {
                denora_cmd_pong(ServerName, ServerName);
                ping = 0;
            }
            SET_SEGV_LOCATION();
            xml_write_block_top(ptr, "user");
            xml_write_tag(ptr, "name", u->nick);
            xml_write_tag(ptr, "username", u->username);
            xml_write_tag(ptr, "host", u->host);
            if (ircd->vhost) {
                xml_write_tag(ptr, "vhost", u->vhost);
            }
            if (ircd->vident && u->vident) {
                xml_write_tag(ptr, "vident", u->vident);
            }
            xml_write_tag(ptr, "realname", u->realname);
            xml_write_tag(ptr, "server", u->server->name);
            if (u->ip) {
                xml_write_tag(ptr, "ip", u->ip);
            }
            xml_write_tag(ptr, "country_code", u->country_code);
            xml_write_tag(ptr, "country_name", u->country_name);
            xml_write_tag_int(ptr, "timestamp", (long int) u->timestamp);
            xml_write_tag_int(ptr, "my_signon", (long int) u->my_signon);
            xml_write_tag_int(ptr, "svid", u->svid);
            xml_write_tag_int(ptr, "mode", u->mode);
            xml_write_tag_int(ptr, "language", u->language);
            xml_write_tag_int(ptr, "isaway", (long int) u->isaway);
            if (u->isaway && u->awaymsg) {
                xml_write_tag(ptr, "awaymsg", u->awaymsg);
            }
            xml_write_tag_int(ptr, "admin", u->admin);
            for (i = 0; i < 128; i++) {
                if (u->mode & umodes[i]) {
                    ircsnprintf(buf, BUFSIZE - 1, "mode_%c", (int) i);
                    xml_write_tag_int(ptr, buf, 1);
                    *buf = '\0';
                }
            }
            xml_write_block_top(ptr, "inchannels");
            for (uc = u->chans; uc; uc = uc->next) {
                xml_write_tag(ptr, "channel", uc->chan->name);
            }
            xml_write_block_bottom(ptr, "inchannels");
            xml_moduleData(ptr, &u->moduleData);
            xml_write_block_bottom(ptr, "user");
            u = next;
            if (counttoping == 10) {
                ping = 1;
                counttoping = 1;
            } else {
                counttoping++;
            }
        }
        xml_write_block_bottom(ptr, "users");

        xml_write_block_top(ptr, "servers");
        s = servlist->links;
        while (s) {
            snext = s->next;
            if (HideUline && s->uline) {
                continue;
            }
            if (ping) {
                denora_cmd_pong(ServerName, ServerName);
                ping = 0;
            }
            xml_write_block_top(ptr, "server");
            xml_write_tag(ptr, "name", s->name);
            xml_write_tag_int(ptr, "hops", s->hops);
            xml_write_tag(ptr, "desc", s->desc);
            xml_write_tag_int(ptr, "flags", s->flags);
            xml_write_tag_int(ptr, "synced", s->sync);
            if (s->suid) {
                xml_write_tag(ptr, "suid", s->suid);
            }
            if (s->version) {
                xml_write_tag(ptr, "version", s->version);
            }
            if (s->uptime) {
                xml_write_tag_int(ptr, "uptime", s->uptime);
            }
            if (s->uplink) {
                xml_write_tag(ptr, "uplink", s->uplink->name);
            }
            xml_write_block_top(ptr, "serverstats");
            if (s->ss->currentusers) {
                xml_write_tag_int(ptr, "currentusers",
                                  s->ss->currentusers);
            }
            xml_write_tag_int(ptr, "maxusers", s->ss->maxusers);
            xml_write_block_bottom(ptr, "serverstats");

            xml_write_block_bottom(ptr, "server");
            s = snext;
            if (counttoping == 10) {
                ping = 1;
                counttoping = 1;
            } else {
                counttoping++;
            }
        }
        xml_write_block_bottom(ptr, "servers");

        xml_write_block_top(ptr, "tld");
        list_sort(Thead, sortusers);
        tn = list_first(Thead);
        while (tn) {
            t = lnode_get(tn);
            if (ping) {
                denora_cmd_pong(ServerName, ServerName);
                ping = 0;
            }
            xml_write_block_top(ptr, "domain");
            xml_write_tag(ptr, "countrycode", t->countrycode);
            xml_write_tag(ptr, "country", t->country);
            xml_write_tag_int(ptr, "current", t->count);
            xml_write_tag_int(ptr, "overall", t->overall);
            xml_write_block_bottom(ptr, "domain");
            tn = list_next(Thead, tn);
            if (counttoping == 10) {
                ping = 1;
                counttoping = 1;
            } else {
                counttoping++;
            }
        }
        xml_write_block_bottom(ptr, "tld");


        xml_write_block_top(ptr, "channels");
        c = firstchan();
        while (c) {
            cnext = nextchan();
            if (ping) {
                denora_cmd_pong(ServerName, ServerName);
                ping = 0;
            }
            xml_write_block_top(ptr, "chans");
            xml_write_tag(ptr, "name", c->name);

            xml_write_tag(ptr, "topic", c->topic);
            xml_write_tag(ptr, "topicsetter", c->topic_setter);
            xml_write_tag_int(ptr, "creation_time",
                              (long int) c->creation_time);
            xml_write_tag_int(ptr, "topic_time", (long int) c->topic_time);
            xml_write_tag_int(ptr, "topic_count", c->stats->topic_count);
            xml_write_tag_int(ptr, "limit", c->limit);
            xml_write_tag_int(ptr, "usercount",
                              (long int) c->stats->usercount);
            xml_write_tag_int(ptr, "maxusercount",
                              (long int) c->stats->maxusercount);
            xml_write_tag_int(ptr, "kickcount",
                              (long int) c->stats->kickcount);
            xml_write_tag_int(ptr, "joincounter",
                              (long int) c->stats->joincounter);
            temp = chan_get_modes(c, 1);
            xml_write_tag(ptr, "mode", temp);
            free(temp);

            if (c->limit) {
                xml_write_tag_int(ptr, "limit", 1);
            }

            if (c->key) {
                xml_write_tag_int(ptr, "key", 1);
            }

            if (ircd->Lmode && c->redirect) {
                xml_write_tag(ptr, "redirect", c->redirect);
            }
            if (ircd->fmode && c->flood) {
                xml_write_tag(ptr, "flood", c->flood);
            }

            xml_write_block_top(ptr, "inchannel");
            for (cu = c->users; cu; cu = cu->next) {
                xml_write_tag(ptr, "user", cu->user->nick);
            }
            xml_write_block_bottom(ptr, "inchannel");

            if (c->bancount) {
                xml_write_tag_int(ptr, "bancount", c->bancount);
                xml_write_block_top(ptr, "bans");
                count = c->bancount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->bans[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "ban", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "bans");
            }

            if (ircd->except && c->exceptcount) {
                xml_write_tag_int(ptr, "exceptcount", c->exceptcount);
                xml_write_block_top(ptr, "exceptions");
                count = c->exceptcount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->excepts[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "except", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "exceptions");
            }

            if (ircd->invitemode && c->invitecount) {
                xml_write_tag_int(ptr, "invitecount", c->invitecount);
                xml_write_block_top(ptr, "invites");
                count = c->invitecount;
                bans = calloc(sizeof(char *) * count, 1);
                for (i = 0; i < count; i++) {
                    bans[i] = sstrdup(c->invite[i]);
                    for (i = 0; i < count; i++) {
                        xml_write_tag(ptr, "invite", bans[i]);
                    }
                }
                free(bans);
                xml_write_block_bottom(ptr, "invites");
            }
            xml_write_block_bottom(ptr, "chans");
            c = cnext;
            if (counttoping == 10) {
                ping = 1;
                counttoping = 1;
            } else {
                counttoping++;
            }
        }
        xml_write_block_bottom(ptr, "channels");

        xml_write_footer(ptr);
    }
}

/*************************************************************************/
