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

Dadmin *adminlists[MAX_ADMINS];
static Dadmin *current;
static int next_index;

/*************************************************************************/

void insert_admin(Dadmin * a)
{
	int adminindex = ADMINHASH(a->name);

	SET_SEGV_LOCATION();

	a->prev = NULL;
	a->next = adminlists[adminindex];
	if (a->next)
	{
		a->next->prev = a;
	}
	adminlists[adminindex] = a;

	return;
}

/*************************************************************************/

Dadmin *find_admin_byname(char *mask)
{
	Dadmin *a;

	SET_SEGV_LOCATION();

	if (!mask)
	{
		return NULL;
	}
	for (a = adminlists[ADMINHASH(mask)]; a; a = a->next)
	{
		if (stricmp(a->name, mask) == 0)
		{
			return a;
		}
	}
	return NULL;
}

/*************************************************************************/

Dadmin *find_admin(char *name, User * u)
{
	Dadmin *a;
	int i;

	for (a = adminlists[ADMINHASH(name)]; a; a = a->next)
	{
		for (i = 0; a->hosts[i]; i++)
		{
			if ((!strcmp(name, a->name))
			        && match_usermask(a->hosts[i], u))
			{
				return a;
			}
		}
	}
	return NULL;
}


/*************************************************************************/

void load_admin_db(void)
{
	/* TODO: check if key,value needs free */
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(AdminDB, ADMIN_VERSION, &key, &value);
	int retval = 0;
	Dadmin *x = NULL;
	Dadmin *a = NULL;

        if (!dbptr)
        {
                return;                 /* Bang, an error occurred */
        }
        SET_SEGV_LOCATION();

	while (1)
	{
		/* read a new entry and fill key and value with it -Certus */
		retval = new_read_db_entry(&key, &value, dbptr->fptr);

		if (retval == DB_READ_ERROR)
		{
			alog(LOG_NORMAL, langstr(ALOG_DB_ERROR), dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_EOF_ERROR)
		{
			alog(LOG_EXTRADEBUG, langstr(ALOG_DEBUG_DB_OK),
			     dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_READ_BLOCKEND)            /* DB_READ_BLOCKEND */
		{


		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
			{
				continue;
			}
			SET_SEGV_LOCATION();

			if (!stricmp(key, "name"))
			{
				a = find_admin_byname(value);
				if (!a)
				{
					x = make_admin(value);
				}
				else
				{
					alog(LOG_ERROR,
					     "Admin %s from denora.conf overrides the one in admin.db",
					     value);
					return;
				}
			}
			else if (!stricmp(key, "password"))
			{
				x->passwd = sstrdup(value);
			}
			else if (!stricmp(key, "host"))
			{
				x->hosts[0] = sstrdup(value);
			}
			else if (!stricmp(key, "lang"))
			{
				x->language = atoi(value);
			}
		}                       /* else */
	}                           /* while */

	return;
}

/*************************************************************************/

void save_admin_db(void)
{
	DenoraDBFile *dbptr = filedb_create(AdminDB, ADMIN_VERSION);
	Dadmin *a;
	int i;

	SET_SEGV_LOCATION();
	for (i = 0; i < MAX_ADMINS; i++)
	{
		for (a = adminlists[i]; a; a = a->next)
		{
			if (!a->configfile)
			{
				new_write_db_entry("name", dbptr, "%s", a->name);
				new_write_db_entry("password", dbptr, "%s", a->passwd);
				new_write_db_entry("host", dbptr, "%s", a->hosts[0]);
				new_write_db_entry("lang", dbptr, "%d", a->language);
				new_write_db_endofblock(dbptr);
			}
		}
	}
	SET_SEGV_LOCATION();

	filedb_close(dbptr, NULL, NULL);  /* close file */

	return;
}



/*************************************************************************/

Dadmin *make_admin(char *mask)
{
	Dadmin *a;

	a = calloc(sizeof(Dadmin), 1);
	SET_SEGV_LOCATION();

	a->name = sstrdup(mask);
	insert_admin(a);
	return a;
}

/*************************************************************************/

int free_admin(Dadmin * a)
{
	int i = 0;

	SET_SEGV_LOCATION();

	if (a)
	{
		if (a->prev)
		{
			a->prev->next = a->next;
		}
		else
		{
			adminlists[ADMINHASH(a->name)] = a->next;
		}
		if (a->next)
		{
			a->next->prev = a->prev;
		}
		while (a->hosts[i])
		{
			free(a->hosts[i]);
			i++;
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

Dadmin *first_admin(void)
{
	next_index = 0;

	SET_SEGV_LOCATION();

	while (next_index < MAX_ADMINS && current == NULL)
	{
		current = adminlists[next_index++];
	}
	SET_SEGV_LOCATION();

	alog(LOG_EXTRADEBUG, "debug: first_admin() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

Dadmin *next_admin(void)
{
	SET_SEGV_LOCATION();

	if (current)
	{
		current = current->next;
	}
	if (!current && next_index < MAX_ADMINS)
	{
		while (next_index < MAX_ADMINS && current == NULL)
		{
			current = adminlists[next_index++];
		}
	}
	SET_SEGV_LOCATION();

	alog(LOG_EXTRADEBUG, "debug: next_admin() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

void add_sqladmin(char *name, char *passwd, int level, char *host, int lang)
{
	int crypted = is_crypted(passwd);

	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return;
	}

	rdb_query(QUERY_LOW,
		  "INSERT INTO %s (uname, passwd, level, host, lang) VALUES ('%s', '%s', %d, %s%s%s, %d) \
		   ON DUPLICATE KEY UPDATE passwd=%s%s%s, level=%d, host='%s', lang=%d",
		  AdminTable, name, crypted ? "'" : "MD5('", passwd, crypted ? "'" : "')", level, host, lang, 
				    crypted ? "'" : "MD5('", passwd, crypted ? "'" : "')", level, host, lang);
	return;
}

/*************************************************************************/

int del_sqladmin(char *name)
{
	SET_SEGV_LOCATION();

	if (!denora->do_sql)
	{
		return -1;
	}

	rdb_query(QUERY_LOW, "DELETE FROM %s WHERE uname = '%s'",
	          AdminTable, name);
	return 0;
}

/*************************************************************************/

void reset_sqladmin(void)
{
	Dadmin *a;
	int i;
	int crypted;

	SET_SEGV_LOCATION();

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

	return;
}
