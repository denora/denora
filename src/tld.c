
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

list_t *Thead;

static int findcc(const void *v, const void *cc)
{
	const TLD *t = (void *) v;
	return (stricmp(t->countrycode, (char *) cc));
}

int sortusers(const void *v, const void *v2)
{
	const TLD *t = (void *) v;
	const TLD *t2 = (void *) v2;
	return (t2->count - t->count);
}

/*************************************************************************/

/**
 * Update a given tld to reflect that a user of that tld has left
 *
 * @param country_code is the two letter code for the country
 * @return void - no returend value
 *
 */
void tld_update(char *country_code)
{
	lnode_t *tn;
	TLD *t = NULL;

	tn = list_find(Thead, country_code, findcc);

	if (tn)
	{
		t = lnode_get(tn);
		if (t->count > 0)
		{
			t->count--;
		}
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET count=%u, overall=%u WHERE code=\'%s\'",
		          TLDTable, t->count, t->overall, country_code);
	}
}

/*************************************************************************/

/**
 * Insert/Update data about the TLD into SQL
 *
 * @param type is either ADD or UPDATE
 * @param code is the two letter code for the country
 * @param country is the full country name
 * @param count is the current count for the TLD
 * @param overall is the total tld count
 * @return void - no returend value
 *
 */
void sql_do_tld(int type, char *code, char *country, int count, int overall)
{
	char *sqlcountry;
#ifdef USE_MYSQL
	MYSQL_RES *mysql_res;
#else
	USE_VAR(code);
	USE_VAR(count);
	USE_VAR(overall);
#endif

	if (!denora->do_sql)
	{
		return;
	}

	sqlcountry = rdb_escape(country);

	SET_SEGV_LOCATION();
	if (type == 1 || type == 4)
	{
		rdb_query(QUERY_HIGH,
		          "SELECT country FROM %s WHERE country = \'%s\';",
		          TLDTable, sqlcountry);
#ifdef USE_MYSQL
		mysql_res = mysql_store_result(mysql);
		if (mysql_res)
		{
			if (mysql_num_rows(mysql_res))         /* does the country already exist in the database? */
			{
				rdb_query(QUERY_LOW,
					"UPDATE %s SET count=%d, overall=%d WHERE code=\'%s\'",
					TLDTable, count, overall, code);
			}
			else
			{
				rdb_query(QUERY_LOW,
					"INSERT INTO %s (code, country, count, overall) VALUES(\'%s\', \'%s\', %d, %d)",
					TLDTable, code, sqlcountry, count, overall);
			}
			mysql_free_result(mysql_res);
		}
#endif
	}

	SET_SEGV_LOCATION();

	if (sqlcountry)
	{
		free(sqlcountry);
	}

	return;
}

/*************************************************************************/

void init_tld(void)
{
	TLD *t;
	lnode_t *tn;
	SET_SEGV_LOCATION();

	Thead = list_create(-1);
	t = malloc(sizeof(TLD));
	bzero(t, sizeof(TLD));
	ircsnprintf(t->countrycode, 5, "???");
	t->country = sstrdup("Unknown");
	tn = lnode_create(t);
	list_append(Thead, tn);
}

/*************************************************************************/

/**
 * Create a new entry in the TLD structs
 *
 * @param country is the two letter code for the country
 * @return TLD struct
 *
 */
TLD *new_tld(const char *countrycode, const char *country)
{
	lnode_t *tn;
	TLD *t = NULL;
	SET_SEGV_LOCATION();

	tn = list_find(Thead, countrycode, findcc);
	if (tn)
	{
		t = lnode_get(tn);
	}
	else
	{
		t = malloc(sizeof(TLD));
		strlcpy(t->countrycode, countrycode, 5);
		t->country = sstrdup(country);
		t->count = 0;
	    t->overall = 0;
		tn = lnode_create(t);
		list_append(Thead, tn);
	}
	return t;
}

/*************************************************************************/

TLD *findtld(const char *countrycode)
{
	lnode_t *tn;
	TLD *t = NULL;

	tn = list_find(Thead, countrycode, findcc);
	if (tn)
	{
		t = lnode_get(tn);
		return t;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

/**
 * Load the TLD database from disk
 *
 * @return void - no returend value
 *
 */
void load_tld_db(void)
{
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(TLDDB, TLD_VERSION, &key, &value);
	TLD *t = NULL;
	int retval = 0;
	char *tempcc = NULL;
	char *tempc = NULL;
	int overall = 0;

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
			t = new_tld(tempcc, tempc);
			t->overall = overall;
			if (tempc)
			{
				free(tempc);
			}
			if (tempcc)
			{
				free(tempcc);
			}
		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
				continue;

			if (!stricmp(key, "ccode"))
			{
				tempcc = sstrdup(value);
			}
			else if (!stricmp(key, "country"))
			{
				tempc = sstrdup(value);
			}
			else if (!stricmp(key, "overall"))
			{
				overall = atoi(value);
			}
		}                       /* else */
	}                           /* while */
}

/*************************************************************************/

/**
 * Save the TLD database to disk
 *
 * @return void - no returend value
 *
 */
void save_tld_db(void)
{
	DenoraDBFile *dbptr = filedb_create(TLDDB, TLD_VERSION);
	TLD *t;
	lnode_t *tn;

	tn = list_first(Thead);
	while (tn != NULL)
	{
		t = lnode_get(tn);
		new_write_db_entry("ccode", dbptr, "%s", t->countrycode);
		new_write_db_entry("country", dbptr, "%s", t->country);
		new_write_db_entry("overall", dbptr, "%u", t->overall);
		new_write_db_endofblock(dbptr);
		tn = list_next(Thead, tn);
	}

	filedb_close(dbptr, NULL, NULL);  /* close file */
}

/*************************************************************************/

void fini_tld(void)
{
	TLD *t;
	lnode_t *tn;

	tn = list_first(Thead);
	while (tn != NULL)
	{
		t = lnode_get(tn);
		free(t->country);
		free(t);
		tn = list_next(Thead, tn);
	}
	list_destroy_nodes(Thead);
}

/*************************************************************************/

/**
 * Handle increasing the TLD information
 *
 * @param country is the name of the country
 * @param code is the two letter country code
 * @return TLD struct
 *
 */
TLD *do_tld(char *country, char *code)
{
	TLD *t, *t2;

	if (code)
	{
		t = findtld(code);
	}
	else
	{
		code = sstrdup("??");
		t = findtld(code);
	}

	SET_SEGV_LOCATION();

	if (!t)
	{
		/* Allocate User structure and fill it in. */
		t2 = new_tld(code, country);
		t2->count = 1;
		t2->overall = 1;
		return t2;
	}
	else
	{
		t->count++;
		if (t->count > t->overall)
		{
			t->overall = t->count;
		}
		if (!t->overall)
		{
			t->overall = 1;
		}
		return t;
	}
}
