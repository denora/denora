/*
 *
 * © 2004-2008 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id$
 *
 */

#ifndef EXTERN_H
#define EXTERN_H

/* IRC Variables */
E IRCDVar *ircd;
E IRCDCAPAB *ircdcap;
E STATVar stats[];
E DenoraVar denora[];

E void init_bans(void);
E Gline *new_Gline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason);
E Zline *new_Zline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason);
E Qline *new_Qline(char *user, char *host, char *setby, char *setat,
                 char *expires, char *reason);
E Gline *findGline(const char *host);
E Zline *findZline(const char *host);
E Qline *findQline(const char *host);
E void fini_bans(void);

E void initIrcdProto(void);

E int UseTSMODE;
E unsigned long umodes[128];
E unsigned long sjoinmodes[128];
E char csmodes[128];
E char *normalizeBuffer(char *);
E char *IRCDModule;
E int UseZIP;

E char *StatsPage;
E char *ChanStatsTrigger;

E int KeepUserTable;
E int UplinkSynced;
E int KeepServerTable;

E char *JupeMaster;

E int ircvsprintf(char *str, const char *pattern, va_list vl);
E int ircvsnprintf(char *str, size_t size, const char *pattern, va_list vl);
E int ircsprintf(char *str, const char *pattern, ...);
E int ircsnprintf(char *str, size_t size, const char *pattern, ...);
E char *itostr(int d);
E int in_str(char *str, char c);

/**** bans.c ****/

E const char *ban_char_to_action(char *val);
E void p10_gline(char *type, char *source, int ac, char **av);
E void sql_do_sqline(char *mask, char *reason);
E int sql_ban_clean(const char *name);
E void sql_do_sgline(char *length, char *mask);
E void sql_do_xline(char *geos, char *reason);
E void sql_do_unxline(char *geos);

/**** channels.c ****/

E Channel *chanlist[1024];
E int CompareCStats(const void *v, const void *cc);
E int CompareStatsChan(const void *v, const void *cc);
E list_t *CStatshead;
E list_t *StatsChanhead;
E void InitCStatsList(void);
E void InitStatsChanList(void);
E void Fini_ChannelStats(void);
E void Fini_StatsChannel(void);

E char *p10_mode_parse(char *mode, int *nomode);

E void add_ban(Channel * chan, char *mask);
E void chan_adduser2(User * user, Channel * c);
E void add_invite(Channel * chan, char *mask);
E void add_quiet(Channel * chan, char *mask);
E void chan_delete(Channel * c);
E void del_ban(Channel * chan, char *mask);
E void set_key(Channel * chan, char *value);
E void set_limit(Channel * chan, char *value);
E void set_rejoinlock(Channel * chan, char *value);
E void del_invite(Channel * chan, char *mask);
E void del_quiet(Channel * chan, char *mask);
E char *get_key(Channel * chan);
E char *get_limit(Channel * chan);
E char *get_rejoinlock(Channel * chan);
E Channel *chan_create(char *chan, time_t ts);
E Channel *join_user_update(User * user, Channel * chan, char *name, time_t chants);
E void add_exception(Channel * chan, char *mask);
E void del_exception(Channel * chan, char *mask);
E char *get_flood(Channel * chan);
E void set_flood(Channel * chan, char *value);
E char *get_redirect(Channel * chan);
E void set_redirect(Channel * chan, char *value);
E char *get_flood_alt(Channel * chan);
E void set_flood_alt(Channel * chan, char *value);
E void sql_channel_ban(int type, Channel *chan, char *mask);
E void sql_channel_exception(int type, Channel *chan, char *mask);
E void sql_channel_invite(int type, Channel *chan, char *mask);
E void sql_channel_quiet(int type, Channel * c, char *mask);

E StatsChannel *statschan_create(char *chan);
E StatsChannel *findstatschan(const char *chan);
E StatsChannel *next_statschan(void);
E StatsChannel *first_statschan(void);
E void StatsChannel_delete(StatsChannel * c);

E Exclude *next_exclude(void);
E Exclude *first_exclude(void);
E Exclude *exlists[1024];
E DENORA_INLINE boolean is_excluded(User *u);
E DENORA_INLINE int isExcludedServer(char *name);
E DENORA_INLINE boolean is_excludedserv(Server * server);

E void post_config_check(void);

E void load_exclude_db(void);
E void save_exclude_db(void);

E void save_stats_db(void);
E void load_stats_db(void);

E void load_chan_db(void);
E void save_chan_db(void);
E ChannelStats *makecs(char *mask);
E int del_cs(ChannelStats * cs);

E Channel *findchan(const char *chan);
E Channel *firstchan(void);
E Channel *nextchan(void);
E void chan_deluser(User * user, Channel * c);
E char *chan_get_modes(Channel * chan, int complete);
E void chan_set_modes(Channel * chan, int ac, char **av);

E void chan_remove_user_status(Channel * chan, User * user, int16 status);
E void chan_set_user_status(Channel * chan, User * user, int16 status);

E void do_cmode(const char *source, int ac, char **av);
E void do_join(const char *source, int ac, char **av);
E void do_p10_burst(char *source, int ac, char **av);
E void do_bmask(char **av);
E void do_kick(const char *source, int ac, char **av);
E void do_p10_kick(const char *source, int ac, char **av);
E void do_part(const char *source, int ac, char **av);
E void do_sjoin(const char *source, int ac, char **av);
E void do_topic(int ac, char **av);

/**** chanstats.c ***/

E void do_cstats(User *u, char *receiver, char *msg);
E void cstats_cron(time_t ts);
E void sumuser(User *u, char *user1, char *user2);
E void renameuser(User *u, char *user1, char *user2);
E int get_hour(void);
E User *finduser_by_sgroup(char *nick, char *sgroup);

/**** compat.c ****/

E int dfprintf(FILE *ptr, const char *fmt, ...);

#if !defined(HAVE_STRICMP)
E int stricmp(const char *s1, const char *s2);
E int strnicmp(const char *s1, const char *s2, size_t len);
#endif

E char *StrReverse(char *t);

char *sstrndup(const char *s, size_t n);
#if !defined(HAVE_STRNDUP)
E char *strndup(const char *s, size_t n);
#endif
#if !defined(HAVE_STRSPN)
E size_t strspn(const char *s, const char *accept);
#endif
#if !defined(HAVE_STRERROR)
E char *strerror(int errnum);
#endif
#if !defined(HAVE_STRSIGNAL)
E char *strsignal(int signum);
#endif
E unsigned char char_atribs[];

#if !defined(HAVE_STRLCPY)
E size_t strlcpy(char *dest, const char *src, size_t size);
#endif

#if !defined(HAVE_STRLCAT)
E size_t strlcat(char *dest, const char *src, size_t count);
#endif

#ifdef _WIN32
E int gettimeofday(struct timeval * tp, struct timezone * tzp);
#endif

/* since win32 doesn't have strict ansi building we can get away with this */
#ifdef __STRICT_ANSI__
E int inet_aton(const char *cp, struct in_addr *inp);
#endif
#ifdef _WIN32
E int inet_aton(const char *cp, struct in_addr *inp);
#endif

E size_t denora_strlen(const char *str);

E void denora_cron(time_t ts);
E void setup_cron_event(void);
E void cron_process_events(const char *name);
E int destroyCronEvent(CronEvent * evh);
E CronEvent *first_cronevent(void);
E CronEvent *next_cronevent(void);
E CronEventHash *first_croneventhash(void);
E CronEventHash *next_croneventhash(void);
E int destroyCronEventHash(CronEventHash * mh);
E int addCoreCronEvent(CronEventHash * hookEvtTable[], CronEvent * evh);
E int users_hourly(const char *name);
E int chans_hourly(const char *name);
E int servers_hourly(const char *name);

/**** config.c ****/

E char *AdminDB;
E Conf_Modules *new_modules;
E Conf_Modules *modules;
E Dadmin *find_admin(char * name, User * u);
E Dadmin *find_admin_byname(char *name);
E Dadmin *make_admin(char *mask);
E int free_admin(Dadmin * a);
E void merge_confs(void);
E void clear_newconfs(void);
E void confparse_error(const char *str, int lnum);

E int is_valid_server(char *name);

E int moduleGetConfigDirective(char *configfile, Directive * d);

E char *RemoteServer;
E int   RemotePort;
E char *RemotePassword;
E char *QuitPrefix;

E int NumExcludeServers;
E char **ExcludeServers;

E char *LocalHost;
E int   LocalPort;

E char *ServerName;
E char *ServerDesc;
E char *ServiceUser;
E char *ServiceHost;

E char *LogChannel;
E char *Smiley;
E char *NetworkName;
E const char *HiddenPrefix;
E char *HiddenSuffix;

E char *s_StatServ;
E char *desc_StatServ;

E char *s_StatServ_alias;
E char *desc_StatServ_alias;

E char *PIDFilename;
E char *MOTDFilename;
E char *OperDBName;
E char *HTMLFilename;
E int DisableMySQLOPT;

E int KeepBackups;
E int KeepBackupsFor;
E int NoBackupOkay;
E int ReadTimeout;
E int WarningTimeout;
E int UpdateTimeout;
E int BackupFreq;
E int HTMLTimeout;
E int UptimeTimeout;
E int PingFrequency;
E int SQLPingFreq;
E int ClearInActive;
E int ClearChanInActive;
E int 	 UserCacheTime;
E int   UserCleanFreq;
E int   ServerCacheTime;
E int   ServerCleanFreq;
E int   KeepLogs;
E int	NoLogs;
E int   DumpCore;
E int   UseTokens;
E int   UseTS6;
E char *Numeric;
E int LargeNet;
E int NickTracking;
E int UserStatsRegistered;
E char *TS6SID;
E char p10id[3];
E char *TS6UPLINK;
E char *uplink;
E char *NickChar;
E int CTCPUsers;
E int CTCPUsersEOB;
E int SP_HTML;
E int PartOnEmpty;

E int   StatsLanguage;
E int   HideStatsO;
E int HideUline;

E int ModulesNumber;
E int ModulesDelayedNumber;

E int HidePasswords;
E char *UserTable;
E char *ChanBansTable;
E char *ChanExceptTable;
E char *ChanQuietTable;
E char *IsOnTable;
E char *ServerTable;
E char *GlineTable;
E char *ChanTable;
E char *MaxValueTable;
E char *ChanInviteTable;
E char *CTCPTable;
E char *TLDTable;
E char *SglineTable;
E char *SqlineTable;
E char *AliasesTable;
E char *CStatsTable;
E char *UStatsTable;
E char *StatsTable;
E char *SpamTable;
E char *CurrentTable;
E char *ChanStatsTable;
E char *ServerStatsTable;
E char *AdminTable;

E char *ChannelDB;
E char *ctcpDB;
E char *ServerDB;
E char *ChannelStatsDB;
E char *TLDDB;
E char *excludeDB;
E char *statsDB;

E int rdb_init(void);
E int rdb_close(void);
E int rdb_clear_table(char *table);
E int rdb_direct_query(char *query);
E char *rdb_error_msg(void);
E char *rdb_errmsg;
E int rdb_check_table(char *table);

E char *SqlHost;
E char *SqlUser;
E char *SqlPass;
E char *SqlName;
E int SqlPort;
E char *SqlSock;
E int SqlRetries;
E int SqlRetryGap;
E int sqltype;
E int CSDefFlag;

E int AutoOp;
E char *AutoMode;

E int SQLDisableDueServerLost;
E int SQLRetryOnServerLost;


/**** confparse.c ****/

E tConf *mainconf;
E void initconfsettigs(void);
E int initconf(const char *filename, int reload, tConf *conftab);

/**** main.c ****/

E void introduce_user(const char *user);
E int init(int ac, char **av);
E void remove_pidfile(void);

E char **langtexts[NUM_LANGS];
E char *langnames[NUM_LANGS];
E int langlist[NUM_LANGS];
E void lang_init(void);
#define getstring(u,index) (langtexts[((u)&&((User*)u)->language?((User*)u)->language:StatsLanguage)][(index)])
#define getstring2(index) (langtexts[(StatsLanguage)][(index)])
E int strftime_lang(char *buf, int size, User * u, int format, struct tm *tm);
E void syntax_error(char *service, User * u, const char *command, int msgnum);
#define langstring(index) (langtexts[(StatsLanguage)][(index)])
#define langstr(index) (langtexts[(StatsLanguage)][(index)])

E int open_log(void);
E void close_log(void);
E void alog(int type, const char *fmt, ...);
E void log_perror(const char *fmt, ...);
E void fatal(const char *fmt, ...)		FORMAT(printf,1,2);
E void fatal_perror(const char *fmt, ...)	FORMAT(printf,1,2);

E char  inbuf[BUFSIZE];
E int   servsock;
E int 	waiting;
E int   started;

E void denora_shutdown(void);
E void denora_restart(void);

E void strtolwr(char *ch);

E int m_nickcoll(char *user);
E int m_away(char *source, char *msg);
E void m_kill(char *source, char *nick, char *msg);
E int m_motd(char *source);
E int m_privmsg(char *source, char *receiver, char *msg);
E int m_notice(char *source, char *receiver, char *msg);
E int m_stats(char *source, int ac, char **av);
E int m_whois(char *source, char *who);
E int m_time(char *source, int ac, char **av);
E int m_version(char *source, int ac, char **av);

E char *stristr(char *s1, char *s2);
E char *strnrepl(char *s, int32 size, const char *old, const char *new);
E char *merge_args(int argc, char **argv);
E void protocol_debug(char *source, int argc, char **argv);
E int match_wild(const char *pattern, const char *str);
E int match_wild_nocase(const char *pattern, const char *str);
E int match_usermask(const char *mask, User * user);
E int dotime(const char *s);

E char *myStrGetToken(const char *str, const char dilim, int token_number);
E char *myStrGetOnlyToken(const char *str, const char dilim, int token_number);
E char *myStrSubString(const char *src, int start, int end);
E char *myStrGetTokenRemainder(const char *str, const char dilim, int token_number);
E int myNumToken(const char *str, const char dilim);
E void doCleanBuffer(char *str);
E DENORA_INLINE boolean nickIsServices(char *nick);
E void moduleCallBackRun(void);
E void moduleCleanStruct(ModuleData **moduleData);

E void statserv(User *u, char *buf);

E int is_stats_admin(User *u);
E int split_buf(char *buf, char ***argv);
E void process(void);

E void send_cmd(const char *source, const char *fmt, ...)
	FORMAT(printf,2,3);
E void vsend_cmd(const char *source, const char *fmt, va_list args)
	FORMAT(printf,2,0);

E void notice_server(char *source, Server * s, char *fmt, ...)
	FORMAT(printf,3,4);
E void notice_user(char *source, User *u, const char *fmt, ...)
	FORMAT(printf,3,4);

E void notice_list(char *source, char *dest, char **text);
E void notice_lang(char *source, User *dest, int message, ...);
E void notice_help(char *source, User *dest, int message, ...);


E int new_open_db_read(DenoraDBFile * dbptr, char **key, char **value);
E int new_open_db_write(DenoraDBFile * dbptr);
E void new_close_db(FILE * fptr, char **key, char **value);
E int new_read_db_entry(char **key, char **value, FILE * fptr);
E int new_write_db_entry(const char *key, DenoraDBFile * dbptr, const char *fmt, ...);
E int new_write_db_endofblock(DenoraDBFile * dbptr);
E void fill_db_ptr(DenoraDBFile * dbptr, int version, int core_version, char *service, char *filename);

E char **buildStringList(char *src, int *number);

/**** servers.c ****/

E void count_kicks(User * kicker, User * kicked, Channel * c);
E void count_modes(User * u, Channel * c);
E void count_topics(User * u, Channel * c);

E Server *servlist;
E Server *me_server;
E Server *first_server(void);
E Server *next_server(void);
E ServStats *first_statsserver(void);
E ServStats *next_statserver(void);
E void sql_motd_store(Server *s);

E Server *server_find(const char *source);
E Server *findserver(Server * s, const char *name);
E Server *findserver_uid(Server *s, const char *name);
E ServStats *findserverstats(const char *name);
E void delete_server(Server * serv, const char *quitreason, int depth);
E void delete_serverstats(ServStats * serv);
E void load_server_db(void);
E void save_server_db(void);

E Server *do_server(const char *source, char *servername, char *hops, char *descript, char *numeric);
E void do_squit(char *servername);
E void capab_parse(int ac, char **av);

E ServStats *make_servstats(const char *server);

E char *ts6_uid_retrieve(void);
E void ts6_uid_increment(unsigned int slot);
E void ts6_uid_init(void);

/**** sockutil.c ****/

E int32 total_read, total_written;
E int32 total_sendmsg;
E int32 total_recmsg;
E int32 read_buffer_len(void);
E int32 write_buffer_len(void);

E int sgetc(deno_socket_t s);
E char *sgets(char *buf, int len, deno_socket_t s);
E char *sgets2(char *buf, int len, deno_socket_t s);
E int sread(deno_socket_t s, char *buf, int len);
E int sputs(char *str, deno_socket_t s);
E int sockprintf(deno_socket_t s, const char *fmt, ...);
E int conn(const char *host, int port, const char *lhost, int lport);
E void disconn(deno_socket_t s);

E CTCPVerStats *ctcplists[1024];
E void insert_ctcp(CTCPVerStats * c);
E CTCPVerStats *makectcp(char *version);
E int del_ctcpver(CTCPVerStats * c);
E void load_ctcp_db(void);
E void save_ctcp_db(void);
E CTCPVerStats *find_ctcpver(char *version);
E void handle_ctcp_version(char *nick, char *version);
E void sql_do_ctcp(int type, char *version, int count, int overall);
E void ctcp_update(char *version);
E list_t *CTCPhead;
E int sortctcp(const void *v, const void *v2);
E void init_ctcp(void);

E Dadmin *adminlists[1024];

/**** users.c ****/

E User *userlist[1024];

E void delete_user(User *user);

E User *finduser(const char *nick);
E User *user_find(const char *source);
E User *find_byuid(const char *uid);
E User *first_uid(void);
E User *next_uid(void);
E Uid *uid_first(void);
E Uid *uid_next(void);
E User *firstuser(void);
E User *nextuser(void);
E Uid *new_uid(const char *nick, char *uid);
E void delete_uid(Uid *u);
E Uid *find_uid(const char *nick);
E Uid *find_nickuid(const char *uid);

E void change_user_host(char *source, char *host);
E void change_user_username(char *source, char *username);
E void change_user_realname(char *source, char *realname);

E User *do_nick(const char *source, char *nick, char *username, char *host,
              char *server, char *realname, time_t ts, uint32 svid, char *ip, char *vhost, char *uid, int hopcount, char *modes, char *account);

E void do_umode(const char *source, int ac, char **av);
E void do_svsumode(int ac, char **av);
E void do_umode2(const char *source, int ac, char **av);
E void do_quit(const char *source, int ac, char **av);
E void do_kill(char *source, char *reason);

E int is_oper(User * user);
E int has_cmode(char *chan, int m);


#ifdef USE_MYSQL
/**** mysql.c ****/
E MYSQL       *mysql;
E MYSQL_FIELD *mysql_fields;
E MYSQL_ROW   mysql_row;

E int db_mysql_init(void);
E int db_mysql_open(void);
E int db_mysql_close(void);
E int db_mysql_query(char *sql);
E char *db_mysql_quote(char *sql);
E void dbMySQLPrepareForQuery(void);

#endif

#ifdef USE_POSTGRE
E PGconn *postgre;
E char *db_postgre_quote(char *sql);
#endif

/**** signals.c ****/

E void do_backtrace(int header);
E VOIDSIG sighandler(int signum);
E void signal_init(void);

E void privmsg(char *source, char *dest, const char *fmt, ...);
E void notice(char *source, char *dest, const char *fmt, ...);

/******************************************************************************/

E void ModuleIRCDName(const char *name);
E void ModuleIRCDOperModes(const char *name);

E void denora_cmd_numeric(char *source, int numeric, const char *fmt, ...);
E void denora_cmd_219(char *source, char *who); 			  	  		  /* 219 */
E void denora_cmd_318(char *source, char *who);           		  	  		  /* 318 */
E void denora_cmd_351(char *source);				  	  		  /* 351 */
E void denora_cmd_372(char *source, char *msg);			 	  		  /* 372 */
E void denora_cmd_422(char *source);				  	  		  /* 422 */
E void denora_cmd_375(char *source);				 	  		  /* 375 */
E void denora_cmd_376(char *source);				 	  		  /* 376 */
E void denora_cmd_391(char *source, char *timestr);                             		  /* 391 */
E void denora_cmd_401(char *source, char *who);			  	  		  /* 401 */
E void denora_cmd_capab(void);						  	  		  /* CAPAB */
E void denora_cmd_connect(void);                             	           	  /* Connect */
E void denora_cmd_global(char *source, const char *fmt, ...);         	  		  /* GLOBOPS */
E void denora_cmd_join(char *user, char *channel, time_t chantime);   	  		  /* JOIN */
E void denora_cmd_mode(char *source, char *dest, const char *fmt, ...);   	  		  /* MODE */
E void denora_cmd_netinfo(int ac, char **av);						  /* NETINFO */
E void denora_cmd_nick(char *nick, char *name, const char *mode);			  		  /* NICK */
E void denora_cmd_chg_nick(char *oldnick, char *newnick);             	  		  /* NICK */
E void denora_cmd_bot_nick(char *nick, char *user,char *host,char *real,char *modes);	  /* NICK */
E void denora_cmd_notice(char *source, char *dest, const char *fmt, ...);     		  /* NOTICE */
E void denora_cmd_action(char *source, char *dest, const char *fmt, ...);
E void denora_cmd_serv_notice(char *source, char *dest, char *msg);		  		  /* NOTICE */
E void denora_cmd_part(char *nick, char *chan, const char *fmt, ...); 	  		  /* PART */
E void denora_cmd_pass(char *pass);                                   	  		  /* PASS */
E void denora_cmd_pong(char *servname, char *who);                    	  		  /* PONG */
E void denora_cmd_privmsg(char *source, char *dest, const char *fmt, ...);    		  /* PRIVMSG */
E void denora_cmd_serv_privmsg(char *source, char *dest, char *msg);	  		  /* PRIVMSG */
E void denora_cmd_protoctl(void);                                         			  /* PROTOCTL */
E void denora_cmd_quit(char *source, const char *fmt, ...);           	  		  /* QUIT */
E void denora_cmd_server(char *servname, int hop, char *descript);    	  		  /* SERVER */
E void denora_cmd_squit(char *servname, char *message);               	  		  /* SQUIT  */
E void denora_cmd_svinfo(void);						  	  		  /* SVINFO */
E void denora_cmd_svsinfo(void);								  /* SVSINFO */
E void denora_cmd_eob(void);									  /* EOB - end of burst */
E void denora_cmd_burst(void);									  /* BURST  - use eob to send burst 0 */
E void denora_cmd_ctcp(char *source, char *dest, const char *fmt, ...);   	  		  /* CTCP */
E void denora_cmd_version(char *server);   	  		  /* VERSION */
E void denora_cmd_stats(char *sender, const char *letter, char *server);  /* STATS */
E void denora_motd(char *sender, char *server);
E void denora_cmd_ping(char *server);
E char *denora_cmd_nickip(char *var);
E void denora_cmd_notice2(char *source, char *dest, char *msg);
E void denora_cmd_privmsg2(char *source, char *dest, char *msg);

E void sql_uline(char *server);
E int denora_event_null(char *source, int ac, char **av);

#if 0
E void SortSwapArray(temp_data *table1, temp_data *table2);
E void BubbleSortData(temp_data table[], int size, int direction);
#endif

E int totalstatschannel;
E int totalctcpcount;

E char *str_signed(unsigned char *str);
E char *common_get_vident(User *u);
E char *common_get_vhost(User *u);
E const char *send_token(const char *token1, const char *token2);

/*
 * base64.c
 */
E char *base64enc(long i);                 /* base64 encode long int   */
E long base64dec(char *b64);               /* base64 decode char       */
E long base64dec_ts(char *ts);             /* base64 decode time stamp */
E char *encode_ip(unsigned char *ip);      /* base64 encode IP address */
E int b64_encode(char *src, size_t srclength, char *target, size_t targsize);
E int b64_decode(char *src, char *target, size_t targsize);
E int decode_ip(char *buf);
E const char *inttobase64(char *buf, unsigned int v, unsigned int count);
E unsigned int base64toIP(char *s);
E void buffer_new(struct buffer_st *b);
E void buffer_add(struct buffer_st *b, char c);
E void buffer_delete(struct buffer_st *b);
E void base64_encode(struct buffer_st *b, const char *source, int length);
E void base64_decode(struct buffer_st *bfr, const char *source, int length);

#define dfree(x) do { if (x) free(x); (x) = NULL; } while(0)

E char *militime_float(char* start);
E char *host_resolve(char *host);

E void db_connect(void);
E int rdb_query(int i, const char *fmt, ...);
E int db_checkserver(char *serv);
E int db_checkserver_online(char *serv);
E int db_getserver(char *serv);
E int db_getservfromnick(char *nick);
E int db_checknick(char *nick);
E int db_checknick_nt(char *nick);
E int db_getnick(char *nick);
E int db_getnick_unsure(char *nick);
E int rdb_insertid(void);
E int db_getchannel(char *chan);
E int db_getchancreate(char *chan);
E void db_removefromchans(int nickid);
E void db_removenick(char *nick, char *msg);
E void db_removenick_nt(char *nick, char *msg);
E void db_checkemptychan(int chanid);
E void db_cleanserver(void);
E void db_cleanuser(void);
E int db_getlusers(int type);
E int db_getchannel_users(char *chan);
E char *db_getchannel_byid(int chanid);

E void do_checkservsmax(void);
E void add_current_user(Server *s);
E void del_current_user(Server *s);
E void do_checkchansmax(void);
E void add_oper_count(User *u);
E void del_oper_count(User *u);

E Exclude *make_exclude(char *mask);
E Exclude *find_exclude(char *mask, char *server);
E void insert_exclude(Exclude * e);
E int del_exclude(Exclude * e);

E void server_store_pong(char *source, uint32 pingtime);
E void ping_servers(void);
E void uptime_servers(void);

E void update_sync_state(char *source, int ac);
E void sql_do_server(char *server, char *hops, char *comment, char *linkedto);
E void sql_do_squit(char *server);
E void sql_do_server_version(char *server, int ac, char **av);
E void sql_do_server_bans_remove(char *type, char *user, char *host);
E void sql_do_server_bans_add(char *type, char *user, char *host, char *setby, char *setat, char *expires, char *reason);
E void sql_do_server_spam_add(char *target, char *action,
                            char *setby, char *expires, char *setat, char *duration,
                            char *reason, char *regex);
E void sql_do_server_spam_remove(char *target, char *action, char *regex);
E void sql_do_nick(User *u);
E void sql_do_sdesc(char *user, char *msg);
E void server_set_desc(char *server, char *msg);
E void do_swhois(char *user, char *msg);
E void do_p10account(User *user, char *account, int flag);
E void sql_do_nick_chg(char *newnick, char *oldnick);
E void sql_do_addusers(int chanid, char *users);
E void sql_do_sjoin(char *chan, char *users, char **modes, int nbmodes);
E void sql_do_part(char *chan, User *u);
E void sql_do_partall(char *nick);
E void sql_do_join(char *chan, char *nick);
E void sql_do_usermodes(User *u, char *modes);
E void sql_reset_usermodes(int nickid, char *nickname);
E void sql_do_chanmodes(char *chan, char **av);
E void denora_set_umode(User * user, int ac, char **av);
E int denora_set_mod_current_buffer(int ac, char **av);
E void do_restart_denora(void);
E void sql_do_uptime(char *source, char *uptime);

E GeoIPRecord * GeoIP_record_by_addr (GeoIP* gi, const char *addr);
E GeoIPRecord * GeoIP_record_by_name (GeoIP* gi, const char *host);
E int GeoIP_record_id_by_addr (GeoIP* gi, const char *addr);
E int GeoIP_init_record_iter (GeoIP* gi);
E int GeoIP_next_record (GeoIP* gi, GeoIPRecord **gir, int *record_iter);
E void GeoIPRecord_delete (GeoIPRecord *gir);

E char *GeoIPDBFileName;
E const char * GeoIPDBDescription[NUM_DB_TYPES];
E const char *GeoIPCountryDBFileName;
E const char GeoIP_country_code[247][3];
E const char GeoIP_country_code3[247][4];
E const char * GeoIP_country_name[247];

E TLD *do_tld(char *country, char *code);
E TLD *findtld(const char *country);
E void load_tld_db(void);
E void save_tld_db(void);
E void tld_update(char *country_code);
E list_t *Thead;
E void init_tld(void);
E TLD *new_tld(const char *countrycode, const char *country);
E void fini_tld(void);
E int sortusers(const void *v, const void *v2);
E void save_databases(void);
E void backup_databases(void);

E char *char_encode(char *s1);

E FILE *new_xml(char *filename);
E void xml_write_header(FILE * ptr);
E void xml_write_footer(FILE * ptr);
E void xml_write_tag(FILE * ptr, const char *tag, char *xdata);
E char *xml_prepare_tag(char *tag, char *xdata);
E void xml_write_tag_int(FILE * ptr, const char *tag, long int xdata);
E void xml_write_block(FILE * ptr, char *block, char *xdata);
E void xml_write_block_top(FILE * ptr, const char *block);
E void xml_write_block_bottom(FILE * ptr, const char *block);
E void xml_moduleData(FILE * ptr, ModuleData ** md);
E char *xml_encode;
E char *xml_doctype;
E char *xml_header;

E void event_message_process(char *eventbuf);
E void eventprintf(const char *fmt, ...);
E void event_process_hook(const char *name, int argc, char **argv);
E void send_event(const char *name, int argc, ...);

E char segv_location[SEGV_LOCATION_BUFSIZE];

#undef PRETTY_FUNCTION
#ifdef HAVE_PRETTY_FUNCTION
#  define PRETTY_FUNCTION	__PRETTY_FUNCTION__
#elif HAVE_FUNCTION
#  define PRETTY_FUNCTION	__FUNCTION__
#elif HAVE_FUNC
#  define PRETTY_FUNCTION	__func__
#else
#  define PRETTY_FUNCTION  "unknown"
#endif

E FILE *logfile;
E int SpeedTest;

#define SET_SEGV_LOCATION() \
	segv_location[0]='\0'; \
	ircsnprintf(segv_location,SEGV_LOCATION_BUFSIZE,"File %s Line %d function %s", __FILE__, __LINE__, PRETTY_FUNCTION);

#define SET_SEGV_LOCATION_EXTRA(debug_text) \
	segv_location[0]='\0'; \
	ircsnprintf(segv_location,SEGV_LOCATION_BUFSIZE,"%s %d %s %s", __FILE__, __LINE__, PRETTY_FUNCTION,(debug_text));

#define SET_START_TIME() \
	if (SpeedTest) alog(LOG_DEBUG, "Execution Time started at %ld", (long int) time(NULL));
#define CHECK_END_TIME() \
	if (SpeedTest) alog(LOG_DEBUG, "Execution Time ended at %ld", (long int) time(NULL));

#define FUNC_SET_START_TIME() \
		alog(LOG_DEBUG, "Function Execution Time started at %ld (%s)", (long int) time(NULL), PRETTY_FUNCTION);
#define FUNC_CHECK_END_TIME() \
		alog(LOG_DEBUG, "Function Execution Time ended at %ld", (long int) time(NULL));


E void do_html(void);
E int toupper(char);
E int tolower(char);

E void insert_cs(ChannelStats * cs);
E void load_cs_db(void);
E void save_cs_db(void);
E ChannelStats *find_cs(char *mask);

E int denora_get_oper_mode(void);
E void pmodule_set_mod_current_buffer(void (*func) (int ac, char **av));
E void pmodule_cmd_bot_nick(void (*func) (char *nick, char *user, char *host, char *real, char *modes));
E void pmodule_cmd_mode(void (*func) (char *source, char *dest, char *buf));
E void pmodule_cmd_version(void (*func) (char *server));
E void pmodule_cmd_nick(void (*func) (char *nick, char *name, const char *modes));
E void pmodule_cmd_privmsg(void (*func) (char *source, char *dest, char *buf));
E void pmodule_cmd_serv_notice(void (*func) (char *source, char *dest, char *msg));
E void pmodule_cmd_quit(void (*func) (char *source, char *buf));
E void pmodule_ircd_version(const char *version);
E int denora_umode(int m);
E int denora_cmode(int m);
E void pmodule_ircd_umodes(unsigned long modes[128]);
E void pmodule_ircd_useTSMode(int use);
E void pmodule_cmd_connect(void (*func) (void));
E void pmodule_ircd_var(IRCDVar * ircdvar);
E void pmodule_irc_var(uint32 mode);
E void pmodule_irc_nickip(char *(*func) (char *data));
E uint32 denora_get_ircd(void);
E void pmodule_ircd_cap(IRCDCAPAB * cap);
E void pmodule_cmd_ctcp(void (*func) (char *source, char *dest, char *buf));
E void pmodule_cmd_part(void (*func) (char *nick, char *chan, char *buf));
E void pmodule_cmd_eob(void (*func) (void));
E void pmodule_cmd_squit(void (*func) (char *servname, char *message));
E void pmodule_cmd_global(void (*func) (char *source, char *buf));
E void pmodule_cmd_join(void (*func) (char *user, char *channel, time_t chantime));
E void pmodule_cmd_pong(void (*func) (char *servname, char *who));
E void pmodule_cmd_serv_privmsg(void (*func) (char *source, char *dest, char *msg));
E void pmodule_cmd_notice(void (*func) (char *source, char *dest, char *buf));
E void pmodule_oper_umode(int mode);
E void pmodule_cmd_stats(void (*func) (char *sender, const char *letter, char *server));
E void pmodule_cmd_motd(void (*func) (char *sender, char *server));
E void pmodule_cmd_ping(void (*func) (char *server));

E int zip_init(int compressionlevel);
E char *unzip_packet(const char *buffer, int length);
E char *zip_buffer(char *buffer, int *length, int flush);

#if !HAVE_GETHOSTBYNAME
E char *pack_ip(const char *ipaddr);
#endif

E int moduleCount(int all);
E int protocolModuleLoaded(void);
E void core_modules_init(void);
E int protocol_module_init(void);
E Module *mod_current_module;
E char *mod_current_module_name;
E User *mod_current_user;
E int mod_current_op;
E char *mod_current_buffer;

E void sql_do_tld(int type, char *code, char *country, int count,
                int overall);
E void delete_tld(TLD * t);

#ifdef __STRICT_ANSI__
#ifndef fileno
int fileno(FILE *stream);
#endif
#endif

#ifndef _WIN32
E adns_state adns;
#endif
E GeoIP *gi;

E void extsock_init(char *lhost, int lport);
E void extsock_process(void);
E int SockIPNumber;
E char **ExtSockIPs;
E int XMLRPC_Enable;
E char *XMLRPC_Host;
E int XMLRPC_Port;
E int buffered_write(deno_socket_t fd, char *buf, int len);
E char *xmlrpc_parse(char *buffer);
E char *xmlrpc_method(char *buffer);
E XMLRPCCmd *findXMLCommand(XMLRPCCmdHash * hookEvtTable[], const char *name);
E void xmlrpc_generic_error(deno_socket_t socket_fd, int code, const char *string, ...);
E void xmlrpc_process(deno_socket_t socket_fd, char *buffer);
E int xmlrpc_split_buf(char *buffer, char ***av);
E XMLRPCCmd *createXMLCommand(const char *name,
                           int (*func) (deno_socket_t fd, int ac, char **av));
E int addXMLCommand(XMLRPCCmdHash * hookEvtTable[], XMLRPCCmd * xml);
E int destroyXMLRPCCommand(XMLRPCCmd * xml);
E int destroyxmlrpchash(XMLRPCCmdHash * mh);
E XMLRPCCmdHash *next_xmlrpchash(void);
E XMLRPCCmdHash *first_xmlrpchash(void);
E XMLRPCCmd *first_xmlrpccmd(void);
E XMLRPCCmd *next_xmlrpccmd(void);
E int delXMLRPCCommand(XMLRPCCmdHash * msgEvtTable[], XMLRPCCmd * xml,
                 char *mod_name);
E void displayXMLRPCcmd(XMLRPCCmd * xml);
E int extsock_val_clnt(struct in_addr iaddr);
E void extsock_close(void);
E char *xmlrpc_write_header(int length);
E void xmlrpc_send(int socket_fd, int argc, ...);
E void displayXMLRPCFromHash(char *name);
E int moduleAddXMLRPCcmd(XMLRPCCmd * xml);
E int addCoreXMLRPCCmd(XMLRPCCmdHash * hookEvtTable[], XMLRPCCmd * xml);
E int moduleXMLRPCDel(const char *name);

E int xmlrpc_getlast_error(void);
E int xmlrpc_register_method(const char *name, int (*func) (deno_socket_t fd, int ac, char **av));
E int xmlrpc_unregister_method(const char *method);

E char *xmlrpc_array(int argc, ...);
E char *xmlrpc_double(char *buf, double value);
E char *xmlrpc_base64(char *buf, char *value);
E char *xmlrpc_boolean(char *buf, int value);
E char *xmlrpc_string(char *buf, char *value);
E char *xmlrpc_integer(char *buf, int value);
E char *xmlrpc_decode64(char *value);
E char *xmlrpc_time2date(char *buf, time_t t);

E int xmlrpc_error_code;

E int xmlrpc_set_options(int type, const char *value);
E char *xmlrpc_char_encode(char *outbuffer, char *s1);
E char *xmlrpc_decode_string(char *buf);

E char *sstrdup(const char *src);

E char *rdb_escape(char *ch);

E void handleModuleOperationQueue(void);
E int queueModuleLoad(char *name, User *u);
E int queueModuleUnload(char *name, User *u);

E void statserv_int(void);

E PrivMsg *findPrivMsg(char *name);
E PrivMsg *createPrivmsg(char *service, void (*func) (User *u, char *buf));
E int addPrivMsg(PrivMsg * m);
E int destroyPrivMsg(PrivMsg * m);
E int delPrivMsg(PrivMsg * m);

#ifndef va_copy
# ifdef __va_copy
#  define VA_COPY(DEST,SRC) __va_copy((DEST),(SRC))
# else
#  define VA_COPY(DEST, SRC) memcpy ((&DEST), (&SRC), sizeof(va_list))
# endif
#else
# ifdef HAVE_VA_LIST_AS_ARRAY
#   define VA_COPY(DEST,SRC) (*(DEST) = *(SRC))
# else
#   define VA_COPY(DEST, SRC) va_copy(DEST, SRC)
# endif
#endif

E void lang_destory(void);
#define USE_VAR(var) static char sizeof##var = sizeof(sizeof##var) + sizeof(var)

#ifdef _WIN32
E char *GetWindowsVersion(void);
E int SupportedWindowsVersion(void);
E char *WindowsCPUArchitecture(void);
#endif

E int QueueEntryInit(void);
E void queue_unlock(void *arg);
E void queue_lock(void);
E void queue_wait(void);
E void queue_signal(void);
E void *queue_thread_main(void *arg);
E int ThreadCount;
E int UseThreading;
E QueueEntry *AddQueueEntry(QueueEntry *qp, char *msg);
E QueueEntry *RemoveQueueEntry(QueueEntry *qp);
E void PrintQueueEntry(QueueEntry *qp);
E void ClearQueueEntry(QueueEntry *qp);
E QueueEntry *ExecuteQueue(QueueEntry *qp);

E QueueEntry *qp;

E void insert_admin(Dadmin * a);
E void load_admin_db(void);
E void save_admin_db(void);
E Dadmin *first_admin(void);
E Dadmin *next_admin(void);
E int add_sqladmin (char *name, char *passwd, int level, char *host, int lang);
E int del_sqladmin (char *name);
E void reset_sqladmin (void);

E char *MakePassword(char *plaintext);
E int ValidPassword(char *plaintext, char *checkvs);

E void ModuleDatabaseBackup(char *dbname);
E void ModuleRemoveBackups(char *dbname);

E FILE *html_open(char *filename);
E FILE *html_open_write(char *filename);
E void html_close(FILE * ptr);
E HTMLTag *createHTMLtag(const char *word, void (*func) (FILE *ptr));
E int addHTMLTag(HTMLTag * h);
E int destroyHTMLTag(HTMLTag * m);
E int delHTMLTag(HTMLTag * m);

/*************************************************************************/

E EvtHook *createEventHook(const char *name, int (*func) (int argc, char **argv));
E EvtHook *findEventHook(EvtHookHash * HookEvtTable[], const char *name);
E int addCoreEventHook(EvtHookHash * HookEvtTable[], EvtHook * evh);
E int moduleAddEventHook(EvtHook * evh);
E int moduleEventDelHook(const char *name);
E int delEventHook(EvtHookHash * HookEvtTable[], EvtHook * evh, char *mod_name);
E int destroyEventHook(EvtHook * evh);
E char *mod_current_evtbuffer;
E Message *first_message(void);
E Message *next_message(void);
E MessageHash *first_messagehash(void);
E MessageHash *next_messagehash(void);
E Command *first_command(void);
E Command *next_command(void);
E CommandHash *first_commandhash(void);
E CommandHash *next_commandhash(void);
E EvtMessage *first_EvtMessage(void);
E EvtMessage *next_EvtMessage(void);
E int moduleAddCallback(char *name,time_t when,int (*func)(int argc, char *argv[]),int argc, char **argv);
E void moduleDelCallback(char *name);
E char *moduleGetData(ModuleData **md, char *key);			/* Get the value for this key from this struct */
E int moduleAddData(ModuleData **md, char *key, char *value);		/* Set the value for this key for this struct */
E void moduleDelData(ModuleData **md, char *key);				/* Delete this key/value pair */
E void moduleDelAllData(ModuleData **md);					/* Delete all key/value pairs for this module for this struct */
E void moduleDelAllDataMod(Module *m);					/* remove all module data from all structs for this module */
E int moduleDataDebug(ModuleData **md);					/* Allow for debug output of a moduleData struct */
E boolean moduleMinVersion(int major,int minor,int patch,int build);	/* Checks if the current version of denora is before or after a given verison */
E EvtMessage *createEventHandler(char *name, int (*func) (char *source, int ac, char **av));
E EvtMessage *findEventHandler(EvtMessageHash * msgEvtTable[], const char *name);
E int addCoreEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm);
E int moduleAddEventHandler(EvtMessage * evm);
E int moduleEventDelHandler(char *name);
E int delEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm, char *mod_name);
E int destroyEventHandler(EvtMessage * evm);
E int addEventHandler(EvtMessageHash * msgEvtTable[], EvtMessage * evm);
/*************************************************************************/
/* Command Managment Functions */
E Command *createCommand(const char *name,int (*func)(User *u, int ac, char **av),int (*has_priv)(User *u),int help_all, int help_reg, int help_oper, int help_admin);
E int destroyCommand(Command *c);					/* destroy a command */
E int addCoreCommand(CommandHash *cmdTable[], Command *c);	/* Add a command to a command table */
E int moduleAddCommand(CommandHash *cmdTable[], Command *c, int pos);
E int addCommand(CommandHash *cmdTable[], Command *c,int pos);
E int delCommand(CommandHash *cmdTable[], Command *c,char *mod_name);		/* Del a command from a cmd table */
E int moduleDelCommand(CommandHash *cmdTable[],char *name);		/* Del a command from a cmd table */
E Command *findCommand(CommandHash *cmdTable[], const char *name);	/* Find a command */

/*************************************************************************/

/* Message Managment Functions */
E Message *createMessage(const char *name,int (*func)(char *source, int ac, char **av));
E Message *findMessage(MessageHash *msgTable[], const char *name);	/* Find a Message */
E int addMessage(MessageHash *msgTable[], Message *m, int pos);		/* Add a Message to a Message table */
E int addCoreMessage(MessageHash *msgTable[], Message *m);		/* Add a Message to a Message table */
E int moduleAddMessage(Message *m, int pos);
E int delMessage(MessageHash *msgTable[], Message *m, char *mod_name);		/* Del a Message from a msg table */
E int moduleDelMessage(char *name);
E int destroyMessage(Message *m);					/* destroy a Message*/

/*************************************************************************/

E void destroy_all(void);
E CronEvent *createCronEvent(const char *name, int (*func) (const char *name));
E CronEvent *findCronEvent(CronEventHash * hookEvtTable[], const char *name);
E int addCronEvent(CronEventHash * hookEvtTable[], CronEvent * evh);
E int delCronEvent(CronEventHash * msgEvtTable[], CronEvent * evm, char *mod_name);
E int displayCronFromHash(char *name);
E int displayCronEvent(CronEvent * evh);
E int moduleCronEventDel(const char *name);
E int moduleAddCronEvent(CronEvent * evh);
E Command *lookup_cmd(Command *list, char *name);
E void run_cmd(char *service, User *u, Command *list, char *name, char *str);
E void help_cmd(char *service, User *u, Command *list, char *name);
E void do_run_cmd(char *service, User * u, Command *c,const char *cmd, char *str);
E void do_help_cmd(char *service, User * u, Command *c,const char *cmd);
E void mod_help_cmd(char *service, User *u, CommandHash *cmdTable[],const char *cmd);
E void mod_run_cmd(char *service, User *u, CommandHash *cmdTable[],const char *cmd, char *str);
E void moduleAddMsgs(void);
E void moduleSetType(MODType type);
E void moduleInsertLanguage(int langNumber, int ac, char **av);
E void moduleNoticeLang(char *source, User *u, int number, ...);
E void moduleNoticeChanLang(char *source, ChannelStats *cs, int number, ...);
E void moduleDeleteLanguage(int langNumber);
E char *moduleGetLangString(User * u, int number);
E int moduleCopyCoreFile(char *name, char *output);
E int loadCoreModule(Module * m, User * u);
E Module *createCoreModule(char *filename);
E void modules_unload_all(void);
E char *ModuleGetErrStr(int status);
/*************************************************************************/
/* Module Managment Functions */
E Module *createModule(char *filename);	/* Create a new module, using the given name */
E int destroyModule(Module *m);		/* Delete the module */
E int addModule(Module *m);		/* Add a module to the module hash */
E int delModule(Module *m);		/* Remove a module from the module hash */
E Module *findModule(char *name);		/* Find a module */
E int loadModule(Module *m,User *u);	/* Load the given module into the program */
E int unloadModule(Module *m, User *u);	/* Unload the given module from the pro */
E int prepForUnload(Module *m);		/* Prepare the module for unload */
E void moduleAddVersion(const char *version);
E void moduleAddAuthor(const char *author);
E void modules_init(void);
E void modules_delayed_init(void);
E void moduleCallBackPrepForUnload(char *mod_name);
E void moduleCallBackDeleteEntry(ModuleCallBack * prev);
E char *moduleGetLastBuffer(void);
E void moduleSetHelpHelp(void (*func) (User * u));
E void moduleDisplayHelp(int service, User *u);
E int moduleCopyFile(char *name, char *output);
E int displayCommand(Command * c);
E int displayCommandFromHash(CommandHash * cmdTable[], char *name);
E int displayMessageFromHash(char *name);
E int displayMessage(Message * m);
E int destroyMessageHash(MessageHash * mh);
E int destroyCommandHash(CommandHash * ch);
E ModuleCallBack *moduleCallBackFindEntry(char *mod_name, boolean * found);
E int displayEventMessage(EvtMessage * evm);
E int displayEventHook(EvtHook * evh);
E int displayHookFromHash(char *name);
E int displayEvtMessageFromHash(char *name);
E EvtMessage *find_event(const char *name);
E EvtHook *find_eventhook(const char *name);
E int addEventHook(EvtHookHash * hookEvtTable[], EvtHook * evh);
E void moduleSetOperHelp(void (*func) (User * u));
E int moduleAddHelp(Command * c, int (*func) (User * u));
E int moduleAddRegHelp(Command * c, int (*func) (User * u));
E int moduleAddAdminHelp(Command * c, int (*func) (User * u));

E CommandHash *STATSERV[MAX_CMD_HASH];
E MessageHash *IRCD[MAX_CMD_HASH];
E EvtMessageHash *EVENT[MAX_CMD_HASH];
E EvtHookHash *EVENTHOOKS[MAX_CMD_HASH];
E CronEventHash *CRONEVENT[MAX_CMD_HASH];
E ModuleHash *MODULE_HASH[MAX_CMD_HASH];
E PrivMsgHash *PRIVMSGHANDLERS[MAX_CMD_HASH];
E HTMLHash *HTMLHANDLERS[MAX_CMD_HASH];

#ifdef _WIN32
E const char *deno_moderr(void);
#endif

E DENORA_INLINE int FileExists(char * fileName);
E DENORA_INLINE int FileRename(char * fileName, char *newfilename);
E DENORA_INLINE FILE *FileOpen(char *file, const char *mode);

E void handle_ircop(int ac, char **av);
E int UserHasMode(char *user, int m);
E void ModuleSetUserMode(int mode, int flag);
E void RemoveUserMode(User *user, char *mode);
E void SetUserMode(User *user, char *mode);
E int delUserMode(UserMode * m);
E int destroyUserMode(UserMode * m);
E int addUserModeCallBack(UserMode * m);
E UserMode *CreateUserModeCallBack(char *mode, int extra, void (*func) (int ac, char **av));
E UserMode *FindUserMode(char *name);
E void init_umodes(void);
E void ModuleUpdateSQLUserMode(void);

E ChanBanMode *FindChanBanMode(char *name);
E ChanBanMode *CreateChanBanMode(int mode, void (*addmask) (Channel *chan, char *mask), void (*delmask) (Channel *chan, char *mask));
E int addChanBanMode(ChanBanMode * m);
E int destroyChanBanMode(ChanBanMode * m);
E int delChanBanMode(ChanBanMode * m);

E list_t *Glinehead;
E list_t *Qlinehead;
E list_t *Zlinehead;

E SpamFilter *findSpamFilter(const char *regex);
E void fini_SpamFilter(void);
E void init_spamfilter(void);
E SpamFilter *new_SpamFilter(char *target, char *action,
                            char *setby, char *expires, char *setat,
                            char *duration, char *reason, char *regex);

E ChanMode *FindChanMode(char *name);
E ChanMode *CreateChanMode(int mode, void (*setvalue) (Channel *chan, char *value), char * (*getvalue) (Channel *chan));
E int addChanMode(ChanMode * m);
E int destroyChanMode(ChanMode * m);
E int delChanMode(ChanMode * m);
E void SetChanMode(Channel *c, char *mode);
E void RemoveChanMode(Channel *c, char *mode);
E void ModuleUpdateSQLChanMode(void);
E void ModuleSetChanMode(int mode, int flag);
E void ModuleSetChanUMode(int mode, char letter, int status_flag);
E void init_cmodes(void);
E void init_csmodes(void);
E int ChanHasMode(char *chan, int m);

#endif	/* EXTERN_H */
