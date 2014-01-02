/*
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 *
 *
 */

#include "denora.h"
sqlite3 *ModDataDB;
char *moddata_db;

/*************************************************************************/

/**
 * Add module data to a struct.
 * This allows module coders to add data to an existing struct
 * @param md The module data for the struct to be used
 * @param key The Key for the key/value pair
 * @param value The value for the key/value pair, this is what will be stored for you
 * @return MOD_ERR_OK will be returned on success
 **/
int moduleAddData(char *mod_name, char *what, char *key, char *value)
{
	int res;

	if (!key || !value)
	{
		alog(LOG_DEBUG,
		     "debug: A module tried to use ModuleAddData() with one ore more NULL arguments... returning");
		do_backtrace(0);
		return MOD_ERR_PARAMS;
	}

	res = DenoraSQLGetNumRowsFromQuery(ModDataDB, "SELECT * FROM moddata WHERE mod_name=%q and key=%q and what=%q", mod_name, key, what);
	if (res)
	{
		res = DenoraExecQuerySQL(ModDataDB, "UPDATE moddata SET value=%q WHERE mod_name=%q and key=%q and what=%q", value, mod_name, key, what); 
	}
	else
	{
		res = DenoraExecQuerySQL(ModDataDB, "INSERT INTO moddata (mod_name, what, key, valye) VALUES ('%q', '%q', '%q', '%q')", mod_name, what, key, value); 
	}
	if (res == SQLITE_OK)
	{
		return MOD_ERR_OK;
	}
	return MOD_ERR_SQL;
}

/*************************************************************************/

/**
 * Returns the value from a key/value pair set.
 * This allows module coders to retrive any data they have previuosly stored in any given struct
 * @param md The module data for the struct to be used
 * @param key The key to find the data for
 * @return the value paired to the given key will be returned, or NULL
 **/
char *moduleGetData(char *mod_name, char *what, char *key)
{
	char **data;

	data = DenoraSQLReturnRow(moddata_db, "SELECT value FROM moddata WHERE mod_name=%q and key=%q and what=%q", mod_name, what, key);

	if (data)
	{
		return data[0];
	}
	return NULL;
}

/*************************************************************************/

/**
 * Delete the key/value pair indicated by "key" for the current module.
 * This allows module coders to remove a previously stored key/value pair.
 * @param md The module data for the struct to be used
 * @param key The key to delete the key/value pair for
 **/
void moduleDelData(char *mod_name, char *what, char *key)
{

  DenoraExecQuerySQL(ModDataDB, "DELETE FROM mod_data WHERE mod_name = '%q' and what = %q and key = %q", mod_name, what, key);

}

/*************************************************************************/

/**
 * This will remove all data for a particular module from existing structs.
 * Its primary use is modulePrepForUnload() however, based on past expericance with module coders wanting to
 * do just about anything and everything, its safe to use from inside the module.
 * @param md The module data for the struct to be used
 **/
void moduleDelAllData(char *mod_name)
{

  DenoraExecQuerySQL(ModDataDB, "DELETE FROM mod_data WHERE mod_name = '%q'", mod_name);

}



