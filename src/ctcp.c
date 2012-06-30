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
 *
 *
 */

#include "denora.h"

list_t *CTCPhead;

static int comparectcp(const void *v, const void *cc)
{
	const CTCPVerStats *t = (void *) v;
	return (stricmp(t->version, (char *) cc));
}

int sortctcp(const void *v, const void *v2)
{
	const CTCPVerStats *t = (void *) v;
	const CTCPVerStats *t2 = (void *) v2;
	return (t2->overall - t->overall);
}

/*************************************************************************/

void init_ctcp(void)
{
	CTCPVerStats *t;
	lnode_t *tn;
	SET_SEGV_LOCATION();

	CTCPhead = list_create(-1);
	t = malloc(sizeof(CTCPVerStats));
	bzero(t, sizeof(CTCPVerStats));
	t->version = sstrdup("?");
	tn = lnode_create(t);
	list_append(CTCPhead, tn);
}

/*************************************************************************/

/**
 * Locate ctcp struct by the mask
 *
 * @param mask is the ctcp to locate
 * @return CTCP stats struct
 *
 */
CTCPVerStats *find_ctcpver(char *mask)
{
	lnode_t *tn;
	CTCPVerStats *c = NULL;

	tn = list_find(CTCPhead, mask, comparectcp);
	if (tn)
	{
		c = lnode_get(tn);
		return c;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

/**
 * Load the CTCP database from disk
 *
 * @return void - no returend value
 *
 */
void load_ctcp_db(void)
{
	DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
	CTCPVerStats *ct = NULL;
	char *key, *value;
	int retval = 0;
	char *version = NULL;
	int tempoverall = 0;

	alog(LOG_NORMAL, "Loading %s", ctcpDB);

	fill_db_ptr(dbptr, 0, CTCP_VERSION, s_StatServ, ctcpDB);
	SET_SEGV_LOCATION();

	/* let's remove existing temp files here, because we only load dbs on startup */
	remove(dbptr->temp_name);

	/* Open the db, fill the rest of dbptr and allocate memory for key and value */
	if (new_open_db_read(dbptr, &key, &value))
	{
		free(dbptr);
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
			new_close_db(dbptr->fptr, &key, &value);
			free(dbptr);
			return;
		}
		else if (retval == DB_EOF_ERROR)
		{
			alog(LOG_EXTRADEBUG, langstr(ALOG_DEBUG_DB_OK),
			     dbptr->filename);
			new_close_db(dbptr->fptr, &key, &value);
			free(dbptr);
			return;
		}
		else if (retval == DB_READ_BLOCKEND)            /* DB_READ_BLOCKEND */
		{
			/* a ctcp entry has completely been read. put any checks in here! */
			if (tempoverall > 1)
			{
				ct = makectcp(version);
				ct->overall = tempoverall;
				free(version);
			}
		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
			{
				continue;
			}

			if (!stricmp(key, "version"))
			{
				version = sstrdup(value);
				strnrepl(version, BUFSIZE, "¶", ":");
			}
			else if (!stricmp(key, "count"))
			{
				tempoverall = atoi(value);
			}
		}                       /* else */
	}                           /* while */
}

/*************************************************************************/

CTCPVerStats *makectcp(char *mask)
{
	lnode_t *tn;
	CTCPVerStats *c = NULL;
	SET_SEGV_LOCATION();

	tn = list_find(CTCPhead, mask, comparectcp);
	if (tn)
	{
		c = lnode_get(tn);
	}
	else
	{
		c = malloc(sizeof(CTCPVerStats));
		c->version = sstrdup(mask);
		c->count = 0;
		c->overall = 0;
		tn = lnode_create(c);
		list_append(CTCPhead, tn);
	}
	return c;
}

/*************************************************************************/

/**
 * Save the CTCP database to disk
 *
 * @return void - no returend value
 *
 */
void save_ctcp_db(void)
{
	DenoraDBFile *dbptr = calloc(1, sizeof(DenoraDBFile));
	CTCPVerStats *c;
	char *version;
	lnode_t *tn;

	fill_db_ptr(dbptr, 0, CTCP_VERSION, s_StatServ, ctcpDB);
	SET_SEGV_LOCATION();

	/* time to backup the old db */
	rename(ctcpDB, dbptr->temp_name);

	if (new_open_db_write(dbptr))
	{
		rename(dbptr->temp_name, ctcpDB);
		free(dbptr);
		return;                 /* Bang, an error occurred */
	}
	SET_SEGV_LOCATION();

	tn = list_first(CTCPhead);
	while (tn != NULL)
	{
		c = lnode_get(tn);
		if (c->overall > 1)
		{
			version = sstrdup(c->version);
			strnrepl(version, BUFSIZE, ":", "¶");
			new_write_db_entry("version", dbptr, "%s", version);
			free(version);
			new_write_db_entry("count", dbptr, "%u", c->overall);
			new_write_db_endofblock(dbptr);
		}
		tn = list_next(CTCPhead, tn);
	}

	if (dbptr)
	{
		new_close_db(dbptr->fptr, NULL, NULL);  /* close file */
		SET_SEGV_LOCATION();
		remove(dbptr->temp_name);       /* saved successfully, no need to keep the old one */
		free(dbptr);            /* free the db struct */
	}
}

/*************************************************************************/

int del_ctcpver(__attribute__((unused))CTCPVerStats * c)
{
	return 0;
}

/*************************************************************************/

void ctcp_update(char *version)
{
	CTCPVerStats *c;

	SET_SEGV_LOCATION();

	c = find_ctcpver(version);
	if (!c)
	{
		return;
	}
	if (c->count > 0)
	{
		c->count--;
	}
	version = rdb_escape(version);
	rdb_query(QUERY_LOW,
	          "UPDATE %s SET count=%u, overall=%u WHERE version=\'%s\'",
	          CTCPTable, c->count, c->overall, version);
	SET_SEGV_LOCATION();
	free(version);
}

/*************************************************************************/

void handle_ctcp_version(char *nick, char *version)
{
	CTCPVerStats *c;
	User *u;

	SET_SEGV_LOCATION();

	if (LargeNet)
	{
		return;
	}

	if (!version)
	{
		/* client sent us nothing */
		return;
	}

	c = find_ctcpver(version);
	if (c)
	{
		c->count++;
		if (c->count > c->overall)
		{
			c->overall = c->count;
		}
		if (denora->do_sql)
		{
			sql_do_ctcp(UPDATE, version, c->count, c->overall);
		}
	}
	else
	{
		c = makectcp(version);
		c->count++;
		c->overall++;
		if (denora->do_sql)
		{
			sql_do_ctcp(ADD, version, 1, 1);
		}
	}
	u = user_find(nick);
	SET_SEGV_LOCATION();

	send_event(EVENT_CTCP_VERSION, 2, nick, version);

	if (u)
	{
		if (!u->ctcp)
		{
			u->ctcp = sstrdup(version);
		}
		else
		{
			if (u->ctcp)
			{
				free(u->ctcp);
			}
			u->ctcp = sstrdup(version);
		}
		version = rdb_escape(version);
		rdb_query
		(QUERY_LOW, "UPDATE %s SET ctcpversion=\'%s\' WHERE nickid=%d",
		 UserTable, version, db_getnick(u->sqlnick));
		free(version);
	}
}

/*************************************************************************/

void sql_do_ctcp(int type, char *version, int count, int overall)
{
	char *temp;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#endif

	if (version)
	{
		temp = rdb_escape(version);

		if (type == 1)
		{
			SET_SEGV_LOCATION();
			rdb_query
			(QUERY_LOW,
			 "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d)",
			 CTCPTable, temp, count, overall);
		}
		if (type == 4)
		{
			SET_SEGV_LOCATION();
			rdb_query(QUERY_HIGH, "SELECT id FROM %s WHERE version=\'%s\'",
			          CTCPTable, temp);
#ifdef USE_MYSQL
			mysql_res = mysql_store_result(mysql);
			if (mysql_res)
			{
				if (mysql_num_rows(mysql_res))
				{
					rdb_query
					(QUERY_LOW,
					 "UPDATE %s SET count=%d, overall=%d WHERE version=\'%s\'",
					 CTCPTable, count, overall, temp);
				}
				else
				{
					rdb_query
					(QUERY_LOW,
					 "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d)",
					 CTCPTable, temp, count, overall);
				}
				SET_SEGV_LOCATION();
				mysql_free_result(mysql_res);
			}
#endif
		}
		SET_SEGV_LOCATION();
		free(temp);
	}
}
