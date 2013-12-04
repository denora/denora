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

sqlite3 *CTCPDatabase;

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
	CTCPVerStats *c = NULL;
	sqlite3_stmt *stmt;
	char **sdata;


	CTCPDatabase = DenoraOpenSQL(AdminDB);
	stmt = DenoraPrepareQuery(CTCPDatabase, "SELECT * FROM %s WHERE version='%q'", mask);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (sdata) {
		c = makectcp(sdata);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(CTCPDatabase);


	if (stricmp(c->version, mask) == 0)
	{
		return c;
	}
	return NULL;
}



/*************************************************************************/

CTCPVerStats *makectcp(char **mask)
{
	CTCPVerStats *c = NULL;

	c = malloc(sizeof(CTCPVerStats));
	c->version = sstrdup(mask[0]);
	c->count = atoi(mask[1]);
	c->overall = atoi(mask[2]);
	return c;
}

/*************************************************************************/

int del_ctcpver(CTCPVerStats * c)
{

	if (c->version)
	{
		free(c->version);
	}
	free(c);
	return 0;
}

/*************************************************************************/

void ctcp_update(char *version)
{
	CTCPVerStats *c;
	char *dbversion;

	c = find_ctcpver(version);
	if (!c)
	{
		return;
	}
	if (c->count > 0)
	{
		c->count--;
	}
	DenoraSQLQuery(ctcpDB, "UPDATE %s SET count=%u, overall=%u WHERE version=\'%q\'",
	          CTCPTable, c->count, c->overall, version);

	if (denora->do_sql)
	{
		dbversion = rdb_escape(version);
		rdb_query(QUERY_LOW,
	          "UPDATE %s SET count=%u, overall=%u WHERE version=\'%s\'",
	          CTCPTable, c->count, c->overall, dbversion);
		free(dbversion);
	}
	free(c);
}

/*************************************************************************/

void handle_ctcp_version(char *nick, char *version)
{
	CTCPVerStats *c;
	User *u;
	char *dbversion;

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
		DenoraSQLQuery(ctcpDB, "UPDATE %s SET count=%u, overall=%u WHERE version=\'%q\'",
	          CTCPTable, c->count, c->overall, version);
		sql_do_ctcp(UPDATE, version, c->count, c->overall);
	}
	else
	{
		DenoraSQLQuery(ctcpDB, "INSERT INTO %s (version, count, overall) VALUES(\'%q\', 1, 1)",
				  CTCPTable, version);
		sql_do_ctcp(ADD, version, 1, 1);
	}
	u = user_find(nick);

	send_event(EVENT_CTCP_VERSION, 2, nick, version);

	if (u)
	{
		u->ctcp = sstrdup(version);
		DenoraSQLQuery(UserDB, "UPDATE %s SET ctcpversion=\'%q\' WHERE nick=%q",
			  UserTable, version, u->nick);

		if (denora->do_sql)
		{
			dbversion = rdb_escape(version);
			rdb_query(QUERY_LOW,
			  "UPDATE %s SET ctcpversion=\'%s\' WHERE nickid=%d",
			  UserTable, dbversion, db_getnick(u->sqlnick));
			free(dbversion);
		}
	}
}

/*************************************************************************/

void sql_do_ctcp(int type, char *version, int count, int overall)
{
	char *temp;

	if (version)
	{
		temp = rdb_escape(version);

		if (type == 1)
		{
			SET_SEGV_LOCATION();
			rdb_query(QUERY_LOW,
				  "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d)",
				  CTCPTable, temp, count, overall);
		}
		if (type == 4)
		{
					rdb_query(QUERY_LOW,
						  "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d) \
							ON DUPLICATE KEY UPDATE count=%d, overall=%d",
						  CTCPTable, temp, count, overall, count, overall);
		}
		free(temp);
	}
}
