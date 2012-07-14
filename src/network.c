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

#if defined(_WIN32) && !defined(HAVE_GETADDRINFO)
adns_state adns;
#endif

/*************************************************************************/

/**
 * Resolve a host to an IP
 * @param host to convert
 * @return ip address
 */
char *host_resolve(char *host)
{
#if defined(HAVE_GETADDRINFO)
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
#elif !defined(_WIN32)
	adns_answer *answer;
#else
	struct hostent *hentp = NULL;
	uint32 ip = INADDR_NONE;
	char *ipreturn;
	struct in_addr addr;
#endif

	SET_SEGV_LOCATION();

	if (!host)
	{
		return sstrdup("0.0.0.0");
	}

#if defined(HAVE_GETADDRINFO)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
 
	if ((status = getaddrinfo(host, NULL, &hints, &res)) == 0) {
		for(p = res;p != NULL; p = p->ai_next) {
			void *addr;
			if (p->ai_family == AF_INET) {
				addr = &(p->ai_addr);
			} else {
				struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
				addr = &(ipv6->sin6_addr);
			}
			inet_ntop(p->ai_family, p->ai_addr, ipstr, sizeof ipstr);
			alog(LOG_DEBUG, "debug: %s resolves to IP address %s", host, ipstr);
			freeaddrinfo(res);
			return sstrdup(ipstr);
		}
	}

#elif !defined(_WIN32)
	adns_synchronous(adns, host, adns_r_a,
			 adns_qf_search | adns_qf_owner, &answer);

	SET_SEGV_LOCATION();

	if (answer->status == adns_s_ok)
	{
		if (answer->nrrs == 0)
		{
			alog(LOG_DEBUG, "ERROR: %s has no IP addresses!", host);
			return sstrdup("0.0.0.0");
		}
		if (answer->nrrs > 1)
		{
			alog(LOG_DEBUG, "debug: %s has %d IP addresses (using %s)",
			     host, answer->nrrs, inet_ntoa(answer->rrs.inaddr[0]));
		}
		else
		{
			alog(LOG_DEBUG, "debug: %s resolves to IP address %s", host,
			     inet_ntoa(answer->rrs.inaddr[0]));
		}
		SET_SEGV_LOCATION();
		return sstrdup(inet_ntoa(answer->rrs.inaddr[0]));
	}

#else
	hentp = gethostbyname(host);

	if (hentp)
	{
		memcpy(&ip, hentp->h_addr, sizeof(hentp->h_length));
		addr.s_addr = ip;
		ipreturn = inet_ntoa(addr);
		alog(LOG_DEBUG, "debug: resolved %s to %s", host, ipreturn);
		return sstrdup(ipreturn);
	}
#endif
	return sstrdup("0.0.0.0");
}

/*************************************************************************/

#if !defined(HAVE_INETATON)
int inet_aton(const char *name, struct in_addr *addr)
{
	uint32 a = inet_addr(name);
	addr->s_addr = a;
	return a != (uint32) - 1;
}
#endif

/*************************************************************************/

#ifndef HAVE_GAI_STRERROR
#ifndef __sun
char *gai_strerror(int ecode)
{
	switch (ecode)
	{
		case EAI_NODATA:
			return (char *) "no address associated with hostname.";
		case EAI_MEMORY:
			return (char *) "memory allocation failure.";
		default:
			return (char *) "unknown error.";
	}
}
#endif
#endif			  /* !HAVE_GAI_STRERROR */

/*************************************************************************/

#ifndef HAVE_FREEADDRINFO
void freeaddrinfo(struct addrinfo *ai)
{
	struct addrinfo *next;

	do
	{
		next = ai->ai_next;
		free(ai);
	}
	while (NULL != (ai = next));
}
#endif			  /* !HAVE_FREEADDRINFO */

/*************************************************************************/

#if !defined(HAVE_GETADDRINFO)
struct addrinfo *malloc_ai(int port, uint32 addr)
{
	struct addrinfo *ai;

	if (NULL != (ai = (struct addrinfo *) malloc(sizeof(struct addrinfo) +
			  sizeof(struct
				 sockaddr_in))))
	{
		memset(ai, 0,
		       sizeof(struct addrinfo) + sizeof(struct sockaddr_in));
		ai->ai_addr = (struct sockaddr *) (ai + 1);
		ai->ai_addrlen = sizeof(struct sockaddr_in);
		ai->ai_addr->sa_family = ai->ai_family = AF_INET;
		ai->ai_socktype = SOCK_STREAM;
		((struct sockaddr_in *) (ai)->ai_addr)->sin_port = port;
		((struct sockaddr_in *) (ai)->ai_addr)->sin_addr.s_addr = addr;
		return ai;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************/

int getaddrinfo(const char *hostname, const char *servname,
		const struct addrinfo *hints, struct addrinfo **res)
{
	struct addrinfo *cur, *prev = NULL;
	struct hostent *hp;
	int i, port;

	if (servname)
		port = htons(atoi(servname));
	else
		port = 0;
	if (hints && hints->ai_flags & AI_PASSIVE)
	{
		if (NULL != (*res = malloc_ai(port, htonl(0x00000000))))
		{
			return 0;
		}
		else
		{
			return EAI_MEMORY;
		}
	}
	if (!hostname)
	{
		if (NULL != (*res = malloc_ai(port, htonl(0x7f000001))))
		{
			return 0;
		}
		else
		{
			return EAI_MEMORY;
		}
	}
	if (inet_addr(hostname) != -1)
	{
		if (NULL != (*res = malloc_ai(port, inet_addr(hostname))))
		{
			return 0;
		}
		else
		{
			return EAI_MEMORY;
		}
	}
	if ((hp = gethostbyname(hostname)) &&
		hp->h_name && hp->h_name[0] && hp->h_addr_list[0])
	{
		for (i = 0; hp->h_addr_list[i]; i++)
			if (NULL != (cur = malloc_ai(port, ((struct in_addr *)
							    hp->
							    h_addr_list[i])->s_addr)))
			{
				if (prev)
				{
					prev->ai_next = cur;
				}
				else
				{
					*res = cur;
				}
				prev = cur;
			}
			else
			{
				if (*res)
				{
					freeaddrinfo(*res);
				}
				return EAI_MEMORY;
			}
		return 0;
	}
	return EAI_NODATA;
}
#endif			  /* !HAVE_GETADDRINFO */
