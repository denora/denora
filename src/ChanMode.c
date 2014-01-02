/* Channel Modes - read MODES for complete details on how Denora
 * handles modes.
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

ChanModeHash *CHANMODEHANDLERS[MAX_MODE_HASH];
unsigned long cmodes[128];
char csmodes[128];
unsigned long sjoinmodes[128];


/*************************************************************************/

/* This is purely so that the Array is set to all 0 before
   we begin to loop it and check for the data
*/
void init_cmodes(void)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		cmodes[i] = 0;
	}
	return;
}

/*************************************************************************/

void init_csmodes(void)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		csmodes[i] = 0;
	}
	for (i = 0; i < 128; i++)
	{
		sjoinmodes[i] = 0;
	}
	return;
}

/*************************************************************************/

void ModuleSetChanUMode(int mode, char letter, int status_flag)
{
	csmodes[letter] = mode;
	sjoinmodes[letter] = status_flag;
	ModuleSetChanMode((int) letter, IRCD_ENABLE);
	return;
}

/*************************************************************************/

int denora_cmode(int m)
{
	return (cmodes[m] ? 1 : 0);
}

/*************************************************************************/

int ChanHasMode(char *chan, int m)
{
	Channel *c;
	char modebuf[16];
	struct c_modes *um;

	c = findchan(chan);
	if (!c)
	{
		return 0;
	}
	if (cmodes[m])
	{
		ircsnprintf(modebuf, sizeof(modebuf), "%c", m);
		for (um = c->modes; um && strcmp(modebuf, um->mode) != 0;
		        um = um->next);
		return (um ? 1 : 0);
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/

void ModuleSetChanMode(int mode, int flag)
{
	if (flag == IRCD_ENABLE)
	{
		cmodes[mode] = flag;
	}
	else if (flag == IRCD_DISABLE)
	{
		cmodes[mode] = flag;
	}
	else
	{
		cmodes[mode] = IRCD_ENABLE;
	}
	return;
}

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
ChanMode *FindChanMode(char *name)
{
	int idx;
	ChanModeHash *privcurrent = NULL;

	if (!name)
	{
		return NULL;
	}
	idx = MODEHASHSENSITVE(name);

	

	for (privcurrent = CHANMODEHANDLERS[idx]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (strcmp(name, privcurrent->mode) == 0)
		{
			return privcurrent->cm;
		}
	}
	return NULL;
}

/*************************************************************************/

void ModuleChanModeUpdate(int mode, void (*setvalue) (Channel * chan, char *value),  char *(*getvalue) (Channel * chan))
{

	ChanMode *cm;
	char *mode_token = ReturnModeFromFlag(mode);

	cm = FindChanMode(mode_token);
	if (cm)
	{
		cm->setvalue = setvalue;     /* Handle                   */
		cm->getvalue = getvalue;
	}
}

ChanUModeArray chanumode_array[] =
{
	{"OP", STATUS_OP},
	{"HALFOP", STATUS_HALFOP},
	{"VOICE", STATUS_VOICE},
	{"OWNER", STATUS_OWNER},
	{"PROTECTED", STATUS_PROTECTED},
	{"SHUNNED", STATUS_SHUNNED},
	{"NONE", STATUS_NONE},
	{NULL, 0}
};


ModeArray ircd_mode_array[] =
{
	{"CMODE_A", CMODE_A},
	{"CMODE_B", CMODE_B},
	{"CMODE_C", CMODE_C},
	{"CMODE_D", CMODE_D},
	{"CMODE_E", CMODE_E},
	{"CMODE_F", CMODE_F},
	{"CMODE_G", CMODE_G},
	{"CMODE_H", CMODE_H},
	{"CMODE_I", CMODE_I},
	{"CMODE_J", CMODE_J},
	{"CMODE_K", CMODE_K},
	{"CMODE_L", CMODE_L},
	{"CMODE_M", CMODE_M},
	{"CMODE_N", CMODE_N},
	{"CMODE_O", CMODE_O},
	{"CMODE_P", CMODE_P},
	{"CMODE_Q", CMODE_Q},
	{"CMODE_R", CMODE_R},
	{"CMODE_S", CMODE_S},
	{"CMODE_T", CMODE_T},
	{"CMODE_U", CMODE_U},
	{"CMODE_V", CMODE_V},
	{"CMODE_W", CMODE_W},
	{"CMODE_X", CMODE_X},
	{"CMODE_Y", CMODE_Y},
	{"CMODE_Z", CMODE_Z},

	{"CMODE_a", CMODE_a},
	{"CMODE_b", CMODE_b},
	{"CMODE_c", CMODE_c},
	{"CMODE_d", CMODE_d},
	{"CMODE_e", CMODE_e},
	{"CMODE_f", CMODE_f},
	{"CMODE_g", CMODE_g},
	{"CMODE_h", CMODE_h},
	{"CMODE_i", CMODE_i},
	{"CMODE_j", CMODE_j},
	{"CMODE_k", CMODE_k},
	{"CMODE_l", CMODE_l},
	{"CMODE_m", CMODE_m},
	{"CMODE_n", CMODE_n},
	{"CMODE_o", CMODE_o},
	{"CMODE_p", CMODE_p},
	{"CMODE_q", CMODE_q},
	{"CMODE_r", CMODE_r},
	{"CMODE_s", CMODE_s},
	{"CMODE_t", CMODE_t},
	{"CMODE_u", CMODE_u},
	{"CMODE_v", CMODE_v},
	{"CMODE_w", CMODE_w},
	{"CMODE_x", CMODE_x},
	{"CMODE_y", CMODE_y},
	{"CMODE_z", CMODE_z},

	{"UMODE_A", UMODE_A},
	{"UMODE_B", UMODE_B},
	{"UMODE_C", UMODE_C},
	{"UMODE_D", UMODE_D},
	{"UMODE_E", UMODE_E},
	{"UMODE_F", UMODE_F},
	{"UMODE_G", UMODE_G},
	{"UMODE_H", UMODE_H},
	{"UMODE_I", UMODE_I},
	{"UMODE_J", UMODE_J},
	{"UMODE_K", UMODE_K},
	{"UMODE_L", UMODE_L},
	{"UMODE_M", UMODE_M},
	{"UMODE_N", UMODE_N},
	{"UMODE_O", UMODE_O},
	{"UMODE_P", UMODE_P},
	{"UMODE_Q", UMODE_Q},
	{"UMODE_R", UMODE_R},
	{"UMODE_S", UMODE_S},
	{"UMODE_T", UMODE_T},
	{"UMODE_U", UMODE_U},
	{"UMODE_V", UMODE_V},
	{"UMODE_W", UMODE_W},
	{"UMODE_X", UMODE_X},
	{"UMODE_Y", UMODE_Y},
	{"UMODE_Z", UMODE_Z},

	{"UMODE_a", UMODE_a},
	{"UMODE_b", UMODE_b},
	{"UMODE_c", UMODE_c},
	{"UMODE_d", UMODE_d},
	{"UMODE_e", UMODE_e},
	{"UMODE_f", UMODE_f},
	{"UMODE_g", UMODE_g},
	{"UMODE_h", UMODE_h},
	{"UMODE_i", UMODE_i},
	{"UMODE_j", UMODE_j},
	{"UMODE_k", UMODE_k},
	{"UMODE_l", UMODE_l},
	{"UMODE_m", UMODE_m},
	{"UMODE_n", UMODE_n},
	{"UMODE_o", UMODE_o},
	{"UMODE_p", UMODE_p},
	{"UMODE_q", UMODE_q},
	{"UMODE_r", UMODE_r},
	{"UMODE_s", UMODE_s},
	{"UMODE_t", UMODE_t},
	{"UMODE_u", UMODE_u},
	{"UMODE_v", UMODE_v},
	{"UMODE_w", UMODE_w},
	{"UMODE_x", UMODE_x},
	{"UMODE_y", UMODE_y},
	{"UMODE_z", UMODE_z},
	{"UMODE_0", UMODE_0},
	{"UMODE_1", UMODE_1},
	{"UMODE_2", UMODE_2},
	{"UMODE_3", UMODE_3},
	{"UMODE_4", UMODE_4},
	{"UMODE_5", UMODE_5},
	{"UMODE_6", UMODE_6},
	{"UMODE_7", UMODE_7},
	{"UMODE_8", UMODE_8},
	{"UMODE_9", UMODE_9},

	{NULL, 0}
};


char *ReturnModeFromFlag(int mode)
{
	int j;


		for (j = 0; ircd_mode_array[j].token; j++)
		{
			if (mode == ircd_mode_array[j].flag)
			{
				return (char *) ircd_mode_array[j].token;
			}
		}

	return NULL;
}

int ReturnModeFromToken(char *tag)
{
	int j;


		for (j = 0; ircd_mode_array[j].token; j++)
		{
			if (tag)
			{
				if (strcmp(tag, ircd_mode_array[j].token) == 0)
				{
					return ircd_mode_array[j].flag;
				}
			}
		}

	return 0;
}


int ReturnChanUModeFromToken(char *tag)
{
	int j;


		for (j = 0; chanumode_array[j].token; j++)
		{
			if (tag)
			{
				if (strcmp(tag, chanumode_array[j].token) == 0)
				{
					return chanumode_array[j].flag;
				}
			}
		}

	return 0;
}


/*************************************************************************/

ChanMode *CreateChanMode(int mode,
                         void (*setvalue) (Channel * chan, char *value),
                         char *(*getvalue) (Channel * chan))
{
	ChanMode *m;
	char modebuf[15];

	if (!mode)
	{
		return NULL;
	}

	

	ircsnprintf(modebuf, sizeof(modebuf), "%c", mode);

	if ((m = malloc(sizeof(ChanMode))) == NULL)
	{
		fatal("Out of memory!");
	}
	m->mode = sstrdup(modebuf); /* Our Name                 */
	m->setvalue = setvalue;     /* Handle                   */
	m->getvalue = getvalue;
	addChanMode(m);
	ModuleSetChanMode(mode, IRCD_ENABLE);
	return m;                   /* return a nice new module */
}

/*************************************************************************/

int addChanMode(ChanMode * m)
{
	int modindex = 0;
	ChanModeHash *privcurrent = NULL;
	ChanModeHash *newHash = NULL;
	ChanModeHash *lastHash = NULL;

	

	modindex = MODEHASHSENSITVE(m->mode);

	for (privcurrent = CHANMODEHANDLERS[modindex]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (stricmp(m->mode, privcurrent->mode) == 0)
			return MOD_ERR_EXISTS;
		lastHash = privcurrent;
	}

	if ((newHash = malloc(sizeof(ChanModeHash))) == NULL)
	{
		fatal("Out of memory");
	}
	newHash->next = NULL;
	newHash->mode = sstrdup(m->mode);
	newHash->cm = m;

	if (lastHash == NULL)
		CHANMODEHANDLERS[modindex] = newHash;
	else
		lastHash->next = newHash;
	return MOD_ERR_OK;
}

/*************************************************************************/

int destroyChanMode(ChanMode * m)
{
	if (!m)
	{
		return MOD_ERR_PARAMS;
	}
	if (m->mode)
		free(m->mode);
	free(m);
	return MOD_ERR_OK;
}

/*************************************************************************/

int delChanMode(ChanMode * m)
{
	int idx = 0;
	ChanModeHash *privcurrent = NULL;
	ChanModeHash *lastHash = NULL;

	if (!m)
	{
		return MOD_ERR_PARAMS;
	}

	idx = CMD_HASH(m->mode);

	for (privcurrent = CHANMODEHANDLERS[idx]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (stricmp(m->mode, privcurrent->mode) == 0)
		{
			if (!lastHash)
			{
				CHANMODEHANDLERS[idx] = privcurrent->next;
			}
			else
			{
				lastHash->next = privcurrent->next;
			}
			destroyChanMode(privcurrent->cm);
			free(privcurrent->mode);
			free(privcurrent);
			return MOD_ERR_OK;
		}
		lastHash = privcurrent;
	}
	return MOD_ERR_NOEXIST;
}

/*************************************************************************/

void SetChanMode(Channel * c, char *mode)
{
	struct c_modes *u;

	u = calloc(sizeof(*u), 1);
	u->next = c->modes;
	if (c->modes)
	{
		c->modes->prev = u;
	}
	c->modes = u;
	u->mode = sstrdup(mode);
	return;
}

/*************************************************************************/

void RemoveChanMode(Channel * c, char *mode)
{
	struct c_modes *u;

	for (u = c->modes; u && stricmp(mode, u->mode) != 0; u = u->next);
	if (u)
	{
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
			c->modes = u->next;
		}
		free(u);
	}
	return;
}

/*************************************************************************/

void ModuleUpdateSQLChanMode(void)
{
	char modebuf[BUFSIZE];
	char *temp = NULL;
	int i = 0;

	if (ircd->cmodes)
		free(ircd->cmodes);

	for (i = 0; i < 128; i++)
	{
		if (cmodes[i])
		{
			if (!temp)
			{
				ircsnprintf(modebuf, sizeof(modebuf), "%c", (char) i);
				temp = sstrdup(modebuf);
			}
			else
			{
				ircsnprintf(modebuf, sizeof(modebuf), "%s%c", temp,
				            (char) i);
				free(temp);
				temp = sstrdup(modebuf);
			}
		}
	}
	if (temp)
	{
		ircd->cmodes = sstrdup(temp);
		free(temp);
	}
	return;
}

/*************************************************************************/


void chan_set_modes(Channel * chan, int ac, char **av)
{
	int add = 1;
	char *modes = sstrdup(av[0]), mode;
	ChanMode *cm;
	ChanBanMode *cbm;
	char modebuf[BUFSIZE];

	alog(LOG_DEBUG, langstr(ALOG_DEBUG_CHANGE_MODE_CHAN), chan->name,
	     merge_args(ac, av));

	ac--;
	

	chan->stats->modecount++;
	chan->stats->modecounttime++;

	while ((mode = *modes++))
	{

		switch (mode)
		{
			case '+':
				add = 1;
				continue;
			case '-':
				add = 0;
				continue;
		}

		if (((int) mode) < 0)
		{
			alog(LOG_DEBUG, langstr(ALOG_DEBUG_BAD_CHAN_MODE), chan->name);
			continue;
		}
		

		ircsnprintf(modebuf, sizeof(modebuf), "%c", mode);

		cm = FindChanMode(modebuf);
		cbm = FindChanBanMode(modebuf);
		if (cm)
		{
			if (add)
				SetChanMode(chan, modebuf);
			else
				RemoveChanMode(chan, modebuf);
			if (add && cm->setvalue)
			{
				if (ac <= 0)
				{
					alog(LOG_ERROR, langstr(ALOG_DEBUG_MODE_NO_PARAM),
					     add ? '+' : '-', mode, chan->name);
					continue;
				}
				ac--;
				av++;
				cm->setvalue(chan, add ? *av : NULL);
				
				if (ac > 0 && *av)
				{
					send_event(EVENT_CHANNEL_MODE, 4,
					           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
					           chan->name, modebuf, *av);
				}
				else
				{
					send_event(EVENT_CHANNEL_MODE, 3,
					           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
					           chan->name, modebuf);
				}
			}
			else
			{
				send_event(EVENT_CHANNEL_MODE, 3,
				           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
				           chan->name, modebuf);
			}
		}
		else if (cbm)
		{
			if (ac <= 0)
			{
				alog(LOG_ERROR, langstr(ALOG_DEBUG_MODE_NO_PARAM),
				     add ? '+' : '-', mode, chan->name);
				continue;
			}
			ac--;
			av++;
			if (add && cbm->addmask)
			{
				cbm->addmask(chan, *av);
			}
			if (!add && cbm->delmask)
			{
				cbm->delmask(chan, *av);
			}
			if (*av)
			{
				send_event(EVENT_CHANNEL_MODE, 4,
				           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
				           chan->name, modebuf, *av);
			}
			else
			{
				send_event(EVENT_CHANNEL_MODE, 3,
				           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
				           chan->name, modebuf);
			}
		}
		else
		{
			/* mode must be one of qaohv and must be ignored */
			if (ac == 0)
			{
				continue;
			}
			ac--;
			av++;
		}
	}

	chan->stats->secret = ChanHasMode(chan->name, CMODE_s);
	chan->stats->private = ChanHasMode(chan->name, CMODE_p);
	return;
}

/*************************************************************************/

/* Returns a fully featured binary modes string. If complete is 0, the
 * eventual parameters won't be added to the string.
 */
char *chan_get_modes(Channel * chan, int complete)
{
	static char modebuf[BUFSIZE];
	struct c_modes *um;
	char *temp = NULL;
	ChanMode *cm;

	


	for (um = chan->modes; um; um = um->next)
	{
		if (!temp)
		{
			ircsnprintf(modebuf, sizeof(modebuf), "%s", um->mode);
			temp = sstrdup(modebuf);
		}
		else
		{
			ircsnprintf(modebuf, sizeof(modebuf), "%s%s", temp, um->mode);
			free(temp);
			temp = sstrdup(modebuf);
		}
	}

	if (complete)
	{
		for (um = chan->modes; um; um = um->next)
		{
			cm = FindChanMode(um->mode);
			if (cm && cm->getvalue)
			{
				ircsnprintf(modebuf, sizeof(modebuf), "%s %s", temp,
				            cm->getvalue);
				free(temp);
				temp = sstrdup(modebuf);
			}
		}
	}

	return temp;
}

/*************************************************************************/

/* adds modes to a given chan */
void sql_do_chanmodes(char *chan, int ac, char **av)
{
	Channel *c;
	int atleastone = 0;
	char db[1000];
	char tmp[14] = "mode_XX=\"X\", ";
	char *modes;
	char *sqlnick;
	char *tokennick;
	int tmpmode;
	int argptr = 1;
	int nickid;
	char *p10nick;
	char *oplevel;
	User *u;

	c = findchan(chan);
	if (!c || c->sqlid < 1)
	{
		return;
	}

	modes = sstrdup(av[0]);
	if (*modes == '0')
	{
		return;
	}

	

	*db = '\0';
	ircsnprintf(db, sizeof(db), "UPDATE %s SET ", ChanTable);

	while (*modes)
	{
		switch (*modes)
		{
			case '+':
				tmp[9] = 'Y';
				break;
			case '-':
				tmp[9] = 'N';
				break;
			default:
				if (!strchr(ircd->cmodes, *modes))
				{
					alog(LOG_DEBUG, langstr(ALOG_DEBUG_UNKNOW_CMODE), *modes);
					alog(LOG_DEBUG, langstr(ALOG_DEBUG_LAST_LINE), inbuf);
					alog(LOG_DEBUG, langstr(ALOG_DEBUG_REPORT_AT));
				}
				else if (*modes == ircd->ban_char)
				{
					argptr++;
				}
				else if (ircd->except && *modes == ircd->except_char)
				{
					argptr++;
				}
				else if (ircd->invitemode && *modes == ircd->invite_char)
				{
					argptr++;
				}
				else if (
						(ircd->owner && *modes == 'q') ||
						(ircd->protect && *modes == 'a') ||
						(ircd->halfop && *modes == 'h') ||
						*modes == 'o' ||
						*modes == 'v'
				        )
				{
					
					if (ircd->p10)
					{
						p10nick = sstrdup(av[argptr++]);
						if (myNumToken(p10nick, ':'))
						{
							tokennick = myStrGetToken(p10nick, ':', 0);
							oplevel = myStrGetTokenRemainder(p10nick, ':', 1);
							sqlnick = sql_escape(tokennick);
							u = user_find(tokennick);
							nickid = db_getnick(sqlnick);
							if (denora->do_sql)
							{
								sql_query("UPDATE %s SET oplevel=%s WHERE chanid=%d and nickid=%d", IsOnTable, oplevel, c->sqlid, nickid);
							}
							free(tokennick);
							free(oplevel);
						}
						else
						{				
							sqlnick = sql_escape(p10nick);
							nickid = db_getnick(sqlnick);
						}
						free(p10nick);
					}
					else 
					{
						sqlnick = sql_escape(av[argptr++]);
						nickid = db_getnick(sqlnick);
					}
					free(sqlnick);
					tmpmode = tolower(*modes);
					if (nickid > 0)
					{
						sql_query(
						          "UPDATE %s SET mode_l%c='%c' WHERE chanid=%d AND nickid=%d",
						          IsOnTable, tmpmode, tmp[9], c->sqlid, nickid);
					}
				}
				else
				{
					atleastone = 1;
					tmp[5] = ((*modes >= 'a') ? 'l' : 'u');
					tmp[6] = tolower(*modes);
					ircsnprintf(&db[strlen(db)], sizeof(db), "%s", tmp);
					if (*modes == 'k')
					{
						
						if (tmp[9] == 'Y' && argptr < ac)
						{
							char *key = sql_escape(av[argptr++]);
							ircsnprintf(&db[strlen(db)], sizeof(db), 
							            "mode_lk_data='%s', ",
							            (HidePasswords ? "HIDDEN" : key));
							free(key);
						}
						else
						{
							ircsnprintf(&db[strlen(db)], sizeof(db),
							            "mode_lk_data='', ");
							argptr++;       /* mode -k needs a parameter */
						}
					}
					else if (ircd->Lmode && *modes == ircd->chanforward)
					{
						
						if (tmp[9] == 'Y' && argptr < ac)
						{
							char *ch = sql_escape(av[argptr++]);
							ircsnprintf(&db[strlen(db)], sizeof(db), 
							            "mode_%s%c_data='%s', ",
							            (ircd->chanforward <= 90 ? "u" : "l"),
							            ircd->chanforward, ch);
							free(ch);
						}
						else
						{
							ircsnprintf(&db[strlen(db)], sizeof(db),
							            "mode_%s%c_data='', ",
							            (ircd->chanforward <= 90 ? "u" : "l"),
							            ircd->chanforward);
						}
					}
					else if (
							(*modes == 'l') ||
							(ircd->fmode && ircd->floodchar && *modes == ircd->floodchar) ||
							(ircd->jmode && ircd->floodchar_alternative && *modes == ircd->floodchar_alternative) ||
							(ircd->jointhrottle && *modes == ircd->jointhrottle) ||
							(ircd->nickchgfloodchar && *modes == ircd->nickchgfloodchar)
						)
					{
						
						tmpmode = tolower(*modes);
						if (tmp[9] == 'Y' && argptr < ac)
						{
							ircsnprintf(&db[strlen(db)], sizeof(db),
							            "mode_%c%c_data='%s', ",
							            tmp[5], tmpmode, av[argptr++]);
						}
						else
						{
							ircsnprintf(&db[strlen(db)], sizeof(db), 
							            "mode_%c%c_data='', ",
							            tmp[5], tmpmode);
						}
					}
				}
				break;
		}
		modes++;
	}
	if (atleastone)
	{
		ircsnprintf(&db[strlen(db) - 2], sizeof(db), " WHERE chanid=%d", c->sqlid);
		sql_query(db);
	}
	return;
}

/*************************************************************************/

char *get_flood(Channel * chan)
{
	
	return chan ? chan->flood : NULL;
}

/*************************************************************************/

char *get_flood_alt(Channel * chan)
{
	
	return chan ? chan->flood_alt : NULL;
}

/*************************************************************************/

char *get_key(Channel * chan)
{
	
	return chan ? chan->key : NULL;
}

/*************************************************************************/

char *get_limit(Channel * chan)
{
	if (!chan || chan->limit == 0)
	{
		return NULL;
	}
	return itostr(chan->limit);
}

/*************************************************************************/

char *get_rejoinlock(Channel * chan)
{
	if (!chan || chan->rejoinlock == 0)
	{
		return NULL;
	}
	return itostr(chan->rejoinlock);
}

/*************************************************************************/

char *get_nickchgflood(Channel * chan)
{
	
	return chan ? chan->nickchgflood : NULL;
}

/*************************************************************************/

char *get_redirect(Channel * chan)
{
	
	return chan ? chan->redirect : NULL;
}

/*************************************************************************/

void set_flood(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	if (chan->flood)
		free(chan->flood);
	chan->flood = (!BadPtr(value) ? sstrdup(value) : NULL);

	alog(LOG_DEBUG, langstr(ALOG_DEBUG_FLOOD_MODE),
	     chan->name, (chan->flood ? chan->flood : langstr(ALOG_NO_FLOOD)));

	return;
}

/*************************************************************************/

void set_flood_alt(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	if (chan->flood_alt)
		free(chan->flood_alt);
	chan->flood_alt = (!BadPtr(value) ? sstrdup(value) : NULL);

	alog(LOG_DEBUG,
	     "debug: Alternative Flood mode for channel %s set to %s",
	     chan->name,
	     (chan->flood_alt ? chan->flood_alt : langstr(ALOG_NO_FLOOD)));

	return;
}

/*************************************************************************/

void set_key(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	if (chan->key)
		free(chan->key);
	chan->key = (!BadPtr(value) ? sstrdup(value) : NULL);

	alog(LOG_DEBUG, langstr(ALOG_KEY_SET_TO), chan->name,
	     (chan->key ? chan->key : langstr(ALOG_NO_KEY)));

	return;
}

/*************************************************************************/

void set_limit(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	chan->limit = (!BadPtr(value) ? strtoul(value, NULL, 10) : 0);

	alog(LOG_DEBUG, langstr(ALOG_LIMIT_SET_TO), chan->name, chan->limit);

	return;
}

/*************************************************************************/

void set_rejoinlock(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	chan->rejoinlock = (!BadPtr(value) ? strtoul(value, NULL, 10) : 0);

	alog(LOG_DEBUG, "debug: Rejoin lock for channel %s set to %u",
	     chan->name, chan->rejoinlock);

	return;
}

/*************************************************************************/

void set_nickchgflood(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	if (chan->nickchgflood)
		free(chan->nickchgflood);
	chan->nickchgflood = (!BadPtr(value) ? sstrdup(value) : NULL);

	alog(LOG_DEBUG, "debug: Nick change flood for %s set to %s",
	     chan->name, chan->nickchgflood);

	return;
}

/*************************************************************************/

void set_redirect(Channel * chan, char *value)
{
	

	if (!chan)
	{
		return;
	}

	if (chan->redirect)
		free(chan->redirect);
	chan->redirect = (!BadPtr(value) ? sstrdup(value) : NULL);

	alog(LOG_DEBUG, langstr(ALOG_REDIRECT_SET_TO), chan->name,
	     (chan->redirect ? chan->redirect : langstr(ALOG_NO_REDIRECT)));

	return;
}

/*************************************************************************/
