/* SQLite v3
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

#ifdef HAVE_SQLITE

/*************************************************************************/

/*************************************************************************/

void db_sqlite_error(int severity, const char *msg, int con)
{
}

/*************************************************************************/

int db_sqlite_init(int con)
{

}

/*************************************************************************/

int db_sqlite_open(int con)
{

}

/*************************************************************************/

int db_sqlite_query(char *sql, int con)
{

}

/*************************************************************************/

char *db_sqlite_quote(char *sql)
{

}

/*************************************************************************/

char *db_sqlite_hidepass(char *sql)
{
	int slen, pos, i;
	char *buf, *hidden;

	if (!sql)
	{
		return sstrdup("");
	}

	buf = strstr(sql, "MD5");
	if (!buf)
	{
		return sql;
	}

	buf = strstr(buf, ", ('");
	if (!buf)
	{
		return sql;
	}

	slen = strlen(sql);
	pos = (slen - strlen(buf)) + 4;

	free(buf);

	hidden = sstrdup(sql);
	for (i = pos; i < slen; i++)
	{
		if (hidden[i] != ')')
		{
			hidden[i] = 'x';
		}
		else
		{
			hidden[i - 1] = sql[i - 1];
			break;
		}
	}

	return hidden;
}

/*************************************************************************/

int db_sqlite_close(int con)
{

	return 1;
}

/*************************************************************************/

void db_sqlite_PrepareForQuery(int con)
{

}

#endif
