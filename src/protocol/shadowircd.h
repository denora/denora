/* ShadowIRCd 6.0+ functions
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: shadowircd.h ??? 2012-07-06 20:24:56Z Joah $
 *
 * Comments on modes taken from http://www.charybdis.be/oper-guide/book1.html
 * Knowledge of ShadowIRCd-specific modes taken from https://github.com/shadowircd/shadowircd/blob/master/help/opers/cmode
 * and https://github.com/shadowircd/shadowircd/blob/master/help/opers/umode
 * Knowledge of ShadowIRCd-specific server capabilities (CAPAB) taken from
 * https://github.com/shadowircd/shadowircd/blob/master/doc/technical/capab.txt
 *
 */

void shadowircd_cmd_tmode(char *source, char *dest, const char *fmt, ...);
void shadowircd_cmd_nick(char *nick, char *name, const char *modes);
void shadowircd_cmd_mode(char *source, char *dest, char *buf);
void shadowircd_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void shadowircd_cmd_notice(char *source, char *dest, char *buf);
void shadowircd_cmd_privmsg(char *source, char *dest, char *buf);
void shadowircd_cmd_serv_notice(char *source, char *dest, char *msg);
void shadowircd_cmd_serv_privmsg(char *source, char *dest, char *msg);
void shadowircd_cmd_quit(char *source, char *buf);
void shadowircd_cmd_pong(char *servname, char *who);
void shadowircd_cmd_join(char *user, char *channel, time_t chantime);
void shadowircd_cmd_part(char *nick, char *chan, char *buf);
void shadowircd_cmd_global(char *source, char *buf);
void shadowircd_cmd_squit(char *servname, char *message);
void shadowircd_cmd_connect(void);
void shadowircd_cmd_eob(void);
void shadowircd_cmd_ctcp(char *source, char *dest, char *buf);
void shadowircd_cmd_version(char *server);
void shadowircd_cmd_stats(char *sender, const char *letter, char *server);
void shadowircd_cmd_motd(char *sender, char *server);
void shadowircd_cmd_ping(char *server);
void shadowircd_cmd_pass(char *pass);
void shadowircd_cmd_server(char *servname, int hop, char *descript);
void shadowircd_cmd_svinfo(void);
void shadowircd_cmd_svsinfo(void);
void shadowircd_cmd_capab(void);
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
int denora_event_euid(char *source, int ac, char **av);
int denora_event_topic(char *source, int ac, char **av);
int denora_event_whois(char *source, int ac, char **av);
int denora_event_part(char *source, int ac, char **av);
int denora_event_server(char *source, int ac, char **av);
int denora_event_nick(char *source, int ac, char **av);
int denora_event_privmsg(char *source, int ac, char **av);
int denora_event_capab(char *source, int ac, char **av);
int denora_event_sjoin(char *source, int ac, char **av);
int denora_event_svinfo(char *source, int ac, char **av);
int denora_event_pass(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_tmode(char *source, int ac, char **av);
int denora_event_bmask(char *source, int ac, char **av);
int denora_event_sid(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);
int denora_event_tburst(char *source, int ac, char **av);
int denora_event_kline(char *source, int ac, char **av);
int denora_event_unkline(char *source, int ac, char **av);
int denora_event_xline(char *source, int ac, char **av);
int denora_event_unxline(char *source, int ac, char **av);
int denora_event_encap(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
