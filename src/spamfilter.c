
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

list_t *Spamhead;

static int findspamfilter(const void *v, const void *cc)
{
	const SpamFilter *t = (void *) v;
	return (stricmp(t->regex, (char *) cc));
}

/*************************************************************************/

void init_spamfilter(void)
{
	SpamFilter *t;
	SET_SEGV_LOCATION();

	Spamhead = list_create(-1);
	t = malloc(sizeof(SpamFilter));
	bzero(t, sizeof(SpamFilter));
}

/*************************************************************************/

/**
 * Create a new entry in the TLD structs
 *
 * @param country is the two letter code for the country
 * @return TLD struct
 *
 */
SpamFilter *new_SpamFilter(char *target, char *action,
                           char *setby, char *expires, char *setat,
                           char *duration, char *reason, char *regex)
{
	lnode_t *tn;
	SpamFilter *sf = NULL;
	SET_SEGV_LOCATION();

	tn = list_find(Spamhead, regex, findspamfilter);
	if (tn)
	{
		sf = lnode_get(tn);
	}
	else
	{
		sf = malloc(sizeof(SpamFilter));
		sf->regex = sstrdup(regex);
		sf->target = sstrdup(target);
		sf->action = sstrdup(action);
		sf->setby = sstrdup(setby);
		sf->expires = sstrdup(expires);
		sf->setat = strtoul(setat, NULL, 10);
		sf->duration = strtoul(duration, NULL, 10);
		sf->reason = sstrdup(reason);
		tn = lnode_create(sf);
		list_append(Spamhead, tn);
	}
	return sf;
}

/*************************************************************************/

SpamFilter *findSpamFilter(const char *regex)
{
	lnode_t *tn;
	SpamFilter *t = NULL;

	tn = list_find(Spamhead, regex, findspamfilter);
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

void fini_SpamFilter(void)
{
	SpamFilter *sf;
	lnode_t *tn;

	tn = list_first(Spamhead);
	while (tn != NULL)
	{
		sf = lnode_get(tn);
		if (sf->regex)
			free(sf->regex);
		if (sf->target)
			free(sf->target);
		if (sf->action)
			free(sf->action);
		if (sf->setby)
			free(sf->setby);
		if (sf->expires)
			free(sf->expires);
		if (sf->reason)
			free(sf->reason);
		free(sf);
		tn = list_next(Spamhead, tn);
	}
	list_destroy_nodes(Spamhead);
}
