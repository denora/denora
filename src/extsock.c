/* External Socket
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

#define MAXQUEUE 10
#define BUFFERSIZE 4086

deno_socket_t svr_fd;
int max_fd;
fd_set master_fdset, temp_fdset;

/*************************************************************************/

/**
 * Init function
 **/
void extsock_init(char *lhost, int lport)
{
#if HAVE_GETHOSTBYNAME
	struct hostent *hp;
#else
	char *addr;
#endif
	int sockopt = 1;
	struct sockaddr_in svr_addr;

	FD_ZERO(&master_fdset);
	FD_ZERO(&temp_fdset);

	if ((svr_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		alog(LOG_DEBUG, "Failure in socket()");
		XMLRPC_Enable = 0;
		return;
	}

	if (setsockopt(svr_fd, SOL_SOCKET, SO_REUSEADDR,
	               (char *) &sockopt, sizeof(int)) == -1)
	{
		alog(LOG_DEBUG, "Failure in setsockopt()");
		XMLRPC_Enable = 0;
		return;
	}

	memset(&svr_addr, 0, sizeof(svr_addr));
	if (lhost)
	{
#if HAVE_GETHOSTBYNAME
		if ((hp = gethostbyname(lhost)) != NULL)
		{
			memcpy((char *) &svr_addr.sin_addr, hp->h_addr, hp->h_length);
			svr_addr.sin_family = hp->h_addrtype;
#else
		if ((addr = pack_ip(lhost)))
		{
			memcpy((char *) &svr_addr.sin_addr, addr, 4);
			svr_addr.sin_family = AF_INET;
#endif
		}
		else
		{
			lhost = NULL;
		}
	}
	else
	{
		alog(LOG_DEBUG, "No ip address to bind information to");
		XMLRPC_Enable = 0;
		return;
	}
	if (lport)
	{
		svr_addr.sin_port = htons((unsigned short) lport);
	}
	else
	{
		alog(LOG_NORMAL, "No port to bind information to");
		XMLRPC_Enable = 0;
		return;
	}

	memset(&(svr_addr.sin_zero), '\0', 8);
	if (bind(svr_fd, (struct sockaddr *) &svr_addr,
	         sizeof(struct sockaddr)) == -1)
	{
		alog(LOG_DEBUG, "Failure to bind to address");
		XMLRPC_Enable = 0;
		return;
	}

	if (listen(svr_fd, MAXQUEUE) == -1)
	{
		alog(LOG_DEBUG, "Failure in listen()");
		XMLRPC_Enable = 0;
		return;
	}

	max_fd = svr_fd;
	FD_SET(svr_fd, &master_fdset);

	alog(LOG_NORMAL, "External socket listening on port %d.", lport);
}

/*************************************************************************/

/**
 * external sockets processing function
 *
 * @return void - no returend value
 */
void extsock_process(void)
{
	int socket_fd, clnt_fd, len, bytes;
	char buffer[BUFFERSIZE];
	struct sockaddr_in clnt_addr;
	struct timeval wait;

	temp_fdset = master_fdset;
	wait.tv_sec = 0;
	wait.tv_usec = 0;

	if (select(max_fd + 1, &temp_fdset, NULL, NULL, &wait) == -1)
	{
		alog(LOG_DEBUG, "Failure in select()");
		XMLRPC_Enable = 0;
		return;
	}

	for (socket_fd = 0; socket_fd <= max_fd; socket_fd++)
	{
		if (FD_ISSET(socket_fd, &temp_fdset))
		{
			if (socket_fd == svr_fd)
			{
				/* Its a new connection */
				len = sizeof(struct sockaddr);
				if ((clnt_fd =
				            accept(svr_fd, (struct sockaddr *) &clnt_addr,
				                   (deno_socklen_t *) & len)) == -1)
				{
					alog(LOG_DEBUG, "Failure in accept()");
					XMLRPC_Enable = 0;
					return;
				}
				else
				{
					alog(LOG_NORMAL, "new connection from %s (socket %d)",
					     inet_ntoa(clnt_addr.sin_addr), clnt_fd);
					if (!extsock_val_clnt(clnt_addr.sin_addr))
					{
						alog(LOG_NORMAL,
						     "%s does not have access to the external socket",
						     inet_ntoa(clnt_addr.sin_addr));
						close(clnt_fd);
						continue;
					}
					FD_SET(clnt_fd, &master_fdset);
					if (clnt_fd > max_fd)
					{
						max_fd = clnt_fd;
					}
				}
			}
			else
			{
				/* Its an existing connection */
				if ((bytes =
				            recv(socket_fd, buffer, sizeof(buffer), 0)) <= 0)
				{
					if (bytes == 0)
					{
						alog(LOG_NORMAL, "socket %d closed.", socket_fd);
					}
					else
					{
						alog(LOG_DEBUG, "Failure in recv()");
					}

					close(socket_fd);
					FD_CLR(socket_fd, &master_fdset);
				}
				else
				{
					buffer[bytes] = '\0';
					alog(LOG_NORMAL, "Recieved data from socket %d: %s",
					     socket_fd, (char *) buffer);
					xmlrpc_process(socket_fd, buffer);
					close(socket_fd);
					FD_CLR(socket_fd, &master_fdset);
				}
			}
		}
	}
}

/*************************************************************************/

/**
 * Close up the external socket and clean up
 *
 * @return void - no returend value
 */
void extsock_close(void)
{
	deno_socket_t sockfd;
	int i;
	char buffer[BUFFERSIZE];

	for (sockfd = 0; sockfd <= max_fd; sockfd++)
	{
		if (!FD_ISSET(sockfd, &temp_fdset))
		{
			if (sockfd == svr_fd)
				continue;
		}
		else
		{
			ircsnprintf(buffer, sizeof(buffer), "%s", "Shutting down");
			if (send(sockfd, buffer, sizeof(buffer), 0) == -1)
			{
				log_perror("send");
			}
			deno_sockclose(sockfd);
		}
	}
	deno_sockclose(svr_fd);
	for (i = 0; i < SockIPNumber; i++)
	{
		if (ExtSockIPs[i])
			free(ExtSockIPs[i]);
	}
	SockIPNumber = 0;
}

/*************************************************************************/

/**
 * Client host validation function
 *
 * @param iaddr of the person connecting
 * @return 0 if not 1 if is valid
 */
int extsock_val_clnt(struct in_addr iaddr)
{
	int i;

	SET_SEGV_LOCATION();

	if (SockIPNumber <= 0)
	{
		alog(LOG_NORMAL, "The ExtSockIP configuration variable was empty");
		return 0;
	}

	SET_SEGV_LOCATION();

	for (i = 0; i < SockIPNumber; i++)
	{
		if (!strcmp(inet_ntoa(iaddr), ExtSockIPs[i]))
		{
			return 1;
		}
	}
	SET_SEGV_LOCATION();

	return 0;
}

/*************************************************************************/
