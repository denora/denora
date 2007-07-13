
/*
 *
 * (C) 2004-2007 Denora Team
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

#include "denora.h"
#include "sockets.h"

/*************************************************************************/

static char read_netbuf[NET_BUFSIZE];
static char *read_curpos = read_netbuf; /* Next byte to return */
static char *read_bufend = read_netbuf; /* Next position for data from socket */
static char *const read_buftop = read_netbuf + NET_BUFSIZE;
int32 total_read = 0;
static char write_netbuf[NET_BUFSIZE];
static char *write_curpos = write_netbuf;       /* Next byte to write to socket */
static char *write_bufend = write_netbuf;       /* Next position for data to socket */
static char *const write_buftop = write_netbuf + NET_BUFSIZE;
static deno_socket_t write_fd = -1;
int32 total_written;
int32 total_sendmsg;
int32 total_recmsg;
int sungetc(int c);
static int lastchar = EOF;

/*************************************************************************/

/**
 * Return amount of data in read buffer.
 * @return int32
 */
int32 read_buffer_len()
{
    SET_SEGV_LOCATION();

    if (read_bufend >= read_curpos) {
        return read_bufend - read_curpos;
    } else {
        return (read_bufend + NET_BUFSIZE) - read_curpos;
    }
}

/*************************************************************************/

/**
 * Read data.
 * @param fd File Pointer
 * @param buf Buffer
 * @param len is an integer Length of buffer
 * @return int
 */
static int buffered_read(deno_socket_t fd, char *buf, int len)
{
    int nread, left = len;
    fd_set fds;
    struct timeval tv = { 0, 0 };
    int errno_save = deno_sockgeterr();

    SET_SEGV_LOCATION();

    if (fd < 0) {
        deno_sockseterr(SOCKERR_EBADF);
        return -1;
    }
    while (left > 0) {
        struct timeval *tvptr = (read_bufend == read_curpos ? NULL : &tv);
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        while (read_bufend != read_curpos - 1
               && !(read_curpos == read_netbuf
                    && read_bufend == read_buftop - 1)
               && select(fd + 1, &fds, 0, 0, tvptr) == 1) {
            int maxread;
            tvptr = &tv;        /* don't wait next time */
            if (read_bufend < read_curpos)      /* wrapped around? */
                maxread = (read_curpos - 1) - read_bufend;
            else if (read_curpos == read_netbuf)
                maxread = read_buftop - read_bufend - 1;
            else
                maxread = read_buftop - read_bufend;
            nread = deno_sockread(fd, read_bufend, maxread);
            errno_save = deno_sockgeterr();
            alog(LOG_DEBUGSOCK, "debug: buffered_read wanted %d, got %d",
                 maxread, nread);
            if (nread <= 0)
                break;
            read_bufend += nread;
            if (read_bufend == read_buftop)
                read_bufend = read_netbuf;
        }
        if (read_curpos == read_bufend) /* No more data on socket */
            break;
        /* See if we can gobble up the rest of the buffer. */
        if (read_curpos + left >= read_buftop && read_bufend < read_curpos) {
            nread = read_buftop - read_curpos;
            memcpy(buf, read_curpos, nread);
            buf += nread;
            left -= nread;
            read_curpos = read_netbuf;
        }
        /* Now everything we need is in a single chunk at read_curpos. */
        if (read_bufend > read_curpos && read_bufend - read_curpos < left)
            nread = read_bufend - read_curpos;
        else
            nread = left;
        if (nread) {
            memcpy(buf, read_curpos, nread);
            buf += nread;
            left -= nread;
            read_curpos += nread;
        }
    }
    total_read += len - left;
    alog(LOG_DEBUGSOCK, "debug: buffered_read(%d,%p,%d) returning %d",
         fd, buf, len, len - left);
    deno_sockseterr(errno_save);
    return len - left;
}

/*************************************************************************/

/**
 * Optimized version of the above for reading a single character; returns
 * the character in an int or EOF, like fgetc().
 * @param fd File Pointer
 * @return int
 */
static int buffered_read_one(deno_socket_t fd)
{
    int nread;
    fd_set fds;
    struct timeval tv = { 0, 0 };
    char c;
    struct timeval *tvptr = (read_bufend == read_curpos ? NULL : &tv);
    int errno_save = deno_sockgeterr();

    if (fd < 0) {
        deno_sockseterr(SOCKERR_EBADF);
        return -1;
    }
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    while (read_bufend != read_curpos - 1
           && !(read_curpos == read_netbuf
                && read_bufend == read_buftop - 1)
           && select(fd + 1, &fds, 0, 0, tvptr) == 1) {
        int maxread;
        tvptr = &tv;            /* don't wait next time */
        if (read_bufend < read_curpos)  /* wrapped around? */
            maxread = (read_curpos - 1) - read_bufend;
        else if (read_curpos == read_netbuf)
            maxread = read_buftop - read_bufend - 1;
        else
            maxread = read_buftop - read_bufend;
        nread = deno_sockread(fd, read_bufend, maxread);
        errno_save = deno_sockgeterr();
        alog(LOG_DEBUGSOCK, "debug: buffered_read_one wanted %d, got %d",
             maxread, nread);
        if (nread <= 0)
            break;
        read_bufend += nread;
        if (read_bufend == read_buftop)
            read_bufend = read_netbuf;
    }
    if (read_curpos == read_bufend) {   /* No more data on socket */
        alog(LOG_DEBUGSOCK, "debug: buffered_read_one(%d) returning %d",
             fd, EOF);
        deno_sockseterr(errno_save);
        return EOF;
    }
    c = *read_curpos++;
    if (read_curpos == read_buftop)
        read_curpos = read_netbuf;
    total_read++;
    alog(LOG_DEBUGSOCK, "debug: buffered_read_one(%d) returning %d", fd,
         c);
    return (int) c & 0xFF;
}

/*************************************************************************/

/**
 * Return amount of data in write buffer.
 * @return int
 */
int32 write_buffer_len()
{
    if (write_bufend >= write_curpos) {
        return write_bufend - write_curpos;
    } else {
        return (write_bufend + NET_BUFSIZE) - write_curpos;
    }
}

/*************************************************************************/

/**
 * Helper routine to try and write up to one chunk of data from the buffer
 * to the socket.  Return how much was written.
 * @param wait Wait
 * @return int
 */
static int flush_write_buffer(int wait)
{
    fd_set fds;
    struct timeval tv = { 0, 0 };
#ifndef _WIN32
    void (*oldsig) (int);
#endif

    int errno_save = deno_sockgeterr();

    if (write_bufend == write_curpos || write_fd == -1)
        return 0;
    FD_ZERO(&fds);
    FD_SET(write_fd, &fds);
    if (select(write_fd + 1, 0, &fds, 0, wait ? NULL : &tv) == 1) {
        int maxwrite, nwritten;
        if (write_curpos > write_bufend) {      /* wrapped around? */
            maxwrite = write_buftop - write_curpos;
        } else if (write_bufend == write_netbuf) {
            maxwrite = write_buftop - write_curpos - 1;
        } else {
            maxwrite = write_bufend - write_curpos;
        }
#ifndef _WIN32
        oldsig = signal(SIGPIPE, SIG_IGN);
        if (oldsig == SIG_ERR) {
            alog(LOG_DEBUG,
                 "debug: Warning: Couldn't set SIGPIPE signal to be ignored");
        }
#endif
        nwritten = deno_sockwrite(write_fd, write_curpos, maxwrite);
#ifndef _WIN32
        if (signal(SIGPIPE, oldsig) == SIG_ERR) {
            alog(LOG_DEBUG,
                 "debug: Warning: Couldn't reset SIGPIPE signal to previous value");
        }
#endif
        errno_save = deno_sockgeterr();
        alog(LOG_DEBUGSOCK, "debug: flush_write_buffer wanted %d, got %d",
             maxwrite, nwritten);
        if (nwritten > 0) {
            write_curpos += nwritten;
            if (write_curpos == write_buftop)
                write_curpos = write_netbuf;
            total_written += nwritten;
            return nwritten;
        }
    }
    deno_sockseterr(errno_save);
    return 0;
}

/*************************************************************************/

/**
 * Write data.
 * @param fd File Pointer
 * @param buf Buffer to write
 * @param len Length to write
 * @return int
 */
int buffered_write(deno_socket_t fd, char *buf, int len)
{
    int nwritten, left = len;
    int errno_save = deno_sockgeterr();

    if (fd < 0) {
        deno_sockseterr(SOCKERR_EBADF);
        return -1;
    }
    write_fd = fd;

    if (BadPtr(buf)) {
        return len;
    }

    while (left > 0) {

        /* Don't try putting anything in the buffer if it's full. */
        if (write_curpos != write_bufend + 1 &&
            (write_curpos != write_netbuf
             || write_bufend != write_buftop - 1)) {
            /* See if we need to write up to the end of the buffer. */
            if (write_bufend + left >= write_buftop
                && write_curpos <= write_bufend) {
                nwritten = write_buftop - write_bufend;
                memcpy(write_bufend, buf, nwritten);
                buf += nwritten;
                left -= nwritten;
                write_bufend = write_netbuf;
            }
            /* Now we can copy a single chunk to write_bufend. */
            if (write_curpos > write_bufend
                && write_curpos - write_bufend - 1 < left)
                nwritten = write_curpos - write_bufend - 1;
            else
                nwritten = left;
            if (nwritten) {
                memcpy(write_bufend, buf, nwritten);
                buf += nwritten;
                left -= nwritten;
                write_bufend += nwritten;
            }
        }

        /* Now write to the socket as much as we can. */
        if (write_curpos == write_bufend + 1 ||
            (write_curpos == write_netbuf
             && write_bufend == write_buftop - 1))
            flush_write_buffer(1);
        else
            flush_write_buffer(0);
        errno_save = deno_sockgeterr();
        if (write_curpos == write_bufend + 1 ||
            (write_curpos == write_netbuf
             && write_bufend == write_buftop - 1)) {
            /* Write failed on full buffer */
            break;
        }
    }

    alog(LOG_DEBUGSOCK, "debug: buffered_write(%d,%p,%d) returning %d",
         fd, buf, len, len - left);
    deno_sockseterr(errno_save);
    return len - left;
}


/*************************************************************************/

/**
 * sgetc ?
 * @param s is socket to read from
 * @return int
 */
int sgetc(deno_socket_t s)
{
    int c;

    if (lastchar != EOF) {
        c = lastchar;
        lastchar = EOF;
        return c;
    }
    return buffered_read_one(s);
}

/*************************************************************************/

/**
 * sungetc ?
 * @param c integer ?
 * @return int
 */
int sungetc(int c)
{
    return lastchar = c;
}

/*************************************************************************/

/**
 * If connection was broken, return NULL.  If the read timed out, return
 * (char *)-1.
 * @param buf Buffer to get
 * @param len Length
 * @param s Socket
 * @return buffer
 */
char *sgets(char *buf, int len, deno_socket_t s)
{
    int c = 0;
    struct timeval tv;
    fd_set fds;
    char *ptr = buf;

    if (len == 0)
        return NULL;
    FD_ZERO(&fds);
    FD_SET(s, &fds);
    tv.tv_sec = ReadTimeout;
    tv.tv_usec = 0;
    while (read_buffer_len() == 0 &&
           (c = select(s + 1, &fds, NULL, NULL, &tv)) < 0) {
        if (deno_sockgeterr() != SOCKERR_EINTR)
            break;
    }
    if (read_buffer_len() == 0 && c == 0)
        return (char *) -1;
    c = sgetc(s);
    while (--len && (*ptr++ = c) != '\n' && (c = sgetc(s)) >= 0);
    if (c < 0)
        return NULL;
    *ptr = 0;
    return buf;
}

/*************************************************************************/

/**
 * sgets2:  Read a line of text from a socket, and strip newline and
 *          carriage return characters from the end of the line.
 * @param buf Buffer to get
 * @param len Length
 * @param s Socket
 * @return buffer
 */
char *sgets2(char *buf, int len, deno_socket_t s)
{
    char *str = sgets(buf, len, s);

    if (!str || str == (char *) -1)
        return str;
    str = buf + strlen(buf) - 1;
    if (*str == '\n')
        *str-- = 0;
    if (*str == '\r')
        *str = 0;
    return buf;
}

/*************************************************************************/

/**
 * Read from a socket.  (Use this instead of read() because it has
 * buffering.)
 * @param s Socket
 * @param buf Buffer to get
 * @param len Length
 * @return int
 */
int sread(deno_socket_t s, char *buf, int len)
{
    return buffered_read(s, buf, len);
}

/*************************************************************************/

/**
 * sputs : write buffer
 * @param s Socket
 * @param str Buffer to write
 * @return int
 */
int sputs(char *str, deno_socket_t s)
{
    return buffered_write(s, str, strlen(str));
}

/*************************************************************************/

/**
 * sockprintf : a socket writting printf()
 * @param s Socket
 * @param fmt format of message
 * @param ... various args
 * @return int
 */
int sockprintf(deno_socket_t s, const char *fmt, ...)
{
    va_list args;
    char buf[16384];            /* Really huge, to try and avoid truncation */
    int value;

    va_start(args, fmt);
    value =
        buffered_write(s, buf, ircvsnprintf(buf, sizeof(buf), fmt, args));
    va_end(args);
    return value;
}

/*************************************************************************/

#if !HAVE_GETHOSTBYNAME

/**
 * Translate an IP dotted-quad address to a 4-byte character string.
 * Return NULL if the given string is not in dotted-quad format.
 * @param ipaddr IP Address
 * @return char 4byte ip char string
 */
char *pack_ip(const char *ipaddr)
{
    static char ipbuf[4];
    int tmp[4], i;

    if (sscanf(ipaddr, "%d.%d.%d.%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3])
        != 4)
        return NULL;
    for (i = 0; i < 4; i++) {
        if (tmp[i] < 0 || tmp[i] > 255)
            return NULL;
        ipbuf[i] = tmp[i];
    }
    return ipbuf;
}

#endif

/*************************************************************************/

#define SERVICE_SERVER 0
#define SERVICE_CLIENT 1
#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

/* error codes used in functions below */
#define ERR_GENERAL	-1      /* could not open a new descriptor or an invalid
                                   value for the protocol was specified */
#define	ERR_BIND	-2      /* call to bind() failed */
#define	ERR_LISTEN	-3      /* call to listen() failed */
#define	ERR_CONNECT	-4      /* call to connect() failed */
#define ERR_GETSOCKNAME	-5      /* call to getsockname() failed */
#define	ERR_RESOLV	-6      /* the name of the host could not be resolved */
#define	ERR_UNSUPP	-7      /* invalid or unsupported request */



/**
 * lhost/lport specify the local side of the connection.  If they are not
 * given (lhost==NULL, lport==0), then they are left free to vary.
 * @param host Remote Host
 * @param port Remote Port
 * @param lhost LocalHost
 * @param lport LocalPort
 * @return int if successful
 */
int conn(const char *host, int port, const char *lhost, int lport)
{

    int sock = -1;
    int protocol = 0;
    int ierr = 0;
    int e;
    struct addrinfo hints, *res, *ress;
    char portnumb[16];
    char lportnumb[16];
    char *portval;

    ircsprintf(portnumb, "%d", port);
    memset(&hints, 0, sizeof(hints));
#ifdef RFC2553BIS
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif
#endif
    hints.ai_socktype =
        (protocol == PROTOCOL_TCP) ? SOCK_STREAM : SOCK_DGRAM;
    if ((e = getaddrinfo(host, portnumb, &hints, &ress))) {
        errno = -1;
        fatal("getaddrinfo(%s, %s): %s (%d)", host, portnumb,
              gai_strerror(e), e);
        return ERR_RESOLV;
    }
    for (res = ress; res; res = res->ai_next) {
        if ((sock =
             socket(res->ai_family, res->ai_socktype,
                    res->ai_protocol)) < 0) {
            ierr = ERR_GENERAL;
            continue;
        }
        if (lhost) {
            struct addrinfo lochints, *locres;

            /* Doing this bind is bad news unless you are sure that
             * the hostname is valid.  This is not true for
             * me at home, since i dynamic-ip it. */
            memset(&lochints, 0, sizeof(lochints));
            lochints.ai_family = res->ai_family;
            lochints.ai_socktype = res->ai_socktype;
            lochints.ai_flags = AI_PASSIVE;
            if (lport) {
                ircsprintf(lportnumb, "%d", lport);
                portval = sstrdup(lportnumb);
            } else {
                portval = sstrdup("0");
            }
            if ((e = getaddrinfo(lhost, portval, &lochints, &locres))) {
                if (res->ai_next) {
                    close(sock);
                    continue;
                } else {
                    close(sock);
                    fatal("getaddrinfo(%s, %s): %s (%d)", lhost, portval,
                          gai_strerror(e), e);
                    return ERR_RESOLV;
                }
            }
            if (bind(sock, locres->ai_addr, locres->ai_addrlen)) {
                int errno_save = deno_sockgeterr();
                deno_sockclose(sock);
                deno_sockseterr(errno_save);
                freeaddrinfo(locres);
                return -1;
            }
            freeaddrinfo(locres);
        }
        if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
            int errno_save = deno_sockgeterr();
            deno_sockclose(sock);
            deno_sockseterr(errno_save);
            return -1;
        }
        ierr = 0;
        break;
    }
    freeaddrinfo(ress);
    if (ierr < 0) {
        close(sock);
        return ierr;
    }
    return sock;
}

/*************************************************************************/

/**
 * Close up the connection
 * @param s Socket
 * @return void
 */
void disconn(deno_socket_t s)
{
    shutdown(s, 2);
    deno_sockclose(s);
}
