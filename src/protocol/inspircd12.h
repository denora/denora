/* InspIRCD
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

/*************************************************************************/

void inspircd_cmd_nick(char *nick, char *name, const char *modes);
void inspircd_cmd_mode(char *source, char *dest, char *buf);
void inspircd_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void inspircd_cmd_notice(char *source, char *dest, char *buf);
void inspircd_cmd_privmsg(char *source, char *dest, char *buf);
void inspircd_cmd_serv_notice(char *source, char *dest, char *msg);
void inspircd_cmd_serv_privmsg(char *source, char *dest, char *msg);
void inspircd_cmd_quit(char *source, char *buf);
void inspircd_cmd_pong(char *servname, char *who);
void inspircd_cmd_join(char *user, char *channel, time_t chantime);
void inspircd_cmd_part(char *nick, char *chan, char *buf);
void inspircd_cmd_global(char *source, char *buf);
void inspircd_cmd_squit(char *servname, char *message);
void inspircd_cmd_connect(void);
void inspircd_cmd_eob(void);
void inspircd_cmd_capab(void);
void inspircd_cmd_ctcp(char *source, char *dest, char *buf);
void inspircd_cmd_stats(char *sender, const char *letter, char *server);
void inspircd_cmd_motd(char *sender, char *servr);
void inspircd_cmd_ping(char *server);
void inspircd_cmd_pass(char *pass);
void inspircd_cmd_server(char *servname, int hop, char *descript);
void inspircd_cmd_version(char *server);

int denora_event_away(char *source, int ac, char **av);
int denora_event_ping(char *source, int ac, char **av);
int denora_event_motd(char *source, int ac, char **av);
int denora_event_join(char *source, int ac, char **av);
int denora_event_kick(char *source, int ac, char **av);
int denora_event_kill(char *source, int ac, char **av);
int denora_event_mode(char *source, int ac, char **av);
int denora_event_fmode(char *source, int ac, char **av);
int denora_event_quit(char *source, int ac, char **av);
int denora_event_squit(char *source, int ac, char **av);
int denora_event_topic(char *source, int ac, char **av);
int denora_event_part(char *source, int ac, char **av);
int denora_event_whois(char *source, int ac, char **av);
int denora_event_server(char *source, int ac, char **av);
int denora_event_nick(char *source, int ac, char **av);
int denora_event_uid(char *source, int ac, char **av);
int denora_event_metadata(char *source, int ac, char **av);
int denora_event_privmsg(char *source, int ac, char **av);
int denora_event_capab(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);

int denora_event_fjoin(char *source, int ac, char **av);
int denora_event_idle(char *source, int ac, char **av);
int denora_event_fhost(char *source, int ac, char **av);
int denora_event_setident(char *source, int ac, char **av);
int denora_event_chgident(char *source, int ac, char **av);
int denora_event_fname(char *source, int ac, char **av);
int denora_event_ftopic(char *source, int ac, char **av);
int denora_event_version(char *source, int ac, char **av);
int denora_event_opertype(char *source, int ac, char **av);

int denora_event_addline(char *source, int ac, char **av);
int denora_event_gline(char *source, int ac, char **av);
int denora_event_eline(char *source, int ac, char **av);
int denora_event_qline(char *source, int ac, char **av);
int denora_event_zline(char *source, int ac, char **av);
int denora_event_svsmode(char *source, int ac, char **av);
int denora_event_sanick(char *source, int ac, char **av);
int denora_event_svsjoin(char *source, int ac, char **av);
int denora_event_eob(char *source, int ac, char **av);
int denora_event_push(char *source, int ac, char **av);

int sjtoken(char *message, char delimiter, char **parv);
int inspircd12_parse_lkill(char *message);
int DenoraInit(int argc, char **argv);

void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
void IRCDModeInit(void);

char *inspircd12_lkill_killer(char *message);
char *inspircd12_lkill_msg(char *message);
