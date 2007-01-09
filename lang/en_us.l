# United States English language file.
#
# Denora (c) 2004-2007 Denora Team
# Contact us at dev@nomadirc.net
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id$
#
# When translating this file to another language, keep in mind that the
# order of parameters for sprintf() is fixed in the source code, so your
# messages need to take the same parameters in the same order as the
# English messages do.  (Obviously, this doesn't hold for the strftime()
# format lines immediately below.)  If you can't get a natural translation
# of a message without changing the order of the parameters, let us know
# (dev@nomadirc.net) which message is causing a problem and I'll see
# what I can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in StatServ help messages,
# "%S" is replaced by "StatServ" (or whatever it is renamed to in
# denora.conf).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.
#
# Also in help messages, please try to limit line lengths to 60 characters
# of text (not including the leading tab).  This length was chosen because
# it does not cause line wrap under default settings on most current IRC
# clients.  Remember that format characters (control-B, control-_) are not
# included in that 60-character limit (since they don't show on the user's
# screen).  Also remember that format specifiers (%S, etc.) will expand
# when displayed, so remember to take this into account; you can assume
# that the length of a pseudoclient name (%S replacement) will be eight
# characters, so reduce the maximum line length by 6 for every %S on a
# line.
#
# Finally, remember to put a tab at the beginning of every line of text
# (including empty lines).  This has to be a tab, not spaces.

###########################################################################
#
# Name of this language
#
###########################################################################

# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Español (Spanish)" or "Français (French)".

LANG_NAME
	English

###########################################################################
#
# General messages
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%b %d %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %b %d %Y
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Sun
	Mon
	Tue
	Wed
	Thu
	Fri
	Sat
# %A
STRFTIME_DAYS_LONG
	Sunday
	Monday
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Feb
	Mar
	Apr
	May
	Jun
	Jul
	Aug
	Sep
	Oct
	Nov
	Dec
# %B
STRFTIME_MONTHS_LONG
	January
	February
	March
	April
	May
	June
	July
	August
	September
	October
	November
	December


# This is used in StatServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Internal error - unable to process request.
UNKNOWN_COMMAND
	Unknown command %s.
UNKNOWN_COMMAND_HELP
	Unknown command %s.  "/msg %s HELP" for help.
SYNTAX_ERROR
	Syntax: %s
MORE_INFO
	/msg %s HELP %s for more information.
NO_HELP_AVAILABLE
	No help available for %s.

SERVICE_OFFLINE
	%s is currently offline.

SQL_DISABLED
	You can not use this command, the SQL code is disabled.

STATS_UPTIME_DHMS
	Stats up %d days, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Stats up %d day, %02d:%02d:%02d
STATS_UPTIME_HMS
	Stats up %d hours, %d minutes, %d seconds
STATS_UPTIME_HM1S
	Stats up %d hours, %d minutes, %d second
STATS_UPTIME_H1MS
	Stats up %d hours, %d minute, %d seconds
STATS_UPTIME_H1M1S
	Stats up %d hours, %d minute, %d second
STATS_UPTIME_1HMS
	Stats up %d hour, %d minutes, %d seconds
STATS_UPTIME_1HM1S
	Stats up %d hour, %d minutes, %d second
STATS_UPTIME_1H1MS
	Stats up %d hour, %d minute, %d seconds
STATS_UPTIME_1H1M1S
	Stats up %d hour, %d minute, %d second
STATS_UPTIME_MS
	Stats up %d minutes, %d seconds
STATS_UPTIME_M1S
	Stats up %d minutes, %d second
STATS_UPTIME_1MS
	Stats up %d minute, %d seconds
STATS_UPTIME_1M1S
	Stats up %d minute, %d second

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Access denied.
PERMISSION_DENIED
	Permission denied.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Yes
SAY_NO
	No

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN not defined; cannot restart. Rerun the \2configure\2 script
	and recompile Denora to enable the RESTART command.

# Module strings

STAT_MODULE_LOADED
	Module %s loaded
STAT_MODULE_UNLOADED
	Module %s unloaded
STAT_MODULE_LOAD_FAIL
	Unable to load module %s
STAT_MODULE_REMOVE_FAIL
	Unable to remove module %s
STAT_MODULE_LOAD_SYNTAX
	MODLOAD FileName
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD FileName
STAT_MODULE_NO_UNLOAD
	This module can not be unloaded.
STAT_MODULE_LIST_HEADER
	Current Module list:
STAT_MODULE_LIST
	Module: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d Modules loaded.
STAT_MODULE_INFO_LIST
	Module: %s Version: %s Author: %s loaded: %s
STAT_MODULE_CMD_LIST
	Providing command: /msg %s %s
STAT_MODULE_MSG_LIST
	Providing IRCD handler for: %s
STAT_MODULE_NO_LIST
	No modules currently loaded
STAT_MODULE_NO_INFO
	No information about module %s is available
STAT_MODULE_INFO_SYNTAX
	MODINFO FileName
MODULE_HELP_HEADER
	The following commands have been loaded by a module:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	%S commands:
	    STATS       Show status of network
	    LOGIN       Makes you a Stats Administrator.
	    LOGOUT      Logout from your admin account.

STAT_HELP_STATS
	Syntax: STATS
	
	Shows the current number of servers, channels and users
	connected to the network.

STAT_HELP_SHUTDOWN
	Syntax: SHUTDOWN
	
	Causes Stats to save all databases and then shut down.
	
	Limited to Stats Admin

STAT_HELP_RESTART
	Syntax: RESTART
	
	Causes Denora to restart.
	(i.e. exit and immediately re-run the executable).

STAT_HELP_RELOAD
	Syntax: RELOAD
	
	Causes Denora to reload the configuration file.

STAT_HELP_ADMIN_CMD
	Commands available to Stats Admins only:
	    ADMIN       Manage Stats Admins
	    CHANSTATS   Manage Monitored Channels for Stats
	    ENVINFO     Show environment information
	    EXCLUDE     Exclude users, channels from stats
	    EXPORT      Export data to XML
	    HTML        Forces to create the HTML file
	    MODLOAD     Load a module
	    MODUNLOAD   Un-Load a module
	    MODLIST     List loaded modules
	    MODINFO     Info about a loaded module
	    RESTART     Causes Denora to restart.
	    RELOAD      Causes Denora to reload the configuration file
	    SET         Set various run time options.
	    SHUTDOWN    Terminate the Stats program with save

STAT_HELP_MODLOAD
	Syntax: MODLOAD FileName

	This command loads the module named FileName from the modules
	directory.

	Limited to Stats Admins.

STAT_HELP_MODUNLOAD
	Syntax: MODUNLOAD FileName

	This command unloads the module named FileName from the modules
	directory.

	Limited to Stats Admins.

STAT_HELP_MODINFO
	Syntax: MODINFO FileName

	This command lists information about the specified loaded module

	Limited to Stats Admins.

STAT_HELP_MODLIST
	Syntax: MODLIST

	Lists all currently loaded modules.

	Limited to Stats Admins.

STAT_HELP_EXCLUDE
	Syntax: EXCLUDE {ADD|DEL|LIST} target
	
	Exclude the given target from Stats
	
	Limited to Stats Admins

STAT_HELP_EXPORT
	Syntax: EXPORT what 
	
	Export the required data to XML, where
	what is one of the following options:
	
	channels,users,tld,servers,stats,all
	
	Limited to Stats Admins

STAT_HELP_HTML
	Syntax: HTML
	
	Forces to generate the HTML file

STAT_HELP_USERDEBUG
	Syntax: USERDEBUG user 
	
	Return Debug info for the give user

STAT_HELP_LOGIN
	Syntax: LOGIN name password
	
	Makes you a Stats Administrator.

STAT_HELP_LOGOUT
	Syntax: LOGOUT
	
	Logout from your admin account.

STAT_HELP_SET
	Syntax: SET OPTION [ON|OFF]

	Set various run time options.

	Possible options are:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Limited to Stats Admins.

STAT_HELP_ENVINFO
	Syntax: ENVINFO
	
	Show environment information.
	
	Limited to Stats Admins.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	You are now a Stats Administrator.

STAT_ADMIN_LOGGED_OUT
	You are now logged out of your admin account.

STAT_ADMIN_ALREADY_LOGGED
	You are already a Stats Administrator!

STAT_LOGIN_SYNTAX
	LOGIN name password

STAT_RELOAD
	Stats' configuration file has been reloaded.

###########################################################################
#
# STATS messages
#
###########################################################################

STAT_STATS_SERVER_COUNT
	Statistics of servers for %s:	
	Servers actually linked:  %d
	Maximum of servers connected: %d

STAT_STATS_CHAN_COUNT
	Statistics of channels for %s:	
	Channels actually used:  %d
	Maximum of channels created: %d

STAT_STATS_USER_COUNT
	Statistics of users for %s	
	Users actually connected:  %d
	Maximum of users connected: %d

STAT_STATS_SYNTAX
	STATS [SERVERS|CHANNELS|USERS]

STAT_STATS_PAGE
	For more stats visit %s

# SET responses
STAT_SET_SYNTAX
	SET option setting
STAT_SET_DEBUG_ON
	Stats are now in debug mode.
STAT_SET_DEBUG_OFF
	Stats are now in non-debug mode.
STAT_SET_DEBUG_LEVEL
	Stats are now in debug mode (level %d).
STAT_SET_DEBUG_ERROR
	Setting for DEBUG must be ON, OFF, or a positive number.
STAT_SET_UNKNOWN_OPTION
	Unknown SET option %s.
STAT_SET_SQL_ON
	SQL code will be used.
STAT_SET_SQL_OFF
	SQL code will not be used.
STAT_SET_SQL_ERROR
	Setting for SQL must be ON or OFF.
STAT_SET_SQL_ERROR_INIT
	Error during init of SQL, check your logs to correct.
STAT_SET_SQL_ERROR_DISABLED
	SQL is not configured for this network. Please edit the
	configuration file (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s is enable
STAT_SET_LIST_OPTION_OFF
	%s is disabled
STAT_SET_HTML_ON
	HTML output will be used.
STAT_SET_HTML_OFF
	HTML output will not be used.
STAT_SET_HTML_ERROR
	Setting for HTML must be ON or OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Channel Stats allows you to monitor channels much like eggdrops
	and get information on number of letters, line count etc..
	
	CHANSTATS ADD channel
	CHANSTATS DEL channel
	CHANSTATS LIST
	CHANSTATS SET channel FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER user1 user2
	CHANSTATS RENAME user1 user2
	
	The CHANSTATS ADD command adds the given channel to the monitored
	channels list.
	
	The CHANSTATS DEL command removes the given channel from the
	monitored channels list.
	
	The CHANSTATS LIST command shows you the list of the monitored
	channles on the network.
	
	The CHANSTATS SET command activates the fantasy commands for the
	given channel. You can make StatServ to show them using normal
	private messages to the channel (ON), or using notices instead.
	
	The CHANSTATS SUMUSER command transfers all stats from user2
	to user1 and deletes user2.
	
	The CHANSTATS RENAME command renames user1 into user2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s is now set to be monitored for channel stats.

STAT_CHANSTATS_CHAN_LARGENET
	LargeNet is enabled you can not added new channels.

STAT_CHANSTATS_LOGCHAN
	LogChannel can not be added to channel stats

STAT_CHANSTATS_NOT_VALID_CHAN
	You have not entered a valid channel name

STAT_CHANSTATS_CHAN_ALREADY
	%s is already set to be monitored for channel stats.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD channel 

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL channel

STAT_CHANSTATS_CHAN_DEL
	Deleting %s from channel stats monitoring.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s does not match monitored channels stats monitoring.

STATS_CHANSTATS_PART
	No Longer monitoring %s

STATS_CHANSTATS_NOCHANNELS
	No Monitored channels to list

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET channel FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Fantasy commands for %s are now enabled

STATS_CHANSTATS_FANTASY_OFF
	Fantasy commands for %s are now disabled

STATS_CHANSTATS_FANTASY_NOTICE
	Fantasy command output for %s is now noticed to user

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER user1 user2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME user1 user2

STATS_CHANSTATS_SUMUSER_DONE
	The data from %s has been transferred to %s

STATS_CHANSTATS_RENAMEUSER_DONE
	User %s has been renamed to %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} target

STAT_EXCLUDE_ADDED
	%s is now being excluded from Stats

STAT_EXCLUDE_ALREADY
	%s is already excluded from Stats

STAT_EXCLUDE_DELETED
	%s has been removed from the excludes list

STAT_EXCLUDE_NOT_FOUND
	%s is not on the exclude list

STAT_EXCLUDE_LIST_HEADER
	Exclude List

# EXPORT responses
STATS_EXPORT_CHANNELS
	Exported Channels to %s

STATS_EXPORT_USERS
	Exported Users to %s

STATS_EXPORT_TLD
	Exported Top Level Domain to %s

STATS_EXPORT_SERVERS
	Exported Servers to %s

STATS_EXPORT_STATS
	Exported stats to %s

STATS_EXPORT_ALL
	Exported all information to  %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s is not existing in the stats db.

STATS_RENAMEUSER_EXISTING
	User %s already exists in the stats db.

STATS_HELP_CHANNEL_1
	The following fantasy commands are available

STATS_HELP_CHANNEL_2
	%stop10  :   The top 10 users of the current channel

STATS_HELP_CHANNEL_3
	%stop    :   The top 3  users of the current channel

STATS_HELP_CHANNEL_4
	%sgtop10 :   The top 10 users of the whole network

STATS_HELP_CHANNEL_5
	%sgtop   :   The top 3  users of the whole network

STATS_HELP_CHANNEL_6
	%sstats  :   User stats for the channel

STATS_HELP_CHANNEL_7
	%sgstats :   User stats for the network

STATS_HELP_CHANNEL_8
	%speak   :   User peak for the current channel

STATS_TOP10_CHANNEL
	Top 10 of %s

STATS_TOP_CHANNEL
	Top 3 of %s

STATS_TOP10_NETWORK
	Top 10 of Network

STATS_TOP_NETWORK
	Top 3 of Network

STATS_USER_NETWORK
	Network stats for %s

STATS_USER_CHANNEL
	Channel stats for %s on %s

STATS_MESSAGE
	%2i %-16s letters: %s, words: %s, lines: %s, smileys: %s, actions: %s

STATS_MESSAGE_ONE
	letters: %s, words: %s, lines: %s, smileys: %s, actions: %s

STATS_CHANNEL_PEAK
	Channel user peak was %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Environment information for Stats:
STAT_ENVINFO_VERSION
	Stats version:            %s
STAT_ENVINFO_PROTOCOL
	IRCd protocol:            %s
STAT_ENVINFO_UPLINK
	Stats uplink:             %s
STAT_ENVINFO_SQL
	SQL code:                 %s
STAT_ENVINFO_HTML
	HTML code:                %s
STAT_ENVINFO_DEBUG
	Debugging:                %s
STAT_ENVINFO_MODULESLOADED
	Number of loaded modules: %d
STAT_ENVINFO_LANGUAGE
	Network language:         %s
STAT_ENVINFO_FOOTER
	End of Environment Information

STATS_HTML_EXPORTED
	Stats HTML file has been generated

# HTML code
HTML_LANG
	<META HTTP-EQUIV="CHARSET" CONTENT="ISO-8859-1">
	<META HTTP-EQUIV="CONTENT-LANGUAGE" CONTENT="English">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Network Statistics:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<td>Current Users: </td> <td> %ld </td> <td>Maximum Users: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Current Channels: </td> <td> %ld </td> <td>Maximum Channels: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Current Opers: </td> <td> %ld </td> <td>Maximum Opers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Current Servers: </td> <td> %ld </td> <td>Maximum Servers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Users Set Away: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Network Statistics for %s

HTML_TLD_TITLE
	Current Top Level Domain Statistics

HTML_TLD_HEADER
	<table><tr><th>Country Code</th><th>Country</th><th>Count</th><th>Overall</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Server name</th></tr>

HTML_SERVER_CONTENT
	<tr><td height="4">Server: </td><td height="4"><a href=#%s> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td height="4">Server: </td><td height="4"><a href=#%s> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) indicates server is online at the moment<br>(Click on a Server name for more information)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Daily Network Statistics:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4"><center>(All Daily Statistics are reset at Midnight)</center></td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Max Daily Users: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Max Daily Channels:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Max Daily Opers: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Max Daily Servers: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Version</th><th align=right>Count</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name=%s>Server:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Current Users:</td><td>%ld (%ld)</td><td>Max %ld at [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Current Opers:</td><td>%ld (%ld)</td><td>Max %ld at [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Current Ping</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Total Users Connected:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Highest Ping</td><td>%ld</td><td>at [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Server Splits</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Last Seen:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>IrcOp Kills</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Server Kills</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Channel</th><th align=right>Members</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Channel</th><th align=right>Total Joins</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) indicates channel is active at the moment

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Unwelcome Channels

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Channel</th><th>Total Kicks</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) indicates channel is active at the moment

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Channel</th><th>Total Topics</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) indicates channel is active at the moment

HTML_TOP10_TOPIC_TITLE
	Top10 Topic Change Channels

HTML_CUR_NETSTATS_TITLE
	Current Network Statistics

HTML_DAILY_TITLE
	Daily Network Statistics

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 Online Channels

HTML_SERVERLIST_TITLE
	Server List

HTML_TOP10_CHANEVER_TITLE
	Top10 Channels Ever

HTML_SERVERDETAILS_TITLE
	Server Details

HTML_TOP10_CLIENT_TITLE
	Top10 Client Versions

HTML_NETMAP_TITLE
	Network Map

HTML_VERSION
	Version is %s %s

HTML_MOST_UNWELCOME_TITLE
	Most Unwelcome Channels

HTML_POPULAR_TITLE
	Popular Channels

HTML_MOST_TOPIC_TITLE
	Most Topic Channels

HTML_NETSTATS_TITLE
	Network Statistics

NOW_STATS_ADMIN
	%s (%s) is now a Stats administrator.

STATS_AUTH_FAILED
	Authentication failed for %s

STATS_ADMIN_LOGOUT
	%s is no longer a Stats administrator.

SHUTDOWN_SAVING_DB
	Updating databases on shutdown, please wait.

CANNOT_OPEN_LOG
	Warning: couldn't open logfile: %s

GLOBAL_LOAD_MOD
	%s loaded module %s

GLOBAL_UNLOAD_MOD
	%s unloaded module %s

GLOBAL_FATAL_ERROR
	FATAL ERROR! %s: %s

CONFIG_PORT_ERROR
	Port numbers must be in the range 1025..65535

CONFIG_HOSTNAME_ERROR
	Lacking hostname definition in connect block

CONFIG_PROTOCOL_ERROR
	Lacking IRCDModule (protocol) definition in connect block

CONFIG_PORT_ERROR_NOTDEF
	Lacking port definition in connect block

CONFIG_PASSWD_ERROR
	Lacking passwd definition in connect block

CONFIG_PING_TO_LOW
	Ping Frequency must be 2 minutes or greater

CONFIG_SQLPING_TO_LOW
	SQL Ping Frequency must be 2 minutes or greater

CONFIG_UPTIME_TO_LOW
	Uptime Frequency must be 2 minutes or greater

CONFIG_DELAYMOD_T0MANY
	Excessive delayed modules (max 128)

CONFIG_TABLE_USER_ERROR
	Lacking users definition in tables block

CONFIG_TABLE_CHANBANS_ERROR
	Lacking chanbans definition in tables block

CONFIG_TABLE_CHANEXCEPT_ERROR
	Lacking chanexcept definition in tables block

CONFIG_TABLE_ISON_ERROR
	Lacking ison definition in tables block

CONFIG_TABLE_SERVER_ERROR
	Lacking server definition in tables block

CONFIG_TABLE_GLINE_ERROR
	Lacking gline definition in tables block

CONFIG_INVALID_LANG
	Invalid language number

CONFIG_ID_HOST_ERROR
	Lacking user (host) definition in identity block

CONFIG_ID_LANG_ERROR
	Lacking language definition in identity block

CONFIG_ID_USER_ERROR
	Lacking user definition in identity block

CONFIG_ID_DESC_ERROR
	Lacking desc definition in identity block

CONFIG_ID_NAME_ERROR
	Lacking name definition in identity block

CONFIG_ERROR_POSTIVE_VALUE
	A positive value is expected

CONFIG_SQL_USER_ERROR
	Lacking user definition in sql block

CONFIG_SQL_NAME_ERROR
	Lacking name definition in sql block

CONFIG_ADMIN_NAME_ERROR
	Lacking name in admin block

CONFIG_ADMIN_HOST_ERROR
	Lacking hostname in admin block

CONFIG_ADMIN_PASS_ERROR
	Lacking passwd in admin block

CONFIG_TIMEOUT_PING_ERROR
	Lacking pingfreq definition in timeout block

CONFIG_TIMEOUT_SQL_ERROR
	Lacking sqlfreq definition in timeout block

CONFIG_TIMEOUT_UPTIME_ERROR
	Lacking uptimefreq definition in timeout block

CONFIG_TIMEOUT_HTML_ERROR
	Lacking html definition in timeout block

CONFIG_TIMEOUT_UPDATE_ERROR
	Lacking update definition in timeout block

CONFIG_TIMEOUT_WARNING_ERROR
	Lacking warning definition in timeout block

CONFIG_TIMEOUT_READ_ERROR
	Lacking read definition in timeout block

CONFIG_INVALID_TIME
	This is not a valid time value

CONFIG_HTML_UPDATE_TIME
	HTML Time Out must be 2 minutes or greater

CONFIG_NETINFO_NAME_ERROR
	Lacking name definition in netinfo block

CONFIG_FILENAME_PID_ERROR
	Lacking pid definition in files block

CONFIG_FILENAME_MOTD_ERROR
	Lacking motd definition in files block

CONFIG_FILENAME_CHANDB_ERROR
	Lacking ChannelDB definition in files block

CONFIG_FILENAME_CTCP_ERROR
	Lacking ctcpDB definition in files block

CONFIG_FILENAME_SERVER_ERROR
	Lacking ServerDB definition in files block

CONFIG_FILENAME_CHANSTATS_ERROR
	Lacking ChannelStatsDB definition in files block

CONFIG_FILENAME_TLD_ERROR
	Lacking TLDDB definition in files block

CONFIG_FILENAME_EXCLUDE_ERROR
	Lacking EXCLUDEDB definition in files block

CONFIG_SS_DESC_ERROR
	Lacking real definition in statserv block

CONFIG_SS_NAME_ERROR
	Lacking nick definition in statserv block

CONFIG_SS_TOLONG
	Nickname too long

CONFIG_SS_ALIASDESC_ERROR
	Lacking aliasreal definition in statserv block

ALOG_FAILED_SQL_CONNECT
	Failed to connect to database : %s

ALOG_SQL_ENABLED
	%s: has been enabled.

ALOG_SQL_DISABLED
	MySQL: has been disabled.

ALOG_SQL_NOTSET
	MySQL Error: Set all required configuration options.

ALOG_HTML_ERR_WRITE
	Error: unable to accessing file for writing: %s

ALOG_DB_ERROR
	WARNING! DB_READ_ERROR in %s

ALOG_DEBUG_DB_OK
	debug: %s read successfully

ALOG_DEBUG_UNKNOW_CMODE
	debug: unknown mode channel mode %c

ALOG_DEBUG_LAST_LINE
	debug: last line is %s

ALOG_DEBUG_REPORT_AT
	debug: report these to the bugtracker at http://bugtracker.nomadirc.net

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Channel: received join of non-existing user %s on channel ID %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Changing modes for %s to %s

AL0G_DEBUG_BAD_CHAN_MODE
	Debug: Malformed mode detected on %s.

ALOG_DEBUG_MODE_NO_PARAM
	channel: mode %c%c with no parameter (?) for channel %s

ALOG_DEBUG_FINDCHAN_BAD
	Error: findchan() called with NULL values

ALOG_DEBUG_FINDSTATCHAN_BAD
	Error: findstatschan() called with NULL values

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: SJOIN for nonexistent user %s on %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: JOIN for nonexistent user %s on %s

ALOG_DEBUG_JOIN
	debug: %s joins %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: KICK for nonexistent user %s on %s: %s

ALOG_DEBUG_KICKED
	debug: kicking %s from %s

ALOG_DEBUG_PART_NONEXISTANT
	user: PART from nonexistent user %s: %s

ALOG_DEBUG_PART
	debug: %s leaves %s

ALOG_DEBUG_PART_BUG
	user: BUG parting %s: channel entry found but c->chan NULL

ALOG_NON_TS_MODE
	TSMODE enabled but MODE has no valid TS

ALOG_DEBUG_NO_FIND_CHAN
	debug: can't find chan %s

ALOG_DEBUG_TOPIC_TS
	debug: encoded TOPIC TS %s converted to %d

ALOG_TOPIC_NONEXISTANT
	channel: TOPIC %s for nonexistent channel %s

ALOG_ADD_BAN_BAD
	debug: add_ban called with NULL values

ALOG_ADD_BAN_ADDED
	debug: Added ban %s to channel %s

ALOG_ADD_EXCEPTION_ADDED
	debug: Added except %s to channel %s

ALOG_ADD_INVITE_ADDED
	debug: Added invite %s to channel %s

ALOG_CREATE_CHAN
	debug: Creating channel %s

ALOG_DELETE_CHAN
	debug: Deleting channel %s

ALOG_BAN_FREE_ERROR
	channel: BUG freeing %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	channel: BUG freeing %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	debug: Deleted ban %s from channel %s

ALOG_EXCEPTION_DEL_CHAN
	debug: Deleted except %s from channel %s

ALOG_INVITE_DEL_CHAN
	debug: Deleted invite %s from channel %s

ALOG_DEBUG_FLOOD_MODE
	debug: Flood mode for channel %s set to %s

ALOG_NO_FLOOD
	no flood settings

ALOG_KEY_SET_TO
	debug: Key of channel %s set to %s

ALOG_NO_KEY
	no key

ALOG_LIMIT_SET_TO
	debug: Limit of channel %s set to %u

ALOG_REDIRECT_SET_TO
	debug: Redirect of channel %s set to %s

ALOG_NO_REDIRECT
	no redirect

ALOG_DEBUG_BAD_FINDSGROUP
	Error: findsgroup() called with NULL values

ALOG_LARGENET_WARNING_1
	You have selected to run Denora with the least amount of data

ALOG_LARGENET_WARNING_2
	stored in SQL, you should only use this option if you are on a

ALOG_LARGENET_WARNING_3
	very large network of 100,000+ users or have a remote sql database

CONFIG_ERROR_BASE
	Conf Error:  %s near line %d of %s

CONFIG_UNKNOWN_FILE
	unknown

CONFIG_ERROR_OPENING
	Unable to open config file %s

ALOG_DEBUG_RESOLVED_HOST
	debug: Resolved %s to %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() called with NULL-arg

ALOG_RESET_DAILY
	resetting daily stats

ALOG_RESETTING_SUNDAY
	resetting weekly stats (on Sunday)

ALOG_RESETTING_MONDAY
	resetting weekly stats (on Monday)

ALOG_RESETTING_TUESDAY
	resetting weekly stats (on Tuesday)

ALOG_RESETTING_WEDNESDAY
	resetting weekly stats (on Wednesday)

ALOG_RESETTING_THURSDAY
	resetting weekly stats (on Thursday)

ALOG_RESETTING_FRIDAY
	resetting weekly stats (on Friday)

ALOG_RESETTING_SATURDAY
	resetting weekly stats (on Saturday)

ALOG_RESETTING_MONTHLY
	resetting monthly stats

ALOG_TRY_TO_DISPLAY
	debug: trying to display message %s

ALOG_DONE_DISPLAY
	debug: done displaying message %s

ALOG_UKNOWN_USER_MODE
	debug: unknown mode : usermode %c (in %s)

ALOG_SWHOIS_ERROR
	SWHOIS set for nonexistent user %s.

ALOG_VHOST_FOR
	debug: %s changes its vhost to %s

ALOG_REALNAME_FOR
	debug: %s changes its realname to %s

ALOG_USERNAME_FOR
	debug: %s changes its username to %s

ALOG_UMODE_NONEXISTANT
	user: MODE %s for nonexistent nick %s: %s

ALOG_UNKNWON_MSG
	debug: unknown message from server (%s)

ALOG_TRYING_LOAD_MOD
	trying to load [%s]

ALOG_STATUS
	status: [%d]

ALOG_TRYING_UNLOAD_MOD
	trying to unload [%s]

RESTARTING
	Restarting

RESTART_FAILED
	Restart failed

SAVING_FFF
	debug: Saving FFF information to disk

ALOG_SIGPIPE_WARNING
	Denora will execute a backtrace, and continue on, however Denora may become unstable

ALOG_NO_STATS_BIN
	Restart attempt failed--STATS_BIN not defined (rerun configure)

ALOG_ERR_REHASH_NO_MEM
	Error during the reload of the configuration file, but out of memory!

ALOG_ERR_REHASH
	Error Reading Configuration File (Received SIGHUP)

BACKTRACE_LINES
	Backtrace: report the following lines

BACKTRACE_NOT_HERE
	Backtrace: not available on this platform - use GDB

ALOG_DEBUG_SERVINTRO
	debug: Server introduced (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Server introduced (%s) from %s

ALOG_DEBUG_PINGTIME
	debug: Ping Time from %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: findserver() called with NULL values

ALOG_ERR_FINDSERVERSTATS
	Error: findserverstats() called with NULL values

ALOG_ERR_FINDSERVERSUID
	Error: findserver_suid() called with NULL values

ALOG_ERR_DEL_SERVSTATS
	Error: delete_serverstats() called with NULL values

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() called for %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() completed

ALOG_ERR_DEL_SERVER
	Error: delete_server() called with NULL values

ALOG_DEL_SERVER_FOR
	debug: delete_server() called for %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() cleared all users for NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() completed

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version received unknown version format

ALOG_MOD_TRY_LOAD
	trying to load [%s]

ALOG_MOD_STATUS
	status: [%d][%s]

ALOG_MOD_ERROR
	Module error: %s

ALOG_MOD_BE_ONLY_ONE
	You cannot have 2 protocol modules loaded.

ALOG_MOD_UNLOAD_SELF
	debug: %s requested unload...

ALOG_CMD_DISPLAY_START
	debug: trying to display command %s

ALOG_CMD_DISPLAY_END
	debug: done displaying command %s

ALOG_CMD_DISPLAY_LIST
	debug: Displaying command list for %s

ALOG_MSG_DISPLAY_START
	debug: trying to display message %s

ALOG_MSG_DISPLAY_END
	debug: done displaying message %s

ALOG_MSG_DISPLAY_LIST
	debug: Displaying message list for %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: Added module CallBack: [%s] due to execute at %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST for nonexistent user %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME for nonexistent user %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME for nonexistent user %s

STAT_ADMIN_ALREADY
	%s is already a stats admin

STAT_ADMIN_CREATED
	%s is now a Stats Admin

STAT_ADMIN_DELETED
	%s has been removed from Stats Admin

STAT_ADMIN_NOTADMIN
	%s is not a Stats Admin

STAT_ADMIN_SHOW
	Username: %s Hostmask: %s Language ID: %d

STAT_HELP_ADMIN
	Add or Remove a Stats Admin does not affect admins whom
	Are added in the config file.
	
	ADMIN ADD username password [hostmask] [lang-number]
	ADMIN DEL username
	ADMIN SETPASS username new-password
	ADMIN SHOW username
	ADMIN LIST
	
	The ADMIN ADD command adds the specified user as a Stats Admin
	
	The ADMIN DEL command removes the specified user from Stats Admin
	
	The ADMIN SETPASS command allows you to change the password of
	the specified admin.
	
	The ADMIN SHOW command allows you to see the details of the
	specified admin
	
	The ADMIN LIST command will display a list of all currently
	available Stat Admins accounts
	
	Limited to Stats Admin

STAT_ADMIN_SYNTAX
	ADMIN {ADD|DEL|SETPASS|SHOW|LIST} [user|pass|host|langnum]

STAT_ADMIN_DELETE_CONFIG
	Can not delete %s as they are stored in the config file

STAT_CHGPASS_OK
	Password has been changed for %s

STAT_CHGPASS_CONFIG
	Cannot change config file admin passwords
