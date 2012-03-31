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

time_t lastrun = 0;
CronEventHash *CRONEVENT[MAX_CMD_HASH];
static CronEventHash *current;
static int next_index;

/*************************************************************************/

/**
 * Process cron events on the timer
 *
 * @param ts is Current time
 * @return void - no returend value
 *
 */
void denora_cron(time_t ts)
{
    static struct tm cron_time;

    /* so it should run at least every few mins (PING) */
#ifdef _WIN32
    localtime_s(&cron_time, &ts);
#else
    cron_time = *localtime(&ts);
#endif

    /* Stop if it's not between :00 and :30, with 40 minutes limit for last run.
       Under normal conditions, upon starting up the crons will "sync" at :00
       within 1-2 hours :) */
    if (cron_time.tm_min > 30 || (ts - lastrun) <= 2400) {
        return;
    }
    SET_SEGV_LOCATION();

    if (cron_time.tm_hour == 0) {       /* midnight */
        /* run midnight routines */
        cron_process_events(CRON_MIDNIGHT);

        if (cron_time.tm_wday == 0) {   /* sunday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_SUNDAY);
        }
        if (cron_time.tm_wday == 1) {   /* monday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_MONDAY);
        }
        if (cron_time.tm_wday == 2) {   /* tuesday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_TUESDAY);
        }
        if (cron_time.tm_wday == 3) {   /* Wednesday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_WEDNESDAY);
        }
        if (cron_time.tm_wday == 4) {   /* Thursday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_THURSDAY);
        }
        if (cron_time.tm_wday == 5) {   /* tuesday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_FRIDAY);
        }
        if (cron_time.tm_wday == 6) {   /* Saturday midnight */
            /* run weekly routines */
            cron_process_events(CRON_WEEKLY_SATURDAY);
        }
        if (cron_time.tm_mday == 1) {   /* 1st day of month midnight */
            /* run monthly routines */
            cron_process_events(CRON_MONTHLY);
        }
        SET_SEGV_LOCATION();
    } else if (cron_time.tm_hour == 1) {
        cron_process_events(CRON_HOUR_1);
    } else if (cron_time.tm_hour == 2) {
        cron_process_events(CRON_HOUR_2);
    } else if (cron_time.tm_hour == 3) {
        cron_process_events(CRON_HOUR_3);
    } else if (cron_time.tm_hour == 4) {
        cron_process_events(CRON_HOUR_4);
    } else if (cron_time.tm_hour == 5) {
        cron_process_events(CRON_HOUR_5);
    } else if (cron_time.tm_hour == 6) {
        cron_process_events(CRON_HOUR_6);
    } else if (cron_time.tm_hour == 7) {
        cron_process_events(CRON_HOUR_7);
    } else if (cron_time.tm_hour == 8) {
        cron_process_events(CRON_HOUR_8);
    } else if (cron_time.tm_hour == 9) {
        cron_process_events(CRON_HOUR_9);
    } else if (cron_time.tm_hour == 10) {
        cron_process_events(CRON_HOUR_10);
    } else if (cron_time.tm_hour == 11) {
        cron_process_events(CRON_HOUR_11);
    } else if (cron_time.tm_hour == 12) {
        cron_process_events(CRON_HOUR_12);
    } else if (cron_time.tm_hour == 13) {
        cron_process_events(CRON_HOUR_13);
    } else if (cron_time.tm_hour == 14) {
        cron_process_events(CRON_HOUR_14);
    } else if (cron_time.tm_hour == 15) {
        cron_process_events(CRON_HOUR_15);
    } else if (cron_time.tm_hour == 16) {
        cron_process_events(CRON_HOUR_16);
    } else if (cron_time.tm_hour == 17) {
        cron_process_events(CRON_HOUR_17);
    } else if (cron_time.tm_hour == 18) {
        cron_process_events(CRON_HOUR_18);
    } else if (cron_time.tm_hour == 19) {
        cron_process_events(CRON_HOUR_19);
    } else if (cron_time.tm_hour == 20) {
        cron_process_events(CRON_HOUR_20);
    } else if (cron_time.tm_hour == 21) {
        cron_process_events(CRON_HOUR_21);
    } else if (cron_time.tm_hour == 22) {
        cron_process_events(CRON_HOUR_22);
    } else if (cron_time.tm_hour == 23) {
        cron_process_events(CRON_HOUR_23);
    }
    cron_process_events(CRON_HOURLY);
    lastrun = ts;
}

/*************************************************************************/

/**
 * Actually processing cron events function
 *
 * @param name of the cron event
 * @return void - no returend value
 *
 */
void cron_process_events(const char *name)
{
    int retVal = 0;
    CronEvent *ccurrent = NULL;
    CronEvent *evh;

    SET_SEGV_LOCATION();

    /* Do something with the message. */
    evh = findCronEvent(CRONEVENT, name);
    if (evh) {
        if (evh->func) {
            retVal = evh->func(name);
            if (retVal == MOD_CONT) {
                ccurrent = evh->next;
                while (ccurrent && ccurrent->func && retVal == MOD_CONT) {
                    retVal = ccurrent->func(name);
                    ccurrent = ccurrent->next;
                }
            }
        }
    }
    SET_SEGV_LOCATION();
}

/*************************************************************************/

CronEvent *createCronEvent(const char *name,
                           int (*func) (const char *name))
{
    CronEvent *evm = NULL;
    if (!func) {
        return NULL;
    }
    SET_SEGV_LOCATION();

    if ((evm = malloc(sizeof(CronEvent))) == NULL) {
        fatal("Out of memory!");
    }
    evm->name = sstrdup(name);
    evm->func = func;
    evm->next = NULL;
    evm->mod_name = NULL;
    return evm;
}

/*************************************************************************/

CronEvent *findCronEvent(CronEventHash * hookEvtTable[], const char *name)
{
    int idx;
    CronEventHash *ccurrent = NULL;
    if (!hookEvtTable || !name) {
        return NULL;
    }
    SET_SEGV_LOCATION();

    idx = CMD_HASH(name);

    for (ccurrent = hookEvtTable[idx]; ccurrent; ccurrent = ccurrent->next) {
        if (stricmp(name, ccurrent->name) == 0) {
            return ccurrent->evh;
        }
    }
    return NULL;
}

/*************************************************************************/

int addCronEvent(CronEventHash * hookEvtTable[], CronEvent * evh)
{
    /* We can assume both param's have been checked by this point.. */
    int cronindex = 0;
    CronEventHash *ccurrent = NULL;
    CronEventHash *newHash = NULL;
    CronEventHash *lastHash = NULL;

    if (!hookEvtTable || !evh) {
        return MOD_ERR_PARAMS;
    }
    SET_SEGV_LOCATION();

    cronindex = CMD_HASH(evh->name);

    for (ccurrent = hookEvtTable[cronindex]; ccurrent;
         ccurrent = ccurrent->next) {
        if (stricmp(evh->name, ccurrent->name) == 0) {  /* the msg exist's we are a addHead */
            evh->next = ccurrent->evh;
            ccurrent->evh = evh;
            alog(LOG_DEBUG,
                 "debug: existing Cron: (0x%p), new msg (0x%p) (%s)",
                 (void *) evh->next, (void *) evh, evh->name);
            return MOD_ERR_OK;
        }
        lastHash = ccurrent;
    }

    if ((newHash = malloc(sizeof(CronEventHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->name = sstrdup(evh->name);
    newHash->evh = evh;
    SET_SEGV_LOCATION();

    if (lastHash == NULL) {
        hookEvtTable[cronindex] = newHash;
    } else {
        lastHash->next = newHash;
    }
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Destory a message, freeing its memory.
 * @param evh the Event Handler to be destroyed
 * @return MOD_ERR_OK on success
 **/
int destroyCronEvent(CronEvent * evh)
{
    if (!evh) {
        return MOD_ERR_PARAMS;
    }
    SET_SEGV_LOCATION();

    if (evh->name) {
        free(evh->name);
    }
    evh->func = NULL;
    if (evh->mod_name) {
        free(evh->mod_name);
    }
    evh->next = NULL;
    free(evh);
    return MOD_ERR_OK;
}

/*************************************************************************/

int delCronEvent(CronEventHash * msgEvtTable[], CronEvent * evm,
                 char *mod_name)
{
    int cronindex = 0;
    CronEventHash *ccurrent = NULL;
    CronEventHash *lastHash = NULL;
    CronEvent *tail = NULL, *last = NULL;

    if (!evm || !msgEvtTable) {
        return MOD_ERR_PARAMS;
    }
    SET_SEGV_LOCATION();

    cronindex = CMD_HASH(evm->name);

    for (ccurrent = msgEvtTable[cronindex]; ccurrent;
         ccurrent = ccurrent->next) {
        if (stricmp(evm->name, ccurrent->name) == 0) {
            destroyCronEvent(evm);
            if (!lastHash) {
                tail = ccurrent->evh;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                ccurrent->evh = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    msgEvtTable[cronindex] = ccurrent->next;
                    if (ccurrent->name) {
                        free(ccurrent->name);
                    }
                    return MOD_ERR_OK;
                }
            } else {
                tail = ccurrent->evh;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                ccurrent->evh = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    lastHash->next = ccurrent->next;
                    if (ccurrent->name) {
                        free(ccurrent->name);
                    }
                    return MOD_ERR_OK;
                }
            }
        }
        lastHash = ccurrent;
    }
    SET_SEGV_LOCATION();

    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

int moduleCronEventDel(const char *name)
{
    CronEvent *evh;
    int status;

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }
    evh = findCronEvent(CRONEVENT, name);
    if (!evh) {
        return MOD_ERR_NOEXIST;
    }
    SET_SEGV_LOCATION();

    status = delCronEvent(CRONEVENT, evh, mod_current_module->name);
    if (denora->debug) {
        displayCronFromHash(evh->name);
    }
    return status;
}

/*************************************************************************/

int moduleAddCronEvent(CronEvent * evh)
{
    int status;

    if (!evh) {
        return MOD_ERR_PARAMS;
    }
    SET_SEGV_LOCATION();

    if ((mod_current_module_name) && (!mod_current_module)) {
        mod_current_module = findModule(mod_current_module_name);
    }

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }                           /* shouldnt happen */
    evh->core = 0;

    if (BadPtr(evh->mod_name)) {
        evh->mod_name = sstrdup(mod_current_module->name);
    }
    SET_SEGV_LOCATION();

    status = addCronEvent(CRONEVENT, evh);
    if (denora->debug) {
        displayCronFromHash(evh->name);
    }
    return status;
}

/*************************************************************************/

int addCoreCronEvent(CronEventHash * hookEvtTable[], CronEvent * evh)
{
    if (!hookEvtTable || !evh) {
        return MOD_ERR_PARAMS;
    }
    SET_SEGV_LOCATION();

    evh->core = 1;
    return addCronEvent(hookEvtTable, evh);
}

/*************************************************************************/

int displayCronFromHash(char *name)
{
    CronEventHash *ccurrent = NULL;
    int cronindex = 0;
    cronindex = CMD_HASH(name);

    alog(LOG_EXTRADEBUG, langstr(ALOG_TRY_TO_DISPLAY), name);
    SET_SEGV_LOCATION();

    for (ccurrent = CRONEVENT[cronindex]; ccurrent;
         ccurrent = ccurrent->next) {
        if (stricmp(name, ccurrent->name) == 0) {
            displayCronEvent(ccurrent->evh);
        }
    }
    alog(LOG_EXTRADEBUG, langstr(ALOG_DONE_DISPLAY), name);
    return 0;
}

/*************************************************************************/

int displayCronEvent(CronEvent * evh)
{
    CronEvent *msg = NULL;
    int i = 0;
    alog(LOG_EXTRADEBUG, "debug: Displaying message list for %s",
         evh->name);
    for (msg = evh; msg; msg = msg->next) {
        alog(LOG_DEBUG, "%d: 0x%p", ++i, (void *) msg);
    }
    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, "debug: end");
    return 0;
}

/*************************************************************************/

void setup_cron_event()
{
    CronEvent *evt;
    evt = createCronEvent(CRON_HOURLY, users_hourly);
    addCronEvent(CRONEVENT, evt);
    evt = createCronEvent(CRON_HOURLY, chans_hourly);
    addCronEvent(CRONEVENT, evt);
    evt = createCronEvent(CRON_HOURLY, servers_hourly);
    addCronEvent(CRONEVENT, evt);
}

/*************************************************************************/

CronEvent *first_cronevent(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL)
        current = CRONEVENT[next_index++];
    return current->evh;
}

/*************************************************************************/

CronEvent *next_cronevent(void)
{
    SET_SEGV_LOCATION();

    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL)
            current = CRONEVENT[next_index++];
    }
    if (current) {
        return current->evh;
    } else {
        return NULL;
    }
}

/*************************************************************************/

CronEventHash *first_croneventhash(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL)
        current = CRONEVENT[next_index++];
    return current;
}

/*************************************************************************/

CronEventHash *next_croneventhash(void)
{
    SET_SEGV_LOCATION();

    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL)
            current = CRONEVENT[next_index++];
    }
    if (current) {
        return current;
    } else {
        return NULL;
    }
}

/*************************************************************************/

int destroyCronEventHash(CronEventHash * mh)
{
    SET_SEGV_LOCATION();

    if (!mh) {
        return MOD_ERR_PARAMS;
    }
    if (mh->name) {
        free(mh->name);
    }
    mh->evh = NULL;
    mh->next = NULL;
    free(mh);
    return MOD_ERR_OK;
}

/*************************************************************************/
