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
 * $Id: example.c 578 2006-03-11 11:11:50Z trystan $
 *
 */

#include "denora.h"
#define AUTHOR "Trystan"
#define VERSION "1.0"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);

int DenoraInit(int argc, char **argv)
{
    if (denora->debug) {
        protocol_debug(NULL, argc, argv);
    }

    if (denora_get_ircd() == IRC_SOLIDIRCD) {
        alog(LOG_NORMAL, "IRCD is SolidIRCD");
    } else if (denora_get_ircd() == IRC_UNREAL32) {
        alog(LOG_NORMAL, "IRCD is Unreal 32");
    } else {
        alog(LOG_NORMAL, "IRCD is something else see ircd.h");
        return MOD_STOP;
    }

    if (denora_umode(UMODE_F)) {
        alog(LOG_NORMAL, "IRCD supports this umode");
    } else {
        alog(LOG_NORMAL, "IRCD does not support this umode");
    }

    if (denora_cmode(CMODE_F)) {
        alog(LOG_NORMAL, "IRCD supports this channel mode");
    } else {
        alog(LOG_NORMAL, "IRCD does not support this channel mode");
    }


    moduleAddAuthor(AUTHOR);
    moduleAddVersion(VERSION);
    return MOD_CONT;
}

void DenoraFini(void)
{
    alog(LOG_NORMAL, "Unloading example.%s", MODULE_EXT);
}
