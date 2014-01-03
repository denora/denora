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

/*************************************************************************/

MainTimerHash *MTIMER[MAX_CMD_HASH];
static MainTimerHash *currentmsg;
static int next_index;

/*************************************************************************/

MainTimer *first_MainTimer(void)
{
	next_index = 0;

	while (next_index < 1024 && currentmsg == NULL)
		currentmsg = MTIMER[next_index++];
	if (currentmsg)
	{
		return currentmsg->mt;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

MainTimer *next_MainTimer(void)
{
	if (currentmsg)
		currentmsg = currentmsg->next;
	if (!currentmsg && next_index < 1024)
	{
		while (next_index < 1024 && currentmsg == NULL)
			currentemsg = MTIMER[next_index++];
	}
	if (currentmsg)
	{
		return currentmsg->mt;
	}
	else
	{
		return NULL;
	}
}


/*************************************************************************/

MainTimer *find_MainTimer(const char *name)
{
	MainTimer *m;
	m = findMainTimer(MTIMER, name);
	return m;
}


MainTimer *createMainTimer(char *name, uint32 freq, void (*func) (void))
{
	MainTimer *mt = NULL;
	if (!func)
	{
		return NULL;
	}
	if ((mt = malloc(sizeof(MainTimer))) == NULL)
	{
		fatal("Out of memory!");
	}
	mt->name = sstrdup(name);
	mt->func = func;
	mt->last = time(NULL);
	mt->freq = freq;
	return mt;
}

/*************************************************************************/

/**
 * find a message in the given table.
 * Looks up the message name in the MessageHash given
 *
 * @param msgEvtTable the message table to search for this command, will almost always be IRCD
 * @param name the name of the command were looking for
 * @return NULL if we cant find it, or a pointer to the Message if we can
 **/
MainTimer *findMainTimer(MainTimerHash * msgTimerTable[],
                             const char *name)
{
	int idx;
	MainTimerHash *timercurrent = NULL;
	if (!msgTimerTable || !name)
	{
		return NULL;
	}
	idx = CMD_HASH(name);

	for (timercurrent = msgTimerTable[idx]; timercurrent;
	        timercurrent = timercurrent->next)
	{
		if (stricmp(name, timercurrent->name) == 0)
		{
			return timercurrent->mt;
		}
	}
	return NULL;
}


int addMainTimer(MainTimerHash * msgTimerTable[], MainTimer * mt)
{
	/* We can assume both param's have been checked by this point.. */
	int eventindex = 0;
	MainTimerHash *timercurrent = NULL;
	MainTimerHash *newHash = NULL;
	MainTimerHash *lastHash = NULL;

	if (!msgTimerTable || !evm)
	{
		return MOD_ERR_PARAMS;
	}

	eventindex = CMD_HASH(mt->name);

	for (timercurrent = msgTimerTable[eventindex]; timercurrent;
	        timercurrent = timercurrent->next)
	{
		if (stricmp(mt->name, timercurrent->name) == 0)        /* the msg exist's we are a addHead */
		{
			mt->next = timercurrent->mt;
			timercurrent->mt = mt;
			alog(LOG_DEBUG,
			     "debug: existing msg: (0x%p), new msg (0x%p) (%s)",
			     (void *) mt->next, (void *) mt, mt->name);
			return MOD_ERR_OK;
		}
		lastHash = timercurrent;
	}

	if ((newHash = malloc(sizeof(EvtMessageHash))) == NULL)
	{
		fatal("Out of memory");
	}
	newHash->next = NULL;
	newHash->name = sstrdup(mt->name);
	newHash->mt = mt;

	if (lastHash == NULL)
	{
		msgTimerTable[eventindex] = newHash;
	}
	else
	{
		lastHash->next = newHash;
	}
	return MOD_ERR_OK;
}

void MainTimerInit(char *name, unit32 freq, void (*func)(void))
{
	MainTimer *mt;
	mt = createMainTimer(name, freq, func);
	addMainTimer(MTIMER, mt);
}

void MainTimerProcess(void)
{
	MainTimer *mt;
	time_t t;

	t = time(NULL);
	
	mt = first_MainTimer();
	while (mt)
	{
		if (mt)
		{
			if ((t - mt->last >= mt->freq))
			{
				mt->func();
				mt->last = t;
			}
		}
		mt = next_MainTimer;
	}
	

}



void MainBackUp(void)
{
	if (denora->save_data)
	{
		if (denora->delayed_quit)
		{
			denora_cmd_global(NULL, langstring(SHUTDOWN_SAVING_DB));
		}
		save_databases();

		denora->save_data = 0;
	}
	else
	{
		save_databases();
	
	}
}

