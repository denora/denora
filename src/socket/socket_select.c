#if defined(POLLER_SELECT)
fd_set select_rfds, select_wfds;

int DenoraInit(int argc, char **argv)
{
	if (denora->protocoldebug)
	{
		protocol_debug(NULL, argc, argv);
	}
	/* Only 1 protocol module may be loaded */
	if (SocketModuleLoaded())
	{
		alog(LOG_NORMAL, langstr(ALOG_MOD_BE_ONLY_ONE));
		return MOD_STOP;
	}

	moduleAddAuthor("Denora");
	moduleAddVersion("2.0");
	moduleSetType(SOCKET);

    FD_ZERO(&select_rfds);
    FD_ZERO(&select_wfds);


	return MOD_CONT;
}

/* these two functions tell the polling system to either watch or stop watching
 * for specific events on the socket.  The available events are currently only
 * reading or writing (error conditions are always monitored).  It is necessary
 * to set a hook on the socket's data event and to add some kind of monitoring
 * to the socket in order to get hooks called for socket data events. */
void socket_monitor(isocket_t *sock, int mask) 
{
    /* If this wasn't an internal call set the want conditions so we know what
     * kind of monitoring the user wants. */
    if (!(mask & SOCKET_FL_INTERNAL)) {
        if (mask & SOCKET_FL_READ)
            sock->state |= SOCKET_FL_WANT_READ;
        if (mask & SOCKET_FL_WRITE)
            sock->state |= SOCKET_FL_WANT_WRITE;
    }

    if (mask & SOCKET_FL_READ)
        FD_SET(sock->fd, &select_rfds);
    if (mask & SOCKET_FL_WRITE)
        FD_SET(sock->fd, &select_wfds);
}


void socket_unmonitor(isocket_t *sock, int mask) 
{
    /* If this wasn't an internal call unset the want conditions so we know
     * what kind of monitoring the user no longer wants */
    if (!(mask & SOCKET_FL_INTERNAL)) {
        if (mask & SOCKET_FL_READ)
            sock->state &= ~SOCKET_FL_WANT_READ;
        if (mask & SOCKET_FL_WRITE)
            sock->state &= ~SOCKET_FL_WANT_WRITE;
    }

    if (mask & SOCKET_FL_READ)
        FD_CLR(sock->fd, &select_rfds);
    if (mask & SOCKET_FL_WRITE)
        FD_CLR(sock->fd, &select_wfds);
}
