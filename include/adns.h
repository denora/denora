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
 * adns is Copyright 1997-2000 Ian Jackson, Copyright 1999-2000 Tony
 *  Finch, and Copyright (C) 1991 Massachusetts Institute of Technology.
 *
 * $Id$
 *
 */

/*
 * adns.h
 * - adns user-visible API (single-threaded, without any locking)
 */

#ifndef ADNS_H_INCLUDED
#define ADNS_H_INCLUDED

#ifndef _WIN32

typedef unsigned char byte;

#define DNS_INADDR_ARPA "in-addr", "arpa"

#define ADNS_POLLFDS_RECOMMENDED 2
#define MAX_POLLFDS  ADNS_POLLFDS_RECOMMENDED

/* All struct in_addr anywhere in adns are in NETWORK byte order. */

	typedef struct adns__state *adns_state;
	typedef struct adns__query *adns_query;

	typedef enum {
		adns_if_noenv = 0x0001,			/* do not look at environment 						 */
		adns_if_noerrprint = 0x0002,	/* never print output to stderr (_debug overrides)   */
		adns_if_noserverwarn = 0x0004,	/* do not warn to stderr about duff nameservers etc  */
		adns_if_debug = 0x0008,			/* enable all output to stderr plus debug msgs 		 */
		adns_if_logpid = 0x0080,		/* include pid in diagnostic output 				 */
		adns_if_noautosys = 0x0010,		/* do not make syscalls at every opportunity 		 */
		adns_if_eintr = 0x0020,			/* allow _wait and _synchronous to return EINTR 	 */
		adns_if_nosigpipe = 0x0040,		/* applic has SIGPIPE set to SIG_IGN, do not protect */
		adns_if_checkc_entex = 0x0100,	/* do consistency checks on entry/exit to adns funcs */
		adns_if_checkc_freq = 0x0300	/* do consistency checks very frequently (slow!) 	 */
	} adns_initflags;

	typedef enum {
		adns_qf_search = 0x00000001,	/* use the searchlist */
		adns_qf_usevc = 0x00000002,	/* use a virtual circuit (TCP connection) */
		adns_qf_owner = 0x00000004,	/* fill in the owner field in the answer */
		adns_qf_quoteok_query = 0x00000010,	/* allow special chars in query domain */
		adns_qf_quoteok_cname = 0x00000000,	/* allow ... in CNAME we go via - now default */
		adns_qf_quoteok_anshost = 0x00000040,	/* allow ... in things supposed to be hostnames */
		adns_qf_quotefail_cname = 0x00000080,	/* refuse if quote-req chars in CNAME we go via */
		adns_qf_cname_loose = 0x00000100,	/* allow refs to CNAMEs - without, get _s_cname */
		adns_qf_cname_forbid = 0x00000200,	/* don't follow CNAMEs, instead give _s_cname */
		adns__qf_internalmask = 0x0ff00000
	} adns_queryflags;

	typedef enum {
		adns__rrt_typemask = 0x0ffff,
		adns__qtf_deref = 0x10000,	/* dereference domains and perhaps produce extra data */
		adns__qtf_mail822 = 0x20000,	/* make mailboxes be in RFC822 rcpt field format */

		adns_r_none = 0,

		adns_r_a = 1,

		adns_r_ns_raw = 2,
		adns_r_ns = adns_r_ns_raw | adns__qtf_deref,

		adns_r_cname = 5,

		adns_r_soa_raw = 6,
		adns_r_soa = adns_r_soa_raw | adns__qtf_mail822,

		adns_r_ptr_raw = 12,
		adns_r_ptr = adns_r_ptr_raw | adns__qtf_deref,

		adns_r_hinfo = 13,

		adns_r_mx_raw = 15,
		adns_r_mx = adns_r_mx_raw | adns__qtf_deref,

		adns_r_txt = 16,

		adns_r_rp_raw = 17,
		adns_r_rp = adns_r_rp_raw | adns__qtf_mail822,

		adns_r_addr = adns_r_a | adns__qtf_deref
	} adns_rrtype;

	typedef enum {
		adns_s_ok,

		/* locally induced errors */
		adns_s_nomemory,
		adns_s_unknownrrtype,
		adns_s_systemfail,

		adns_s_max_localfail = 29,

		/* remotely induced errors, detected locally */
		adns_s_timeout,
		adns_s_allservfail,
		adns_s_norecurse,
		adns_s_invalidresponse,
		adns_s_unknownformat,

		adns_s_max_remotefail = 59,

		/* remotely induced errors, reported by remote server to us */
		adns_s_rcodeservfail,
		adns_s_rcodeformaterror,
		adns_s_rcodenotimplemented,
		adns_s_rcoderefused,
		adns_s_rcodeunknown,

		adns_s_max_tempfail = 99,

		/* remote configuration errors */
		adns_s_inconsistent,	/* PTR gives domain whose A does not exist and match */
		adns_s_prohibitedcname,	/* CNAME found where eg A expected (not if _qf_loosecname) */
		adns_s_answerdomaininvalid,
		adns_s_answerdomaintoolong,
		adns_s_invaliddata,

		adns_s_max_misconfig = 199,

		/* permanent problems with the query */
		adns_s_querydomainwrong,
		adns_s_querydomaininvalid,
		adns_s_querydomaintoolong,

		adns_s_max_misquery = 299,

		/* permanent errors */
		adns_s_nxdomain,
		adns_s_nodata,

		adns_s_max_permfail = 499
	} adns_status;

	typedef struct {
		int len;
		union {
			struct sockaddr sa;
			struct sockaddr_in inet;
		} addr;
	} adns_rr_addr;

	typedef struct {
		char *host;
		adns_status astatus;
		int naddrs;	/* temp fail => -1, perm fail => 0, s_ok => >0 */
		adns_rr_addr *addrs;
	} adns_rr_hostaddr;

	typedef struct {
		char *(array[2]);
	} adns_rr_strpair;

	typedef struct {
		int i;
		adns_rr_hostaddr ha;
	} adns_rr_inthostaddr;

	typedef struct {
		/* Used both for mx_raw, in which case i is the preference and str the domain,
		 * and for txt, in which case each entry has i for the `text' length,
		 * and str for the data (which will have had an extra nul appended
		 * so that if it was plain text it is now a null-terminated string).
		 */
		int i;
		char *str;
	} adns_rr_intstr;

	typedef struct {
		adns_rr_intstr array[2];
	} adns_rr_intstrpair;

	typedef struct {
		char *mname, *rname;
		unsigned long serial, refresh, retry, expire, minimum;
	} adns_rr_soa;

	typedef struct {
		adns_status status;
		char *cname;	/* always NULL if query was for CNAME records */
		char *owner;	/* only set if requested in query flags, and may be 0 on error anyway */
		adns_rrtype type;	/* guaranteed to be same as in query */
		time_t expires;	/* expiry time, defined only if _s_ok, nxdomain or nodata. NOT TTL! */
		int nrrs, rrsz;	/* nrrs is 0 if an error occurs */
		union {
			void *untyped;
			unsigned char *bytes;
			char *(*str);	/* ns_raw, cname, ptr, ptr_raw */
			adns_rr_intstr *(*manyistr);	/* txt (list of strings ends with i=-1, str=0) */
			adns_rr_addr *addr;	/* addr */
			struct in_addr *inaddr;	/* a */
			adns_rr_hostaddr *hostaddr;	/* ns */
			adns_rr_intstrpair *intstrpair;	/* hinfo */
			adns_rr_strpair *strpair;	/* rp, rp_raw */
			adns_rr_inthostaddr *inthostaddr;	/* mx */
			adns_rr_intstr *intstr;	/* mx_raw */
			adns_rr_soa *soa;	/* soa, soa_raw */
		} rrs;
	} adns_answer;

typedef void (*fd_update) (int fd, short what);

E int adns_init(adns_state *newstate_r, adns_initflags flags,
		      FILE * diagfile);

E int adns_init_strcfg(adns_state *newstate_r, adns_initflags flags,
		     FILE *diagfile /*0=>discard*/, const char *configtext);

E int adns_synchronous(adns_state ads,
			    const char *owner, adns_rrtype type,
			    adns_queryflags flags, adns_answer ** answer_r);

E int adns_submit(adns_state ads,
				const char *owner, adns_rrtype type,
				adns_queryflags flags, void *context,
				adns_query *query_r);

/* The owner should be quoted in master file format. */

E int adns_check(adns_state ads,
				adns_query * query_io, adns_answer **answer_r,
				void **context_r);

E int adns_wait(adns_state ads,
				adns_query * query_io, adns_answer **answer_r,
				void **context_r);

/* same as adns_wait but uses poll(2) internally */
E int adns_wait_poll(adns_state ads,
				adns_query * query_io, adns_answer **answer_r,
				void **context_r);

E void adns_cancel(adns_query query);

/* The adns_query you get back from _submit is valid (ie, can be
 * legitimately passed into adns functions) until it is returned by
 * adns_check or adns_wait, or passed to adns_cancel.  After that it
 * must not be used.  You can rely on it not being reused until the
 * first adns_submit or _transact call using the same adns_state after
 * it became invalid, so you may compare it for equality with other
 * query handles until you next call _query or _transact.
 *
 * _submit and _synchronous return ENOSYS if they don't understand the
 * query type.
 */

E int adns_submit_reverse(adns_state ads,
				const struct sockaddr *addr, adns_rrtype type,
				adns_queryflags flags, void *context,
				adns_query *query_r);
/* type must be _r_ptr or _r_ptr_raw.  _qf_search is ignored.
 * addr->sa_family must be AF_INET or you get ENOSYS.
 */

E int adns_submit_reverse_any(adns_state ads,
				const struct sockaddr *addr, const char *rzone,
				adns_rrtype type, adns_queryflags flags,
			    void *context, adns_query *query_r);
/* For RBL-style reverse `zone's; look up
 *   <reversed-address>.<zone>
 * Any type is allowed.  _qf_search is ignored.
 * addr->sa_family must be AF_INET or you get ENOSYS.
 */

E void adns_finish(adns_state ads);
/* You may call this even if you have queries outstanding;
 * they will be cancelled.
 */


E void adns_forallqueries_begin(adns_state ads);
E adns_query adns_forallqueries_next(adns_state ads, void **context_r);
/* Iterator functions, which you can use to loop over the outstanding
 * (submitted but not yet successfuly checked/waited) queries.
 *
 * You can only have one iteration going at once.  You may call _begin
 * at any time; after that, an iteration will be in progress.  You may
 * only call _next when an iteration is in progress - anything else
 * may coredump.  The iteration remains in progress until _next
 * returns 0, indicating that all the queries have been walked over,
 * or ANY other adns function is called with the same adns_state (or a
 * query in the same adns_state).  There is no need to explicitly
 * finish an iteration.
 *
 * context_r may be 0.  *context_r may not be set when _next returns 0.
 */

E  void adns_checkconsistency(adns_state ads, adns_query qu);
/* Checks the consistency of adns's internal data structures.
 * If any error is found, the program will abort().
 * You may pass 0 for qu; if you pass non-null then additional checks
 * are done to make sure that qu is a valid query.
 */

/*
 * Example expected/legal calling sequence for submit/check/wait:
 *  adns_init
 *  adns_submit 1
 *  adns_submit 2
 *  adns_submit 3
 *  adns_wait 1
 *  adns_check 3 -> EAGAIN
 *  adns_wait 2
 *  adns_wait 3
 *  ....
 *  adns_finish
 */

/*
 * Entrypoints for generic asynch io:
 * (these entrypoints are not very useful except in combination with *
 * some of the other I/O model calls which can tell you which fds to
 * be interested in):
 *
 * Note that any adns call may cause adns to open and close fds, so
 * you must call beforeselect or beforepoll again just before
 * blocking, or you may not have an up-to-date list of it's fds.
 */

E int adns_processany(adns_state ads);
/* Gives adns flow-of-control for a bit.  This will never block, and
 * can be used with any threading/asynch-io model.  If some error
 * occurred which might cause an event loop to spin then the errno
 * value is returned.
 */

E int adns_processreadable(adns_state ads, deno_socket_t fd, const struct timeval *now);
E int adns_processwriteable(adns_state ads, deno_socket_t fd, const struct timeval *now);
E int adns_processexceptional(adns_state ads, deno_socket_t fd, const struct timeval *now);
E void adns_processtimeouts(adns_state ads, const struct timeval *now);
E void adns_firsttimeout(adns_state ads,
		       struct timeval **tv_mod, struct timeval *tv_buf,
		       struct timeval now);
E void adns_globalsystemfailure(adns_state ads);
E void adns_beforeselect(adns_state ads, int *maxfd, fd_set *readfds,
		       fd_set *writefds, fd_set *exceptfds,
		       struct timeval **tv_mod, struct timeval *tv_buf,
			       const struct timeval *now);
E void adns_afterselect(adns_state ads, int maxfd, const fd_set *readfds,
		      const fd_set *writefds, const fd_set *exceptfds, const struct timeval *now);

struct pollfd;
E int adns_beforepoll(adns_state ads, struct pollfd *fds, int *nfds_io, int *timeout_io,
			    const struct timeval *now);

E void adns_afterpoll(adns_state ads, const struct pollfd *fds, int nfds,
		    const struct timeval *now);
E adns_status adns_rr_info(adns_rrtype type,
			 const char **rrtname_r, const char **fmtname_r,
			 int *len_r, const void *datap, char **data_r);
E const char *adns_strerror(adns_status st);
E const char *adns_errtypeabbrev(adns_status st);

#define ALIST_INIT(list) ((list).head= (list).tail= 0)
#define ALINK_INIT(link) ((link).next= (link).back= 0)

#define ALIST_UNLINK_PART(list,node,part) \
  do { \
    if ((node)->part back) (node)->part back->part next= (node)->part next; \
      else                                  (list).head= (node)->part next; \
    if ((node)->part next) (node)->part next->part back= (node)->part back; \
      else                                  (list).tail= (node)->part back; \
  } while(0)

#define ALIST_LINK_TAIL_PART(list,node,part) \
  do { \
    (node)->part next= 0; \
    (node)->part back= (list).tail; \
    if ((list).tail) (list).tail->part next= (node); else (list).head= (node); \
    (list).tail= (node); \
  } while(0)

#define ALIST_UNLINK(list,node) ALIST_UNLINK_PART(list,node,)
#define ALIST_LINK_TAIL(list,node) ALIST_LINK_TAIL_PART(list,node,)

typedef enum {
	cc_user,
	cc_entex,
	cc_freq
} consistency_checks;

typedef enum {
	rcode_noerror,
	rcode_formaterror,
	rcode_servfail,
	rcode_nxdomain,
	rcode_notimp,
	rcode_refused
} dns_rcode;

/* Shared data structures */

typedef union {
	adns_status status;
	char *cp;
	adns_rrtype type;
	int i;
	struct in_addr ia;
	unsigned long ul;
} rr_align;

typedef struct {
	int used, avail;
	byte *buf;
} vbuf;

typedef struct {
	adns_state ads;
	adns_query qu;
	int serv;
	const byte *dgram;
	int dglen, nsstart, nscount, arcount;
	struct timeval now;
} parseinfo;

typedef struct {
	adns_rrtype type;
	const char *rrtname;
	const char *fmtname;
	int rrsz;

	void (*makefinal) (adns_query qu, void *data);
	adns_status(*convstring) (vbuf * vb, const void *data);
	adns_status (*parse)(const parseinfo *pai, int cbyte, int max, void *store_r);
	int (*diff_needswap)(adns_state ads, const void *datap_a, const void *datap_b);
} typeinfo;

typedef struct allocnode {
	struct allocnode *next, *back;
} allocnode;

extern union maxalign {
	byte d[1];
	struct in_addr ia;
	long l;
	void *p;
	void (*fp) (void);
	union maxalign *up;
} data;

typedef struct {
	void *ext;
	void (*callback) (adns_query parent, adns_query child);
	union {
		adns_rr_addr ptr_parent_addr;
		adns_rr_hostaddr *hostaddr;
	} info;
} qcontext;

struct adns__query {
	adns_state ads;
	enum { query_tosend, query_tcpw, query_childw, query_done } state;
	adns_query back, next, parent;
	struct {
		adns_query head, tail;
	} children;
	struct {
		adns_query back, next;
	} siblings;
	struct {
		allocnode *head, *tail;
	} allocations;
	int interim_allocd, preserved_allocd;
	void *final_allocspace;

	const typeinfo *typei;
	byte *query_dgram;
	int query_dglen;

	vbuf vb;
	adns_answer *answer;
	byte *cname_dgram;
	int cname_dglen, cname_begin;
	vbuf search_vb;
	int search_origlen, search_pos, search_doneabs;
	int id, flags, retries;
	int udpnextserver;
	unsigned long udpsent;	/* bitmap indexed by server */
	struct timeval timeout;
	time_t expires;		/* Earliest expiry time of any record we used. */
	qcontext ctx;
};

struct query_queue {
	adns_query head, tail;
};

struct adns__state {
	adns_initflags iflags;
	FILE *diagfile;
	fd_update fdfunc;
	int configerrno;
	struct query_queue udpw, tcpw, childw, output;
	adns_query forallnext;
  int nextid;
  deno_socket_t udpsocket, tcpsocket;
	vbuf tcpsend, tcprecv;
  int nservers, nsortlist, nsearchlist, searchndots, tcpserver, tcprecv_skip;
	enum adns__tcpstate {
		server_disconnected, server_connecting,
		server_ok, server_broken
	} tcpstate;
	struct timeval tcptimeout;
#ifdef HAVE_POLL
	struct pollfd pollfds_buf[MAX_POLLFDS];
#endif
	struct server {
		struct in_addr addr;
	} servers[MAXSERVERS];
	struct sortlist {
		struct in_addr base, mask;
	} sortlist[MAXSORTLIST];
	char **searchlist;
};

int adns__vbuf_ensure(vbuf * vb, int want);
int adns__vbuf_appendstr(vbuf * vb, const char *adata);	/* does not include nul */
int adns__vbuf_append(vbuf * vb, const byte * adata, int len);
/* 1=>success, 0=>realloc failed */
void adns__vbuf_appendq(vbuf * vb, const byte * adata, int len);
void adns__vbuf_init(vbuf * vb);
void adns__vbuf_free(vbuf * vb);

const char *adns__diag_domain(adns_state ads, int serv, adns_query qu,
			      vbuf *vb, const byte *dgram, int dglen, int cbyte);

void adns__isort(void *array, int nobjs, int sz, void *tempbuf,
		 int (*needswap)(void *context, const void *a, const void *b),
		 void *context);

adns_status adns__mkquery(adns_state ads, vbuf * vb, int *id_r,
			  const char *owner, int ol,
			  const typeinfo * typei, adns_queryflags flags);

adns_status adns__mkquery_frdgram(adns_state ads, vbuf * vb, int *id_r,
				  const byte *qd_dgram, int qd_dglen, int qd_begin,
				  adns_rrtype type, adns_queryflags flags);

void adns__querysend_tcp(adns_query qu, struct timeval now);

void adns__query_send(adns_query qu, struct timeval now);

adns_status adns__internal_submit(adns_state ads, adns_query * query_r,
				  const typeinfo *typei, vbuf *qumsg_vb, int id,
				  adns_queryflags flags, struct timeval now,
				  const qcontext * ctx);

void adns__search_next(adns_state ads, adns_query qu, struct timeval now);
void *adns__alloc_interim(adns_query qu, size_t sz);
void *adns__alloc_preserved(adns_query qu, size_t sz);
void adns__transfer_interim(adns_query from, adns_query to, void *block, size_t sz);
void *adns__alloc_mine(adns_query qu, size_t sz);
void *adns__alloc_final(adns_query qu, size_t sz);
void adns__makefinal_block(adns_query qu, void **blpp, size_t sz);
void adns__makefinal_str(adns_query qu, char **strp);
void adns__reset_preserved(adns_query qu);
void adns__query_done(adns_query qu);
void adns__query_fail(adns_query qu, adns_status xstat);

void adns__procdgram(adns_state ads, const byte * dgram, int len,
		     int serv, int viatcp, struct timeval now);

const typeinfo *adns__findtype(adns_rrtype type);

typedef struct {
	adns_state ads;
	adns_query qu;
	int serv;
	const byte *dgram;
	int dglen, max, cbyte, namelen;
	int *dmend_r;
} findlabel_state;

void adns__findlabel_start(findlabel_state * afls, adns_state ads,
			   int serv, adns_query qu,
			   const byte * dgram, int dglen, int max,
			   int dmbegin, int *dmend_rlater);

adns_status adns__findlabel_next(findlabel_state *afls, int *lablen_r, int *labstart_r);

typedef enum {
	pdf_quoteok = 0x001
} parsedomain_flags;

adns_status adns__parse_domain(adns_state ads, int serv, adns_query qu,
			       vbuf * vb, adns_queryflags flags,
			       const byte * dgram, int dglen, int *cbyte_io, int max);

adns_status adns__parse_domain_more(findlabel_state * afls, adns_state ads,
				    adns_query qu, vbuf *vb, parsedomain_flags flags,
				    const byte * dgram);

adns_status adns__findrr(adns_query qu, int serv,
			 const byte * dgram, int dglen, int *cbyte_io,
			 int *type_r, int *class_r, unsigned long *ttl_r,
			 int *rdlen_r, int *rdstart_r,
			 int *ownermatchedquery_r);

adns_status adns__findrr_anychk(adns_query qu, int serv,
				const byte *dgram, int dglen, int *cbyte_io,
				int *type_r, int *class_r, unsigned long *ttl_r,
				int *rdlen_r, int *rdstart_r,
				const byte *eo_dgram, int eo_dglen, int eo_cbyte,
				int *eo_matched_r);

void adns__update_expires(adns_query qu, unsigned long ttl, struct timeval now);

int vbuf__append_quoted1035(vbuf * vb, const byte * buf, int len);

void adns__tcp_broken(adns_state ads, const char *what, const char *why);

void adns__tcp_tryconnect(adns_state ads, struct timeval now);

void adns__autosys(adns_state ads, struct timeval now);

void adns__must_gettimeofday(adns_state ads, const struct timeval **now_io,
			     struct timeval *tv_buf);

int adns__pollfds(adns_state ads, struct pollfd pollfds_buf[MAX_POLLFDS]);
void adns__fdevents(adns_state ads,
		    const struct pollfd *pollfds, int npollfds,
		    int maxfd, const fd_set * readfds,
		    const fd_set * writefds, const fd_set * exceptfds,
		    struct timeval now, int *r_r);
int adns__internal_check(adns_state ads,
			 adns_query * query_io,
			 adns_answer **answer,
			 void **context_r);

void adns__timeouts(adns_state ads, int act,
		    struct timeval **tv_io, struct timeval *tvbuf,
		    struct timeval now);

void adns__consistency(adns_state ads, adns_query qu, consistency_checks cc);

#define MEM_ROUND(sz) \
  (( ((sz)+sizeof(union maxalign)-1) / sizeof(union maxalign) ) \
   * sizeof(union maxalign) )

#define GETIL_B(cb) (((dgram)[(cb)++]) & 0x0ff)
#define GET_B(cb,tv) ((tv)= GETIL_B((cb)))
#define GET_W(cb,tv) ((tv)=0, (tv)|=(GETIL_B((cb))<<8), (tv)|=GETIL_B(cb), (tv))
#define GET_L(cb,tv) ( (tv)=0, \
		       (tv)|=(GETIL_B((cb))<<24), \
		       (tv)|=(GETIL_B((cb))<<16), \
		       (tv)|=(GETIL_B((cb))<<8), \
		       (tv)|=GETIL_B(cb), \
		       (tv) )

#endif

#endif
