/* CTCP
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */

#include "denora.h"

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
	sqlite3 *db;
	char **sdata;


	db = DenoraOpenSQL(DenoraDB);
	stmt = DenoraPrepareQuery(db, "SELECT * FROM %s WHERE version='%q'", mask);
	sdata = DenoraSQLFetchRow(stmt, FETCH_ARRAY_NUM);
	if (sdata) {
		c = makectcp(sdata);
	}
	sqlite3_finalize(stmt);
	DenoraCloseSQl(db);

	if (c)
	{
		if (stricmp(c->version, mask) == 0)
		{
			return c;
		}
	}
	return NULL;
}



/*************************************************************************/

CTCPVerStats *makectcp(char **mask)
{
	CTCPVerStats *c = NULL;

	if (mask)
	{
		c = malloc(sizeof(CTCPVerStats));
		c->version = sstrdup(mask[0]);
		c->count = atoi(mask[1]);
		c->overall = atoi(mask[2]);
		return c;
	}
	return NULL;
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

	c = find_ctcpver(version);
	if (!c)
	{
		return;
	}
	if (c->count > 0)
	{
		c->count--;
	}
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET count=%u, overall=%u WHERE version=\'%q\'",
	          CTCPTable, c->count, c->overall, version);

	del_ctcpver(c);
}

/*************************************************************************/

void handle_ctcp_version(char *nick, char *version)
{
	CTCPVerStats *c;
	User *u;
	char *dbversion;

	if (DisableCTCP)
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
		DenoraSQLQuery(DenoraDB, "UPDATE %s SET count=%u, overall=%u WHERE version=\'%q\'",
	          CTCPTable, c->count, c->overall, version);
	}
	else
	{
		DenoraSQLQuery(DenoraDB, "INSERT INTO %s (version, count, overall) VALUES(\'%q\', 1, 1)",
				  CTCPTable, version);
	}
	send_event(EVENT_CTCP_VERSION, 2, nick, version);

	DenoraSQLQuery(DenoraDB, "UPDATE %s SET ctcpversion=\'%q\' WHERE nick=%q",
			  UserTable, version, nick);
}

/*************************************************************************/

void sql_do_ctcp(int type, char *version, int count, int overall)
{
	if (version)
	{
		if (type == 1)
		{
			
			DenoraSQLQuery(DenoraDB, "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d)",
				  CTCPTable, version, count, overall);
		}
		if (type == 4)
		{
			DenoraSQLQuery(DenoraDB, "INSERT INTO %s (version, count, overall) VALUES(\'%s\', %d, %d) \
							ON DUPLICATE KEY UPDATE count=%d, overall=%d",
						  CTCPTable, version, count, overall, count, overall);
		}
	}
}
