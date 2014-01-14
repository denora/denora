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

Uid *UIDhead;
#define DLLIST_COMPARATOR(e1, e2) (strcmp(e1->uid, e2->uid))
	SGLIB_DEFINE_DL_LIST_PROTOTYPES(Uid, DLLIST_COMPARATOR, prev, next);
	SGLIB_DEFINE_DL_LIST_FUNCTIONS(Uid, DLLIST_COMPARATOR, prev, next);


/*************************************************************************/

void init_uids(void)
{
	SET_SEGV_LOCATION();

	alog(LOG_EXTRADEBUG,"Init uid link list");

	UIDhead = NULL;
}

/*************************************************************************/

Uid *new_uid(const char *nick, char *uid)
{
	Uid *u, te;

	alog(LOG_EXTRADEBUG, "new_uid for %s -> %s", nick, uid);

    u = malloc(sizeof(Uid));
   	u->uid = sstrdup(uid);
	u->nick = sstrdup(nick);
   	sglib_Uid_add(&UIDhead, u);
   	sglib_Uid_sort(&UIDhead);
	alog(LOG_EXTRADEBUG, "Added nick %s", u->nick);
	return u;
}

/*************************************************************************/

User *find_byuid(const char *uid)
{
	Uid *u, te, *memb;
	
	alog(LOG_EXTRADEBUG, "find_byuid(%s)", uid);

	te.uid = (char *) uid;
	if (sglib_Uid_find_member(UIDhead, &te))
	{
		SGLIB_DL_LIST_FIND_MEMBER(Uid, UIDhead, &te, DLLIST_COMPARATOR, prev, next, memb);
		if (memb)
		{
			alog(LOG_EXTRADEBUG, "Memb -> uid %s nick %s", memb->uid, memb->nick);
			return finduser(memb->nick);
		}
	}
	alog(LOG_EXTRADEBUG, "Did not find %s", uid);

	return NULL;
}

/*************************************************************************/

void delete_uid(Uid * u)
{
	alog(LOG_EXTRADEBUG, "delete_uid(%s)", u->uid);

	sglib_Uid_delete(&UIDhead, u);
	if (u->uid)
		free(u->uid);
	if (u->nick)
		free(u->nick);
	free(u);
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
	Uid *u;
	
	for(u=sglib_Uid_get_first(UIDhead); u != NULL; u=u->next)
	{
		if (u && !strcmp(u->nick, nick))
		{
			return u;
			break;
		}
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
	Uid *u;
	
	alog(LOG_EXTRADEBUG, "find_nickuid(%s)", uid);

	for(u=sglib_Uid_get_first(UIDhead); u != NULL; u=u->next)
	{
		if (u && !strcmp(u->uid, uid))
		{
			return u;
			break;
		}
	}
	alog(LOG_DEBUG, "find_nickuid(%s) -> null", uid);
	return NULL;
}

char *p10_uid_gen(void);

char *uid_gen(void)
{
	if (ircd->ts6 && UseTS6)
	{
		return ts6_uid_retrieve();
	}
	else if (ircd->p10)
	{
		return p10_uid_gen();
	}
	else
	{
		return NULL;
	}
}

static int p10nickcnt;
static char nicknumbuf[6];

char *p10_uid_gen(void)
{
	
	ircsnprintf(nicknumbuf, 6, "%sAA%c", p10id, (p10nickcnt + 'A'));
	p10nickcnt++;
	return  (char *) nicknumbuf;
}

/*************************************************************************/

/* TS6 UID generator common code.
 *
 * Derived from atheme-services, uid.c (hg 2954:116d46894b4c).
 *         -nenolod
 */
static int ts6_uid_initted = 0;
static char ts6_new_uid[10];    /* allow for \0 */
static unsigned int ts6_uid_index = 9;  /* last slot in uid buf */

void ts6_uid_init(void)
{
	/* check just in case... you can never be too safe. */
	if (TS6SID != NULL)
	{
		ircsnprintf(ts6_new_uid, 10, "%sAAAAAA", TS6SID);
		ts6_uid_initted = 1;
	}
	else
	{
		alog(LOG_NORMAL,
		     "warning: no TS6SID specified, disabling TS6 support.");
		UseTS6 = 0;
		return;
	}
}

void ts6_uid_increment(unsigned int slot)
{
	if (slot != strlen(TS6SID))
	{
		if (ts6_new_uid[slot] == 'Z')
			ts6_new_uid[slot] = '0';
		else if (ts6_new_uid[slot] == '9')
		{
			ts6_new_uid[slot] = 'A';
			ts6_uid_increment(slot - 1);
		}
		else
			ts6_new_uid[slot]++;
	}
	else
	{
		if (ts6_new_uid[slot] == 'Z')
			for (slot = 3; slot < 9; slot++)
				ts6_new_uid[slot] = 'A';
		else
			ts6_new_uid[slot]++;
	}
}

char *ts6_uid_retrieve(void)
{
	if (ts6_uid_initted != 1)
		ts6_uid_init();

	ts6_uid_increment(ts6_uid_index - 1);

	return ts6_new_uid;
}
