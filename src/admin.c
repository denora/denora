/*
 *
 * (c) 2004-2013 Denora Team
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

sqlite3* AdminDatabase;

/*************************************************************************/

Dadmin *find_admin_byname(char *mask)
{
	Dadmin *a;
	sqlite3_stmt *stmt;
	char **sdata;

	if (!mask)
	{
		return NULL;
	}

	AdminDatabase = DenoraOpenSQL(AdminDB);
	stmt = DenoraPrepareQuery(AdminDatabase, "SELECT * FROM %s WHERE uname='%q'", mask);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (sdata) {
		a = make_admin(sdata);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(AdminDatabase);

	if (stricmp(a->name, mask) == 0)
	{
		return a;
	}
	return NULL;
}

/*************************************************************************/

Dadmin *find_admin(char *name, User * u)
{
	Dadmin *a;

	a = find_admin_byname(name);

	if (match_usermask(a->hosts, u))
	{
		return a;
	}
	return NULL;
}


/*************************************************************************/

Dadmin *make_admin(char **data)
{
	Dadmin *a;

	a = calloc(sizeof(Dadmin), 1);

	a->name = sstrdup(data[0]);
	a->passwd = sstrdup(data[1]);	
	a->hosts = sstrdup(data[3]);
	a->language = atoi(data[4]);
	a->level = atoi(data[2]);
	a->configfile = atoi(data[5]);

	return a;
}

/*************************************************************************/

int free_admin(Dadmin * a)
{
	int i = 0;

	if (a)
	{
		if (a->hosts)
		{
			free(a->hosts);
		}
		if (a->passwd)
		{
			free(a->passwd);
		}
		if (a->name)
		{
			free(a->name);
		}
		free(a);
		return 1;
	}
	return 0;
}

/*************************************************************************/

int add_sqladmin(char *name, char *passwd, int level, char *host, int lang, int configadmin)
{
	int res;
	int crypted = is_crypted(passwd);

	res = DenoraExecQuerySQL(AdminDatabase, "INSERT INTO %s (uname, passwd, level, host, lang, config) VALUES ('%q', '%q', %d, %s, %d, %d)", 
		  AdminTable, name, passwd, level, host, lang, configadmin); 

	if (!denora->do_sql)
	{
		rdb_query(QUERY_LOW,
			  "INSERT INTO %s (uname, passwd, level, host, lang) VALUES ('%s', '%s', %d, %s%s%s, %d) \
			   ON DUPLICATE KEY UPDATE passwd=%s%s%s, level=%d, host='%s', lang=%d",
		  	   AdminTable, name, crypted ? "'" : "MD5('", passwd, crypted ? "'" : "')", level, host, lang, 
				    crypted ? "'" : "MD5('", passwd, crypted ? "'" : "')", level, host, lang);
	}
	return res;
}

/*************************************************************************/

int del_sqladmin(char *name)
{
	int res;

	res = DenoraExecQuerySQL(AdminDatabase, "DELETE FROM %s WHERE uname = '%q'", AdminTable, name);
	
	if (denora->do_sql)
	{
			rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname = '%s'",  AdminTable, name);
	}

	return res;
}

/*************************************************************************/

void reset_sqladmin(void)
{
	Dadmin *a;
	int i;
	int crypted;

	SET_SEGV_LOCATION();
/* this was for mysql benefit will probably move it to the mysql module once that work
   has started
*/
#if 0
	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW, "TRUNCATE TABLE %s", AdminTable);
		for (i = 0; i < MAX_ADMINS; i++)
		{
			for (a = adminlists[i]; a; a = a->next)
			{
				crypted = is_crypted(a->passwd);
				rdb_query(QUERY_LOW,
					"INSERT INTO %s (uname, passwd, level, host, lang) VALUES ('%s', %s%s%s, %d, '%s', %d) \
					ON DUPLICATE KEY UPDATE passwd=%s%s%s, level=%d, host='%s', lang=%d",
					AdminTable, a->name, crypted ? "'" : "MD5('", a->passwd, crypted ? "'" : "')", a->configfile, 
					a->hosts[0], a->language, crypted ? "'" : "MD5('", a->passwd, crypted ? "'" : "')", a->configfile,
					a->hosts[0], a->language);
			}
		}
	}
#endif
	return;
}


int AdminSetPassword(Dadmin * a, char *newpass)
{
	int crypted;

	free(a->passwd);
	a->passwd = sstrdup(MakePassword(newpass));
	crypted = is_crypted(a->passwd);
	DenoraExecQuerySQL(AdminDatabase, "UPDATE %s SET passwd=%s%q%s WHERE uname = '%q'", AdminTable, crypted ? "'" : "MD5('", a->passwd, crypted ? "'" : "')", a->name);

	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW, "UPDATE %s SET passwd=%s%s%s WHERE uname = '%s'", AdminTable, crypted ? "'" : "MD5('", a->passwd, crypted ? "'" : "')", a->name);
	}
	return 1;
}