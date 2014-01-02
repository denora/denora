
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
sqlite3 *TLDDatabase;

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
	TLD *t;

	t = findtld(country_code);

	if (t)
	{
		if (t->count > 0)
		{
			t->count--;
		}
		DenoraExecQuerySQL(TLDDatabase, "UPDATE %s SET count=%u, overall=%u WHERE code=\'%s\'",
		          TLDTable, t->count, t->overall, country_code);

		if (denora->do_sql)
		{
			sql_query("UPDATE %s SET count=%u, overall=%u WHERE code=\'%s\'",
			          TLDTable, t->count, t->overall, country_code);
		}
		fini_tld(t);
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

	if (type == 1 || type == 4)
	{
		if (denora->do_sql)
		{
			sqlcountry = sql_escape(country);
			sql_query("INSERT INTO %s  (code, country, count, overall) VALUES(\'%s\', \'%s\', %d, %d) \
					ON DUPLICATE KEY UPDATE count=%d, overall=%d",
					TLDTable, code, sqlcountry, count, overall, count, overall);
			if (sqlcountry)
			{
				free(sqlcountry);
			}
		}
	}

	return;
}

/*************************************************************************/

/**
 * Create a new entry in the TLD structs
 *
 * @param country is the two letter code for the country
 * @return TLD struct
 *
 */
TLD *make_tld(char **data)
{
	TLD *t;

	t = malloc(sizeof(TLD));
	t->countrycode = sstrdup(data[0]);
	t->country = sstrdup(data[1]);
	if (!data[2])
	{
		t->count = 1;
	}
	else
	{
		t->count = atoi(data[2]);
	}
	if (!data[3])
	{
		t->overall = 1;
	}
	else
	{
		t->overall = atoi(data[3]);
	}
	return t;
}

/*************************************************************************/

TLD *findtld(const char *countrycode)
{
	TLD *t;
	sqlite3_stmt *stmt;
	char **sdata;

	if (!countrycode)
	{
		return NULL;
	}

	TLDDatabase = DenoraOpenSQL(TLDDB);
	stmt = DenoraPrepareQuery(TLDDatabase, "SELECT * FROM %s WHERE code='%q'", TLDTable, countrycode);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (sdata) {
		t = make_tld(sdata);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(TLDDatabase);

	if (stricmp(t->countrycode, countrycode) == 0)
	{
		return t;
	}
	return NULL;
}

/*************************************************************************/

void fini_tld(TLD *t)
{
	if (t->countrycode)
	{
		free(t->countrycode);
	}
	if (t->country)
	{
		free(t->country);
	}
	free(t);
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
	char **data;

	if (code)
	{
		t = findtld(code);
	}
	else
	{
		code = sstrdup("??");
		t = findtld(code);
	}

	if (!t)
	{
		DenoraExecQuerySQL(TLDDatabase, "INSERT INTO %s (code, country, count, overall) VALUES(\'%s\', \'%s\', %d, %d)",
				TLDTable, code, country, 1, 1);

		data = DenoraCallocArray(1);
		data[0] = code;
		data[1] = country;
		t2 = make_tld(data);
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
		DenoraExecQuerySQL(TLDDatabase, "UPDATE %s SET count=%d, overall=%d WHERE code=\'%s\'",
				TLDTable, t->count, t->overall, t->countrycode);
		return t;
	}
}
