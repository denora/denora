
/* signals
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

VOIDSIG signal_segfault(int signum);
VOIDSIG signal_restart(int signum);
VOIDSIG signal_rehash(int signum);
VOIDSIG signal_int(int signum);
VOIDSIG signal_pipe(int signum);
VOIDSIG signal_die(int signum);
char segv_location[SEGV_LOCATION_BUFSIZE];

#if !defined(HAVE_STRSIGNAL)
define strsignal(int sig) get_signame(int sig);
#elsif !defined(strsignal) and (!defined(__CYGWIN__) || (__FreeBSD__) || (__OpenBSD__))
char *strsignal(int sig);
#endif

int sigpipecount;
int32 sigpipetime;

/*************************************************************************/

#ifndef _WIN32
void signal_init()
{
#ifdef	POSIX_SIGNALS
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;

# ifdef	SIGWINCH
	act.sa_handler = SIG_IGN;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGWINCH);
	(void) sigaction(SIGWINCH, &act, NULL);
# endif

	act.sa_handler = signal_pipe;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGPIPE);
	(void) sigaction(SIGPIPE, &act, NULL);

	act.sa_handler = sighandler;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGALRM);
	(void) sigaction(SIGALRM, &act, NULL);

	act.sa_handler = signal_rehash;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGHUP);
	(void) sigaction(SIGHUP, &act, NULL);

	act.sa_handler = signal_restart;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGUSR2);
	(void) sigaction(SIGUSR2, &act, NULL);

	/* Only die with SIGINT if we are in nofork mode */
	if (denora->nofork)
	{
		act.sa_handler = signal_int;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGINT);
		(void) sigaction(SIGINT, &act, NULL);
	}

	act.sa_handler = signal_die;
	(void) sigemptyset(&act.sa_mask);
	(void) sigaddset(&act.sa_mask, SIGTERM);
	(void) sigaction(SIGTERM, &act, NULL);
	if (!DumpCore)
	{
		act.sa_handler = signal_segfault;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGSEGV);
		(void) sigaction(SIGSEGV, &act, NULL);
	}
	else
	{
		act.sa_handler = SIG_DFL;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGSEGV);
		(void) sigaction(SIGSEGV, &act, NULL);

		act.sa_handler = SIG_DFL;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGBUS);
		(void) sigaction(SIGBUS, &act, NULL);

		act.sa_handler = SIG_DFL;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGILL);
		(void) sigaction(SIGILL, &act, NULL);

		act.sa_handler = SIG_DFL;
		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGTRAP);
		(void) sigaction(SIGTRAP, &act, NULL);

	}
#else
# ifndef	HAVE_RELIABLE_SIGNALS
	(void) signal(SIGPIPE, signal_pipe);
#  ifdef	SIGWINCH
	(void) signal(SIGWINCH, sighandler);
#  endif
# else
#  ifdef	SIGWINCH
	(void) signal(SIGWINCH, SIG_IGN);
#  endif
	(void) signal(SIGPIPE, SIG_IGN);
# endif
	(void) signal(SIGALRM, sighandler);
	(void) signal(SIGHUP, signal_rehash);
	(void) signal(SIGTERM, signal_die);
	(void) signal(SIGUSR2, signal_restart);

	if (denora->nofork)
	{
		(void) signal(SIGINT, signal_int);
	}
	if (!DumpCore)
	{
		(void) signal(SIGSEGV, signal_segfault);
		(void) signal(SIGBUS, sighandler);
		(void) signal(SIGILL, sighandler);
		(void) signal(SIGTRAP, sighandler);
	}
	else
	{
		(void) signal(SIGSEGV, SIG_DFL);
		(void) signal(SIGBUS, SIG_DFL);
		(void) signal(SIGILL, SIG_DFL);
		(void) signal(SIGTRAP, SIG_DFL);
	}
#endif

#ifdef SIGIOT
	(void) signal(SIGIOT, sighandler);
#endif
	(void) signal(SIGFPE, sighandler);
	(void) signal(SIGQUIT, sighandler);

#if !defined(USE_THREADS)
	(void) signal(SIGUSR1, sighandler); /* This is our "out-of-memory" panic switch */
#endif

}
#else
void signal_init()
{
	if (!DumpCore)
	{
		(void) signal(SIGSEGV, signal_segfault);
	}
	else
	{
		(void) signal(SIGSEGV, SIG_DFL);
	}
}
#endif

/*************************************************************************/

VOIDSIG signal_pipe(int signum)
{
	int32 sum;

#ifdef SIGPIPE
	if (signum == SIGPIPE)
	{
		send_event(EVENT_SIGNAL, 2, "SIGPIPE",
		           "Received SIGPIPE: Broken pipe");
		alog(LOG_NORMAL, "Received SIGPIPE: Broken pipe");
		if (sigpipecount >= 2)
		{
			sum = time(NULL) - sigpipetime;
			if (sum < 3600)
			{
				alog(LOG_NORMAL,
				     "This error has happened serval times in less then an hour.");
				alog(LOG_NORMAL,
				     "This generally means there is a serious problem that you need to address.");
				alog(LOG_NORMAL, "Denora will now exit");
				denora_shutdown();
				exit(-1);
			}
			else if (sum < 86400)
			{
				alog(LOG_NORMAL,
				     "This error has happened serval times in less then 24 period");
				alog(LOG_NORMAL,
				     "You should consider restarting or resolveing the issue before continue on");
				sigpipecount++;
				sigpipetime = time(NULL);
			}
			else
			{
				alog(LOG_NORMAL,
				     "This error has happened serval times in greater then 24 period");
				alog(LOG_NORMAL,
				     "Operation is still iffy but lowering the sigpipe count");
				sigpipecount--;
				sigpipetime = time(NULL);
			}
		}
		else
		{
			alog(LOG_NORMAL, langstr(ALOG_SIGPIPE_WARNING));
			do_backtrace(0);
			sigpipecount++;
			sigpipetime = time(NULL);
		}
	}
#endif
}

/*************************************************************************/

VOIDSIG signal_segfault(int signum)
{
	if (signum == SIGSEGV)
	{
		send_event(EVENT_SIGNAL, 2, "SIGSEGV",
		           "Received SIGSEGV: Segfault");
		alog(LOG_NORMAL, "Received SIGSEGV: Segfault");
		do_backtrace(1);
		exit(0);
	}
}

/*************************************************************************/

VOIDSIG signal_restart(int signum)
{
#ifdef SIGUSR2
	if (signum == SIGUSR2)
	{
#endif
		alog(LOG_NORMAL, "Received SIGUSR2, restarting.");
		if (!denora->qmsg)
		{
			denora->qmsg = sstrdup("Restarting on SIGUSR2");
		}
		send_event(EVENT_SIGNAL, 2, "SIGUSR2", denora->qmsg);
#ifdef STATS_BIN
		denora_restart();
#else
		denora->qmsg = sstrdup(langstr(ALOG_NO_STATS_BIN));
#endif
		exit(-1);
#ifdef SIGUSR2
	}
#endif
}

/*************************************************************************/

VOIDSIG signal_rehash(int signum)
{
#ifdef	POSIX_SIGNALS
	struct sigaction act;
#endif
#ifdef SIGHUP
	if (signum == SIGHUP)
	{
#endif
		Dadmin *a;
		int i;

		alog(LOG_NORMAL,
		     "Received SIGHUP: Saving Databases & Rehash Configuration");
		if (initconf(denora->config, 1, mainconf) == -1)
		{
			denora->qmsg = calloc(50, 1);
			if (!denora->qmsg)
			{
				denora->qmsg = sstrdup(langstr(ALOG_ERR_REHASH_NO_MEM));
			}
			else
			{
				ircsnprintf(denora->qmsg, sizeof(denora->qmsg), "%s",
				            langstr(ALOG_ERR_REHASH));
			}
			denora->quitting = 1;
			send_event(EVENT_SIGNAL, 2, "SIGHUP", denora->qmsg);
		}
		else
		{
			/* Remove all config file admins from admin struct before re-reading config file */
			for (i = 0; i < 1024; i++)
			{
				for (a = adminlists[i]; a; a = a->next)
				{
					if (a->configfile)
					{
						free_admin(a);
					}
					break;
				}
			}
			merge_confs();
		}
		/* Reload GeoIP db files */
		if (gidb)
			GeoIP_delete(gidb);
		if (gidb_v6)
			GeoIP_delete(gidb_v6);
		gidb = GeoIP_new(GEOIP_STANDARD);
		gidb_v6 = GeoIP_open_type(GEOIP_COUNTRY_EDITION_V6, GEOIP_STANDARD);
#ifdef	POSIX_SIGNALS
		act.sa_handler = signal_rehash;
		act.sa_flags = 0;

		(void) sigemptyset(&act.sa_mask);
		(void) sigaddset(&act.sa_mask, SIGHUP);
		(void) sigaction(SIGHUP, &act, NULL);
#else
# ifndef _WIN32
		(void) signal(SIGHUP, signal_rehash);   /* sysV -argv */
# endif
#endif
#ifdef SIGHUP
	}
#endif
}

/*************************************************************************/

VOIDSIG signal_die(int signum)
{
	if (signum == SIGTERM)
	{
		alog(LOG_NORMAL, "Received SIGTERM, exiting.");
		denora->qmsg = sstrdup("Shutting down on SIGTERM");
		send_event(EVENT_SIGNAL, 2, "SIGTERM", denora->qmsg);
		denora_shutdown();
		exit(-1);
	}
}

/*************************************************************************/

VOIDSIG signal_int(int signum)
{
	if (signum == SIGINT)
	{
		alog(LOG_NORMAL, "Received SIGINT, exiting.");
		denora->qmsg = sstrdup("Shutting down on SIGINT");
		send_event(EVENT_SIGNAL, 2, "SIGINT", denora->qmsg);
		denora_shutdown();
		exit(-1);
	}
}

/*************************************************************************/

/* If we get a weird signal, come here. */

VOIDSIG sighandler(int signum)
{
#ifndef _WIN32
	if (started)
	{
		if (signum == SIGQUIT)
		{
			/* nothing -- terminate below */
		}
		else if (!waiting)
		{
			alog(LOG_NORMAL, "PANIC! buffer = %s", inbuf);
			/* Cut off if this would make IRC command >510 characters. */
			if (strlen(inbuf) > 448)
			{
				inbuf[446] = '>';
				inbuf[447] = '>';
				inbuf[448] = 0;
			}
			denora_cmd_global(NULL, "PANIC! buffer = %s\r\n", inbuf);
		}
		else if (waiting < 0)
		{
			/* This is static on the off-chance we run low on stack */
			static char buf[BUFSIZE];
			switch (waiting)
			{
				case -1:
					ircsnprintf(buf, sizeof(buf), "in timed_update");
					break;
				default:
					ircsnprintf(buf, sizeof(buf), "waiting=%d", waiting);
			}
			denora_cmd_global(NULL, "PANIC! %s (%s)", buf,
			                  strsignal(signum));
			alog(LOG_NORMAL, "PANIC! %s (%s)", buf, strsignal(signum));
		}
	}
#endif
	if (
#if !defined(USE_THREADS)
#ifndef _WIN32
	    signum == SIGUSR1 ||
#endif
#endif
	    !(denora->qmsg = calloc(BUFSIZE, 1)))
	{
		denora->qmsg = sstrdup("Out of memory!");
	}
	else
	{
		ircsnprintf(denora->qmsg, BUFSIZE, "Stats terminating: %s",
		            strsignal(signum));
	}
	send_event(EVENT_SIGNAL, 2, "unknown", denora->qmsg);
	if (started)
	{
		denora_shutdown();
		exit(0);
	}
	else
	{
		if (isatty(2))
		{
			fprintf(stderr, "%s\n", denora->qmsg);
		}
		else
		{
			alog(LOG_NORMAL, "%s", denora->qmsg);
		}
		exit(1);
	}
	return;
}

/*************************************************************************/

void do_backtrace(int header)
{
#ifndef _WIN32
#ifdef HAVE_BACKTRACE
	void *array[50];
	size_t size;
	char **strings;
	int idx;
	int i;
	ModuleHash *current = NULL;

	if (header)
	{
		alog(LOG_NORMAL, "Backtrace: Segmentation fault detected");
		alog(LOG_NORMAL, langstr(BACKTRACE_LINES));
		alog(LOG_NORMAL,
		     "The following lines should not be seen as a definitive backtrace");
		alog(LOG_NORMAL,
		     "If reproducable please use gdb to get proper backtraces");
		alog(LOG_NORMAL, "Example:");
		alog(LOG_NORMAL, "gdb stats");
		alog(LOG_NORMAL, "run -nofork -debug -protocoldebug");
		alog(LOG_NORMAL, "crash the program");
		alog(LOG_NORMAL, "bt full");
		alog(LOG_NORMAL,
		     "paste results and following line into the bugtracker");
		alog(LOG_NORMAL,
		     "[================ COPY THE FOLLOWING =================]");
		alog(LOG_NORMAL, "Backtrace: %s", segv_location);
		alog(LOG_NORMAL, "[inbuf][%s]", inbuf);
		alog(LOG_NORMAL,
		     "Backtrace: Denora version %s build #%s, compiled %s %s",
		     denora->version, denora->build, denora->date, denora->time);
		alog(LOG_NORMAL,
		     "[modules]: Listing all currently loaded modules");
		for (idx = 0; idx != MAX_CMD_HASH; idx++)
		{
			for (current = MODULE_HASH[idx]; current;
			        current = current->next)
			{
				alog(LOG_DEBUG, "[name=%s][type=%d][version %s]",
				     current->name, current->m->type, current->m->version);
			}
		}
	}
	else
	{
		alog(LOG_NORMAL, "Execution trace: %s", segv_location);
		alog(LOG_NORMAL, "Execution trace: in progress");
	}
	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);
	for (i = 0; i < (int) size; i++)
	{
		alog(LOG_NORMAL, "%s(%d): %s",
		     (header ? "Backtrace" : "Execution Trace"), i, strings[i]);
	}
	free(strings);
	alog(LOG_NORMAL, "%s: complete",
	     (header ? "Backtrace" : "Execution Trace"));
	if (header)
	{
		alog(LOG_NORMAL,
		     "[=================================================]");
	}
#else
	if (segv_location)
	{
		alog(LOG_NORMAL, "Backtrace: %s", segv_location);
	}
	if (header)
	{
		alog(LOG_NORMAL, langstr(BACKTRACE_NOT_HERE));
	}
#endif
#else
	char *winver;
	if (segv_location)
	{
		alog(LOG_NORMAL, "Backtrace: %s", segv_location);
	}
	alog(LOG_NORMAL, "Backtrace not supported on win32");
	winver = GetWindowsVersion();
	alog(LOG_NORMAL, "Running %s", winver);
	free(winver);
#endif
}

/*************************************************************************/
