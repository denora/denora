/* Unreal 3.1.x
 *
 * (C) 2004-2013 Denora Team
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

void unreal31_cmd_nick(char *nick, char *name, const char *modes);
void unreal31_cmd_mode(char *source, char *dest, char *buf);
void unreal31_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void unreal31_cmd_notice(char *source, char *dest, char *buf);
void unreal31_cmd_privmsg(char *source, char *dest, char *buf);
void unreal31_cmd_serv_notice(char *source, char *dest, char *msg);
void unreal31_cmd_serv_privmsg(char *source, char *dest, char *msg);
void unreal31_cmd_quit(char *source, char *buf);
void unreal31_cmd_pong(char *servname, char *who);
void unreal31_cmd_join(char *user, char *channel, time_t chantime);
void unreal31_cmd_part(char *nick, char *chan, char *buf);
void unreal31_cmd_global(char *source, char *buf);
void unreal31_cmd_squit(char *servname, char *message);
void unreal31_cmd_connect(void);
void unreal31_cmd_eob(void);
void unreal31_cmd_capab(void);
void unreal31_cmd_ctcp(char *source, char *dest, char *buf);
void unreal31_cmd_stats(char *sender, const char *letter, char *server);
void unreal31_cmd_motd(char *sender, char *servr);
void unreal31_cmd_ping(char *server);
void unreal31_cmd_pass(char *pass);
void unreal31_cmd_server(char *servname, int hop, char *descript);
void unreal31_cmd_version(char *server);
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
int denora_event_chghost(char *source, int ac, char **av);
int denora_event_sethost(char *source, int ac, char **av);
int denora_event_chgident(char *source, int ac, char **av);
int denora_event_setident(char *source, int ac, char **av);
int denora_event_chgname(char *source, int ac, char **av);
int denora_event_setname(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
