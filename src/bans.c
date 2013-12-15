/* Network Bans
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

static int find_gline(const void *v, const void *cc)
{
	const Gline *t = (void *) v;
	return (stricmp(t->host, (char *) cc));
}

static int find_zline(const void *v, const void *cc)
{
	const Zline *t = (void *) v;
	return (stricmp(t->host, (char *) cc));
}

static int find_qline(const void *v, const void *cc)
{
	const Qline *t = (void *) v;
	return (stricmp(t->host, (char *) cc));
}

list_t *Glinehead;
list_t *Qlinehead;
list_t *Zlinehead;

/*************************************************************************/

void init_bans(void)
{
	Gline *g;
	Zline *z;
	Qline *q;
	

	Glinehead = list_create(-1);
	g = malloc(sizeof(Gline));
	bzero(g, sizeof(Gline));

	Zlinehead = list_create(-1);
	z = malloc(sizeof(Zline));
	bzero(z, sizeof(Zline));

	Qlinehead = list_create(-1);
	q = malloc(sizeof(Qline));
	bzero(q, sizeof(Qline));

}

/*************************************************************************/

Gline *new_Gline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
	lnode_t *tn;
	Gline *g = NULL;
	

	tn = list_find(Glinehead, host, find_gline);
	if (tn)
	{
		g = lnode_get(tn);
	}
	else
	{
		g = malloc(sizeof(Gline));
		g->host = sstrdup(host);
		g->user = sstrdup(user);
		g->setby = sstrdup(setby);
		g->expires = strtoul(expires, NULL, 10);
		g->setat = strtoul(setat, NULL, 10);
		g->reason = sstrdup(reason);
		tn = lnode_create(g);
		list_append(Glinehead, tn);
	}
	return g;
}

/*************************************************************************/

Zline *new_Zline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
	lnode_t *tn;
	Zline *z = NULL;
	

	tn = list_find(Zlinehead, host, find_zline);
	if (tn)
	{
		z = lnode_get(tn);
	}
	else
	{
		z = malloc(sizeof(Gline));
		z->host = sstrdup(host);
		z->user = sstrdup(user);
		z->setby = sstrdup(setby);
		z->expires = strtoul(expires, NULL, 10);
		z->setat = strtoul(setat, NULL, 10);
		z->reason = sstrdup(reason);
		tn = lnode_create(z);
		list_append(Zlinehead, tn);
	}
	return z;
}

/*************************************************************************/

Qline *new_Qline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason)
{
	lnode_t *tn;
	Qline *q = NULL;
	

	tn = list_find(Qlinehead, host, find_qline);
	if (tn)
	{
		q = lnode_get(tn);
	}
	else
	{
		q = malloc(sizeof(Gline));
		q->host = sstrdup(host);
		q->user = sstrdup(user);
		q->setby = sstrdup(setby);
		q->expires = strtoul(expires, NULL, 10);
		q->setat = strtoul(setat, NULL, 10);
		q->reason = sstrdup(reason);
		tn = lnode_create(q);
		list_append(Qlinehead, tn);
	}
	return q;
}

/*************************************************************************/

Gline *findGline(const char *host)
{
	lnode_t *tn;
	Gline *t = NULL;

	tn = list_find(Glinehead, host, find_gline);
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

Zline *findZline(const char *host)
{
	lnode_t *tn;
	Zline *t = NULL;

	tn = list_find(Zlinehead, host, find_zline);
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

Qline *findQline(const char *host)
{
	lnode_t *tn;
	Qline *t = NULL;

	tn = list_find(Qlinehead, host, find_qline);
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

void fini_bans(void)
{
	Gline *g;
	Qline *q;
	Zline *z;
	lnode_t *tn;

	tn = list_first(Glinehead);
	while (tn != NULL)
	{
		g = lnode_get(tn);
		if (g->user)
			free(g->user);
		if (g->host)
			free(g->host);
		if (g->setby)
			free(g->setby);
		if (g->reason)
			free(g->reason);
		free(g);
		tn = list_next(Glinehead, tn);
	}
	list_destroy_nodes(Glinehead);

	tn = list_first(Zlinehead);
	while (tn != NULL)
	{
		z = lnode_get(tn);
		if (z->user)
			free(z->user);
		if (z->host)
			free(z->host);
		if (z->setby)
			free(z->setby);
		if (z->reason)
			free(z->reason);
		free(z);
		tn = list_next(Zlinehead, tn);
	}
	list_destroy_nodes(Zlinehead);

	tn = list_first(Qlinehead);
	while (tn != NULL)
	{
		q = lnode_get(tn);
		if (q->user)
			free(q->user);
		if (q->host)
			free(q->host);
		if (q->setby)
			free(q->setby);
		if (q->reason)
			free(q->reason);
		free(q);
		tn = list_next(Qlinehead, tn);
	}
	list_destroy_nodes(Qlinehead);
}

/*************************************************************************/

/**
 * Parse a P10 ircd gline message into a format that sql_do_server_bans_* can handle
 *
 * @param source Server that sent the GLINE
 * @param ac Parameter Count
 * @param av Char array in 0 index
 *
 * @return void - no returend value
 */
void p10_gline(char *type, char *source, int ac, char **av)
{
	Server *s = NULL;
	User *u = NULL;
	char buf[BUFSIZE];
	char *user;
	char *host;
	char *address;
	char *setby;
	char expires[12];

	

	if (ac < 4 || ac > 6)
	{
		alog(LOG_DEBUG,
		     "debug: %s called with %d needed 4,5 or 6", PRETTY_FUNCTION,
		     ac);
		return;
	}

	if (source)
	{
		s = server_find(source);
		if (!s)
		{
			u = user_find(source);
			if (u)
			{
				setby = sstrdup(u->sqlnick);
			}
			else
			{
				setby = sstrdup(source);
			}
		}
		else
		{
			setby = sstrdup(s->name);
		}
	}
	else
	{
		alog(LOG_DEBUG,
		     "debug: %s called without a source", PRETTY_FUNCTION);
		return;
	}

	

	address = (*av[1] == '!') ? myStrGetToken(av[1], '!', 1) : av[1];
	if (*address == '+')
	{
		address = myStrGetToken(address, '+', 1);
	}
	else if (*address == '-')
	{
		address = myStrGetToken(address, '-', 1);
	}

	if (strstr(address, "@"))
	{
		user = myStrGetToken(address, '@', 0);
		host = myStrGetToken(address, '@', 1);
	}
	else
	{
		user = sstrdup("*");
		host = sstrdup(address);
	}

	

	if (*av[1] == '+')
	{
		*expires = '\0';
		if (ac == 6)
		{
			sprintf(expires, "%d", atoi(av[3]) + atoi(av[4]));
			sql_do_server_bans_add(type, user, host, setby, av[3], expires,
					       av[5]);
		}
		else if (ac == 5)
		{
			sprintf(expires, "%d", atoi(av[2]) + atoi(av[3]));
			sql_do_server_bans_add(type, user, host, setby, av[3], expires,
			                       av[4]);
		}
		else if (ac == 4)
		{
			*buf = '\0';
			ircsnprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
			sprintf(expires, "%ld", atoi(av[2]) + (long int) time(NULL));
			sql_do_server_bans_add(type, user, host, setby, buf, expires,
			                       av[3]);
		}
	}
	else
	{
		sql_do_server_bans_remove(type, user, host);
	}
	

	/*
	 * Free the data that was allocated
	 */
	free(user);
	free(host);
	free(address);
	free(setby);

	return;
}

/*************************************************************************/

/**
 * Parse SQLINE message for SQL
 *
 * @param mask The SQLINE mask that has been banned
 * @param reason The reason for the SQLINE ban
 *
 * @return void - no returend value
 */
void sql_do_sqline(char *mask, char *reason)
{
	SQLres *sql_res;
	char *sqlreason;
	char *sqlmask;

	

	/* Do not execute if
	 * SQL is disabled
	 * LargeNet is enabled
	 * Mask as not passed as a varaible
	 * Reason was not passed as a variable
	 */
	if (!denora->do_sql || LargeNet || BadPtr(mask) || BadPtr(reason))
	{
		return;
	}

	

	sqlmask = sql_escape(mask);

	sql_query("SELECT mask FROM %s WHERE mask = \'%s\' LIMIT 1",
	 SqlineTable, sqlmask);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		sqlreason = sql_escape(reason);
		if (sql_num_rows(sql_res) == 0)
		{
			sql_query("INSERT INTO %s (mask, reason) values('%s', '%s')", SqlineTable, sqlmask, sqlreason);
		}
		else
		{
			sql_query("UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'", SqlineTable, sqlreason, sqlmask);
		}
		sql_free_result(sql_res);
		free(sqlreason);
	}

	free(sqlmask);

	return;
}

/*************************************************************************/

/**
 * Parse SGLINE message for SQL
 * Note that the length should indicated where to split the mask into two pieces
 *
 * @param length number of characters that will indicate the split point in mask and reason
 * @param mask this holds both the mask and reason separated by a :
 * @return void - no returend value
 *
 */
void sql_do_sgline(char *length, char *mask)
{
	long int len;               /* length when converted to integer */
	char *reason;               /* reason for sgline                */
	char *sqlmask;
	int errsave;
	SQLres *sql_res;
	char *sqlreason;

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 * 3. Lenght was not passed
	 * 4. Mask was not passed
	 */
	if (!denora->do_sql || LargeNet || !length || !mask)
	{
		return;
	}

	errsave = errno;            /* temp copy the variable so we can reset the error state */
	errno = 0;                  /* make the errno 0 so we don't report wrong errors */

	len = strtol(length, NULL, 10);
	/*
	 * Check error state, to see if we were overflowed
	 */
	if (errno == ERANGE)
	{
		alog(LOG_DEBUG, "%s strtol() set errno to ERANGE possible %s",
		     PRETTY_FUNCTION,
		     (len == LONG_MAX ? "overflow" : "underflow"));
	}
	errno = errsave;

	

	if (((int) strlen(mask) > len) && (mask[len]) == ':')
	{
		mask[len] = '\0';
		reason = mask + len + 1;
	}
	else
	{
		return;
	}

	sqlmask = sql_escape(mask);

	sql_query("SELECT mask FROM %s WHERE mask = \'%s\' LIMIT 1",
	 SglineTable, sqlmask);

	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		sqlreason = sql_escape(reason);
		if (sql_num_rows(sql_res) == 0)
		{
			sql_query("INSERT INTO %s (mask, reason) values('%s', '%s')",
			          SglineTable, sqlmask, sqlreason);
		}
		else
		{
			sql_query("UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
			          SglineTable, sqlreason, sqlmask);
		}
		sql_free_result(sql_res);
		free(sqlreason);
	}

	free(reason);
	free(sqlmask);

	return;
}

/*************************************************************************/

/**
 * Parse XLINE message for SQL
 *
 * @param geos mask that will be banned by XLINE
 * @param reason is the reason for the XLINE
 * @return void - no returend value
 *
 */
void sql_do_xline(char *geos, char *reason)
{
	SQLres *sql_res;
	char *sqlreason;
	char *sqlgeos;
	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 */
	if (!denora->do_sql || LargeNet)
	{
		return;
	}

	

	sqlgeos = sql_escape(geos);

	sql_query("SELECT mask FROM %s WHERE mask = \'%s\' LIMIT 1",
	 SglineTable, sqlgeos);

	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		sqlreason = sql_escape(reason);
		if (sql_num_rows(sql_res) == 0)
		{
			sql_query("INSERT INTO %s (mask, reason) values('%s', '%s')",
			          SglineTable, sqlgeos, sqlreason);
		}
		else
		{
			sql_query("UPDATE %s SET reason=\'%s\' WHERE mask=\'%s\'",
			          SglineTable, sqlreason, sqlgeos);
		}
		sql_free_result(sql_res);
		free(sqlreason);
	}

	free(sqlgeos);

	return;
}

/*************************************************************************/

/**
 * Parse UNXLINE message for SQL
 * @param geos mask that will be removed by UNXLINE
 * @return void - no returend value
 */
void sql_do_unxline(char *geos)
{
	char *sqlgeos;
	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 * 3. geos was NULL
	 */
	if (!denora->do_sql || LargeNet || !geos)
	{
		return;
	}
	

	sqlgeos = sql_escape(geos);

	sql_query("DELETE FROM %s WHERE mask=\'%s\'",
		  SglineTable, sqlgeos);

	

	free(sqlgeos);

	return;
}

/*************************************************************************/

/**
 * Unreal passes in TKL the expires as the exact time of the expiration
 * for this reason and the fact they use "type" we do not add the expire + time
 * only on ircds where this is not done
 *
 * @param type If Unreal the type is Gline, Shun, etc..
 * @param user is the user ident that is banned
 * @param host is the user host that is banned
 * @param setby is the person or server that has set the ban
 * @param setat is the unix timestamp of when the ban was set
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param reason is the reason for the ban
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_add(char *type, char *user, char *host,
                            char *setby, char *setat, char *expires,
                            char *reason)
{
	SQLres *sql_res;
	int sqlid = -1;
	int checkdur = 0;
	int expire = 0;
	uint32 setattime = 0;
	char *sqlreason, *sqluser, *sqlhost;
	Gline *g = NULL;
	Qline *q = NULL;
	Zline *z = NULL;
	char **sql_row;

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 */
	if (!denora->do_sql || LargeNet)
	{
		return;
	}
	if (!stricmp(type, "G"))
	{
		g = findGline(host);
		if (g)
		{
			if (g->setat == (int32) strtoul(setat, NULL, 10)
			        && g->expires == (int32) strtoul(expires, NULL, 10))
			{
				return;
			}
		}
		else
		{
			new_Gline(user, host, setby, setat, expires, reason);
		}
	}

	if (!stricmp(type, "Q"))
	{
		q = findQline(host);
		if (q)
		{
			if (q->setat == (int32) strtoul(setat, NULL, 10)
			        && q->expires == (int32) strtoul(expires, NULL, 10))
			{
				return;
			}
		}
		else
		{
			new_Qline(user, host, setby, setat, expires, reason);
		}
	}

	if (!stricmp(type, "Z"))
	{
		z = findZline(host);
		if (z)
		{
			if (z->setat == (int32) strtoul(setat, NULL, 10)
			        && z->expires == (int32) strtoul(expires, NULL, 10))
			{
				return;
			}
		}
		else
		{
			new_Zline(user, host, setby, setat, expires, reason);
		}
	}


	sqlreason = sql_escape(reason);
	sqluser   = sql_escape(user);
	sqlhost   = sql_escape(host);

	if (type)
	{
		if ((g && g->sqlid) || (q && q->sqlid) || (z && z->sqlid))
		{
			if (g && g->sqlid)
			{
				sqlid = g->sqlid;
			}
			if (q && q->sqlid)
			{
				sqlid = q->sqlid;
			}
			if (z && z->sqlid)
			{
				sqlid = z->sqlid;
			}
			if (sqlid > 0)
			{
				sql_query("UPDATE %s SET setat=%ld, expires=%ld, reason=\'%s\' WHERE id = \'%d\'",
				         GlineTable, strtoul(setat, NULL, 10), strtoul(expires, NULL, 10), sqlreason, sqlid);
			}
		}
		else
		{
			sql_query("SELECT id FROM %s WHERE type = \'%s\' and user=\'%s\' and host=\'%s\' LIMIT 1",
			           GlineTable, type, sqluser, sqlhost);

			
			sql_res = sql_set_result(sqlcon);
			if (sql_res)
			{
				if (sql_num_rows(sql_res) == 0)
				{
					sql_query(
					 "INSERT INTO %s (type, user, host, setby, setat, expires, reason) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',%ld,%ld,\'%s\')",
					 GlineTable, type, sqluser, sqlhost, setby,
					 strtoul(setat, NULL, 10), strtoul(expires, NULL, 10), sqlreason);
				}
				else
				{
					sql_row = sql_fetch_row(sql_res);
					sqlid = strtol(sql_row[0], NULL, 10);
					if (sqlid > 0)
					{
						if (g)
						{
							g->sqlid = sqlid;
						}
						if (q)
						{
							q->sqlid = sqlid;
						}
						if (z)
						{
							z->sqlid = sqlid;
						}
						sql_query("UPDATE %s SET setat=%ld, expires=%ld, reason=\'%s\' WHERE id = \'%d\'",
						         GlineTable, strtoul(setat, NULL, 10), strtoul(expires, NULL, 10), sqlreason, sqlid);
					}
				}
				sql_free_result(sql_res);
			}
		}
	}
	else
	{
		

		checkdur = strtoul(expires, NULL, 10);
		setattime = strtoul(setat, NULL, 10);
		if (checkdur != 0)
		{
			expire = checkdur + setattime;
		}
		else
		{
			expire = checkdur;
		}

		sql_query("SELECT id FROM %s WHERE user=\'%s\' and host=\'%s\' LIMIT 1",
		          GlineTable, sqluser, sqlhost);

		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res) == 0)
			{
				sql_query
				("INSERT INTO %s (user, host, setby, setat, expires, reason) VALUES(\'%s\',\'%s\',\'%s\',%ld,%ld,\'%s\')",
				 GlineTable, sqluser, sqlhost, setby, setattime, expire, sqlreason);
			}
			else
			{
				sql_query
				(
				 "UPDATE %s SET setat=%ld, expires=%ld, reason=\'%s\' WHERE user=\'%s\' and host=\'%s\'",
				 GlineTable, setattime, expire, sqlreason, sqluser, sqlhost);
			}
			sql_free_result(sql_res);
		}
	}
	

	free(sqlreason);
	free(sqluser);
	free(sqlhost);
	return;
}

/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages into SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param setby is the person or server that has set the ban
 * @param expires is the unix timestamp of when the ban should expire or might be 0 for no expire
 * @param setat is the unix timestamp of when the ban was set
 * @param duration is the number of seconds the ban will last
 * @param reason is the reason for the ban
 * @param regex is the regular expression for the spam filter
 *
 * @return void - no returend value
 */
void sql_do_server_spam_add(char *target, char *action,
                            char *setby, char *expires, char *setat,
                            char *duration, char *reason, char *regex)
{
	SQLres *sql_res;
	int sqlid;
	SpamFilter *sf;
	char *sqlaction, *sqlreason, *sqlregex;
	char **sql_row;
	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 */
	if (!denora->do_sql || LargeNet)
	{
		return;
	}
	sf = findSpamFilter(regex);
	if (sf)
	{
		if (sf->duration == (int32) strtoul(duration, NULL, 10)
		        && sf->setat == (int32) strtoul(setat, NULL, 10))
		{
			return;
		}
	}
	else
	{
		new_SpamFilter(target, action, setby, expires, setat, duration,
		               reason, regex);
	}

	

	sqlaction = sql_escape(action);
	sqlreason = sql_escape(reason);

	if (sf && sf->sqlid > 0)
	{
		sql_query(
			 "UPDATE %s SET target=\'%s\', action=\'%s\', setby=\'%s\', expires=%ld, setat=%ld, duration=%ld, reason=\'%s\' WHERE id=\'%d\'",
			 SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10),
			 strtoul(setat, NULL, 10), strtoul(duration, NULL, 10),
			 sqlreason, sf->sqlid);

		free(sqlaction);
		free(sqlreason);
		return;
	}

	

	sqlregex = sql_escape(regex);

	sql_query("SELECT id FROM %s WHERE regex = \'%s\' LIMIT 1",
	          SpamTable, sqlregex);

	

	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) == 0)
		{
			sql_query("INSERT INTO %s (target, action, setby, expires, setat, duration, reason, regex) VALUES(\'%s\',\'%s\',\'%s\',%ld, %ld,%ld, \'%s\', \'%s\')",
			          SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10), 
			          strtoul(duration, NULL, 10), sqlreason, sqlregex);
		}
		else
		{
			sql_row = sql_fetch_row(sql_res);
			sqlid = strtol(sql_row[0], NULL, 10);
			if (sqlid > 0)
			{
				if (sf)
				{
					sf->sqlid = sqlid;
				}
				sql_query("UPDATE %s SET target=\'%s\', action=\'%s\', setby=\'%s\', expires=%ld, setat=%ld, duration=%ld, reason=\'%s\' WHERE id=\'%d\'",
				          SpamTable, target, sqlaction, setby, strtoul(expires, NULL, 10), strtoul(setat, NULL, 10),
				          strtoul(duration, NULL, 10), sqlreason, sqlid);
			}
		}
		sql_free_result(sql_res);
	}

	

	free(sqlreason);
	free(sqlaction);
	free(sqlregex);

	return;
}

/*************************************************************************/

/**
 * Parse GLINE messages and remove from SQL
 *
 * @param type If Unreal the type is Gline, Shun, etc.. this is optional
 * @param user is the user ident that is being unbanned
 * @param host is the user host that is being unbanned
 *
 * @return void - no returend value
 *
 */
void sql_do_server_bans_remove(char *type, char *user, char *host)
{
	char *sqluser, *sqlhost;
	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 */
	if (!denora->do_sql || LargeNet)
	{
		return;
	}

	

	sqluser = sql_escape(user);
	sqlhost = sql_escape(host);

	if (type)
	{
		sql_query(
		          "DELETE FROM %s WHERE type=\'%s\' and user=\'%s\' and host=\'%s\'",
		          GlineTable, type, sqluser, sqlhost);
	}
	else
	{
		sql_query("DELETE FROM %s WHERE user=\'%s\' and host=\'%s\'",
		          GlineTable, sqluser, sqlhost);
	}
	

	free(sqluser);
	free(sqlhost);

	return;
}

/*************************************************************************/

/**
 * Parse Unreal Spamfilter messages remove from SQL
 *
 * @param target If Unreal the spamfilter target type
 * @param action is flag value as to what to do if spamfilter is tripped
 * @param regex is the regular expression that made up the ban
 *
 * @return void - no returend value
 */
void sql_do_server_spam_remove(char *target, char *action, char *regex)
{
	char *sqltarget, *sqlaction, *sqlregex;
	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 * 3. Regex was NULL
	 * 4. Action was NULL
	 * 5. Target was NULL
	 */
	if (!denora->do_sql || LargeNet || !regex || !action || !target)
	{
		return;
	}

	

	sqltarget = sql_escape(target);
	sqlaction = sql_escape(action);
	sqlregex = sql_escape(regex);

	/*
	 * Query the spam table and remove bans that match the following
	 * 1. Target
	 * 2. Action text
	 * 3. Regex
	 */
	sql_query("DELETE FROM %s WHERE target=\'%s\' and action=\'%s\' and regex=\'%s'",
	          SpamTable, sqltarget, sqlaction, sqlregex);

	

	free(sqltarget);
	free(sqlaction);
	free(sqlregex);

	return;
}

/*************************************************************************/

/**
 * Clean up the GlineTable and SpamTable since some ircds assume we should
 * remove the bans from ourselves without being told.
 *
 * @param name containing when the event was called
 * @return MOD_CONT to indicate event is done
 *
 */
int sql_ban_clean(const char *name)
{
	/*
	 * Prevent compiler warnings that this variable is not used
	 */
	USE_VAR(name);

	

	/*
	 * Do not execute if
	 * 1. SQL code has been disabled
	 * 2. LargeNet is enabled
	 */
	if (!denora->do_sql || LargeNet)
	{
		return MOD_CONT;
	}

	

	/*
	 * Query the gline table and remove bans that the expiration is
	 * 1. Not 0 (ie.. no expire)
	 * 2. Less then the current time
	 */
	sql_query(
	          "DELETE FROM %s WHERE expires !=0 AND expires < %ld",
	          GlineTable, time(NULL));

	/*
	 * Check if the ircd supports spamfilter and if so we should clean that
	 * up as well
	 */
	

	if (ircd->spamfilter)
	{
		/*
		 * Query the spamfilter table and remove bans that the expiration is
		 * 1. Not 0 (ie.. no expire)
		 * 2. Less then the current time
		 */
		sql_query(
		          "DELETE FROM %s WHERE expires != 0 AND expires < %ld",
		          SpamTable, time(NULL));
	}
	

	/*
	 * return MOD_CONT when we are all done.
	 */
	return MOD_CONT;
}

/*************************************************************************/
