/*
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: modules.c 682 2006-08-28 19:50:32Z trystan $
 *
 */

#include "denora.h"
CommandHash *STATSERV[MAX_CMD_HASH];
MessageHash *IRCD[MAX_CMD_HASH];
ModuleHash *MODULE_HASH[MAX_CMD_HASH];

Module *mod_current_module;
char *mod_current_module_name = NULL;
char *mod_current_buffer = NULL;
int mod_current_op;
User *mod_current_user;
ModuleCallBack *moduleCallBackHead = NULL;
ModuleQueue *mod_operation_queue = NULL;

int destroyModuleHash(ModuleHash * m);
ModuleHash *first_modulehash(void);
ModuleHash *next_modulehash(void);
Module *first_module(void);
Module *next_module(void);

void queueModuleOperation(Module * m, ModuleOperation op, User * u);

static CommandHash *currentcmd;
static MessageHash *current;
static ModuleHash *currentmod;
static int next_index;

/*************************************************************************/

/**
 * Automaticaly load modules at startup.
 * This will load modules at startup before the IRCD link is attempted, this
 * allows admins to have a module relating to ircd support load
 */
void modules_init(void)
{
#ifdef USE_MODULES
    int idx;
    Module *m;
    int status;
    Conf_Modules *amodules = modules;

    SET_SEGV_LOCATION();

    for (idx = 0; idx < ModulesNumber; idx++) {
        m = findModule(amodules->autoload[idx]);
        if (!m) {
            m = createModule(amodules->autoload[idx]);
            mod_current_module = m;
            mod_current_user = NULL;
            alog(LOG_NORMAL, langstr(ALOG_MOD_TRY_LOAD),
                 mod_current_module->name);
            status = loadModule(mod_current_module, NULL);
            alog(LOG_NORMAL, langstr(ALOG_MOD_STATUS), status,
                 ModuleGetErrStr(status));
            if (status != MOD_ERR_OK) {
                destroyModule(m);
            }
            mod_current_module = NULL;
            mod_current_user = NULL;
        }
    }
#endif
}

/*******************************************************************************
 * Command Functions
 *******************************************************************************/
/**
 * Create a Command struct ready for use in denora.
 * @param name the name of the command
 * @param func pointer to the function to execute when command is given
 * @param has_priv pointer to function to check user priv's
 * @param help_all help file index for all users
 * @param help_reg help file index for all regustered users
 * @param help_oper help file index for all opers
 * @param help_admin help file indenx for all stats admins
 * @return a "ready to use" Command struct will be returned
 */
Command *createCommand(const char *name,
                       int (*func) (User * u, int ac, char **av),
                       int (*has_priv) (User * u), int help_all,
                       int help_reg, int help_oper, int help_admin)
{
    Command *c;
    if (!name || !*name) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    if ((c = malloc(sizeof(Command))) == NULL) {
        fatal("Out of memory!");
    }
    c->name = sstrdup(name);
    c->routine = func;
    c->has_priv = has_priv;
    c->helpmsg_all = help_all;
    c->helpmsg_reg = help_reg;
    c->helpmsg_admin = help_admin;
    c->helpmsg_oper = help_oper;
    c->help_param1 = NULL;
    c->help_param2 = NULL;
    c->help_param3 = NULL;
    c->help_param4 = NULL;
    c->next = NULL;
    c->mod_name = NULL;
    c->service = NULL;
    c->all_help = NULL;
    c->regular_help = NULL;
    c->admin_help = NULL;
    return c;
}

/*************************************************************************/

/**
 * Destroy a command struct freeing any memory.
 * @param c Command to destroy
 * @return MOD_ERR_OK on success, anything else on fail
 */
int destroyCommand(Command * c)
{
    SET_SEGV_LOCATION();

    if (!c) {
        return MOD_ERR_PARAMS;
    }
    if (c->core == 1 && !denora->delayed_quit) {
        return MOD_ERR_UNKNOWN;
    }
    if (c->name) {
        free(c->name);
    }
    c->routine = NULL;
    c->has_priv = NULL;
    c->helpmsg_all = -1;
    c->helpmsg_reg = -1;
    c->helpmsg_admin = -1;
    if (c->help_param1) {
        free(c->help_param1);
    }
    if (c->help_param2) {
        free(c->help_param2);
    }
    if (c->help_param3) {
        free(c->help_param3);
    }
    if (c->help_param4) {
        free(c->help_param4);
    }
    if (c->mod_name) {
        free(c->mod_name);
    }
    if (c->service) {
        free(c->service);
    }
    c->next = NULL;
    free(c);
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Add a CORE command ot the given command hash
 * @param cmdTable the command table to add the command to
 * @param c the command to add
 * @return MOD_ERR_OK on success
 */
int addCoreCommand(CommandHash * cmdTable[], Command * c)
{
    SET_SEGV_LOCATION();

    if (!cmdTable || !c) {
        return MOD_ERR_PARAMS;
    }
    c->core = 1;
    c->next = NULL;
    return addCommand(cmdTable, c, 0);
}

/*************************************************************************/

/**
 * Add a module provided command to the given service.
 * e.g. moduleAddCommand(NICKSERV,c,MOD_HEAD);
 * @param cmdTable the services to add the command to
 * @param c the command to add
 * @param pos the position to add to, MOD_HEAD, MOD_TAIL, MOD_UNIQUE
 * @see createCommand
 * @return MOD_ERR_OK on successfully adding the command
 */
int moduleAddCommand(CommandHash * cmdTable[], Command * c, int pos)
{
    int status;

    SET_SEGV_LOCATION();

    if (!cmdTable || !c) {
        return MOD_ERR_PARAMS;
    }
    /* ok, this appears to be a module adding a command from outside
     * of DenoraInit, try to look up its module struct for it */
    if ((mod_current_module_name) && (!mod_current_module)) {
        mod_current_module = findModule(mod_current_module_name);
    }

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }                           /* shouldnt happen */
    c->core = 0;
    if (!c->mod_name) {
        c->mod_name = sstrdup(mod_current_module->name);
    }


    if (cmdTable == STATSERV) {
        if (s_StatServ) {
            c->service = sstrdup(s_StatServ);
        } else {
            return MOD_ERR_NOSERVICE;
        }
    } else
        c->service = sstrdup("Unknown");

    if (denora->debug) {
        displayCommandFromHash(cmdTable, c->name);
    }
    status = addCommand(cmdTable, c, pos);
    if (denora->debug) {
        displayCommandFromHash(cmdTable, c->name);
    }
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL, "ERROR! [%d]", status);
    }
    return status;
}

/*************************************************************************/

/**
 * Delete a command from the service given.
 * @param cmdTable the cmdTable for the services to remove the command from
 * @param name the name of the command to delete from the service
 * @return returns MOD_ERR_OK on success
 */
int moduleDelCommand(CommandHash * cmdTable[], char *name)
{
    Command *c = NULL;
    Command *cmd = NULL;
    int status = 0;

    SET_SEGV_LOCATION();

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }

    c = findCommand(cmdTable, name);
    if (!c) {
        return MOD_ERR_NOEXIST;
    }


    for (cmd = c; cmd; cmd = cmd->next) {
        if (cmd->mod_name
            && stricmp(cmd->mod_name, mod_current_module->name) == 0) {
            if (denora->debug) {
                displayCommandFromHash(cmdTable, name);
            }
            status = delCommand(cmdTable, cmd, mod_current_module->name);
            if (denora->debug) {
                displayCommandFromHash(cmdTable, name);
            }
        }
    }
    return status;
}

/*************************************************************************/

/**
 * Output the command stack into the log files.
 * This will print the call-stack for a given command into the log files, very useful for debugging.
 * @param cmdTable the command table to read from
 * @param name the name of the command to print
 * @return 0 is returned, it has no relevence yet :)
 */
int displayCommandFromHash(CommandHash * cmdTable[], char *name)
{
    CommandHash *ccurrent = NULL;
    int modindex = 0;
    modindex = CMD_HASH(name);

    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, langstr(ALOG_CMD_DISPLAY_START), name);

    for (ccurrent = cmdTable[modindex]; ccurrent;
         ccurrent = ccurrent->next) {
        if (stricmp(name, ccurrent->name) == 0) {
            displayCommand(ccurrent->c);
        }
    }
    alog(LOG_EXTRADEBUG, langstr(ALOG_CMD_DISPLAY_END), name);
    return 0;
}

/*************************************************************************/

/**
 * Output the command stack into the log files.
 * This will print the call-stack for a given command into the log files, very useful for debugging.
 * @param c the command struct to print
 * @return 0 is returned, it has no relevence yet :)
 */

int displayCommand(Command * c)
{
    Command *cmd = NULL;
    int i = 0;

    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, langstr(ALOG_CMD_DISPLAY_LIST), c->name);
    for (cmd = c; cmd; cmd = cmd->next) {
        alog(LOG_DEBUG, "%d:  0x%p", ++i, (void *) cmd);
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
int displayMessageFromHash(char *name)
{
    MessageHash *mcurrent = NULL;
    int modindex = 0;
    modindex = CMD_HASH(name);

    SET_SEGV_LOCATION();

    alog(LOG_EXTRADEBUG, langstr(ALOG_MSG_DISPLAY_START), name);
    for (mcurrent = IRCD[modindex]; mcurrent; mcurrent = mcurrent->next) {
        if (stricmp(name, mcurrent->name) == 0) {
            displayMessage(mcurrent->m);
        }
    }
    alog(LOG_EXTRADEBUG, langstr(ALOG_MSG_DISPLAY_END), name);
    return 0;
}

/*************************************************************************/

/**
 * Displays a message list for a given message.
 * Again this is of little use other than debugging.
 * @param m the message to display
 * @return 0 is returned and has no meaning 
 */
int displayMessage(Message * m)
{
    Message *msg = NULL;
    int i = 0;

    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, langstr(ALOG_MSG_DISPLAY_LIST), m->name);
    for (msg = m; msg; msg = msg->next) {
        alog(LOG_DEBUG, "%d: 0x%p", ++i, (void *) msg);
    }
    alog(LOG_DEBUG, "debug: end");
    return 0;
}

/*************************************************************************/

/**
 * Add a command to a command table.
 * only add if were unique, pos = 0;
 * if we want it at the "head" of that command, pos = 1
 * at the tail, pos = 2
 * @param cmdTable the table to add the command to
 * @param c the command to add
 * @param pos the position in the cmd call stack to add the command
 * @return MOD_ERR_OK will be returned on success.
 */
int addCommand(CommandHash * cmdTable[], Command * c, int pos)
{
    /* We can assume both param's have been checked by this point.. */
    int modindex = 0;
    CommandHash *ccurrent = NULL;
    CommandHash *newHash = NULL;
    CommandHash *lastHash = NULL;
    Command *tail = NULL;

    SET_SEGV_LOCATION();

    if (!cmdTable || !c || (pos < 0 || pos > 2)) {
        return MOD_ERR_PARAMS;
    }

    if (mod_current_module_name && !c->mod_name) {
        return MOD_ERR_NO_MOD_NAME;
    }

    modindex = CMD_HASH(c->name);

    for (ccurrent = cmdTable[modindex]; ccurrent;
         ccurrent = ccurrent->next) {
        if ((c->service) && (ccurrent->c) && (ccurrent->c->service)
            && (!strcmp(c->service, ccurrent->c->service) == 0)) {
            continue;
        }
        if ((stricmp(c->name, ccurrent->name) == 0)) {  /* the cmd exist's we are a addHead */
            if (pos == 1) {
                c->next = ccurrent->c;
                ccurrent->c = c;
                alog(LOG_DEBUG,
                     "debug: existing cmd: (0x%p), new cmd (0x%p)",
                     (void *) c->next, (void *) c);
                return MOD_ERR_OK;
            } else if (pos == 2) {

                tail = ccurrent->c;
                while (tail->next)
                    tail = tail->next;
                alog(LOG_DEBUG,
                     "debug: existing cmd: (0x%p), new cmd (0x%p)",
                     (void *) tail, (void *) c);
                tail->next = c;
                c->next = NULL;

                return MOD_ERR_OK;
            } else
                return MOD_ERR_EXISTS;
        }
        lastHash = ccurrent;
    }

    if ((newHash = malloc(sizeof(CommandHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->name = sstrdup(c->name);
    newHash->c = c;

    if (lastHash == NULL)
        cmdTable[modindex] = newHash;
    else
        lastHash->next = newHash;

    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Remove a command from the command hash.
 * @param cmdTable the command table to remove the command from
 * @param c the command to remove
 * @param mod_name the name of the module who owns the command
 * @return MOD_ERR_OK will be returned on success
 */
int delCommand(CommandHash * cmdTable[], Command * c, char *mod_name)
{
    int modindex = 0;
    CommandHash *CurrentCommand = NULL;
    CommandHash *lastHash = NULL;
    Command *tail = NULL, *last = NULL;

    SET_SEGV_LOCATION();

    if (!c || !cmdTable) {
        return MOD_ERR_PARAMS;
    }

    modindex = CMD_HASH(c->name);
    for (CurrentCommand = cmdTable[modindex]; CurrentCommand;
         CurrentCommand = CurrentCommand->next) {
        if (stricmp(c->name, CurrentCommand->name) == 0) {
            if (!lastHash) {
                tail = CurrentCommand->c;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                CurrentCommand->c = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    cmdTable[modindex] = CurrentCommand->next;
                    free(CurrentCommand->name);
                    return MOD_ERR_OK;
                }
            } else {
                tail = CurrentCommand->c;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                CurrentCommand->c = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    lastHash->next = CurrentCommand->next;
                    free(CurrentCommand->name);
                    return MOD_ERR_OK;
                }
            }
        }
        lastHash = CurrentCommand;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

/**
 * Search the command table gieven for a command.
 * @param cmdTable the name of the command table to search
 * @param name the name of the command to look for
 * @return returns a pointer to the found command struct, or NULL
 */
Command *findCommand(CommandHash * cmdTable[], const char *name)
{
    int idx;
    CommandHash *ccurrent = NULL;

    SET_SEGV_LOCATION();

    if (!cmdTable || !name) {
        return NULL;
    }

    idx = CMD_HASH(name);

    for (ccurrent = cmdTable[idx]; ccurrent; ccurrent = ccurrent->next) {
        if (stricmp(name, ccurrent->name) == 0) {
            return ccurrent->c;
        }
    }
    return NULL;
}

/*******************************************************************************
 * Message Functions
 *******************************************************************************/

 /**
  * Create a new Message struct.
  * @param name the name of the message
  * @param func a pointer to the function to call when we recive this message
  * @return a new Message object
  **/
Message *createMessage(const char *name,
                       int (*func) (char *source, int ac, char **av))
{
    Message *m = NULL;

    SET_SEGV_LOCATION();

    if (!name || !func) {
        return NULL;
    }
    if ((m = malloc(sizeof(Message))) == NULL) {
        fatal("Out of memory!");
    }
    m->name = sstrdup(name);
    m->func = func;
    m->mod_name = NULL;
    m->next = NULL;
    return m;
}

/*************************************************************************/

/** 
 * find a message in the given table. 
 * Looks up the message name in the MessageHash given
 * @param msgTable the message table to search for this command, will almost always be IRCD
 * @param name the name of the command were looking for
 * @return NULL if we cant find it, or a pointer to the Message if we can
 **/
Message *findMessage(MessageHash * msgTable[], const char *name)
{
    int idx;
    MessageHash *mcurrent = NULL;

    SET_SEGV_LOCATION();

    if (!msgTable || !name) {
        return NULL;
    }
    idx = CMD_HASH(name);

    for (mcurrent = msgTable[idx]; mcurrent; mcurrent = mcurrent->next) {
        if (ircd->tokencaseless) {
            if (stricmp(name, mcurrent->name) == 0) {
                return mcurrent->m;
            }
        } else {
            if (strcmp(name, mcurrent->name) == 0) {
                return mcurrent->m;
            }
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Automaticaly load modules at startup.
 * This will load modules at startup before the IRCD link is attempted, this
 * allows admins to have a module relating to ircd support load
 */
void core_modules_init(void)
{
#ifdef USE_MODULES
    Module *m;
    char *mname = NULL;
#ifndef _WIN32
    DIR *dirp;
    struct dirent *dp;
#endif
    char buf[BUFSIZE];
    int status;

#ifndef _WIN32
    ircsnprintf(buf, BUFSIZE, "%s/core", denora->dir);
#else
    ircsnprintf(buf, BUFSIZE, "\\%s", "core");
#endif

#ifndef _WIN32
    if ((dirp = opendir(buf)) == NULL) {
        alog(LOG_ERROR, "ERROR: cannot open directory (%s)", buf);
        return;
    }

    while ((dp = readdir(dirp)) != NULL) {
#ifndef CYGWIN_VERSION_CHECK_FOR_NEEDS_D_INO
        if (dp->d_ino == 0) {
            continue;
        }
#endif
        if (!stricmp(dp->d_name, ".") || !stricmp(dp->d_name, "..")
            || !stricmp(dp->d_name, "runtime")) {
            continue;
        }
        mname = sstrdup(dp->d_name);
        mname[strlen(dp->d_name) - 3] = '\0';
        m = findModule(mname);
        if (!m) {
            m = createCoreModule(mname);
            mod_current_module = m;
            mod_current_user = NULL;
            alog(LOG_DEBUG, "trying to load [%s]",
                 mod_current_module->name);
            status = loadCoreModule(mod_current_module, NULL);
            alog(LOG_DEBUG, langstr(ALOG_MOD_STATUS), status,
                 ModuleGetErrStr(status));
            if (status != MOD_ERR_OK) {
                destroyModule(m);
            }
            mod_current_module = NULL;
            mod_current_user = NULL;
        }
        free(mname);
    }
    closedir(dirp);
#else
    Win32LoadCoreDir(buf);
#endif
#endif
}

/*************************************************************************/

/**
 * Loads a given module.
 * @param m the module to load
 * @param u the user who loaded it, NULL for auto-load
 * @return MOD_ERR_OK on success, anything else on fail
 */
int loadCoreModule(Module * m, User * u)
{
#ifdef USE_MODULES
    char buf[4096];
    const char *err;
    int (*func) (int, char **);
    int (*version) (void);
    int ret = 0;
    char *argv[1];
    int argc = 0;
    Module *m2;
#ifdef _WIN32
    char buffer[_MAX_PATH];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
#endif
    if (!m || !m->name) {
        return MOD_ERR_PARAMS;
    }
    if (m->handle) {
        return MOD_ERR_EXISTS;
    }
    if ((m2 = findModule(m->name)) != NULL) {
        return MOD_ERR_EXISTS;
    }
#ifndef _WIN32
    ircsnprintf(buf, BUFSIZE, "%s/core/runtime/%s%s.XXXXXX", denora->dir, m->name, MODULE_EXT); /* Get full path with module extension */
#else
    ircsnprintf(buf, BUFSIZE, "%s\\core\\runtime\\%s%s.XXXXXX", buffer, m->name, MODULE_EXT);   /* Get full path with module extension */
#endif
    /* Don't skip return value checking! -GD */
    if ((ret = moduleCopyCoreFile(m->name, buf)) != MOD_ERR_OK) {
        if (u)
            notice_lang(s_StatServ, u, STAT_MODULE_LOAD_FAIL, m->name);
        m->filename = sstrdup(buf);
        return ret;
    }

    /*
     * Reset dlerror() since it can contain error from previous
     * call to dlopen()/dlsym().
     */
    err = deno_moderr();
    m->filename = sstrdup(buf);

    deno_modclearerr();
    m->handle = deno_modopen(m->filename);
    if ((err = deno_moderr()) != NULL) {
        alog(LOG_NORMAL, langstr(ALOG_MOD_ERROR), err);
        if (u) {
            notice_lang(s_StatServ, u, STAT_MODULE_LOAD_FAIL, m->name);
        }
        return MOD_ERR_NOLOAD;
    }
    deno_modclearerr();
    func = deno_modsym(m->handle, "DenoraInit");
    if ((err = deno_moderr()) != NULL) {
        deno_modclose(m->handle);       /* If no DenoraInit - it isnt an Denora Module, close it */
        return MOD_ERR_NOLOAD;
    }
    if (func) {
        version =
            (int (*)()) deno_modsym(m->handle, "getDenoraBuildVersion");
        if (version) {
            if (version() >= VERSION_BUILD) {
                alog(LOG_DEBUG,
                     "Module %s compiled against current or newer Denora revision %d, this is %d",
                     m->name, version(), VERSION_BUILD);
            } else {
                alog(LOG_DEBUG,
                     "Module %s is compiled against an old version of Denora (%d) current is %d",
                     m->name, version(), VERSION_BUILD);
                alog(LOG_DEBUG,
                     "Rebuild module %s against the current version to resolve this error",
                     m->name);
                notice(s_StatServ, u->nick,
                       "Module %s is compiled against an old version of Denora (%d) current is %d",
                       m->name, version(), VERSION_BUILD);
                notice(s_StatServ, u->nick,
                       "Rebuild module %s against the current version to resolve this error",
                       m->name);
                deno_modclose(m->handle);
                deno_modclearerr();
                return MOD_ERR_NOLOAD;
            }
        } else {
            deno_modclose(m->handle);
            deno_modclearerr();
            alog(LOG_DEBUG,
                 "Module %s is compiled against an older version of Denora (unknown)",
                 m->name);
            alog(LOG_DEBUG,
                 "Rebuild module %s against the current version to resolve this error",
                 m->name);
            notice(s_StatServ, u->nick,
                   "Module %s is compiled against an older version of Denora (unknown)",
                   m->name);
            notice(s_StatServ, u->nick,
                   "Rebuild module %s against the current version to resolve this error",
                   m->name);
            return MOD_ERR_NOLOAD;
        }
        mod_current_module_name = m->name;
        /* argv[0] is the user if there was one, or NULL if not */
        if (u) {
            argv[0] = sstrdup(u->nick);
        } else {
            argv[0] = NULL;
        }
        argc++;

        ret = func(argc, argv); /* exec AhearnInit */
        if (u) {
            free(argv[0]);
        }
        if (ret == MOD_STOP) {
            alog(LOG_DEBUG, "%s requested unload...", m->name);
            unloadModule(m, NULL);
            mod_current_module_name = NULL;
            return MOD_ERR_NOLOAD;
        }

        mod_current_module_name = NULL;
    }

    addModule(m);
    return MOD_ERR_OK;

#else
    return MOD_ERR_NOLOAD;
#endif
}

/*************************************************************************/

int moduleCopyCoreFile(char *name, char *output)
{
#ifdef USE_MODULES
    int ch;
    FILE *source, *target;
    char input[4096];
    int srcfp;
#ifdef _WIN32
    char *srcop;
    char buffer[_MAX_PATH];
    char win32filename[MAXPATHLEN];
#ifdef MSVS2005
    errno_t err;
#endif

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
#endif
#ifndef _WIN32
    ircsnprintf(input, sizeof(input), "%s/core/%s%s", denora->dir, name, MODULE_EXT);   /* Get full path with module extension */
    if ((srcfp = mkstemp(output)) == -1) {
        return MOD_ERR_FILE_IO;
    }
#else
    ircsnprintf(input, sizeof(input), "%s\\core\\%s%s", buffer, name, MODULE_EXT);      /* Get full path with module extension */
    srcop = _mktemp(output);
    if (srcop == NULL) {
        return MOD_ERR_FILE_IO;
    }
#endif
    alog(LOG_DEBUG, "Runtime module location: %s", output);
    if ((source = FileOpen(input, FILE_READ)) == NULL) {
        alog(LOG_ERROR, "ERROR: the file (%s) does not exist", input);
        return MOD_ERR_NOEXIST;
    }
#ifndef _WIN32
    if ((target = fdopen(srcfp, "w")) == NULL) {
#else
    if ((target = FileOpen(srcop, FILE_WRITE)) == NULL) {
#endif
        alog(LOG_ERROR, "ERROR: the file (%s) can not be open for writing",
             output);
        return MOD_ERR_FILE_IO;
    }
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, target);
    }
    fclose(source);
    if (fclose(target) != 0) {
        return MOD_ERR_FILE_IO;
    }
#endif
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Automaticaly load modules at startup, delayed.
 * This function waits until the IRCD link has been made, and then attempts
 * to load the specified modules.
 */
void modules_delayed_init(void)
{
#ifdef USE_MODULES
    int idx;
    Module *m;
    Conf_Modules *amodules = modules;
    int status;

    SET_SEGV_LOCATION();

    for (idx = 0; idx < ModulesDelayedNumber; idx++) {
        m = findModule(amodules->delayed[idx]);
        if (!m) {
            m = createModule(amodules->delayed[idx]);
            mod_current_module = m;
            mod_current_user = NULL;
            alog(LOG_NORMAL, langstr(ALOG_MOD_TRY_LOAD),
                 mod_current_module->name);
            status = loadModule(mod_current_module, NULL);
            alog(LOG_DEBUG, langstr(ALOG_MOD_STATUS), status,
                 ModuleGetErrStr(status));
            if (status != MOD_ERR_OK) {
                destroyModule(m);
            }
            mod_current_module = NULL;
            mod_current_user = NULL;
        }
    }
#endif
}

/*************************************************************************/

/**
 * Create a new module, setting up the default values as needed.
 * @param filename the filename of the new module
 * @return a newly created module struct
 */
Module *createModule(char *filename)
{
    Module *m;
    if (!filename) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    if ((m = malloc(sizeof(Module))) == NULL) {
        fatal("Out of memory!");
    }
    m->name = sstrdup(filename);        /* Our Name */
    m->handle = NULL;           /* Handle */
    m->version = NULL;
    m->author = NULL;
    m->operHelp = NULL;
    m->type = THIRD;

    return m;                   /* return a nice new module */
}

/*************************************************************************/

/**
 * Create a new module, setting up the default values as needed.
 * @param filename the filename of the new module
 * @return a newly created module struct
 */
Module *createCoreModule(char *filename)
{
    Module *m;
    if (!filename) {
        return NULL;
    }

    SET_SEGV_LOCATION();

    if ((m = malloc(sizeof(Module))) == NULL) {
        fatal("Out of memory!");
    }
    m->name = sstrdup(filename);        /* Our Name */
    m->handle = NULL;           /* Handle */
    m->version = NULL;
    m->author = NULL;
    m->operHelp = NULL;
    m->type = CORE;

    return m;                   /* return a nice new module */
}

/*************************************************************************/

/**
 * Destroy the module.
 * free up all memory used by our module struct.
 * @param m the module to free
 * @return MOD_ERR_OK on success, anything else on fail
 */
int destroyModule(Module * m)
{
    SET_SEGV_LOCATION();

    if (!m) {
        return MOD_ERR_PARAMS;
    }
    if (m->name) {
        free(m->name);
    }
    if (m->filename) {
        remove(m->filename);
        free(m->filename);
    }
    m->handle = NULL;
    if (m->author) {
        free(m->author);
    }
    if (m->version) {
        free(m->version);
    }
    /* No need to free our cmd/msg list, as they will always be empty by the module is destroyed */
    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyModuleHash(ModuleHash * m)
{
    SET_SEGV_LOCATION();

    if (!m) {
        return MOD_ERR_PARAMS;
    }
    if (m->name) {
        free(m->name);
    }
    /* No need to free our cmd/msg list, as they will always be empty by the module is destroyed */
    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Add the module to the list of currently loaded modules.
 * @param m the currently loaded module
 * @return MOD_ERR_OK on success, anything else on fail
 */
int addModule(Module * m)
{
    int modindex = 0;
    ModuleHash *mcurrent = NULL;
    ModuleHash *newHash = NULL;
    ModuleHash *lastHash = NULL;

    SET_SEGV_LOCATION();

    modindex = CMD_HASH(m->name);

    for (mcurrent = MODULE_HASH[modindex]; mcurrent;
         mcurrent = mcurrent->next) {
        if (stricmp(m->name, mcurrent->name) == 0)
            return MOD_ERR_EXISTS;
        lastHash = mcurrent;
    }

    if ((newHash = malloc(sizeof(ModuleHash))) == NULL) {
        fatal("Out of memory");
    }
    m->time = time(NULL);
    newHash->next = NULL;
    newHash->name = sstrdup(m->name);
    newHash->m = m;

    if (lastHash == NULL)
        MODULE_HASH[modindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Remove the module from the list of loaded modules.
 * @param m module to remove
 * @return MOD_ERR_OK on success anything else on fail
 */
int delModule(Module * m)
{
    int modindex = 0;
    ModuleHash *mcurrent = NULL;
    ModuleHash *lastHash = NULL;

    SET_SEGV_LOCATION();

    if (!m) {
        return MOD_ERR_PARAMS;
    }

    modindex = CMD_HASH(m->name);

    for (mcurrent = MODULE_HASH[modindex]; mcurrent;
         mcurrent = mcurrent->next) {
        if (stricmp(m->name, mcurrent->name) == 0) {
            if (!lastHash) {
                MODULE_HASH[modindex] = mcurrent->next;
            } else {
                lastHash->next = mcurrent->next;
            }
            destroyModule(mcurrent->m);
            free(mcurrent->name);
            free(mcurrent);
            return MOD_ERR_OK;
        }
        lastHash = mcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
Module *findModule(char *name)
{
    int idx;
    ModuleHash *mcurrent = NULL;

    if (!name) {
        return NULL;
    }
    idx = CMD_HASH(name);

    SET_SEGV_LOCATION();

    for (mcurrent = MODULE_HASH[idx]; mcurrent; mcurrent = mcurrent->next) {
        if (stricmp(name, mcurrent->name) == 0) {
            return mcurrent->m;
        }
    }
    return NULL;

}

/*************************************************************************/

/** 
 * Copy the module from the modules folder to the runtime folder.
 * This will prevent module updates while the modules is loaded from
 * triggering a segfault, as the actaul file in use will be in the 
 * runtime folder.
 * @param name the name of the module to copy
 * @param output is the output filename
 * @return MOD_ERR_OK on success
 */
int moduleCopyFile(char *name, char *output)
{
#ifdef USE_MODULES
    int ch;
    FILE *source, *target;
    int srcfp;
    char input[4096];
#ifdef MSVS2005
    errno_t err;
#endif
#ifdef _WIN32
    char *srcop;
    char buffer[_MAX_PATH];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
    ircsnprintf(input, sizeof(input), "%s%s%s%s", buffer, MODULE_PATH,
                name, MODULE_EXT);
    srcop = _mktemp(output);
    if (srcop == NULL) {
        return MOD_ERR_FILE_IO;
    }
#else
    ircsnprintf(input, sizeof(input), "%s%s%s", MODULE_PATH, name,
                MODULE_EXT);
    if ((srcfp = mkstemp(output)) == -1) {
        return MOD_ERR_FILE_IO;
    }
#endif

    SET_SEGV_LOCATION();

    if ((source = FileOpen(input, FILE_READ)) == NULL) {
        return MOD_ERR_NOEXIST;
    }
#ifndef _WIN32
    if ((target = fdopen(srcfp, "w")) == NULL) {
#else
    if ((target = FileOpen(srcop, FILE_WRITE)) == NULL) {
#endif
        return MOD_ERR_FILE_IO;
    }
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, target);
    }
    fclose(source);
    if (fclose(target) != 0) {
        return MOD_ERR_FILE_IO;
    }
#endif
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Loads a given module.
 * @param m the module to load
 * @param u the user who loaded it, NULL for auto-load
 * @return MOD_ERR_OK on success, anything else on fail
 */
int loadModule(Module * m, User * u)
{
#ifdef USE_MODULES
    char buf[4096];
    const char *err;
    int (*func) (int, char **);
    int (*version) (void);
    int ret = 0;
    char *argv[1];
    int argc = 0;
    Module *m2;
#ifdef _WIN32
    char buffer[_MAX_PATH];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
#endif
    if (!m || !m->name) {
        return MOD_ERR_PARAMS;
    }
    if (m->handle) {
        return MOD_ERR_EXISTS;
    }
    if ((m2 = findModule(m->name)) != NULL) {
        return MOD_ERR_EXISTS;
    }
#ifndef _WIN32
    ircsnprintf(buf, sizeof(buf), "%sruntime/%s%s.XXXXXX", MODULE_PATH,
                m->name, MODULE_EXT);
#else
    ircsnprintf(buf, sizeof(buf), "%s%sruntime\\%s%s.XXXXXX", buffer,
                MODULE_PATH, m->name, MODULE_EXT);
#endif
    /* Don't skip return value checking! -GD */
    if ((ret = moduleCopyFile(m->name, buf)) != MOD_ERR_OK) {
        if (u)
            notice_lang(s_StatServ, u, STAT_MODULE_LOAD_FAIL, m->name);
        m->filename = sstrdup(buf);
        return ret;
    }

    m->filename = sstrdup(buf);
    deno_modclearerr();
    m->handle = deno_modopen(m->filename);
    if ((err = deno_moderr()) != NULL) {
        alog(LOG_NORMAL, langstr(ALOG_MOD_ERROR), err);
        if (u) {
            notice_lang(s_StatServ, u, STAT_MODULE_LOAD_FAIL, m->name);
        }
        return MOD_ERR_NOLOAD;
    }
    deno_modclearerr();
    func = deno_modsym(m->handle, "DenoraInit");
    if ((err = deno_moderr()) != NULL) {
        deno_modclose(m->handle);       /* If no DenoraInit - it isnt an Denora Module, close it */
        return MOD_ERR_NOLOAD;
    }
    if (func) {
        version =
            (int (*)()) deno_modsym(m->handle, "getDenoraBuildVersion");
        if (version) {
            if (version() >= VERSION_BUILD) {
                alog(LOG_DEBUG,
                     "Module %s compiled against current or newer Denora revision %d, this is %d",
                     m->name, version(), VERSION_BUILD);
            } else {
                alog(LOG_DEBUG,
                     "Module %s is compiled against an old version of Denora (%d) current is %d",
                     m->name, version(), VERSION_BUILD);
                alog(LOG_DEBUG,
                     "Rebuild module %s against the current version to resolve this error",
                     m->name);
                deno_modclose(m->handle);
                deno_modclearerr();
                return MOD_ERR_NOLOAD;
            }
        } else {
            deno_modclose(m->handle);
            deno_modclearerr();
            alog(LOG_DEBUG,
                 "Module %s is compiled against an older version of Denora (unknown)",
                 m->name);
            alog(LOG_DEBUG,
                 "Rebuild module %s against the current version to resolve this error",
                 m->name);
            return MOD_ERR_NOLOAD;
        }
        mod_current_module_name = m->name;
        /* argv[0] is the user if there was one, or NULL if not */
        if (u) {
            argv[0] = sstrdup(u->nick);
        } else {
            argv[0] = NULL;
        }
        argc++;

        ret = func(argc, argv); /* exec DenoraInit */
        if (u) {
            free(argv[0]);
        }
        if (m->type == PROTOCOL && protocolModuleLoaded()) {
            alog(LOG_NORMAL, langstr(ALOG_MOD_BE_ONLY_ONE));
            ret = MOD_STOP;
        }
        if (ret == MOD_STOP) {
            alog(LOG_DEBUG, langstr(ALOG_MOD_UNLOAD_SELF), m->name);
            unloadModule(m, NULL);
            mod_current_module_name = NULL;
            return MOD_ERR_NOLOAD;
        }

        mod_current_module_name = NULL;
    }

    if (u) {
        denora_cmd_global(s_StatServ, langstring(GLOBAL_LOAD_MOD), u->nick,
                          m->name);
        notice_lang(s_StatServ, u, STAT_MODULE_LOADED, m->name);
    }
    addModule(m);
    return MOD_ERR_OK;

#else
    return MOD_ERR_NOLOAD;
#endif
}

/*************************************************************************/

/**
 * Unload the given module.
 * @param m the module to unload
 * @param u the user who unloaded it
 * @return MOD_ERR_OK on success, anything else on fail
 */
int unloadModule(Module * m, User * u)
{
#ifdef USE_MODULES
    void (*func) (void);

    if (!m || !m->handle) {
        if (u) {
            notice_lang(s_StatServ, u, STAT_MODULE_REMOVE_FAIL, m->name);
        }
        return MOD_ERR_PARAMS;
    }

    if (m->type == PROTOCOL) {
        if (u) {
            notice_lang(s_StatServ, u, STAT_MODULE_NO_UNLOAD);
        }
        return MOD_ERR_NOUNLOAD;
    }

    if (prepForUnload(mod_current_module) != MOD_ERR_OK) {
        return MOD_ERR_UNKNOWN;
    }

    func = deno_modsym(m->handle, "DenoraFini");
    if (func) {
        func();                 /* exec DenoraFini */
    }

    if ((deno_modclose(m->handle)) != 0) {
        alog(LOG_ERROR, langstr(ALOG_MOD_ERROR), deno_moderr());
        if (u) {
            notice_lang(s_StatServ, u, STAT_MODULE_REMOVE_FAIL, m->name);
        }
        return MOD_ERR_NOUNLOAD;
    } else {
        if (u) {
            denora_cmd_global(s_StatServ, langstring(GLOBAL_UNLOAD_MOD),
                              u->nick, m->name);
            notice_lang(s_StatServ, u, STAT_MODULE_UNLOADED, m->name);
        }
        delModule(m);
        return MOD_ERR_OK;
    }
#else
    return MOD_ERR_NOUNLOAD;
#endif
}

/*************************************************************************/

/**
 * Prepare a module to be unloaded.
 * Remove all commands and messages this module is providing, and delete 
 * any callbacks which are still pending.
 * @param m the module to prepare for unload
 * @return MOD_ERR_OK on success
 */
int prepForUnload(Module * m)
{
    int idx;
    CommandHash *ccurrent = NULL;
    MessageHash *mcurrent = NULL;
    EvtMessageHash *ecurrent = NULL;
    EvtHookHash *ehcurrent = NULL;
    Command *c;
    EvtMessage *eMsg;
    EvtHook *eHook;
    Message *msg;
    int status = 0;

    SET_SEGV_LOCATION();

    if (!m) {
        return MOD_ERR_PARAMS;
    }

    /* Kill any active callbacks this module has */
    moduleCallBackPrepForUnload(m->name);

    /* Remove any stored data this module has */
    moduleDelAllDataMod(m);

    /**
     * ok, im going to walk every hash looking for commands we own, now, not exactly elegant or efficiant :)
     **/

    for (idx = 0; idx < MAX_CMD_HASH; idx++) {
        for (ccurrent = OS_cmdTable[idx]; ccurrent;
             ccurrent = ccurrent->next) {
            for (c = ccurrent->c; c; c = c->next) {
                if ((c->mod_name) && (stricmp(c->mod_name, m->name) == 0)) {
                    moduleDelCommand(STATSERV, c->name);
                }
            }
        }

        for (mcurrent = IRCD[idx]; mcurrent; mcurrent = mcurrent->next) {
            for (msg = mcurrent->m; msg; msg = msg->next) {
                if ((msg->mod_name)
                    && (stricmp(msg->mod_name, m->name) == 0)) {
                    moduleDelMessage(msg->name);
                }
            }
        }


        for (ecurrent = EVENT[idx]; ecurrent; ecurrent = ecurrent->next) {
            for (eMsg = ecurrent->evm; eMsg; eMsg = eMsg->next) {
                if ((eMsg->mod_name)
                    && (stricmp(eMsg->mod_name, m->name) == 0)) {
                    status = delEventHandler(EVENT, eMsg, m->name);
                }
            }
        }
        for (ehcurrent = EVENTHOOKS[idx]; ehcurrent;
             ehcurrent = ehcurrent->next) {
            for (eHook = ehcurrent->evh; eHook; eHook = eHook->next) {
                if ((eHook->mod_name)
                    && (stricmp(eHook->mod_name, m->name) == 0)) {
                    status = delEventHook(EVENTHOOKS, eHook, m->name);
                }
            }
        }
    }
    return MOD_ERR_OK;
}


/*************************************************************************/

/**
 * Enqueue a module operation (load/unload/reload)
 * @param m Module to perform the operation on
 * @param op Operation to perform on the module
 * @param u User who requested the operation
 **/
void queueModuleOperation(Module * m, ModuleOperation op, User * u)
{
    ModuleQueue *qm;

    qm = calloc(1, sizeof(ModuleQueue));
    qm->m = m;
    qm->op = op;
    qm->u = u;
    qm->next = mod_operation_queue;
    mod_operation_queue = qm;
}

/*************************************************************************/

/**
 * Enqueue a module to load
 * @param name Name of the module to load
 * @param u User who requested the load
 * @return 1 on success, 0 on error
 **/
int queueModuleLoad(char *name, User * u)
{
    Module *m;

    if (!name || !u)
        return 0;

    if (findModule(name))
        return 0;
    m = createModule(name);
    queueModuleOperation(m, MOD_OP_LOAD, u);

    return 1;
}

/*************************************************************************/

/**
 * Enqueue a module to unload
 * @param name Name of the module to unload
 * @param u User who requested the unload
 * @return 1 on success, 0 on error
 **/
int queueModuleUnload(char *name, User * u)
{
    Module *m;

    if (!name || !u) {
        return 0;
    }

    m = findModule(name);
    if (!m) {
        return 0;
    }
    queueModuleOperation(m, MOD_OP_UNLOAD, u);

    return 1;
}

/*************************************************************************/

/**
 * Execute all queued module operations
 **/
void handleModuleOperationQueue(void)
{
    ModuleQueue *next;
    int status;

    if (!mod_operation_queue)
        return;

    while (mod_operation_queue) {
        next = mod_operation_queue->next;

        mod_current_module = mod_operation_queue->m;
        mod_current_user = mod_operation_queue->u;

        if (mod_operation_queue->op == MOD_OP_LOAD) {
            alog(LOG_NORMAL, "Trying to load module [%s]",
                 mod_operation_queue->m->name);
            status =
                loadModule(mod_operation_queue->m, mod_operation_queue->u);
            alog(LOG_NORMAL, "Module loading status: [%d][%s]", status,
                 ModuleGetErrStr(status));
            if (status != MOD_ERR_OK) {
                destroyModule(mod_operation_queue->m);
            }
        } else if (mod_operation_queue->op == MOD_OP_UNLOAD) {
            alog(LOG_NORMAL, "Trying to unload module [%s]",
                 mod_operation_queue->m->name);
            status =
                unloadModule(mod_operation_queue->m,
                             mod_operation_queue->u);
            alog(LOG_NORMAL, "Module unloading status: [%d][%s]", status,
                 ModuleGetErrStr(status));
        }

        /* Remove the ModuleQueue from memory */
        free(mod_operation_queue);

        mod_operation_queue = next;
    }

    mod_current_module = NULL;
    mod_current_user = NULL;
}

/*************************************************************************/

void modules_unload_all(void)
{
#ifdef USE_MODULES
    int idx;
    ModuleHash *mh, *next;
    void (*func) (void);

    for (idx = 0; idx < MAX_CMD_HASH; idx++) {
        mh = MODULE_HASH[idx];
        while (mh) {
            next = mh->next;

            if (prepForUnload(mh->m) != MOD_ERR_OK) {
                mh = next;
                continue;
            }

            func = deno_modsym(mh->m->handle, "DenoraFini");
            if (func) {
                mod_current_module_name = mh->m->name;
                func();         /* exec DenoraFini */
                mod_current_module_name = NULL;
            }

            if ((deno_modclose(mh->m->handle)) != 0)
                alog(LOG_DEBUG, "moderr: %s", deno_moderr());
            else
                delModule(mh->m);

            mh = next;
        }
    }
#endif
}

/*************************************************************************/

/** 
 * Load the ircd protocol module up 
 **/
int protocol_module_init(void)
{
    int ret = 0;
    Module *m;

    if (BadPtr(IRCDModule)) {
        alog(LOG_NORMAL, "Error: IRCDModule is incorrectly defined");
        return ret;
    }

    m = createModule(IRCDModule);
    mod_current_module = m;
    mod_current_user = NULL;
    moduleSetType(PROTOCOL);
    ret = loadModule(mod_current_module, NULL);
    if (ret != MOD_ERR_OK) {
        alog(LOG_NORMAL, "Error Status [%d]", ret);
        exit(1);
    }
    alog(LOG_NORMAL, "Loading IRCD Protocol Module: [%s]",
         mod_current_module->name);
    alog(LOG_NORMAL, "IRCD Protocol Author: [%s]",
         mod_current_module->author);
    alog(LOG_NORMAL, "IRCD Protocol Version: [%s]",
         mod_current_module->version);
    alog(LOG_EXTRADEBUG, "debug: status: [%d]", ret);
    mod_current_module = NULL;
    return ret;
}

/*************************************************************************/

/**
 * Count how many modules are loaded up
 **/
int moduleCount(int all)
{
    int idx = 0;
    int counter = 0;
    ModuleHash *modcurrent = NULL;

    for (idx = 0; idx != MAX_CMD_HASH; idx++) {
        for (modcurrent = MODULE_HASH[idx]; modcurrent;
             modcurrent = modcurrent->next) {
            if (all) {
                counter++;
            } else {
                switch (modcurrent->m->type) {
                case THIRD:
                    counter++;
                    break;
                case PROTOCOL:
                    counter++;
                    break;
                case SUPPORTED:
                    counter++;
                    break;
                case QATESTED:
                    counter++;
                    break;
                default:
                    break;
                }

            }
        }
    }

    return counter;
}

/*************************************************************************/

/** 
    * Search all loaded modules looking for a protocol module. 
    * @return 1 if one is found. 
    **/
int protocolModuleLoaded()
{
    int idx = 0;
    ModuleHash *modcurrent = NULL;

    for (idx = 0; idx != MAX_CMD_HASH; idx++) {
        for (modcurrent = MODULE_HASH[idx]; modcurrent;
             modcurrent = modcurrent->next) {
            if (modcurrent->m->type == PROTOCOL) {
                return 1;
            }
        }
    }
    return 0;
}

/*******************************************************************************
 * Module Callback Functions
 *******************************************************************************/

 /**
  * Adds a timed callback for the current module.
  * This allows modules to request that denora executes one of there functions at a time in the future, without an event to trigger it
  * @param name the name of the callback, this is used for refrence mostly, but is needed it you want to delete this particular callback later on
  * @param when when should the function be executed, this is a time in the future, seconds since 00:00:00 1970-01-01 UTC
  * @param func the function to be executed when the callback is ran, its format MUST be int func(int argc, char **argv); 
  * @param argc the argument count for the argv paramter
  * @param argv a argument list to be passed to the called function.
  * @return MOD_ERR_OK on success, anything else on fail.
  * @see moduleDelCallBack
  **/
int moduleAddCallback(char *name, time_t when,
                      int (*func) (int argc, char *argv[]), int argc,
                      char **argv)
{
    ModuleCallBack *new, *tmp, *prev;
    int i;

    SET_SEGV_LOCATION();

    new = malloc(sizeof(ModuleCallBack));
    if (!new)
        return MOD_ERR_MEMORY;

    if (name)
        new->name = sstrdup(name);
    else
        new->name = NULL;
    new->when = when;
    if (mod_current_module_name) {
        new->owner_name = sstrdup(mod_current_module_name);
    } else {
        new->owner_name = NULL;
    }
    new->func = func;
    new->argc = argc;
    new->argv = malloc(sizeof(char *) * argc);
    for (i = 0; i < argc; i++) {
        new->argv[i] = sstrdup(argv[i]);
    }
    new->next = NULL;

    if (moduleCallBackHead == NULL) {
        moduleCallBackHead = new;
    } else {                    /* find place in list */
        tmp = moduleCallBackHead;
        prev = tmp;
        if (new->when < tmp->when) {
            new->next = tmp;
            moduleCallBackHead = new;
        } else {
            while (tmp && new->when >= tmp->when) {
                prev = tmp;
                tmp = tmp->next;
            }
            prev->next = new;
            new->next = tmp;
        }
    }
    alog(LOG_DEBUG,
         langstr(ALOG_MOD_CALLBACK_DISPLAY),
         new->name ? new->name : "?", (long int) new->when);
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Execute a stored call back
 **/
void moduleCallBackRun(void)
{
    ModuleCallBack *tmp;

    SET_SEGV_LOCATION();

    if (!moduleCallBackHead) {
        return;
    }
    tmp = moduleCallBackHead;
    if (tmp->when <= time(NULL)) {
        alog(LOG_DEBUG, "debug: Executing callback: %s",
             tmp->name ? tmp->name : "?");
        if (tmp->func) {
            mod_current_module_name = tmp->owner_name;
            tmp->func(tmp->argc, tmp->argv);
            mod_current_module = NULL;
            moduleCallBackDeleteEntry(NULL);    /* delete the head */
        }
    }
    return;
}

/*************************************************************************/

/**
 * Removes a entry from the modules callback list
 * @param prev a pointer to the previous entry in the list, NULL for the head
 **/
void moduleCallBackDeleteEntry(ModuleCallBack * prev)
{
    ModuleCallBack *tmp = NULL;
    int i;

    SET_SEGV_LOCATION();

    if (prev == NULL) {
        tmp = moduleCallBackHead;
        moduleCallBackHead = tmp->next;
    } else {
        tmp = prev->next;
        prev->next = tmp->next;
    }
    if (tmp->name)
        free(tmp->name);
    if (tmp->owner_name)
        free(tmp->owner_name);
    tmp->func = NULL;
    for (i = 0; i < tmp->argc; i++) {
        free(tmp->argv[i]);
    }
    tmp->argc = 0;
    tmp->next = NULL;
    free(tmp);
}

/*************************************************************************/

/**
 * Search the module callback list for a given module
 * @param mod_name the name of the module were looking for
 * @param found have we found it?
 * @return a pointer to the ModuleCallBack struct or NULL - dont forget to check the found paramter!
 **/
ModuleCallBack *moduleCallBackFindEntry(char *mod_name, boolean * found)
{
    ModuleCallBack *prev = NULL, *modcurrent = NULL;
    *found = false;
    modcurrent = moduleCallBackHead;

    SET_SEGV_LOCATION();

    while (modcurrent != NULL) {
        if (modcurrent->owner_name
            && (strcmp(mod_name, modcurrent->owner_name) == 0)) {
            *found = true;
            break;
        } else {
            prev = modcurrent;
            modcurrent = modcurrent->next;
        }
    }
    if (modcurrent == moduleCallBackHead) {
        return NULL;
    } else {
        return prev;
    }
}

/*************************************************************************/

/**
 * Allow module coders to delete a callback by name.
 * @param name the name of the callback they wish to delete
 **/
void moduleDelCallback(char *name)
{
    ModuleCallBack *modcurrent = NULL;
    ModuleCallBack *prev = NULL, *tmp = NULL;
    int del = 0;

    SET_SEGV_LOCATION();

    if (!mod_current_module_name) {
        return;
    }
    if (!name) {
        return;
    }
    modcurrent = moduleCallBackHead;
    while (modcurrent) {
        if ((modcurrent->owner_name) && (modcurrent->name)) {
            if ((strcmp(mod_current_module_name, modcurrent->owner_name) ==
                 0)
                && (strcmp(modcurrent->name, name) == 0)) {
                alog(LOG_DEBUG,
                     "debug: Removing CallBack %s for module %s", name,
                     mod_current_module_name);
                tmp = modcurrent->next; /* get a pointer to the next record, as once we delete this record, we'll lose it :) */
                moduleCallBackDeleteEntry(prev);        /* delete this record */
                del = 1;        /* set the record deleted flag */
            }
        }
        if (del == 1) {         /* if a record was deleted */
            modcurrent = tmp;   /* use the value we stored in temp */
            tmp = NULL;         /* clear it for next time */
            del = 0;            /* reset the flag */
        } else {
            prev = modcurrent;  /* just carry on as normal */
            modcurrent = modcurrent->next;
        }
    }
}

/*************************************************************************/

/**
 * Remove all outstanding module callbacks for the given module.
 * When a module is unloaded, any callbacks it had outstanding must be removed, else when they attempt to execute the func pointer will no longer be valid, and we'll seg.
 * @param mod_name the name of the module we are preping for unload 
 **/
void moduleCallBackPrepForUnload(char *mod_name)
{
    boolean found = false;
    ModuleCallBack *tmp = NULL;

    tmp = moduleCallBackFindEntry(mod_name, &found);
    SET_SEGV_LOCATION();

    while (found) {
        alog(LOG_DEBUG, "debug: Removing CallBack for module %s",
             mod_name);
        moduleCallBackDeleteEntry(tmp);
        tmp = moduleCallBackFindEntry(mod_name, &found);
    }
}


/*************************************************************************/

Command *first_command(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentcmd == NULL)
        currentcmd = STATSERV[next_index++];
    return currentcmd->c;
}

/*************************************************************************/

Command *next_command(void)
{
    SET_SEGV_LOCATION();

    if (currentcmd)
        currentcmd = currentcmd->next;
    if (!currentcmd && next_index < 1024) {
        while (next_index < 1024 && currentcmd == NULL)
            currentcmd = STATSERV[next_index++];
    }
    if (currentcmd) {
        return currentcmd->c;
    } else {
        return NULL;
    }
}

/*************************************************************************/

CommandHash *first_commandhash(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentcmd == NULL)
        currentcmd = STATSERV[next_index++];
    return currentcmd;
}

/*************************************************************************/

CommandHash *next_commandhash(void)
{
    SET_SEGV_LOCATION();

    if (currentcmd)
        currentcmd = currentcmd->next;
    if (!currentcmd && next_index < 1024) {
        while (next_index < 1024 && currentcmd == NULL)
            currentcmd = STATSERV[next_index++];
    }
    if (currentcmd) {
        return currentcmd;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Message *first_message(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL)
        current = IRCD[next_index++];
    return current->m;
}

/*************************************************************************/

Message *next_message(void)
{
    SET_SEGV_LOCATION();

    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL)
            current = IRCD[next_index++];
    }
    if (current) {
        return current->m;
    } else {
        return NULL;
    }
}

/*************************************************************************/

MessageHash *first_messagehash(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && current == NULL)
        current = IRCD[next_index++];
    return current;
}

/*************************************************************************/

MessageHash *next_messagehash(void)
{
    SET_SEGV_LOCATION();

    if (current)
        current = current->next;
    if (!current && next_index < 1024) {
        while (next_index < 1024 && current == NULL)
            current = IRCD[next_index++];
    }
    if (current) {
        return current;
    } else {
        return NULL;
    }
}

/*************************************************************************/

/**
 * Add a message to the MessageHash.
 * @param msgTable the MessageHash we want to add a message to
 * @param m the Message we want to add
 * @param pos the position we want to add the message to, E.G. MOD_HEAD, MOD_TAIL, MOD_UNIQUE 
 * @return MOD_ERR_OK on a successful add.
 **/

int addMessage(MessageHash * msgTable[], Message * m, int pos)
{
    /* We can assume both param's have been checked by this point.. */
    int modindex = 0;
    MessageHash *msgcurrent = NULL;
    MessageHash *newHash = NULL;
    MessageHash *lastHash = NULL;
    Message *tail = NULL;
    int match = 0;

    SET_SEGV_LOCATION();

    if (!msgTable || !m || (pos < 0 || pos > 2)) {
        return MOD_ERR_PARAMS;
    }

    modindex = CMD_HASH(m->name);

    for (msgcurrent = msgTable[modindex]; msgcurrent;
         msgcurrent = msgcurrent->next) {
        if ((UseTokens) && (!ircd->tokencaseless)) {
            match = strcmp(m->name, msgcurrent->name);
        } else {
            match = stricmp(m->name, msgcurrent->name);
        }
        if (match == 0) {       /* the msg exist's we are a addHead */
            if (pos == 1) {
                m->next = msgcurrent->m;
                msgcurrent->m = m;
                alog(LOG_DEBUG,
                     "debug: existing msg: (0x%p) (%s) , new msg (0x%p) (%s)",
                     (void *) m->next, m->next->name, (void *) m, m->name);
                return MOD_ERR_OK;
            } else if (pos == 2) {
                tail = msgcurrent->m;
                while (tail->next)
                    tail = tail->next;
                alog(LOG_DEBUG,
                     "debug: existing msg: (0x%p), new msg (0x%p) (%s)",
                     (void *) tail, (void *) m, m->name);
                tail->next = m;
                m->next = NULL;
                return MOD_ERR_OK;
            } else
                return MOD_ERR_EXISTS;
        }
        lastHash = msgcurrent;
    }

    if ((newHash = malloc(sizeof(MessageHash))) == NULL) {
        fatal("Out of memory");
    }
    newHash->next = NULL;
    newHash->name = sstrdup(m->name);
    newHash->m = m;

    if (lastHash == NULL)
        msgTable[modindex] = newHash;
    else
        lastHash->next = newHash;
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Add the given message (m) to the MessageHash marking it as a core command
 * @param msgTable the MessageHash we want to add to
 * @param m the Message we are adding
 * @return MOD_ERR_OK on a successful add.
 **/
int addCoreMessage(MessageHash * msgTable[], Message * m)
{
    SET_SEGV_LOCATION();

    if (!msgTable || !m) {
        return MOD_ERR_PARAMS;
    }
    m->core = 1;
    return addMessage(msgTable, m, 0);
}

/*************************************************************************/

/**
 * Add a module message to the IRCD message hash
 * @param m the Message to add
 * @param pos the Position to add the message to, e.g. MOD_HEAD, MOD_TAIL, MOD_UNIQUE
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleAddMessage(Message * m, int pos)
{
    int status;

    SET_SEGV_LOCATION();

    if (!m) {
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
    m->core = 0;
    if (!m->mod_name) {
        m->mod_name = sstrdup(mod_current_module->name);
    }

    status = addMessage(IRCD, m, pos);
    if (denora->debug) {
        displayMessageFromHash(m->name);
    }
    return status;
}

/*************************************************************************/

ModuleHash *first_modulehash(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentmod == NULL)
        currentmod = MODULE_HASH[next_index++];
    return currentmod;
}

/*************************************************************************/

ModuleHash *next_modulehash(void)
{
    SET_SEGV_LOCATION();

    if (currentmod)
        currentmod = currentmod->next;
    if (!currentmod && next_index < 1024) {
        while (next_index < 1024 && currentmod == NULL)
            currentmod = MODULE_HASH[next_index++];
    }
    if (currentmod) {
        return currentmod;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Module *first_module(void)
{
    next_index = 0;

    SET_SEGV_LOCATION();

    while (next_index < 1024 && currentmod == NULL)
        currentmod = MODULE_HASH[next_index++];
    if (currentmod) {
        return currentmod->m;
    } else {
        return NULL;
    }
}

/*************************************************************************/

Module *next_module(void)
{
    SET_SEGV_LOCATION();

    if (currentmod)
        currentmod = currentmod->next;
    if (!currentmod && next_index < 1024) {
        while (next_index < 1024 && currentmod == NULL)
            currentmod = MODULE_HASH[next_index++];
    }
    if (currentmod) {
        return currentmod->m;
    } else {
        return NULL;
    }
}

/*************************************************************************/

/**
 * remove the given message from the IRCD message hash
 * @param name the name of the message to remove
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int moduleDelMessage(char *name)
{
    Message *m;
    int status;

    SET_SEGV_LOCATION();

    if (!mod_current_module) {
        return MOD_ERR_UNKNOWN;
    }
    m = findMessage(IRCD, name);
    if (!m) {
        return MOD_ERR_NOEXIST;
    }

    status = delMessage(IRCD, m, mod_current_module->name);
    if (denora->debug) {
        displayMessageFromHash(m->name);
    }
    return status;
}

/*************************************************************************/

/**
 * remove the given message from the given message hash, for the given module
 * @param msgTable which MessageHash we are removing from
 * @param m the Message we want to remove
 * @param mod_name the name of the module we are removing
 * @return MOD_ERR_OK on success, althing else on fail.
 **/
int delMessage(MessageHash * msgTable[], Message * m, char *mod_name)
{
    int msgindex = 0;
    MessageHash *msgcurrent = NULL;
    MessageHash *lastHash = NULL;
    Message *tail = NULL, *last = NULL;

    SET_SEGV_LOCATION();

    if (!m || !msgTable) {
        return MOD_ERR_PARAMS;
    }

    msgindex = CMD_HASH(m->name);

    for (msgcurrent = msgTable[msgindex]; msgcurrent;
         msgcurrent = msgcurrent->next) {
        if (stricmp(m->name, msgcurrent->name) == 0) {
            if (!lastHash) {
                tail = msgcurrent->m;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                msgcurrent->m = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    msgTable[msgindex] = msgcurrent->next;
                    free(msgcurrent->name);
                    return MOD_ERR_OK;
                }
            } else {
                tail = msgcurrent->m;
                if (tail->next) {
                    while (tail) {
                        if (mod_name && tail->mod_name
                            && (stricmp(mod_name, tail->mod_name) == 0)) {
                            if (last) {
                                last->next = tail->next;
                            } else {
                                msgcurrent->m = tail->next;
                            }
                            return MOD_ERR_OK;
                        }
                        last = tail;
                        tail = tail->next;
                    }
                } else {
                    lastHash->next = msgcurrent->next;
                    free(msgcurrent->name);
                    return MOD_ERR_OK;
                }
            }
        }
        lastHash = msgcurrent;
    }
    return MOD_ERR_NOEXIST;
}

/*************************************************************************/

/**
 * Destory a message, freeing its memory.
 * @param m the message to be destroyed
 * @return MOD_ERR_SUCCESS on success
 **/
int destroyMessage(Message * m)
{
    SET_SEGV_LOCATION();

    if (!m) {
        return MOD_ERR_PARAMS;
    }
    if (m->name) {
        free(m->name);
    }
    m->func = NULL;
    if (m->mod_name) {
        free(m->mod_name);
    }
    m->next = NULL;
    free(m);
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyMessageHash(MessageHash * mh)
{
    SET_SEGV_LOCATION();

    if (!mh) {
        return MOD_ERR_PARAMS;
    }
    if (mh->name) {
        free(mh->name);
    }
    mh->m = NULL;
    mh->next = NULL;
    free(mh);
    return MOD_ERR_OK;
}

/*************************************************************************/

int destroyCommandHash(CommandHash * ch)
{
    SET_SEGV_LOCATION();

    if (!ch) {
        return MOD_ERR_PARAMS;
    }
    if (ch->name) {
        free(ch->name);
    }
    ch->c = NULL;
    ch->next = NULL;
    free(ch);
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Return the Command corresponding to the given name, or NULL if no such
 * command exists.
 * @param list Command struct
 * @param cmd Command to look up
 * @return Command Struct for the given cmd
 */
Command *lookup_cmd(Command * list, char *cmd)
{
    Command *c;

    SET_SEGV_LOCATION();

    for (c = list; c->name; c++) {
        if (stricmp(c->name, cmd) == 0) {
            return c;
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Run the routine for the given command, if it exists and the user has
 * privilege to do so; if not, print an appropriate error message.
 * @param service Services Client
 * @param u User Struct
 * @param list Command struct
 * @param cmd Command
 * @param str is the string buffer
 * @return void
 */
void run_cmd(char *service, User * u, Command * list, char *cmd, char *str)
{
    Command *c;
    SET_SEGV_LOCATION();
    c = lookup_cmd(list, cmd);
    do_run_cmd(service, u, c, cmd, str);
}

/*************************************************************************/

/**
 * Run the routine for the given command, if it exists and the user has
 * privilege to do so; if not, print an appropriate error message.
 * @param service Services Client
 * @param u User Struct
 * @param cmdTable Command Hash Table
 * @param cmd Command
 * @param str is the string buffer
 * @return void
 */
void mod_run_cmd(char *service, User * u, CommandHash * cmdTable[],
                 const char *cmd, char *str)
{
    Command *c;
    SET_SEGV_LOCATION();
    c = findCommand(cmdTable, cmd);
    do_run_cmd(service, u, c, cmd, str);
}


/*************************************************************************/

/**
 * Run the given command
 * @param service Services Client
 * @param u User Struct
 * @param c Command Struct
 * @param cmd Command
 * @param str is the buffer containing the command
 * @return void
 */
void do_run_cmd(char *service, User * u, Command * c, const char *cmd,
                char *str)
{
    int retVal = 0;
    Command *cmdcurrent;
    int ac;
    char **av;
    SET_SEGV_LOCATION();

    if (str) {
        ac = split_buf(str, &av);
    } else {
        ac = 0;
        av = NULL;
    }

    if (c && c->routine) {
        if ((c->has_priv == NULL) || c->has_priv(u)) {
            mod_current_module_name = c->mod_name;
            retVal = c->routine(u, ac, av);
            mod_current_module_name = NULL;
            if (retVal == MOD_CONT) {
                cmdcurrent = c->next;
                while (cmdcurrent && retVal == MOD_CONT) {
                    mod_current_module_name = cmdcurrent->mod_name;
                    retVal = cmdcurrent->routine(u, ac, av);
                    mod_current_module_name = NULL;
                    cmdcurrent = cmdcurrent->next;
                }
            }
        }

        else {
            notice_lang(service, u, ACCESS_DENIED);
            alog(LOG_NORMAL,
                 "Access denied for %s with service %s and command %s",
                 u->nick, service, cmd);
        }
    } else {
        notice_lang(service, u, UNKNOWN_COMMAND_HELP, cmd, service);
    }
    if (ac) {
        free(av);
    }
}

/*************************************************************************/

/**
 * Print a help message for the given command.
 * @param service Services Client
 * @param u User Struct
 * @param c Command Struct
 * @param cmd Command
 * @return void
 */
void do_help_cmd(char *service, User * u, Command * c, const char *cmd)
{
    Command *cmdcurrent;
    int has_had_help = 0;
    int cont = MOD_CONT;
    const char *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;

    SET_SEGV_LOCATION();

    for (cmdcurrent = c; (cmdcurrent) && (cont == MOD_CONT);
         cmdcurrent = cmdcurrent->next) {
        p1 = cmdcurrent->help_param1;
        p2 = cmdcurrent->help_param2;
        p3 = cmdcurrent->help_param3;
        p4 = cmdcurrent->help_param4;
        if (cmdcurrent->helpmsg_all >= 0) {
            notice_help(service, u, cmdcurrent->helpmsg_all, p1, p2, p3,
                        p4);
            has_had_help = 1;
        } else if (cmdcurrent->all_help) {
            cont = cmdcurrent->all_help(u);
            has_had_help = 1;
        }
        if (is_stats_admin(u)) {
            if (cmdcurrent->helpmsg_admin >= 0) {
                notice_help(service, u, cmdcurrent->helpmsg_admin, p1, p2,
                            p3, p4);
                has_had_help = 1;
            } else if (cmdcurrent->admin_help) {
                cont = cmdcurrent->admin_help(u);
                has_had_help = 1;
            }
        } else {
            if (cmdcurrent->helpmsg_reg >= 0) {
                notice_help(service, u, cmdcurrent->helpmsg_reg, p1, p2,
                            p3, p4);
                has_had_help = 1;
            } else if (cmdcurrent->regular_help) {
                cont = cmdcurrent->regular_help(u);
                has_had_help = 1;
            }
        }
    }
    if (has_had_help == 0) {
        notice_lang(service, u, NO_HELP_AVAILABLE, cmd);
    }
}

/*************************************************************************/

/**
 * Find the Help Command
 * @param service Services Client
 * @param u User Struct
 * @param list Command Struct
 * @param cmd Command
 * @return void
 */
void help_cmd(char *service, User * u, Command * list, char *cmd)
{
    Command *c;
    SET_SEGV_LOCATION();
    c = lookup_cmd(list, cmd);
    do_help_cmd(service, u, c, cmd);
}

/*************************************************************************/

/**
 * Find the Help Command
 * @param service Services Client
 * @param u User Struct
 * @param cmdTable Command Hash Table
 * @param cmd Command
 * @return void
 */
void mod_help_cmd(char *service, User * u, CommandHash * cmdTable[],
                  const char *cmd)
{
    Command *c;
    SET_SEGV_LOCATION();
    c = findCommand(cmdTable, cmd);
    do_help_cmd(service, u, c, cmd);
}
