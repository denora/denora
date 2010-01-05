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
 * Remove any data from any module used in the given struct.
 * Useful for cleaning up when a User leave's the net, a NickCore is deleted, etc...
 * @param moduleData the moduleData struct to "clean"
 **/
void moduleCleanStruct(ModuleData ** moduleData)
{
    ModuleData *modcurrent = *moduleData;
    ModuleData *next = NULL;

    SET_SEGV_LOCATION();

    while (modcurrent) {
        next = modcurrent->next;
        free(modcurrent->moduleName);
        free(modcurrent->key);
        free(modcurrent->value);
        modcurrent->next = NULL;
        free(modcurrent);
        modcurrent = next;
    }
    *moduleData = NULL;
}

/*************************************************************************/

/**
 * Output module data information into the log file.
 * This is a vwey "debug only" function to dump the whole contents
 * of a moduleData struct into the log files.
 * @param md The module data for the struct to be used
 * @return 0 is always returned;
 **/
int moduleDataDebug(ModuleData ** md)
{
    ModuleData *modcurrent = NULL;
    alog(LOG_DEBUG, "debug: Dumping module data....");

    SET_SEGV_LOCATION();

    for (modcurrent = *md; modcurrent; modcurrent = modcurrent->next) {
        alog(LOG_DEBUG, "debug: Module: [%s]", modcurrent->moduleName);
        alog(LOG_DEBUG, "debug: Key [%s]\tValue [%s]", modcurrent->key,
             modcurrent->value);
    }
    alog(LOG_DEBUG, "debug: End of module data dump");
    return 0;
}

/*************************************************************************/

/**
 * Add module data to a struct.
 * This allows module coders to add data to an existing struct
 * @param md The module data for the struct to be used
 * @param key The Key for the key/value pair
 * @param value The value for the key/value pair, this is what will be stored for you
 * @return MOD_ERR_OK will be returned on success
 **/
int moduleAddData(ModuleData ** md, char *key, char *value)
{
    char *mod_name = sstrdup(mod_current_module_name);
    ModuleData *newData = NULL;
    ModuleData *tmp = *md;

    SET_SEGV_LOCATION();

    if (!key || !value) {
        alog(LOG_DEBUG,
             "debug: A module tried to use ModuleAddData() with one ore more NULL arguments... returning");
        do_backtrace(0);
        free(mod_name);
        return MOD_ERR_PARAMS;
    }

    if (mod_current_module_name == NULL) {
        alog(LOG_DEBUG,
             "debug: moduleAddData() called with mod_current_module_name being NULL");
        if (denora->debug) {
            do_backtrace(0);
        }
    }

    moduleDelData(md, key);     /* Remove any existing module data for this module with the same key */

    newData = malloc(sizeof(ModuleData));
    if (!newData) {
        free(mod_name);
        return MOD_ERR_MEMORY;
    }

    newData->moduleName = sstrdup(mod_name);
    newData->key = sstrdup(key);
    newData->value = sstrdup(value);
    if (tmp) {
        newData->next = tmp;
    } else {
        newData->next = NULL;
    }
    *md = newData;

    free(mod_name);

    if (denora->debug) {
        moduleDataDebug(md);
    }
    return MOD_ERR_OK;
}

/*************************************************************************/

/**
 * Returns the value from a key/value pair set.
 * This allows module coders to retrive any data they have previuosly stored in any given struct
 * @param md The module data for the struct to be used
 * @param key The key to find the data for
 * @return the value paired to the given key will be returned, or NULL 
 **/
char *moduleGetData(ModuleData ** md, char *key)
{

    char *mod_name = sstrdup(mod_current_module_name);
    ModuleData *modcurrent = *md;

    SET_SEGV_LOCATION();

    alog(LOG_DEBUG, "debug: moduleGetData %p : key %s", (void *) md, key);
    alog(LOG_DEBUG, "debug: Current Module %s", mod_name);

    if (mod_current_module_name == NULL) {
        alog(LOG_DEBUG,
             "moduleGetData() called with mod_current_module_name being NULL");
        if (denora->debug) {
            do_backtrace(0);
        }
    }

    while (modcurrent) {
        if ((stricmp(modcurrent->moduleName, mod_name) == 0)
            && (stricmp(modcurrent->key, key) == 0)) {
            free(mod_name);
            return sstrdup(modcurrent->value);
        }
        modcurrent = modcurrent->next;
    }
    free(mod_name);
    return NULL;
}

/*************************************************************************/

/**
 * Delete the key/value pair indicated by "key" for the current module.
 * This allows module coders to remove a previously stored key/value pair.
 * @param md The module data for the struct to be used
 * @param key The key to delete the key/value pair for
 **/
void moduleDelData(ModuleData ** md, char *key)
{
    char *mod_name = sstrdup(mod_current_module_name);
    ModuleData *modcurrent = *md;
    ModuleData *prev = NULL;
    ModuleData *next = NULL;

    SET_SEGV_LOCATION();

    if (mod_current_module_name == NULL) {
        alog(LOG_DEBUG,
             "debug: moduleDelData() called with mod_current_module_name being NULL");
        if (denora->debug) {
            do_backtrace(0);
        }
    }

    if (key) {
        while (modcurrent) {
            next = modcurrent->next;
            if ((stricmp(modcurrent->moduleName, mod_name) == 0)
                && (stricmp(modcurrent->key, key) == 0)) {
                if (prev) {
                    prev->next = modcurrent->next;
                } else {
                    *md = modcurrent->next;
                }
                free(modcurrent->moduleName);
                free(modcurrent->key);
                free(modcurrent->value);
                modcurrent->next = NULL;
                free(modcurrent);
            } else {
                prev = modcurrent;
            }
            prev = modcurrent;
            modcurrent = next;
        }
    }
    free(mod_name);
}

/*************************************************************************/

/**
 * This will remove all data for a particular module from existing structs.
 * Its primary use is modulePrepForUnload() however, based on past expericance with module coders wanting to 
 * do just about anything and everything, its safe to use from inside the module.
 * @param md The module data for the struct to be used
 **/
void moduleDelAllData(ModuleData ** md)
{
    char *mod_name = sstrdup(mod_current_module_name);
    ModuleData *modcurrent = *md;
    ModuleData *prev = NULL;
    ModuleData *next = NULL;

    SET_SEGV_LOCATION();

    if (mod_current_module_name == NULL) {
        alog(LOG_DEBUG,
             "debug: moduleDelAllData() called with mod_current_module_name being NULL");
        if (denora->debug) {
            do_backtrace(0);
        }
    }

    while (modcurrent) {
        next = modcurrent->next;
        if ((stricmp(modcurrent->moduleName, mod_name) == 0)) {
            if (prev) {
                prev->next = modcurrent->next;
            } else {
                *md = modcurrent->next;
            }
            free(modcurrent->moduleName);
            free(modcurrent->key);
            free(modcurrent->value);
            modcurrent->next = NULL;
            free(modcurrent);
        } else {
            prev = modcurrent;
        }
        modcurrent = next;
    }
    free(mod_name);
}

/*************************************************************************/

/**
 * This will delete all module data used in any struct by module m.
 * @param m The module to clear all data for
 **/
void moduleDelAllDataMod(Module * m)
{
    boolean freeme = false;
    int i;
    User *user;

    SET_SEGV_LOCATION();

    if (!mod_current_module_name) {
        mod_current_module_name = sstrdup(m->name);
        freeme = true;
    }

    for (i = 0; i < 1024; i++) {
        /* Remove the users */
        for (user = userlist[i]; user; user = user->next) {
            moduleDelAllData(&user->moduleData);
        }
    }

    if (freeme) {
        free(mod_current_module_name);
        mod_current_module_name = NULL;
    }
}
