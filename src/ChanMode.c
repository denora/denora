/* Channel Modes - read MODES for complete details on how Denora
 * handles modes.
 *
 * (c) 2004-2010 Denora Team
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

ChanModeHash *CHANMODEHANDLERS[MAX_MODE_HASH];
unsigned long cmodes[128];
char csmodes[128];
unsigned long sjoinmodes[128];


/*************************************************************************/

/* This is purely so that the Array is set to all 0 before
   we begin to loop it and check for the data
*/
void init_cmodes(void)
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        cmodes[i] = 0;
    }
}

/*************************************************************************/

void init_csmodes(void)
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        csmodes[i] = 0;
    }
    for (i = 0; i < 128; i++) {
        sjoinmodes[i] = 0;
    }
}

/*************************************************************************/

void ModuleSetChanUMode(int mode, char letter, int status_flag)
{
    csmodes[mode] = letter;
    sjoinmodes[mode] = status_flag;
    ModuleSetChanMode((int) letter, IRCD_ENABLE);
}

/*************************************************************************/

int denora_cmode(int m)
{
    return (cmodes[m] ? 1 : 0);
}

/*************************************************************************/

int ChanHasMode(char *chan, int m)
{
    Channel *c;
    char modebuf[16];
    struct c_modes *um;

    c = findchan(chan);
    if (!c) {
        return 0;
    }
    if (cmodes[m]) {
        ircsnprintf(modebuf, sizeof(modebuf), "%c", m);
        for (um = c->modes; um && strcmp(modebuf, um->mode) != 0;
             um = um->next);
        return (um ? 1 : 0);
    } else {
        return 0;
    }
}

/*************************************************************************/

void ModuleSetChanMode(int mode, int flag)
{
    if (flag == IRCD_ENABLE) {
        cmodes[mode] = flag;
    } else if (flag == IRCD_DISABLE) {
        cmodes[mode] = flag;
    } else {
        cmodes[mode] = IRCD_ENABLE;
    }
}

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
ChanMode *FindChanMode(char *name)
{
    int idx;
    ChanModeHash *privcurrent = NULL;

    if (!name) {
        return NULL;
    }
    idx = MODEHASHSENSITVE(name);

    SET_SEGV_LOCATION();

    for (privcurrent = CHANMODEHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (strcmp(name, privcurrent->mode) == 0) {
            return privcurrent->cm;
        }
    }
    return NULL;
}

/*************************************************************************/

ChanMode *CreateChanMode(int mode,
                         void (*setvalue) (Channel * chan, char *value),
                         char *(*getvalue) (Channel * chan))
{
    ChanMode *m;
    char modebuf[15];

    if (!mode) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    ircsnprintf(modebuf, sizeof(modebuf), "%c", mode);

    if ((m = malloc(sizeof(ChanMode))) == NULL) {
        fatal("Out of memory!");
    }
    m->mode = sstrdup(modebuf); /* Our Name                 */
    m->setvalue = setvalue;     /* Handle                   */
    m->getvalue = getvalue;
    addChanMode(m);
    ModuleSetChanMode(mode, IRCD_ENABLE);
    return m;                   /* return a nice new module */
}

/*************************************************************************/

int addChanMode(ChanMode * m)
{
    int modindex = 0;
    ChanModeHash *privcurrent = NULL;
    ChanModeHash *newHash = NULL;
    ChanModeHash *lastHash = NULL;

    SET_SEGV_LOCATION();

    modindex = MODEHASHSENSITVE(m->mode);

    for (privcurrent = CHANMODEHANDLERS[modindex]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->mode, privcurrent->mode) == 0)
            return MOD_ERR_EXISTS;
        lastHash = privcurrent;
    }

    if ((newHash = malloc(sizeof(ChanModeHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->mode = sstrdup(m->mode);
    newHash->cm = m;

    if (lastHash == NULL)
        CHANMODEHANDLERS[modindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyChanMode(ChanMode * m)
{
    if (!m) {
        return MOD_ERR_PARAMS;
    }

    if (m->mode) {
        free(m->mode);
    }

    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

int delChanMode(ChanMode * m)
{
    int idx = 0;
    ChanModeHash *privcurrent = NULL;
    ChanModeHash *lastHash = NULL;

    if (!m) {
        return MOD_ERR_PARAMS;
    }

    idx = CMD_HASH(m->mode);

    for (privcurrent = CHANMODEHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->mode, privcurrent->mode) == 0) {
            if (!lastHash) {
                CHANMODEHANDLERS[idx] = privcurrent->next;
            } else {
                lastHash->next = privcurrent->next;
            }
            destroyChanMode(privcurrent->cm);
            free(privcurrent->mode);
            free(privcurrent);
            return MOD_ERR_OK;
        }
        lastHash = privcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

void SetChanMode(Channel * c, char *mode)
{
    struct c_modes *u;

    u = calloc(sizeof(*u), 1);
    u->next = c->modes;
    if (c->modes) {
        c->modes->prev = u;
    }
    c->modes = u;
    u->mode = sstrdup(mode);
}

/*************************************************************************/

void RemoveChanMode(Channel * c, char *mode)
{
    struct c_modes *u;

    for (u = c->modes; u && stricmp(mode, u->mode) != 0; u = u->next);
    if (u) {
        if (u->next) {
            u->next->prev = u->prev;
        }
        if (u->prev) {
            u->prev->next = u->next;
        } else {
            c->modes = u->next;
        }
        free(u);
    }
}

/*************************************************************************/

void ModuleUpdateSQLChanMode(void)
{
    char modebuf[BUFSIZE];
    char *temp = NULL;
    int i = 0;

    if (ircd->cmodes) {
        free(ircd->cmodes);
    }
    for (i = 0; i < 128; i++) {
        if (cmodes[i]) {
            if (!temp) {
                ircsnprintf(modebuf, sizeof(modebuf), "%c", (char) i);
                temp = sstrdup(modebuf);
            } else {
                ircsnprintf(modebuf, sizeof(modebuf), "%s%c", temp,
                            (char) i);
                free(temp);
                temp = sstrdup(modebuf);
            }
        }
    }
    if (temp) {
        ircd->cmodes = sstrdup(temp);
        free(temp);
    }
}

/*************************************************************************/


void chan_set_modes(Channel * chan, int ac, char **av)
{
    int add = 1;
    char *modes = av[0], mode;
    ChanMode *cm;
    ChanBanMode *cbm;
    char modebuf[BUFSIZE];

    alog(LOG_DEBUG, langstr(ALOG_DEBUG_CHANGE_MODE_CHAN), chan->name,
         merge_args(ac, av));

    ac--;
    SET_SEGV_LOCATION();

    chan->stats->modecount++;
    chan->stats->modecounttime++;

    while ((mode = *modes++)) {

        switch (mode) {
        case '+':
            add = 1;
            continue;
        case '-':
            add = 0;
            continue;
        }

        if (((int) mode) < 0) {
            alog(LOG_DEBUG, langstr(ALOG_DEBUG_BAD_CHAN_MODE), chan->name);
            continue;
        }
        SET_SEGV_LOCATION();

        ircsnprintf(modebuf, sizeof(modebuf), "%c", mode);

        cm = FindChanMode(modebuf);
        cbm = FindChanBanMode(modebuf);
        if (cm) {
            if (add)
                SetChanMode(chan, modebuf);
            else
                RemoveChanMode(chan, modebuf);

            if (cm->setvalue) {
                if (ac <= 0) {
                    alog(LOG_ERROR, langstr(ALOG_DEBUG_MODE_NO_PARAM),
                         add ? '+' : '-', mode, chan->name);
                    continue;
                }
                ac--;
                av++;
                cm->setvalue(chan, add ? *av : NULL);
                SET_SEGV_LOCATION();
                if (ac > 0 && *av) {
                    send_event(EVENT_CHANNEL_MODE, 4,
                               (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
                               chan->name, modebuf, *av);
                } else {
                    send_event(EVENT_CHANNEL_MODE, 3,
                               (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
                               chan->name, modebuf);
                }
            } else {
                send_event(EVENT_CHANNEL_MODE, 3,
                           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
                           chan->name, modebuf);
            }
        } else if (cbm) {
            if (ac <= 0) {
                alog(LOG_ERROR, langstr(ALOG_DEBUG_MODE_NO_PARAM),
                     add ? '+' : '-', mode, chan->name);
                continue;
            }
            ac--;
            av++;
            if (add && cbm->addmask) {
                cbm->addmask(chan, *av);
            }
            if (!add && cbm->delmask) {
                cbm->delmask(chan, *av);
            }
            if (*av) {
                send_event(EVENT_CHANNEL_MODE, 4,
                           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
                           chan->name, modebuf, *av);
            } else {
                send_event(EVENT_CHANNEL_MODE, 3,
                           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
                           chan->name, modebuf);
            }
        } else {
            /* mode must be one of qaohv and must be ignored */
            if (ac == 0) {
                continue;
            }
            ac--;
            av++;
        }
    }

    chan->stats->secret = ChanHasMode(chan->name, CMODE_s);
    chan->stats->private = ChanHasMode(chan->name, CMODE_p);
}

/*************************************************************************/

/* Returns a fully featured binary modes string. If complete is 0, the
 * eventual parameters won't be added to the string.
 */
char *chan_get_modes(Channel * chan, int complete)
{
    static char modebuf[BUFSIZE];
    struct c_modes *um;
    char *temp = NULL;
    ChanMode *cm;

    SET_SEGV_LOCATION();


    for (um = chan->modes; um; um = um->next) {
        if (!temp) {
            ircsnprintf(modebuf, sizeof(modebuf), "%s", um->mode);
            temp = sstrdup(modebuf);
        } else {
            ircsnprintf(modebuf, sizeof(modebuf), "%s%s", temp, um->mode);
            free(temp);
            temp = sstrdup(modebuf);
        }
    }

    if (complete) {
        for (um = chan->modes; um; um = um->next) {
            cm = FindChanMode(um->mode);
            if (cm && cm->getvalue) {
                ircsnprintf(modebuf, sizeof(modebuf), "%s %s", temp,
                            cm->getvalue);
                free(temp);
                temp = sstrdup(modebuf);
            }
        }
    }

    return temp;
}

/*************************************************************************/

/* adds modes to a given chanid */
void sql_do_chanmodes(char *chan, int ac, char **av)
{
    int atleastone = 0;
    char db[1000];              /* should be enough for long queries */
    char tmp[14] = "mode_XX=\"X\", ";
    char *modes;
    int argptr = 1;
    char buf[BUFSIZE];
    int nickid;
    char *user;
    int chanid;

    chan = rdb_escape(chan);
    chanid = db_getchannel(chan);

    if (!chanid) {
        free(chan);
        return;
    }

    modes = av[0];

    SET_SEGV_LOCATION();
    ircsnprintf(buf, sizeof(buf), "UPDATE %s SET ", ChanTable);

    if (*modes == '0') {
        return;
    }
    strlcpy(db, buf, sizeof(db));
    while (*modes) {
        switch (*modes) {
        case '+':
            tmp[9] = 'Y';
            break;
        case '-':
            tmp[9] = 'N';
            break;
        default:
            if (!strchr(ircd->cmodes, *modes)) {
                alog(LOG_DEBUG, langstr(ALOG_DEBUG_UNKNOW_CMODE), *modes);
                alog(LOG_DEBUG, langstr(ALOG_DEBUG_LAST_LINE), inbuf);
                alog(LOG_DEBUG, langstr(ALOG_DEBUG_REPORT_AT));
            } else if (*modes == ircd->ban_char) {
                argptr++;
            } else if (ircd->except && *modes == ircd->except_char) {
                argptr++;
            } else if (ircd->invitemode && *modes == ircd->invite_char) {
                argptr++;
            } else if (ircd->halfop && *modes == 'h') {
                SET_SEGV_LOCATION();
                user = rdb_escape(av[argptr++]);
                nickid = db_getnick(user);
                if (nickid) {
                    rdb_query
                        (QUERY_LOW,
                         "UPDATE %s SET mode_lh=\'%c\' WHERE chanid=%d AND nickid=%d",
                         IsOnTable, tmp[9], chanid, nickid);
                }
                free(user);
            } else if (ircd->owner && *modes == 'q') {
                SET_SEGV_LOCATION();
                user = rdb_escape(av[argptr++]);
                nickid = db_getnick(user);
                if (nickid) {
                    rdb_query
                        (QUERY_LOW,
                         "UPDATE %s SET mode_lq=\'%c\' WHERE chanid=%d AND nickid=%d",
                         IsOnTable, tmp[9], chanid, nickid);
                }
                free(user);
            } else if (ircd->protect && *modes == 'a') {
                SET_SEGV_LOCATION();
                user = rdb_escape(av[argptr++]);
                nickid = db_getnick(user);
                if (nickid) {
                    rdb_query
                        (QUERY_LOW,
                         "UPDATE %s SET mode_la=\'%c\' WHERE chanid=%d AND nickid=%d",
                         IsOnTable, tmp[9], chanid, nickid);
                }
                free(user);
            } else if (*modes == 'o' || *modes == 'v') {
                SET_SEGV_LOCATION();
                user = rdb_escape(av[argptr++]);
                nickid = db_getnick(user);
                if (nickid) {
                    rdb_query
                        (QUERY_LOW,
                         "UPDATE %s SET mode_l%c=\'%c\' WHERE chanid=%d AND nickid=%d",
                         IsOnTable, *modes, tmp[9], chanid, nickid);
                }
                free(user);
            } else {
                atleastone = 1;
                tmp[5] = ((*modes >= 'a') ? 'l' : 'u');
                tmp[6] = tolower(*modes);
                strlcat(db, tmp, sizeof(db));
                if (*modes == 'k') {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        char *key;
                        key = rdb_escape(av[argptr++]);
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_lk_data=\'%s\', ",
                                    (HidePasswords ? "HIDDEN" : key));
                        strlcat(db, buf, sizeof(db));
                        free(key);
                    } else {
                        strlcat(db, "mode_lk_data=\'\', ", sizeof(db));
                        argptr++;       /* mode -k needs a parameter */
                    }
                } else if (*modes == 'l') {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_ll_data=\'%s\', ", av[argptr++]);
                        strlcat(db, buf, sizeof(db));
                    } else {
                        strlcat(db, "mode_ll_data=\'\', ", sizeof(db));
                    }
                } else if (ircd->Lmode && *modes == ircd->chanforward) {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        char *ch = rdb_escape(av[argptr++]);
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'%s\', ",
                                    (ircd->chanforward <= 90 ? "u" : "l"),
                                    ircd->chanforward, ch);
                        strlcat(db, buf, sizeof(db));
                        free(ch);
                    } else {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'\', ",
                                    (ircd->chanforward <= 90 ? "u" : "l"),
                                    ircd->chanforward);
                        strlcat(db, buf, sizeof(db));
                    }
                } else if (ircd->fmode && ircd->floodchar
                           && *modes == ircd->floodchar) {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'%s\', ",
                                    (ircd->floodchar <= 90 ? "u" : "l"),
                                    ircd->floodchar, av[argptr++]);
                        strlcat(db, buf, sizeof(db));
                    } else {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'\', ",
                                    (ircd->floodchar <= 90 ? "u" : "l"),
                                    ircd->floodchar);
                        strlcat(db, buf, sizeof(db));
                    }
                } else if (ircd->jmode && ircd->floodchar_alternative
                           && *modes == ircd->floodchar_alternative) {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'%s\', ",
                                    (ircd->floodchar_alternative <=
                                     90 ? "u" : "l"),
                                    ircd->floodchar_alternative,
                                    av[argptr++]);
                        strlcat(db, buf, sizeof(db));
                    } else {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'\', ",
                                    (ircd->floodchar_alternative <=
                                     90 ? "u" : "l"),
                                    ircd->floodchar_alternative);
                        strlcat(db, buf, sizeof(db));
                    }
                } else if (ircd->jointhrottle
                           && *modes == ircd->jointhrottle) {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'%s\', ",
                                    (ircd->jointhrottle <= 90 ? "u" : "l"),
                                    ircd->jointhrottle, av[argptr++]);
                        strlcat(db, buf, sizeof(db));
                    } else {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'\', ",
                                    (ircd->jointhrottle <= 90 ? "u" : "l"),
                                    ircd->jointhrottle);
                        strlcat(db, buf, sizeof(db));
                    }
                } else if (ircd->nickchgfloodchar
                           && *modes == ircd->nickchgfloodchar) {
                    SET_SEGV_LOCATION();
                    if (tmp[9] == 'Y' && argptr < ac) {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'%s\', ",
                                    (ircd->nickchgfloodchar <=
                                     90 ? "u" : "l"),
                                    ircd->nickchgfloodchar, av[argptr++]);
                        strlcat(db, buf, sizeof(db));
                    } else {
                        *buf = '\0';
                        ircsnprintf(buf, BUFSIZE - 1,
                                    "mode_%s%c_data=\'\', ",
                                    (ircd->nickchgfloodchar <=
                                     90 ? "u" : "l"),
                                    ircd->nickchgfloodchar);
                        strlcat(db, buf, sizeof(db));
                    }
                }
            }
            break;
        }
        modes++;
    }
    if (atleastone) {
        ircsnprintf(&db[strlen(db) - 2], sizeof(db), " WHERE chanid=%d",
                    chanid);
        rdb_query(QUERY_LOW, db);
    }
    free(chan);
}

/*************************************************************************/

char *get_flood(Channel * chan)
{
    SET_SEGV_LOCATION();
    if (!chan) {
        return NULL;
    }
    return chan->flood;
}

/*************************************************************************/

char *get_flood_alt(Channel * chan)
{
    SET_SEGV_LOCATION();
    if (!chan) {
        return NULL;
    }
    return chan->flood_alt;
}

/*************************************************************************/

char *get_key(Channel * chan)
{
    SET_SEGV_LOCATION();
    if (!chan) {
        return NULL;
    }
    return chan->key;
}

/*************************************************************************/

char *get_limit(Channel * chan)
{
    if (!chan) {
        return NULL;
    }
    SET_SEGV_LOCATION();

    if (chan->limit == 0) {
        return NULL;
    }
    return itostr(chan->limit);
}

/*************************************************************************/

char *get_rejoinlock(Channel * chan)
{
    if (!chan) {
        return NULL;
    }
    SET_SEGV_LOCATION();

    if (chan->rejoinlock == 0) {
        return NULL;
    }
    return itostr(chan->rejoinlock);
}

/*************************************************************************/

char *get_nickchgflood(Channel * chan)
{
    SET_SEGV_LOCATION();
    if (!chan) {
        return NULL;
    }
    return chan->nickchgflood;
}

/*************************************************************************/

char *get_redirect(Channel * chan)
{
    SET_SEGV_LOCATION();
    if (!chan) {
        return NULL;
    }
    return chan->redirect;
}

/*************************************************************************/

void set_flood(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->flood) {
        free(chan->flood);
    }

    chan->flood = (!BadPtr(value) ? sstrdup(value) : NULL);

    alog(LOG_DEBUG, langstr(ALOG_DEBUG_FLOOD_MODE),
         chan->name, (chan->flood ? chan->flood : langstr(ALOG_NO_FLOOD)));
}

/*************************************************************************/

void set_flood_alt(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->flood_alt) {
        free(chan->flood_alt);
    }

    chan->flood_alt = (!BadPtr(value) ? sstrdup(value) : NULL);

    alog(LOG_DEBUG,
         "debug: Alternative Flood mode for channel %s set to %s",
         chan->name,
         (chan->flood_alt ? chan->flood_alt : langstr(ALOG_NO_FLOOD)));
}

/*************************************************************************/

void set_key(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->key) {
        free(chan->key);
    }

    chan->key = (!BadPtr(value) ? sstrdup(value) : NULL);

    alog(LOG_DEBUG, langstr(ALOG_KEY_SET_TO), chan->name,
         (chan->key ? chan->key : langstr(ALOG_NO_KEY)));
}

/*************************************************************************/

void set_limit(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    chan->limit = (!BadPtr(value) ? strtoul(value, NULL, 10) : 0);

    alog(LOG_DEBUG, langstr(ALOG_LIMIT_SET_TO), chan->name, chan->limit);
}

/*************************************************************************/

void set_rejoinlock(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    chan->rejoinlock = (!BadPtr(value) ? strtoul(value, NULL, 10) : 0);

    alog(LOG_DEBUG, "debug: Rejoin lock for channel %s set to %u",
         chan->name, chan->rejoinlock);
}

/*************************************************************************/

void set_nickchgflood(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->nickchgflood) {
        free(chan->nickchgflood);
    }

    chan->nickchgflood = (!BadPtr(value) ? sstrdup(value) : NULL);

    alog(LOG_DEBUG, "debug: Nick change flood for %s set to %s",
         chan->name, chan->nickchgflood);
}

/*************************************************************************/

void set_redirect(Channel * chan, char *value)
{
    SET_SEGV_LOCATION();

    if (!chan) {
        return;
    }

    if (chan->redirect) {
        free(chan->redirect);
    }
    chan->redirect = (!BadPtr(value) ? sstrdup(value) : NULL);

    alog(LOG_DEBUG, langstr(ALOG_REDIRECT_SET_TO), chan->name,
         (chan->redirect ? chan->redirect : langstr(ALOG_NO_REDIRECT)));
}

/*************************************************************************/
