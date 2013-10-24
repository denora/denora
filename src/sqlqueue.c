/*
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

#ifdef USE_THREADS
#ifndef _WIN32
deno_mutex_t queuemut = PTHREAD_MUTEX_INITIALIZER;
deno_cond_t queuecond = PTHREAD_COND_INITIALIZER;
#else
deno_mutex_t queuemut;
deno_cond_t queuecond;
#endif

QueueEntry *qp;
int qp_job_count;
int qp_exec_count;
int qp_total;

/*************************************************************************/

int QueueEntryInit(void)
{
	deno_thread_t th;

	if (UseThreading)
	{
		if (deno_thread_create(th, queue_thread_main, NULL))
		{
			return 0;
		}
		if (deno_thread_detach(th))
		{
			return 0;
		}
		alog(LOG_DEBUG, "debug: Creating Queue thread 0x%x",
		     th);
#ifdef USE_MYSQL
		db_mysql_init(1);
#endif
	}

	alog(LOG_DEBUG, "Queue Thread initialized");
	return 1;
}

/*************************************************************************/

void queue_unlock(void *arg)
{
	USE_VAR(arg);
	alog(LOG_EXTRADEBUG, "debug: Thread 0x%x: Unlocking queue mutex",
	     deno_thread_self());
	deno_mutex_unlock(queuemut);
}

/*************************************************************************/

void queue_lock(void)
{
	alog(LOG_EXTRADEBUG, "debug: Thread 0x%x: Locking proxy queue mutex",
	     deno_thread_self());
	deno_mutex_lock(queuemut);
}

/*************************************************************************/

void queue_wait(void)
{
	alog(LOG_EXTRADEBUG, "debug: Thread 0x%x: waiting proxy queue condition",
	     deno_thread_self());
	deno_cond_wait(queuecond, queuemut);
}

/*************************************************************************/

void queue_signal(void)
{
	alog(LOG_EXTRADEBUG, "debug: Thread 0x%x: Signaling proxy queue condition",
	     deno_thread_self());
	deno_cond_signal(queuecond);
}

/*************************************************************************/

void *queue_thread_main(void *arg)
{
	USE_VAR(arg);

	while (1)
	{
		deno_cleanup_push(queue_unlock, NULL);
		queue_lock();
		queue_wait();
		deno_cleanup_pop(1);

		while (1)
		{
			deno_cleanup_push(queue_unlock, NULL);
			queue_lock();
			deno_cleanup_pop(1);

			while (qp)
			{
				qp = ExecuteQueue(qp);
			}
			break;
		}
	}
	return NULL;
}

/***************************************************************************************/

QueueEntry *AddQueueEntry(QueueEntry * qep, char *sqlmsg)
{
	QueueEntry *lp = qep, *qtmp;

	alog(LOG_DEBUG, "Threading Adding Query %s", sqlmsg);
	alog(LOG_DEBUG, "Threading Stats: %d Jobs - %d Total - %d Executed", qp_job_count, qp_total, qp_exec_count);

	queue_lock();

	/* Create a queue entry */
	qtmp = (QueueEntry *) malloc(sizeof(QueueEntry));
	qtmp->link = NULL;
	qtmp->msg = sstrdup(sqlmsg);

	qp_job_count++;
	qp_total++;

	/* Find the last member of the queue */
	if (qep)
	{
		while (qep && qep->link)
		{
			alog(LOG_DEBUG,"Checking qep 0x%x : link 0x%x", qep, qep->link);
			qep = qep->link;
		}

		qep->link = qtmp;
	}
	else
	{
		lp = qtmp;
	}

	queue_signal();
	queue_unlock(NULL);
	return lp;
}

/***************************************************************************************/

QueueEntry *RemoveQueueEntry(QueueEntry * qep)
{
	QueueEntry *tempp = qep->link;

	alog(LOG_EXTRADEBUG, "Removing Queue entry data");
	alog(LOG_EXTRADEBUG, "SQL %s", qep->msg);

	free(qep->msg);
	free(qep);
	qp_job_count--;
	return tempp;
}

/***************************************************************************************/

void PrintQueueEntry(QueueEntry * qep)
{
	int i = 0;

	if (!qep)
	{
		alog(LOG_DEBUG, "Queue is currently Empty");
	}
	else
	{
		while (qep)
		{
			alog(LOG_DEBUG, "List Entry %d", i);
			alog(LOG_DEBUG, "Message %s", qep->msg);
			i++;
			qep = qep->link;
		}
	}
}

/***************************************************************************************/

QueueEntry *ExecuteQueue(QueueEntry * qep)
{
	if (qep)
	{
		rdb_direct_query(qep->msg, 1);
		qp_exec_count++;
		return RemoveQueueEntry(qep);
	}
	return NULL;
}

/***************************************************************************************/

void ClearQueueEntry(QueueEntry * qep)
{
	while (qep)
	{
		qep = RemoveQueueEntry(qep);
	}
}

/***************************************************************************************/
#endif
