/*************************************************************************/
/* Internal Events                                                       */
/*                                                                       */
/* (c) 2004-2011 Denora Team                                             */
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

#define EVENT_START "start"
#define EVENT_STOP  "stop"

#define EVENT_MODE_ADD    "add"
#define EVENT_MODE_REMOVE "remove"

#define EVENT_DB_SAVING "db_saving"
#define EVENT_NEWNICK   "newnick"
#define EVENT_CONNECT "connect"
#define EVENT_DB_EXPIRE "db_expire"
#define EVENT_RESTART "restart"
#define EVENT_SHUTDOWN "shutdown"
#define EVENT_SIGNAL "signal"
#define EVENT_CHANGE_NICK "change_nick"
#define EVENT_USER_LOGOFF "user_logoff"
#define EVENT_CTCP_VERSION "ctcp_version"
#define EVENT_SERVER "new_server"
#define EVENT_JUPED_SERVER "juped_server"
#define EVENT_SQUIT	 "server_squit"
#define EVENT_FANTASY "fantasy_command"
#define EVENT_DB_BACKUP "db_backup"
#define EVENT_USER_JOIN "user_join"
#define EVENT_USER_PART "user_part"
#define EVENT_CHAN_PRIVMSG "chan_privmsg"
#define EVENT_SENT_CTCP_VERSION "ctcp_sent_version"
#define EVENT_CHAN_KICK "chan_kick"
#define EVENT_USER_MODE "user_mode"
#define EVENT_CHANNEL_MODE "channel_mode"
#define EVENT_CHANNEL_TOPIC "channel_topic"
#define EVENT_GLOBAL_KILL "global_kill"
#define EVENT_SERVER_KILL "server_kill"
#define EVENT_RELOAD "event_reload"
#define EVENT_DELETE_USER "event_delete_user"
#define EVENT_POST_MODE "event_post_mode"
#define EVENT_UPLINK_SYNC_COMPLETE "uplink_sync_complete"
#define EVENT_SERVER_SYNC_COMPLETE "server_sync_complete"


