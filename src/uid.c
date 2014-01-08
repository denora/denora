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

list_t *UIDhead;

static int find_check_uid(const void *v, const void *cc)
{
	const Uid *t = (void *) v;
	return (stricmp(t->uid, (char *) cc));
}

static int find_check_nick(const void *v, const void *cc)
{
	const Uid *t = (void *) v;
	return (stricmp(t->nick, (char *) cc));
}



/*************************************************************************/

void init_uids(void)
{
	Uid *u;
	SET_SEGV_LOCATION();

	alog(LOG_DEBUG,"Init uid link list");

	UIDhead = list_create(-1);
	u = malloc(sizeof(Uid));
	bzero(u, sizeof(Uid));
}

/*************************************************************************/

Uid *new_uid(const char *nick, char *uid)
{
	lnode_t *tn;
	Uid *u = NULL;
	SET_SEGV_LOCATION();

	alog(LOG_DEBUG, "adding uid %s for %s", uid, nick);

	if (UIDhead)
	{
		tn = list_find(UIDhead, uid, find_check_uid);
		if (tn)
		{
			u = lnode_get(tn);
		}
		else
		{
			u = malloc(sizeof(Uid));
			u->uid = sstrdup(uid);
			u->nick = sstrdup(nick);
			tn = lnode_create(u);
			list_append(UIDhead, tn);
		}
	}
	else
	{
			u = malloc(sizeof(Uid));
			u->uid = sstrdup(uid);
			u->nick = sstrdup(nick);
			tn = lnode_create(u);
			list_append(UIDhead, tn);

	}
	return u;
}

/*************************************************************************/

User *find_byuid(const char *uid)
{
	lnode_t *tn;
	Uid *t = NULL;
	User *u;

	if (UIDhead)
	{
		tn = list_find(UIDhead, uid, find_check_uid);
		if (tn)
		{
			t = lnode_get(tn);
			u = finduser(t->nick);
			return u;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

/*************************************************************************/

void delete_uid(Uid * u)
{
	Uid *temp;
	lnode_t *tn;

	if (UIDhead)
	{
		tn = list_first(UIDhead);
		while (tn != NULL)
		{
			temp = lnode_get(tn);
			if (!stricmp(u->uid, temp->uid))
			{
				if (temp->nick)
					free(temp->nick);
				if (temp->uid)
					free(temp->uid);
				free(temp);
				break;
			}
			tn = list_next(UIDhead, tn);
		}
	}
	return;
}

/*************************************************************************/

/**
 * Find the uid struct for the given user nick
 *
 * @param nick is the user to look for
 *
 * @return uid struct
 *
 */
Uid *find_uid(const char *nick)
{
	lnode_t *tn;
	Uid *t = NULL;

	if (UIDhead)
	{
		tn = list_find(UIDhead, nick, find_check_nick);
		if (tn)
		{
			t = lnode_get(tn);
			return t;
		}
		else
		{
			return NULL;
		}
		return NULL;
	}
	return NULL;
}

/*************************************************************************/

/**
 * Find the uid struct for the given uid
 *
 * @param uid is the user id to look for
 *
 * @return uid struct
 *
 */
Uid *find_nickuid(const char *uid)
{
	lnode_t *tn;
	Uid *t = NULL;

	tn = list_find(UIDhead, uid, find_check_nick);
	if (tn)
	{
		t = lnode_get(tn);
		return t;
	}
	else
	{
		return NULL;
	}
	return NULL;
}




