/* GLINE/TKL Bans 
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 *
 */
/*************************************************************************/

#include "denora.h"

#define MODULE_VERSION "2.0"
#define MODULE_NAME "gline"

void sql_do_server_bans_add(char *type, char *user, char *host, char *setby, char *setat, char *expires, char *reason);
void sql_do_server_bans_remove(char *type, char *user, char *host);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
	if (denora->debug >= 2)
	{
		protocol_debug(NULL, argc, argv);
	}
	
	alog(LOG_NORMAL,   "[%s] version %s", MODULE_NAME, MODULE_VERSION);
	
	if (DisableNetBans)
	{
		alog(LOG_NORMAL,   "[%s] DisableNetBans enabled unloading module", MODULE_NAME);
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion(MODULE_VERSION);
	moduleSetType(CORE);


	Gline_Parse_Add_Handler(sql_do_server_bans_add);
	Gline_Parse_Del_Handler(sql_do_server_bans_remove);
	
	return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/**
 * Parse a P10 ircd gline message into a format that sql_do_server_bans_* can handle
 *
 * @param source Server that sent the GLINE
 * @param ac Parameter Count
 * @param av Char array in 0 index
 *
 * @return void - no returend value
 */
void p10_gline(char *type, char *source, int ac, char **av)
{
	Server *s = NULL;
	User *u = NULL;
	char buf[12];
	char *user;
	char *host;
	char *address;
	char *setby;
	char expires[12];

	

	if (ac < 4 || ac > 6)
	{
		alog(LOG_DEBUG,
		     "debug: %s called with %d needed 4,5 or 6", PRETTY_FUNCTION,
		     ac);
		return;
	}

	if (source)
	{
		s = server_find(source);
		if (!s)
		{
			u = user_find(source);
			if (u)
			{
				setby = sstrdup(u->sqlnick);
			}
			else
			{
				setby = sstrdup(source);
			}
		}
		else
		{
			setby = sstrdup(s->name);
		}
	}
	else
	{
		alog(LOG_DEBUG,
		     "debug: %s called without a source", PRETTY_FUNCTION);
		return;
	}

	

	address = (*av[1] == '!') ? myStrGetToken(av[1], '!', 1) : av[1];
	if (*address == '+')
	{
		address = myStrGetToken(address, '+', 1);
	}
	else if (*address == '-')
	{
		address = myStrGetToken(address, '-', 1);
	}

	if (strstr(address, "@"))
	{
		user = myStrGetToken(address, '@', 0);
		host = myStrGetToken(address, '@', 1);
	}
	else
	{
		user = sstrdup("*");
		host = sstrdup(address);
	}

	

	if (*av[1] == '+')
	{
		*expires = '\0';
		if (ac == 6)
		{
			sprintf(expires, "%d", atoi(av[3]) + atoi(av[4]));
			gline.add(type, user, host, setby, av[3], expires,
					       av[5]);
		}
		else if (ac == 5)
		{
			sprintf(expires, "%d", atoi(av[2]) + atoi(av[3]));
			gline.add(type, user, host, setby, av[3], expires,
			                       av[4]);
		}
		else if (ac == 4)
		{
			*buf = '\0';
			ircsnprintf(buf, 12, "%ld", (long int) time(NULL));
			sprintf(expires, "%ld", atoi(av[2]) + (long int) time(NULL));
			gline.add(type, user, host, setby, buf, expires,
			                       av[3]);
		}
	}
	else
	{
		gline.del(type, user, host);
	}
	

	/*
	 * Free the data that was allocated
	 */
	free(user);
	free(host);
	free(address);
	free(setby);

	return;
}

