/*
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

/*************************************************************************/

/* tokens allowing subtokens */

#define CONFT_CONNECT     "CONNECT"
#define CONFF_CONNECT                   0x00000001
#define CONFT_IDENTITY    "IDENTITY"
#define CONFF_IDENTITY                  0x00000002
#define CONFT_STATSERV    "STATSERV"
#define CONFF_STATSERV                  0x00000004
#define CONFT_FILENAMES   "FILENAMES"
#define CONFF_FILENAMES                 0x00000008
#define CONFT_NETINFO     "NETINFO"
#define CONFF_NETINFO                   0x00000010
#define CONFT_TIMEOUT     "TIMEOUT"
#define CONFF_TIMEOUT                   0x00000020
#define CONFT_OPTIONS     "OPTIONS"
#define CONFF_OPTIONS                   0x00000040
#define CONFT_ADMIN       "ADMIN"
#define CONFF_ADMIN                     0x00000080
#define CONFT_SQL         "SQL"
#define CONFF_SQL                       0x00000100
#define CONFT_TABLES      "TABLES"
#define CONFF_TABLES                    0x00000200
#define CONFT_MODULES     "MODULES"
#define CONFF_MODULES                   0x00000400
#define CONFT_BACKUP     "BACKUP"
#define CONFF_BACKUP                    0x00000800
#define CONFT_XMLRPC     "XMLRPC"
#define CONFF_XMLRPC                    0x00001000
#define CONFT_THREAD     "THREADING"
#define CONFF_THREAD                    0x00002000


/* subtokens */

#define SCONFT_HOSTNAME     "HOSTNAME"
#define SCONFF_HOSTNAME                 0x00000001
#define SCONFT_PORT         "PORT"
#define SCONFF_PORT                     0x00000002
#define SCONFT_PASSWD       "PASSWD"
#define SCONFF_PASSWD                   0x00000004
#define SCONFT_BINDHOST     "BINDHOST"
#define SCONFF_BINDHOST                 0x00000008
#define SCONFT_BINDPORT     "BINDPORT"
#define SCONFF_BINDPORT                 0x00000010
#define SCONFT_NAME         "NAME"
#define SCONFF_NAME                     0x00000020
#define SCONFT_DESC         "DESC"
#define SCONFF_DESC                     0x00000040
#define SCONFT_USER         "USER"
#define SCONFF_USER                     0x00000080
#define SCONFT_LANGUAGE     "LANGUAGE"
#define SCONFF_LANGUAGE                 0x00000100
#define SCONFT_NICK         "NICK"
#define SCONFF_NICK                     0x00000200
#define SCONFT_REAL         "REAL"
#define SCONFF_REAL                     0x00000400
#define SCONFT_ALIASNICK    "ALIASNICK"
#define SCONFF_ALIASNICK                0x00001000
#define SCONFT_ALIASREAL    "ALIASREAL"
#define SCONFF_ALIASREAL                0x00002000
#define SCONFT_PID          "PID"
#define SCONFF_PID                      0x00004000
#define SCONFT_MOTD         "MOTD"
#define SCONFF_MOTD                     0x00008000
#define SCONFT_NUMERIC      "NUMERIC"
#define SCONFF_NUMERIC                  0x00010000
#define SCONFT_NICKCHAR     "NICKCHAR"
#define SCONFF_NICKCHAR                 0x00020000
#define SCONFT_CTCPUSERS    "CTCPUSERS"
#define SCONFF_CTCPUSERS                0x00040000
#define SCONFT_HTMLFILE     "HTMLFILE"
#define SCONFF_HTMLFILE                 0x00080000
#define SCONFT_TEMPFILE     "TEPLATEFILE"
#define SCONFF_TEMPFILE                 0x00100000
#define SCONFT_LARGENET     "LARGENET"
#define SCONFF_LARGENET                 0x00200000
#define SCONFT_SP_HTML      "SPHTML"
#define SCONFF_SP_HTML                  0x00400000
#define SCONFT_EXCLUDESERV  "EXCLUDESERV"
#define SCONFF_EXCLUDESERV              0x00800000
#define SCONFT_AUTOOP       "AUTOOP"
#define SCONFF_AUTOOP                   0x01000000
#define SCONFT_STATSPAGE    "statspage"
#define SCONFF_STATSPAGE                0x02000000
#define SCONFT_AUTOMODE    "automode"
#define SCONFF_AUTOMODE                 0x04000000
#define SCONFT_CTCPEOB     "ctcpeob"
#define SCONFF_CTCPEOB                  0x08000000
#define SCONFT_PARTONEMPTY  "partonempty"
#define SCONFF_PARTONEMPTY              0x10000000
#define SCONFT_NICKTRACKING "NONICKTRACKING"
#define SCONFF_NICKTRACKING             0x20000000


#define SCONFT_PROTOCOL   "PROTOCOL"
#define SCONFF_PROTOCOL                 0x00100000



#define SCONFT_CHANDB      "CHANNELDB"
#define SCONFF_CHANDB                   0x00100000
#define SCONFT_CTCPDB      "CTCPDB"
#define SCONFF_CTCPDB                   0x00200000
#define SCONFT_SERVERDB    "SERVERDB"
#define SCONFF_SERVERDB                 0x00400000
#define SCONFT_CHANSTATSDB "CHANSTATSDB"
#define SCONFF_CHANSTATSDB              0x00800000
#define SCONFT_TLDDB   	   "TLDDB"
#define SCONFF_TLDDB                    0x01000000
#define SCONFT_EXCLUDEDB   "EXCLUDEDB"
#define SCONFF_EXCLUDEDB                0x02000000
#define SCONFT_STATSDB     "STATSDB"
#define SCONFF_STATSDB                  0x04000000
#define SCONFT_ADMINDB     "ADMINDB"
#define SCONFF_ADMINDB                  0x08000000


#define TIMEFT_READ          "READ"
#define TIMEFF_READ                     0x00000001
#define TIMEFT_WARNING       "WARNING"
#define TIMEFF_WARNING                  0x00000002
#define TIMEFT_UCACHE        "USERCACHE"
#define TIMEFF_UCACHE                   0x00000004
#define TIMEFT_UFREQ         "USERFREQ"
#define TIMEFF_UFREQ                    0x00000008
#define TIMEFT_SCACHE        "SERVERCACHE"
#define TIMEFF_SCACHE                   0x00000010
#define TIMEFT_SFREQ         "SERVERFREQ"
#define TIMEFF_SFREQ                    0x00000020
#define TIMEFT_UPDATE        "UPDATE"
#define TIMEFF_UPDATE                   0x00000040
#define TIMEFT_HTMLUPDATE    "HTMLUPDATE"
#define TIMEFF_HTMLUPDATE               0x00000080
#define TIMEFT_PINGFREQ      "PINGFREQ"
#define TIMEFF_PINGFREQ                 0x00000100
#define TIMEFT_UPTIMEFREQ    "UPTIMEFREQ"
#define TIMEFF_UPTIMEFREQ               0x00000200
#define TIMEFT_SQLFREQ       "SQLFREQ"
#define TIMEFF_SQLFREQ                  0x00000400
#define TIMEFT_CINACTIVE     "CLEARINACTIVE"
#define TIMEFF_CINACTIVE                0x00000800
#define TIMEFT_CHANINACTIVE  "CLEARCHANINACTIVE"
#define TIMEFF_CHANINACTIVE             0x00001000



#define BACKUPFT_FREQ 	     "FREQ"
#define BACKUPFF_FREQ                     0x00000001
#define BACKUPFT_KEEPFOR     "KEEPFOR"
#define BACKUPFF_KEEPFOR                  0x00000002
#define BACKUPFT_KEEPBACKUPS "KEEPBACKUPS"
#define BACKUPFF_KEEPBACKUPS              0x00000004



/* sql block definitions */

#define SQLFT_TYPE       			   "TYPE"
#define SQLFF_TYPE                     0x00000001
#define SQLFT_HOST       			   "HOST"
#define SQLFF_HOST                     0x00000002
#define SQLFT_USER       			   "USER"
#define SQLFF_USER                     0x00000004
#define SQLFT_PASSWD     			   "PASSWD"
#define SQLFF_PASSWD                   0x00000008
#define SQLFT_NAME       			   "NAME"
#define SQLFF_NAME                     0x00000010
#define SQLFT_SOCK       			   "SOCK"
#define SQLFF_SOCK                     0x00000020
#define SQLFT_PORT       			   "PORT"
#define SQLFF_PORT                     0x00000040
#define SQLFT_RETRIES    			   "RETRIES"
#define SQLFF_RETRIES                  0x00000080
#define SQLFT_RETRYGAP   			   "RETRYGAP"
#define SQLFF_RETRYGAP                 0x00000100
#define SQLFT_RETRYONLOST 			   "RETRYONLOST"
#define SQLFF_RETRYONLOST              0x00000200
#define SQLFT_DISABLEOPT 			   "DISABLEOPT"
#define SQLFF_DISABLEOPT               0x00000400
#define SQLFT_KEEPUSERS   			   "KEEPUSERS"
#define SQLFF_KEEPUSERS                0x00000800
#define SQLFT_KEEPSERVERS 			   "KEEPSERVERS"
#define SQLFF_KEEPSERVERS              0x00001000

/* tables block definitions */

#define TABFT_USERS       "USERS"
#define TABFF_USERS                    0x00000001
#define TABFT_CHANBANS    "CHANBANS"
#define TABFF_CHANBANS                 0x00000002
#define TABFT_CHANEXCEPT  "CHANEXCEPT"
#define TABFF_CHANEXCEPT               0x00000004
#define TABFT_ISON        "ISON"
#define TABFF_ISON                     0x00000008
#define TABFT_SERVER      "SERVER"
#define TABFF_SERVER                   0x00000010
#define TABFT_GLINE       "GLINE"
#define TABFF_GLINE                    0x00000020
#define TABFT_CHAN        "CHAN"
#define TABFF_CHAN                     0x00000040
#define TABFT_MAXVALUE    "MAXVALUE"
#define TABFF_MAXVALUE                 0x00000080
#define TABFT_CHANINVITE  "CHANINVITE"
#define TABFF_CHANINVITE               0x00000100
#define TABFT_TLD         "TLD"
#define TABFF_TLD                      0x00000200
#define TABFT_CTCP        "CTCP"
#define TABFF_CTCP                     0x00000400
#define TABFT_SGLINE      "SGLINE"
#define TABFF_SGLINE                   0x00000800
#define TABFT_SQLINE      "SQLINE"
#define TABFF_SQLINE                   0x00001000
#define TABFT_CSALIASES   "CSALIASES"
#define TABFF_CSALIASES                0x00002000
#define TABFT_CSCSTATS    "CSCSTATS"
#define TABFF_CSCSTATS                 0x00004000
#define TABFT_CSUSTATS    "CSUSTATS"
#define TABFF_CSUSTATS                 0x00008000
#define TABFT_STATS       "STATS"
#define TABFF_STATS                    0x00010000
#define TABFT_SPAMFILTER  "SPAMFILTER"
#define TABFF_SPAMFILTER               0x00020000
#define TABFT_CURRENT     "CURRENT"
#define TABFF_CURRENT                  0x00040000
#define TABFT_CHANSTATS   "CHANSTATS"
#define TABFF_CHANSTATS                0x00080000
#define TABFT_SERVSTATS   "SERVSTATS"
#define TABFF_SERVSTATS                0x00100000
#define TABFT_CHANQUIET   "CHANQUIET"
#define TABFF_CHANQUIET                0x00200000



/* these are the strings for options ONLY */
#define OPTT_KEEPLOGS    "KEEPLOGS"
#define OPTF_KEEPLOGS                  0x00000001
#define OPTT_LOGCHAN     "LOGCHAN"
#define OPTF_LOGCHAN                   0x00000002
#define OPTT_DUMPCORE    "DUMPCORE"
#define OPTF_DUMPCORE                  0x00000004
#define OPTT_HIDESTATSO  "HIDESTATSO"
#define OPTF_HIDESTATSO                0x00000008
#define OPTT_TOKENS      "TOKENS"
#define OPTF_TOKENS                    0x00000010
#define OPTT_TS6         "TS6"
#define OPTF_TS6                       0x00000020
#define OPTT_ZIP         "ZIP"
#define OPTF_ZIP                       0x00000040
#define OPTT_CSDEF       "chanstatsdef"
#define OPTF_CSDEF                     0x00000080
#define OPTT_SMILEY      "smiley"
#define OPTF_SMILEY                    0x00000100
#define OPTT_HIDEULINE      "hideuline"
#define OPTF_HIDEULINE                 0x00000200
#define OPTT_NOLOGS      "nologs"
#define OPTF_NOLOGS                    0x00000400
#define OPTT_TRIGGER      "trigger"
#define OPTF_TRIGGER               	   0x00000800

#define OPTT_JUPEMASTER      "jupemaster"
#define OPTF_JUPEMASTER            	   0x00001000


/* xmlrpc */
#define XMLRPC_BINDHOST    "XMLRPC_HOST"
#define XMLRPC_BINDHOST_F                  0x00000001
#define XMLRPC_BINDPORT    "XMLRPC_PORT"
#define XMLRPC_BINDPORT_F                  0x00000002
#define XMLRPC_ACCEPTHOST  "XMLRPC_ACCEPT"
#define XMLRPC_ACCEPTHOST_F                0x00000004
#define XMLRPC_ENABLE      "XMLRPC_ENABLE"
#define XMLRPC_ENABLE_F                	   0x00000008

/* threading */
#define THREADING_USE    "usethread"
#define THREADING_USE_F                  0x00000001
#define THREADING_COUNT  "threadcount"
#define THREADING_COUNT_F                0x00000002

/* module block definitions */

#define MBTT_AUTOLOAD     "AUTOLOAD"
#define MBTF_AUTOLOAD                   0x00000001
#define MBTT_DELAYED      "DELAYED"
#define MBTF_DELAYED                    0x00000002

/*************************************************************************/

/* functions for parsing variables into appropriate variables */

#ifdef CONF_TABS

E int confadd_connect(cVar **, int lnum);
E int confadd_identity(cVar **, int lnum);
E int confadd_statserv(cVar **, int lnum);
E int confadd_filenames(cVar **, int lnum);
E int confadd_netinfo(cVar **, int lnum);
E int confadd_timeout(cVar **, int lnum);
E int confadd_options(cVar **, int lnum);
E int confadd_admin(cVar **, int lnum);
E int confadd_sql(cVar **, int lnum);
E int confadd_tables(cVar **, int lnum);
E int confadd_modules(cVar **, int lnum);
E int confadd_backup(cVar **, int lnum);
E int confadd_xmlrpc(cVar **, int lnum);
E int confadd_threading(cVar **, int lnum);
E sConf sconftab[];

sConf confopentab[] =
{
    {0,SCONFF_STRING,0},
    {(char *) 0, 0, 0}
};


sConf confconnecttab[] =
{
    {SCONFT_HOSTNAME, SCONFF_HOSTNAME, VARTYPE_NAME},
    {SCONFT_PORT, SCONFF_PORT, VARTYPE_INT},
    {SCONFT_PASSWD, SCONFF_PASSWD, VARTYPE_NAME},
    {SCONFT_BINDHOST, SCONFF_BINDHOST, VARTYPE_NAME},
    {SCONFT_BINDPORT, SCONFF_BINDPORT, VARTYPE_INT},
    {SCONFT_PROTOCOL, SCONFF_PROTOCOL, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

sConf confidentitytab[] =
{
    {SCONFT_NAME, SCONFF_NAME, VARTYPE_NAME},
    {SCONFT_DESC, SCONFF_DESC, VARTYPE_STRING},
    {SCONFT_USER, SCONFF_USER, VARTYPE_NAME},
    {SCONFT_LANGUAGE, SCONFF_LANGUAGE, VARTYPE_INT},
    {(char *) 0, 0, 0}
};

sConf confstatservtab[] =
{
    {SCONFT_NICK, SCONFF_NICK, VARTYPE_NAME},
    {SCONFT_REAL, SCONFF_REAL, VARTYPE_STRING},
    {SCONFT_ALIASNICK, SCONFF_ALIASNICK, VARTYPE_NAME},
    {SCONFT_ALIASREAL, SCONFF_ALIASREAL, VARTYPE_STRING},
    {SCONFT_AUTOOP, SCONFF_AUTOOP, VARTYPE_NONE},
    {SCONFT_AUTOMODE, SCONFF_AUTOMODE, VARTYPE_STRING},
    {SCONFT_PARTONEMPTY, SCONFF_PARTONEMPTY, VARTYPE_NONE},
    {(char *) 0, 0, 0}
};

sConf conffilenamestab[] =
{
    {SCONFT_PID, SCONFF_PID, VARTYPE_NAME},
    {SCONFT_MOTD, SCONFF_MOTD, VARTYPE_NAME},
    {SCONFT_HTMLFILE, SCONFF_HTMLFILE, VARTYPE_STRING},
    {SCONFT_TEMPFILE, SCONFF_TEMPFILE, VARTYPE_NAME},
    {SCONFT_STATSDB, SCONFF_STATSDB, VARTYPE_NAME},
    {SCONFT_CHANDB, SCONFF_CHANDB, VARTYPE_NAME},
    {SCONFT_CTCPDB, SCONFF_CTCPDB, VARTYPE_NAME},
    {SCONFT_SERVERDB, SCONFF_SERVERDB, VARTYPE_NAME},
    {SCONFT_CHANSTATSDB, SCONFF_CHANSTATSDB, VARTYPE_NAME},
    {SCONFT_TLDDB, SCONFF_TLDDB, VARTYPE_NAME},
    {SCONFT_EXCLUDEDB, SCONFF_EXCLUDEDB, VARTYPE_NAME},
    {SCONFT_ADMINDB, SCONFF_ADMINDB, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

sConf confnetinfotab[] =
{
    {SCONFT_NAME, SCONFF_NAME, VARTYPE_NAME},
    {SCONFT_NUMERIC, SCONFF_NUMERIC, VARTYPE_NAME},
    {SCONFT_NICKCHAR, SCONFF_NICKCHAR, VARTYPE_STRING},
    {SCONFT_CTCPUSERS, SCONFF_CTCPUSERS, VARTYPE_NONE},
    {SCONFT_LARGENET, SCONFF_LARGENET, VARTYPE_NONE},
    {SCONFT_NICKTRACKING, SCONFF_NICKTRACKING, VARTYPE_NONE},
    {SCONFT_SP_HTML, SCONFF_SP_HTML, VARTYPE_NONE},
    {SCONFT_EXCLUDESERV, SCONFF_EXCLUDESERV, VARTYPE_STRING},
    {SCONFT_STATSPAGE, SCONFF_STATSPAGE, VARTYPE_STRING},
    {SCONFT_CTCPEOB, SCONFF_CTCPEOB, VARTYPE_NONE},
    {(char *) 0, 0, 0}
};

sConf conftimeouttab[] =
{
    {TIMEFT_READ, TIMEFF_READ, VARTYPE_NAME},
    {TIMEFT_WARNING, TIMEFF_WARNING, VARTYPE_NAME},
    {TIMEFT_UCACHE, TIMEFF_UCACHE, VARTYPE_NAME},
    {TIMEFT_UFREQ, TIMEFF_UFREQ, VARTYPE_NAME},
    {TIMEFT_SCACHE, TIMEFF_SCACHE, VARTYPE_NAME},
    {TIMEFT_SFREQ, TIMEFF_SFREQ, VARTYPE_NAME},
    {TIMEFT_UPDATE, TIMEFF_UPDATE, VARTYPE_NAME},
    {TIMEFT_HTMLUPDATE, TIMEFF_HTMLUPDATE, VARTYPE_NAME},
    {TIMEFT_PINGFREQ, TIMEFF_PINGFREQ, VARTYPE_NAME},
    {TIMEFT_UPTIMEFREQ, TIMEFF_UPTIMEFREQ, VARTYPE_NAME},
    {TIMEFT_SQLFREQ, TIMEFF_SQLFREQ, VARTYPE_NAME},
    {TIMEFT_CINACTIVE, TIMEFF_CINACTIVE, VARTYPE_NAME},
    {TIMEFT_CHANINACTIVE, TIMEFF_CHANINACTIVE, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

sConf confoptionstab[] =
{
    {OPTT_KEEPLOGS, OPTF_KEEPLOGS, VARTYPE_INT},
    {OPTT_LOGCHAN, OPTF_LOGCHAN, VARTYPE_NAME},
    {OPTT_DUMPCORE, OPTF_DUMPCORE, VARTYPE_NONE},
    {OPTT_HIDESTATSO, OPTF_HIDESTATSO, VARTYPE_NONE},
    {OPTT_TOKENS, OPTF_TOKENS, VARTYPE_NONE},
    {OPTT_TS6, OPTF_TS6, VARTYPE_NONE},
    {OPTT_ZIP, OPTF_ZIP, VARTYPE_NONE},
    {OPTT_CSDEF, OPTF_CSDEF, VARTYPE_NAME},
    {OPTT_SMILEY, OPTF_SMILEY, VARTYPE_STRING},
    {OPTT_HIDEULINE, OPTF_HIDEULINE, VARTYPE_NONE},
    {OPTT_NOLOGS, OPTF_NOLOGS, VARTYPE_NONE},
    {OPTT_TRIGGER, OPTF_TRIGGER, VARTYPE_STRING},
    {OPTT_JUPEMASTER, OPTF_JUPEMASTER, VARTYPE_STRING},
    {(char *) 0, 0, 0}
};

sConf confadmintab[] =
{
    {SCONFT_NAME, SCONFF_NAME, VARTYPE_NAME},
    {SCONFT_PASSWD, SCONFF_PASSWD, VARTYPE_NAME},
    {SCONFT_HOSTNAME, SCONFF_HOSTNAME, VARTYPE_NAME},
    {SCONFT_LANGUAGE, SCONFF_LANGUAGE, VARTYPE_INT},
    {(char *) 0, 0, 0}
};

sConf confxmlrpctab[] =
{
    {XMLRPC_BINDHOST, XMLRPC_BINDHOST_F, VARTYPE_NAME},
    {XMLRPC_BINDPORT, XMLRPC_BINDPORT_F, VARTYPE_INT},
    {XMLRPC_ACCEPTHOST, XMLRPC_ACCEPTHOST_F, VARTYPE_NAME},
    {XMLRPC_ENABLE, XMLRPC_ENABLE_F, VARTYPE_NONE},
    {(char *) 0, 0, 0}
};

sConf confthreadingtab[] =
{
    {THREADING_USE, THREADING_USE_F, VARTYPE_NONE},
    {THREADING_COUNT, THREADING_COUNT_F, VARTYPE_INT},
    {(char *) 0, 0, 0}
};

sConf confsqltab[] =
{
    {SQLFT_TYPE, SQLFF_TYPE, VARTYPE_NAME},
    {SQLFT_HOST, SQLFF_HOST, VARTYPE_NAME},
    {SQLFT_USER, SQLFF_USER, VARTYPE_NAME},
    {SQLFT_PASSWD, SQLFF_PASSWD, VARTYPE_NAME},
    {SQLFT_NAME, SQLFF_NAME, VARTYPE_NAME},
    {SQLFT_SOCK, SQLFF_SOCK, VARTYPE_NAME},
    {SQLFT_PORT, SQLFF_PORT, VARTYPE_INT},
    {SQLFT_RETRIES, SQLFF_RETRIES, VARTYPE_INT},
    {SQLFT_RETRYGAP, SQLFF_RETRYGAP, VARTYPE_INT},
    {SQLFT_RETRYONLOST, SQLFF_RETRYONLOST, VARTYPE_INT},
    {SQLFT_DISABLEOPT, SQLFF_DISABLEOPT, VARTYPE_NONE},
    {SQLFT_KEEPUSERS, SQLFF_KEEPUSERS, VARTYPE_NONE},
    {SQLFT_KEEPSERVERS, SQLFF_KEEPSERVERS, VARTYPE_NONE},
    {(char *) 0, 0, 0}
};

sConf conftablestab[] =
{
    {TABFT_USERS, TABFF_USERS, VARTYPE_NAME},
    {TABFT_CHANBANS, TABFF_CHANBANS, VARTYPE_NAME},
    {TABFT_CHANEXCEPT, TABFF_CHANEXCEPT, VARTYPE_NAME},
    {TABFT_ISON, TABFF_ISON, VARTYPE_NAME},
    {TABFT_SERVER, TABFF_SERVER, VARTYPE_NAME},
    {TABFT_GLINE, TABFF_GLINE, VARTYPE_NAME},
    {TABFT_CHAN, TABFF_CHAN, VARTYPE_NAME},
    {TABFT_MAXVALUE, TABFF_MAXVALUE, VARTYPE_NAME},
    {TABFT_CHANINVITE, TABFF_CHANINVITE, VARTYPE_NAME},
    {TABFT_TLD, TABFF_TLD, VARTYPE_NAME},
    {TABFT_CTCP, TABFF_CTCP, VARTYPE_NAME},
    {TABFT_SQLINE, TABFF_SQLINE, VARTYPE_NAME},
    {TABFT_SGLINE, TABFF_SGLINE, VARTYPE_NAME},
    {TABFT_CSALIASES, TABFF_CSALIASES, VARTYPE_NAME},
    {TABFT_CSCSTATS, TABFF_CSCSTATS, VARTYPE_NAME},
    {TABFT_CSUSTATS, TABFF_CSUSTATS, VARTYPE_NAME},
    {TABFT_STATS, TABFF_STATS, VARTYPE_NAME},
    {TABFT_SPAMFILTER, TABFF_SPAMFILTER, VARTYPE_NAME},
    {TABFT_CURRENT, TABFF_CURRENT, VARTYPE_NAME},
    {TABFT_CHANSTATS, TABFF_CHANSTATS, VARTYPE_NAME},
    {TABFT_SERVSTATS, TABFF_SERVSTATS, VARTYPE_NAME},
    {TABFT_CHANQUIET, TABFF_CHANQUIET, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

sConf confmodulestab[] =
{
    {MBTT_AUTOLOAD, MBTF_AUTOLOAD, VARTYPE_NAME},
    {MBTT_DELAYED, MBTF_DELAYED, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

sConf confbackuptab[] =
{
    {BACKUPFT_KEEPBACKUPS, BACKUPFF_KEEPBACKUPS, VARTYPE_NONE},
    {BACKUPFT_KEEPFOR, BACKUPFF_KEEPFOR, VARTYPE_NAME},
    {BACKUPFT_FREQ, BACKUPFF_FREQ, VARTYPE_NAME},
    {(char *) 0, 0, 0}
};

struct TopConf tconftab[] =
{
    {CONFT_CONNECT, CONFF_CONNECT, 0, confconnecttab, confadd_connect},
    {CONFT_IDENTITY, CONFF_IDENTITY, 0, confidentitytab, confadd_identity},
    {CONFT_STATSERV, CONFF_STATSERV, 0, confstatservtab, confadd_statserv},
    {CONFT_FILENAMES, CONFF_FILENAMES, 0, conffilenamestab, confadd_filenames},
    {CONFT_NETINFO, CONFF_NETINFO, 0, confnetinfotab, confadd_netinfo},
    {CONFT_TIMEOUT, CONFF_TIMEOUT, 0, conftimeouttab, confadd_timeout},
    {CONFT_OPTIONS, CONFF_OPTIONS, 0, confoptionstab, confadd_options},
    {CONFT_ADMIN, CONFF_ADMIN, 0, confadmintab, confadd_admin},
    {CONFT_SQL, CONFF_SQL, 0, confsqltab, confadd_sql},
    {CONFT_TABLES, CONFF_TABLES, 0, conftablestab, confadd_tables},
    {CONFT_MODULES, CONFF_MODULES, 0, confmodulestab, confadd_modules},
    {CONFT_BACKUP, CONFF_BACKUP, 0, confbackuptab, confadd_backup},
    {CONFT_XMLRPC, CONFF_XMLRPC, 0, confxmlrpctab, confadd_xmlrpc},
    {CONFT_THREAD, CONFF_THREAD, 0, confthreadingtab, confadd_threading},
};


#endif
