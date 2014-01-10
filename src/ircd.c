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

IRCDProto ircdproto;
IRCDModes ircd_modes;

/**
 * Globals we want from the protocol file
 **/
IRCDVar *ircd;
IRCDCAPAB *ircdcap;

int UseTSMODE;
uint32 irc_var;

/*************************************************************************/

/**
 * Initiate a protocol struct ready for use
 *
 * @return void - no returend value
 *
 */
void initIrcdProto(void)
{
	ircdproto.ircd_set_mod_current_buffer = NULL;
	ircdproto.ircd_cmd_nick = NULL;
	ircdproto.ircd_cmd_mode = NULL;
	ircdproto.ircd_cmd_bot_nick = NULL;
	ircdproto.ircd_cmd_notice = NULL;
	ircdproto.ircd_cmd_privmsg = NULL;
	ircdproto.ircd_cmd_serv_notice = NULL;
	ircdproto.ircd_cmd_serv_privmsg = NULL;
	ircdproto.ircd_cmd_quit = NULL;
	ircdproto.ircd_cmd_pong = NULL;
	ircdproto.ircd_cmd_join = NULL;
	ircdproto.ircd_cmd_part = NULL;
	ircdproto.ircd_cmd_global = NULL;
	ircdproto.ircd_cmd_squit = NULL;
	ircdproto.ircd_cmd_connect = NULL;
	ircdproto.ircd_cmd_eob = NULL;
	ircdproto.ircd_cmd_ctcp = NULL;
	ircdproto.ircd_cmd_version = NULL;
	ircdproto.ircd_cmd_stats = NULL;
	ircdproto.ircd_cmd_motd = NULL;
	ircdproto.ircd_cmd_ping = NULL;
}

/*************************************************************************/

/**
 * Special function, returns 1 if executed, 0 if not
 *
 * @return void - no returend value
 *
 */
int denora_set_mod_current_buffer(int ac, char **av)
{
	if (ircdproto.ircd_set_mod_current_buffer)
	{
		ircdproto.ircd_set_mod_current_buffer(ac, av);
		return 1;
	}
	return 0;
}

/*************************************************************************/

/**
 * Send the MOTD command to a server
 *
 * @param sender is the person whom is sending the command
 * @param server is the server the command to
 * @return void - no returend value
 *
 */
void denora_motd(char *sender, char *server)
{
	if (ircdproto.ircd_cmd_motd)
	{
		if (sender && server)
		{
			ircdproto.ircd_cmd_motd(sender, server);
		}
	}
}

/*************************************************************************/

/**
 * Send the PING command to a server
 *
 * @param server is the server the command to
 * @return void - no returend value
 *
 */
void denora_cmd_ping(char *server)
{
	if (ircdproto.ircd_cmd_ping)
	{
		if (server)
		{
			ircdproto.ircd_cmd_ping(server);
		}
	}
}

/*************************************************************************/

void denora_cmd_nick(char *nick, char *name, const char *modes)
{
	char buf[BUFSIZE];
	Uid *ud = NULL;
	User *u;
	char *ipchar;
	char *uid;

	*buf = '\0';

	uid = uid_gen();
	new_uid(nick, uid);

	ircdproto.ircd_cmd_nick(nick, name, modes);

	ipchar = host_resolve(ServiceHost);

	if (ircd->p10)
	{
		ud = find_uid(nick);
		u = do_nick(buf, nick, ServiceUser, ServiceHost,
		            ServerName, name, time(NULL), 0, ipchar, ServiceHost,
		            (ud ? ud->uid : NULL), 1, (char *) modes, NULL);
	}
	else if (ircd->ts6 && UseTS6 && Numeric)
	{
		ud = find_uid(nick);
		u = do_nick(buf, nick, ServiceUser, ServiceHost,
		            ServerName, name, time(NULL), 0, ipchar, ServiceHost,
		            (ud ? ud->uid : NULL), 1, (char *) modes, NULL);
	}
	else
	{
		u = do_nick(buf, nick, ServiceUser, ServiceHost,
		            ServerName, name, time(NULL), 0, ipchar, ServiceHost,
		            NULL, 1, (char *) modes, NULL);
	}
	free(ipchar);

	if (u)
	{
		u->isservice++;
		if (!is_excluded(u))
		{
			make_exclude(u->nick);
		}
	}
	sleep(5);
}

/*************************************************************************/

void denora_cmd_mode(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto.ircd_cmd_mode(source, dest, buf);
}

/*************************************************************************/

void denora_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                         char *modes)
{
	char buf[BUFSIZE];
	Uid *ud = NULL;
	User *u;
	char *ipchar;

	*buf = '\0';

	ipchar = host_resolve(ServiceHost);

	ircdproto.ircd_cmd_bot_nick(nick, user, host, real, modes);

	if (ircd->p10)
	{
		ud = find_uid(nick);
		u = do_nick(buf, nick, user, host,
		            ServerName, real, time(NULL), 0, ipchar, host,
		            (ud ? ud->uid : NULL), 1, modes, NULL);
	}
	else if (ircd->ts6 && UseTS6 && Numeric)
	{
		ud = find_uid(nick);
		u = do_nick(buf, nick, user, host,
		            ServerName, real, time(NULL), 0, ipchar, host,
		            (ud ? ud->uid : NULL), 1, modes, NULL);
	}
	else
	{
		u = do_nick(buf, nick, user, host,
		            ServerName, real, time(NULL), 0, ipchar, host,
		            NULL, 1, modes, NULL);
	}
	free(ipchar);

	if (u)
	{
		u->isservice++;
		if (!is_excluded(u))
		{
			make_exclude(u->nick);
		}
	}
}

/*************************************************************************/

void denora_cmd_notice(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	else
	{
		return;
	}
	ircdproto.ircd_cmd_notice(source, dest, buf);
}

/*************************************************************************/

void denora_cmd_action(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	char actionbuf[BUFSIZE];

	*buf = '\0';
	*actionbuf = '\0';

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	else
	{
		return;
	}

	ircsnprintf(actionbuf, BUFSIZE - 1, "%cACTION %s %c", 1, buf, 1);
	ircdproto.ircd_cmd_privmsg(source, dest, actionbuf);
}

/*************************************************************************/

void denora_cmd_notice2(char *source, char *dest, char *msg)
{
	ircdproto.ircd_cmd_notice(source, dest, msg);
}

/*************************************************************************/

void denora_cmd_privmsg(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	*buf = '\0';

	if (!source || !*source || !dest || !*dest)
	{
		return;
	}

	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto.ircd_cmd_privmsg(source, dest, buf);
}

/*************************************************************************/

void denora_cmd_privmsg2(char *source, char *dest, char *msg)
{
	ircdproto.ircd_cmd_privmsg(source, dest, msg);
}

/*************************************************************************/

void denora_cmd_serv_notice(char *source, char *dest, char *msg)
{
	ircdproto.ircd_cmd_serv_notice(source, dest, msg);
}

/*************************************************************************/

void denora_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
	ircdproto.ircd_cmd_serv_privmsg(source, dest, msg);
}

/*************************************************************************/

void denora_cmd_quit(char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto.ircd_cmd_quit(source, buf);
}

/*************************************************************************/

void denora_cmd_pong(char *servname, char *who)
{
	ircdproto.ircd_cmd_pong(servname, who);
}

/*************************************************************************/

void denora_cmd_join(char *user, char *channel, time_t chantime)
{
	char *chan[1];

	if (!BadChanName(channel))
	{
		ircdproto.ircd_cmd_join(user, channel, chantime);
		if (!LargeNet)
		{
			chan[0] = sstrdup(channel);
			do_join(user, 1, chan);
			free(chan[0]);
		}
		if (AutoOp && AutoMode)
			denora_automode(channel);
	}
}

/*************************************************************************/

void denora_cmd_part(char *nick, char *chan, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	char *v[128];

	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	if (!nick || !chan)
	{
		return;
	}
	v[0] = sstrdup(chan);
	v[1] = sstrdup(buf);
	ircdproto.ircd_cmd_part(nick, chan, buf);
	do_part(nick, 2, v);
	free(v[0]);
	free(v[1]);
}

/*************************************************************************/

void denora_cmd_global(char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto.ircd_cmd_global((source ? source : ServerName), buf);
}

/*************************************************************************/

void denora_cmd_squit(char *servname, char *message)
{
	if (!servname || !message)
	{
		return;
	}
	ircdproto.ircd_cmd_squit(servname, message);
}

/*************************************************************************/

/**
 * Send a series of commands needed to connect
 *
 * @return void - no returend value
 *
 */
void denora_cmd_connect(void)
{
	if (ircdproto.ircd_cmd_connect)
	{
		ircdproto.ircd_cmd_connect();
	}
}

/*************************************************************************/

void denora_cmd_stats(char *sender, const char *letter, char *server)
{
	ircdproto.ircd_cmd_stats(sender, letter, server);
}

/*************************************************************************/

/**
 * Send EOB (end of burst)
 *
 * @return void - no returend value
 *
 */
void denora_cmd_eob(void)
{
	if (ircdproto.ircd_cmd_eob)
	{
		ircdproto.ircd_cmd_eob();
	}
}

/*************************************************************************/

/**
 * Send VERSION command
 *
 * @param server is the version to send the version command to
 * @return void - no returend value
 *
 */
void denora_cmd_version(char *server)
{
	if (ircdproto.ircd_cmd_version)
	{
		if (server)
		{
			ircdproto.ircd_cmd_version(server);
		}
	}
}

/*************************************************************************/

void denora_cmd_ctcp(char *source, char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];
	char *s;

	*buf = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}

	s = normalizeBuffer(buf);

	ircdproto.ircd_cmd_ctcp(source, dest, s);
	free(s);
}

/*************************************************************************/

/**
 * Set the function that will send the NICK command on nick changes
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void
pmodule_cmd_nick(void (*func) (char *nick, char *name, const char *modes))
{
	if (func)
	{
		ircdproto.ircd_cmd_nick = func;
	}
}

/*************************************************************************/

/**
 * Set the function that will send the VERSION command
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void pmodule_cmd_version(void (*func) (char *server))
{
	if (func)
	{
		ircdproto.ircd_cmd_version = func;
	}
}

/*************************************************************************/

/**
 * Set the function that will send the MODE command
 *
 * @param func is a function pointer which takes the argument of source, dest, buf
 * @return void - no returend value
 *
 */
void pmodule_cmd_mode(void (*func) (char *source, char *dest, char *buf))
{
	ircdproto.ircd_cmd_mode = func;
}

/*************************************************************************/

void pmodule_cmd_bot_nick(void (*func)
                          (char *nick, char *user, char *host, char *real,
                           char *modes))
{
	ircdproto.ircd_cmd_bot_nick = func;
}

/*************************************************************************/

/**
 * Set the function that will send the NOTICE command
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void pmodule_cmd_notice(void (*func) (char *source, char *dest, char *buf))
{
	if (func)
	{
		ircdproto.ircd_cmd_notice = func;
	}
}

/*************************************************************************/

void pmodule_set_mod_current_buffer(void (*func) (int ac, char **av))
{
	ircdproto.ircd_set_mod_current_buffer = func;
}

/*************************************************************************/

/**
 * Set the function that will send the PRIVMSG command
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void
pmodule_cmd_privmsg(void (*func) (char *source, char *dest, char *buf))
{
	if (func)
	{
		ircdproto.ircd_cmd_privmsg = func;
	}
}

/*************************************************************************/

void
pmodule_cmd_serv_notice(void (*func) (char *source, char *dest, char *msg))
{
	ircdproto.ircd_cmd_serv_notice = func;
}

/*************************************************************************/

void pmodule_cmd_serv_privmsg(void (*func)
                              (char *source, char *dest, char *msg))
{
	ircdproto.ircd_cmd_serv_privmsg = func;
}

/*************************************************************************/

/**
 * Set the function that will send the PING command
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void pmodule_cmd_ping(void (*func) (char *server))
{
	if (func)
	{
		ircdproto.ircd_cmd_ping = func;
	}
}

/*************************************************************************/

/**
 * Set the function that will send the MOTD command
 *
 * @param func is a function pointer which takes the argument of server
 * @return void - no returend value
 *
 */
void pmodule_cmd_motd(void (*func) (char *sender, char *server))
{
	if (func)
	{
		ircdproto.ircd_cmd_motd = func;
	}
}

/*************************************************************************/

void pmodule_cmd_quit(void (*func) (char *source, char *buf))
{
	ircdproto.ircd_cmd_quit = func;
}

/*************************************************************************/

void pmodule_cmd_pong(void (*func) (char *servname, char *who))
{
	ircdproto.ircd_cmd_pong = func;
}

/*************************************************************************/

void
pmodule_cmd_join(void (*func) (char *user, char *channel, time_t chantime))
{
	ircdproto.ircd_cmd_join = func;
}

/*************************************************************************/

void pmodule_cmd_part(void (*func) (char *nick, char *chan, char *buf))
{
	ircdproto.ircd_cmd_part = func;
}

/*************************************************************************/

void pmodule_cmd_global(void (*func) (char *source, char *buf))
{
	ircdproto.ircd_cmd_global = func;
}

/*************************************************************************/

void pmodule_cmd_squit(void (*func) (char *servname, char *message))
{
	ircdproto.ircd_cmd_squit = func;
}

/*************************************************************************/

void pmodule_cmd_stats(void (*func)
                       (char *sender, const char *letter, char *server))
{
	ircdproto.ircd_cmd_stats = func;
}

/*************************************************************************/

void pmodule_cmd_connect(void (*func) (void))
{
	ircdproto.ircd_cmd_connect = func;
}

/*************************************************************************/

void pmodule_cmd_ctcp(void (*func) (char *source, char *dest, char *buf))
{
	ircdproto.ircd_cmd_ctcp = func;
}

/*************************************************************************/

void pmodule_cmd_eob(void (*func) (void))
{
	ircdproto.ircd_cmd_eob = func;
}

/*************************************************************************/

void pmodule_ircd_var(IRCDVar * ircdvar)
{
	ircd = ircdvar;
}

/*************************************************************************/

void pmodule_ircd_cap(IRCDCAPAB * cap)
{
	ircdcap = cap;
}

/*************************************************************************/

void pmodule_ircd_version(const char *version)
{
	denora->version_protocol = sstrdup(version);
}

/*************************************************************************/

void pmodule_ircd_useTSMode(int use)
{
	UseTSMODE = use;
}

/*************************************************************************/

void pmodule_oper_umode(int mode)
{
	ircd_modes.user_oper = mode;
}

/*************************************************************************/

int denora_get_oper_mode()
{
	return ircd_modes.user_oper;
}

/*************************************************************************/

void pmodule_irc_var(uint32 mode)
{
	ircd_modes.ircd_var = mode;
}

/*************************************************************************/

/**
 * Return the hex value for the IRCD
 *
 * @return integer in hex of the ircd, if not set it returns IRC_OTHER
 *
 */
uint32 denora_get_ircd()
{
	if (ircd_modes.ircd_var)
	{
		return ircd_modes.ircd_var;
	}
	else
	{
		return IRC_OTHER;
	}
}

/*************************************************************************/
