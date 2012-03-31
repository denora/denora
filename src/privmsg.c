/*
 *
 * (c) 2004-2012 Denora Team
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

PrivMsgHash *PRIVMSGHANDLERS[MAX_CMD_HASH];

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
PrivMsg *findPrivMsg(char *name)
{
    int idx;
    PrivMsgHash *privcurrent = NULL;
    if (!name) {
        return NULL;
    }
    idx = CMD_HASH(name);

    SET_SEGV_LOCATION();

    for (privcurrent = PRIVMSGHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(name, privcurrent->service) == 0) {
            return privcurrent->p;
        }
    }
    return NULL;

}

/*************************************************************************/

PrivMsg *createPrivmsg(char *service, void (*func) (User * u, char *buf))
{
    PrivMsg *m;

    if (!service) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    if ((m = malloc(sizeof(PrivMsg))) == NULL) {
        fatal("Out of memory!");
    }
    m->service = sstrdup(service);      /* Our Name                 */
    m->handler = func;          /* Handle                   */
    return m;                   /* return a nice new module */
}

/*************************************************************************/

int addPrivMsg(PrivMsg * m)
{
    int modindex = 0;
    PrivMsgHash *privcurrent = NULL;
    PrivMsgHash *newHash = NULL;
    PrivMsgHash *lastHash = NULL;

    SET_SEGV_LOCATION();

    modindex = CMD_HASH(m->service);

    for (privcurrent = PRIVMSGHANDLERS[modindex]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->service, privcurrent->service) == 0)
            return MOD_ERR_EXISTS;
        lastHash = privcurrent;
    }

    if ((newHash = malloc(sizeof(PrivMsgHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->service = sstrdup(m->service);
    newHash->p = m;

    if (lastHash == NULL)
        PRIVMSGHANDLERS[modindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyPrivMsg(PrivMsg * m)
{
    if (!m) {
        return MOD_ERR_PARAMS;
    }

    if (m->service) {
        free(m->service);
    }

    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

int delPrivMsg(PrivMsg * m)
{
    int idx = 0;
    PrivMsgHash *privcurrent = NULL;
    PrivMsgHash *lastHash = NULL;

    if (!m) {
        return MOD_ERR_PARAMS;
    }

    idx = CMD_HASH(m->service);

    for (privcurrent = PRIVMSGHANDLERS[idx]; privcurrent;
         privcurrent = privcurrent->next) {
        if (stricmp(m->service, privcurrent->service) == 0) {
            if (!lastHash) {
                PRIVMSGHANDLERS[idx] = privcurrent->next;
            } else {
                lastHash->next = privcurrent->next;
            }
            destroyPrivMsg(privcurrent->p);
            free(privcurrent->service);
            free(privcurrent);
            return MOD_ERR_OK;
        }
        lastHash = privcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/
