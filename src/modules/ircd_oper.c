/*
 * ircd_oper module for Denora 1.4+
 * (c) Jobe (jobe@mdbnet.co.uk)
 */

#include "denora.h"
#define AUTHOR "Jobe"
#define VERSION "1.0.1"
#define MYNAME "ircd_oper"

#define PRIVSONOPER "CHAN_LIMIT,MODE_LCHAN,DEOP_LCHAN,SHOW_INVIS,SHOW_ALL_INVIS,UNLIMIT_QUERY,KILL,LOCAL_KILL,GLINE,LOCAL_GLINE,ZLINE,LOCAL_ZLINE,SHUN,LOCAL_SHUN,JUPE,LOCAL_JUPE,OPMODE,LOCAL_OPMODE,WHOX,BADCHAN,LOCAL_BADCHAN,SEE_CHAN,PROPAGATE,DISPLAY,SEE_OPERS,LIST_CHAN,SEE_SECRET_CHAN,HIDE_IDLE,HIDE_CHANNELS"
#define PRIVSONADMIN "CHAN_LIMIT,MODE_LCHAN,DEOP_LCHAN,SHOW_INVIS,SHOW_ALL_INVIS,UNLIMIT_QUERY,KILL,LOCAL_KILL,REHASH,REMOTEREHASH,RESTART,DIE,GLINE,LOCAL_GLINE,ZLINE,LOCAL_ZLINE,SHUN,LOCAL_SHUN,JUPE,LOCAL_JUPE,OPMODE,LOCAL_OPMODE,SET,WHOX,BADCHAN,LOCAL_BADCHAN,SEE_CHAN,PROPAGATE,DISPLAY,SEE_OPERS,WIDE_GLINE,WIDE_ZLINE,WIDE_SHUN,LIST_CHAN,FORCE_OPMODE,FORCE_LOCAL_OPMODE,CHECK,SEE_SECRET_CHAN,WHOIS_NOTICE,HIDE_IDLE,HIDE_CHANNELS"
#define MODESONOPER "+owsg"
#define MODESONADMIN "+owsga"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int do_oper(char *source, int ac, char **av);

int DenoraInit(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
	Message *msg = NULL;
	int status;

	if (denora_get_ircd() == IRC_NEFARIOUS) {
		msg = createMessage("OPER", do_oper);
		status = moduleAddMessage(msg, MOD_HEAD);
		if (status != MOD_ERR_OK) {
			/* something went wrong say something about */
			alog(LOG_NORMAL, "[%s%s] unable to bind to 'OPER' error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
			return MOD_STOP;
		}
	} else {
		alog(LOG_NORMAL, "[%s%s] Youre IRCd is not supported, unloading!", MYNAME, MODULE_EXT);
		return MOD_STOP;
	}

	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);
	return MOD_CONT;
}

void DenoraFini(void)
{
	alog(LOG_NORMAL, "Unloading ircd_oper%s", MODULE_EXT);
}

int do_oper(char *source, int ac, char **av)
{
	User *u;
	Dadmin *aadmin;

	u = find_byuid(source);

	if (u) {
		if (ac >= 3) {
			aadmin = find_admin(av[1], u);

			if (!aadmin) {
				send_cmd(p10id, "491 %s :No O-lines for your host", u->uid);
			} else {
				if (ValidPassword(av[2], aadmin->passwd)) {
					char* parv[2];
					parv[0] = u->uid;
					if (aadmin->configfile == 1) {
						parv[1] = sstrdup(MODESONADMIN);
					} else {
						parv[1] = sstrdup(MODESONOPER);
					}

					do_umode(u->uid, 2, parv);

					if (aadmin->configfile == 1) {
						send_cmd(p10id, "PRIVS %s %s", u->uid, PRIVSONADMIN);
						send_cmd(p10id, "M %s %s", u->nick, MODESONADMIN);
						send_cmd(p10id, "SNO 1 :%s (%s@%s) is now an IRC Administrator (%s)", u->nick,
								u->username, u->host, aadmin->name);
					} else {
						send_cmd(p10id, "PRIVS %s %s", u->uid, PRIVSONOPER);
						send_cmd(p10id, "M %s %s", u->nick, MODESONOPER);
						send_cmd(p10id, "SNO 1 :%s (%s@%s) is now an IRC Operator (%s)", u->nick,
								u->username, u->host, aadmin->name);
					}

					send_cmd(p10id, "381 %s :You are now an IRC Operator", u->uid);
				} else {
					send_cmd(p10id, "464 %s :Password Incorrect", u->uid);
				}
			}
		} else {
			send_cmd(p10id, "461 %s OPER :Not enough parameters", u->uid);
		}
	}
  	return MOD_CONT;
}
