/*
 *
 * (c) 2004-2013 Denora Team
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

static int socket_setflags(int fd);
static inline void socket_event(isocket_t *);

unsigned int maxsockets = 1024; /* default is for 1024 sockets maximum */
unsigned int cursockets = 0; /* current number of sockets open */
struct isocket_list allsockets;

/* hints structure.  pretty meager, except that we assume we're going to
 * listen() by default */
struct addrinfo gai_hint;


void init_socketsystem(void) 
{

    gai_hint.ai_flags = AI_PASSIVE;
    gai_hint.ai_family = PF_UNSPEC;

	Socket_module_init();
}


/* This functions destroys the passed socket structure, and closes it if
 * necessary.  It actually only marks the socket as dead so that it can be
 * safely reaped outside of a polling cycle. */
int destroy_socket(isocket_t *sock) 
{
    if (sock == NULL)
        return 0;

    /* we only close the socket and mark it as dead, we reap dead sockets after
     * the polling phase.  see reap_dead_sockets() */
    close_socket(sock);
    sock->state |= SOCKET_FL_DEAD;

    return 1; /* always successful */
}

/* This function is a light wrapper for the system listen() call, and should be
 * used only after a socket is bound and opened. */
int socket_listen(isocket_t *sock) 
{
    if (sock == NULL || !(sock->state & SOCKET_FL_OPEN))
        return 0;

    if (listen(sock->fd, 128)) {
        alog(LOG_ERROR, "listen() failed: %s", strerror(errno));
        sock->err = errno;
        return 0;
    }

    sock->state |= SOCKET_FL_LISTENING;
    return 1;
}



/* This function is used in conjunction with listening sockets to accept new
 * connections.  If a connection is available on the socket, a new socket
 * structure is allocated and filled in and passed back to the caller.  This
 * function may be called repeatedly to exhaust the list of available incoming
 * connections on a socket. */
isocket_t *socket_accept(isocket_t *sock) 
{
    isocket_t *s;
    size_t alen = 0;
    int fd = -1;
    struct sockaddr *sa;

    if (sock == NULL || !(sock->state & SOCKET_FL_LISTENING))
        return NULL;

    sa = malloc(sock->sockaddr.addrlen); 
    if (cursockets >= maxsockets) {
        alog(LOG_DEBUG, "attempt to accept() a connection when cursockets >= maxsockets");
        /* try and accept the connection anyhow, and close it.  it might be
         * worthwhile to stop listening on this til cursockets drops down below
         * maxsockets.  Let's remember that for later. ;) */
        while ((fd = accept(sock->fd, sa, &alen)) > -1)
            close(fd); /* heh.. */
        free(sa);
        return NULL;
    }

    /* allocate space for our sockaddr. */
    alen = sock->sockaddr.addrlen;
    if ((fd = accept(sock->fd, sa, &alen)) < 0) {
        if (errno != EWOULDBLOCK) {
            alog(LOG_ERROR, "accept() failed: %s", strerror(errno));
            sock->err = errno;
        }
        free(sa);
        return NULL;
    }
    s = create_socket();
    s->fd = fd;
    s->state |= SOCKET_FL_OPEN;
    if (!socket_setflags(s->fd)) {
        destroy_socket(s);
        free(sa);
        return NULL;
    }
    s->state |= SOCKET_FL_CONNECTED;
    cursockets++;
        
    /* fill in our peer address. */
    s->peeraddr.addr = sa;
    s->peeraddr.addrlen = alen;
    s->peeraddr.family = sock->sockaddr.family;
    s->peeraddr.type = sock->sockaddr.type;
    s->peeraddr.protocol = sock->sockaddr.protocol;
    /* fill in our local address, too. */
    sa = malloc(alen);
    if (getsockname(s->fd, sa, &alen)) {
        alog(LOG_ERROR, "getsockname(%d) failed: %s", s->fd, strerror(errno));
        s->err = errno;
    }
    s->sockaddr.addr = sa;
    s->sockaddr.addrlen = alen;
    s->sockaddr.family = sock->sockaddr.family;
    s->sockaddr.type = sock->sockaddr.type;
    s->sockaddr.protocol = sock->sockaddr.protocol;

    return s;
}


/* connect to the specified address (requires a previously created socket),
 * it is recommended the address be in  form that does not require lookups,
 * since the address lookups here are not non-blocking. */
int socket_connect(isocket_t *sock, char *host, char *port, int type) {

    if (sock == NULL || sock->state & SOCKET_FL_CONNECTED)
        return 0;

    /* use gai_hint from set_socket_address. */
    gai_hint.ai_flags = 0;
    if (!set_socket_address(&sock->peeraddr, host, port, type)) 
	{
        alog(LOG_ERROR, "socket_connect(%s, %s): failed to get remote address", host, port);
        return 0;
    }
    gai_hint.ai_flags = AI_PASSIVE; /* re-set this for regular calls. */

    if (connect(sock->fd, sock->peeraddr.addr, sock->peeraddr.addrlen) &&
            errno != EINPROGRESS) {
        /* ECONNREFUSED is not a very interesting error, but others might be.
         * log ECONNREFUSED as debug. */
        alog((errno != ECONNREFUSED ? LOG_ERROR : LOG_DEBUG), "connect() failed: %s", strerror(errno));
        sock->err = errno;
        return 0;
    }

    sock->state |= SOCKET_FL_CONNECTED;
    return 1;
}



/* These two functions are used to read data from, and write data to a socket,
 * respectively.  They return the amount of data actually read, and should be
 * passed a buffer with enough space to hold nbytes of data.  No more than
 * 'nbytes' will be read.  Since sockets are non-blocking, any 'try again'
 * conditions are caught by the functions, which will return 0 if no data could
 * be sent or no data is availble. */
int ssl_socket_read(isocket_t *sock, char *buf, size_t nbytes) {
    int ret;

    if (!(sock->state & SOCKET_FL_OPEN))
        return -1;

    /* in case something stupid happens ;) */
    assert(nbytes > 0 && nbytes < INT_MAX);

    errno = 0;
#ifdef HAVE_RECV
        ret = recv(sock->fd, buf, nbytes, SOCKET_RECV_FLAGS);
#else
        ret = read(sock->fd, buf, nbytes);
#endif
    if (ret < 0) {
        switch (errno) {
            case EAGAIN:
            case EINTR:
                return 0; /* ignore this */
            default:
                sock->state |= SOCKET_FL_ERROR_PENDING;
                sock->err = errno;
                return -1;
        }
    } else if (ret == 0) {
        /* EOF received (usually (always?) errno will not be set) */
        sock->state |= SOCKET_FL_ERROR_PENDING;
        if (errno != 0)
            sock->err = errno;
        else
            sock->state |= SOCKET_FL_EOF;
        return -1;
    }

    return ret;
}


int socket_write(isocket_t *sock, char *buf, size_t nbytes) 
{
    int ret;

    if (!(sock->state & SOCKET_FL_OPEN))
        return -1;

    assert(nbytes > 0 && nbytes < INT_MAX);

    errno = 0;
#ifdef HAVE_SEND
        ret = send(sock->fd, buf, nbytes, SOCKET_SEND_FLAGS);
#else
        ret = write(sock->fd, buf, nbytes);
#endif

    if (ret == -1) {
        switch (errno) {
            case EAGAIN:
            case EINTR:
                sock->state &= ~SOCKET_FL_WRITE_PENDING;
                if (sock->state & SOCKET_FL_WANT_WRITE)
                    socket_monitor(sock, SOCKET_FL_WRITE);
                return 0;
            default:
                sock->state |= SOCKET_FL_ERROR_PENDING;
                sock->err = errno;
                return -1;
        }
    }

    if ((size_t)ret != nbytes && sock->state & SOCKET_FL_WANT_WRITE) {
        sock->state &= ~SOCKET_FL_WRITE_PENDING;
        socket_monitor(sock, SOCKET_FL_WRITE);
    }
    return ret;
}


/* a little function which will return the error condition of a socket.  Unless
 * the socket has an internal error (currently just EOF) we simply return
 * strerror on the socket's errno value */
const char *socket_strerror(isocket_t *sock) {

    if (sock->state & SOCKET_FL_EOF)
        return "Received EOF from peer";
    else
        return strerror(sock->err);
}

/* this sets a socket as non-blocking, and possibly sets some other useful
 * options.  currently we only support fcntl() for doing this. */
static int socket_setflags(int fd) 
{
    int opt;

#ifdef HAVE_SETSOCKOPT
# ifdef SO_REUSEADDR
    opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
# endif
#endif
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
        alog(LOG_ERROR, "fcntl(O_NONBLOCK) failed: %s", strerror(errno));
        return 0;
    }

    return 1;
}

/* while this function may at first seem unnecessary, it is actually very
 * necessary.  we cannot safely destroy sockets in the polling loop, because
 * some event systems (namely kqueue for the present) generate two events for
 * sockets, and when one succeeds and causes a closure, there may still be an
 * event for the other which ends up getting called when it shouldn't be.
 * Also, when OpenSSL support is enabled this function performs SSL handshake
 * timeouts. */
void reap_dead_sockets(void) 
{
    isocket_t *sp, *sp2;

    sp = LIST_FIRST(&allsockets);

    while (sp != NULL) {
        sp2 = LIST_NEXT(sp, intlp);

        /* if it's dead, clear it away. */
        if (SOCKET_DEAD(sp)) {
            if (sp->sockaddr.addr != NULL)
                free(sp->sockaddr.addr);
            if (sp->peeraddr.addr != NULL)
                free(sp->peeraddr.addr);
            destroy_event(sp->datahook);
            LIST_REMOVE(sp, intlp);
            free(sp);
        }
        sp = sp2;
    }
}


/* Function to determine the type of an address.  We do cheap best effort
 * here.  If it looks IPv6-y we try inet_pton with AF_INET6, if it looks
 * IPv4-y we try inet_pton with AF_INET, and as long as one succeeds we
 * return that type. */
int get_address_type(const char *addr) {
    const char *s = addr;
    unsigned char buf[IPADDR_SIZE];

#ifdef INET6
    s = strchr(addr, ':');
    if (s != NULL && strchr(s + 1, ':') != NULL) {
        /* either IPv6 or nothing, let's see */
        if (inet_pton(PF_INET6, addr, buf) == 1)
            return PF_INET6;
        return PF_UNSPEC;
    }
#endif

    if (inet_pton(PF_INET, addr, buf) == 1)
        return PF_INET;
    return PF_UNSPEC;
}


/* this creates a new socket structure, making it completely empty.  the socket
 * must then be bound and opened, using the functions below. */
isocket_t *create_socket(void) {
    isocket_t *sock = NULL;

    sock = malloc(sizeof(isocket_t));
    /* only one hook allowed per socket */
    sock->datahook = create_event(EVENT_FL_ONEHOOK|EVENT_FL_NORETURN);
    sock->state = sock->err = 0;
    sock->udata = NULL; /* only place udata is ever touched */
    sock->sockaddr.addr = sock->peeraddr.addr = NULL;
    sock->sockaddr.family = sock->peeraddr.family = PF_UNSPEC;
    sock->fd = -1;

    LIST_INSERT_HEAD(&allsockets, sock, intlp);
    return sock;
}



/* This function 'opens' a socket.  This means that it binds the socket to an
 * address, allocates an fd for use, and sets the socket nonblocking. */
int open_socket(isocket_t *sock) {
    if (sock == NULL || sock->sockaddr.addr == NULL ||
            sock->state & SOCKET_FL_OPEN)
        return 0;

    if (cursockets >= maxsockets) {
        log_debug("attempt to open new fd denied, cursockets >= maxsockets");
        return 0;
    }
    sock->fd = socket(sock->sockaddr.family, sock->sockaddr.type,
            sock->sockaddr.protocol);
    if (sock->fd == -1) {
        log_error("socket() failed: %s", strerror(errno));
        sock->err = errno;
        return 0;
    }
        
    if (!socket_setflags(sock->fd)) {
        close(sock->fd);
        return 0;
    }

    if (bind(sock->fd, sock->sockaddr.addr, sock->sockaddr.addrlen)) {
        char host[FQDN_MAXLEN];
        int port;
        get_socket_address(isock_laddr(sock), host, FQDN_MAXLEN, &port);
        log_error("bind(%s/%d) failed: %s", host, port, strerror(errno));
        sock->err = errno;
        return 0;
    }

    sock->state |= SOCKET_FL_OPEN;
    cursockets++;
    return 1;
}

/* This function closes a socket, and removes any monitoring that may be set on
 * the socket. */
int close_socket(isocket_t *sock) {
    if (sock == NULL || !(sock->state & SOCKET_FL_OPEN))
        return 0;

    if (close(sock->fd)) {
        log_error("close() failed: %s", strerror(errno));
        sock->err = errno;
        return 0;
    }

    sock->state &= ~SOCKET_FL_OPEN;
    cursockets--;

    /* any events associated with this socket will be deleted automagically
     * when it is no longer valid at the next call to kevent, so don't bother
     * doing it */
#ifndef POLLER_KQUEUE
    socket_unmonitor(sock, SOCKET_FL_PENDING); /* turn it all off */
#endif

    sock->fd = -1;
    return 1;
}

/* This is a wrapper around getaddrinfo which we use to set various bits for
 * our socket.  This is a one-off call!  If you call it again, the previous
 * data will be blown away.  We only use the first item returned from
 * getaddrinfo, and there should probably be a way to set preferences for this.
 * Oh well. */
int set_socket_address(struct isock_address *addr, char *host, char *port,
        int type) {
    struct addrinfo *ai;
    int error;

    if (addr == NULL)
        return 0;

    if (addr->addr != NULL) {
        /* blow away old data */
        free(addr->addr);
    }

    gai_hint.ai_socktype = type;
    if ((error = getaddrinfo(host, port, &gai_hint, &ai))) {
        log_warn("getaddrinfo(%s, %s): %s", host, port, gai_strerror(error));
        return 0;
    }

    if (addr->addr != NULL)
        free(addr->addr);
    /* copy over necessary stuff.  allocate space for the sockaddr, too. */
    addr->addr = malloc(ai->ai_addrlen);
    memcpy(addr->addr, ai->ai_addr, ai->ai_addrlen);
    addr->addrlen = ai->ai_addrlen;
    addr->family = ai->ai_family;
    addr->type = ai->ai_socktype;
    addr->protocol = ai->ai_protocol;

    freeaddrinfo(ai);
    return 1;
}

/* This function gets the address data from an 'isock_address' structure and
 * returns it in the form of a host (never looked up) and an integer port. */
int get_socket_address(struct isock_address *addr, char *host, size_t hlen,
        int *port) {
    char shost[NI_MAXHOST + 1];
    char sport[NI_MAXSERV + 1];
    int error;

    if (addr == NULL)
        return 0;

    if ((error = getnameinfo(addr->addr, (socklen_t)addr->addrlen,
                    shost, NI_MAXHOST, sport, NI_MAXSERV,
                    NI_NUMERICHOST|NI_NUMERICSERV))) {
        log_warn("getnameinfo(): %s", gai_strerror(error));
        return 0;
    }

    if (host != NULL)
        strlcpy(host, shost, hlen);
    if (port != NULL)
        *port = str_conv_int(sport, 0);
    return 1;
}


/* this function is called by the different pollers on sockets with pending
 * data.  because of SSL it is no longer as simple as activating a hook on a
 * socket. */
static inline void socket_event(isocket_t *isp) 
{

    /* If we haven't done anything else, this is all we need to do. */
    hook_event(isp->datahook, isp);
    isp->state &= ~SOCKET_FL_PENDING;
}


/*************************************************************************/

/**
 * Load the ircd protocol module up
 **/
int Socket_module_init(void)
{
	int ret = 0;
	Module *m;

	if (BadPtr(SocketModule))
	{
		alog(LOG_NORMAL, "Error: SocketModule is incorrectly defined");
		return ret;
	}

	m = createModule(SocketModule);
	mod_current_module = m;
	mod_current_user = NULL;
	moduleSetType(SOCKET);
	ret = loadModule(mod_current_module, NULL);
	if (ret != MOD_ERR_OK)
	{
		alog(LOG_NORMAL, "Error Status [%d]", ret);
		exit(1);
	}
	alog(LOG_NORMAL, "Loading Socket Module: [%s]",
	     mod_current_module->name);
	alog(LOG_NORMAL, "Author: [%s]",
	     mod_current_module->author);
	alog(LOG_NORMAL, "Version: [%s]",
	     mod_current_module->version);
	alog(LOG_EXTRADEBUG, "debug: status: [%d]", ret);
	mod_current_module = NULL;
	return ret;
}