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

/*************************************************************************/

Exclude *find_exclude(char *mask, int flag)
{
	Exclude *e;
	sqlite3_stmt *stmt;
	char **sdata;
	sqlite3 *db;

	if (!mask)
	{
		return NULL;
	}

	db = DenoraOpenSQL(DenoraDB);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s WHERE name='%q'", ExcludeTable, mask);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (sdata) {
		if (match_wild_nocase(sdata[0], mask) && flag == atoi(sdata[1]))
		{
			return make_exclude(sdata);
		}
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);
	return NULL;	
}

/*************************************************************************/

DENORA_INLINE boolean is_excluded_user(User * u)
{
	Exclude *e;
	e = find_exclude(u->nick, EXCLUDE_USER);
	if (e)
	{
		del_exclude(e);
		return true;
	}
	return false;
}

/*************************************************************************/

DENORA_INLINE boolean is_excluded_server(Server * server)
{
	Exclude *e;
	e = find_exclude(server->name, EXCLUDE_SERVER);
	if (e)
	{
		del_exclude(e);
		return true;
	}
	return false;
}

/*************************************************************************/

Exclude *make_exclude(char **data)
{
	Exclude *e;
	User *u;

	e = calloc(sizeof(Exclude), 1);
	e->name = sstrdup(data[0]);
	e->flag = atoi(data[1]);

	return e;
}


void Create_Exclude(char *key, int flag)
{

	DenoraSQLQuery(DenoraDB, "INSERT INTO %s (key, flag) VALUES ('%q', %d)", 
		  ExcludeTable, key, flag); 

}

void Delete_Exclude(char *key, int flag)
{

	DenoraSQLQuery(DenoraDB, "DELETE FROM %s WHERE key = '%q' and flag = %d", ExcludeTable, key, flag);
	
}

/*************************************************************************/

int del_exclude(Exclude * e)
{
	if (e)
	{
		if (e->name)
		{
			
			free(e->name);
		}
		free(e);
		return 1;
	}
	return 0;
}

