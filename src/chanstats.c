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

static void make_stats(User * u, char *receiver, char *msg);
static uint32 countsmileys(char *text);
static int check_db(User * u, Channel * c);
static char *newsplit(char **rest);
list_t *CStatshead;

int CompareCStats(const void *v, const void *cc)
{
	const ChannelStats *cs = (void *) v;
	return (stricmp(cs->name, (char *) cc));
}

/*************************************************************************/

void InitCStatsList(void)
{
	ChannelStats *cs;
	

	CStatshead = list_create(-1);
	cs = malloc(sizeof(ChannelStats));
	bzero(cs, sizeof(ChannelStats));
}

/*************************************************************************/

/**
 * Locate ChannelStats struct by the mask
 *
 * @param mask is the ChannelStats to locate
 * @return ChannelStats stats struct
 *
 */
ChannelStats *find_cs(char *mask)
{
	lnode_t *tn;
	ChannelStats *c = NULL;

	tn = list_find(CStatshead, mask, CompareCStats);
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

ChannelStats *makecs(char *mask)
{
	lnode_t *tn;
	ChannelStats *c = NULL;
	

	tn = list_find(CStatshead, mask, CompareCStats);
	if (tn)
	{
		c = lnode_get(tn);
	}
	else
	{
		c = malloc(sizeof(ChannelStats));
		c->name = sstrdup(mask);
		c->flags = 0;
		c->timeadded = 0;
		tn = lnode_create(c);
		list_append(CStatshead, tn);
	}
	return c;
}

/*************************************************************************/

int del_cs(ChannelStats * cs)
{
	lnode_t *tn;
	ChannelStats *c = NULL;
	

	if (cs)
	{
		tn = list_find(CStatshead, cs->name, CompareCStats);
		if (tn)
		{
			c = lnode_get(tn);
			lnode_destroy(ListDelete(CStatshead, tn));
			free(c);
		}
	}
	return 0;
}

/*************************************************************************/

void Fini_ChannelStats(void)
{
	ChannelStats *t;
	lnode_t *tn;

	tn = list_first(CStatshead);
	while (tn != NULL)
	{
		t = lnode_get(tn);
		free(t);
		tn = list_next(CStatshead, tn);
	}
	list_destroy_nodes(CStatshead);
}

/*************************************************************************/

/**
 * Load the ChannelStats database from disk
 *
 * @return void - no returend value
 *
 */
void load_cs_db(void)
{
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(ChannelStatsDB, CS_VERSION, &key, &value);
	ChannelStats *cs = NULL;
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
			/* a ctcp entry has completely been read. put any checks in here! */
		}
		else
		{
			/* DB_READ_SUCCESS */

			if (!*value || !*key)
				continue;

			if (!stricmp(key, "name"))
			{
				cs = makecs(value);
			}
			else if (!stricmp(key, "flags"))
			{
				cs->flags = atoi(value);
			}
			else if (!stricmp(key, "timeadded"))
			{
				cs->timeadded = atoi(value);
			}
		}                       /* else */
	}                           /* while */
}

/*************************************************************************/

void save_cs_db(void)
{
	DenoraDBFile *dbptr = filedb_create(ChannelStatsDB, CS_VERSION);
	ChannelStats *cs;
	lnode_t *tn;

	tn = list_first(CStatshead);
	while (tn != NULL)
	{
		cs = lnode_get(tn);
		new_write_db_entry("name", dbptr, "%s", cs->name);
		new_write_db_entry("flags", dbptr, "%d", cs->flags);
		new_write_db_entry("timeadded", dbptr, "%ld", cs->timeadded);
		new_write_db_endofblock(dbptr);
		tn = list_next(CStatshead, tn);
	}

	

	filedb_close(dbptr, NULL, NULL);  /* close file */
}

/*****************************************************************************************/

void do_cstats(User * u, char *receiver, char *msg)
{
	char *command, *buf = NULL;
	ChannelStats *cs;

	

	send_event(EVENT_CHAN_PRIVMSG, 3, u->nick, receiver, msg);

	if (!(cs = find_cs(receiver)))
	{
		/* no error message if you place statserv in the logchan which is the same as
		   services logchan it will throw fatal errors */
		return;
	}

	if (cs && cs->flags & CS_FANTASY)
	{
		buf = sstrdup(msg);
		command = strtok(buf, " ");
		

		if (command && (*command == *ChanStatsTrigger))
		{
			char *params = strtok(NULL, "");
			/* Strip off the fantasy character */
			command++;
			if (params)
			{
				send_event(EVENT_FANTASY, 4, command, u->nick,
				           receiver, params);
			}
			else
			{
				send_event(EVENT_FANTASY, 3, command, u->nick, receiver);
			}
		}
	}
	
	make_stats(u, receiver, msg);
	if (buf)
		free(buf);
}

/******************************************************************************************/

static void make_stats(User * u, char *receiver, char *msg)
{
	uint32 letters = 0, words = 1, action = 0, smileys = 0, i = 0;
	long int hour;
	char *buf;
	Channel *c;

	

	if (!denora->do_sql)
	{
		return;
	}

	if (!u)
	{
		return;
	}

	if (!stricmp(u->nick, s_StatServ))
	{
		return;
	}

	c = findchan(receiver);
	check_db(u, c);
	
	buf = normalizeBuffer(msg); /* remove control letters from message */
	if (BadPtr(buf))
	{
		return;
	}
	letters = strlen(buf);      /* count letters */
	for (i = 0; i < letters; i++)       /* count words */
	{
		if ((buf[i] == ' ') && (buf[i + 1] != ' '))
			words++;
	}
	smileys = countsmileys(buf);        /* count smileys */
	free(buf);
	/* is a action ? */
	if (strncmp("\01ACTION", msg, 7) == 0)      /* is a action ? */
	{
		action = 1;
		letters = letters - 7;  /* remove from the counted letters */
		words--;                /* remove from the counted words */
	}
	
	hour = get_hour();

	/* update user SQL */
	/* update user */
	if (u->cstats != 2)         /* check for ignore */
	{
		sql_query
		(
		 "UPDATE %s SET letters=letters+%u, words=words+%u, line=line+1, "
		 "actions=actions+%u, smileys=smileys+%u, lastspoke=%ld, time%ld=time%ld+1 "
		 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
		 UStatsTable, letters, words, action, smileys,
		 (long int) time(NULL), hour, hour, u->sgroup, c->sqlchan);
	}
	

	/* update chan */
	sql_query
	(
	 "UPDATE %s SET letters=letters+%u, words=words+%u, line=line+1, "
	 "actions=actions+%u, smileys=smileys+%u, lastspoke=%ld, time%ld=time%ld+1 WHERE chan=\'%s\';",
	 CStatsTable, letters, words, action, smileys,
	 (long int) time(NULL), hour, hour, c->sqlchan);
}

/*************************************************************************/

void count_kicks(User * kicker, User * kicked, Channel * c)
{
	if (!c)
	{
		return;
	}
	if (!denora->do_sql)
	{
		return;
	}

	if (!find_cs(c->name))
	{
		return;
	}

	

	if (kicker && !is_excluded_user(kicker))
	{
		check_db(kicker, c);
		if (kicker->cstats != 2)        /* check for ignore */
		{
			sql_query
			("UPDATE %s SET kicks=kicks+1 "
			 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
			 UStatsTable, kicker->sgroup, c->sqlchan);
			sql_query
			(
			 "UPDATE %s SET kicks=kicks+1 WHERE chan=\'%s\';",
			 CStatsTable, c->sqlchan);
		}
	}

	

	if (kicked && !is_excluded_user(kicked))
	{
		check_db(kicked, c);
		if (kicked->cstats != 2)        /* check for ignore */
		{
			sql_query
			("UPDATE %s SET kicked=kicked+1 "
			 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
			 UStatsTable, kicked->sgroup, c->sqlchan);
		}
	}
}

/*************************************************************************/

void count_topics(User * u, Channel * c)
{
	if (!denora->do_sql)
	{
		return;
	}

	if (!find_cs(c->name))
	{
		return;
	}
	

	/*
	 * need to exclude them from being counted if it is from a user whom is to be excluded
	 */
	if (is_excluded_user(u))
	{
		return;
	}

	check_db(u, c);

	

	if (u->cstats != 2)         /* check for ignore */
	{
		sql_query
		("UPDATE %s SET topics=topics+1 "
		 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
		 UStatsTable, u->sgroup, c->sqlchan);
	}
	sql_query
	("UPDATE %s SET topics=topics+1 WHERE chan=\'%s\';",
	 CStatsTable, c->sqlchan);
	
}

/*************************************************************************/

void count_modes(User * u, Channel * c)
{
	if (!denora->do_sql)
	{
		return;
	}

	if (!find_cs(c->name))
	{
		return;
	}
	

	/*
	 * need to exclude them from being counted if it is from a user whom is to be excluded
	 */
	if (is_excluded_user(u))
	{
		return;
	}

	check_db(u, c);
	
	if (u->cstats != 2)         /* check for ignore */
	{
		sql_query
		("UPDATE %s SET modes=modes+1 "
		 "WHERE (uname=\'%s\' AND (chan=\'global\' OR chan=\'%s\'));",
		 UStatsTable, u->sgroup, c->sqlchan);
	}
	sql_query
	("UPDATE %s SET modes=modes+1 WHERE chan=\'%s\';",
	 CStatsTable, c->sqlchan);
	
}

/*************************************************************************/

static int check_db(User * u, Channel * c)
{

	int i, excluded;
	SQLres *sql_res;
	char **sql_row;
	

	/* Check if user has +r and/or +B */
	excluded = 0;
	if (UserStatsExcludeBots && UserHasMode(u->nick, UMODE_B))
	{
		/* User is a bot so he gets ignored */
		u->cstats = 2;
		excluded = 1;
	}
	if (UserStatsRegistered && !excluded)
	{
		if (!UserHasMode(u->nick, UMODE_r) && !u->account)
		{
			/* User has no +r nor account so he gets ignored */
			u->cstats = 2;
		}
		else if ((UserHasMode(u->nick, UMODE_r) || u->account) && (u->cstats == 2))
		{
			/* User is registered but is set to ignore, so reset, since he might
			 * just have identified */
			u->cstats = 0;
		}
	}

	/* get alias from db */
	if (u->cstats == 0)
	{
		sql_query(
		          "SELECT `uname`,`ignore` FROM %s WHERE uname=\'%s\';",
		          AliasesTable, u->sqlnick);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res) > 0)          /* exists this nick already in the database? */
			{
				sql_row = sql_fetch_row(sql_res);
				if (u->sgroup)
				{
					free(u->sgroup);
				}
				u->sgroup = sql_escape(sql_row[0]);
				if (!stricmp(sql_row[1], "Y"))
				{
					u->cstats = 2;
				}
				else
				{
					u->cstats = 1;
				}
			}
			else if (u->sgroup && u->lastuname && stricmp(u->sgroup, u->lastuname))
			{
				/* user already had a stats user, so we'll take that one */
				alog(LOG_DEBUG, "Replacing sgroup %s with last used %s",
				     u->sgroup, u->lastuname);
				free(u->sgroup);
				u->sgroup = sstrdup(u->lastuname);
				free(u->lastuname);
				u->lastuname = NULL;
			}
			else                /* num_rows = 0 */
			{
				/* create alias and global */
				sql_query(
				          "INSERT INTO %s (nick, uname) VALUES (\'%s\', \'%s\') ON DUPLICATE KEY UPDATE uname=\'%s\'",
				          AliasesTable, u->sqlnick, u->sqlnick,
				          u->sqlnick);
				for (i = 0; i < 4; i++)
				{
					sql_query("INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'global\', type=%i, firstadded=%ld;",
					 UStatsTable, u->sqlnick, i,
					 (long int) time(NULL));
				}
				u->cstats = 1;
				if (u->sgroup)
					free(u->sgroup);
				u->sgroup = sstrdup(u->sqlnick);
			}
			sql_free_result(sql_res);
		}
	}

	
	if (c->cstats == 0)
	{
		
		sql_query(
		          "SELECT chan FROM %s WHERE chan=\'%s\' AND type=0;",
		          CStatsTable, c->sqlchan);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res) > 0)          /* exists this chan already in the database? */
			{
				c->cstats = 1;
			}
			else                /* num_rows = 0 */
			{
				/* create total, daily, weekly, monthly */
				for (i = 0; i < 4; i++)
				{
					sql_query(
					          "INSERT INTO %s SET chan=\'%s\', type=%d;",
					          CStatsTable, c->sqlchan, i);
				}
				c->cstats = 1;
			}                   /* num_rows */
			sql_free_result(sql_res);
		}
	}                           /* c->cstats */
	
	if (u->cstats != 2)
	{
		if (u->lastuname && u->sgroup && stricmp(u->sgroup, u->lastuname))
		{
			alog(LOG_DEBUG,
			     "sgroup %s and lastuname %s differ, so we merge them",
			     u->sgroup, u->lastuname);
			sumuser(u, u->lastuname, u->sgroup);
			free(u->lastuname);
			u->lastuname = NULL;
			return 1;
		}
		sql_query
		(
		 "SELECT `uname` FROM %s WHERE `uname`=\'%s\' AND `chan`=\'%s\';",
		 UStatsTable, u->sgroup, c->sqlchan);
		sql_res = sql_set_result(sqlcon);
		if (sql_res)
		{
			if (sql_num_rows(sql_res) == 0)
			{
				/* create entries for total, daily, weekly, monthly, */
				for (i = 0; i < 4; i++)
				{
					sql_query
					(
					 "INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'global\', type=%i, firstadded=%ld;",
					 UStatsTable, u->sgroup, i, (long int) time(NULL));
					sql_query
					(
					 "INSERT IGNORE INTO %s SET uname=\'%s\', chan=\'%s\', type=%i, firstadded=%ld;",
					 UStatsTable, u->sgroup, c->sqlchan, i,
					 (long int) time(NULL));
				}
			}
			sql_free_result(sql_res);
		}
	}
	return 1;
}

/*************************************************************************/

/* transfer all stats from user2 to user1 and delete user2 */
void sumuser(User * u, char *user1, char *user2)
{
	User *u2;
	int i;
	SQLres *sql_res_chans;
	SQLres *sql_res;
	char **sql_row;
	char **sql_row_chans;
	char *chan_;
	char *user1_, *user2_;

	if (!denora->do_sql || !strcmp(user1, user2))
	{
		return;
	}

	user1_ = sql_escape(user1);
	user2_ = sql_escape(user2);
	

	sql_query("SELECT uname FROM %s WHERE uname = \'%s\';",
	 UStatsTable, user1_);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) == 0)     /* exists this nick already in the database? */
		{
			sql_free_result(sql_res);
			notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user1);
			goto end;
		}
		sql_free_result(sql_res);
	}
	sql_query("SELECT uname FROM %s WHERE uname = \'%s\';",
	 UStatsTable, user2_);
	sql_res = sql_set_result(sqlcon);
	if (sql_num_rows(sql_res) == 0)         /* exists this nick already in the database? */
	{
		sql_free_result(sql_res);
		notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user2);
		goto end;
	}
	sql_free_result(sql_res);
	/* get all chans of user2 */
	sql_query("SELECT chan FROM %s WHERE uname = \'%s\' AND type = 0;",
	          UStatsTable, user2_);
	sql_res_chans = sql_set_result(sqlcon);
	/* for each chan: */
	/* read total, daily, weekly and  monthly stats and add them to user1 */
	while ((sql_row_chans = sql_fetch_row(sql_res_chans)) != NULL)
	{
		chan_ = sql_escape(sql_row_chans[0]);
		/* read total, daily, weekly and  monthly stats and add them to user1 */
		for (i = 0; i < 4; i++)
		{
			sql_query
			(
			 "SELECT letters, words,line,actions,smileys,kicks,kicked,modes,topics,wasted,time0,time1,time2,time3,time4,time5,time6,time7,time8,time9,time10,time11,time12,time13,time14,time15,time16,time17,time18,time19,time20,time21,time22,time23 FROM %s WHERE uname = \'%s\' AND chan = \'%s\' AND type = %i;",
			 UStatsTable, user2_, chan_, i);
			sql_res = sql_set_result(sqlcon);
			sql_row = sql_fetch_row(sql_res);
			if (sql_row != NULL)
			{
				sql_query("UPDATE %s SET letters=letters+%s, words=words+%s, line=line+%s, actions=actions+%s, smileys=smileys+%s, kicks=kicks+%s, kicked=kicked+%s, modes=modes+%s, topics=topics+%s, wasted=wasted+%s WHERE uname=\'%s\' AND chan=\'%s\' AND type=%i;", UStatsTable, sql_row[0],     /* letters */
				          sql_row[1],     /* words */
				          sql_row[2],     /* lines */
				          sql_row[3],     /* actions */
				          sql_row[4],     /* smileys */
				          sql_row[5],     /* kicks */
				          sql_row[6],     /* kicked */
				          sql_row[7],     /* modes */
				          sql_row[8],     /* topics */
				          sql_row[9],     /* wasted */
				          user1_, chan_, i);
				sql_query("UPDATE %s SET time0=time0+%s, time1=time1+%s, time2=time2+%s, time3=time3+%s, time4=time4+%s, time5=time5+%s, time6=time6+%s, time7=time7+%s, time8=time8+%s, time9=time9+%s, time10=time10+%s, time11=time11+%s, time12=time12+%s, time13=time13+%s, time14=time14+%s, time15=time15+%s, time16=time16+%s, time17=time17+%s, time18=time18+%s, time19=time19+%s, time20=time20+%s, time21=time21+%s, time22=time22+%s, time23=time23+%s WHERE uname=\'%s\' AND chan=\'%s\' AND type=%i;", UStatsTable, sql_row[10],        /* time0 */
				          sql_row[11],    /* time1 */
				          sql_row[12],    /* time2 */
				          sql_row[13],    /* time3 */
				          sql_row[14],    /* time4 */
				          sql_row[15],    /* time5 */
				          sql_row[16],    /* time6 */
				          sql_row[17],    /* time7 */
				          sql_row[18],    /* time8 */
				          sql_row[19],    /* time9 */
				          sql_row[20],    /* time10 */
				          sql_row[21],    /* time11 */
				          sql_row[22],    /* time12 */
				          sql_row[23],    /* time13 */
				          sql_row[24],    /* time14 */
				          sql_row[25],    /* time15 */
				          sql_row[26],    /* time16 */
				          sql_row[27],    /* time17 */
				          sql_row[28],    /* time18 */
				          sql_row[29],    /* time19 */
				          sql_row[30],    /* time20 */
				          sql_row[31],    /* time21 */
				          sql_row[32],    /* time22 */
				          sql_row[33],    /* time23 */
				          user1_, chan_, i);
			}
			else
			{
				alog(LOG_ERROR, "Query for user %s unexpectedly returned no results in sumuser", user2_);
			}
			sql_free_result(sql_res);
		}
		free(chan_);
	}                           /* while */

	sql_free_result(sql_res_chans);
	/* delete stats of user2 from ustats */
	sql_query("DELETE FROM %s WHERE uname = \'%s\';",
	          UStatsTable, user2_);
	/* change aliases */
	sql_query("UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
	          AliasesTable, user1_, user2_);
	/* check for ignore flag. if at least one is found, replicate */
	sql_query("SELECT `ignore` FROM %s WHERE `uname` = \'%s\' AND `ignore`=\'Y\';",
	          AliasesTable, user1_);
	sql_res = sql_set_result(sqlcon);
	if (sql_num_rows(sql_res) > 0)
	{
		sql_query("UPDATE `%s` SET `ignore` = \'Y\' WHERE `uname` = \'%s\';",
		          AliasesTable, user1_);
	}
	sql_free_result(sql_res);

	notice_lang(s_StatServ, u, STATS_CHANSTATS_SUMUSER_DONE, user2, user1);
	alog(LOG_NORMAL, "Merged stats user %s with %s", user2, user1);

	while ((u2 = finduser_by_sgroup(user2, user2_)))
	{
		if (u2->sgroup)
			free(u2->sgroup);
		u2->sgroup = sstrdup(user1_);
	}

end:
	
	free(user1_);
	free(user2_);
}

/*************************************************************************/

/* rename stats user1 to user2 */

void renameuser(User * u, char *user1, char *user2)
{
	User *u2;
	SQLres *sql_res;
	char *user1_, *user2_;

	if (!denora->do_sql)
	{
		return;
	}

	user1_ = sql_escape(user1);
	user2_ = sql_escape(user2);
	

	sql_query("SELECT uname FROM %s WHERE uname = \'%s\';",
	 UStatsTable, user1_);
	sql_res = sql_set_result(sqlcon);
	if (sql_res)
	{
		if (sql_num_rows(sql_res) == 0)     /* exists this nick already in the database? */
		{
			sql_free_result(sql_res);
			notice_lang(s_StatServ, u, STATS_SUMUSER_NOTFOUND, user1);
			goto end;
		}
		sql_free_result(sql_res);
	}
	sql_query("SELECT uname FROM %s WHERE uname = \'%s\';",
	 UStatsTable, user2_);
	sql_res = sql_set_result(sqlcon);
	if (sql_num_rows(sql_res) != 0)         /* exists this nick already in the database? */
	{
		sql_free_result(sql_res);
		notice_lang(s_StatServ, u, STATS_RENAMEUSER_EXISTING, user2);
		goto end;
	}
	sql_free_result(sql_res);

	/* renames user1 to user2 */
	sql_query(
	          "UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
	          UStatsTable, user2_, user1_);
	/* updates aliases */
	sql_query(
	          "UPDATE %s SET uname = \'%s\' WHERE uname = \'%s\';",
	          AliasesTable, user2_, user1_);
	while ((u2 = finduser_by_sgroup(user1, user1_)))
	{
		if (u2->sgroup)
			free(u2->sgroup);
		u2->sgroup = sstrdup(user2_);
	}
	notice_lang(s_StatServ, u, STATS_CHANSTATS_RENAMEUSER_DONE, user1,
	            user2);

end:
	free(user1_);
	free(user2_);
}

/*************************************************************************/

/* Find a user by nick.  Return NULL if user could not be found. */

User *finduser_by_sgroup(char *nick, char *sgroup)
{
	User *user;

	

	if (BadPtr(sgroup))
	{
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_BAD_FINDSGROUP));
		return NULL;
	}

	user = userlist[NICKHASH(nick)];
	while (user && user->sgroup && (stricmp(user->sgroup, sgroup) != 0))
		user = user->next;
	return user;
}

/*************************************************************************/

static uint32 countsmileys(char *text)
{
	char buf[1024], *pbuf, *smiley, *p;
	uint32 ismileys = 0;

	ircsnprintf(buf, sizeof(buf), "%s", Smiley);
	pbuf = buf;
	

	while (strlen(pbuf) > 0)
	{
		smiley = newsplit(&pbuf);
		p = strstr(text, smiley);
		while (p)
		{
			ismileys++;
			p += strlen(smiley);
			p = strstr(p, smiley);
		}
	}
	
	return ismileys;
}

/*************************************************************************/

static char *newsplit(char **rest)
{
	register char *o, *r;

	

	if (!rest)
		return *rest = NULL;
	o = *rest;
	while (*o == ' ')
		o++;
	r = o;
	while (*o && (*o != ' '))
		o++;
	if (*o)
		*o++ = 0;
	*rest = o;
	return r;
}

/*************************************************************************/

int get_hour(void)
{
	time_t now;
	static struct tm t;

	
	now = time(NULL);
#ifdef _WIN32
	localtime_s(&t, &now);
#else
	t = *localtime(&now);
#endif
	return t.tm_hour;
}

/*************************************************************************/
