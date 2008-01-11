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

/*************************************************************************/

/**
 * Return the error string based on the error code
 *
 * @param status is the error code
 * @return string containing error string
 *
 */
char *ModuleGetErrStr(int status)
{
    const char *module_err_str[] = {
        "Module Error, Okay",
        "Module Error, allocating memory",
        "Module Error, not enough parameters",
        "Module Error, already loaded",
        "Module Error, file does not exist",
        "Module Error, No User",
        "Module Error, Error during load time or module returned MOD_STOP",
        "Module Error, Unable to unload"
            "Module Error, Unknown Error occuried",
        "Module Error, File I/O Error",
        "Module Error, No Service found for request",
        "Module Error, No module name for request",
    };
    return (char *) module_err_str[status];
}

/*************************************************************************/

/** 
 * Get the text of the given lanugage string in the corrent language, or 
 * in english. 
 * @param u The user to send the message to 
 * @param number The message number 
 **/
char *moduleGetLangString(User * u, int number)
{
    int lang = StatsLanguage;

    if ((mod_current_module_name)
        && (!mod_current_module
            || strcmp(mod_current_module_name, mod_current_module->name)))
        mod_current_module = findModule(mod_current_module_name);

    /* Find the users lang, and use it if we can */
    if (u && u->admin)
        lang = u->language;

    /* If the users lang isnt supported, drop back to English */
    if (mod_current_module->lang[lang].argc == 0)
        lang = LANG_EN_US;

    /* If the requested lang string exists for the language */
    if (mod_current_module->lang[lang].argc > number)
        return mod_current_module->lang[lang].argv[number];
    /* Return an empty string otherwise, because we might be used without 
     * the return value being checked. If we would return NULL, bad things 
     * would happen! 
     */
    else
        return (char *) "";
}


/*************************************************************************/

/**
 * Send a notice to the user in the correct language, or english.
 * @param source Who sends the notice
 * @param cs The Channel to send the message to
 * @param number The message number
 * @param ... The argument list
 **/
void moduleNoticeChanLang(char *source, ChannelStats * cs, int number, ...)
{
    va_list va;
    char buffer[4096], outbuf[4096];
    char *fmt = NULL;
    int lang = StatsLanguage;
    char *s, *t, *buf;

    if (!cs) {
        return;
    }

    if ((mod_current_module_name)
        && (!mod_current_module
            || strcmp(mod_current_module_name,
                      mod_current_module->name))) {
        mod_current_module = findModule(mod_current_module_name);
    }

    /* If the users lang isnt supported, drop back to Enlgish */
    if (mod_current_module->lang[lang].argc == 0) {
        lang = LANG_EN_US;
    }

    /* If the requested lang string exists for the language */
    if (mod_current_module->lang[lang].argc > number) {
        fmt = mod_current_module->lang[lang].argv[number];

        buf = sstrdup(fmt);
        va_start(va, number);
        ircvsnprintf(buffer, 4095, fmt, va);
        va_end(va);
        s = buffer;
        while (*s) {
            t = s;
            s += strcspn(s, "\n");
            if (*s)
                *s++ = '\0';
            strlcpy(outbuf, t, sizeof(outbuf));
            if (cs->flags & CS_NOTICE) {
                notice(source, cs->name, outbuf);
            } else {
                denora_cmd_privmsg(source, cs->name, outbuf);
            }
        }
        free(buf);
    } else {
        alog(LOG_ERROR,
             "%s: INVALID language string call, language: [%d], String [%d]",
             mod_current_module->name, lang, number);
    }
}


/*************************************************************************/

/**
 * Delete a language from a module
 * @param langNumber the language Number to delete
 **/
void moduleDeleteLanguage(int langNumber)
{
    int idx = 0;
    if ((mod_current_module_name)
        && (!mod_current_module
            || strcmp(mod_current_module_name,
                      mod_current_module->name))) {
        mod_current_module = findModule(mod_current_module_name);
    }
    for (idx = 0; idx > mod_current_module->lang[langNumber].argc; idx++) {
        free(mod_current_module->lang[langNumber].argv[idx]);
    }
    mod_current_module->lang[langNumber].argc = 0;
}

/*************************************************************************/

/**
 * Allow a module to add a set of language strings to Denora
 * @param langNumber the language number for the strings
 * @param ac The language count for the strings
 * @param av The language sring list.
 **/
void moduleInsertLanguage(int langNumber, int ac, char **av)
{
    int i;

    if ((mod_current_module_name)
        && (!mod_current_module
            || strcmp(mod_current_module_name,
                      mod_current_module->name))) {
        mod_current_module = findModule(mod_current_module_name);
    }

    alog(LOG_DEBUG, "debug: %s Adding %d texts for language %d",
         mod_current_module->name, ac, langNumber);

    if (mod_current_module->lang[langNumber].argc > 0) {
        moduleDeleteLanguage(langNumber);
    }

    mod_current_module->lang[langNumber].argc = ac;
    mod_current_module->lang[langNumber].argv =
        malloc(sizeof(char *) * ac);
    for (i = 0; i < ac; i++) {
        mod_current_module->lang[langNumber].argv[i] = sstrdup(av[i]);
    }
}

/*************************************************************************/

/**
 * Send a notice to the user in the correct language, or english.
 * @param source Who sends the notice
 * @param u The user to send the message to
 * @param number The message number
 * @param ... The argument list
 **/
void moduleNoticeLang(char *source, User * u, int number, ...)
{
    va_list va;
    char buffer[4096], outbuf[4096];
    char *fmt = NULL;
    int lang = StatsLanguage;
    char *s, *t, *buf;

    if ((mod_current_module_name)
        && (!mod_current_module
            || strcmp(mod_current_module_name,
                      mod_current_module->name))) {
        mod_current_module = findModule(mod_current_module_name);
    }
    /* Find the users lang, and use it if we cant */
    if (u && u->language) {
        lang = u->language;
    }

    /* If the users lang isnt supported, drop back to Enlgish */
    if (mod_current_module->lang[lang].argc == 0) {
        lang = LANG_EN_US;
    }

    /* If the requested lang string exists for the language */
    if (mod_current_module->lang[lang].argc > number) {
        fmt = mod_current_module->lang[lang].argv[number];

        buf = sstrdup(fmt);
        va_start(va, number);
        ircvsnprintf(buffer, 4095, fmt, va);
        va_end(va);
        s = buffer;
        while (*s) {
            t = s;
            s += strcspn(s, "\n");
            if (*s)
                *s++ = '\0';
            strlcpy(outbuf, t, sizeof(outbuf));
            notice(source, u->nick, outbuf);
        }
        free(buf);
    } else {
        alog(LOG_ERROR,
             "%s: INVALID language string call, language: [%d], String [%d]",
             mod_current_module->name, lang, number);
    }
}


/*************************************************************************/

/** 
 * Module setType() 
 * Lets the module set a type, CORE,PROTOCOL,3RD etc.. 
 **/
void moduleSetType(MODType type)
{
    if ((mod_current_module_name) && (!mod_current_module)) {
        mod_current_module = findModule(mod_current_module_name);
    }
    mod_current_module->type = type;
}

/*************************************************************************/

/**
 * Check the current version of denora against a given version number
 * Specifiying -1 for minor,patch or build 
 * @param major The major version of denora, the first part of the verison number
 * @param minor The minor version of denora, the second part of the version number
 * @param patch The patch version of denora, the third part of the version number
 * @param build The build revision of denora from SVN
 * @return True if the version newer than the version specified.
 **/
boolean moduleMinVersion(int major, int minor, int patch, int build)
{
    boolean ret = false;

    SET_SEGV_LOCATION();

    if (VERSION_MAJOR > major) {        /* Def. new */
        ret = true;
    } else if (VERSION_MAJOR == major) {        /* Might be newer */
        if (minor == -1) {
            return true;
        }                       /* They dont care about minor */
        if (VERSION_MINOR > minor) {    /* Def. newer */
            ret = true;
        } else if (VERSION_MINOR == minor) {    /* Might be newer */
            if (patch == -1) {
                return true;
            }                   /* They dont care about patch */
            if (VERSION_PATCH > patch) {
                ret = true;
            } else if (VERSION_PATCH == patch) {
                if (build == -1) {
                    return true;
                }               /* They dont care about build */
                if (VERSION_BUILD >= build) {
                    ret = true;
                }
            }
        }
    }
    return ret;
}

/*******************************************************************************
 * Module HELP Functions
 *******************************************************************************/
 /**
  * Add help for Stats Admins.
  * @param c the Command to add help for
  * @param func the function to run when this help is asked for
  **/
int moduleAddAdminHelp(Command * c, int (*func) (User * u))
{
    SET_SEGV_LOCATION();
    if (c) {
        c->admin_help = func;
        return MOD_STOP;
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
  * Add help for registered users
  * @param c the Command to add help for
  * @param func the function to run when this help is asked for
  **/
int moduleAddRegHelp(Command * c, int (*func) (User * u))
{
    SET_SEGV_LOCATION();
    if (c) {
        c->regular_help = func;
        return MOD_STOP;
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
  * Add help for all users
  * @param c the Command to add help for
  * @param func the function to run when this help is asked for
  **/
int moduleAddHelp(Command * c, int (*func) (User * u))
{
    SET_SEGV_LOCATION();
    if (c) {
        c->all_help = func;
        return MOD_STOP;
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
  * Add output to statserv help.
  * when doing a /msg statserv help, your function will be called to allow it to send out a notice() 
  * with the code you wish to dispaly
  * @param func a pointer to the function which will display the code
  **/
void moduleSetOperHelp(void (*func) (User * u))
{
    SET_SEGV_LOCATION();
    if (mod_current_module) {
        mod_current_module->operHelp = func;
    }
}

/*************************************************************************/

/**
 * Display any extra module help for the given service.
 * @param service which services is help being dispalyed for?
 * @param u which user is requesting the help
 **/
void moduleDisplayHelp(int service, User * u)
{
#ifdef USE_MODULES
    int idx;
    int header_shown = 0;
    ModuleHash *modcurrent = NULL;
    SET_SEGV_LOCATION();

    for (idx = 0; idx != MAX_CMD_HASH; idx++) {
        for (modcurrent = MODULE_HASH[idx]; modcurrent;
             modcurrent = modcurrent->next) {
            if ((service == 1) && modcurrent->m->operHelp) {
                if (header_shown == 0) {
                    notice_lang(s_StatServ, u, MODULE_HELP_HEADER);
                    header_shown = 1;
                }
                modcurrent->m->operHelp(u);
            }
        }
    }
#endif
}

/*************************************************************************/

/**
 * Return a copy of the complete last buffer.
 * This is needed for modules who cant trust the strtok() buffer, as we dont know who will have already 
 * messed about with it.
 *
 * @return a pointer to a copy of the last buffer - DONT mess with this, copy if first if you must do things to it.
 **/
char *moduleGetLastBuffer(void)
{
    char *tmp = NULL;
    SET_SEGV_LOCATION();

    if (mod_current_buffer) {
        tmp = strchr(mod_current_buffer, ' ');
        if (tmp) {
            tmp++;
        }
    }
    if (tmp) {
        return sstrdup(tmp);
    } else {
        return sstrdup("");
    }
}

/*************************************************************************/

/**
 * Add the modules version info.
 * @param version the version of the current module
 **/
void moduleAddVersion(const char *version)
{
    SET_SEGV_LOCATION();

    if (mod_current_module && version) {
        mod_current_module->version = sstrdup(version);
    }
}

/*************************************************************************/

/**
 * Add the modules author info
 * @param author the author of the module
 **/
void moduleAddAuthor(const char *author)
{
    SET_SEGV_LOCATION();

    if (mod_current_module && author) {
        mod_current_module->author = sstrdup(author);
    }
}
