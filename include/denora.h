/*************************************************************************/
/* (C) 2004-2006 Denora Team                                             */
/* Contact us at info@nomadirc.net                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/* $Id$                      */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/* Include all nesscary header files for this project                    */
/*************************************************************************/

#include "sysconf.h"             /* Include auto generated header which  */
                                 /* describes the users system this file */
                                 /* is created by running configure      */

#ifdef __sun                     /* If the system reports as Sun OS      */
#include "os/sun.h"              /* include hacks for the Sun OS         */
#endif                           /* end if                               */

#include <errno.h>		 /* Include system error numbers header  */
#include <stdio.h>               /* Include standard I/O header          */
#include <stddef.h>		 /* Include standard defines header      */
#include <stdarg.h>              /* Include standard arguements header   */

#ifdef HAVE_STDLIB_H		 /* If system has standard library       */
#include <stdlib.h>              /* Include standard library header      */
#endif                           /* end if                               */

#ifdef HAVE_STRING_H		 /* If system has string library         */
#include <string.h>              /* Include string library header        */
#endif                           /* end if                               */

#if HAVE_STRINGS_H		 /* If system has strings library        */
#include <strings.h>             /* Include strigns library header       */
#endif                           /* end if                               */

#include <time.h>		 /* Include time library header          */
#include <limits.h>		 /* Include limits header                */

#ifdef HAS_SYS_TYPES_H	     	 /* If system has sys/types header       */
#include <sys/types.h>           /* Include sys/types header             */
#endif				 /* end if                               */

#include <sys/stat.h>		 /* Include sys/stats header             */
#include <fcntl.h>               /* Include file control header          */
#include <ctype.h>               /* Include C data types header          */
#include <assert.h>              /* Include assert header                */

#ifdef HAVE_STDINT_H		 /* If system has standard integer       */
#include <stdint.h>              /* include standard integer header      */
#endif				 /* end if                               */

#if HAVE_SYS_SELECT_H		 /* If system has select header          */
#include <sys/select.h>          /* Include sys/select header            */
#endif				 /* end if                               */

#ifdef HAVE_SIGNAL_H             /* If system has signal header          */
#include <signal.h>              /* Include signal header                */
#endif				 /* end if                               */

#ifdef HAVE_POLL_H
#include <poll.h>
#endif				/* end if                               */

/*************************************************************************/
/* Include the header for modules as long as we can and its not win32    */
/*************************************************************************/

#if defined(USE_MODULES) && !defined(_WIN32)
#include <dlfcn.h>
#endif

/*************************************************************************/
/* Following includes build info for the given Operating System          */
/*************************************************************************/

#ifdef _WIN32
 #ifdef MSVS2005
  #include "os/win32vs2005.h"
 #else
  #if defined(__MINGW32__)
   #include "os/mingw.h"
  #else
   #include "os/win32.h"
  #endif
 #endif
#else
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__NetBSD__)
 #include "os/bsd.h"
#else
 #if defined(__CYGWIN__)
  #include "os/cygwin.h"
 #else
  #include "os/linux.h"
 #endif
#endif
#endif
#ifdef _AIX
#include "os/aix.h"
#endif

/*************************************************************************/
/* If system can use threading include headers for threading             */
/*************************************************************************/

#ifdef USE_THREADS
 #ifndef _WIN32                    /* Non Win32                          */
 #include <pthread.h>              /* Include pthread header             */
 #endif                            /* end if                             */
#include "threads.h"               /* our common threading header        */
#endif

/*************************************************************************/
/* MySQL headers that we want to include and defines that older versions */
/* do not have. Note that  MYSQL_HEADER_PREFIX should be trapped from    */
/* sysconf.h                                                             */
/*************************************************************************/

#ifdef USE_MYSQL
#ifdef MYSQL_HEADER_PREFIX
# include <mysql/mysql_version.h>
# include <mysql/mysql.h>
# include <mysql/errmsg.h>
# include <mysql/mysqld_error.h> 
#else
# include <mysql_version.h>
# include <mysql.h>
# include <errmsg.h>
# include <mysqld_error.h> 
#endif
#ifndef ER_USER_LIMIT_REACHED
#define ER_USER_LIMIT_REACHED 1226
#endif
#endif

/*************************************************************************/
/* Include the zlib header file if the system can has it                 */
/*************************************************************************/

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

/*************************************************************************/
/* Include the openssl header file if the system can has it              */
/*************************************************************************/

#ifdef HAVE_SSL
#include <ssl.h>
#endif

/*************************************************************************/
/* Include the crypt header file if the system can has it                */
/*************************************************************************/

#ifdef HAVE_CRYPT
#include <crypt.h>
#endif

/*************************************************************************/
/* Include the postgre header file if the system can has it              */
/*************************************************************************/

#ifdef USE_POSTGRE
#include <libpq-fe.h>
#endif

/*************************************************************************/
/* Include the Dmalloc header file if the system can has it, and user    */
/* had requested it, if not attempt to include the malloc header file    */
/*************************************************************************/

#ifdef HAVE_DMALLOC
#include <dmalloc.h>
#else
#ifdef HAVE_MALLOC_H
#ifndef __OpenBSD__
#include <malloc.h>
#endif
#endif
#endif

/*************************************************************************/
/* These are our headers that setup various things                       */
/*************************************************************************/

#include "defines.h"
#include "language.h"
#include "GeoIP.h"
#include "zip.h"
#include "events.h"
#include "sockets.h"
#include "version.h"
#include "ircd.h"
#include "modes.h"
#include "cron.h"
#include "list-array.h"

/*************************************************************************/
/* Remove standard C functions and replace with our own functions        */
/*************************************************************************/

#undef tolower
#undef toupper
#define tolower tolower_
#define toupper toupper_

/*************************************************************************/
/* Macro functions that help us with a few items                         */
/*************************************************************************/

/* Length of an array: */
#define lenof(a)	(sizeof(a) / sizeof(*(a)))

/* is a digit ie, 0, to 9 */
#define IsDigit(c) (char_atribs[(unsigned char)(c)]&DIGIT)

/* check of the pointer is NULL or 0 lenght */
#define BadPtr(x) (!(x) || (*(x) == '\0'))

/* check if the character is NULL, 0 lenght or begins with % */
#define BadChar(x) (!(x) || (*(x) == '%') || (*(x) == '\0'))

/* make sure that the channel name begins with # and isn't null */
#define BadChanName(x) (!(x) || (*(x) != '#') || (*(x) == '\0'))

#define SWAP_CHAR( x, y ) {char c; c = x; x = y; y = c;}

#define DenoraFree(x) if (!BadPtr(x)) free(x)

/*************************************************************************/
/* Setup how we define our macro fro externing data                      */
/*************************************************************************/

#ifndef _WIN32
 #if defined(__CYGWIN__)
   #define E __declspec(dllexport)
 #else
  #define E extern
 #endif
#else
 #ifndef MODULE_COMPILE
  #define E extern __declspec(dllexport)
 #else
  #define E extern __declspec(dllimport)
 #endif
#endif

/*************************************************************************/
/* Setup a true/false boolean                                            */
/* Windows defines a boolean type as an unsigned char. It does however   */
/* need true/false. -- codemastr                                         */
/*************************************************************************/

#ifndef _WIN32
typedef enum { false, true } boolean;
#else
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif /* _WIN32 */

/*************************************************************************/
/* Some enum stuff                                                       */
/*************************************************************************/

typedef enum { 
   CORE,        	    /* Core Module                               */
   PROTOCOL,    	    /* IRCD Protocol Module                      */
   THIRD,                   /* Third Party Module                        */
   SUPPORTED,               /* Supported (included module)               */
   QATESTED                 /* QA Team has checked and cleared it        */
} MODType;

typedef enum { 
	MOD_OP_LOAD, 
	MOD_OP_UNLOAD 
} ModuleOperation;

/*************************************************************************/
/* Setup the structs                                                     */
/*************************************************************************/

typedef struct server_ Server;			    /* Server struct     */
typedef struct user_ User;			    /* User struct       */
typedef struct channel_ Channel;		    /* Channel struct    */
typedef struct ModuleData_ ModuleData;              /* ModuleData struct */
typedef struct metadata_ MetaData;                  /* MetaData struct   */
typedef struct tld_ TLD;			    /* TLD Struct        */
typedef struct ircdvars_ IRCDVar;		    /* IRCD Struct       */
typedef struct statvars_ STATVar;		    /* Stats Struct      */
typedef struct ircdcapab_ IRCDCAPAB;                /* IRCD Capab Struct */
typedef struct denoravars_ DenoraVar;		    /* Denora Struct     */
typedef struct uid_ Uid;			    /* UID Struct        */
typedef struct serverstats_ ServStats;
typedef struct ctcpverstats_ CTCPVerStats;
typedef struct exclude_ Exclude;
typedef struct chanstats_ ChannelStats;
typedef struct statschan_ StatsChannel;
typedef struct db_file_ DenoraDBFile;
typedef struct serverbans_ ServerBans;
typedef struct capabinfo_ CapabInfo;
typedef struct dadmin_ Dadmin;
typedef struct Conf_Modules Conf_Modules;
typedef struct TopConf tConf;
typedef struct SubConf sConf;
typedef struct ConfVar cVar;
typedef struct queueentry_ QueueEntry;
typedef struct PrivMsgHandler_ PrivMsg;
typedef struct PrivMsgHash_ PrivMsgHash;
typedef struct htmltag_ HTMLTag;
typedef struct htmlhash_ HTMLHash;
typedef struct ModuleQueue_ ModuleQueue;
typedef struct ModuleLang_ ModuleLang;
typedef struct Module_ Module;
typedef struct ModuleHash_ ModuleHash;
typedef struct ModuleCallBack_ ModuleCallBack;
typedef struct EvtMessage_ EvtMessage;
typedef struct EvtMessageHash_ EvtMessageHash;
typedef struct EvtHook_ EvtHook;
typedef struct EvtHookHash_ EvtHookHash;
typedef struct CronEvent_ CronEvent;
typedef struct CronEventHash_ CronEventHash;
typedef struct Command_ Command;
typedef struct CommandHash_ CommandHash;
typedef struct Message_ Message;
typedef struct MessageHash_ MessageHash;
typedef struct usermode_ UserMode;
typedef struct usermodehash_ UserModeHash;
typedef struct chanbanmode_ ChanBanMode;
typedef struct chanbanmodehash_ ChanBanModeHash;
typedef struct chanmode_ ChanMode;
typedef struct chanmodehash_ ChanModeHash;
typedef struct spamfilter_ SpamFilter;

typedef struct gline_ Gline;
typedef struct qline_ Qline;
typedef struct zline_ Zline;


/*************************************************************************/
/* Define out the hash tables                                            */
/*************************************************************************/

#define STATSERV OS_cmdTable
#define IRCD IRCD_cmdTable
#define MODULE_HASH Module_table
#define EVENT EVENT_cmdTable
#define EVENTHOOKS HOOK_cmdTable
#define CRONEVENT CRONHOOK_cmdTable
#define PRIVMSGHANDLERS PRIVMSGHANDLERS_cmdTable
#define XMLRPCCMD XMLRPCCMD_cmdTable
#define USERMODEHANDLERS usermode_table

/*************************************************************************/
/* Command Struct                                                        */
/*************************************************************************/

struct Command_ {
    char *name;
    int (*routine)(User *u, int ac, char **av);
    int (*has_priv)(User *u);	/* Returns 1 if user may use command, else 0 */

    int helpmsg_all;	/* Displayed to all users; -1 = no message */
    int helpmsg_reg;	/* Displayed to regular users only */
    int helpmsg_admin;	/* Displayed to Stats admins only */
	int helpmsg_oper;

    char *help_param1;
    char *help_param2;
    char *help_param3;
    char *help_param4;

    /* Module related stuff */
    int core;           /* Can this command be deleted? */
    char *mod_name;	/* Name of the module who owns us, NULL for core's  */
    char *service;	/* Service we provide this command for */
    int (*all_help)(User *u);
    int (*regular_help)(User *u);
    int (*admin_help)(User *u);

    Command *next;	/* Next command responsible for the same command */
};

/*************************************************************************/
/* Command Hash                                                          */
/*************************************************************************/

struct CommandHash_ {
        char *name;	/* Name of the command */
        Command *c;	/* Actual command */
        CommandHash *next; /* Next command */
};

/*************************************************************************/
/* SpamFilter                                                            */
/*************************************************************************/

struct spamfilter_ {
		char *regex;
		char *target;
		char *action;
		char *setby;
		char *expires;
		int32 setat;
		int32 duration;
		char *reason;
};

/*************************************************************************/
/* Gline                                                                 */
/*************************************************************************/

struct gline_ {
		char *user;
		char *host;
		char *setby;
		int32 expires;
		int32 setat;
		char *reason;
};

/*************************************************************************/
/* Qline                                                                 */
/*************************************************************************/

struct qline_ {
		char *user;
		char *host;
		char *setby;
		int32 expires;
		int32 setat;
		char *reason;
};

/*************************************************************************/
/* Zline                                                                 */
/*************************************************************************/

struct zline_ {
		char *user;
		char *host;
		char *setby;
		int32 expires;
		int32 setat;
		char *reason;
};

/*************************************************************************/
/* Message Struct                                                        */
/*************************************************************************/

struct Message_ {
    char *name;
    int (*func)(char *source, int ac, char **av);
    int core;
    char *mod_name;
    Message *next;
};

/*************************************************************************/
/* Message Hash                                                          */
/*************************************************************************/

struct MessageHash_ {
        char *name;
        Message *m;
        MessageHash *next;
};

/*************************************************************************/
/* Event Message Struct                                                  */
/*************************************************************************/

struct EvtMessage_ {
    char *name;
    int (*func)(char *source, int ac, char **av);
    int core;
    char *mod_name;
    EvtMessage *next;
};

/*************************************************************************/
/* Event Message Hash                                                    */
/*************************************************************************/

struct EvtMessageHash_ {
        char *name;
        EvtMessage *evm;
        EvtMessageHash *next;
};

/*************************************************************************/
/* Event Hook Struct                                                     */
/*************************************************************************/

struct EvtHook_ {
    int (*func)(int argc, char **argv);
    int core;
	char *name;
    char *mod_name;
    EvtHook *next;
};

/*************************************************************************/
/* Event Hook Hash                                                       */
/*************************************************************************/

struct EvtHookHash_ {
        char *name;
        EvtHook *evh;
        EvtHookHash *next;
};

/*************************************************************************/
/* Cron Event                                                            */
/*************************************************************************/

struct CronEvent_ {
    int (*func)(const char *name);
	char *name;
    int core;
    char *mod_name;
    CronEvent *next;
};

/*************************************************************************/
/* Cron Event Hash                                                       */
/*************************************************************************/

struct CronEventHash_ {
        char *name;
        CronEvent *evh;
        CronEventHash *next;
};

/*************************************************************************/
/* Module Call back stuff                                                */
/*************************************************************************/

struct ModuleCallBack_ {
	char *name;
	char *owner_name;
	time_t when;
	int (*func)(int argc, char *argv[]);
	int argc;
	char **argv;
	ModuleCallBack *next;
};

/*************************************************************************/
/* Module Language                                                       */
/*************************************************************************/

struct ModuleLang_ {
    int argc;
    char **argv;
};

/*************************************************************************/
/* Module Queue - shows how to load/unload modules                       */
/*************************************************************************/

struct ModuleQueue_ {
	Module *m;
	ModuleOperation op;
	User *u;
	
	ModuleQueue *next;
};

/*************************************************************************/
/* Module Struct - contains module data                                  */
/*************************************************************************/

struct Module_ {
	char *name;
	char *filename;
	void *handle;
	time_t time;
	char *version;
	char *author;
	MODType type;
	void (*operHelp)(User *u); /* 5 */

	MessageHash *msgList[MAX_CMD_HASH];
	ModuleLang lang[NUM_LANGS];
};

/*************************************************************************/
/* Module Hash Struct - contains list of all modules                     */
/*************************************************************************/

struct ModuleHash_ {
        char *name;
        Module *m;
        ModuleHash *next;
};

/*************************************************************************/
/* Server Bans (not currently used)                                      */
/*************************************************************************/

struct serverbans_ {
  ServerBans *next, *prev;
  char *type;
  char *user;
  char *host;
  char *setby;
  char *setat;
  char *expires;
  char *reason;
};

/*************************************************************************/
/* UID struct - needed for P10/TS6 ircds to track our internal UIDs      */
/*************************************************************************/

struct uid_ {
    Uid *next, *prev;
    char nick[NICKMAX];
    char *uid;
};

/*************************************************************************/
/* Database struct                                                       */
/*************************************************************************/

struct db_file_ {
    FILE *fptr;              /* Pointer to the opened file               */
    int db_version;          /* The db version of the datafiles          */
    int core_db_version;     /* The current db version of this source    */
    char service[256];       /* StatServ/etc.                            */
    char filename[BUFSIZE];  /* Filename of the database                 */
    char temp_name[BUFSIZE]; /* Temp filename of the database            */
};

/*************************************************************************/
/* Denora operation struct                                               */
/*************************************************************************/

struct denoravars_ {
	const char *dir;
	const char *logname;
	const char *config;
	const char *version;
	const char *versiondotted;
	const char *flags_os;
	const char *flag_mods;
	const char *flag_sql;
	char *version_protocol;
	const char *date;
	const char *time;
	const char *build;
	char *qmsg;
	time_t start_time;
	int do_sql;
	int sqldebug;
	char *uplink;
	uint32 capab;
	int quitting;
	int delayed_quit;
	int do_html;
	int socketdebug;
	int debug;
	int nofork;
	int save_data;
	int protocoldebug;
};

/*************************************************************************/
/* Statiscal Data Numbers Struct                                         */
/*************************************************************************/

struct statvars_ {
	uint32 users;
	uint32 opers;
	uint32 chans;
	uint32 servers;
	uint32 away;

	uint32 users_max;
	time_t users_max_time;
	uint32 opers_max;
	time_t opers_max_time;
	uint32 chans_max;
	time_t chans_max_time;
	uint32 servers_max;
	time_t servers_max_time;

	uint32 daily_users;
	time_t daily_users_time;
	uint32 daily_opers;
	time_t daily_opers_time;
	uint32 daily_servers;
	time_t daily_servers_time;
	uint32 daily_chans;
	time_t daily_chans_time;

	uint32 totalusersever;

};

/*************************************************************************/
/* IRCD Protocol struct                                                  */
/*************************************************************************/

struct ircdvars_ {
	const char *name;				/* Name of the IRCD        			 */
	const char *statservmode;		/* Mode used by StatServ   			 */
	int vhost;		        /* IRCD supports vhost		         */
	int sgline;		        /* IRCD supports SGline		         */
	int sgline_table;       /* Whether we need the sgline table  */
	int sqline;				/* IRCD supports SQline		         */
	int sqline_table;       /* Whether we need the sqline table  */
	int szline;				/* IRCD supports SZline		         */
	int except;		        /* IRCD supports exception +e		 */
	int vident;		        /* IRCD supports vidents		     */
	int nickip;		        /* IRCD sends IP on NICK		     */
	int nickvhost;		        /* Users vhost sent during NICK cmd  */
	int fmode;		        /* IRCD supprts channel flood mode this is normally +f */
	int jmode;			/* IRCD supprts a secondary flood mode protection, this is normally +j */
	int Lmode;			/* IRCD supports linked channels 	 */
	uint32 chan_fmode;	        /* IRCD_CMODE_* for primary flood  	 */
	uint32 chan_jmode;	        /* IRCD_CMODE_* for secondary flood  */
	uint32 chan_lmode;	        /* IRCD_CMODE_* for linked channels	 */
	char *chanmodes;	        /* If the ircd sends CHANMODE in CAPAB this is where we store it */
	int token;		        /* Does Denora support the tokens for the ircd */
	int tokencaseless;	        /* IRCD TOKENs are not case senstive 
                                         * UnrealIRCD is not case senstive, 
                                     	 *  most IRCD are case senstive 
					 */
	int sjb64;		        /* SJOIN time stamps are encoded in base 64 formatting */
    int invitemode;		        /* IRCD supports Invite exception +I */
   	int sjoinbanchar;			/* SJOIN bans use this char          */
   	int sjoinexchar;			/* SJOIN expection use this char     */
   	int sjoinivchar;			/* SJOIN Invite use this char        */
	uint32 vhostmode;	        /* IRCD_UMODE_* for Vhost            */
	int owner;
	int protect;
	int halfop;
	char *usermodes;
	char *cmodes;
   	int floodchar;
   	int floodchar_alternative;
	int vhostchar;
	int vhostchar2;
	int chanforward;
	int p10;
	int ts6;
	int numerics;			/* IRCD uses Numerics to respond to some messages */
	int gagged;			/* channel mode where user can not speak but is not banned */
	int spamfilter;
	int ban_char;
	int except_char;
	int invite_char;
	int zip;
	int ssl;
	int uline;
	char *nickchars;
	int has_svid;
	int hideoper;
	int extrawarning;
	int syncstate;
};

/*************************************************************************/
/* IRCD Capabilities struct                                              */
/*************************************************************************/

struct ircdcapab_ {
  uint32 noquit;
  uint32 tsmode;
  uint32 unconnect;  
  uint32 nickip;
  uint32 nsjoin;
  uint32 zip;
  uint32 burst;
  uint32 ts5;
  uint32 ts3;
  uint32 dkey;
  uint32 pt4;
  uint32 scs;
  uint32 qs;
  uint32 uid;
  uint32 knock;
  uint32 client;
  uint32 ipv6;
  uint32 ssj5;
  uint32 sn2;
  uint32 token;
  uint32 vhost;
  uint32 ssj3;
  uint32 nick2;
  uint32 umode2;
  uint32 vl;
  uint32 tlkext;
  uint32 dodkey;
  uint32 dozip;
  uint32 chanmodes;
  uint32 sjb64;
  uint32 nickchars;
};

/**************************************************************************/
/* ModuleData strucs used to allow modules to add / delete module         */
/* Data from existing structs                                             */
/**************************************************************************/

struct ModuleData_ {
	char *moduleName;		/* Which module we belong to  */
	char *key;			/* The key                    */
	char *value;			/* The Value                  */
	ModuleData *next;		/* The next ModuleData record */
};

/**************************************************************************/
/* MetaData strucs used to allow core and modules to add / delete / share */
/* Data independent of a struct                                           */
/**************************************************************************/

struct metadata_ {
	int flags;			/* Private or Public          */
	char *key;			/* The key                    */
	char *value;			/* The Value                  */
	MetaData *next;                  /* The next MetaData record   */
};
 
/**************************************************************************/
/* Server Struct - this is not the stats this is destoried on shutdown    */
/**************************************************************************/

struct server_ {
    Server *next, *prev;

    char *name;				/* Server name 			          */
    uint16 hops;		        /* Hops between services and server 	  */
    char *desc;			        /* Server description 			  */
    uint16 flags;			/* Some info flags			  */
    char *suid;				/* Server Universal ID		     	  */
    int sync;	        		/* Whether is synced or not	          */
    char *version;			/* Version 				  */
    uint32 uptime;			/* Uptime				  */
    ServStats *ss;			/* Server Stats Struct                    */
    Server *uplink;			/* Uplink Server Struct  		  */
    time_t lag_time;            
    uint32 ping;                        /* ping time                              */
    uint32 lastping;                    /* Last time pinged                       */
    uint32 sqlid;			/* server sql id                          */
    Server *links;		        /* Linked list head for linked servers 	  */
    int uline;                          /* Server is ulined                       */
    ModuleData *moduleData;		/* Module Data                            */
    char **slinks;                      /* Array of Server Links                  */
	int slinks_count;
	char *motd;
};

/**************************************************************************/
/* Capab Info - used so we can check stuff quicker with less lines        */
/**************************************************************************/

struct capabinfo_ {
	const char *token;
	uint32 flag;
};

/**************************************************************************/
/* Server Statisical Struct                                               */
/**************************************************************************/

struct serverstats_ {
	ServStats *next, *prev;

    	char name[NICKMAX];		/* Server name 				*/
    	uint32 uptime;			/* Uptime				*/
	uint32 currentusers; 	        /* Curretn Users 			*/
	uint32 maxusers;		/* Max Users				*/
	uint32 opers;			/* Current Opers			*/
	uint32 maxopers;		/* Max Opers				*/
	time_t maxopertime;		/* Time of Max Opers		        */
	time_t maxusertime;		/* Time of Max Users			*/
	uint32 highestping;             /* Largest Ping 			*/
	time_t maxpingtime;		/* Largest ping time			*/
	uint32 totalusersever;          /* Total users ever			*/
	uint32 splits;			/* Total number of server splits	*/
	uint32 ircopskills;		
	uint32 serverkills;
	uint32 split_stats;
	time_t lastseen;
	ModuleData *moduleData;
};

/**************************************************************************/
/* Top Level Domain Statisical Struct                                     */
/**************************************************************************/

struct tld_ {
    TLD *next, *prev;

    char *country;
    char countrycode[NICKMAX];
    uint32 count;
    uint32 overall;
    ModuleData *moduleData;
};

/**************************************************************************/
/* Temp Struct used for sorting                                           */
/**************************************************************************/

typedef struct {
   char name[BUFSIZE];
   int count;
} temp_data;

/**************************************************************************/
/* CTCP Statisical Struct                                                 */
/**************************************************************************/

struct ctcpverstats_ {
    CTCPVerStats *next, *prev;
    CTCPVerStats *nextsort, *prevsort;

    char *version;
    uint32 count;
    uint32 overall;
    ModuleData *moduleData;
};

/*************************************************************************/

struct exclude_ {
    Exclude *next, *prev;

    char *name;
    uint32 flag;
};

/**************************************************************************/
/* User Struct                                                            */
/**************************************************************************/

struct user_ {
    User *next, *prev;
    char nick[NICKMAX];
    char *username;             /* User's real ident		*/
    char *host;                 /* User's real hostname 	*/
    char *vhost;        	/* User's virtual hostname      */
    char *vident;       	/* User's virtual ident 	*/
    char *realname;		/* Realname 			*/
    Server *server;	        /* Server struct user is on     */
    char *ip;			/* User's resolved IP           */
    time_t timestamp;	        /* Timestamp of the nick 	*/
    time_t my_signon;	        /* When did _we_ see the user?  */
    uint32 svid;		/* Services ID 			*/
    uint32 mode;		/* See below 			*/
    char *uid;			/* Univeral ID			*/
    uint16 language;
    uint16 admin;
    char *ctcp;
    int sqlid;			/* mysql id number */
	char *sqlnick;
	char *swhois;
	int hopcount;

	int isaway;
	char *awaymsg;

    int cstats;          /* used for channelstats - dont modify it */
    char *sgroup;        /* contains the escaped name of the stats-group */

    ModuleData *moduleData;		/* defined for it, it should allow the module Add/Get */
    char *country_code;
    char *country_name;

    int isservice;
    
    struct u_chanlist {
        struct u_chanlist *next, *prev;
        Channel *chan;
        int16 status;    /* Associated flags; see CSTATUS_* below. */
    } *chans;            /* Channels user has joined */

    struct u_modes {
        struct u_modes *next, *prev;
        char *mode;
    } *modes;
};

/*************************************************************************/

struct usermode_ {
	UserMode *next, *prev;
    char *mode;
	int extra;
    void (*set) (int ac, char **av);
};

struct usermodehash_ {
	UserModeHash *next, *prev;
    char *mode;
	UserMode *m;
};

/*************************************************************************/

struct chanmode_ {
	ChanMode *next, *prev;
    char *mode;
    void (*setvalue) (Channel *chan, char *value);
	char * (*getvalue) 		(Channel *chan);
};

struct chanmodehash_ {
	ChanModeHash *next, *prev;
    char *mode;
	ChanMode *cm;
};

/*************************************************************************/

struct chanbanmode_ {
    ChanBanMode *next, *prev;
    char *mode;
	void (*addmask) (Channel *chan, char *mask);
	void (*delmask) (Channel *chan, char *mask);
};

struct chanbanmodehash_ {
    ChanBanModeHash *next, *prev;
    char *mode;
    ChanBanMode *cbm;
};

/*************************************************************************/

struct chanstats_ {
    ChannelStats *next, *prev;
    char *name;
    uint32 flags;
	uint32 timeadded;
};

/*************************************************************************/

struct statschan_ {
    StatsChannel *next, *prev;
    StatsChannel *nextsort, *prevsort;
    char name[CHANMAX];
    uint32 kickcount;
    time_t kickcounttime;
    uint32 joincounter;
    time_t joincounttime;
    uint32 topic_count;
    time_t topiccounttime;
    uint16 usercount;
    uint16 maxusercount;
    time_t maxusertime;
    int in_use;
    uint32 partcount;
    time_t partcounttime;
    uint32 modecount;
    time_t modecounttime;
    ModuleData *moduleData;
};

/*************************************************************************/

struct channel_ {
    Channel *next, *prev;
    char name[CHANMAX];
    time_t creation_time;		/* When channel was created */
    char *topic;
    char topic_setter[NICKMAX];		/* Who set the topic */
    time_t topic_time;			/* When topic was set */
    uint32 limit;			/* 0 if none */
    char *key;				/* NULL if none */
    char *redirect;			/* +L; NULL if none */
    char *flood;			/* +f; NULL if none */
    char *flood_alt;
    int32 bancount, bansize;
    char **bans;
    int32 exceptcount, exceptsize;
    char **excepts;
    int32 invitecount, invitesize;
    char **invite;
    int32 quietcount, quietsize;
    char **quiet;
    char *sqlchan;
    struct c_userlist {
		struct c_userlist *next, *prev;
		User *user;
    } *users;
    StatsChannel *stats;
    int cstats;          /* used for channelstats - dont modify it */
    int sqlid;
    int statservon;

    time_t server_modetime;		/* Time of last server MODE */
    int16 server_modecount;		/* Number of server MODEs this second */
    ModuleData *moduleData;

    struct c_modes {
        struct c_modes *next, *prev;
        char *mode;
    } *modes;
};

/* Configuration structures */

struct dadmin_
{
	Dadmin *prev, *next;

    char *hosts[MAXHOSTS+1];
    char *passwd;
    char *name;
    uint16 language;
    int   legal;
	int   configfile;    /* Admin was loaded by the config file  */
};

struct Conf_Modules
{
    char *autoload[128];
    char *delayed[128];
};

/**************************************************************************/
/* Privmsg Handlers                                                       */
/**************************************************************************/

struct PrivMsgHandler_ {
	char *service;
	void (*handler) (User * u, char *buf);
};

struct PrivMsgHash_ {
	char *service;
	PrivMsg *p;
	PrivMsgHash *next;
};

/**************************************************************************/
/* HTML Tag struct                                                        */
/**************************************************************************/

struct htmltag_ {
	char *tag;
	void (*handler) (FILE *ptr);
};

struct htmlhash_ {
	char *tag;
	HTMLTag *h;
	HTMLHash *next;
};

/*************************************************************************/
/**
 * IRCD Protocol module support struct.
 * protocol modules register the command they want touse for function X with our set
 * functions, we then call the correct function for the anope_ commands.
 **/
typedef struct ircd_proto_ {
	void (*ircd_set_mod_current_buffer)(int ac, char **av);
    void (*ircd_cmd_nick)(char *nick, char *name, const char *modes);
	void (*ircd_cmd_bot_nick) (char *nick, char *user, char *host, char *real, char *modes);
    void (*ircd_cmd_mode)(char *source, char *dest, char *buf);
    void (*ircd_cmd_notice)(char *source, char *dest, char *buf);
    void (*ircd_cmd_privmsg)(char *source, char *dest, char *buf);
    void (*ircd_cmd_serv_notice)(char *source, char *dest, char *msg);
    void (*ircd_cmd_serv_privmsg)(char *source, char *dest, char *msg);
    void (*ircd_cmd_quit)(char *source, char *buf);
    void (*ircd_cmd_pong)(char *servname, char *who);
    void (*ircd_cmd_join)(char *user, char *channel, time_t chantime);
    void (*ircd_cmd_part)(char *nick, char *chan, char *buf);
    void (*ircd_cmd_global)(char *source, char *buf);
    void (*ircd_cmd_squit)(char *servname, char *message);
    void (*ircd_cmd_connect)(void);
    void (*ircd_cmd_ctcp)(char *source, char *dest, char *buf);
    void (*ircd_cmd_eob)(void);
    void (*ircd_cmd_version)(char *server);
    void (*ircd_cmd_stats)(char *sender, const char *letter, char *server);
    void (*ircd_cmd_motd)(char *sender, char *server);
    void (*ircd_cmd_ping)(char *server);
} IRCDProto;

typedef struct ircd_modes_ {
        int user_oper;
		uint32 ircd_var;
} IRCDModes;

/**************************************************************************/
/* Base64 struct data                                                     */
/**************************************************************************/

struct buffer_st {
  char *data;
  int length;
  char *ptr;
  int offset;
};

/**************************************************************************/
/* First In / First Out Queue                                             */
/**************************************************************************/

struct queueentry_ {
   char *msg;
   QueueEntry *link;
};

/**************************************************************************/
/* Config File Related Stuff                                              */
/**************************************************************************/

/* top level conf options */
struct TopConf
{
    const char *tok;        		   /* our token string             */
    unsigned int flag;      	           /* our token flag               */
    unsigned int nest;                     /* tokens we allow to nest here */
    sConf *subtok;          	           /* sub-tokens allowed in here   */
    int (*func) (cVar * vars[], int lnum); /* function to call to add this */
};

/* sub-token options */
struct SubConf
{
    const char *tok;        /* our token string             */
    unsigned long flag;     /* our token flag               */
    unsigned int var;       /* our variable type            */
};

struct ConfVar
{
    sConf   *type;
    char    *value;
    int      loaded;        /* 1 - identified.  
                             * 2 - variable loaded
                             * 3 - delimited cleared */
};

/*************************************************************************/
/* Flat Config Details                                                   */
/*************************************************************************/

#define MAXPARAMS	8

/* Configuration directives */

typedef struct {
    const char *name;
    struct {
        int type;               /* PARAM_* below */
        int flags;              /* Same */
        void *ptr;              /* Pointer to where to store the value */
    } params[MAXPARAMS];
} Directive;

#define PARAM_NONE	0
#define PARAM_INT	1
#define PARAM_POSINT	2       /* Positive integer only */
#define PARAM_PORT	3       /* 1..65535 only */
#define PARAM_STRING	4
#define PARAM_TIME	5
#define PARAM_STRING_ARRAY 6    /* Array of string */
#define PARAM_SET	-1      /* Not a real parameter; just set the
                                 *    given integer variable to 1 */
/* Flags: */
#define PARAM_OPTIONAL	0x01
#define PARAM_FULLONLY	0x02    /* Directive only allowed if !STREAMLINED */
#define PARAM_RELOAD    0x04    /* Directive is reloadable */

/**************************************************************************/
/* These include need to be down here due to the fact that they use       */
/* structs that are created above                                         */
/**************************************************************************/

#include "list.h"
#include "modules.h"
#include "xmlrpc.h"
#include "adns.h"
#include "extern.h"

/*************************************************************************/

