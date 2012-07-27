/*
 * unreal_kline module for Denora 1.2+
 * (c) Trystan
 */

#include "denora.h"
#define AUTHOR "Trystan"
#define VERSION "1.1"
#define MYNAME "unreal_kline"

int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int my_server(int argc, char **argv);
int get_kline(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv)
{
    EvtHook *hook = NULL;
    Message *msg = NULL;
    int status;

	USE_VAR(argc);
	USE_VAR(argv);

    hook = createEventHook(EVENT_SERVER, my_server);
    status = moduleAddEventHook(hook);
    if (status != MOD_ERR_OK) {
        /* something went wrong say something about */
        alog(LOG_NORMAL, "[%s%s] unable to bind to EVENT_SERVER error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
        return MOD_STOP;
    }

    if (denora_get_ircd() == IRC_UNREAL32) {
        msg = createMessage("223", get_kline);
        status = moduleAddMessage(msg, MOD_HEAD);
        if (status != MOD_ERR_OK) {
            /* something went wrong say something about */
            alog(LOG_NORMAL, "[%s%s] unable to bind to 223 error [%d][%s]", MYNAME, MODULE_EXT, status, ModuleGetErrStr(status));
            return MOD_STOP;
        }
    } else {
        return MOD_STOP;
    }

    moduleAddAuthor(AUTHOR);
    moduleAddVersion(VERSION);
    return MOD_CONT;
}

void DenoraFini(void)
{
    alog(LOG_NORMAL, "Unloading ircd_port%s", MODULE_EXT);
}

int my_server(int argc, char **argv)
{
    if (argc >= 1) {
        if (stricmp(ServerName, argv[0])) {
            denora_cmd_stats(s_StatServ, "k", argv[0]);
        }
    }
    return MOD_CONT;
}

/*
[Jan 10 03:19:45.232386 2006] debug: Received: :Luna.NomadIrc.Net 223 Denora K *@*.someisp.com 164370 8430 Trystan!tslee@NetAdmin.Nomadirc.net :Abuse (Adds a K:line for 2 days)
[Jan 10 03:19:45.232473 2006] Source Luna.NomadIrc.Net
[Jan 10 03:19:45.232528 2006] av[0] = Denora
[Jan 10 03:19:45.232583 2006] av[1] = K
[Jan 10 03:19:45.232638 2006] av[2] = *@*.someisp.com
[Jan 10 03:19:45.232693 2006] av[3] = 164370
[Jan 10 03:19:45.232749 2006] av[4] = 8430
[Jan 10 03:19:45.232804 2006] av[5] = Trystan!tslee@NetAdmin.Nomadirc.net
[Jan 10 03:19:45.232860 2006] av[6] = Abuse (Adds a K:line for 2 days)
*/

int get_kline(char *source, int ac, char **av)
{
    char *user, *host;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

        if (denora->protocoldebug)
        {
                protocol_debug(source, ac, av);
        }

    if (!strcmp(av[1], "K")) {
      user = myStrGetToken(av[2], '@', 0);
      host = myStrGetToken(av[2], '@', 1);
      if (ac == 7) {
       ircsnprintf(buf, BUFSIZE, "%ld", time(NULL) + atoi(av[3]));
       ircsnprintf(buf2, BUFSIZE, "%ld", time(NULL) - atoi(av[4]));
       sql_do_server_bans_add(av[1], user, host, av[5], buf, buf2, av[6]);
      } else {
       sql_do_server_bans_add(av[1], user, host, av[5], (char *) "0", (char *) "0", av[6]);
      }
      if (user) { free(user); }
      if (host) { free(host); }
    }
    return MOD_CONT;
}
