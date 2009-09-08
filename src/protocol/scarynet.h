/* ScaryNet ircu IRCD functions
 *
 * (C) 2006 Alexander Maassen
 * Contact me at outsider@scarynet.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: scarynet.h 627 2006-07-23 14:05:55Z trystan $
 *
 */

void scarynet_cmd_nick(char *nick, char *name, const char *modes);
void scarynet_cmd_mode(char *source, char *dest, char *buf);
void scarynet_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void scarynet_cmd_notice(char *source, char *dest, char *buf);
void scarynet_cmd_privmsg(char *source, char *dest, char *buf);
void scarynet_cmd_serv_notice(char *source, char *dest, char *msg);
void scarynet_cmd_serv_privmsg(char *source, char *dest, char *msg);
void scarynet_cmd_quit(char *source, char *buf);
void scarynet_cmd_pong(char *servname, char *who);
void scarynet_cmd_join(char *user, char *channel, time_t chantime);
void scarynet_cmd_server(char *servname, int hop, char *descript);
void scarynet_cmd_part(char *nick, char *chan, char *buf);
void scarynet_cmd_global(char *source, char *buf);
void scarynet_cmd_squit(char *servname, char *message);
void scarynet_cmd_connect(void);
void scarynet_cmd_eob(void);
void scarynet_cmd_capab(void);
void scarynet_cmd_ctcp(char *source, char *dest, char *buf);
void scarynet_cmd_pass(char *pass);
void scarynet_cmd_version(char *server);
void scarynet_cmd_stats(char *sender, const char *letter, char *server);
void scarynet_cmd_motd(char *sender, char *server);
void scarynet_cmd_ping(char *server);
void IRCDModeInit(void);

int denora_event_436(char *source, int ac, char **av);
int denora_event_account(char *source, int ac, char **av);
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
int denora_event_gnotice(char *source, int ac, char **av);
int denora_event_privmsg(char *source, int ac, char **av);
int denora_event_sjoin(char *source, int ac, char **av);
int denora_event_eob(char *source, int ac, char **av);
int denora_event_pass(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_globops(char *source, int ac, char **av);
int denora_event_burst(char *source, int ac, char **av);
int denora_event_create(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);
int denora_event_gline(char *source, int ac, char **av);
int denora_event_jupe(char *source, int ac, char **av);
int denora_event_rping(char *source, int ac, char **av);
int denora_event_rpong(char *source, int ac, char **av);
int denora_event_clearmode(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
char *scarynet_nickip(char *host);

int scarynet_parse_lkill(char *message);
char *scarynet_lkill_killer(char *message);
char *scarynet_lkill_msg(char *message);
