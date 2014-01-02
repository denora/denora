/* User Modes - read MODES for complete details on how Denora
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

UserModeHash *USERMODEHANDLERS[MAX_CMD_HASH];
unsigned long umodes[128];

/*************************************************************************/

/* This is purely so that the Array is set to all 0 before
   we begin to loop it and check for the data
*/
void init_umodes(void)
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		umodes[i] = 0;
	}
}

/*************************************************************************/

int denora_umode(int m)
{
	return (umodes[m] ? 1 : 0);
}

/*************************************************************************/

/**
 * Search the list of loaded modules for the given name.
 * @param name the name of the module to find
 * @return a pointer to the module found, or NULL
 */
UserMode *FindUserMode(char *name)
{
	int idx;
	UserModeHash *privcurrent = NULL;

	if (!name)
	{
		return NULL;
	}
	idx = CMD_HASH(name);

	

	for (privcurrent = USERMODEHANDLERS[idx]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (stricmp(name, privcurrent->mode) == 0)
		{
			return privcurrent->m;
		}
	}
	return NULL;
}

/*************************************************************************/

UserMode *CreateUserModeCallBack(char *mode, int extra,
                                 void (*func) (int ac, char **av))
{
	UserMode *m;

	if (!mode)
	{
		return NULL;
	}

	

	if ((m = malloc(sizeof(UserMode))) == NULL)
	{
		fatal("Out of memory!");
	}
	m->mode = sstrdup(mode);    /* Our Name                 */
	m->set = func;              /* Handle                   */
	m->extra = extra;
	addUserModeCallBack(m);
	return m;                   /* return a nice new module */
}

/*************************************************************************/

int addUserModeCallBack(UserMode * m)
{
	int modindex = 0;
	UserModeHash *privcurrent = NULL;
	UserModeHash *newHash = NULL;
	UserModeHash *lastHash = NULL;

	

	modindex = CMD_HASH(m->mode);

	for (privcurrent = USERMODEHANDLERS[modindex]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (stricmp(m->mode, privcurrent->mode) == 0)
			return MOD_ERR_EXISTS;
		lastHash = privcurrent;
	}

	if ((newHash = malloc(sizeof(UserModeHash))) == NULL)
	{
		fatal("Out of memory");
	}
	newHash->next = NULL;
	newHash->mode = sstrdup(m->mode);
	newHash->m = m;

	if (lastHash == NULL)
		USERMODEHANDLERS[modindex] = newHash;
	else
		lastHash->next = newHash;
	return MOD_ERR_OK;
}

/*************************************************************************/

int destroyUserMode(UserMode * m)
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

int delUserMode(UserMode * m)
{
	int idx = 0;
	UserModeHash *privcurrent = NULL;
	UserModeHash *lastHash = NULL;

	if (!m)
	{
		return MOD_ERR_PARAMS;
	}

	idx = CMD_HASH(m->mode);

	for (privcurrent = USERMODEHANDLERS[idx]; privcurrent;
	        privcurrent = privcurrent->next)
	{
		if (stricmp(m->mode, privcurrent->mode) == 0)
		{
			if (!lastHash)
			{
				USERMODEHANDLERS[idx] = privcurrent->next;
			}
			else
			{
				lastHash->next = privcurrent->next;
			}
			destroyUserMode(privcurrent->m);
			free(privcurrent->mode);
			free(privcurrent);
			return MOD_ERR_OK;
		}
		lastHash = privcurrent;
	}
	return MOD_ERR_NOEXIST;
}

/*************************************************************************/

void SetUserMode(User * user, char *mode)
{
	struct u_modes *u;

	u = calloc(sizeof(*u), 1);
	u->next = user->modes;
	if (user->modes)
	{
		user->modes->prev = u;
	}
	user->modes = u;
	u->mode = sstrdup(mode);
}

/*************************************************************************/

void RemoveUserMode(User * user, char *mode)
{
	struct u_modes *u;

	if (!user || BadPtr(mode))
	{
		return;
	}

	for (u = user->modes; u && stricmp(mode, u->mode) != 0; u = u->next);
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
			user->modes = u->next;
		}
		free(u);
	}
}

/*************************************************************************/

void ModuleSetUserMode(int mode, int flag)
{
	if (flag == IRCD_DISABLE)
	{
		umodes[mode] = flag;
	}
	else
	{
		umodes[mode] = IRCD_ENABLE;
	}
}

/*************************************************************************/

int UserHasMode(char *user, int m)
{
	User *u;
	char modebuf[16];
	struct u_modes *um;

	u = user_find(user);
	if (!u)
	{
		return 0;
	}
	if (umodes[m])
	{
		ircsnprintf(modebuf, sizeof(modebuf), "%c", m);
		for (um = u->modes; um && strcmp(modebuf, um->mode) != 0;
		        um = um->next);
		return (um ? 1 : 0);
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/

void handle_ircop(int ac, char **av)
{
	User *u;

	if (ac >= 1)
	{
		u = user_find(av[1]);
		if (u)
		{
			if (!stricmp(av[0], MODE_ADD))
			{
				add_oper_count(u);
			}
			if (!stricmp(av[0], MODE_REMOVE))
			{
				del_oper_count(u);
			}
		}
	}
}

/*************************************************************************/

/**
 * Handle User MODE changes
 *
 * @param user is the struct to update
 * @param ac is the array count
 * @param av is the array
 *
 * @return void - no returend value
 *
 */
void denora_set_umode(User * user, int ac, char **av)
{
	int add = 1;                	/* 1 if adding modes, 0 if deleting */
	int was_oper = is_oper(user);	/* usefull to attempt to prevent double oper counting */
	char *modes = av[0];
	char modebuf[BUFSIZE];
	char *newav[127];
	UserMode *um;
	int modeready = 0;

	ac--;

	if (!user || !modes)
	{
		/* Prevent NULLs from doing bad things */
		return;
	}

	alog(LOG_DEBUG, "debug: Changing mode for %s to %s", user->nick, modes);

	while (*modes)
	{
		switch (*modes)
		{
			case '+':
				add = 1;
				newav[0] = (char *) MODE_ADD;
				newav[1] = sstrdup(user->nick);
				modeready = 1;
				break;
			case '-':
				add = 0;
				newav[0] = (char *) MODE_REMOVE;
				newav[1] = sstrdup(user->nick);
				modeready = 1;
				break;
			default:
				ircsnprintf(modebuf, sizeof(modebuf), "%c", *modes);
				if (modeready)
				{
					if (add)
					{
						SetUserMode(user, modebuf);
					}
					else
					{
						RemoveUserMode(user, modebuf);
					}
					um = FindUserMode(modebuf);
					if (um)
					{
						if (um->set)
						{
							if (um->extra)
							{
								ac--;
								av++;
								newav[2] = sstrdup(*av);
								um->set(3, newav);
								send_event(EVENT_USER_MODE, 4,
								           (add ? EVENT_MODE_ADD :
								            EVENT_MODE_REMOVE), user->nick,
								           modebuf, *av);
								free(newav[2]);
							}
							else
							{
								/* Discard double (de)oper */
								if (strcmp(um->mode,"o") && ((was_oper && add) || (!was_oper && !add)))
									continue;
								um->set(2, newav);
								send_event(EVENT_USER_MODE, 3,
								           (add ? EVENT_MODE_ADD :
								            EVENT_MODE_REMOVE), user->nick,
								           modebuf);
							}
						}
					}
					else
					{
						send_event(EVENT_USER_MODE, 3,
						           (add ? EVENT_MODE_ADD : EVENT_MODE_REMOVE),
						           user->nick, modebuf);
					}
				}
		}
		(void) *modes++;
	}
	if (newav[1])
		free(newav[1]);
	send_event(EVENT_POST_MODE, 1, user->nick);
}

/*************************************************************************/

void ModuleUpdateSQLUserMode(void)
{
	char modebuf[BUFSIZE];
	char *temp = NULL;
	int i = 0;

	if (ircd->usermodes)
		free(ircd->usermodes);
	for (i = 0; i < 128; i++)
	{
		if (umodes[i])
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
				if (temp)
					free(temp);
				temp = sstrdup(modebuf);
			}
		}
	}
	if (temp)
	{
		ircd->usermodes = sstrdup(temp);
		free(temp);
	}
}

/*************************************************************************/

void denora_automode(char *chan)
{
	char *ud = find_uid(s_StatServ);
        char *modes = sstrdup(AutoMode);
        char nickbuf[BUFSIZE];
        *nickbuf = '\0';

	while (*modes)
	{
		switch (*modes)
		{
			case '+':
				break;
			case '-':
				break;
			default:
				ircsnprintf(nickbuf, BUFSIZE, "%s %s", nickbuf, ((ircd->p10 || UseTS6) && ud) ? ud : s_StatServ);
		}
		(void) *modes++;
	}
	denora_cmd_mode(ServerName, chan, "%s%s", AutoMode, nickbuf);
}
