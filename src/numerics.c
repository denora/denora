
/*
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

#include "denora.h"

/*************************************************************************/

/**
 * Process numeric 372 messages
 *
 * @param source is the person or server that send the request
 * @param msg is the message going back to the user
 * @return void - no returend value
 *
 */
void denora_cmd_372(char *source, char *msg)
{
    SET_SEGV_LOCATION();

    if (ircd->numerics) {
        denora_cmd_numeric(source, 372, ":- %s", msg);
    }
}

/*************************************************************************/

/**
 * Process numeric 422 messages
 *
 * @param source is the person or server that send the request
 * @return void - no returend value
 *
 */
void denora_cmd_422(char *source)
{
    SET_SEGV_LOCATION();

    if (ircd->numerics) {
        denora_cmd_numeric(source, 422, ":MOTD file not found!");
    }
}

/*************************************************************************/

/**
 * Process numeric 351 messages
 *
 * @param source is the person or server that send the request
 * @return void - no returend value
 *
 */
void denora_cmd_351(char *source)
{
    SET_SEGV_LOCATION();

    if (ircd->numerics) {
        denora_cmd_numeric(source, 351,
                           "Denora-%s %s :%s -- build #%s, compiled %s %s",
                           denora->version, ServerName, ircd->name,
                           denora->build, denora->date, denora->time);
    }
}

/*************************************************************************/

/**
 * Process numeric 375 messages
 *
 * @param source is the person or server that send the request
 * @return void - no returend value
 *
 */
void denora_cmd_375(char *source)
{
    SET_SEGV_LOCATION();

    if (ircd->numerics) {
        denora_cmd_numeric(source, 375, ":- %s Message of the Day",
                           ServerName);
    }
}

/*************************************************************************/

/**
 * Process numeric 376 messages
 *
 * @param source is the person or server that send the request
 * @return void - no returend value
 *
 */
void denora_cmd_376(char *source)
{
    SET_SEGV_LOCATION();

    if (ircd->numerics) {
        denora_cmd_numeric(source, 376, ":End of /MOTD command.");
    }
}

/*************************************************************************/

/**
 * Process numeric 391 messages
 *
 * @param source is the person or server that send the request
 * @param timestr is the formatted time reply
 * @return void - no returend value
 *
 */
void denora_cmd_391(char *source, char *timestr)
{
    SET_SEGV_LOCATION();

    if (!timestr) {
        return;
    }
    if (ircd->numerics) {
        denora_cmd_numeric(source, 391, "%s :%s", ServerName, timestr);
    }
}

/*************************************************************************/

/**
 * Process numeric 219 messages
 *
 * @param source is the person or server that send the request
 * @param letter is the stats letter that was requested
 * @return void - no returend value
 *
 */
void denora_cmd_219(char *source, char *letter)
{
    SET_SEGV_LOCATION();

    if (!source) {
        return;
    }

    if (ircd->numerics) {
        if (letter) {
            denora_cmd_numeric(source, 219, "%c :End of /STATS report.",
                               *letter);
        } else {
            denora_cmd_numeric(source, 219, "l :End of /STATS report.");
        }
    }
}

/*************************************************************************/

/**
 * Process numeric 401 messages
 *
 * @param source is the person or server that send the request
 * @param who is whois requested person
 * @return void - no returend value
 *
 */
void denora_cmd_401(char *source, char *who)
{
    SET_SEGV_LOCATION();

    if (!source || !who) {
        return;
    }
    if (ircd->numerics) {
        denora_cmd_numeric(source, 401, "%s :No such service.", who);
    }
}

/*************************************************************************/

/**
 * Process numeric 318 messages
 *
 * @param source is the person or server that send the request
 * @param who is whois requested person
 * @return void - no returend value
 *
 */
void denora_cmd_318(char *source, char *who)
{
    SET_SEGV_LOCATION();

    if (!source || !who) {
        return;
    }
    if (ircd->numerics) {
        denora_cmd_numeric(source, 318, "%s :End of /WHOIS list.", who);
    }
}

/*************************************************************************/

/**
 * Generic way of send out numeric messages
 *
 * @param numeric is the numeric to send
 * @param fmt is the message that will be sent
 * @param ... mixed arguments
 *
 * @return void - no returend value
 *
 */
void denora_cmd_numeric(char *source, int numeric, const char *fmt, ...)
{
    va_list args;
    User *u;
    char buf[BUFSIZE];
    *buf = '\0';

    SET_SEGV_LOCATION();

    if (fmt) {
        va_start(args, fmt);
        ircvsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }

    u = finduser(source);

    if (ircd->numerics) {
        if (ircd->p10) {
            send_cmd(p10id, "%d %s %s", numeric, (u ? u->uid : source),
                     buf);
        } else if (ircd->ts6 && UseTS6) {
            send_cmd((TS6SID ? TS6SID : ServerName), "%d %s %s", numeric,
                     (UseTS6 ? (u ? u->uid : source) : source), buf);
        } else {
            send_cmd(ServerName, "%d %s %s", numeric,
                     (u ? u->nick : source), buf);
        }
    }
}

/*************************************************************************/
