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

#ifdef HAVE_OPENSSL
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#ifdef HAVE_OPENSSL
    if (me.ssl.enabled && (me.ssl.ctx = create_ssl_context()) == NULL) {
        log_error("could not create default SSL context.  ssl is disabled.");
        me.ssl.enabled = 0;
    }
#endif



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
#ifdef HAVE_OPENSSL
    if (SOCKET_SSL(sock))
        ret = SSL_read(sock->ssl, buf, nbytes);
    else
#endif
#ifdef HAVE_RECV
        ret = recv(sock->fd, buf, nbytes, SOCKET_RECV_FLAGS);
#else
        ret = read(sock->fd, buf, nbytes);
#endif

#ifdef HAVE_OPENSSL
    if (SOCKET_SSL(sock))
        switch (SSL_get_error(sock->ssl, ret)) {
            case SSL_ERROR_WANT_READ:
                sock->state |= SOCKET_FL_SSLWANT_READ;
                socket_monitor(sock, SOCKET_FL_READ | SOCKET_FL_INTERNAL);
                return 0;
            case SSL_ERROR_WANT_WRITE:
                sock->state |= SOCKET_FL_SSLWANT_WRITE;
                socket_monitor(sock, SOCKET_FL_WRITE | SOCKET_FL_INTERNAL);
                return 0;
            case SSL_ERROR_NONE:
                break; /* ignore this case */
            case SSL_ERROR_SYSCALL:
                /* log this to debug */
                if (ERR_peek_error() == 0) {
                    if (ret == 0)
                        log_debug("socket_read(SSL%d, %p, %d): "
                                "invalid eof", sock->fd, buf, nbytes);
                    else
                        log_debug("socket_read(SSL%d, %p, %d): %s", sock->fd,
                                buf, nbytes, strerror(errno));
                } else
                    log_debug("socket_read(SSL%d, %p, %d): %s", sock->fd, buf,
                            nbytes, ERR_error_string(ERR_get_error(), NULL));
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
            case SSL_ERROR_SSL:
                /* This is a pretty nasty case. */
                log_error("socket_read(SSL%d, %p, %d): %s", sock->fd, buf,
                        nbytes, ERR_error_string(ERR_get_error(), NULL));
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
            default:
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
        }
    else
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


int socket_write(isocket_t *sock, char *buf, size_t nbytes) {
    int ret;

    if (!(sock->state & SOCKET_FL_OPEN))
        return -1;

    assert(nbytes > 0 && nbytes < INT_MAX);

    errno = 0;
#ifdef HAVE_OPENSSL
    if (SOCKET_SSL(sock))
        ret = SSL_write(sock->ssl, buf, nbytes);
    else
#endif
#ifdef HAVE_SEND
        ret = send(sock->fd, buf, nbytes, SOCKET_SEND_FLAGS);
#else
        ret = write(sock->fd, buf, nbytes);
#endif

#ifdef HAVE_OPENSSL
    if (SOCKET_SSL(sock))
        switch (SSL_get_error(sock->ssl, ret)) {
            case SSL_ERROR_WANT_READ:
                sock->state |= SOCKET_FL_SSLWANT_READ;
                socket_monitor(sock, SOCKET_FL_READ | SOCKET_FL_INTERNAL);
                return 0;
            case SSL_ERROR_WANT_WRITE:
                sock->state |= SOCKET_FL_SSLWANT_WRITE;
                socket_monitor(sock, SOCKET_FL_WRITE | SOCKET_FL_INTERNAL);
                return 0;
            case SSL_ERROR_NONE:
                break; /* ignore this case */
            case SSL_ERROR_SYSCALL:
                /* log this to debug */
                if (ERR_peek_error() == 0) {
                    if (ret == 0)
                        log_debug("socket_write(SSL%d, %p, %d): "
                                "invalid eof", sock->fd, buf, nbytes);
                    else
                        log_debug("socket_write(SSL%d, %p, %d): %s", sock->fd,
                                buf, nbytes, strerror(errno));
                } else
                    log_debug("socket_write(SSL%d, %p, %d): %s", sock->fd, buf,
                            nbytes, ERR_error_string(ERR_get_error(), NULL));
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
            case SSL_ERROR_SSL:
                /* This is a pretty nasty case. */
                log_error("socket_write(SSL%d, %p, %d): %s", sock->fd, buf,
                        nbytes, ERR_error_string(ERR_get_error(), NULL));
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
            default:
                sock->state |= SOCKET_FL_ERROR_PENDING;
                return -1;
        }
    else
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
#ifdef HAVE_OPENSSL
        else if (SOCKET_SSL(sp) && sp->ssl_start != 0 &&
                SOCKET_SSL_HANDSHAKING(sp) &&
                sp->ssl_start + me.ssl.hs_timeout < me.now) {
            sp->state |= SOCKET_FL_ERROR_PENDING; /* flag an error condition on
                                                     the socket so that it will
                                                     get hooked where it is
                                                     necessary. */
            sp->err = ETIMEDOUT; /* set an appropriate error condition */
        }
#endif

        sp = sp2;
    }
}


#ifdef HAVE_OPENSSL
/* Below are the special SSL-only functions */
static int socket_ssl_verify_callback(int status, X509_STORE_CTX *store) {
    char buf[256];

    if (!status) {
        /* just do error reporting. */
        X509 *cert = X509_STORE_CTX_get_current_cert(store);
        int err = X509_STORE_CTX_get_error(store);

        log_warn("certificate error (depth %d): %d:%s",
                X509_STORE_CTX_get_error_depth(store), err,
                X509_verify_cert_error_string(err));
        X509_NAME_oneline(X509_get_issuer_name(cert), buf, 256);
        log_warn("issuer=%s", buf);
        X509_NAME_oneline(X509_get_subject_name(cert), buf, 256);
        log_warn("subject=%s", buf);
    }

    return status;
}

SSL_CTX *create_ssl_context(void) {
    SSL_CTX *ctx;

    if (!me.ssl.enabled)
        return NULL;

    /* Here we create our default SSL context.  This requires at least a
     * certificate file and a keyfile (probably the same file). */
    ctx = SSL_CTX_new(SSLv23_method());
    SSL_CTX_set_options(ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2);
    if (*me.ssl.cafile != '\0') {
        if (SSL_CTX_load_verify_locations(ctx, me.ssl.cafile, NULL) != 1) {
            log_error("error loading SSL CA file %s: %s", me.ssl.cafile,
                    ERR_error_string(ERR_get_error(), NULL));
            SSL_CTX_free(ctx);
            return NULL;
        }
    }
    if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
        log_error("error loading default SSL CA data: %s",
                ERR_error_string(ERR_get_error(), NULL));
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_certificate_chain_file(ctx, me.ssl.certfile) != 1) {
        log_error("error using certificate file in %s: %s", me.ssl.certfile,
                ERR_error_string(ERR_get_error(), NULL));
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, me.ssl.keyfile, SSL_FILETYPE_PEM) !=
            1) {
        log_error("error using private key file in %s: %s", me.ssl.keyfile,
                ERR_error_string(ERR_get_error(), NULL));
        SSL_CTX_free(ctx);
        return NULL;
    }
    if (me.ssl.verify == true)
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, socket_ssl_verify_callback);
    else
        SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    return ctx;
}

/* This function enables SSL on sockets.  In the case of listening sockets it
 * simply sets the SSL flag on them as a convenience.  You are responsible for
 * checking that a socket has been accepted on an SSL listener and engaging the
 * SSL negotation using this function.  For other types of sockets this
 * function will initialize the 'ssl' member of the structure in preparation
 * for calls to socket_ssl_accept/socket_ssl_connect (socket_ssl_handshake)) */
bool socket_ssl_enable(isocket_t *isp) {

    if (!me.ssl.enabled)
        return false;

    /* Make sure the socket is in the right state. */
    if (!(isp->state & SOCKET_FL_OPEN) ||
            !((isp->state & SOCKET_FL_LISTENING) ||
                (isp->state & SOCKET_FL_OPEN)))
        return false;

    if (isp->state & SOCKET_FL_LISTENING) {
        isp->state |= SOCKET_FL_SSL;
        return true; /* that's it! */
    }

    /* otherwise... */
    if ((isp->ssl = SSL_new(me.ssl.ctx)) == NULL) {
        log_error("could not create new SSL session: %s",
                ERR_error_string(ERR_get_error(), NULL));
        return false;
    }
    if (SSL_set_fd(isp->ssl, isp->fd) != 1) {
        log_error("could not set fd on SSL: %s",
                ERR_error_string(ERR_get_error(), NULL));
        SSL_free(isp->ssl);
        isp->ssl = NULL;
        return false;
    }

    isp->state |= SOCKET_FL_SSL;
    return true;
}

/* this function initiates a client SSL connection on the given socket.  it may
 * be called multiple times if the transaction would block. */
bool socket_ssl_handshake(isocket_t *isp, bool serv) {
    int ret;

    if (!me.ssl.enabled)
        return false;

    /* If we haven't set accept/conn transaction states do so now. */
    if (!SOCKET_SSL_HANDSHAKING(isp)) {
        isp->state |= SOCKET_FL_SSL_HANDSHAKE;
        if (serv)
            SSL_set_accept_state(isp->ssl);
        else
            SSL_set_connect_state(isp->ssl);
        isp->ssl_start = me.now;
    }

    switch (SSL_get_error(isp->ssl, (ret = SSL_do_handshake(isp->ssl)))) {
        case SSL_ERROR_NONE:
            /* This is the success case which indicates that the connection has
             * been fully established. */
            isp->state &= ~SOCKET_FL_SSL_HANDSHAKE;
            return true;
        case SSL_ERROR_ZERO_RETURN:
            /* This case indicates failure at the protocl layer. */
            isp->state &= ~SOCKET_FL_SSL_HANDSHAKE;
            log_debug("while doing SSL handshake on fd %d the handshake "
                    "was shut down by the peer.", isp->fd);
            return false;
        case SSL_ERROR_WANT_READ:
            /* this is the case when we simply need more i/o later. */
            isp->state |= SOCKET_FL_SSLWANT_READ;
            socket_monitor(isp, SOCKET_FL_READ | SOCKET_FL_INTERNAL);
            return true;
        case SSL_ERROR_WANT_WRITE:
            /* this is the case when we need more i/o later.  it is slightly
             * different because we handle write-tracking differently. */
            isp->state &= ~SOCKET_FL_WRITE_PENDING;
            isp->state |= SOCKET_FL_SSLWANT_WRITE;
            socket_monitor(isp, SOCKET_FL_WRITE | SOCKET_FL_INTERNAL);
            return true;
        case SSL_ERROR_SYSCALL:
            /* this is a pretty nasty case.  basically if an OS syscall fails,
             * we get this, then we need to figure out what failed and where.
             * bleah. */
            if (ERR_peek_error() == 0) {
                if (ret == 0)
                    log_debug("while doing SSL handshake on fd %d the "
                            "handshake was improperly terminated.", isp->fd);
                else
                    log_debug("while doing SSL handshake on fd %d the "
                            "system failed: %s", isp->fd, strerror(errno));
            } else
                log_debug("while doing SSL handhskae on fd %d an error "
                        "occured: %s", isp->fd, 
                        ERR_error_string(ERR_get_error(), NULL));
            isp->state &= ~SOCKET_FL_SSL_HANDSHAKE;
            isp->state |= SOCKET_FL_ERROR_PENDING;
            isp->err = ECONNABORTED;
            return false;
        case SSL_ERROR_SSL:
            /* This is a pretty nasty case. */
            log_error("while doing SSL handshake on fd %d the SSL "
                    "library failed: %s", isp->fd,
                    ERR_error_string(ERR_get_error(), NULL));
            isp->state &= ~SOCKET_FL_SSL_HANDSHAKE;
            isp->state |= SOCKET_FL_ERROR_PENDING;
            isp->err = ECONNABORTED;
            return false;
        default:
            log_error("yikes!  unhandled case in socket_ssl_handshake!");
            isp->state |= SOCKET_FL_ERROR_PENDING;
            isp->err = 0;
            return false;
    }

    return true;
}
#endif

/* this function is called by the different pollers on sockets with pending
 * data.  because of SSL it is no longer as simple as activating a hook on a
 * socket. */
static inline void socket_event(isocket_t *isp) {

#ifdef HAVE_OPENSSL
    /* see if we're doing SSL on this socket.  if we are we need to make sure
     * that the SSL requisite conditions are being met for the socket.  if
     * they're not, continue to wait for them to be met. */
    if (SOCKET_SSL(isp) && !SOCKET_LISTENING(isp)) {
        if (SOCKET_SSL_HANDSHAKING(isp)) {
            /* If we're handshaking see if we can finish that off. */
            if (!socket_ssl_handshake(isp, 0)) {
                /* some kind of fatal error occured. */
                isp->state |= SOCKET_FL_ERROR_PENDING;
                return;
            }

            /* otherwise the handshake was successful.  we fall through below
             * to set/unset the read conditions as necessary and then trigger
             * the socket's hook.  even if the socket isn't monitoring at all
             * we will trip the socket hook at least once (so that the consumer
             * will be able to see that the SSL handshake has finished) */
        }
        if (isp->state & SOCKET_FL_SSLWANT_READ) {
            if (isp->state & SOCKET_FL_READ_PENDING) {
                isp->state &= ~SOCKET_FL_SSLWANT_READ;
                if (!(isp->state & SOCKET_FL_WANT_READ))
                    socket_unmonitor(isp, SOCKET_FL_READ);
            } else
                return; /* read condition not fulfilled. */
        }
        if (isp->state & SOCKET_FL_SSLWANT_WRITE) {
            if (isp->state & SOCKET_FL_WRITE_PENDING) {
                isp->state &= ~SOCKET_FL_SSLWANT_WRITE;
                if (!(isp->state & SOCKET_FL_WANT_WRITE))
                    socket_unmonitor(isp, SOCKET_FL_WRITE);
            } else
                return; /* read condition not fulfilled. */
        }
    }
#endif

    /* If we haven't done anything else, this is all we need to do. */
    hook_event(isp->datahook, isp);
    isp->state &= ~SOCKET_FL_PENDING;
}

