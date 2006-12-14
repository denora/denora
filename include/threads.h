/*
 *
 * (C) 2004 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: threads.h 632 2006-07-29 10:22:43Z crazy $
 *
 */

#ifndef THREADS_H
#define THREADS_H

#ifdef _WIN32
typedef long deno_thread_t;
typedef HANDLE deno_mutex_t;
typedef HANDLE deno_cond_t;
typedef unsigned 	(__stdcall *deno_thread_start) (void *);
typedef struct 
{
	deno_thread_start func;
	void *arg;
} deno_cleanup_t;

extern deno_thread_start __declspec(thread) cleanup_func;

#define deno_thread_create(thread,start,arg)  	!_beginthreadex(NULL, 0, (deno_thread_start)start, arg, 0, &thread)
#define deno_thread_self()						GetCurrentThreadId()
#define deno_thread_detach(thread)				0
#define deno_mutex_lock(mutex)					WaitForSingleObject(mutex, INFINITE)
#define deno_mutex_unlock(mutex)				ReleaseMutex(mutex)
/* deno_cond_wait is in compat.c */
#define deno_cond_signal(cond)					SetEvent(cond)
#define deno_cleanup_push(func, arg)			cleanup_func = (deno_thread_start)func
#define deno_cleanup_pop(execute)				cleanup_func(NULL)

#else

typedef pthread_t deno_thread_t;
typedef pthread_mutex_t deno_mutex_t;
typedef pthread_cond_t deno_cond_t;
typedef void 		*(*deno_thread_start) (void *);

#define deno_thread_create(thread,start,arg)	pthread_create(&thread, NULL, start, arg)
#define deno_thread_self()						pthread_self()
#define deno_thread_detach(thread)				pthread_detach(thread)
#define deno_mutex_lock(mutex)					pthread_mutex_lock(&mutex)
#define deno_mutex_unlock(mutex)				pthread_mutex_unlock(&mutex)
#define deno_cond_wait(cond, mutex)				pthread_cond_wait(&cond, &mutex)
#define deno_cond_signal(cond)					pthread_cond_signal(&cond)
#define deno_cleanup_push(func, arg)			pthread_cleanup_push(func, arg)
#define deno_cleanup_pop(execute)				pthread_cleanup_pop(execute)
#define deno_thread_cancel(thread)				pthread_cancel(thread)

#endif

#endif
