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

Exclude *exlists[1024];
static Exclude *current;
static int next_index;

/*************************************************************************/

void insert_exclude(Exclude * e)
{
	int exindex = EXCLUDEHASH(e->name);

	e->prev = NULL;
	e->next = exlists[exindex];
	if (e->next)
		e->next->prev = e;
	exlists[exindex] = e;
}

/*************************************************************************/

Exclude *find_exclude(char *mask, char *server)
{
	Exclude *e;

	

	if (!mask)
	{
		return NULL;
	}

	for (e = exlists[EXCLUDEHASH(mask)]; e; e = e->next)
	{
		if (match_wild_nocase(e->name, mask))
		{
			return e;
		}

		if (NumExcludeServers && server)
		{
			if (isExcludedServer(server))
			{
				return e;
			}
		}
	}
	return NULL;
}

/*************************************************************************/

DENORA_INLINE boolean is_excluded(User * u)
{
	Exclude *e;
	e = find_exclude(u->nick, u->server->name);
	return (e ? true : false);
}

/*************************************************************************/

DENORA_INLINE boolean is_excludedserv(Server * server)
{
	if (server && NumExcludeServers && isExcludedServer(server->name))
	{
		return true;
	}

	return false;
}

/*************************************************************************/

DENORA_INLINE int isExcludedServer(char *name)
{
	int j;

	for (j = 0; j < NumExcludeServers; j++)
	{
		if (match_wild_nocase(ExcludeServers[j], name))
		{
			return 1;
		}
	}
	return 0;
}

/*************************************************************************/

/* Load/save data files. */

void load_exclude_db(void)
{
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(excludeDB, EXCLUDE_VERSION, &key, &value);
	Exclude *e = NULL;
	int retval = 0;

        if (!dbptr)
        {
                return;                 /* Bang, an error occurred */
        }
        

	while (1)
	{
		/* read a new entry and fill key and value with it -Certus */
		retval = new_read_db_entry(&key, &value, dbptr->fptr);

		if (retval == DB_READ_ERROR)
		{
			alog(LOG_NORMAL, "WARNING! DB_READ_ERROR in %s",
			     dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_EOF_ERROR)
		{
			alog(LOG_EXTRADEBUG, "debug: %s read successfully",
			     dbptr->filename);
			filedb_close(dbptr, &key, &value);
			return;
		}
		else if (retval == DB_READ_BLOCKEND)            /* DB_READ_BLOCKEND */
		{
			/* a exclude entry has completely been read. put any checks in here! */
		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
				continue;

			if (!stricmp(key, "name"))
			{
				e = make_exclude(value);
			}
			else if (!stricmp(key, "flag"))
			{
				e->flag = atoi(value);
				if (!e->flag)
				{
					e->flag |= EXCLUDE_USER;
				}
			}
		}                       /* else */
	}                           /* while */
}

/*************************************************************************/

Exclude *make_exclude(char *mask)
{
	Exclude *e;
	User *u;

	e = calloc(sizeof(Exclude), 1);
	e->name = sstrdup(mask);
	e->flag = 1;
	insert_exclude(e);

	u = user_find(mask);
	if (u)
	{
		u->cstats = 0;
	}
	return e;
}

/*************************************************************************/

void save_exclude_db(void)
{
	DenoraDBFile *dbptr = filedb_create(excludeDB, EXCLUDE_VERSION);
	Exclude *e;
	int i;

	for (i = 0; i < 1024; i++)
	{
		for (e = exlists[i]; e; e = e->next)
		{
			new_write_db_entry("name", dbptr, "%s", e->name);
			new_write_db_entry("flag", dbptr, "%d", e->flag);
			new_write_db_endofblock(dbptr);
		}
	}

	filedb_close(dbptr, NULL, NULL);  /* close file */
}

/*************************************************************************/

int del_exclude(Exclude * e)
{
	

	

	if (e)
	{
		if (e->prev)
		{
			e->prev->next = e->next;
		}
		else
		{
			exlists[EXCLUDEHASH(e->name)] = e->next;
		}
		if (e->next)
		{
			e->next->prev = e->prev;
		}
		if (e->name)
		{
			
			free(e->name);
		}
		free(e);
		return 1;
	}
	return 0;
}

/*************************************************************************/

Exclude *first_exclude(void)
{
	next_index = 0;

	

	while (next_index < 1024 && current == NULL)
	{
		current = exlists[next_index++];
	}
	

	alog(LOG_EXTRADEBUG, "debug: first_exclude() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

Exclude *next_exclude(void)
{
	

	if (current)
		current = current->next;
	if (!current && next_index < 1024)
	{
		while (next_index < 1024 && current == NULL)
		{
			current = exlists[next_index++];
		}
	}
	

	alog(LOG_EXTRADEBUG, "debug: next_exclude() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}
