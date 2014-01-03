/* Stats -- main source file.
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *
 */

#include "denora.h"

/******** Global variables! ********/

/* Input buffer - global, so we can dump it if something goes wrong */
char inbuf[BUFSIZE];

/* Socket for talking to server */
int servsock = -1;

/* Parameters */
char **my_av;

/* Set to 1 if we are waiting for input */
int waiting = 0;

/* Set to 1 after we've set everything up */
int started = 0;

int SpeedTest;

/*************************************************************************/

DenoraVar denora[] =
{
	{
		STATS_DIR,                 /* Stats Dir */
		LOG_FILENAME,              /* Log File Name */
		NULL,                /* Config file Name */
		VERSION_STRING,            /* Version string */
		VERSION_STRING_DOTTED,     /* Version string dotted */
		STATS_BUILD,               /* Version flags OS */
		VER_SQL,                   /* Version flags SQL */
		VER_MODULE,                /* Version flag Module */
		NULL,                      /* Version Protocol */
		__DATE__,                  /* Date Compiled */
		__TIME__,                  /* Time Compiled */
		BUILD,                     /* Build Number */
		NULL,                      /* quit message */
		0,                         /* time started */
		0,                         /* sql code in use */
		0,                         /* sql debug */
		NULL,                      /* uplink server */
		0,                         /* capab of our uplink */
		0,                         /* quitting */
		0,                         /* delayed quit */
		0,                         /* html output */
		0,                         /* socket debug */
		0,                         /* Debug */
		0,                         /* nofork */
		0,                         /* Should we update the databases now? */
		0,                         /* protocol debug */
	}
};

/*************************************************************************/

STATVar stats[] =
{
	{
		0,                         /* users */
		0,                         /* opers */
		0,                         /* channels */
		0,                         /* servers */
		0,                         /* away */
		0,                         /* user max */
		0,                         /* user time */
		0,                         /* oper max */
		0,                         /* oper time */
		0,                         /* chan max */
		0,                         /* chan time */
		0,                         /* server max */
		0,                         /* servers_max_time */
		0,                         /* daily_users */
		0,                         /* daily_users_time */
		0,                         /* daily_opers */
		0,                         /* daily_opers_time */
		0,                         /* daily_servers */
		0,                         /* daily_servers_time */
		0,                         /* daily_chans */
		0,                         /* daily_chans_time */
		0,                         /* totalusersever */
	}
};

/*************************************************************************/

/* process:  Main processing routine.  Takes the string in inbuf (global
 *           variable) and does something appropriate with it. */

void process()
{
	int retVal = 0;
	Message *msgcurrent = NULL;
	char source[64];
	char cmd[64];
	char buf[513];              /* Longest legal IRC command line */
	char bufp10[513];           /* Longest legal IRC command line */
	char *s;
	int ac;                     /* Parameters for the command */
	char **av;
	Message *m;
	char *p10token;
	int p10parse = 1;
	Server *serv;
	int is_server = 0;

	/* zero out the buffers before we do much else */
	*buf = '\0';
	*source = '\0';
	*cmd = '\0';
	*bufp10 = '\0';

	

	/* If debugging, log the buffer */
	alog(LOG_DEBUG, "debug: Received: %s", inbuf);

	/* First make a copy of the buffer so we have the original in case we
	 * crash - in that case, we want to know what we crashed on. */
	strlcpy(buf, inbuf, sizeof(buf));

	/* Split the buffer into pieces. */
	if (*buf == ':' || *buf == '@')
	{
		s = strpbrk(buf, " ");
		if (!s)
			return;
		*s = 0;
		while (isspace(*++s));
		strlcpy(source, buf + 1, sizeof(source));
		memmove(buf, s, strlen(s) + 1);
	}
	else if (ircd->p10)
	{
		if (!*buf)
		{
			return;
		}
		strlcpy(bufp10, buf, sizeof(bufp10));
		p10token = myStrGetToken(bufp10, ' ', 0);
		if (!stricmp(p10token, "SERVER"))
		{
			p10parse = 0;
		}
		if (!stricmp(p10token, "PASS"))
		{
			p10parse = 0;
		}
		if (!stricmp(p10token, "ERROR"))
		{
			p10parse = 0;
		}
		if (!stricmp(p10token, "NOTICE"))
		{
			p10parse = 0;
		}
		if (p10parse)
		{
			s = strpbrk(buf, " ");
			if (!s)
			{
				free(p10token);
				return;
			}
			*s = 0;
			while (isspace(*++s));
			strlcpy(source, buf, sizeof(source));
			memmove(buf, s, strlen(s) + 1);
		}
		free(p10token);
	}
	else
	{
		*source = 0;
	}
	serv = server_find(source);
	if (serv)
	{
		is_server = 1;
	}

	if (!is_server)
	{
		doCleanBuffer((char *) buf);
	}

	s = strpbrk(buf, " ");
	if (s)
	{
		*s = 0;
		while (isspace(*++s));
	}
	else
		s = buf + strlen(buf);
	strlcpy(cmd, buf, sizeof(cmd));
	ac = split_buf(s, &av);

	/* fix to moduleGetLastBuffer() bug 296 */
	/* old logic was that since its meant for PRIVMSG that we would get
	   the NICK as AV[0] and the rest would be in av[1], however on Bahamut
	   based systems when you do /cs it assumes we will translate the command
	   to the NICK and thus AV[0] is the message. The new logic is to check
	   av[0] to see if its a service nick if so assign mod_current_buffer the
	   value from AV[1] else just assign av[0] - TSL */
	/* First check if the ircd proto module overrides this -GD */
	if (!denora_set_mod_current_buffer(ac, av))
	{
		if (!is_server) 
		{
			if (ac >= 1 && av[0])
			{
				if (nickIsServices(av[0]))
				{
					if (av[1])
					{
						mod_current_buffer = sstrdup(av[1]);
					}
					else
					{
						mod_current_buffer = sstrdup(av[0]);
					}
				}
				else
				{
					mod_current_buffer = sstrdup(av[0]);
				}
			}	
			else
			{
				mod_current_buffer = NULL;
			}
		}
		else
		{
			if (ac >= 1 && av[0])
			{
					mod_current_buffer = sstrdup(av[0]);
			}
			else
			{
				mod_current_buffer = NULL;
			}
		}
	}
	/* Do something with the message. */
	m = findMessage(IRCD, cmd);
	total_recmsg++;
	
	if (m)
	{
		if (m->func)
		{
			mod_current_module_name = m->mod_name;
			retVal = m->func(source, ac, av);
			mod_current_module_name = NULL;
			
			if (retVal == MOD_CONT)
			{
				if (m->next)
				{
					msgcurrent = m->next;
					if (msgcurrent)
					{
						
						while (msgcurrent && msgcurrent->func
						        && retVal == MOD_CONT)
						{
							mod_current_module_name = msgcurrent->mod_name;
							retVal = msgcurrent->func(source, ac, av);
							mod_current_module_name = NULL;
							msgcurrent = msgcurrent->next;
						}
					}
				}
			}
		}
	}
	else
	{
		alog(LOG_DEBUG, langstr(ALOG_UNKNWON_MSG), inbuf);
	}

	/* Load/unload modules if needed */
	handleModuleOperationQueue();

	/* Free argument list we created */
	if (av)
		free(av);
}

/*************************************************************************/

/* Restarts denora */

void denora_restart(void)
{
	ChannelStats *cs;
	lnode_t *tn;

	
	alog(LOG_NORMAL, langstr(RESTARTING));
	if (!denora->qmsg)
	{
		denora->qmsg = sstrdup(langstr(RESTARTING));
	}
	send_event(EVENT_RESTART, 1, EVENT_START);
	if (started)
	{
		if (!LargeNet)
		{
			tn = list_first(CStatshead);
			while (tn != NULL)
			{
				cs = lnode_get(tn);
				denora_cmd_part(s_StatServ, cs->name, "%s", denora->qmsg);
				tn = list_next(CStatshead, tn);
			}
		}
		if (LogChannel)
		{
			denora_cmd_part(s_StatServ, LogChannel, "%s", denora->qmsg);
		}
		denora_cmd_squit(ServerName, denora->qmsg);
	}
	disconn(servsock);
	close_log();
#ifdef _WIN32
	chdir("..");
#endif
#ifdef STATS_BIN
	execv(STATS_BIN, my_av);
#endif
	open_log();
	log_perror("%s", langstr(RESTART_FAILED));
	close_log();
}

/*************************************************************************/
/**
 * Added to allow do_restart from statserv access to the static functions without making them
 * fair game to every other function - not exactly ideal :|
 **/
void do_restart_denora(void)
{
	denora_restart();
	save_databases();
	
	exit(1);
}

/*************************************************************************/

/* Terminates denora */

void denora_shutdown(void)
{
	ChannelStats *cs;
	lnode_t *tn;

	

	send_event(EVENT_SHUTDOWN, 1, EVENT_START);
	save_databases();

	/* we are going down time to tell
	   some internal functions we are dead */
	denora->delayed_quit = 1;

	if (!denora->qmsg)
	{
		denora->qmsg = sstrdup("Terminating, reason unknown");
	}
	alog(LOG_NORMAL, "%s", denora->qmsg);
	if (started)
	{
		if (!LargeNet)
		{
			tn = list_first(CStatshead);
			while (tn != NULL)
			{
				cs = lnode_get(tn);
				denora_cmd_part(s_StatServ, cs->name, "%s", denora->qmsg);
				tn = list_next(CStatshead, tn);
			}
		}
		if (LogChannel)
		{
			denora_cmd_part(s_StatServ, LogChannel, "%s", denora->qmsg);
		}
		denora_cmd_squit(ServerName, denora->qmsg);
	}
	if (XMLRPC_Enable)
	{
		extsock_close();
	}
	disconn(servsock);
	send_event(EVENT_SHUTDOWN, 1, EVENT_STOP);
	if (started)
	{
		destroy_all();
	}
	if (denora->do_sql)
	{
		sql_close(sqlcon);
	}
	if (gidb)
	{
		GeoIP_delete(gidb);
	}
	if (gidb_v6)
	{
		GeoIP_delete(gidb_v6);
	}
	close_log();
	lang_destory();
}

/*************************************************************************/

void save_databases()
{
	send_event(EVENT_DB_SAVING, 1, EVENT_START);

	alog(LOG_DEBUG, langstr(SAVING_FFF));
	save_stats_db();
	save_cs_db();
	save_server_db();
	save_chan_db();
	sql_ban_clean(NULL);
	send_event(EVENT_DB_SAVING, 1, EVENT_STOP);
}

/*************************************************************************/

/* Main routine.  (What does it look like? :-) ) */

int main(int ac, char **av)
{
	char *bufres;
	time_t t;

	GeoIP_setup_custom_directory((char *) STATS_DIR);
	gidb = GeoIP_new(GEOIP_STANDARD);
	gidb_v6 = GeoIP_open_type(GEOIP_COUNTRY_EDITION_V6, GEOIP_STANDARD);

	UplinkSynced = 0;
	denora->debug = 0;
	
	t = time(NULL);

	/* Initialization stuff. */
	if (init(ac, av) != 0)
	{
		destroy_all();
		return 0;
	}
	signal_init();
	init_bans();

	/* We have a line left over from earlier, so process it first. */
	process();

	MainTimerInit("update", UpdateTimeout, MainBackUp);
	MainTimerInit("html", HTMLTimeout, do_html);
	MainTimerInit("ping", PingFrequency, ping_servers);
	MainTimerInit("backup", BackupFreq, backup_databases);
	MainTimerInit("sql", SQLPingFreq, sql_ping);

	/* set these globals to NULL on startup, they are mainly for module
	   coders */
	xml_encode = NULL;
	xml_doctype = NULL;
	xml_header = NULL;

	started = 1;

	/*** Main loop. ***/

	while (!denora->quitting)
	{
		alog(LOG_DEBUGSOCK, "debug: Top of main loop");

		if (denora->delayed_quit)
			break;
		
		MainTimerProcess();
		extsock_process();
		moduleCallBackRun();
		denora_cron(t);

		waiting = 1;
		bufres = sgets2(inbuf, sizeof(inbuf), servsock);
		waiting = 0;
		if (bufres && bufres != (char *) 0 && bufres != (char *) -1)
		{
			process();
		}
		else if (bufres == (char *) 0)
		{
			int errno_save = errno;
			denora->qmsg = calloc(BUFSIZE, 1);
			if (denora->qmsg)
			{
				ircsnprintf(denora->qmsg, BUFSIZE,
				            "Read error from server: %s (error num: %d)",
				            ErrMsgStr(errno_save), errno_save);
			}
			else
			{
				denora->qmsg = sstrdup("Read error from server");
			}
			denora->quitting = 1;
		}
		waiting = -4;
	}


	/* Check for restart instead of exit */
	if (denora->save_data == -2)
	{
		denora_restart();
		return 1;
	}

	/* Disconnect and exit */
	denora_shutdown();
	return 0;
}


/*************************************************************************/

void introduce_user(const char *user)
{
	/* Watch out for infinite loops... */
#define LTSIZE 20
	static int lasttimes[LTSIZE];
	if (lasttimes[0] >= time(NULL) - 3)
		fatal("introduce_user() loop detected");
	memmove(lasttimes, lasttimes + 1, sizeof(lasttimes) - sizeof(int));
	lasttimes[LTSIZE - 1] = time(NULL);
#undef LTSIZE

	
	if (!user || stricmp(user, s_StatServ) == 0)
	{
		denora_cmd_nick(s_StatServ, desc_StatServ, ircd->servicesmode);
	}

	if (s_StatServ_alias && !LargeNet)
	{
		
		if (!user || stricmp(user, s_StatServ_alias) == 0)
		{
			denora_cmd_nick(s_StatServ_alias, desc_StatServ_alias,
			                ircd->servicesmode);
		}
	}
	
}

/*************************************************************************/

/* Set GID if necessary.  Return 0 if successful (or if RUNGROUP not
 * defined), else print an error message to logfile and return -1.
 */

static int set_group(void)
{
#if defined(RUNGROUP) && defined(HAVE_SETGRENT)
	struct group *gr;

	setgrent();
	while ((gr = getgrent()) != NULL)
	{
		if (strcmp(gr->gr_name, RUNGROUP) == 0)
		{
			break;
		}
	}
	endgrent();
	if (gr)
	{
		setgid(gr->gr_gid);
		return 0;
	}
	else
	{
		alog(LOG_ERROR, "Error: Unable to find the group name `%s'",
		     RUNGROUP);
		alog(LOG_ERROR,
		     "Check your RUNGROUP setting and confirm that the group `%s' exists",
		     RUNGROUP);
		return -1;
	}
#else
	
#if defined(RUNGROUP)
	alog(LOG_DEBUG,
	     "debug: RUNGROUP listed but system does not support setgid()");
#endif
	return 0;
#endif
}

/*************************************************************************/

static int parse_dir_options(int ac, char **av)
{
	int i;
	char *s;

	for (i = 1; i < ac; i++)
	{
		s = av[i];
		if (*s == '-')
		{
			s++;
			if (strcmp(s, "dir") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-dir requires a parameter\n");
					return -1;
				}
				denora->dir = av[i];
			}
			else if (strcmp(s, "log") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-log requires a parameter\n");
					return -1;
				}
				denora->logname = sstrdup(av[i]);
			}
			else if (strcmp(s, "config") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-config requires a parameter\n");
					return -1;
				}
				denora->config = sstrdup(av[i]);
			}

		}
	}
	return 0;
}

/*************************************************************************/

/* Parse command-line options.  Return 0 if all went well, -1 for an error
 * with an option, or 1 for -help.
 */

static int parse_options(int ac, char **av)
{
	int i;
	char *s, *t;
	char *extra, *value;

	for (i = 1; i < ac; i++)
	{
		s = av[i];
		if (*s == '-')
		{
			s++;
			if (*s == '-')
				s++;
			value = myStrGetToken(s, '=', 0);
			extra = myStrGetToken(s, '=', 1);

			if (strcmp(value, "remote") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-remote requires hostname[:port]\n");
					return 0;
				}
				s = av[i];
				t = strchr(s, ':');
				if (t)
				{
					*t++ = 0;
					if (atoi(t) >= 1 || atoi(t) <= 65535)
						RemotePort = atoi(t);
					else
					{
						fprintf(stderr,
						        "-remote: port number must be a positive integer.  Using default.\n");
						return 0;
					}
				}
				if (RemoteServer)
					free(RemoteServer);
				RemoteServer = sstrdup(s);
			}
			else if (strcmp(value, "local") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr,
					        "-local requires hostname or [hostname]:[port]\n");
					return 0;
				}
				s = av[i];
				t = strchr(s, ':');
				if (t)
				{
					*t++ = 0;
					if (atoi(t) >= 1 || atoi(t) <= 65535)
						LocalPort = atoi(t);
					else
					{
						fprintf(stderr,
						        "-local: port number must be a positive integer or 0.  Using default.\n");
						return 0;
					}
				}
				if (LocalHost)
					free(LocalHost);
				LocalHost = sstrdup(s);
			}
			else if (strcmp(value, "name") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-name requires a parameter\n");
					return 0;
				}
				if (ServerName)
					free(ServerName);
				ServerName = sstrdup(av[i]);
			}
			else if (strcmp(value, "desc") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-desc requires a parameter\n");
					return 0;
				}
				if (ServerDesc)
					free(ServerDesc);
				ServerDesc = sstrdup(av[i]);
			}
			else if (strcmp(value, "user") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-user requires a parameter\n");
					return 0;
				}
				if (ServiceUser)
					free(ServiceUser);
				ServiceUser = sstrdup(av[i]);
			}
			else if (strcmp(value, "host") == 0)
			{
				if (++i >= ac)
				{
					fprintf(stderr, "-host requires a parameter\n");
					return 0;
				}
				if (ServiceHost)
					free(ServiceHost);
				ServiceHost = sstrdup(av[i]);
			}
			else if (strcmp(value, "dir") == 0)
			{
				/* Handled by parse_dir_options() */
				i++;            /* Skip parameter */
			}
			else if (strcmp(value, "log") == 0)
			{
				/* Handled by parse_dir_options(), too */
				i++;            /* Skip parameter */
			}
			else if (strcmp(value, "debug") == 0)
			{
				if (extra)
				{
					denora->debug = atoi(extra);
				}
				else
				{
					denora->debug++;
				}
			}
			else if (strcmp(value, "protocoldebug") == 0)
			{
				denora->protocoldebug++;
			}
			else if (strcmp(value, "speedtest") == 0)
			{
				SpeedTest++;
			}
			else if (strcmp(value, "sql") == 0)
			{
				denora->sqldebug = 1;
			}
			else if (strcmp(value, "socketdebug") == 0)
			{
				denora->socketdebug = 1;
			}
			else if (strcmp(value, "nofork") == 0)
			{
				denora->nofork = 1;
			}
			else if (strcmp(value, "mkpass") == 0)
			{
				if (extra)
				{
					fprintf(stdout, "Encrypted Password: %s\n",
					        MakePassword(extra));
				}
				else
				{
					fprintf(stdout,
					        "Please use syntax of -mkpass=password\n");
				}
				return 0;
			}
			else if (!strcmp(value, "version") || !strcmp(value, "v"))
			{
				fprintf(stdout,
				        "Denora-%s %s %s%s -- build #%s, compiled %s %s\n",
				        denora->version, denora->flags_os,
				        (denora->flag_mods ? denora->flag_mods : "")
				        , (denora->flag_sql ? denora->flag_sql : ""),
				        denora->build, denora->date, denora->time);
				return 0;
			}
			else if (!strcmp(value, "help") || !strcmp(value, "?"))
			{
				fprintf(stdout,
				        "Denora-%s %s %s%s -- build #%s, compiled %s %s\n",
				        denora->version, denora->flags_os,
				        (denora->flag_mods ? denora->flag_mods : "")
				        , (denora->flag_sql ? denora->flag_sql : ""),
				        denora->build, denora->date, denora->time);
				fprintf(stdout,
				        "Denora IRC Stats (http://www.denorastats.org/)\n");
				fprintf(stdout, "Usage ./stats [options] ...\n");
				fprintf(stdout,
				        "-remote        -remote hostname[:port]\n");
				fprintf(stdout, "-local         -local hostname[:port]\n");
				fprintf(stdout, "-name          -name servername\n");
				fprintf(stdout, "-desc          -desc serverdesc\n");
				fprintf(stdout, "-user          -user serviceuser\n");
				fprintf(stdout, "-host          -host servicehost\n");
				fprintf(stdout, "-debug         -debug\n");
				fprintf(stdout, "-nofork        -nofork\n");
				fprintf(stdout, "-sql           -sql\n");
				fprintf(stdout, "-protocoldebug -protocoldebug\n");
				fprintf(stdout, "-socketdebug   -socketdebug\n");
				fprintf(stdout, "-version       -version\n");
				fprintf(stdout, "-help          -help\n");
				fprintf(stdout, "-log           -log logfilename\n");
				fprintf(stdout, "-dir           -dir statsdirectory\n");
				fprintf(stdout, "-mkpass        -mkpass=password\n");
				fprintf(stdout, "-config        -config=filename\n");

				fprintf(stdout, "\n");
				fprintf(stdout,
				        "Further support is available from http://www.denorastats.org/\n");
				fprintf(stdout,
				        "Or visit US on IRC at irc.denorastats.org #denora\n");
				return 0;
			}
			else
			{
				fprintf(stderr, "Unknown option -%s\n", value);
				return 0;
			}
			free(extra);
			free(value);
		}
		else
		{
			fprintf(stderr, "Non-option arguments not allowed\n");
			return 0;
		}
	}
	return 1;
}

/*************************************************************************/

/* Remove our PID file.  Done at exit. */

void remove_pidfile(void)
{
	if (PIDFilename)
	{
		remove(PIDFilename);
	}
}

/*************************************************************************/

/* Create our PID file and write the PID to it. */

static void write_pidfile(void)
{
	FILE *pidfile;

	

	if ((pidfile = FileOpen(PIDFilename, FILE_WRITE)) != NULL)
	{
		fprintf(pidfile, "%d\n", (int) getpid());
		fclose(pidfile);
	}
	else
	{
		log_perror("Warning: cannot write to PID file %s", PIDFilename);
	}
}

/*************************************************************************/

/* Overall initialization routine.  Returns 0 on success, -1 on failure. */

int init(int ac, char **av)
{
	int i;
	int openlog_failed = 0, openlog_errno = 0;
	int started_from_term = isatty(0) && isatty(1) && isatty(2);
	char *progname;

	/* Set file creation mask and group ID. */
#if defined(DEFUMASK) && HAVE_UMASK
	umask(DEFUMASK);
#endif
	if (set_group() < 0)
	{
		return -1;
	}
	/* Parse command line for -dir option. */
	parse_dir_options(ac, av);


    /* Find program name. */
    if ((progname = strrchr(av[0], '/')) != NULL)
        progname++;
    else
        progname = av[0];

#ifdef _WIN32
    if (strcmp(progname, "denoraxmlcheck.exe") == 0)
#else
    if (strcmp(progname, "denoraxmlcheck") == 0)
#endif
    {
        denoraxmlcheck(ac, av);
        return 0;
    }

	/* Chdir to Denora data directory. */
	if (chdir(denora->dir) < 0)
	{
		fprintf(stderr, "chdir(%s): %s\n", denora->dir, ErrMsgStr(errno));
		return -1;
	}

	/* Open logfile, and complain if we didn't. */
	if (open_log() < 0)
	{
		openlog_errno = errno;
		if (started_from_term)
		{
			fprintf(stderr, "Warning: unable to open log file %s: %s\n",
			        denora->logname, ErrMsgStr(errno));
		}
		else
		{
			openlog_failed = 1;
		}
	}

	/* Initialize multi-language support */
	lang_init();
	alog(LOG_DEBUG, "debug: Loaded languages");

	/* Prepare the config struct for the parser */
	DenoraConfigInit();

	/* Must Set if not set by the command line */
	if (!denora->config)
	{
		denora->config =  sstrdup(STATS_CONF);
	}

	/* Read configuration file; exit if there are problems. */
	if (!DenoraParseXMLConfig((char *) denora->config))
	{
		printf("%s not found, please rename example.xml to denora.xml\n", denora->config);
		printf("try \"mv example.xml denora.xml\"\n");
		printf("Denora not started\n");
		exit(-1);
	}

	/* Parse all remaining command-line options. */
	if (!parse_options(ac, av))
	{
		exit(1);
	}

	/* Since you can rem out whole blocks and segfault
	   denora we run a bad pointer check on variables */
	post_config_check();

	alog(LOG_NORMAL,"================================================================");
	alog(LOG_NORMAL, "Denora v%s starting up", VERSION_STRING);
	alog(LOG_NORMAL, "IRCD Protocol %s, Encryption Module %s", IRCDModule, ENCModule);
	alog(LOG_NORMAL, "SQLite3 Version %s", SQLITE_VERSION);
	if (SQLModule)
	{
		alog(LOG_NORMAL, "SQL Module %s", SQLModule);
	}

#ifndef _WIN32
	/* Detach ourselves if requested. */
	if (!denora->nofork)
	{
		alog(LOG_NORMAL, "Preparing to fork into the back ground");
		if ((i = fork()) < 0)
		{
			perror("fork()");
			return -1;
		}
		else if (i != 0)
		{
			exit(0);
		}
		if (started_from_term)
		{
			close(0);
			close(1);
			close(2);
		}
		if (setpgid(0, 0) < 0)
		{
			perror("setpgid()");
			return -1;
		}
	}
#else
	/* Initialize winsocks -- codemastr */
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(1, 1), &wsa))
		{
			alog(LOG_ERROR, "Failed to initialized WinSock library");
			return -1;
		}
	}
	if (!SupportedWindowsVersion())
	{
		alog(LOG_ERROR, "Not supported version of Windows");
		return -1;
	}
	else
	{
		denora->flags_os = GetWindowsVersion();
		alog(LOG_NORMAL, "Running: %s", denora->flags_os);
	}

	if (!denora->nofork)
	{
		alog(LOG_NORMAL, "Launching Denora into the background");
		FreeConsole();
	}
#endif



	/* setup user modes array now all at 0, during protocol init they
	   will be setting these to 1 as needed.
	 */
	init_umodes();
	init_cmodes();
	init_csmodes();

	/* Add IRCD Protocol Module; exit if there are errors */
	if (protocol_module_init() != MOD_ERR_OK)
	{
		return -1;
	}
	if (!denora->version_protocol)
	{
		denora->version_protocol = sstrdup("Bad No Protocol");
		return -1;
	}
	if (encode_module_init() != MOD_ERR_OK)
	{
		return -1;
	}
	if (sql_module_init() != MOD_ERR_OK)
	{
		return -1;
	}

	zip_init(ZIP_DEFAULT_LEVEL);
	statserv_int();

	/* Add Core MSG handles */
	moduleAddMsgs();

	CreateUserModeCallBack((char *) "o", MODE_NOPARAM, handle_ircop);

	/* Write our PID to the PID file. */
	write_pidfile();

	load_stats_db();
	InitCStatsList();
	load_cs_db();
	InitStatsChanList();
	load_chan_db();

	/* Announce ourselves to the logfile. */
	alog(LOG_NORMAL,
	     "Denora %s (IRCd protocol: %s) starting up (options:%s)",
	     denora->version,
	     (denora->version_protocol ? denora->
	      version_protocol : "None Set"),
	     denora->debug ? " debug on" : " debug off");
	denora->start_time = time(NULL);

	setup_cron_event();

	if (denora->do_sql && !sql_init())
	{
		alog(LOG_ERROR, "Error: Disabling SQL due to errors with SQL");
	}

	if (XMLRPC_Enable)
	{
		extsock_init(XMLRPC_Host, XMLRPC_Port);
	}

	core_modules_init();

	/* load any custom modules */
	modules_init();

	/* Connect to the remote server */
	servsock = conn(RemoteServer, RemotePort, LocalHost, LocalPort);
	if (servsock < 0)
	{
		fatal_perror("Can't connect to server");
	}
	alog(LOG_NORMAL, "Connected to Server (%s:%d)", RemoteServer,
	     RemotePort);

	send_event(EVENT_CONNECT, 1, EVENT_START);
	denora_cmd_connect();

	sgets2(inbuf, sizeof(inbuf), servsock);
	if (strnicmp(inbuf, "ERROR", 5) == 0)
	{
		/* Close server socket first to stop wallops, since the other
		 * server doesn't want to listen to us anyway */
		disconn(servsock);
		servsock = -1;
		fatal("Remote server returned: %s", inbuf);
	}

	/* Announce a logfile error if there was one */
	if (openlog_failed)
	{
		denora_cmd_global(NULL, langstring(CANNOT_OPEN_LOG), ErrMsgStr(openlog_errno));
	}

	/* Bring in our pseudo-clients */
	introduce_user(NULL);

	/* And hybrid needs StatServ joined in the logchan */
	if (!BadPtr(LogChannel))
	{
		denora_cmd_join(s_StatServ, LogChannel, time(NULL));
	}

	denora_cmd_eob();
	send_event(EVENT_CONNECT, 1, EVENT_STOP);

	/* Dumping stats.db maxvalues to sql */
	DenoraSQLQuery(DenoraDB,"UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='channels'",
		 MaxValueTable, stats->chans_max,
		 (long int) stats->chans_max_time);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='users'",
		 MaxValueTable, stats->users_max,
		 (long int) stats->users_max_time);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='servers'",
		 MaxValueTable, stats->servers_max,
		 (long int) stats->servers_max_time);
	DenoraSQLQuery(DenoraDB, "UPDATE %s SET val=%d, time=FROM_UNIXTIME(%ld) WHERE type='opers'",
		 MaxValueTable, stats->opers_max,
		 (long int) stats->opers_max_time);

	/**
	  * Load our delayed modules - modules that are planing on making clients need to wait till now
	  * where as modules wanting to modify our ircd connection messages need to load earlier :|
	  **/
	modules_delayed_init();

	/* Success! */
	return 0;
}

/*************************************************************************/
