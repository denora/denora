/* Network Bans - Modules
 * 
 * Spamfilters - loads from the spamfilter module can be called via thei struct
 *  Spamfilter.add()
 *  Spamfilter.del()
 *  Spamfilter.find()
 *  Spamfilter.fini()
 *  spamfilter.expired()
 *
 * SQlines - load from sqline module and are added/del from their struct
 *  sqline.parse()
 *
 *
 * SGlines - load from sqline module and are added/del from their struct
 *  sgline.parse()
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

typedef struct spammod_
{
	void (*add)(char *target, char *action, char *setby, char *expires, char *setat, char *duration, char *reason, char *regex);
	void (*del)(char *target, char *action, char *regex);
	SpamFilter *(*find)(const char *regex);
	void (*fini)(SpamFilter *sf);
	void (*expired)(void);
} SpamFilterMod;

SpamFilterMod spamfilter;

typedef struct sqline_
{
	void (*parse)(char *mask, char *reason);
} SqlineMod;

SqlineMod sqline;

typedef struct sgline_
{
	void (*parse)(char *mask, char *reason);
} SglineMod;

SglineMod sgline;


typedef struct xline_
{
	void (*add)(char *mask, char *reason);
	void (*del)(char *mask);
} xlineMod;

xlineMod xline;

void init_NetworkBansMod(void)
{
	spamfilter.add = NULL;
	spamfilter.del = NULL;
	spamfilter.find = NULL;
	spamfilter.fini = NULL;
	spamfilter.expired = NULL;
	sqline.parse = NULL;
	sgline.parse = NULL;
	xline.add = NULL;
	xline.del = NULL;
}

/*************************************************************************/

void SpamFilter_Add_Handler(void (*func) (char *target, char *action, char *setby, char *expires, char *setat, char *duration, char *reason, char *regex))
{
	spamfilter.add = func;
}

void SpamFilter_Del_Handler(void (*func) (char *target, char *action, char *regex))
{
	spamfilter.del = func;
}

void SpamFilter_Find_Handler(SpamFilter *(*func)(const char *regex))
{
	spamfilter.find = func;
}

void SpamFilter_Fini_Handler(void (*func) (SpamFilter *sf))
{
	spamfilter.fini = func;
}

void SpamFilter_Expired_Handler(void (*func) (void))
{
	spamfilter.expired = func;
}

/*************************************************************************/

void HandleExpiredSpamfilters(void)
{
	if (ircd->spamfilter)
	{
		spamfilter.expired();
	}
}

void SQline_Parse_Handler(void (*func) (char *mask, char *reason))
{
	sqline.parse = func;
}

void SGline_Parse_Handler(void (*func) (char *mask, char *reason))
{
	sgline.parse = func;
}


void xline_add_Handler(void (*func) (char *mask, char *reason))
{
	xline.add = func;
}

void xline_del_Handler(void (*func) (char *mask))
{
	xline.del = func;
}