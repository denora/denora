/*************************************************************************/
/* Asuka P10 ircd Protocol                                               */
/* (c) 2004-2012 Denora Team                                             */
/* Contact us at info@denorastats.org                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/* $Id$                      */
/*                                                                       */
/*************************************************************************/

void asuka_cmd_nick(char *nick, char *name, const char *modes);
void asuka_cmd_mode(char *source, char *dest, char *buf);
void asuka_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void asuka_cmd_notice(char *source, char *dest, char *buf);
void asuka_cmd_privmsg(char *source, char *dest, char *buf);
void asuka_cmd_serv_notice(char *source, char *dest, char *msg);
void asuka_cmd_serv_privmsg(char *source, char *dest, char *msg);
void asuka_cmd_quit(char *source, char *buf);
void asuka_cmd_pong(char *servname, char *who);
void asuka_cmd_join(char *user, char *channel, time_t chantime);
void asuka_cmd_server(char *servname, int hop, char *descript);
void asuka_cmd_part(char *nick, char *chan, char *buf);
void asuka_cmd_global(char *source, char *buf);
void asuka_cmd_squit(char *servname, char *message);
void asuka_cmd_connect(void);
void asuka_cmd_eob(void);
void asuka_cmd_ctcp(char *source, char *dest, char *buf);
void asuka_cmd_pass(char *pass);
void asuka_cmd_version(char *server);
void asuka_cmd_stats(char *sender, const char *letter, char *server);
void asuka_cmd_motd(char *sender, char *server);
void asuka_cmd_ping(char *server);

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
int denora_event_sjoin(char *source, int ac, char **av);
int denora_event_eob(char *source, int ac, char **av);
int denora_event_pass(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_create(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);
int denora_event_gline(char *source, int ac, char **av);
int denora_event_jupe(char *source, int ac, char **av);
int denora_event_clearmode(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
char *asuka_nickip(char *host);
void IRCDModeInit(void);

int asuka_parse_lkill(char *message);
char *asuka_lkill_killer(char *message);
char *asuka_lkill_msg(char *message);
