/* IrcDreams P10 IRCD
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 */

void ircdreams_cmd_nick(char *nick, char *name, const char *modes);
void ircdreams_cmd_mode(char *source, char *dest, char *buf);
void ircdreams_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void ircdreams_cmd_notice(char *source, char *dest, char *buf);
void ircdreams_cmd_privmsg(char *source, char *dest, char *buf);
void ircdreams_cmd_serv_notice(char *source, char *dest, char *msg);
void ircdreams_cmd_serv_privmsg(char *source, char *dest, char *msg);
void ircdreams_cmd_quit(char *source, char *buf);
void ircdreams_cmd_pong(char *servname, char *who);
void ircdreams_cmd_join(char *user, char *channel, time_t chantime);
void ircdreams_cmd_server(char *servname, int hop, char *descript);
void ircdreams_cmd_part(char *nick, char *chan, char *buf);
void ircdreams_cmd_global(char *source, char *buf);
void ircdreams_cmd_squit(char *servname, char *message);
void ircdreams_cmd_connect(void);
void ircdreams_cmd_eob(void);
void ircdreams_cmd_capab(void);
void ircdreams_cmd_ctcp(char *source, char *dest, char *buf);
void ircdreams_cmd_pass(char *pass);
void ircdreams_cmd_version(char *server);
void ircdreams_cmd_stats(char *sender, const char *letter, char *server);
void ircdreams_cmd_motd(char *sender, char *server);
void ircdreams_cmd_ping(char *server);

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
int denora_event_sgline(char *source, int ac, char **av);
int denora_event_chghost(char *source, int ac, char **av);
void IRCDModeInit(void);
int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
char *ircdreams_nickip(char *host);

