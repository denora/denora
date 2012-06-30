
/*
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

/*************************************************************************/

/* Main StatServ routine. */

void statserv_int(void)
{
	PrivMsg *p;

	p = createPrivmsg(s_StatServ, statserv);
	addPrivMsg(p);

	if (s_StatServ_alias && !LargeNet)
	{
		p = createPrivmsg(s_StatServ_alias, statserv);
		addPrivMsg(p);
	}
}

/*************************************************************************/

void statserv(User * u, char *buf)
{
	char *cmd, *buf2;
	char *str;

	buf2 = sstrdup(buf);
	cmd = myStrGetToken(buf, ' ', 0);
	str = myStrGetTokenRemainder(buf, ' ', 1);

	SET_SEGV_LOCATION();

	if (!cmd)
	{
		free(buf2);
		return;
	}
	else if (stricmp(cmd, "\1PING") == 0)
	{
		denora_cmd_ctcp(s_StatServ, u->nick, "PING %s", str);
	}
	else
	{
		SET_SEGV_LOCATION();
		/* CTCP VERSION that arrive this way are not ignored */
		if (!stricmp(cmd, "\1VERSION"))
		{
			free(buf2);
			free(cmd);
			if (str)
			{
				free(str);
			}
			return;
		}
		/* do not show the login password in the logchannel */
		/* there is a special log message in do_login */
		if (stricmp(cmd, "LOGIN") != 0 && stricmp(cmd, "ADMIN") != 0)
		{
			alog(LOG_NORMAL, "%s: %s: %s", s_StatServ, u->nick, buf2);
		}
		mod_run_cmd(s_StatServ, u, STATSERV, cmd, str);
	}
	free(cmd);
	if (str)
	{
		free(str);
	}
	free(buf2);
}

/*************************************************************************/
