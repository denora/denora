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
 * $Id$
 *
 */

#ifndef SOCKETS_H
#define SOCKETS_H

#ifdef _WIN32

typedef SOCKET				deno_socket_t;
typedef int					deno_socklen_t;
#define deno_sockread(fd, buf, len)	recv(fd, buf, len, 0)
#define deno_sockwrite(fd, buf, len)	 send(fd, buf, len, 0)
#define deno_sockclose(fd)		closesocket(fd)
#define deno_sockgeterr()		WSAGetLastError()
#define deno_sockseterr(err)		WSASetLastError(err)
#define deno_sockgetstrerr()    	deno_sockstrerror(WSAGetLastError())
/* deno_sockstrerror in sockutil.c */
/* deno_socksetnonb in sockutil.c */
#define deno_sockerrnonb(err)		(err == WSAEINPROGRESS || err == WSAEWOULDBLOCK)
#define SOCKERR_EBADF			WSAENOTSOCK
#define SOCKERR_EINTR			WSAEINTR
#define SOCKERR_EINVAL			WSAEINVAL
#define SOCKERR_EINPROGRESS		WSAEINPROGRESS
#define SOCKERR_EWOULDBLOCK     WSAEWOULDBLOCK
#define SOCKERR_EMSGSIZE        WSAEMSGSIZE
#define SOCKERR_EAGAIN          WSAEPROCLIM
#define SOCKERR_ENOBUFS         WSAENOBUFS

#else

typedef	int				deno_socket_t;
typedef	socklen_t		deno_socklen_t;
#define deno_sockread(fd, buf, len)	read(fd, buf, len)
#define deno_sockwrite(fd, buf, len) 	write(fd, buf, len)
#define deno_sockclose(fd)		close(fd)
#define deno_sockgeterr()		errno
#define deno_sockseterr(err)	errno = err
#define deno_sockstrerror()		strerror(err)
#define deno_sockgetstrerr()    strerror(errno)
#define deno_socksetnonb(fd)	fcntl(fd, F_SETFL, O_NONBLOCK)
#define deno_sockerrnonb(err)	(err == EINPROGRESS)
#define SOCKERR_EBADF			EBADF
#define SOCKERR_EINTR			EINTR
#define SOCKERR_EINVAL			EINVAL
#define SOCKERR_EINPROGRESS		EINPROGRESS
#define SOCKERR_EWOULDBLOCK     EWOULDBLOCK
#define SOCKERR_EMSGSIZE        EMSGSIZE
#define SOCKERR_EAGAIN          EAGAIN
#define SOCKERR_ENOBUFS         ENOBUFS
#endif

#ifndef EAI_NODATA
#define EAI_NODATA	1
#endif
#ifndef EAI_MEMORY
#define EAI_MEMORY	2
#endif

#ifndef AI_PASSIVE
# define AI_PASSIVE        1
# define AI_CANONNAME      2
#endif

#ifndef NI_NUMERICHOST
# define NI_NUMERICHOST    2
# define NI_NAMEREQD       4
# define NI_NUMERICSERV    8
#endif

#ifdef __STRICT_ANSI__
#undef HAVE_STRUCT_ADDRINFO
#undef HAVE_GETADDRINFO
#undef HAVE_GAI_STRERROR
#undef HAVE_FREEADDRINFO
#endif

#if !defined(HAVE_STRUCT_ADDRINFO)
struct addrinfo {
	int	ai_flags;		/* AI_PASSIVE, AI_CANONNAME */
	int	ai_family;		/* PF_xxx */
	int	ai_socktype;		/* SOCK_xxx */
	int	ai_protocol;		/* 0 or IPPROTO_xxx for IPv4 and IPv6 */
	size_t	ai_addrlen;		/* length of ai_addr */
	char	*ai_canonname;		/* canonical name for hostname */
	struct sockaddr *ai_addr;	/* binary address */
	struct addrinfo *ai_next;	/* next structure in linked list */
};
#endif /* !HAVE_STRUCT_ADDRINFO */

#if !defined(HAVE_GETADDRINFO)
int getaddrinfo(const char *hostname, const char *servname, 
                const struct addrinfo *hints, struct addrinfo **res);
#endif /* !HAVE_GETADDRINFO */

#if !defined(HAVE_GAI_STRERROR)
#ifndef __sun
char *gai_strerror(int ecode);
#endif
#endif

#if !defined(HAVE_FREEADDRINFO)
#ifndef __sun
void freeaddrinfo(struct addrinfo *ai);
#endif
#endif

#ifndef HAVE_STRUCT_SOCKADDR_STORAGE
#define	_SS_MAXSIZE	128	/* Implementation specific max size */
#define	_SS_ALIGNSIZE	(sizeof(int))
#define	_SS_PAD1SIZE	(_SS_ALIGNSIZE - sizeof(u_short))
#define	_SS_PAD2SIZE	(_SS_MAXSIZE - (sizeof(u_short) + \
					_SS_PAD1SIZE + _SS_ALIGNSIZE))

struct sockaddr_storage {
  u_short	ss_family;
  char		__ss_pad1[_SS_PAD1SIZE];
  int			__ss_align;
  char		__ss_pad2[_SS_PAD2SIZE];
};
#endif /* !HAVE_STRUCT_SOCKADDR_STORAGE */

#ifndef IN6_IS_ADDR_LOOPBACK
#define IN6_IS_ADDR_LOOPBACK(a) \
	(((u_int32_t *) (a))[0] == 0 && ((u_int32_t *) (a))[1] == 0 && \
	 ((u_int32_t *) (a))[2] == 0 && ((u_int32_t *) (a))[3] == htonl (1))
#endif /* !IN6_IS_ADDR_LOOPBACK */

#ifndef HAVE_STRUCT_IN6_ADDR
struct in6_addr {
	u_int8_t		s6_addr[16];
};
#endif /* !HAVE_STRUCT_IN6_ADDR */

#if !defined(HAVE_STRUCT_SOCKADDR_IN6)
struct sockaddr_in6 {
   unsigned short sin6_family;
	u_int16_t sin6_port;
	u_int32_t sin6_flowinfo;
	struct in6_addr sin6_addr;
};
#endif

#if !defined(AF_INET6)
/* Define it to something that should never appear */
#define AF_INET6 AF_MAX
#endif

#endif

