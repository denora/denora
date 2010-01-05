/*
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

/*************************************************************************/

/**
 * Declare all the list's we want to use here
 **/

EvtMessageHash *EVENT[MAX_CMD_HASH];
EvtHookHash *EVENTHOOKS[MAX_CMD_HASH];
char *mod_current_evtbuffer = NULL;

static EvtMessageHash *currentemsg;
static int next_index;

/*************************************************************************/

EvtMessage *first_EvtMessage(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentemsg == NULL)
        currentemsg = EVENT[next_index++];
    if (currentemsg) {
        return currentemsg->evm;
    } else {
        return NULL;
    }
}

/*************************************************************************/

EvtMessage *next_EvtMessage(void)
{
    SET_SEGV_LOCATION();

    if (currentemsg)
        currentemsg = currentemsg->next;
    if (!currentemsg && next_index < 1024) {
        while (next_index < 1024 && currentemsg == NULL)
            currentemsg = EVENT[next_index++];
    }
    if (currentemsg) {
        return currentemsg->evm;
    } else {
        return NULL;
    }
}

/*************************************************************************/

EvtMessage *find_event(const char *name)
{
    EvtMessage *m;
    m = findEventHandler(EVENT, name);
    return m;
}

/*************************************************************************/

EvtHook *find_eventhook(const char *name)
{
    EvtHook *m;
    m = findEventHook(EVENTHOOKS, name);
    return m;
}

/*************************************************************************/

void send_event(const char *name, int argc, ...)
{
    va_list va;
    char *a;
    int idx = 0;
    char **argv;

    argv = (char **) malloc(sizeof(char *) * argc);
    va_start(va, argc);
    for (idx = 0; idx < argc; idx++) {
        a = va_arg(va, char *);
        if (a) {
            argv[idx] = sstrdup(a);
        }
    }
    va_end(va);

    alog(LOG_EXTRADEBUG, "debug: Emitting event \'%s\' (%d args)", name,
         argc);

    event_process_hook(name, argc, argv);

    /**
     * Now that the events have seen the message, free it up
     **/
    for (idx = 0; idx < argc; idx++) {
        if (!BadPtr(argv[idx])) {
            free(argv[idx]);
        }
    }
    free(argv);
}

/*************************************************************************/

void eventprintf(const char *fmt, ...)
{
    va_list args;
    char buf[16384];            /* Really huge, to try and avoid truncation */
    char *event;

    va_start(args, fmt);
    ircvsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    event = sstrdup(buf);
    event_message_process(event);
    if (event) {
        free(event);
    }
    return;
}

/*************************************************************************/

void event_message_process(char *eventbuf)
{
    int retVal = 0;
    EvtMessage *eventcurrent = NULL;
    char source[64];
    char cmd[64];
    char buf[512];              /* Longest legal IRC command line */
    char *s;
    int ac;                     /* Parameters for the command */
    char **av;
    EvtMessage *evm;

    /* zero out the buffers before we do much else */
    *buf = '\0';
    *source = '\0';
    *cmd = '\0';

    strlcpy(buf, eventbuf, sizeof(buf));

    doCleanBuffer((char *) buf);

    /* Split the buffer into pieces. */
    if (*buf == ':') {
        s = strpbrk(buf, " ");
        if (!s)
            return;
        *s = 0;
        while (isspace(*++s));
        strlcpy(source, buf + 1, sizeof(source));
        memmove(buf, s, strlen(s) + 1);
    } else {
        *source = 0;
    }
    if (!*buf)
        return;
    s = strpbrk(buf, " ");
    if (s) {
        *s = 0;
        while (isspace(*++s));
    } else
        s = buf + strlen(buf);
    strlcpy(cmd, buf, sizeof(cmd));
    ac = split_buf(s, &av);

    /* Do something with the message. */
    evm = find_event(cmd);
    if (evm) {
        if (evm->func) {
            mod_current_module_name = evm->mod_name;
            retVal = evm->func(source, ac, av);
            mod_current_module_name = NULL;
            if (retVal == MOD_CONT) {
                eventcurrent = evm->next;
                while (eventcurrent && eventcurrent->func
                       && retVal == MOD_CONT) {
                    mod_current_module_name = eventcurrent->mod_name;
                    retVal = eventcurrent->func(source, ac, av);
                    mod_current_module_name = NULL;
                    eventcurrent = eventcurrent->next;
                }
            }
        }
    }
    /* Free argument list we created */
    free(av);
}

/*************************************************************************/

void event_process_hook(const char *name, int argc, char **argv)
{
    int retVal = 0;
    EvtHook *eventcurrent = NULL;
    EvtHook *evh;

    /* Do something with the message. */
    evh = find_eventhook(name);
    if (evh) {
        if (evh->func) {
            mod_current_module_name = evh->mod_name;
            retVal = evh->func(argc, argv);
            mod_current_module_name = NULL;
            if (retVal == MOD_CONT) {
                eventcurrent = evh->next;
                while (eventcurrent && eventcurrent->func
                       && retVal == MOD_CONT) {
                    mod_current_module_name = eventcurrent->mod_name;
                    retVal = eventcurrent->func(argc, argv);
                    mod_current_module_name = NULL;
                    eventcurrent = eventcurrent->next;
                }
            }
        }
    }
}

/*************************************************************************/

/**
 * Displays a message list for a given message.
 * Again this is of little use other than debugging.
 * @param evm the message to display
 * @return 0 is returned and has no meaning 
 */
int displayEventMessage(EvtMessage * evm)
{
    EvtMessage *msg = NULL;
    int i = 0;
    alog(LOG_DEBUG, "debug: Displaying message list for %s", evm->name);
    for (msg = evm; msg; msg = msg->next) {
        alog(LOG_DEBUG, "%d: 0x%p", ++i, (void *) msg);
    }
    alog(LOG_DEBUG, "debug: end");
    return 0;
}

/*************************************************************************/

/**
 * Displays a message list for a given message.
 * Again this is of little use other than debugging.
 * @param evh the message to display
 * @return 0 is returned and has no meaning 
 */
int displayEventHook(EvtHook * evh)
{
    EvtHook *msg = NULL;
    int i = 0;
    alog(LOG_DEBUG, "debug: Displaying message list for %s", evh->name);
    for (msg = evh; msg; msg = msg->next) {
        alog(LOG_DEBUG, "debug: %d: 0x%p", ++i, (void *) msg);
    }
    alog(LOG_DEBUG, "debug: end");
    return 0;
}

/*************************************************************************/

/**
 * Display the message call stak.
 * Prints the call stack for a message based on the message name, again useful for debugging and little lese :)
 * @param name the name of the message to print info for
 * @return the return int has no relevence atm :)
 */
int displayHookFromHash(char *name)
{
    EvtHookHash *eventcurrent = NULL;
    int eventindex = 0;
    eventindex = CMD_HASH(name);

    alog(LOG_EXTRADEBUG, "debug: trying to display message %s", name);

    for (eventcurrent = EVENTHOOKS[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(name, eventcurrent->name) == 0) {
            displayEventHook(eventcurrent->evh);
        }
    }
    alog(LOG_EXTRADEBUG, "debug: done displaying message %s", name);
    return 0;
}

/*************************************************************************/

/**
 * Display the message call stak.
 * Prints the call stack for a message based on the message name, again useful for debugging and little lese :)
 * @param name the name of the message to print info for
 * @return the return int has no relevence atm :)
 */
int displayEvtMessageFromHash(char *name)
{
    EvtMessageHash *eventcurrent = NULL;
    int eventindex = 0;
    eventindex = CMD_HASH(name);

    alog(LOG_EXTRADEBUG, "debug: trying to display message %s", name);

    for (eventcurrent = EVENT[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(name, eventcurrent->name) == 0) {
            displayEventMessage(eventcurrent->evm);
        }
    }
    alog(LOG_EXTRADEBUG, "debug: done displaying message %s", name);
    return 0;
}

/*************************************************************************/

 /**
  * Create a new Message struct.
  * @param name the name of the message
  * @param func a pointer to the function to call when we recive this message
  * @return a new Message object
  **/
EvtMessage *createEventHandler(char *name,
                               int (*func) (char *source, int ac,
                                            char **av))
{
    EvtMessage *evm = NULL;
    if (!func) {
        return NULL;
    }
    if ((evm = malloc(sizeof(EvtMessage))) == NULL) {
        fatal("Out of memory!");
    }
    evm->name = sstrdup(name);
    evm->func = func;
    evm->mod_name = NULL;
    evm->next = NULL;
    return evm;
}

/*************************************************************************/

 /**
  * Create a new Message struct.
  * @param name the name of the message
  * @param func a pointer to the function to call when we recive this message
  * @return a new Message object
  **/
EvtHook *createEventHook(const char *name,
                         int (*func) (int argc, char **argv))
{
    EvtHook *evh = NULL;
    if (!func) {
        return NULL;
    }
    if ((evh = malloc(sizeof(EvtHook))) == NULL) {
        fatal("Out of memory!");
    }
    evh->name = sstrdup(name);
    evh->func = func;
    evh->mod_name = NULL;
    evh->next = NULL;
    return evh;
}

/*************************************************************************/

/** 
 * find a message in the given table. 
 * Looks up the message name in the MessageHash given
 *
 * @param msgEvtTable the message table to search for this command, will almost always be IRCD
 * @param name the name of the command were looking for
 * @return NULL if we cant find it, or a pointer to the Message if we can
 **/
EvtMessage *findEventHandler(EvtMessageHash * msgEvtTable[],
                             const char *name)
{
    int idx;
    EvtMessageHash *eventcurrent = NULL;
    if (!msgEvtTable || !name) {
        return NULL;
    }
    idx = CMD_HASH(name);

    for (eventcurrent = msgEvtTable[idx]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(name, eventcurrent->name) == 0) {
            return eventcurrent->evm;
        }
    }
    return NULL;
}

/*************************************************************************/

/** 
 * find a message in the given table. 
 * Looks up the message name in the MessageHash given
 * @param hookEvtTable the message table to search for this command, will almost always be IRCD
 * @param name the name of the command were looking for
 * @return NULL if we cant find it, or a pointer to the Message if we can
 **/
EvtHook *findEventHook(EvtHookHash * hookEvtTable[], const char *name)
{
    int idx;
    EvtHookHash *eventcurrent = NULL;
    if (!hookEvtTable || !name) {
        return NULL;
    }
    idx = CMD_HASH(name);

    for (eventcurrent = hookEvtTable[idx]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(name, eventcurrent->name) == 0) {
            return eventcurrent->evh;
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Add the given message (m) to the MessageHash marking it as a core command
 * @param msgEvtTable the MessageHash we want to add to
 * @param evm the Message we are adding
 * @return MOD_ERR_OK on a successful add.
 **/
int addCoreEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm)
{
    if (!msgEvtTable || !evm) {
        return MOD_ERR_PARAMS;
    }
    evm->core = 1;
    return addEventHandler(msgEvtTable, evm);
}

/*************************************************************************/

/**
 * Add a message to the MessageHash.
 *
 * MOD_ERR_PARAMS - if not enough parameters
 * MOD_ERR_OK     - if successful
 *
 * @param msgEvtTable the MessageHash we want to add a message to
 * @param evm the Message we want to add
 * @return MOD_ERR_OK on a successful add.
 **/
int addEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm)
{
    /* We can assume both param's have been checked by this point.. */
    int eventindex = 0;
    EvtMessageHash *eventcurrent = NULL;
    EvtMessageHash *newHash = NULL;
    EvtMessageHash *lastHash = NULL;

    if (!msgEvtTable || !evm) {
        return MOD_ERR_PARAMS;
    }

    eventindex = CMD_HASH(evm->name);

    for (eventcurrent = msgEvtTable[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(evm->name, eventcurrent->name) == 0) {      /* the msg exist's we are a addHead */
            evm->next = eventcurrent->evm;
            eventcurrent->evm = evm;
            alog(LOG_DEBUG,
                 "debug: existing msg: (0x%p), new msg (0x%p) (%s)",
                 (void *) evm->next, (void *) evm, evm->name);
            return MOD_ERR_OK;
        }
        lastHash = eventcurrent;
    }

    if ((newHash = malloc(sizeof(EvtMessageHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->name = sstrdup(evm->name);
    newHash->evm = evm;

    if (lastHash == NULL) {
        msgEvtTable[eventindex] = newHash;
    } else {
        lastHash->next = newHash;
    }
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Add a message to the MessageHash.
 * @param hookEvtTable the MessageHash we want to add a message to
 * @param evh the Message we want to add
 * @return MOD_ERR_OK on a successful add.
 **/
int addEventHook(EvtHookHash * hookEvtTable[], EvtHook * evh)
{
    /* We can assume both param's have been checked by this point.. */
    int eventindex = 0;
    EvtHookHash *eventcurrent = NULL;
    EvtHookHash *newHash = NULL;
    EvtHookHash *lastHash = NULL;

    if (!hookEvtTable || !evh) {
        return MOD_ERR_PARAMS;
    }

    eventindex = CMD_HASH(evh->name);

    for (eventcurrent = hookEvtTable[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(evh->name, eventcurrent->name) == 0) {      /* the msg exist's we are a addHead */
            evh->next = eventcurrent->evh;
            eventcurrent->evh = evh;
            alog(LOG_DEBUG,
                 "debug: existing msg: (0x%p), new msg (0x%p) (%s)",
                 (void *) evh->next, (void *) evh, evh->name);
            return MOD_ERR_OK;
        }
        lastHash = eventcurrent;
    }

    if ((newHash = malloc(sizeof(EvtHookHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->name = sstrdup(evh->name);
    newHash->evh = evh;

    if (lastHash == NULL)
        hookEvtTable[eventindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Add the given message (m) to the MessageHash marking it as a core command
 * @param hookEvtTable the MessageHash we want to add to
 * @param evh the Message we are adding
 * @return MOD_ERR_OK on a successful add.
 **/
int addCoreEventHook(EvtHookHash * hookEvtTable[], EvtHook * evh)
{
    if (!hookEvtTable || !evh) {
        return MOD_ERR_PARAMS;
    }
    evh->core = 1;
    return addEventHook(hookEvtTable, evh);
}

/*************************************************************************/

/**
 * Add a module message to the IRCD message hash
 * @param evm the Message to add
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleAddEventHandler(EvtMessage * evm)
{
    int status;

    if (!evm) {
        return MOD_ERR_PARAMS;
    }

    /* ok, this appears to be a module adding a message from outside
     * of DenoraInit, try to look up its module struct for it */
    if ((mod_current_module_name) && (!mod_current_module)) {
        mod_current_module = findModule(mod_current_module_name);
    }

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }                           /* shouldnt happen */
    evm->core = 0;
    if (!evm->mod_name) {
        evm->mod_name = sstrdup(mod_current_module->name);
    }

    status = addEventHandler(EVENT, evm);
    if (denora->debug) {
        displayEvtMessageFromHash(evm->name);
    }
    return status;
}

/*************************************************************************/

/**
 * Add a module message to the IRCD message hash
 * @param evh the Message to add
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleAddEventHook(EvtHook * evh)
{
    int status;

    if (!evh) {
        return MOD_ERR_PARAMS;
    }

    if ((mod_current_module_name) && (!mod_current_module)) {
        mod_current_module = findModule(mod_current_module_name);
    }

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }                           /* shouldnt happen */
    evh->core = 0;
    if (!evh->mod_name) {
        evh->mod_name = sstrdup(mod_current_module->name);
    }

    status = addEventHook(EVENTHOOKS, evh);
    if (denora->debug) {
        displayHookFromHash(evh->name);
    }
    return status;
}

/*************************************************************************/

/**
 * remove the given message from the IRCD message hash
 * @param name the name of the message to remove
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleEventDelHandler(char *name)
{
    EvtMessage *evm;
    int status;

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }
    evm = findEventHandler(EVENT, name);
    if (!evm) {
        return MOD_ERR_NOEXIST;
    }

    status = delEventHandler(EVENT, evm, mod_current_module->name);
    if (denora->debug) {
        displayEvtMessageFromHash(evm->name);
    }
    return status;
}

/*************************************************************************/

/**
 * remove the given message from the IRCD message hash
 * @param name the name of the message to remove
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleEventDelHook(const char *name)
{
    EvtHook *evh;
    int status;

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }
    evh = findEventHook(EVENTHOOKS, name);
    if (!evh) {
        return MOD_ERR_NOEXIST;
    }

    status = delEventHook(EVENTHOOKS, evh, mod_current_module->name);
    if (denora->debug) {
        displayHookFromHash(evh->name);
    }
    return status;
}

/*************************************************************************/

/**
 * remove the given message from the given message hash, for the given module
 * @param msgEvtTable which MessageHash we are removing from
 * @param evm the Message we want to remove
 * @param mod_name the name of the module we are removing
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int delEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm,
                    char *mod_name)
{
    int eventindex = 0;
    EvtMessageHash *eventcurrent = NULL;
    EvtMessageHash *lastHash = NULL;
    EvtMessage *tail = NULL, *last = NULL;

    if (!evm || !msgEvtTable) {
        return MOD_ERR_PARAMS;
    }

    eventindex = CMD_HASH(evm->name);

    for (eventcurrent = msgEvtTable[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(evm->name, eventcurrent->name) == 0) {
            if (!lastHash) {
                tail = eventcurrent->evm;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                eventcurrent->evm = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    msgEvtTable[eventindex] = eventcurrent->next;
                    free(eventcurrent->name);
                    return MOD_ERR_OK;
                }
            } else {
                tail = eventcurrent->evm;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                eventcurrent->evm = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    lastHash->next = eventcurrent->next;
                    free(eventcurrent->name);
                    return MOD_ERR_OK;
                }
            }
        }
        lastHash = eventcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

/**
 * remove the given message from the given message hash, for the given module
 * @param hookEvtTable which MessageHash we are removing from
 * @param evh the Message we want to remove
 * @param mod_name the name of the module we are removing
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int delEventHook(EvtHookHash * hookEvtTable[], EvtHook * evh,
                 char *mod_name)
{
    int eventindex = 0;
    EvtHookHash *eventcurrent = NULL;
    EvtHookHash *lastHash = NULL;
    EvtHook *tail = NULL, *last = NULL;

    if (!evh || !hookEvtTable) {
        return MOD_ERR_PARAMS;
    }

    eventindex = CMD_HASH(evh->name);

    for (eventcurrent = hookEvtTable[eventindex]; eventcurrent;
         eventcurrent = eventcurrent->next) {
        if (stricmp(evh->name, eventcurrent->name) == 0) {
            if (!lastHash) {
                tail = eventcurrent->evh;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                eventcurrent->evh = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    hookEvtTable[eventindex] = eventcurrent->next;
                    free(eventcurrent->name);
                    return MOD_ERR_OK;
                }
            } else {
                tail = eventcurrent->evh;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                eventcurrent->evh = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    lastHash->next = eventcurrent->next;
                    free(eventcurrent->name);
                    return MOD_ERR_OK;
                }
            }
        }
        lastHash = eventcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

/**
 * Destory a message, freeing its memory.
 * @param evm the message to be destroyed
 * @return MOD_ERR_OK on success
 **/
int destroyEventHandler(EvtMessage * evm)
{
    if (!evm) {
        return MOD_ERR_PARAMS;
    }
    if (evm->name) {
        free(evm->name);
    }
    evm->func = NULL;
    if (evm->mod_name) {
        free(evm->mod_name);
    }
    evm->next = NULL;
    free(evm);
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Destory a message, freeing its memory.
 * @param evh the message to be destroyed
 * @return MOD_ERR_OK on success
 **/
int destroyEventHook(EvtHook * evh)
{
    if (!evh) {
        return MOD_ERR_PARAMS;
    }
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





/* EOF */
