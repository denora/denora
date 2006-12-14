/* Ptlink
 *
 * (C) 2004-2006 Denora Team
 * Contact us at info@nomadirc.net
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: ptlink.h 627 2006-07-23 14:05:55Z trystan $
 *
 */

/*
   The following variables are set to define the TS protocol version
   that we support. 

   PTLink 6.14 to 6.17  TS CURRENT is 6  and MIN is 3
   PTlink 6.18          TS CURRENT is 9  and MIN is 3
   PTLink 6.19 		TS CURRENT is 10 and MIN is 9

   If you are running 6.18 or 6.19 do not touch these values as they will
   allow you to connect

   If you are running an older version of PTLink, first think about updating
   your ircd, or changing the TS_CURRENT to 6 to allow services to connect
*/

#define PTLINK_TS_CURRENT 9
#define PTLINK_TS_MIN 3

void ptlink_cmd_nick(char *nick, char *name, const char *modes);
void ptlink_cmd_mode(char *source, char *dest, char *buf);
void ptlink_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void ptlink_cmd_notice(char *source, char *dest, char *buf);
void ptlink_cmd_privmsg(char *source, char *dest, char *buf);
void ptlink_cmd_serv_notice(char *source, char *dest, char *msg);
void ptlink_cmd_serv_privmsg(char *source, char *dest, char *msg);
void ptlink_cmd_quit(char *source, char *buf);
void ptlink_cmd_pong(char *servname, char *who);
void ptlink_cmd_join(char *user, char *channel, time_t chantime);
void ptlink_cmd_part(char *nick, char *chan, char *buf);
void ptlink_cmd_global(char *source, char *buf);
void ptlink_cmd_squit(char *servname, char *message);
void ptlink_cmd_connect(void);
void ptlink_cmd_eob(void);
void ptlink_cmd_ctcp(char *source, char *dest, char *buf);
void ptlink_cmd_version(char *server);
void ptlink_cmd_stats(char *sender, const char *letter, char *server);
void ptlink_cmd_motd(char *sender, char *server);
void ptlink_cmd_ping(char *server);
void ptlink_cmd_pass(char *pass);
void ptlink_cmd_server(char *servname, int hop, char *descript);
void ptlink_cmd_svsinfo(void);
void ptlink_cmd_svinfo(void);
void ptlink_cmd_capab(void);
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
int denora_event_chghost(char *source, int ac, char **av);
int denora_event_svsinfo(char *source, int ac, char **av);
int denora_event_pass(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_globops(char *source, int ac, char **av);
int denora_event_akill(char *source, int ac, char **av);
int denora_event_rakill(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);
int denora_event_newmask(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
