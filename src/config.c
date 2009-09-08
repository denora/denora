/*
 *
 * (c) 2004-2009 Denora Team
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

/*************************************************************************/

#include "denora.h"
#define CONF_TABS
#include "confparse.h"

void flatconfig_error(int linenum, const char *message, ...);
int parse_directive(Directive * d, char *dir, int ac, char *av[MAXPARAMS],
                    int linenum, int reload, char *s);

/*************************************************************************/

/* notes on config.c
 * The parser works on two primary depths - blocks and tokens:
 *
 * block {
 *      token value;
 *      token "string value";
 *      token 123;               # int value
 *      token;                   # nonvar token
 * };
 *
 * It can also parse non-token blocks:
 *
 * block (
 *      "string string string";
 *      "string blah";
 * };
 *
 * Blocks are defined by tconftab (in confparse.h)
 * Tokens are defined by sconftab (^^^^^^^^^^^^^^)
 *
 * Each block must have a function that takes the values collected
 * and checks them against the requirements.  These functions are also
 * handy for getting variables out of the array that they are stored in.
 *
 * The array variables are placed in (an array of cVar structs) contains
 * all values for the the block, and the corrisponding sconftab item.
 */

/* these are our global lists of ACTIVE conf entries */

Conf_Modules *modules = NULL;
char *RemoteServer;
int RemotePort;
char *RemotePassword;
char *IRCDModule;
char *QuitPrefix;

char *LocalHost;
int LocalPort;

int NumExcludeServers;
char **ExcludeServers;

char *ServerName;
char *ServerDesc;
char *ServiceUser;
char *ServiceHost;
static char *temp_userhost;

char *LogChannel;
char *NetworkName;
const char *HiddenPrefix;
char *HiddenSuffix;

char *s_StatServ;
char *desc_StatServ;

char *s_StatServ_alias;
char *desc_StatServ_alias;

char *PIDFilename;
char *MOTDFilename;
char *HTMLFilename;

int ReadTimeout;
int WarningTimeout;
int KeepLogs;
int NoLogs = 0;
char *ChanStatsTrigger;
int KeepBackups;
int DumpCore;
int UseTokens;
int StatsLanguage = 0;

int HideStatsO;
int HideUline;

int ModulesNumber;
int ModulesDelayedNumber;

char *ChannelDB;
char *ctcpDB;
char *AdminDB;
char *ServerDB;
char *ChannelStatsDB;
char *TLDDB;
char *excludeDB;
char *statsDB;

char *SqlHost;
char *SqlUser;
char *SqlPass;
char *SqlName;
int SqlPort;
char *SqlSock;
int SqlRetries = 0;
int SqlRetryGap = 0;
int sqltype = 0;
int DisableMySQLOPT = 0;

char *Numeric;
int LargeNet;
int UserStatsRegistered = 0;

int UserCacheTime;
int UserCleanFreq;
int ServerCacheTime;
int ServerCleanFreq;
int UpdateTimeout;
int HTMLTimeout;
int UptimeTimeout;
int PingFrequency;
int SQLPingFreq;
int ClearInActive;
int ClearChanInActive;

int HidePasswords;
char *Smiley;

char *UserTable;
char *ChanBansTable;
char *ChanExceptTable;
char *ChanQuietTable;
char *IsOnTable;
char *ServerTable;
char *GlineTable;
char *ChanTable;
char *MaxValueTable;
char *ChanInviteTable;
char *TLDTable;
char *CTCPTable;
char *SglineTable;
char *SqlineTable;
char *ChanStatsTable;
char *ServerStatsTable;
char *AdminTable;

char *AliasesTable;
char *CStatsTable;
char *UStatsTable;
char *CurrentTable;
char *StatsTable;
char *SpamTable;

int SP_HTML;

int UseTS6;
int UseZIP;
char *NickChar;
int CTCPUsers;
int CTCPUsersEOB;

int CSDefFlag = 0;
int BackupFreq;
int KeepBackupsFor;

int AutoOp;
char *AutoMode;

int SockIPNumber;
char **ExtSockIPs;
int XMLRPC_Enable;
char *XMLRPC_Host;
int XMLRPC_Port;

char *JupeMaster;

/* this set of lists is used for loading and rehashing the config file */

Conf_Modules *new_modules = NULL;

int SQLDisableDueServerLost;
int SQLRetryOnServerLost;

char *StatsPage;
int PartOnEmpty;

tConf *mainconf;

int ThreadCount;
int UseThreading;

int TimeStarted;
int TimeStopped;

int KeepUserTable;
int KeepServerTable;

/*************************************************************************/

void initconfsettigs(void)
{
    mainconf = tconftab;
}

/*************************************************************************/

int confadd_connect(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_HOSTNAME)) {
            tmp->type = NULL;
            RemoteServer = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_PORT)) {
            tmp->type = NULL;
            RemotePort = atoi(tmp->value);
            if (RemotePort < 1 || RemotePort > 65535) {
                confparse_error(langstring(CONFIG_PORT_ERROR), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SCONFF_PASSWD)) {
            tmp->type = NULL;
            RemotePassword = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_PROTOCOL)) {
            tmp->type = NULL;
            IRCDModule = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_QUITPREFIX)) {
            tmp->type = NULL;
            QuitPrefix = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_BINDHOST)) {
            tmp->type = NULL;
            LocalHost = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_BINDPORT)) {
            tmp->type = NULL;
            LocalPort = atoi(tmp->value);
            if (LocalPort < 1 || LocalPort > 65535) {
                confparse_error(langstring(CONFIG_PORT_ERROR), lnum);
                return -1;
            }
        }
    }
    if (!RemoteServer) {
        confparse_error(langstring(CONFIG_HOSTNAME_ERROR), lnum);
        return -1;
    }
    if (BadPtr(IRCDModule)) {
        confparse_error(langstring(CONFIG_PROTOCOL_ERROR), lnum);
        return -1;
    }
    if (!QuitPrefix)
        QuitPrefix = sstrdup("Quit:");
    if (!RemotePort) {
        confparse_error(langstring(CONFIG_PORT_ERROR_NOTDEF), lnum);
        return -1;
    }
    if (BadPtr(RemotePassword)) {
        confparse_error(langstring(CONFIG_PASSWD_ERROR), lnum);
        return -1;
    }
    return lnum;
}

/*************************************************************************/

int confadd_identity(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_NAME)) {
            /* validate server name */
            if (!is_valid_server(tmp->value)) {
                confparse_error("Invalid server name", lnum);
                return -1;
            }
            tmp->type = NULL;
            ServerName = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_DESC)) {
            tmp->type = NULL;
            ServerDesc = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_USER)) {
            char *s;
            tmp->type = NULL;
            temp_userhost = sstrdup(tmp->value);

            /* Strip the user and host from user token */
            if (!(s = strchr(temp_userhost, '@'))) {
                confparse_error("Missing `@' for Stats User", lnum);
                return -1;
            }
            *s++ = 0;
            ServiceUser = temp_userhost;
            ServiceHost = s;
        } else if (tmp->type && (tmp->type->flag & SCONFF_LANGUAGE)) {
            tmp->type = NULL;
            StatsLanguage = strtol(tmp->value, NULL, 10);
            if (StatsLanguage < 1 || StatsLanguage > NUM_LANGS) {
                StatsLanguage = 1;
                confparse_error(langstring(CONFIG_INVALID_LANG), lnum);
                return -1;
            }
        }
    }
    if (BadPtr(ServerName)) {
        confparse_error(langstring(CONFIG_ID_NAME_ERROR), lnum);
        return -1;
    }
    if (BadPtr(ServerDesc) || !ServerDesc) {
        confparse_error(langstring(CONFIG_ID_DESC_ERROR), lnum);
        return -1;
    }
    if (BadPtr(ServiceUser)) {
        confparse_error(langstring(CONFIG_ID_USER_ERROR), lnum);
        return -1;
    }
    if (BadPtr(ServiceHost)) {
        confparse_error(langstring(CONFIG_ID_HOST_ERROR), lnum);
        return -1;
    }
    if (!StatsLanguage) {
        confparse_error(langstring(CONFIG_ID_LANG_ERROR), lnum);
        return -1;
    }
    if (StatsLanguage) {
        /* Now we reduce StatsLanguage by 1 */
        StatsLanguage--;
    }
    return lnum;
}

/*************************************************************************/

int confadd_statserv(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_NICK)) {
            tmp->type = NULL;
            s_StatServ = sstrdup(tmp->value);
            if (strlen(s_StatServ) > NICKMAX) {
                confparse_error(langstring(CONFIG_SS_TOLONG), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SCONFF_REAL)) {
            tmp->type = NULL;
            desc_StatServ = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_ALIASNICK)) {
            tmp->type = NULL;
            s_StatServ_alias = sstrdup(tmp->value);
            if (strlen(s_StatServ_alias) > NICKMAX) {
                confparse_error(langstring(CONFIG_SS_TOLONG), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SCONFF_ALIASREAL)) {
            tmp->type = NULL;
            desc_StatServ_alias = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_AUTOOP)) {
            tmp->type = NULL;
            AutoOp = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_AUTOMODE)) {
            tmp->type = NULL;
            AutoMode = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_PARTONEMPTY)) {
            tmp->type = NULL;
            PartOnEmpty = 1;
        }
    }
    if (BadPtr(s_StatServ)) {
        confparse_error(langstring(CONFIG_SS_NAME_ERROR), lnum);
        return -1;
    }
    if (BadPtr(desc_StatServ)) {
        confparse_error(langstring(CONFIG_SS_DESC_ERROR), lnum);
        return -1;
    }
    if (s_StatServ_alias && BadPtr(desc_StatServ_alias)) {
        confparse_error(langstring(CONFIG_SS_ALIASDESC_ERROR), lnum);
        return -1;
    }
    if (s_StatServ_alias) {
        if (BadPtr(s_StatServ_alias)) {
            confparse_error(langstring(CONFIG_SS_ALIASDESC_ERROR), lnum);
            return -1;
        }
    }
    return lnum;
}

/*************************************************************************/

int confadd_filenames(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_PID)) {
            tmp->type = NULL;
            PIDFilename = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_MOTD)) {
            tmp->type = NULL;
            MOTDFilename = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_HTMLFILE)) {
            tmp->type = NULL;
            HTMLFilename = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_CHANDB)) {
            tmp->type = NULL;
            ChannelDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_STATSDB)) {
            tmp->type = NULL;
            statsDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_CTCPDB)) {
            tmp->type = NULL;
            ctcpDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_ADMINDB)) {
            tmp->type = NULL;
            AdminDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_SERVERDB)) {
            tmp->type = NULL;
            ServerDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_CHANSTATSDB)) {
            tmp->type = NULL;
            ChannelStatsDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_TLDDB)) {
            tmp->type = NULL;
            TLDDB = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_EXCLUDEDB)) {
            tmp->type = NULL;
            excludeDB = sstrdup(tmp->value);
        }
    }
    if (!PIDFilename) {
        confparse_error(langstring(CONFIG_FILENAME_PID_ERROR), lnum);
        return -1;
    }
    if (HTMLFilename) {
        denora->do_html = 1;
    }
    if (!ChannelDB) {
        confparse_error(langstring(CONFIG_FILENAME_CHANDB_ERROR), lnum);
        return -1;
    }
    if (!ctcpDB) {
        confparse_error(langstring(CONFIG_FILENAME_CTCP_ERROR), lnum);
        return -1;
    }
    if (!ServerDB) {
        confparse_error(langstring(CONFIG_FILENAME_SERVER_ERROR), lnum);
        return -1;
    }
    if (!ChannelStatsDB) {
        confparse_error(langstring(CONFIG_FILENAME_CHANSTATS_ERROR), lnum);
        return -1;
    }
    if (!TLDDB) {
        confparse_error(langstring(CONFIG_FILENAME_TLD_ERROR), lnum);
        return -1;
    }
    if (!excludeDB) {
        confparse_error(langstring(CONFIG_FILENAME_EXCLUDE_ERROR), lnum);
        return -1;
    }
    if (!statsDB) {
        confparse_error("statsDB is not defined", lnum);
        return -1;
    }
    if (!AdminDB) {
        confparse_error("AdminDB is not defined", lnum);
        return -1;
    }
    return lnum;
}

/*************************************************************************/

int confadd_netinfo(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_NAME)) {
            tmp->type = NULL;
            NetworkName = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_NUMERIC)) {
            tmp->type = NULL;
            Numeric = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_LARGENET)) {
            tmp->type = NULL;
            LargeNet = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_USTATSREG)) {
            tmp->type = NULL;
            UserStatsRegistered = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_USTATSNOB)) {
			tmp->type = NULL;
			UserStatsExcludeBots = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_SP_HTML)) {
            tmp->type = NULL;
            SP_HTML = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_EXCLUDESERV)) {
            tmp->type = NULL;
            ExcludeServers =
                buildStringList(tmp->value, &NumExcludeServers);
        } else if (tmp->type && (tmp->type->flag & SCONFF_STATSPAGE)) {
            tmp->type = NULL;
            StatsPage = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_NICKCHAR)) {
            tmp->type = NULL;
            NickChar = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_CTCPUSERS)) {
            tmp->type = NULL;
            CTCPUsers = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_CTCPEOB)) {
            tmp->type = NULL;
            CTCPUsersEOB = 1;
        } else if (tmp->type && (tmp->type->flag & SCONFF_HIDDENPREFIX)) {
            tmp->type = NULL;
            HiddenPrefix = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_HIDDENSUFFIX)) {
            tmp->type = NULL;
            HiddenSuffix = sstrdup(tmp->value);
        }
    }
    if (BadPtr(NetworkName)) {
        confparse_error(langstring(CONFIG_NETINFO_NAME_ERROR), lnum);
        return -1;
    }
    if (!HiddenPrefix) {
        HiddenPrefix = "";
    }
    if (!HiddenSuffix) {
        HiddenSuffix = sstrdup(".users.mynet.tld");
    }
    if (LargeNet) {
        alog(LOG_NORMAL, langstr(ALOG_LARGENET_WARNING_1));
        alog(LOG_NORMAL, langstr(ALOG_LARGENET_WARNING_2));
        alog(LOG_NORMAL, langstr(ALOG_LARGENET_WARNING_3));
    }
    return lnum;
}

/*************************************************************************/

int confadd_timeout(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & TIMEFF_READ)) {
            tmp->type = NULL;
            ReadTimeout = dotime(tmp->value);
            if (ReadTimeout < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_WARNING)) {
            tmp->type = NULL;
            WarningTimeout = dotime(tmp->value);
            if (WarningTimeout < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_UPDATE)) {
            tmp->type = NULL;
            UpdateTimeout = dotime(tmp->value);
            if (UpdateTimeout < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_HTMLUPDATE)) {
            tmp->type = NULL;
            HTMLTimeout = dotime(tmp->value);
            if (HTMLTimeout < 119) {
                confparse_error(langstring(CONFIG_HTML_UPDATE_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_UPTIMEFREQ)) {
            tmp->type = NULL;
            UptimeTimeout = dotime(tmp->value);
            if (UptimeTimeout < 119) {
                confparse_error(langstring(CONFIG_UPTIME_TO_LOW), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_PINGFREQ)) {
            tmp->type = NULL;
            PingFrequency = dotime(tmp->value);
            if (PingFrequency < 119) {
                confparse_error(langstring(CONFIG_PING_TO_LOW), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_SQLFREQ)) {
            tmp->type = NULL;
            SQLPingFreq = dotime(tmp->value);
            if (SQLPingFreq < 120) {
                confparse_error(langstring(CONFIG_SQLPING_TO_LOW), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_CINACTIVE)) {
            tmp->type = NULL;
            ClearInActive = dotime(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TIMEFF_CHANINACTIVE)) {
            tmp->type = NULL;
            ClearChanInActive = dotime(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TIMEFF_UCACHE)) {
            tmp->type = NULL;
            UserCacheTime = dotime(tmp->value);
            if (UserCacheTime < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_UFREQ)) {
            tmp->type = NULL;
            UserCleanFreq = dotime(tmp->value);
            if (UserCleanFreq < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_SCACHE)) {
            tmp->type = NULL;
            ServerCacheTime = dotime(tmp->value);
            if (ServerCacheTime < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & TIMEFF_SFREQ)) {
            tmp->type = NULL;
            ServerCleanFreq = dotime(tmp->value);
            if (ServerCleanFreq < 0) {
                confparse_error(langstring(CONFIG_INVALID_TIME), lnum);
                return -1;
            }
        }
    }

    if (!ReadTimeout) {
        confparse_error(langstring(CONFIG_TIMEOUT_READ_ERROR), lnum);
        return -1;
    }
    if (!WarningTimeout) {
        confparse_error(langstring(CONFIG_TIMEOUT_WARNING_ERROR), lnum);
        return -1;
    }
    if (!UpdateTimeout) {
        confparse_error(langstring(CONFIG_TIMEOUT_UPDATE_ERROR), lnum);
        return -1;
    }
    if (!PingFrequency) {
        confparse_error(langstring(CONFIG_TIMEOUT_PING_ERROR), lnum);
        return -1;
    }
    if (!UptimeTimeout) {
        confparse_error(langstring(CONFIG_TIMEOUT_UPTIME_ERROR), lnum);
        return -1;
    }
    if (!HTMLTimeout) {
        confparse_error(langstring(CONFIG_TIMEOUT_HTML_ERROR), lnum);
        return -1;
    }
    if (!SQLPingFreq) {
        confparse_error(langstring(CONFIG_TIMEOUT_SQL_ERROR), lnum);
        return -1;
    }
    if (!ClearInActive) {
        ClearInActive = dotime("30d");
    }
    if (!ClearChanInActive) {
        ClearChanInActive = dotime("30d");
    }

    return lnum;
}

/*************************************************************************/

int confadd_options(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & OPTF_KEEPLOGS)) {
            tmp->type = NULL;
            KeepLogs = atoi(tmp->value);
            if (KeepLogs < 0) {
                confparse_error(langstring(CONFIG_ERROR_POSTIVE_VALUE),
                                lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & OPTF_LOGCHAN)) {
            tmp->type = NULL;
            LogChannel = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & OPTF_SMILEY)) {
            tmp->type = NULL;
            Smiley = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & OPTF_DUMPCORE)) {
            tmp->type = NULL;
            DumpCore = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_HIDESTATSO)) {
            tmp->type = NULL;
            HideStatsO = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_HIDEULINE)) {
            tmp->type = NULL;
            HideUline = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_TOKENS)) {
            tmp->type = NULL;
            UseTokens = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_NOLOGS)) {
            tmp->type = NULL;
            NoLogs = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_TRIGGER)) {
            tmp->type = NULL;
            if (strlen(tmp->value) == 1) {
                ChanStatsTrigger = sstrdup(tmp->value);
            } else {
                confparse_error("trigger can only be 1 character longer",
                                lnum);
            }
        } else if (tmp->type && (tmp->type->flag & OPTF_TS6)) {
            tmp->type = NULL;
            UseTS6 = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_ZIP)) {
            tmp->type = NULL;
            UseZIP = 1;
        } else if (tmp->type && (tmp->type->flag & OPTF_JUPEMASTER)) {
            tmp->type = NULL;
            JupeMaster = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & OPTF_CSDEF)) {
            tmp->type = NULL;
            if (!stricmp(tmp->value, "on")) {
                CSDefFlag = 1;
            }
            if (!stricmp(tmp->value, "notice")) {
                CSDefFlag = 2;
            }
        }
    }
    if (!Smiley) {
        Smiley = sstrdup(SMILEYS);
    }
    if (!ChanStatsTrigger) {
        ChanStatsTrigger = sstrdup("!");
    }
    return lnum;
}

/*************************************************************************/

int confadd_admin(cVar * vars[], int lnum)
{
    cVar *tmp;
    Dadmin *x = NULL;
    int c = 0, hc = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SCONFF_NAME)) {
            tmp->type = NULL;
            x = make_admin(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_PASSWD)) {
            tmp->type = NULL;
            x->passwd = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SCONFF_HOSTNAME)) {
            if ((hc + 1) > MAXHOSTS) {
                confparse_error("Excessive host definitions", lnum);
                free_admin(x);
                return -1;
            }
            tmp->type = NULL;
            if (!strchr(tmp->value, '@') && *tmp->value != '/') {
                char *newhost;
                int len = 3;
                len += strlen(tmp->value);
                newhost = (char *) malloc(len);
                ircsnprintf(newhost, sizeof(newhost), "*@%s", tmp->value);
                x->hosts[hc] = newhost;
            } else
                x->hosts[hc] = sstrdup(tmp->value);
            hc++;
        } else if (tmp->type && (tmp->type->flag & SCONFF_LANGUAGE)) {
            tmp->type = NULL;
            x->language = atoi(tmp->value);
            if (x->language < 1 || x->language > NUM_LANGS) {
                confparse_error(langstring(CONFIG_INVALID_LANG), lnum);
                return -1;
            }
            x->language--;
        }
    }
    if (!x->name) {
        confparse_error(langstring(CONFIG_ADMIN_NAME_ERROR), lnum);
        free_admin(x);
        return -1;
    }
    if (!x->hosts[0]) {
        confparse_error(langstring(CONFIG_ADMIN_HOST_ERROR), lnum);
        free_admin(x);
        return -1;
    }
    if (!x->passwd) {
        confparse_error(langstring(CONFIG_ADMIN_PASS_ERROR), lnum);
        free_admin(x);
        return -1;
    }
    x->configfile = 1;
    return lnum;
}

/*************************************************************************/

int confadd_xmlrpc(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & XMLRPC_BINDHOST_F)) {
            tmp->type = NULL;
            XMLRPC_Host = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & XMLRPC_BINDPORT_F)) {
            tmp->type = NULL;
            XMLRPC_Port = atoi(tmp->value);
        } else if (tmp->type && (tmp->type->flag & XMLRPC_ACCEPTHOST_F)) {
            tmp->type = NULL;
            SockIPNumber++;
            ExtSockIPs =
                realloc(ExtSockIPs, sizeof(char *) * SockIPNumber);
            ExtSockIPs[SockIPNumber - 1] = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & XMLRPC_ENABLE_F)) {
            tmp->type = NULL;
            XMLRPC_Enable = 1;
        }
    }
    if (XMLRPC_Enable) {
        if (!XMLRPC_Host) {
            if (LocalHost) {
                XMLRPC_Host = sstrdup(LocalHost);
            } else {
                confparse_error("XMLRPC enabled by no host defined", lnum);
                return -1;
            }
        }
    }

    return lnum;
}

/*************************************************************************/


int confadd_sql(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & SQLFF_TYPE)) {
            tmp->type = NULL;
            if (!stricmp(tmp->value, "mysql")) {
                sqltype = SQL_MYSQL;
            }
            if (!stricmp(tmp->value, "postgre")) {
                sqltype = SQL_POSTGRE;
            }
        } else if (tmp->type && (tmp->type->flag & SQLFF_HOST)) {
            tmp->type = NULL;
            SqlHost = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SQLFF_USER)) {
            tmp->type = NULL;
            SqlUser = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SQLFF_PASSWD)) {
            tmp->type = NULL;
            SqlPass = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SQLFF_NAME)) {
            tmp->type = NULL;
            SqlName = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SQLFF_SOCK)) {
            tmp->type = NULL;
            SqlSock = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & SQLFF_PORT)) {
            tmp->type = NULL;
            SqlPort = atoi(tmp->value);
            if (SqlPort < 1 || SqlPort > 65535) {
                confparse_error(langstring(CONFIG_PORT_ERROR), lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SQLFF_RETRIES)) {
            tmp->type = NULL;
            SqlRetries = atoi(tmp->value);
            if (SqlRetries < 0) {
                confparse_error(langstring(CONFIG_ERROR_POSTIVE_VALUE),
                                lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SQLFF_RETRYGAP)) {
            tmp->type = NULL;
            SqlRetryGap = atoi(tmp->value);
            if (SqlRetryGap < 0) {
                confparse_error(langstring(CONFIG_ERROR_POSTIVE_VALUE),
                                lnum);
                return -1;
            }
        } else if (tmp->type && (tmp->type->flag & SQLFF_RETRYONLOST)) {
            tmp->type = NULL;
            SQLRetryOnServerLost = 1;
        } else if (tmp->type && (tmp->type->flag & SQLFF_DISABLEOPT)) {
            tmp->type = NULL;
            DisableMySQLOPT = 1;
        } else if (tmp->type && (tmp->type->flag & SQLFF_KEEPUSERS)) {
            tmp->type = NULL;
            KeepUserTable = 1;
        } else if (tmp->type && (tmp->type->flag & SQLFF_KEEPSERVERS)) {
            tmp->type = NULL;
            KeepServerTable = 1;
        }
    }
    if (!SqlHost) {
        denora->do_sql = 0;
    }
    if (SqlHost && !SqlUser) {
        confparse_error(langstring(CONFIG_SQL_USER_ERROR), lnum);
        return -1;
    }
    if (SqlHost && !SqlName) {
        confparse_error(langstring(CONFIG_SQL_NAME_ERROR), lnum);
        return -1;
    }
    if (DisableMySQLOPT) {
        alog(LOG_NORMAL,
             "=============================================================");
        alog(LOG_NORMAL,
             "=== MySQL Optimization Disabled                           ===");
        alog(LOG_NORMAL,
             "You have choosen to run Denora without MySQL optimization");
        alog(LOG_NORMAL,
             "Should only run it like this if you are having problems with");
        alog(LOG_NORMAL,
             "MySQL and zlib being out of sync with each other");
        alog(LOG_NORMAL,
             "=============================================================");
    }

    return lnum;
}

/*************************************************************************/

int confadd_backup(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & BACKUPFF_KEEPBACKUPS)) {
            tmp->type = NULL;
            KeepBackups++;
        } else if (tmp->type && (tmp->type->flag & BACKUPFF_KEEPFOR)) {
            tmp->type = NULL;
            KeepBackupsFor = dotime(tmp->value);
        } else if (tmp->type && (tmp->type->flag & BACKUPFF_FREQ)) {
            tmp->type = NULL;
            BackupFreq = dotime(tmp->value);
        }
    }
    if (KeepBackups && !KeepBackupsFor) {
        confparse_error("KeepBackupsFor is not defined", lnum);
        return -1;
    }
    if (KeepBackups && !BackupFreq) {
        confparse_error("BackupFreq is not defined", lnum);
        return -1;
    }
    return lnum;
}

/*************************************************************************/

int confadd_threading(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & THREADING_USE_F)) {
            tmp->type = NULL;
            UseThreading++;
        } else if (tmp->type && (tmp->type->flag & THREADING_COUNT_F)) {
            tmp->type = NULL;
            ThreadCount = atoi(tmp->value);
        }
    }
    return lnum;
}

/*************************************************************************/

int confadd_tables(cVar * vars[], int lnum)
{
    cVar *tmp;
    int c = 0;

    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & TABFF_USERS)) {
            tmp->type = NULL;
            UserTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHANBANS)) {
            tmp->type = NULL;
            ChanBansTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHANEXCEPT)) {
            tmp->type = NULL;
            ChanExceptTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_ISON)) {
            tmp->type = NULL;
            IsOnTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_SERVER)) {
            tmp->type = NULL;
            ServerTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_GLINE)) {
            tmp->type = NULL;
            GlineTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHAN)) {
            tmp->type = NULL;
            ChanTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_MAXVALUE)) {
            tmp->type = NULL;
            MaxValueTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHANINVITE)) {
            tmp->type = NULL;
            ChanInviteTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_TLD)) {
            tmp->type = NULL;
            TLDTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CTCP)) {
            tmp->type = NULL;
            CTCPTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_SGLINE)) {
            tmp->type = NULL;
            SglineTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_SQLINE)) {
            tmp->type = NULL;
            SqlineTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CSALIASES)) {
            tmp->type = NULL;
            AliasesTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CSCSTATS)) {
            tmp->type = NULL;
            CStatsTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CSUSTATS)) {
            tmp->type = NULL;
            UStatsTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_STATS)) {
            tmp->type = NULL;
            StatsTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHANSTATS)) {
            tmp->type = NULL;
            ChanStatsTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_SERVSTATS)) {
            tmp->type = NULL;
            ServerStatsTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_SPAMFILTER)) {
            tmp->type = NULL;
            SpamTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CURRENT)) {
            tmp->type = NULL;
            CurrentTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_CHANQUIET)) {
            tmp->type = NULL;
            ChanQuietTable = sstrdup(tmp->value);
        } else if (tmp->type && (tmp->type->flag & TABFF_ADMINS)) {
            tmp->type = NULL;
            AdminTable = sstrdup(tmp->value);
        }

    }
    if (!UserTable) {
        confparse_error(langstring(CONFIG_TABLE_USER_ERROR), lnum);
        return -1;
    }
    if (!ChanBansTable) {
        confparse_error(langstring(CONFIG_TABLE_CHANBANS_ERROR), lnum);
        return -1;
    }
    if (!ChanExceptTable) {
        confparse_error(langstring(CONFIG_TABLE_CHANEXCEPT_ERROR), lnum);
        return -1;
    }
    if (!IsOnTable) {
        confparse_error(langstring(CONFIG_TABLE_ISON_ERROR), lnum);
        return -1;
    }
    if (!ServerTable) {
        confparse_error(langstring(CONFIG_TABLE_SERVER_ERROR), lnum);
        return -1;
    }
    if (!GlineTable) {
        confparse_error(langstring(CONFIG_TABLE_GLINE_ERROR), lnum);
        return -1;
    }
    if (!ChanTable) {
        confparse_error("Lacking chan definition in tables block", lnum);
        return -1;
    }
    if (!MaxValueTable) {
        confparse_error("Lacking maxvalue definition in tables block",
                        lnum);
        return -1;
    }
    if (!ChanInviteTable) {
        ChanInviteTable = sstrdup("chaninvites");
        return -1;
    }
    if (!TLDTable) {
        confparse_error("Lacking tld definition in tables block", lnum);
        return -1;
    }
    if (!CTCPTable) {
        confparse_error("Lacking ctcp definition in tables block", lnum);
        return -1;
    }
    if (!ChanStatsTable) {
        confparse_error("Lacking chanstats definition in tables block",
                        lnum);
        return -1;
    }
    if (!ServerStatsTable) {
        confparse_error("Lacking servstats definition in tables block",
                        lnum);
        return -1;
    }
    if (!SglineTable) {
        SglineTable = sstrdup("sgline");
    }
    if (!SqlineTable) {
        SqlineTable = sstrdup("sqline");
    }
    if (!AliasesTable) {
        AliasesTable = sstrdup("aliases");
    }
    if (!CStatsTable) {
        CStatsTable = sstrdup("cstats");
    }
    if (!UStatsTable) {
        UStatsTable = sstrdup("ustats");
    }
    if (!StatsTable) {
        StatsTable = sstrdup("stats");
    }
    if (!SpamTable) {
        SpamTable = sstrdup("spamfilters");
    }
    if (!CurrentTable) {
        CurrentTable = sstrdup("current");
    }
    if (!ChanQuietTable) {
        ChanQuietTable = sstrdup("chanquiet");
    }
    if (!AdminTable) {
        AdminTable = sstrdup("admin");
    }
    return lnum;
}

/*************************************************************************/

int confadd_modules(cVar * vars[], int lnum)
{
    cVar *tmp;
    Conf_Modules *x = new_modules;
    int c = 0, ac = 0, oc = 0;

    /* we dont free here because we do it if we fail */

    if (!x) {
        x = (Conf_Modules *) malloc(sizeof(Conf_Modules));
        memset((char *) x, '\0', sizeof(Conf_Modules));
        new_modules = x;
    } else {
        confparse_error("Multiple module blocks in config file", lnum);
        return -1;
    }
    for (tmp = vars[c]; tmp; tmp = vars[++c]) {
        if (tmp->type && (tmp->type->flag & MBTF_AUTOLOAD)) {
            if ((ac + 1) > 128) {
                confparse_error("Excessive autoloading modules (max 128)",
                                lnum);
                return -1;
            }
            tmp->type = NULL;
            x->autoload[ac] = sstrdup(tmp->value);
            ac++;
        } else if (tmp->type && (tmp->type->flag & MBTF_DELAYED)) {
            if ((oc + 1) > 128) {
                confparse_error(langstring(CONFIG_DELAYMOD_T0MANY), lnum);
                return -1;
            }
            tmp->type = NULL;
            x->delayed[oc] = sstrdup(tmp->value);
            oc++;
        }
    }
    ModulesNumber = ac;
    ModulesDelayedNumber = oc;
    return lnum;
}

/*************************************************************************/

/* Used on RELOAD */
void merge_confs()
{
    int i;

    /* dont worry about accually merging module data - its fairly
     * inactive and static data.  Just replace it.
     */
    if (modules) {
        for (i = 0; modules->autoload[i]; i++)
            free(modules->autoload[i]);
        for (i = 0; modules->delayed[i]; i++)
            free(modules->delayed[i]);
        free(modules);
    }
    modules = new_modules;
    new_modules = NULL;

    /* Rebuild admin sql table */
    reset_sqladmin();
    return;
}

/*************************************************************************/

/* Used if RELOAD fails */
void clear_newconfs()
{
    int i;

    if (new_modules) {
        for (i = 0; new_modules->autoload[i]; i++)
            free(new_modules->autoload[i]);
        for (i = 0; new_modules->delayed[i]; i++)
            free(new_modules->delayed[i]);
        free(new_modules);
        new_modules = NULL;
    }
    return;
}

/*************************************************************************/

static void free_vars(cVar * vars[])
{
    int i = 0;

    while (vars[i]) {
        if (vars[i]->value) {
            free(vars[i]->value);
        }
        free(vars[i]);
        i++;
    }
}

/*************************************************************************/

static const char *current_file = "unknown";
static int quote = 0;

void confparse_error(const char *problem, int line)
{
    alog(LOG_ERROR, langstr(CONFIG_ERROR_BASE), problem, line,
         current_file);
    if (!denora->nofork && isatty(2)) {
        dfprintf(stderr, langstr(CONFIG_ERROR_BASE), problem, line,
                 current_file);
        fprintf(stderr, "\n");
    }
    return;
}

/*************************************************************************/

/* check_quote
 * this routine skips over any ignored items inside our file
 */
static char *check_quote(char *cur)
{
    if (quote) {
        while ((cur = strchr(cur, '*')))
            if ((*(++cur) == '/')) {
                cur++;
                quote = 0;
                break;
            }
        if (!cur)
            return cur;
    }
    while ((*cur == ' ') || (*cur == '\t'))
        cur++;
    /* now we've hit something .. check for single line quotes */
    if (!*cur || *cur == '#' || *cur == '\n' ||
        (*cur == '/' && *(cur + 1) == '/'))
        return NULL;
    /* check for multiple line quotes */
    if ((*cur == '/') && (*(cur + 1) == '*')) {
        cur += 2;
        quote = 1;
        while ((cur = strchr(cur, '*')))
            if ((*(++cur) == '/')) {
                cur++;
                quote = 0;
                break;
            }
        if (!cur)
            return cur;
        else
            return check_quote(cur);
    }
    return cur;
}

/*************************************************************************/

static char *parse_block(tConf * conftab, tConf * block, char *cur,
                         FILE * file, int *lnum)
{
    char *tok, *var, *var2;
    char line[LINE_MAX];
    tConf *b2 = NULL;
    sConf *item = NULL;
    sConf *subconftab = block->subtok;
    cVar *vars[MAX_VALUES] = { 0 };
    int vnum = 0, tlnum = 0, clear = 0, done = 0, skip = 0;

    if ((subconftab) && (subconftab->flag == SCONFF_STRING)) {
        /* this subtype only takes freeform variables
         * dont bother looking for tokens
         */
        int i = 0;
        while (!BadPtr(cur) || ((fgets(line, LINE_MAX, file) != NULL) &&
                                (*lnum)++ && (cur = line))) {
            cur = check_quote(cur);
            if (BadPtr(cur))
                continue;
            if (clear) {
                if (*cur != ';') {
                    confparse_error("Missing semicolon", *lnum);
                    free_vars(vars);
                    return NULL;
                } else
                    cur++;
                clear = 0;
                cur = check_quote(cur);
                if (BadPtr(cur))
                    continue;
            }
            if (done) {
                if (*cur != ';') {
                    confparse_error("Missing block end semicolon", *lnum);
                    free_vars(vars);
                    return NULL;
                } else
                    cur++;
                if (((*block->func) (vars, *lnum)) == -1) {
                    free_vars(vars);
                    return NULL;
                }
                if (BadPtr(cur))
                    *cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
                free_vars(vars);
                return cur;
            }
            cur = check_quote(cur);
            if (BadPtr(cur))
                continue;
            if (*cur == '}') {
                done = 1;
                cur++;
                cur = check_quote(cur);
                if (BadPtr(cur))
                    continue;
                if (*cur != ';') {
                    confparse_error("Missing block end semicolon", *lnum);
                    free_vars(vars);
                    return NULL;
                } else
                    cur++;
                if (((*block->func) (vars, *lnum)) == -1) {
                    free_vars(vars);
                    return NULL;
                }
                if (BadPtr(cur))
                    *cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
                free_vars(vars);
                return cur;
            }
            vars[vnum] = (cVar *) malloc(sizeof(cVar));
            memset((char *) vars[vnum], '\0', sizeof(cVar));
            vars[vnum]->loaded = 1;
            vars[vnum]->type = NULL;
            tok = cur;
            if (*cur == '"') {
                i = 1;
                cur++;
            }
            var = cur;
            if (i == 1) {
                while (!BadPtr(cur) && (*cur != '"'))
                    cur++;
                if (BadPtr(cur)) {
                    confparse_error("Cant find closequote", *lnum);
                    free_vars(vars);
                    return NULL;
                }
                *cur = '\0';
                cur++;
                while (!BadPtr(cur) && (*cur != ';'))
                    cur++;
            } else {
                while (!BadPtr(cur) && (*cur != ';')) {
                    if ((*cur == ' ')) {
                        *cur = '\0';
                        if (vars[vnum]->loaded == 1) {
                            vars[vnum]->value = sstrdup(var);
                            vars[vnum]->loaded = 2;
                        }
                    } else if (vars[vnum]->loaded == 2) {
                        confparse_error("Junk after value", *lnum);
                        free_vars(vars);
                        return NULL;
                    }
                    cur++;
                }
            }
            tlnum = *lnum;
            if (BadPtr(cur)) {
                clear = 1;
                continue;
            }
            *cur = '\0';
            cur++;
            if (vars[vnum]->loaded == 1)
                vars[vnum]->value = sstrdup(var);
            vars[vnum]->loaded = 3;
            vnum++;
        }
        confparse_error("Unexpected EOF: Syntax Error", tlnum);
        free_vars(vars);
        return NULL;
    }

    while (!BadPtr(cur)
           || ((fgets(line, LINE_MAX, file) != NULL) && (*lnum)++
               && (cur = line))) {
        cur = check_quote(cur);
        if (BadPtr(cur))
            continue;
        if (clear) {
            /* if we're looking for a closing semicolon, check for it first
             * if we cant find it, ignore it and hope for the best
             */
            if (*cur != ';') {
                confparse_error("Missing semicolon ", *lnum);
                free_vars(vars);
                return NULL;
            } else
                cur++;
            clear = 0;
            if (vars[vnum]) {
                vars[vnum]->loaded = 3;
                vnum++;
            }
            item = NULL;
            cur = check_quote(cur);
            if (BadPtr(cur))
                continue;
        }
        if (done) {
            /* we've found the end of our block, now we're looking for the
             * closing semicolon.  if we cant find it, ignore it and
             * hope for the best
             */
            if (*cur != ';') {
                confparse_error("Missing block end semicolon", *lnum);
                free_vars(vars);
                return NULL;
            } else
                cur++;
            if (((*block->func) (vars, *lnum)) == -1) {
                free_vars(vars);
                return NULL;
            }
            if (BadPtr(cur))
                *cur = '#';     /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
            free_vars(vars);
            return cur;
        }
        if (b2 && b2->tok) {
            /* we've identified a nested block in a previous loop.
             * we didnt get an openquote yet, so look for that.
             * we must find this.  keep looking til we do.
             */
            if (*cur != '{') {
                confparse_error("Junk after nested block token", *lnum);
                free_vars(vars);
                return NULL;
            }
            cur++;
            cur = check_quote(cur);
            cur = parse_block(conftab, b2, cur, file, lnum);
            b2 = NULL;
            continue;
        }
        if (!item || !item->tok) {
            /* if we dont already have a specific token we're working on
             * find one here.
             */
            cur = check_quote(cur);
            if (BadPtr(cur))
                continue;
            tok = cur;
            tlnum = *lnum;
            if (*cur == '}') {
                /* if we've got a closebracket, then we've hit the end
                 * of our block.
                 */
                done = 1;
                cur++;
                cur = check_quote(cur);
                if (BadPtr(cur))
                    continue;
                if (*cur != ';') {
                    confparse_error("Missing block end semicolon", *lnum);
                    free_vars(vars);
                    return NULL;
                } else
                    cur++;
                if (((*block->func) (vars, *lnum)) == -1) {
                    free_vars(vars);
                    return NULL;
                }
                if (BadPtr(cur))
                    *cur = '#'; /* we cant return a bad pointer because
                                 * that will pull us out of the conf read
                                 * so this will just get ignored
                                 * kludgy, but effective */
                free_vars(vars);
                return cur;

            }
            /* our token ends where whitespace or a semicolon begins */
            while (!BadPtr(cur) && ((*cur != ' ') && (*cur != ';') &&
                                    (*cur != '\t') && (*cur != '\n')
                                    && (*cur != '\r')))
                cur++;
            if (BadPtr(cur)) {
                confparse_error("Unterminated token", *lnum);
                free_vars(vars);
                return NULL;
            } else {
                if (*cur == ';')
                    skip = 1;
                *cur = '\0';
            }
            cur++;
            if (block->nest) {
                /* we allow nested stuff inside here, so check for it. */
                for (b2 = conftab; b2->tok; b2++)
                    if (!stricmp(b2->tok, tok))
                        break;
                if (b2 && b2->tok)
                    if (!(block->nest & b2->flag))
                        b2 = NULL;
                if (b2 && b2->tok) {
                    /* recurse through the block we found */
                    tlnum = *lnum;
                    cur = check_quote(cur);
                    if (BadPtr(cur))
                        continue;
                    if (*cur != '{') {
                        confparse_error("Junk after nested block name",
                                        *lnum);
                        free_vars(vars);
                        return NULL;
                    }
                    cur++;
                    cur = check_quote(cur);
                    cur = parse_block(conftab, b2, cur, file, lnum);
                    if (!cur) {
                        free_vars(vars);
                        return NULL;
                    }
                    b2 = NULL;
                    continue;
                }
            }
            if (BadPtr(tok)) {
                continue;
            }

            /* find our token */
            for (item = subconftab; item && item->tok; item++)
                if (!stricmp(item->tok, tok))
                    break;
            if (!item->tok) {
                confparse_error("Unknown token", *lnum);
                alog(LOG_DEBUG, "debug: error token value = %s", tok);
                free_vars(vars);
                return NULL;
            }
            /* create our variable */
            vars[vnum] = (cVar *) malloc(sizeof(cVar));
            memset((char *) vars[vnum], '\0', sizeof(cVar));
            vars[vnum]->type = item;
            vars[vnum]->loaded = 1;
        }
        if (item->var & VARTYPE_NONE) {
            /* we dont need to grab a variable for this type
             * just look for the closing semicolon, and move on */
            vars[vnum]->loaded = 2;
            if (!skip) {
                /* we've already gotten our semicolon back
                 * at the end of our token.  dont look for it. */
                cur = check_quote(cur);
                while (!BadPtr(cur) && (*cur != ';'))
                    cur++;
                if (BadPtr(cur)) {
                    clear = 1;
                    continue;
                }
                cur++;
            }
            skip = 0;
            vars[vnum]->loaded = 3;
            vnum++;
            item = NULL;
            continue;
        }
        if (item->var & VARTYPE_STRING) {
            /* we're looking for a string here, so we require
             * quotes around the string...
             */
            cur = check_quote(cur);
            while (!BadPtr(cur) && (*cur != '"'))
                cur++;
            if (BadPtr(cur))
                continue;
            cur++;
            var = cur;
            while (!BadPtr(cur) && (*cur != '"'))
                cur++;
            if (BadPtr(cur)) {
                confparse_error("Unterminated quote", *lnum);
                free_vars(vars);
                return NULL;
            }
            *cur = '\0';
            cur++;
            vars[vnum]->value = sstrdup(var);
            vars[vnum]->loaded = 2;
            while (!BadPtr(cur) && (*cur != ';'))
                cur++;
            if (BadPtr(cur)) {
                clear = 1;
                continue;
            }
            cur++;
            vars[vnum]->loaded = 3;
            vnum++;
            item = NULL;
            continue;
        }
        if (item->var & VARTYPE_INT) {
            cur = check_quote(cur);
            var = cur;
            while (!BadPtr(cur) && ((*cur != ';') && (*cur != '\t') &&
                                    (*cur != '\n') && (*cur != ' ')
                                    && (*cur != '\r')))
                cur++;
            if (BadPtr(cur)) {
                clear = 1;
                continue;
            }
            if (*cur != ';')
                clear = 1;
            *cur = '\0';
            cur++;
            var2 = var;
            while (*var) {
                if (IsDigit(*var))
                    var++;
                else {
                    confparse_error("Expecting integer value", *lnum);
                    free_vars(vars);
                    return NULL;
                }
            }
            if (!item)
                continue;
            var = var2;
            vars[vnum]->value = sstrdup(var);
            vars[vnum]->loaded = 3;
            vnum++;
            item = NULL;
            continue;
        }
        if (item->var & VARTYPE_NAME) {
            cur = check_quote(cur);
            if (!BadPtr(cur) && (*cur == '"'))
                cur++;
            var = cur;
            while (!BadPtr(cur) && (*cur != ';')) {
                if ((*cur == ' ') || (*cur == '"') || (*cur == '\t')) {
                    *cur = '\0';
                    if (vars[vnum]->loaded == 1) {
                        vars[vnum]->value = sstrdup(var);
                        vars[vnum]->loaded = 2;
                    }
                }
                cur++;
            }
            if (BadPtr(cur)) {
                clear = 1;
                continue;
            }
            *cur = '\0';
            cur++;
            if (vars[vnum]->loaded == 1)
                vars[vnum]->value = sstrdup(var);
            vars[vnum]->loaded = 3;
            vnum++;
            item = NULL;
            continue;
        }
        confparse_error("Unexpected EOF:  Syntax Error", tlnum);
        free_vars(vars);
        return NULL;
    }
    confparse_error("Unexpected EOF:  Syntax Error", tlnum);
    free_vars(vars);
    return NULL;
}

/*************************************************************************/

/* Called on startup, check the config file */

int initconf(const char *filename, int reload, tConf * conftab)
{
    int lnum = 0, blnum = 0, clear = 0;
    char line[LINE_MAX];
    char *cur = NULL;
    char *tok;
    tConf *block = NULL;
    FILE *file;
    const char *strlang;
    current_file = filename;

    if (!(file = FileOpen((char *) filename, FILE_READ))) {
        strlang = langstr(CONFIG_ERROR_OPENING);
        if (!reload) {
            log_perror(strlang, filename);
        } else {
            alog(LOG_ERROR, langstr(CONFIG_ERROR_OPENING), filename);
        }
        return -1;
    }

    while (!BadPtr(cur)
           || ((fgets(line, LINE_MAX, file) != NULL) && ++lnum
               && (cur = line))) {
        cur = check_quote(cur);
        if (BadPtr(cur))
            continue;
        /* now, we should be ok to get that token.. */
        if (!block) {
            tok = cur;
            while ((*cur != ' ') && (*cur != '\n') && (*cur != '{')
                   && (*cur != '\r'))
                cur++;          /* find the whitespace following the token */
            if (*cur == '{')
                clear = 1;
            *cur = '\0';
            cur++;
            if (!stricmp("INCLUDE", tok)) {
                /* this is an include - find pull out the file name
                 * and parse this file now
                 */
                char *var;
                cur = check_quote(cur);
                if ((*cur == '"') || *cur == '<')
                    cur++;
                var = cur;
                while ((*cur != ' ') && (*cur != '"') && (*cur != '>') &&
                       (*cur != '\n') && (*cur != ';') && (*cur != '\t')
                       && (*cur != '\r'))
                    cur++;
                if (BadPtr(cur)) {
                    confparse_error("Bad include line", lnum);
                    return -1;
                }
                *cur = '\0';
                cur++;
                if (initconf(var, reload, conftab) == -1)
                    return -1;
                current_file = filename;        /* reset */
                continue;
            }
            if (BadPtr(tok))
                continue;

            for (block = conftab; block->tok; block++) {
                if (!BadPtr(block->tok) && !stricmp(block->tok, tok))
                    break;
            }
            if (!block->tok) {
                alog(LOG_DEBUG, "debug: token is %s", tok);
                confparse_error("Unknown block type", lnum);
                printf("tok %s %d\r\n", tok, *tok);
                return -1;
            }
            blnum = lnum;
        }
        cur = check_quote(cur);
        if (BadPtr(cur))
            continue;
        if ((*cur == '{') || clear)
            cur++;
        else {
            confparse_error("Junk after block name", lnum);
            return -1;
        }
        if ((cur = parse_block(conftab, block, cur, file, &lnum)) == NULL) {
            return -1;
        }
        clear = 0;
        block = NULL;
        continue;
    }
    if (clear) {
        confparse_error("Unexpected EOF:  Syntax error", blnum);
        return -1;
    }
    return 1;
}

/*************************************************************************/
/**
 * Deal with modules who want to lookup config directives!
 * @param h The Directive to lookup in the config file
 * @return 1 on success, 0 on error
 **/
int moduleGetConfigDirective(char *configfile, Directive * d)
{
    FILE *config;
    char *dir = NULL;
    char buf[1024];
    int linenum = 0;
    int ac = 0;
    char *av[MAXPARAMS];
    char *s = NULL;
    char *t;
    int retval = 1;

    config = FileOpen(configfile, FILE_READ);
    if (!config) {
        alog(LOG_NORMAL, "Can't open %s", configfile);
        return 0;
    }
    while (fgets(buf, sizeof(buf), config)) {
        linenum++;
        if (*buf == '#' || *buf == '\r' || *buf == '\n')
            continue;
        dir = myStrGetOnlyToken(buf, '\t', 0);
        if (dir) {
            s = myStrGetTokenRemainder(buf, '\t', 1);
        } else {
            dir = myStrGetOnlyToken(buf, ' ', 0);
            if (dir || (dir = myStrGetOnlyToken(buf, '\n', 0))) {
                s = myStrGetTokenRemainder(buf, ' ', 1);
            } else {
                continue;
            }
        }
        if (stricmp(dir, d->name) == 0) {
            if (s) {
                while (isspace(*s))
                    s++;
                while (*s) {
                    if (ac >= MAXPARAMS) {
                        alog(LOG_ERROR,
                             "module error: too many config. params");
                        break;
                    }
                    t = s;
                    if (*s == '"') {
                        t++;
                        s++;
                        while (*s && *s != '"') {
                            if (*s == '\\' && s[1] != 0)
                                s++;
                            s++;
                        }
                        if (!*s)
                            alog(LOG_ERROR,
                                 "module error: Warning: unterminated double-quoted string");
                        else
                            *s++ = 0;
                    } else {
                        s += strcspn(s, " \t\r\n");
                        if (*s)
                            *s++ = 0;
                    }
                    av[ac++] = t;
                    while (isspace(*s))
                        s++;
                }
            }
            retval = parse_directive(d, dir, ac, av, linenum, 0, s);
        }
    }
    if (dir) {
        free(dir);
    }
    if (s) {
        free(s);
    }

    fclose(config);
    return retval;
}

/*************************************************************************/

/* Print an error message to the log (and the console, if open). */

void flatconfig_error(int linenum, const char *message, ...)
{
    char buf[4096];
    va_list args;

    va_start(args, message);
    ircvsnprintf(buf, sizeof(buf), message, args);
    va_end(args);



    if (linenum)
        alog(LOG_DEBUG, "%d : %s", linenum, buf);
    else
        alog(LOG_DEBUG, "%s", buf);
}

/*************************************************************************/

/* Parse a configuration line.  Return 1 on success; otherwise, print an
 * appropriate error message and return 0.  Destroys the buffer by side
 * effect.
 */

int parse_directive(Directive * d, char *dir, int ac, char *av[MAXPARAMS],
                    int linenum, int reload, char *s)
{
    int retval = 1;
    int i;
    long val;
    int coptind;

    if (stricmp(dir, d->name) != 0)
        return 1;
    coptind = 0;
    for (i = 0; i < MAXPARAMS && d->params[i].type != PARAM_NONE; i++) {
        if (reload && !(d->params[i].flags & PARAM_RELOAD))
            continue;

        if (d->params[i].type == PARAM_SET) {
            *(int *) d->params[i].ptr = 1;
            continue;
        }

        if (coptind >= ac) {
            if (!(d->params[i].flags & PARAM_OPTIONAL)) {
                flatconfig_error(linenum, "Not enough parameters for `%s'",
                                 d->name);
                retval = 0;
            }
            break;
        }
        switch (d->params[i].type) {
        case PARAM_INT:
            val = strtol(av[coptind++], &s, 0);
            if (*s) {
                flatconfig_error(linenum,
                                 "%s: Expected an integer for parameter %d",
                                 d->name, coptind);
                retval = 0;
                break;
            }
            *(int *) d->params[i].ptr = val;
            break;
        case PARAM_POSINT:
            val = strtol(av[coptind++], &s, 0);
            if (*s || val <= 0) {
                flatconfig_error(linenum,
                                 "%s: Expected a positive integer for parameter %d",
                                 d->name, coptind);
                retval = 0;
                break;
            }
            if (errno == ERANGE && val == LONG_MAX) {
                /* well the true top off is 2,147,483,647 but lets not give them the real top */
                flatconfig_error(linenum,
                                 "%s: paramter %d is to large, reduce this value (0 to 2,147,483,646)",
                                 d->name, coptind);
            }
            *(int *) d->params[i].ptr = val;
            break;
        case PARAM_PORT:
            val = strtol(av[coptind++], &s, 0);
            if (*s) {
                flatconfig_error(linenum,
                                 "%s: Expected a port number for parameter %d",
                                 d->name, coptind);
                retval = 0;
                break;
            }
            if (val < 1 || val > 65535) {
                flatconfig_error(linenum,
                                 "Port numbers must be in the range 1..65535");
                retval = 0;
                break;
            }
            *(int *) d->params[i].ptr = val;
            break;
        case PARAM_STRING:
            *(char **) d->params[i].ptr = sstrdup(av[coptind++]);
            if (!d->params[i].ptr) {
                flatconfig_error(linenum, "%s: Out of memory", d->name);
                return 0;
            }
            break;
        case PARAM_TIME:
            val = dotime(av[coptind++]);
            if (val < 0) {
                flatconfig_error(linenum,
                                 "%s: Expected a time value for parameter %d",
                                 d->name, coptind);
                retval = 0;
                break;
            }
            *(int *) d->params[i].ptr = val;
            break;
        default:
            flatconfig_error(linenum, "%s: Unknown type %d for param %d",
                             d->name, d->params[i].type, i + 1);
            retval = 0;         /* don't bother continuing--something's bizarre */
            break;
        }
    }
    return retval;;
}

/*************************************************************************/
