/*************************************************************************/
/* (c) 2004-2012 Denora Team                                             */
/* Contact us at info@denorastats.org                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/*                      */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/* Define our name, so modules can build a check for if denora           */
/*************************************************************************/

#define DENORA "denora"

/*************************************************************************/
/* Name of configuration file (in Stats directory)                       */
/*************************************************************************/

#define STATS_CONF      "denora.conf"

/*************************************************************************/
/* Name of log file (in Stats directory)                                 */
/*************************************************************************/

#define LOG_FILENAME    "denora.log"

/*************************************************************************/
/* Maximum amount of data from/to the network to buffer (bytes).         */
/*************************************************************************/

#define NET_BUFSIZE     65536

/*************************************************************************/
/* Size of input buffer (note: this is different from BUFSIZ)            */
/* This must be big enough to hold at least one full IRC message, or     */
/* messy things will happen.                                             */
/*************************************************************************/

#define BUFSIZE         1024

/*************************************************************************/
/* Maximum length of a channel name, including the trailing null.  Any   */
/* channels with a length longer than (CHANMAX-1) including the leading  */
/* # will not be usable.                                                 */
/*************************************************************************/

#define CHANMAX         64

/*************************************************************************/
/* Maximum length of the server names                                    */
/*************************************************************************/

#define SERVERMAX       128

/*************************************************************************/
/* Maximum length of a nickname, including the trailing null.  This MUST */
/* be at least one greater than the maximum allowable nickname length on */
/* your network, or people will run into problems using Stats!  The      */
/* default (32) works with all servers we know of.                       */
/*************************************************************************/

#define NICKMAX         32

/*************************************************************************/
/* Maximum length of a username                                          */
/*************************************************************************/

#define USERMAX         10

/*************************************************************************/
/* Maximum length of a domain                                            */
/*************************************************************************/

#define HOSTMAX         64


#define MAXHOSTS 32


#ifndef MAX_CMD_HASH
#define MAX_CMD_HASH 1024
#endif

#ifndef LINE_MAX
#define LINE_MAX 256
#endif

/* Some Linux boxes (or maybe glibc includes) require this for the
 * prototype of strsignal(). */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MAX_SQL_BUF   4096

#define SQL_WARNING 2
#define SQL_ERROR   4

#ifndef NAME_MAX
# define NAME_MAX 255
#endif

#ifndef BUFSIZ
# define BUFSIZ 256
#else
# if BUFSIZ < 256
#  define BUFSIZ 256
# endif
#endif

#define MAX_VALUES 128          /* maximum values per block */

#ifdef NEXT
#define VOIDSIG int     /* whether signal() returns int of void */
#else
#define VOIDSIG void    /* whether signal() returns int of void */
#endif

#ifdef  BSD_RELIABLE_SIGNALS
#define  HAVE_RELIABLE_SIGNALS
#endif
#ifdef  SYSV_UNRELIABLE_SIGNALS
#undef   HAVE_RELIABLE_SIGNALS
#endif
#ifdef  POSIX_SIGNALS
#define  HAVE_RELIABLE_SIGNALS
#endif

#ifdef __STRICT_ANSI__
#undef POSIX_SIGNALS
#endif

#define SERVER_ISME  1
#define SERVER_JUPED 2

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!
 */
#define LANG_EN_US             0       /* United States English */
#define LANG_ES                1       /* Spanish */
#define LANG_DE                2       /* German */
#define LANG_FR                3       /* French */
#define LANG_NL                4       /* Dutch */
#define LANG_IT                5       /* Italian */
#define LANG_RU                6       /* Russian */
#define LANG_PT_BR             7       /* Portuguese */

#define NUM_LANGS              8        /* Number of languages */
#define USED_LANGS             8        /* Number of languages provided */

#define DEF_LANGUAGE    LANG_EN_US

#define CAPAB_NOQUIT    0x00000001
#define CAPAB_TSMODE    0x00000002
#define CAPAB_UNCONNECT 0x00000004
#define CAPAB_NICKIP    0x00000008
#define CAPAB_NSJOIN    0x00000010
#define CAPAB_ZIP       0x00000020
#define CAPAB_BURST     0x00000040
#define CAPAB_TS3       0x00000080
#define CAPAB_TS5       0x00000100
#define CAPAB_DKEY      0x00000200
#define CAPAB_DOZIP     0x00000400
#define CAPAB_DODKEY    0x00000800
#define CAPAB_QS        0x00001000
#define CAPAB_SCS       0x00002000
#define CAPAB_PT4       0x00004000
#define CAPAB_UID       0x00008000
#define CAPAB_KNOCK     0x00010000
#define CAPAB_CLIENT    0x00020000
#define CAPAB_IPV6      0x00040000
#define CAPAB_SSJ5      0x00080000
#define CAPAB_SN2       0x00100000
#define CAPAB_VHOST     0x00200000
#define CAPAB_TOKEN     0x00400000
#define CAPAB_SSJ3      0x00800000
#define CAPAB_NICK2     0x01000000
#define CAPAB_UMODE2    0x02000000
#define CAPAB_VL        0x04000000
#define CAPAB_TLKEXT    0x08000000
#define CAPAB_CHANMODE  0x10000000
#define CAPAB_SJB64     0x20000000
#define CAPAB_NICKCHARS 0x40000000

#define LOG_DEBUG  		1
#define LOG_NONEXISTANT 2
#define LOG_ERROR       3
#define LOG_EXTRADEBUG  4
#define LOG_NORMAL		5
#define LOG_SQLDEBUG	6
#define LOG_DEBUGSOCK	7
#define LOG_PROTOCOL    8
#define LOG_ADNS    	9


#define EXCLUDE_USER	 0x00000001
#define EXCLUDE_SERVER   0x00000002
#define EXCLUDE_SERVUSER 0x00000004

#define LUSERS_USERS 1
#define LUSERS_USERSINV 2
#define LUSERS_OPERS 3
#define LUSERS_SERV 4
#define LUSERS_CHAN 5
#define LUSERS_USERSGLOB 6
#define LUSERS_USERSMAX 7

#define CS_FANTASY        0x00000001
#define CS_NOTICE         0x00000002

#define ADD 1
#define DEL 2
#define ALL 3
#define UPDATE 4

#define PRINT 1
#define CNTRL 2
#define ALPHA 4
#define PUNCT 8
#define DIGIT 16
#define SPACE 32

#define SEGV_LOCATION_BUFSIZE	255

#ifndef        INADDR_NONE
#define        INADDR_NONE     -1
#endif

#define TLD_VERSION 1
#define SERVER_VERSION 1
#define CTCP_VERSION 1
#define CHAN_VERSION 1
#define CS_VERSION 1
#define TLD_VERSION 1
#define EXCLUDE_VERSION 1
#define STATSDB_VERSION 1
#define ADMIN_VERSION 1

#define SMILEYS " :) ;) :-) ;-) ^_^ :-D :D :-P :P =) ;D :p "
#define TOTAL	   0
#define DAILY      1
#define WEEKLY     2
#define MONTHLY    3

#define HASH(chan)	((chan)[1] ? ((chan)[1]&31)<<5 | ((chan)[2]&31) : 0)
#define STATSHASH(chan)	((chan)[1] ? ((chan)[1]&31)<<5 | ((chan)[2]&31) : 0)
#define NICKHASH(nick)      (((nick)[0]&31)<<5 | ((nick)[1]&31))
#define CTCPHASH(version)	((tolower((version)[0])&31)<<5 | (tolower((version)[1])&31))
#define EXCLUDEHASH(name)	((tolower((name)[0])&31)<<5 | (tolower((name)[1])&31))
#define SERVERHASH(name)	(((name)[0]&31)<<5 | ((name)[1]&31))
#define STATSSERVERHASH(name)	(((name)[0]&31)<<5 | ((name)[1]&31))
#define CSHASH(nick)	((tolower((nick)[0])&31)<<5 | (tolower((nick)[1])&31))
#define TLDHASH(tld)	(((tld)[0]&31)<<5 | ((tld)[1]&31))
#define TLDHASH2(tld)	(((tld)[0]&31)<<5 | ((tld)[1]&31))
#define USERHASH(nick)	(((nick)[0]&31)<<5 | ((nick)[1]&31))
#define UIDHASH(nick)	(((nick)[0]&31)<<5 | ((nick)[1]&31))
#define ADMINHASH(nick)	(((nick)[0]&31)<<5 | ((nick)[1]&31))
#define CMD_HASH(x)      (((x)[0]&31)<<5 | ((x)[1]&31))	/* Will gen a hash from a string :) */
#define MODEHASH(x)      (((x)[0]&31)<<5 | ((x)[1]&31))
#define MODEHASH2(x)      (((x)[0]&31)<<6 | ((x)[1]&31))
#define MODEHASHSENSITVE(x)  ((x[0] >= 65 && x[0] <= 90 ? MODEHASH2(x) : MODEHASH(x)))


/* Cheaper than isspace() or isblank() */
#define issp(c) ((c) == 32)

#define STARTBUFSIZE 8192

#define MYTIME "%b %d %H:%M:%S %Y"

#define CHECK_ERR(err, msg) { \
		if (err != Z_OK) { \
				alog(LOG_ERROR, "%s error: %d", msg, err); \
		} \
}

/*
   kinda of a hack as CYGWIN runs root as 18
   but in there we really don't care to stop them
   as they are windows users under a hybrid shell
   this should be left here for any other OS that
   might decide to make root id something other
   then 0
*/
#define ROOT_UID 0

#define SQL_MYSQL 1
#define SQL_POSTGRE 2

/*************************************************************************/

/* BLOCKEND and VALUEEND have to be characters which can't be sent
 * over IRC, otherwise we would get false values.
 * -Certus
 */

#define SEPARATOR ':'           /* End of a key, seperates keys from values */
#define BLOCKEND  '\n'          /* End of a block, e.g. a whole nick/channel or a subblock */
#define VALUEEND   '\000'       /* End of a value */
#define SUBSTART   '\010'       /* Beginning of a new subblock, closed by a BLOCKEND */

#define DB_READ_SUCCESS   0
#define DB_READ_ERROR     1
#define DB_EOF_ERROR      2
#define DB_READ_BLOCKEND  3 

#define DB_WRITE_SUCCESS  0
#define DB_WRITE_ERROR    1
#define DB_WRITE_NOVAL    2

#define MAXKEYLEN 128
#define MAXVALLEN 1024

# define SIGNATURE	0x5AFEC0DE
# define FREE_SIGNATURE	0xDEADBEEF	/* Used for freed memory */

#ifndef MODULE_PATH
#ifdef _WIN32
#define MODULE_PATH "\\modules"
#else
#define MODULE_PATH "/modules"
#endif
#endif

#ifndef STATS_DIR
#ifdef _WIN32
#define STATS_DIR "stats\\"
#else
#define STATS_DIR "stats/"
#endif
#endif

#define IRCD_ENABLE 1
#define IRCD_DISABLE 0

/* GNU C attributes. */
#ifndef FUNCATTR
#ifdef HAVE_GNUC25_ATTRIB
#define FUNCATTR(x) __attribute__(x)
#else
#define FUNCATTR(x)
#endif
#endif

/* GNU C printf formats, or null. */
#ifndef ATTRPRINTF
#ifdef HAVE_GNUC25_PRINTFFORMAT
#define ATTRPRINTF(si,tc) format(printf,si,tc)
#else
#define ATTRPRINTF(si,tc)
#endif
#endif
#ifndef PRINTFFORMAT
#define PRINTFFORMAT(si,tc) FUNCATTR((ATTRPRINTF(si,tc)))
#endif

#define MAXSERVERS 5
#define MAXSORTLIST 15
#define UDPMAXRETRIES 15
#define UDPRETRYMS 2000
#define TCPWAITMS 30000
#define TCPCONNMS 14000
#define TCPIDLEMS 30000
#define MAXTTLBELIEVE (7*86400)	/* any TTL > 7 days is capped */

#define DNS_PORT 53
#define DNS_MAXUDP 512
#define DNS_MAXLABEL 63
#define DNS_MAXDOMAIN 255
#define DNS_HDRSIZE 12
#define DNS_IDOFFSET 0
#define DNS_CLASS_IN 1

/* Telling compilers about printf()-like functions: */
#ifdef __GNUC__
# define FORMAT(type,fmt,start) __attribute__((format(type,fmt,start)))
#else
# define FORMAT(type,fmt,start)
#endif

/* Some default config stuff that needs not live in confparse.h  */
#define SCONFF_STRING                   0x80000000    /* allow freeform strings */

/* variable types */

#define VARTYPE_INT     0x0001  /* integers             */
#define VARTYPE_STRING  0x0002  /* freeform strings     */
#define VARTYPE_NAME    0x0004  /* non-free name        */
#define VARTYPE_NONE    0x0008  /* doesnt take any var  */

#define QUERY_HIGH 1
#define QUERY_LOW  2

#define SORT_DESC 1
#define SORT_ASC  2

#define META_PRIVATE 1
#define META_PUBLIC  2

/**********************************************************************
 * Module Returns
 **********************************************************************/
#define MOD_ERR_OK			0
#define MOD_ERR_MEMORY		1
#define MOD_ERR_PARAMS		2
#define MOD_ERR_EXISTS		3
#define MOD_ERR_NOEXIST		4
#define MOD_ERR_NOUSER		5
#define MOD_ERR_NOLOAD      6
#define MOD_ERR_NOUNLOAD    7
#define MOD_ERR_SYNTAX		8
#define MOD_ERR_NODELETE	9
#define MOD_ERR_UNKNOWN		10
#define MOD_ERR_FILE_IO     11
#define MOD_ERR_NOSERVICE   12
#define MOD_ERR_NO_MOD_NAME 13

#define MAX_CMD_HASH 1024
#define MAX_MODE_HASH 26625


#define MOD_STOP 1
#define MOD_CONT 0

#define MOD_UNIQUE 0
#define MOD_HEAD   1
#define MOD_TAIL   2

#if defined(USE_MODULES) && !defined(_WIN32)
/* Define these for systems without them */
#ifndef RTLD_NOW
#define RTLD_NOW 0
#endif
#ifndef RTLD_LAZY
#define RTLD_LAZY RTLD_NOW
#endif
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL 0
#endif
#ifndef RTLD_LOCAL
#define RTLD_LOCAL 0
#endif
#endif

#ifndef __STRICT_ANSI__
#ifndef _WIN32
#define DENORA_INLINE inline
#else
#define DENORA_INLINE
#endif
#else
#define DENORA_INLINE
#endif


#ifdef __STRICT_ANSI__
#ifndef timercmp
#  define timercmp(tvp, uvp, cmp) (((tvp)->tv_sec cmp (uvp)->tv_sec) || ((tvp)->tv_sec == (uvp)->tv_sec && ((tvp)->tv_usec cmp (uvp)->tv_usec)))
#endif
#ifndef timerclear
#	define timerclear(tvp)  (tvp)->tv_sec = (tvp)->tv_usec = 0
#endif
#endif

#define SYNC_COMPLETE 1

#define STATUS_OP 1
#define STATUS_HALFOP 2
#define STATUS_VOICE 3
#define STATUS_OWNER 4
#define STATUS_PROTECTED 5
#define STATUS_SHUNNED 6



