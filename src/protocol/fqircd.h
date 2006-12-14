/* FreeQuest IRCd 2.1.*
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: fqircd.h 627 2006-07-23 14:05:55Z trystan $
 *
 * last tested against fqircd-2.1(134902769)
 */

/*************************************************************************/

void fqircd_cmd_nick(char *nick, char *name, const char *modes);
void fqircd_cmd_mode(char *source, char *dest, char *buf);
void fqircd_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void fqircd_cmd_notice(char *source, char *dest, char *buf);
void fqircd_cmd_privmsg(char *source, char *dest, char *buf);
void fqircd_cmd_serv_notice(char *source, char *dest, char *msg);
void fqircd_cmd_serv_privmsg(char *source, char *dest, char *msg);
void fqircd_cmd_quit(char *source, char *buf);
void fqircd_cmd_pong(char *servname, char *who);
void fqircd_cmd_join(char *user, char *channel, time_t chantime);
void fqircd_cmd_part(char *nick, char *chan, char *buf);
void fqircd_cmd_global(char *source, char *buf);
void fqircd_cmd_squit(char *servname, char *message);
void fqircd_cmd_connect(void);
void fqircd_cmd_eob(void);
void fqircd_cmd_ctcp(char *source, char *dest, char *buf);
void fqircd_cmd_version(char *server);
void fqircd_cmd_stats(char *sender, const char *letter, char *server);
void fqircd_cmd_motd(char *sender, char *server);
void fqircd_cmd_ping(char *server);
void fqircd_cmd_pass(char *pass);
void fqircd_cmd_server(char *servname, int hop, char *descript);
void fqircd_cmd_capab(void);
void fqircd_cmd_svinfo(void);
void fqircd_cmd_burst(void);
void IRCDModeInit(void);
int denora_event_436(char *source, int ac, char **av);
int denora_event_away(char *source, int ac, char **av);
int denora_event_ping(char *source, int ac, char **av);
int denora_event_motd(char *source, int ac, char **av);
int denora_event_join(char *source, int ac, char **av);
int denora_event_kick(char *source, int ac, char **av);
int denora_event_kill(char *source, int ac, char **av);
int denora_event_mode(char *source, int ac, char **av);
int denora_event_quit(char *source, int ac, char **av);
int denora_event_squit(char *source, int ac, char **av);
int denora_event_topic(char *source, int ac, char **av);
int denora_event_whois(char *source, int ac, char **av);
int denora_event_part(char *source, int ac, char **av);
int denora_event_server(char *source, int ac, char **av);
int denora_event_nick(char *source, int ac, char **av);
int denora_event_privmsg(char *source, int ac, char **av);
int denora_event_capab(char *source, int ac, char **av);
int denora_event_sjoin(char *source, int ac, char **av);
int denora_event_svinfo(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_burst(char *source, int ac, char **av);
int denora_event_sqline(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
char *fqircd_nickip(char *host);
