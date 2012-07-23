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

Channel *chanlist[1024];
static Channel *current;
static int next_index;
int totalstatschannel;

list_t *StatsChanhead;

int CompareStatsChan(const void *v, const void *cc)
{
	const StatsChannel *cs = (void *) v;
	return (stricmp(cs->name, (char *) cc));
}

/*************************************************************************/

void load_chan_db(void)
{
	/* TODO: check if key,value needs free */
	char *key, *value;
	DenoraDBFile *dbptr = filedb_open(ChannelDB, CHAN_VERSION, &key, &value);
	StatsChannel *ss = NULL;
	int retval = 0;

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
			/* a channel has completely been read. put any checks in here! */
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
				ss = statschan_create(value);
				/* For backwards compatibility with older chan.db files */
				ss->secret = 1;
				ss->private = 1;
			}
			else if (!stricmp(key, "kickcnt"))
			{
				ss->kickcount = atoi(value);
			}
			else if (!stricmp(key, "joincnt"))
			{
				ss->joincounter = atoi(value);
			}
			else if (!stricmp(key, "topiccnt"))
			{
				ss->topic_count = atoi(value);
			}
			else if (!stricmp(key, "maxusercnt"))
			{
				ss->maxusercount = atoi(value);
			}
			else if (!stricmp(key, "maxusertime"))
			{
				ss->maxusertime = atoi(value);
			}
			else if (!stricmp(key, "partcount"))
			{
				ss->partcount = atoi(value);
			}
			else if (!stricmp(key, "partcounttime"))
			{
				ss->partcounttime = atoi(value);
			}
			else if (!stricmp(key, "kickcounttime"))
			{
				ss->kickcounttime = atoi(value);
			}
			else if (!stricmp(key, "joincounttime"))
			{
				ss->joincounttime = atoi(value);
			}
			else if (!stricmp(key, "topiccounttime"))
			{
				ss->topiccounttime = atoi(value);
			}
			else if (!stricmp(key, "modecount"))
			{
				ss->modecount = atoi(value);
			}
			else if (!stricmp(key, "modecounttime"))
			{
				ss->modecounttime = atoi(value);
			}
			else if (!stricmp(key, "secret"))
			{
				ss->secret = atoi(value);
			}
			else if (!stricmp(key, "private"))
			{
				ss->private = atoi(value);
			}
		}                       /* else */
	}                           /* while */
}

/*************************************************************************/

void save_chan_db(void)
{
	DenoraDBFile *dbptr = filedb_create(ChannelDB, CHAN_VERSION);
	StatsChannel *ss;
	lnode_t *tn;

	SET_SEGV_LOCATION();
	tn = list_first(StatsChanhead);
	while (tn != NULL)
	{
		ss = lnode_get(tn);
		if (ss->joincounttime > (time(NULL) - ClearChanInActive))
		{
			new_write_db_entry("name", dbptr, "%s", ss->name);
			new_write_db_entry("kickcnt", dbptr, "%d", ss->kickcount);
			new_write_db_entry("joincnt", dbptr, "%d", ss->joincounter);
			new_write_db_entry("topiccnt", dbptr, "%d", ss->topic_count);
			new_write_db_entry("maxusercnt", dbptr, "%d",
			                   ss->maxusercount);
			new_write_db_entry("maxusertime", dbptr, "%ld",
			                   (long int) ss->maxusertime);
			new_write_db_entry("partcount", dbptr, "%d", ss->partcount);
			new_write_db_entry("partcounttime", dbptr, "%ld",
			                   (long int) ss->partcounttime);

			new_write_db_entry("kickcounttime", dbptr, "%ld",
			                   (long int) ss->kickcounttime);

			new_write_db_entry("joincounttime", dbptr, "%ld",
			                   (long int) ss->joincounttime);

			new_write_db_entry("topiccounttime", dbptr, "%ld",
			                   (long int) ss->topiccounttime);

			new_write_db_entry("modecount", dbptr, "%d", ss->modecount);
			new_write_db_entry("modecounttime", dbptr, "%ld",
			                   (long int) ss->modecounttime);
			new_write_db_entry("secret", dbptr, "%i", ss->secret);
			new_write_db_entry("private", dbptr, "%i", ss->private);
			new_write_db_endofblock(dbptr);
		}
		tn = list_next(StatsChanhead, tn);
	}
	SET_SEGV_LOCATION();
	filedb_close(dbptr, NULL, NULL);  /* close file */
}

/*************************************************************************/

void InitStatsChanList(void)
{
	StatsChannel *cs;
	SET_SEGV_LOCATION();

	StatsChanhead = list_create(-1);
	cs = malloc(sizeof(StatsChannel));
	bzero(cs, sizeof(StatsChannel));
}

/*************************************************************************/

/* PART */
void sql_do_part(char *chan, User * u)
{
	char *sqlchan;
	int chanid, nickid;

	SET_SEGV_LOCATION();
	sqlchan = rdb_escape(chan);
	chanid = db_getchannel(sqlchan);
	nickid = db_getnick(u->sqlnick);

	if (chanid || nickid)
	{
		rdb_query(QUERY_LOW,
		          "DELETE FROM %s WHERE nickid=%d AND chanid=%d",
		          IsOnTable, nickid, chanid);
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET currentusers=currentusers-1 WHERE chanid=%d",
		          ChanTable, chanid);
		if (!ChanHasMode(chan, ircd->persist_char))
			db_checkemptychan(chanid);
	}
	SET_SEGV_LOCATION();
	free(sqlchan);
}

/*************************************************************************/

/**
 * SQL handler for when a user joins channel 0, and parts all channels
 *
 * @param nick is the nick that part the channels
 *
 * @return void - no returend value
 *
 */
void sql_do_partall(char *nick)
{
	char *sqlnick;

	if (!BadPtr(nick))
	{
		sqlnick = rdb_escape(nick);
		SET_SEGV_LOCATION();
		db_removefromchans(db_getnick(sqlnick));
		free(sqlnick);
	}
}

/*************************************************************************/

/**
 * SQL handler for when a user joins a channel
 *
 * @param chan is the channel the user joined
 * @param nick is the nick that joined
 *
 * @return void - no returend value
 *
 */
void sql_do_join(char *chan, char *nick)
{
	int chanid;

	if (!denora->do_sql || BadPtr(chan) || BadPtr(nick))
	{
		return;
	}
	SET_SEGV_LOCATION();
	chanid = db_getchancreate(chan);
	sql_do_addusers(chanid, nick);
}

/*************************************************************************/

/* add one or more users to a chanid */
void sql_do_addusers(int chanid, char *users)
{
	int op, voice, halfop, owner, protect, shunned;
	char *nextusers;
	char flagbuf[BUFSIZE];
	char valuebuf[BUFSIZE];
	int nickid, status_flag;
	User *u;

	SET_SEGV_LOCATION();

	while (users && (*users))
	{
		/* Neostats is broken and sends an additional space. */
		if (*users == ' ')
			users++;

		/* Unreal uses SJOIN to send bans and exempts. Just ignore them. */
		if ((*users == ircd->sjoinbanchar) || (*users == ircd->sjoinexchar)
		        || (*users == ircd->sjoinivchar))
		{
			nextusers = strchr(users, ' ');
			if (nextusers)
				*nextusers = '\0';
			users = nextusers;
			if (users)
				users++;
			continue;
		}
		op = 0;
		halfop = 0;
		voice = 0;
		owner = 0;
		protect = 0;
		shunned = 0;

		SET_SEGV_LOCATION();

		while ((int) *users >= 0 && sjoinmodes[(int) *users] != 0)
		{
			status_flag = sjoinmodes[(int) *users++];
			if (status_flag == STATUS_OP)
			{
				op++;
			}
			if (status_flag == STATUS_HALFOP)
			{
				halfop++;
			}
			if (status_flag == STATUS_VOICE)
			{
				voice++;
			}
			if (status_flag == STATUS_OWNER)
			{
				owner++;
			}
			if (status_flag == STATUS_PROTECTED)
			{
				protect++;
			}
			if (status_flag == STATUS_SHUNNED)
			{
				shunned++;
			}
		}

		nextusers = strchr(users, ' ');
		if (nextusers)
		{
			*nextusers = '\0';
		}
		u = user_find(users);
		users = rdb_escape((u ? u->nick : users));
		nickid = db_checknick((u ? u->nick : users));
		if (nickid != -1)
		{
			/* Build the query dynamically */
			/* First, setup the head and tail for the basic query */
			ircsnprintf(flagbuf, sizeof(flagbuf),
			            "INSERT IGNORE INTO %s (nickid, chanid, mode_lo, mode_lv",
			            IsOnTable);
			ircsnprintf(valuebuf, sizeof(valuebuf),
			            ") VALUES (%d, %d, \'%s\', \'%s\'", nickid, chanid,
			            (op ? "Y" : "N"), (voice ? "Y" : "N"));

			/* Next, check if an ircd supports a certain mode and add */
			if (ircd->owner)
			{
				strlcat(flagbuf, ", mode_lq", sizeof(flagbuf));
				strlcat(valuebuf, (owner ? ",\'Y\'" : ",\'N\'"),
				        sizeof(valuebuf));
			}
			if (ircd->protect)
			{
				strlcat(flagbuf, ", mode_la", sizeof(flagbuf));
				strlcat(valuebuf, (protect ? ",\'Y\'" : ",\'N\'"),
				        sizeof(valuebuf));
			}
			if (ircd->halfop)
			{
				strlcat(flagbuf, ", mode_lh", sizeof(flagbuf));
				strlcat(valuebuf, (halfop ? ",\'Y\'" : ",\'N\'"),
				        sizeof(valuebuf));
			}
			if (ircd->gagged)
			{
				strlcat(flagbuf, ", mode_lg", sizeof(flagbuf));
				strlcat(valuebuf, (shunned ? ",\'Y\'" : ",\'N\'"),
				        sizeof(valuebuf));
			}

			/* Close the querystring */
			strlcat(valuebuf, ")", sizeof(valuebuf));

			/* Put it together */
			strlcat(flagbuf, valuebuf, sizeof(valuebuf));

			/* Execute */
			rdb_query(QUERY_HIGH, flagbuf);
		}
		else
		{
			alog(LOG_NONEXISTANT,
			     langstr(ALOG_DEBUG_NONEXISTANT_USER_JOIN),
			     (u ? u->nick : users), chanid);
		}
		free(users);
		users = nextusers;
		if (users)
		{
			users++;
		}
	}
}

/*************************************************************************/

/**
 * SQL handler for SJOIN messages
 *
 * @param chan is the channel the user joined
 * @param users is the string of nicks that joined
 * @param modes is the channel user modes
 * @param nmodes is if the modes were passed
 *
 * @return void - no returend value
 *
 */
void sql_do_sjoin(char *chan, char *users, char **modes, int nbmodes)
{
	int chanid;

	if (!chan || !users)
	{
		return;
	}

	SET_SEGV_LOCATION();
	chanid = db_getchancreate(chan);
	sql_do_addusers(chanid, users);
	if (nbmodes)
	{
		sql_do_chanmodes(chan, nbmodes, modes);
	}
}

/*************************************************************************/

void chan_deluser(User * user, Channel * c)
{
	struct c_userlist *u;
	ChannelStats *cs;

	SET_SEGV_LOCATION();

	if (!c->users)
	{
		/* TODO: Fix this corruption */
		alog(LOG_DEBUG, "dbg: Channel %s seems to have no users.",c->name);
		return;
	}

        u = c->users;

	if (u && u->user != user)
	{
        	while ((u = u->next))
        	{
                	if (u->user == user)
				break;
		}
        }

	if (!u)
	{
		return;
	}

	if (u->next)
	{
		u->next->prev = u->prev;
	}
	if (u->prev)
	{
		u->prev->next = u->next;
	}
	else
	{
		c->users = u->next;
	}
	SET_SEGV_LOCATION();
	free(u);
	c->stats->usercount--;
	SET_SEGV_LOCATION();

	cs = find_cs(c->name);

	if (c->stats->usercount == 1 && cs && PartOnEmpty && !denora->qmsg)
	{
		denora_cmd_part(s_StatServ, c->name, "Parting Empty Channel");
		c->statservon = 0;
	}
	else if (!c->users && !ChanHasMode(c->name, ircd->persist_char))
	{
		chan_delete(c);
	}
}

/*************************************************************************/

/* Remove the status of an user on a channel */
void chan_remove_user_status(Channel * chan, User * user, int16 status)
{
	struct u_chanlist *uc;

	SET_SEGV_LOCATION();

	for (uc = user->chans; uc; uc = uc->next)
	{
		if (uc->chan == chan)
		{
			uc->status &= ~status;
			break;
		}
	}
}

/*************************************************************************/

/* Return the Channel structure corresponding to the named channel, or NULL
 * if the channel was not found.  chan is assumed to be non-NULL and valid
 * (i.e. pointing to a channel name of 2 or more characters).
 */
Channel *findchan(const char *chan)
{
	Channel *c;

	SET_SEGV_LOCATION();

	if (!chan || !*chan)
	{
		alog(LOG_ERROR, langstr(ALOG_DEBUG_FINDCHAN_BAD));
		do_backtrace(0);
		return NULL;
	}

	alog(LOG_EXTRADEBUG, "debug: findchan(%p)", chan);
	c = chanlist[HASH(chan)];
	SET_SEGV_LOCATION();
	while (c)
	{
		if (stricmp(c->name, chan) == 0)
		{
			return c;
		}
		c = c->next;
	}
	alog(LOG_EXTRADEBUG, "debug: findchan(%s) -> %p", chan, (void *) c);
	return NULL;
}

/*************************************************************************/

StatsChannel *findstatschan(const char *chan)
{
	lnode_t *tn;
	StatsChannel *c = NULL;

	SET_SEGV_LOCATION();

	if (!chan || !*chan)
	{
		return NULL;
	}
	alog(LOG_EXTRADEBUG, "debug: findstatschan(%s)", chan);

	tn = list_find(StatsChanhead, chan, CompareStatsChan);
	if (tn)
	{
		c = lnode_get(tn);
		alog(LOG_EXTRADEBUG, "debug: findstatschan(%s) -> %p %s", chan,
		     (void *) c, c->name);
		return c;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

/* Iterate over all channels in the channel list.  Return NULL at end of
 * list.
 */
Channel *firstchan(void)
{
	next_index = 0;
	SET_SEGV_LOCATION();

	while (next_index < 1024 && current == NULL)
	{
		current = chanlist[next_index++];
	}
	SET_SEGV_LOCATION();
	alog(LOG_EXTRADEBUG, "debug: firstchan() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

Channel *nextchan(void)
{
	SET_SEGV_LOCATION();

	if (current)
	{
		current = current->next;
	}
	if (!current && next_index < 1024)
	{
		while (next_index < 1024 && current == NULL)
			current = chanlist[next_index++];
	}
	SET_SEGV_LOCATION();

	alog(LOG_EXTRADEBUG, "debug: nextchan() returning %s",
	     current ? current->name : "NULL (end of list)");
	return current;
}

/*************************************************************************/

/**
 * Handle BMASK data
 *
 * @param ac is the count
 * @param av is the array
 * @return void - no returend value
 */
void do_bmask(char **av)
{
	Channel *c;
	char *bans;
	char *b;
	int count, i;

	c = findchan(av[1]);

	if (c)
	{
		bans = sstrdup(av[3]);
		count = myNumToken(bans, ' ');
		for (i = 0; i <= count - 1; i++)
		{
			b = myStrGetToken(bans, ' ', i);
			if (!stricmp(av[2], "b"))
			{
				add_ban(c, b);
			}
			if (!stricmp(av[2], "e"))
			{
				add_exception(c, b);
			}
			if (!stricmp(av[2], "I"))
			{
				add_invite(c, b);
			}
			if (!stricmp(av[2], "q"))
			{
				add_quiet(c, b);
			}
			free(b);
		}
		free(bans);
	}
}

char *p10_mode_parse(char *mode, int *nomode)
{
	char *s;
	char modebuf[15];
	char *temp = NULL;
	const char *flag;

	nomode = 0;

	/* We make all the users join */
	s = sstrdup(mode);
	while (*s)
	{
		switch (*s)
		{
			case 'o':
				flag = "@";
				break;
			case 'h':
				flag = "%";
				break;
			case 'v':
				flag = "+";
				break;
			case 'a':
				flag = "~";
				break;
			case 'q':
				flag = "*";
				break;
			default:
				alog(LOG_ERROR,
				     "ERROR: Unknown user flag in p10_mode_parse()");
				flag = "";
				break;
		}
		while (csmodes[(int) *flag] != 0)
		{
			nomode++;
			if (temp)
			{
				ircsnprintf(modebuf, sizeof(modebuf), "%s%c", temp, *s);
				free(temp);
				temp = sstrdup(modebuf);
			}
			else
			{
				ircsnprintf(modebuf, sizeof(modebuf), "+%c", *s);
				temp = sstrdup(modebuf);
			}
			break;
		}
		(void) *s++;
	}
	return temp;
}


/*************************************************************************/

/**
 * Handle P10 Burst event messages, and pass to correct functions
 *
 * @param ac is the count
 * @param av is the array
 * @return void - no returend value
 */
void do_p10_burst(char *source, int ac, char **av)
{
	/*
	 * av[0] <channel>
	 * av[1] <timestamp>
	 * av[2+] [<+modes> [<mode extra parameters>]] [<users>] [<%bans>]
	 * - <modes> is a parameter of which the first character is a +, it is parsed as a channel mode change;
	 *           there can be extra parameters after the mode parameter for key and limit
	 * - <users> parameter has no prefix char. it is a comma separated list, where each entry is
	 *           either <nicknum> or <nicknum>:<modes>. if the colon+modes is present, it defines membership
	 *           modes which apply to that entry, and to all following entries until there's another entry which
	 *           defines modes. the modes paramer contains the membership mode chars ("o" for ops, etc). for sending,
	 *           it must have the modes from high priority to low priority - "ov" is valid, "vo" is not.
	 * - <bans> parameter: the first character is a %, then a space separated list of bans.
	 * - in a generated burst message, the users must be sorted by the modes: first users w/o modes, then users
	 *   with voice, then with op, then with op+voice: num,num:v,num:o,num:ov
	 */

	Channel *c;
	User *user;
	char *s, *t, *m;
	char *v[32];
	char *x[32];
	int i = 0;
	int nomode = 0;
	int pc = 2;
	char *newav[32];
	int j = 2;
	char *flag;
	char *modes;
	int except = 0;

	c = findchan(av[0]);
	if (!c)
	{
		c = chan_create(av[0], strtol(av[1], NULL, 10));
		db_getchancreate(av[0]);
	}
	if (c)
	{
		while (pc < ac)
		{
			switch (*av[pc])
			{
					/* set modes, and extra modes if needed */
				case '+':
					newav[0] = av[0];
					newav[1] = av[pc];
					if (myNumToken(av[pc], 'l') && myNumToken(av[pc], 'k') && myNumToken(av[pc], 'L'))
					{
						newav[2] = av[pc + 1];
						newav[3] = av[pc + 2];
						newav[4] = av[pc + 3];
						do_cmode(source, 5, newav);
						pc = pc + 3;
					}
					else if ((myNumToken(av[pc], 'l') && myNumToken(av[pc], 'k')) || (myNumToken(av[pc], 'l') && myNumToken(av[pc], 'L')) || (myNumToken(av[pc], 'k') && myNumToken(av[pc], 'L')))
					{
						newav[2] = av[pc + 1];
						newav[3] = av[pc + 2];
						do_cmode(source, 4, newav);
						pc = pc + 2;
					}
					else if (myNumToken(av[pc], 'l') || myNumToken(av[pc], 'k') || myNumToken(av[pc], 'L'))
					{
						newav[2] = av[pc + 1];
						do_cmode(source, 3, newav);
						pc = pc + 1;
					}
					else
					{
						do_cmode(source, 2, newav);
					}
					pc++;
					break;
					/* set bans and excepts */
				case '%':
					i = 0;
					while ((s = myStrGetToken(av[pc], ' ', i)))
					{
						if (strcmp(s, "~") == 0)
						{
							except = 1;
							free(s);
							i++;
							continue;
						}
						if (except == 0)
						{
							if (s[0] == '%')
								s++;
							if (s[0] != 0)
								add_ban(c, s);
						}
						else
						{
							add_exception(c, s);
						}
						i++;
					}
					pc++;
					break;
					/* add users */
				default:
					flag = NULL;
					while ((s = myStrGetToken(av[pc], ',', i)))
					{
						nomode = 0;
						t = myStrGetToken(s, ':', 0);
						m = myStrGetToken(s, ':', 1);
						user = user_find(t);
						if (!user)
						{
							alog(LOG_NONEXISTANT,
							     langstr(ALOG_DEBUG_SJOIN_NONEXISTANT), t,
							     av[0]);
							i++;
							free(t);
							free(m);
							continue;
						}
						v[0] = av[0];
						/* make user join the channel */
						do_join(user->nick, 1, v);
						if (m)
						{
							flag = sstrdup(m);
							free(m);
						}
						if (flag)
						{
							x[0] = av[0];
							x[1] = p10_mode_parse(flag, &nomode);
							if (x[1])
							{
								if (*x[1] == '+')
								{
									modes = sstrdup(x[1]);
									j = 2;
									while (*modes)
									{
										switch (*modes)
										{
											case '+':
												break;
											default:
												x[j++] = user->nick;
												break;
										}
										modes++;
									}
									/* setting user qaohv flags */
									do_cmode(user->nick, j, x);
									free(x[1]);
								}
							}
						}
						i++;
						free(t);
					}
					if (flag)
						free(flag);
					pc++;
					break;
			}
		}
	}
	else
	{
		alog(LOG_ERROR,
		     "ERROR: Unable to find channel %s in do_p10_burst()", av[0]);
	}
}

/*************************************************************************/

/**
 * Handle JOIN event messages
 *
 * @param source is the source of the message
 * @param ac is the count
 * @param av is the array
 * @return void - no returend value
 */
void do_join(const char *source, int ac, char **av)
{
	User *user;
	char *s, *t;
	struct u_chanlist *c, *nextc;
	time_t ts = time(NULL);

	SET_SEGV_LOCATION();

	if (ac < 1)
	{
		return;
	}
	/* if ircd is p10, we should copy source to a prefixednick var,
	 * then go thorugh source and ++ in case of @%+, and pass prefixednick
	 * to sql func instead of nick */
	user = user_find(source);
	if (!user)
	{
		alog(LOG_NONEXISTANT, langstr(ALOG_DEBUG_JOIN_NONEXISTANT),
		     source, merge_args(ac, av));
		return;
	}

	t = av[0];
	while (*(s = t))
	{
		t = s + strcspn(s, ",");
		if (*t)
			*t++ = 0;
		SET_SEGV_LOCATION();

		if (*s == '0')
		{
			c = user->chans;
			while (c)
			{
				nextc = c->next;
				send_event(EVENT_USER_PART, 3, user->nick, c->chan->name,
				           "Left all channels");
				chan_deluser(user, c->chan);
				if (denora->do_sql)
					sql_do_partall(user->nick);
				free(c);
				c = nextc;
			}
			user->chans = NULL;
			continue;
		}

		if (ac == 2)
		{
			alog(LOG_DEBUG, "debug: received a new TS for JOIN: %ld",
			     (long int) ts);
			ts = strtoul(av[1], NULL, 10);
		}

		join_user_update(user, findchan(s), s, ts);
		if (denora->do_sql)
		{
			sql_do_join(s, user->nick);
		}
		SET_SEGV_LOCATION();
	}
}

/*************************************************************************/

void do_p10_kick(const char *source, int ac, char **av)
{
	char *v[32];
	User *u, *u2;

	if (ac != 3)
	{
		return;
	}

	u = find_byuid(source);
	u2 = find_byuid(av[1]);

	v[0] = sstrdup(av[0]);
	v[1] = sstrdup((u2 ? u2->nick : av[1]));
	v[2] = sstrdup(av[1]);
	do_kick((u ? u->nick : source), ac, v);
	free(v[0]);
	free(v[1]);
	free(v[2]);
}

/*************************************************************************/

/* Handle a KICK command.
 *    av[0] = channel
 *    av[1] = nick(s) being kicked
 *    av[2] = reason
 */
void do_kick(const char *source, int ac, char **av)
{
	User *user, *kicker;
	char *s, *t, *chan;
	struct u_chanlist *c;
	Channel *c2;
	ChannelStats *cs;
	int chanid;
	char nickbuf[BUFSIZE];
	*nickbuf = '\0';

	SET_SEGV_LOCATION();

	t = av[1];
	while (*(s = t))
	{
		t = s + strcspn(s, ",");
		if (*t)
			*t++ = 0;

		user = user_find(s);
		if (!user)
		{
			alog(LOG_NONEXISTANT, langstr(ALOG_DEBUG_KICK_NONEXISTANT), s,
			     av[0], merge_args(ac - 2, av + 2));
			continue;
		}
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_KICKED), s, av[0]);

		c2 = findchan(av[0]);
		if (c2)
		{
			c2->stats->kickcount++;
			c2->stats->kickcounttime = time(NULL);
			if (denora->do_sql)
			{
				chan = rdb_escape(c2->name);
				chanid = db_getchannel(chan);
				rdb_query(QUERY_LOW,
				          "UPDATE %s SET kickcount=%d WHERE chanid=%d",
				          ChanTable, c2->stats->kickcount, chanid);
				sql_do_part(chan, user);
				free(chan);
			}
		}

		kicker = user_find(source);
		/* if we can't find the person whom kicked they are likely
		   a server or nonexistent either way we shouldn't count them */
		if (kicker && denora->do_sql && !LargeNet)
		{
			count_kicks(kicker, user, c2);
		}
		SET_SEGV_LOCATION();

		for (c = user->chans; c && stricmp(av[0], c->chan->name) != 0;
		        c = c->next);
		if (c)
		{
			send_event(EVENT_CHAN_KICK, 3, user->nick, av[0],
			           (ac >= 3 ? av[2] : ""));
			chan_deluser(user, c->chan);
			SET_SEGV_LOCATION();
			if (c->next)
			{
				c->next->prev = c->prev;
			}
			if (c->prev)
			{
				c->prev->next = c->next;
			}
			else
			{
				user->chans = c->next;
			}
			SET_SEGV_LOCATION();
			/* free(c); disabled to fix #491 */
		}
		if ((cs = find_cs(av[0])))
		{
			if (!stricmp(s, s_StatServ))
			{
				alog(LOG_DEBUG, "TESTING: calling denora_cmd_join channels.c:989");
				denora_cmd_join(s_StatServ, av[0], time(NULL));
			}
		}
	}
}

/*************************************************************************/

/* Handle a PART command.
 *    av[0] = channels to leave
 *    av[1] = reason (optional)
 */
void do_part(const char *source, int ac, char **av)
{
	User *user;
	char *s, *t;
	struct u_chanlist *c;
	Channel *c2;

	SET_SEGV_LOCATION();

	if (ac < 1)
	{
		return;
	}

	user = user_find(source);
	if (!user)
	{
		alog(LOG_NONEXISTANT, langstr(ALOG_DEBUG_PART_NONEXISTANT),
		     source, merge_args(ac, av));
		return;
	}
	SET_SEGV_LOCATION();
	t = av[0];
	while (*(s = t))
	{
		t = s + strcspn(s, ",");
		if (*t)
			*t++ = 0;
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_PART), user->nick, s);

		for (c = user->chans; c && stricmp(s, c->chan->name) != 0;
		        c = c->next);
		if (c)
		{
			if (!c->chan)
			{
				alog(LOG_ERROR, langstr(ALOG_DEBUG_PART_BUG), s);
				return;
			}
			SET_SEGV_LOCATION();
			send_event(EVENT_USER_PART, 3, user->nick, c->chan->name,
			           av[1] ? av[1] : "");
			chan_deluser(user, c->chan);
			c2 = findchan(av[0]);
			if (c2)
			{
				c2->stats->partcount++;
				c2->stats->partcounttime = time(NULL);
			}

			if (denora->do_sql)
				sql_do_part(s, user);
			if (c->next)
			{
				c->next->prev = c->prev;
			}
			if (c->prev)
			{
				c->prev->next = c->next;
			}
			else
			{
				user->chans = c->next;
			}
			SET_SEGV_LOCATION();
			free(c);
		}
	}
}

/*************************************************************************/

/* Handle a SJOIN command.

   On channel creation, syntax is:

   av[0] = timestamp
   av[1] = channel name
   av[2|3|4] = modes   \   depends of whether the modes k and l
   av[3|4|5] = users   /   are set or not.

   When a single user joins an (existing) channel, it is:

   av[0] = timestamp
   av[1] = user

   ============================================================

   Unreal SJOIN

   On Services connect there is
   SJOIN !11LkOb #ircops +nt :@Trystan &*!*@*.aol.com "*@*.home.com

   av[0] = time stamp (base64)
   av[1] = channel
   av[2] = modes
   av[3] = users + bans + exceptions

   On Channel Creation or a User joins an existing
   Luna.NomadIrc.Net SJOIN !11LkW9 #akill :@Trystan
   Luna.NomadIrc.Net SJOIN !11LkW9 #akill :Trystan`

   av[0] = time stamp (base64)
   av[1] = channel
   av[2] = users

*/
void do_sjoin(const char *source, int ac, char **av)
{
	Channel *c;
	User *user;
	Server *serv;
	char *s = NULL;
	char *buf, *end, cubuf[7], *end2, *cumodes[6];
	char *sqlusers = NULL;
	int ts = 0;
	int is_created = 0;
	int keep_their_modes = 1;

	SET_SEGV_LOCATION();

	serv = findserver(servlist, source);

	if (ircd->sjb64)
	{
		ts = base64dec_ts(av[0]);
	}
	else
	{
		ts = strtoul(av[0], NULL, 10);
	}
	c = findchan(av[1]);
	if (c != NULL)
	{
		if (c->creation_time == 0 || ts == 0)
			c->creation_time = 0;
		else if (c->creation_time > ts)
		{
			c->creation_time = ts;
		}
		else if (c->creation_time < ts)
			keep_their_modes = 0;
	}
	else
		is_created = 1;

	/* Double check to avoid unknown modes that need parameters */
	if (ac >= 4)
	{
		if (denora->do_sql)
		{
			sqlusers = sstrdup(av[ac - 1]);
			sql_do_sjoin(av[1], sqlusers, &av[2], (ac > 3) ? ac - 3 : 0);
			free(sqlusers);
		}

		cubuf[0] = '+';
		cumodes[0] = cubuf;

		/* We make all the users join */
		s = av[ac - 1];         /* Users are always the last element */

		while (*s)
		{
			end = strchr(s, ' ');
			if (end)
				*end = 0;

			end2 = cubuf + 1;


			if (ircd->sjoinbanchar)
			{
				if (*s == ircd->sjoinbanchar && keep_their_modes)
				{
					buf = myStrGetToken(s, ircd->sjoinbanchar, 1);
					add_ban(c, buf);
					free(buf);
					if (!end)
						break;
					s = end + 1;
					continue;
				}
			}
			if (ircd->sjoinexchar)
			{
				if (*s == ircd->sjoinexchar && keep_their_modes)
				{
					buf = myStrGetToken(s, ircd->sjoinexchar, 1);
					add_exception(c, buf);
					free(buf);
					if (!end)
						break;
					s = end + 1;
					continue;
				}
			}

			if (ircd->sjoinivchar)
			{
				if (*s == ircd->sjoinivchar && keep_their_modes)
				{
					buf = myStrGetToken(s, ircd->sjoinivchar, 1);
					add_invite(c, buf);
					free(buf);
					if (!end)
						break;
					s = end + 1;
					continue;
				}
			}

			while (csmodes[(int) *s] != 0)
				*end2++ = csmodes[(int) *s++];
			*end2 = 0;

			user = user_find(s);
			if (!user)
			{
				alog(LOG_NONEXISTANT,
				     langstr(ALOG_DEBUG_SJOIN_NONEXISTANT), s, av[1]);
				return;
			}

			c = join_user_update(user, c, av[1], ts);
			/* We update user mode on the channel */
			if (end2 - cubuf > 1 && keep_their_modes)
			{
				int i;

				for (i = 1; i < end2 - cubuf; i++)
					cumodes[i] = user->nick;
				chan_set_modes(c, 1 + (end2 - cubuf - 1), cumodes);
			}

			if (!end)
				break;
			s = end + 1;
		}

		if (c)
		{
			/* We now update the channel mode. */
			chan_set_modes(c, ac - 3, &av[2]);
			SET_SEGV_LOCATION();
		}

		/* Unreal just had to be different */
	}
	else if (ac == 3 && !ircd->ts6)
	{
		if (denora->do_sql)
		{
			sqlusers = sstrdup(av[ac - 1]);
			sql_do_sjoin(av[1], sqlusers, &av[2], (ac > 3) ? ac - 3 : 0);
			free(sqlusers);
		}
		c = findchan(av[1]);
		cubuf[0] = '+';
		cumodes[0] = cubuf;

		/* We make all the users join */
		s = av[2];              /* Users are always the last element */

		while (*s)
		{
			end = strchr(s, ' ');
			if (end)
				*end = 0;

			end2 = cubuf + 1;

			while (csmodes[(int) *s] != 0)
				*end2++ = csmodes[(int) *s++];
			*end2 = 0;

			user = user_find(s);
			if (!user)
			{
				alog(LOG_NONEXISTANT,
				     langstr(ALOG_DEBUG_SJOIN_NONEXISTANT), s, av[1]);
				return;
			}

			c = join_user_update(user, c, av[1], ts);
			SET_SEGV_LOCATION();

			/* We update user mode on the channel */
			if (end2 - cubuf > 1)
			{
				int i;

				for (i = 1; i < end2 - cubuf; i++)
				{
					cumodes[i] = user->nick;
				}
				chan_set_modes(c, 1 + (end2 - cubuf - 1), cumodes);
				SET_SEGV_LOCATION();
			}

			if (!end)
				break;
			s = end + 1;
		}

	}
	else if (ac == 3 && ircd->ts6)
	{
		if (denora->do_sql)
		{
			sqlusers = sstrdup(source);
			sql_do_sjoin(av[1], sqlusers, &av[2], (ac > 3) ? ac - 3 : 0);
			free(sqlusers);
		}

		cubuf[0] = '+';
		cumodes[0] = cubuf;

		/* We make all the users join */
		s = av[2];              /* Users are always the last element */

		while (*s)
		{
			end = strchr(s, ' ');
			if (end)
				*end = 0;

			end2 = cubuf + 1;

			while (csmodes[(int) *s] != 0)
				*end2++ = csmodes[(int) *s++];
			*end2 = 0;

			user = user_find(s);
			if (!user)
			{
				alog(LOG_NONEXISTANT,
				     langstr(ALOG_DEBUG_SJOIN_NONEXISTANT), s, av[1]);
				free(s);
				return;
			}

			c = join_user_update(user, c, av[1], ts);
			SET_SEGV_LOCATION();

			/* We update user mode on the channel */
			if (end2 - cubuf > 1 && keep_their_modes)
			{
				int i;

				for (i = 1; i < end2 - cubuf; i++)
				{
					cumodes[i] = user->nick;
				}
				chan_set_modes(c, 1 + (end2 - cubuf - 1), cumodes);
				SET_SEGV_LOCATION();
			}

			if (!end)
				break;
			s = end + 1;
		}
	}
	else if (ac == 2)
	{
		if (denora->do_sql)
		{
			sqlusers = sstrdup(source);
			sql_do_join(av[1], sqlusers);
			free(sqlusers);
		}
		user = user_find(source);
		if (!user)
		{
			alog(LOG_NONEXISTANT, langstr(ALOG_DEBUG_SJOIN_NONEXISTANT),
			     source, av[1]);
			return;
		}
		SET_SEGV_LOCATION();
		c = join_user_update(user, c, av[1], ts);
	}
}


/*************************************************************************/

/* Handle a channel MODE command. */
void do_cmode(const char *source, int ac, char **av)
{
	Channel *chan;
	int i;
	char *t;
	User *u;
	Exclude *e;

	SET_SEGV_LOCATION();

	if (ircdcap->tsmode)
	{
		/* TSMODE for bahamut - leave this code out to break MODEs. -GD */
		/* if they don't send it in CAPAB check if we just want to enable it */
		if (denora->capab & ircdcap->tsmode || UseTSMODE)
		{
			for (i = 0; i < (int) strlen(av[1]); i++)
			{
				if (!isdigit(av[1][i]))
					break;
			}
			if (av[1][i] == '\0')
			{
				/* We have a valid TS field in av[1] now, so we can strip it off */
				/* After we swap av[0] and av[1] of course to not break stuff! :) */
				t = av[0];
				av[0] = av[1];
				av[1] = t;
				ac--;
				av++;
			}
			else
			{
				alog(LOG_ERROR, langstr(ALOG_NON_TS_MODE));
			}
		}
	}

	/* :42XAAAAAO TMODE 1106409026 #ircops +b *!*@*.aol.com */
	if (UseTS6 && ircd->ts6)
	{
		if (isdigit(av[0][0]))
		{
			ac--;
			av++;
		}
	}

	chan = findchan(av[0]);
	if (!chan)
	{
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_NO_FIND_CHAN), av[0]);
		return;
	}
	SET_SEGV_LOCATION();

	/* This shouldn't trigger on +o, etc. */
	if (strchr(source, '.') && !av[1][strcspn(av[1], "bovahq")])
	{
		if (time(NULL) != chan->server_modetime)
		{
			chan->server_modecount = 0;
			chan->server_modetime = time(NULL);
		}
		chan->server_modecount++;
	}

	ac--;
	av++;
	SET_SEGV_LOCATION();

	chan_set_modes(chan, ac, av);
	SET_SEGV_LOCATION();
	if (denora->do_sql)
	{
		sql_do_chanmodes(chan->name, ac, av);
	}
	if ((u = user_find(source)))
	{
		if (denora->do_sql && !LargeNet)
		{
			e = find_exclude(u->nick, u->server->name);
			if (!e)
			{
				count_modes(u, chan);
			}
		}
	}
}

/*************************************************************************/

/* Handle a TOPIC command. */

void do_topic(int ac, char **av)
{
	Channel *c = findchan(av[0]);
	int chanid;
	User *u;
	Exclude *e;
	char *s, *author, *topic;

	SET_SEGV_LOCATION();

	if (!c)
	{
		alog(LOG_NONEXISTANT, langstr(ALOG_TOPIC_NONEXISTANT),
		     merge_args(ac - 1, av + 1), av[0]);
		return;
	}

	if (ircd->sjb64 && ac > 2 && *av[2] == '!')
	{
		c->topic_time = base64dec_ts(av[2]);
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_TOPIC_TS), av[2],
		     c->topic_time);
	}
	else
	{
		c->topic_time =
		    (((int) ac ==
		      (int) 2) ? (time_t) time(NULL) : (time_t) strtoul(av[2],
		              NULL, 10));
	}

	if (c->topic)
	{
		free(c->topic);
		c->topic = NULL;
	}
	c->topic = (ac > 3 ? sstrdup(av[3]) : NULL);

	if (strstr(av[1], "!"))
	{
		s = myStrGetToken(av[1], '!', 0);
		strlcpy(c->topic_setter, s, sizeof(c->topic_setter));
		free(s);
	}
	else
	{
		strlcpy(c->topic_setter, av[1], sizeof(c->topic_setter));
	}
	c->stats->topic_count++;
	c->stats->topiccounttime = c->topic_time;

	if (denora->do_sql)
	{
		SET_SEGV_LOCATION();
		author = rdb_escape(c->topic_setter);
		topic = (c->topic ? rdb_escape(c->topic) : NULL);
		chanid = db_getchannel(c->sqlchan);
		if (chanid)
		{
			rdb_query
			(QUERY_LOW,
			 "UPDATE %s SET topic=\'%s\', topicauthor=\'%s\', topictime=FROM_UNIXTIME(%ld) WHERE chanid=%d",
			 ChanTable, (topic ? topic : ""), author,
			 (long int) c->topic_time, chanid);
		}
		u = user_find(c->topic_setter);
		if (u && !LargeNet)
		{
			e = find_exclude(u->nick, u->server->name);
			if (!e)
			{
				count_topics(u, c);
			}
		}
		free(author);
		if (topic)
			free(topic);
	}
	SET_SEGV_LOCATION();
	send_event(EVENT_CHANNEL_TOPIC, 3, c->name, c->topic_setter,
	           (c->topic ? c->topic : (char *) ""));
}

/*
 * Clears all given modes from a channel
 * av[0] = channel name
 * av[1] = modes to be removed
 */
void chan_clearmodes(const char *source, int ac, char **av)
{
	Channel *c;
	char mode, *modebuf, *newav[2];
	int i, p = 1;

	if (ac != 2)
	{
		alog(LOG_ERROR, "Invalid number of arguments passed to chan_clearmodes");
		return;
	}

	modebuf = sstrdup("-");
	c = findchan(av[0]);
	if (c)
	{
		while ((mode = *av[1]++))
		{
			switch (mode)
			{
				case 'b': /* remove all bans */
					for (i = 0; i < c->bancount; ++i)
					{
						if (c->bans[i])
						{
							free(c->bans[i]);
						}
						else
						{
							alog(LOG_ERROR, langstr(ALOG_BAN_FREE_ERROR), c->name, i);
						}
					}
					if (denora->do_sql) sql_channel_ban(ALL, c, NULL);
					break;
				case 'e': /* remove all ban exceptions */
					for (i = 0; i < c->exceptcount; ++i)
					{
						if (c->excepts[i])
						{
							free(c->excepts[i]);
						}
						else
						{
							alog(LOG_ERROR, langstr(ALOG_EXCEPTION_FREE_ERROR),
							     c->name, i);
						}
					}
					if (denora->do_sql) sql_channel_exception(ALL, c, NULL);
					break;
				case 'o': /* remove all ops */
					if (denora->do_sql)
					{
						rdb_query
						(QUERY_LOW,
						 "UPDATE %s SET mode_lo=\'N\' WHERE chanid=%d",
						 IsOnTable, c->sqlid);
					}
					break;
				case 'h': /* remove all halfops */
					if (denora->do_sql)
					{
						rdb_query
						(QUERY_LOW,
						 "UPDATE %s SET mode_lh=\'N\' WHERE chanid=%d",
						 IsOnTable, c->sqlid);
					}
					break;
				case 'v': /* remove all voices */
					if (denora->do_sql)
					{
						rdb_query
						(QUERY_LOW,
						 "UPDATE %s SET mode_lv=\'N\' WHERE chanid=%d",
						 IsOnTable, c->sqlid);
					}
					break;
				default:
					modebuf[p++] = mode;
			}
		}
		/* remove all given modes */
		newav[0] = av[0];
		newav[1] = sstrdup(modebuf);
		SET_SEGV_LOCATION();
		do_cmode(source, 2, newav);
	}
}

/*************************************************************************/

/* Add/remove a user to/from a channel, creating or deleting the channel as
 * necessary.  If creating the channel, restore mode lock and topic as
 * necessary.  Also check for auto-opping and auto-voicing.
 * Modified, so ignored users won't get any status via services -certus
 */
void chan_adduser2(User * user, Channel * c)
{
	struct c_userlist *u;
	ChannelStats *cs;
	char nickbuf[BUFSIZE];
	*nickbuf = '\0';

	SET_SEGV_LOCATION();

	u = calloc(sizeof(struct c_userlist), 1);
	u->next = c->users;
	if (c->users)
	{
		c->users->prev = u;
	}
	c->users = u;
	u->user = user;
	c->stats->usercount++;

	if (!c->stats->maxusertime)
	{
		c->stats->maxusertime = time(NULL);
	}
	if (c->stats->usercount > c->stats->maxusercount)
	{
		c->stats->maxusercount++;
		c->stats->maxusertime = time(NULL);
	}
	send_event(EVENT_USER_JOIN, 2, user->nick, c->name);
	if (denora->do_sql)
	{
		rdb_query
		(QUERY_LOW,
		 "UPDATE %s SET currentusers=%d, maxusers=%d, maxusertime=%ld WHERE chanid=%d",
		 ChanTable, c->stats->usercount, c->stats->maxusercount,
		 (long int) c->stats->maxusertime, db_getchancreate(c->name));
	}
	c->stats->joincounter++;
	c->stats->joincounttime = time(NULL);

	if (!stricmp(user->nick, s_StatServ))
	{
		c->statservon = 1;
	}
	cs = find_cs(c->name);

	if (cs && c->statservon == 0)
	{
		alog(LOG_DEBUG, "TESTING: calling denora_cmd_join channels.c:1643");
		denora_cmd_join(s_StatServ, c->name, c->creation_time);
	}

	SET_SEGV_LOCATION();
}

/*************************************************************************/

/* This creates the channel structure (was originally in
 *  chan_adduser, but splitted to make it more efficient to use for
 *  SJOINs).
 */
Channel *chan_create(char *chan, time_t ts)
{
	Channel *c;
	StatsChannel *sc;
	Channel **list;

	SET_SEGV_LOCATION();

	if (BadPtr(chan))
	{
		alog(LOG_DEBUG, "chan_create called with NULL arguments");
		return NULL;
	}

	alog(LOG_DEBUG, langstr(ALOG_CREATE_CHAN), chan);

	/* Allocate pre-cleared memory */
	c = calloc(sizeof(Channel), 1);
	strlcpy(c->name, chan, sizeof(c->name));
	list = &chanlist[HASH(c->name)];
	c->next = *list;
	if (*list)
	{
		(*list)->prev = c;
	}
	SET_SEGV_LOCATION();

	*list = c;
	c->creation_time = ts;
	c->cstats = 0;
	sc = findstatschan(c->name);
	if (sc)
	{
		c->stats = sc;
	}
	else
	{
		sc = statschan_create(chan);
		c->stats = sc;
	}
	SET_SEGV_LOCATION();
	c->sqlchan = rdb_escape(chan);

	c->stats->in_use = 1;
	stats->chans++;
	do_checkchansmax();
	return c;
}

/*************************************************************************/

StatsChannel *statschan_create(char *chan)
{
	lnode_t *tn;
	StatsChannel *c = NULL;
	SET_SEGV_LOCATION();

	tn = list_find(StatsChanhead, chan, CompareStatsChan);
	if (tn)
	{
		c = lnode_get(tn);
	}
	else
	{
		c = malloc(sizeof(StatsChannel));
		strlcpy(c->name, chan, sizeof(c->name));
		c->kickcount = 0;
		c->kickcounttime = 0;
		c->joincounter = 0;
		c->joincounttime = 0;
		c->topic_count = 0;
		c->topiccounttime = 0;
		c->usercount = 0;
		c->maxusercount = 0;
		c->maxusertime = 0;
		c->in_use = 0;
		c->partcount = 0;
		c->partcounttime = 0;
		c->modecount = 0;
		c->modecounttime = 0;
		c->secret = 0;
		c->private = 0;
		tn = lnode_create(c);
		list_append(StatsChanhead, tn);
	}
	return c;
}


/*************************************************************************/

void Fini_StatsChannel(void)
{
	StatsChannel *t;
	lnode_t *tn;

	tn = list_first(StatsChanhead);
	while (tn != NULL)
	{
		t = lnode_get(tn);
		free(t);
		tn = list_next(StatsChanhead, tn);
	}
	list_destroy_nodes(StatsChanhead);
}

/*************************************************************************/

/* This destroys the channel structure, freeing everything in it. */

void chan_delete(Channel * c)
{
	int i;

	SET_SEGV_LOCATION();

	if (!c)
	{
		return;
	}

	alog(LOG_EXTRADEBUG, langstr(ALOG_DELETE_CHAN), c->name);

	c->stats->in_use = 0;
	stats->chans--;

	if (c->topic)
		free(c->topic);
	SET_SEGV_LOCATION();

	if (c->key)
		free(c->key);
	if (ircd->fmode && c->flood)
	{
		free(c->flood);
	}
	if (ircd->jmode && c->flood_alt)
	{
		free(c->flood_alt);
	}
	if (ircd->Lmode && c->redirect)
	{
		free(c->redirect);
	}
	SET_SEGV_LOCATION();

	for (i = 0; i < c->bancount; ++i)
	{
		if (c->bans[i])
		{
			free(c->bans[i]);
		}
		else
		{
			alog(LOG_ERROR, langstr(ALOG_BAN_FREE_ERROR), c->name, i);
		}
	}
	if (c->bansize && c->bans)
	{
		free(c->bans);
	}
	if (c->sqlchan)
		free(c->sqlchan);

	if (denora->do_sql)
		sql_channel_ban(ALL, c, NULL);
	SET_SEGV_LOCATION();

	if (ircd->except)
	{
		for (i = 0; i < c->exceptcount; ++i)
		{
			if (c->excepts[i])
			{
				free(c->excepts[i]);
			}
			else
			{
				alog(LOG_ERROR, langstr(ALOG_EXCEPTION_FREE_ERROR),
				     c->name, i);
			}
		}
		if (c->exceptsize && c->excepts)
		{
			free(c->excepts);
		}
		if (denora->do_sql)
		{
			sql_channel_exception(ALL, c, NULL);
		}
	}

	SET_SEGV_LOCATION();

	if (ircd->invitemode)
	{
		for (i = 0; i < c->invitecount; ++i)
		{
			if (c->invite[i])
			{
				free(c->invite[i]);
			}
			else
			{
				alog(LOG_ERROR, langstr(ALOG_INVITE_FREE_ERROR), c->name,
				     i);
			}
		}
		if (c->invitesize && c->invite)
		{
			free(c->invite);
		}
		if (denora->do_sql)
		{
			sql_channel_invite(ALL, c, NULL);
		}
	}
	StatsChannel_delete(c->stats);

	if (c->next)
	{
		c->next->prev = c->prev;
	}
	if (c->prev)
	{
		c->prev->next = c->next;
	}
	else
	{
		chanlist[HASH(c->name)] = c->next;
	}

	if (denora->do_sql)
	{
		rdb_query(QUERY_LOW,
		          "UPDATE %s SET val=%d, time=%ld WHERE type='chans'",
		          CurrentTable, stats->chans, time(NULL));
	}

	SET_SEGV_LOCATION();
	free(c);
}

/*************************************************************************/

void StatsChannel_delete(__attribute__((unused))StatsChannel * c)
{
}

/*************************************************************************/

Channel *join_user_update(User * user, Channel * chan, char *name,
                          time_t chants)
{
	struct u_chanlist *c;

	SET_SEGV_LOCATION();

	/* If it's a new channel, so we need to create it first. */
	if (!chan)
	{
		chan = chan_create(name, chants);
		/* Second chance failed we got issues */
		if (!chan)
		{
			return NULL;
		}
	}

	SET_SEGV_LOCATION();

	if (ircd->p10)
	{
		alog(LOG_DEBUG, "debug: %s (uid %s) joins %s", user->nick,
		     user->uid, chan->name);
	}
	else
	{
		alog(LOG_DEBUG, langstr(ALOG_DEBUG_JOIN), user->nick, chan->name);
	}

	c = calloc(sizeof(*c), 1);
	c->next = user->chans;
	if (user->chans)
	{
		user->chans->prev = c;
	}
	user->chans = c;
	c->chan = chan;

	chan_adduser2(user, chan);

	SET_SEGV_LOCATION();

	return chan;
}
