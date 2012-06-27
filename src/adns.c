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
 * 
 * 
 *
 */

#include "denora.h"

#ifndef _WIN32

/* *INDENT-OFF* */

#define MKQUERY_START(vb) (rqp= (vb)->buf+(vb)->used)
#define MKQUERY_ADDB(b) *rqp++= (b)
#define MKQUERY_ADDW(w) (MKQUERY_ADDB(((w)>>8)&0x0ff), MKQUERY_ADDB((w)&0x0ff))
#define MKQUERY_STOP(vb) ((vb)->used= rqp-(vb)->buf)

static void ccf_nameserver(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_search(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_sortlist(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_options(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_clearnss(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_include(adns_state ads, const char *fn, int lno, const char *buf);
static void ccf_lookup(adns_state ads, const char *fn, int lno, const char *buf);
static void configparseerr(adns_state ads, const char *fn, int lno, const char *fmt, ...);
static int nextword(const char **bufp_io, const char **word_r, int *l_r);

static const struct configcommandinfo {
  const char *name;
  void (*fn)(adns_state ads, const char *fn, int lno, const char *buf);
} configcommandinfos[]= {
  { "nameserver",        ccf_nameserver  },
  { "domain",            ccf_search      },
  { "search",            ccf_search      },
  { "sortlist",          ccf_sortlist    },
  { "options",           ccf_options     },
  { "clearnameservers",  ccf_clearnss    },
  { "include",           ccf_include     },
  { "lookup",            ccf_lookup      }, /* OpenBSD */
  {  NULL,               NULL            }
};

typedef union {
    FILE *file;
    const char *text;
} getline_ctx;

static void ccf_lookup(adns_state ads, const char *fn, int lno,
		       const char *buf) {
  int found_bind=0;
  const char *word;
  int l;

  if (!*buf) {
    configparseerr(ads,fn,lno,"`lookup' directive with no databases");
    return;
  }

  while (nextword(&buf,&word,&l)) {
    if (l==4 && !memcmp(word,"bind",4)) {
      found_bind=1;
    } else if (l==4 && !memcmp(word,"file",4)) {
      /* ignore this and hope /etc/hosts is not essential */
    } else if (l==2 && !memcmp(word,"yp",2)) {
      alog(LOG_ADNS,"%s:%d: yp lookups not supported by adns", fn,lno);
      found_bind=-1;
    } else {
      alog(LOG_ADNS,"%s:%d: unknown `lookup' database `%.*s'",
		 fn,lno, l,word);
      found_bind=-1;
    }
  }
  if (!found_bind)
    alog(LOG_ADNS,"%s:%d: `lookup' specified, but not `bind'", fn,lno);
}

static DENORA_INLINE void timevaladd(struct timeval *tv_io, long ms)
{
    struct timeval tmp;
    assert(ms >= 0);
    tmp = *tv_io;
    tmp.tv_usec += (ms % 1000) * 1000;
    tmp.tv_sec += ms / 1000;
    if (tmp.tv_usec >= 1000000) {
        tmp.tv_sec++;
        tmp.tv_usec -= 1000000;
    }
    *tv_io = tmp;
}

static DENORA_INLINE int ctype_whitespace(int c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

static DENORA_INLINE int ctype_digit(int c)
{
    return c >= '0' && c <= '9';
}

static DENORA_INLINE int ctype_alpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static DENORA_INLINE int ctype_822special(int c)
{
    return strchr("()<>@,;:\\\".[]", c) != 0;
}

static DENORA_INLINE int ctype_domainunquoted(int c)
{
    return ctype_alpha(c) || ctype_digit(c)
        || (strchr("-_/+", c) != 0);
}

static DENORA_INLINE int errno_resources(int e)
{
    return e == ENOMEM || e == SOCKERR_ENOBUFS;
}

void adns__vbuf_init(vbuf * vb)
{
    vb->used = vb->avail = 0;
    vb->buf = 0;
}

int adns__vbuf_ensure(vbuf * vb, int want)
{
    void *nb;

    if (vb->avail >= want)
        return 1;
    nb = realloc(vb->buf, want);
    if (!nb)
        return 0;
    vb->buf = nb;
    vb->avail = want;
    return 1;
}

void adns__vbuf_appendq(vbuf * vb, const byte * adata, int len)
{
    memcpy(vb->buf + vb->used, adata, len);
    vb->used += len;
}

int adns__vbuf_append(vbuf * vb, const byte * adata, int len)
{
    int newlen;
    void *nb;

    newlen = vb->used + len;
    if (vb->avail < newlen) {
        if (newlen < 20)
            newlen = 20;
        newlen <<= 1;
        nb = realloc(vb->buf, newlen);
        if (!nb) {
            newlen = vb->used + len;
            nb = realloc(vb->buf, newlen);
        }
        if (!nb)
            return 0;
        vb->buf = nb;
        vb->avail = newlen;
    }
    adns__vbuf_appendq(vb, adata, len);
    return 1;
}

int adns__vbuf_appendstr(vbuf * vb, const char *adata)
{
    int l;
    l = strlen(adata);
    return adns__vbuf_append(vb, (const byte *) adata, l);
}

void adns__vbuf_free(vbuf * vb)
{
    if (vb->buf)
        free(vb->buf);
    adns__vbuf_init(vb);
}

/* Additional diagnostic functions */

const char *adns__diag_domain(adns_state ads, int serv, adns_query qu,
                              vbuf * vb, const byte * dgram, int dglen,
                              int cbyte)
{
    adns_status st;

    st = adns__parse_domain(ads, serv, qu, vb, pdf_quoteok, dgram,
                            dglen, &cbyte, dglen);
    if (st == adns_s_nomemory) {
        return "<cannot report domain... out of memory>";
    }
    if (st) {
        vb->used = 0;
        if (!(adns__vbuf_appendstr(vb, "<bad format... ") &&
              adns__vbuf_appendstr(vb, adns_strerror(st)) &&
              adns__vbuf_appendstr(vb, ">") &&
              adns__vbuf_append(vb, (const byte *) "", 1))) {
            return "<cannot report bad format... out of memory>";
        }
    }
    if (!vb->used) {
        adns__vbuf_appendstr(vb, "<truncated ...>");
        adns__vbuf_append(vb, (const byte *) "", 1);
    }
    return (const char *) vb->buf;
}

adns_status adns_rr_info(adns_rrtype type,
                         const char **rrtname_r, const char **fmtname_r,
                         int *len_r, const void *datap, char **data_r)
{
    const typeinfo *typei;
    vbuf vb;
    adns_status st;

    typei = adns__findtype(type);
    if (!typei)
        return adns_s_unknownrrtype;

    if (rrtname_r)
        *rrtname_r = typei->rrtname;
    if (fmtname_r)
        *fmtname_r = typei->fmtname;
    if (len_r)
        *len_r = typei->rrsz;

    if (!datap)
        return adns_s_ok;

    adns__vbuf_init(&vb);
    st = typei->convstring(&vb, datap);
    if (st)
        goto x_freevb;
    if (!adns__vbuf_append(&vb, (const byte *) "", 1)) {
        st = adns_s_nomemory;
        goto x_freevb;
    }
    assert((int) strlen((char *) vb.buf) == vb.used - 1);
    *data_r = realloc(vb.buf, vb.used);
    if (!*data_r)
        *data_r = (char *) vb.buf;
    return adns_s_ok;

  x_freevb:
    adns__vbuf_free(&vb);
    return st;
}


#define SINFO(n,s) { adns_s_##n, #n, s }

static const struct sinfo {
    adns_status st;
    const char *abbrev;
    const char *string;
} sinfos[] = {
    SINFO(ok, "OK"),
        SINFO(nomemory, "Out of memory"),
        SINFO(unknownrrtype, "Query not implemented in DNS library"),
        SINFO(systemfail, "General resolver or system failure"),
        SINFO(timeout, "DNS query timed out"),
        SINFO(allservfail, "All nameservers failed"),
        SINFO(norecurse, "Recursion denied by nameserver"),
        SINFO(invalidresponse, "Nameserver sent bad response"),
        SINFO(unknownformat, "Nameserver used unknown format"),
        SINFO(rcodeservfail, "Nameserver reports failure"),
        SINFO(rcodeformaterror, "Query not understood by nameserver"),
        SINFO(rcodenotimplemented, "Query not implemented by nameserver"),
        SINFO(rcoderefused, "Query refused by nameserver"),
        SINFO(rcodeunknown, "Nameserver sent unknown response code"),
        SINFO(inconsistent, "Inconsistent resource records in DNS"),
        SINFO(prohibitedcname,
              "DNS alias found where canonical name wanted"),
        SINFO(answerdomaininvalid,
              "Found syntactically invalid domain name"),
        SINFO(answerdomaintoolong, "Found overly-long domain name"),
        SINFO(invaliddata, "Found invalid DNS data"),
        SINFO(querydomainwrong,
              "Domain invalid for particular DNS query type"),
        SINFO(querydomaininvalid, "Domain name is syntactically invalid"),
        SINFO(querydomaintoolong, "Domain name or component is too long"),
        SINFO(nxdomain, "No such domain"), SINFO(nodata, "No such data")
};

static int si_compar(const void *key, const void *elem)
{
    const adns_status *st = key;
    const struct sinfo *si = elem;

    return *st < si->st ? -1 : *st > si->st ? 1 : 0;
}

static const struct sinfo *findsinfo(adns_status st)
{
    return bsearch(&st, sinfos, sizeof(sinfos) / sizeof(*sinfos),
                   sizeof(*sinfos), si_compar);
}

const char *adns_strerror(adns_status st)
{
    const struct sinfo *si;

    si = findsinfo(st);
    return si->string;
}

#define STINFO(max) { adns_s_max_##max, #max }

static const struct stinfo {
    adns_status stmax;
    const char *abbrev;
} stinfos[] = {
    {
    adns_s_ok, "ok"},
        STINFO(localfail),
        STINFO(remotefail),
        STINFO(tempfail),
        STINFO(misconfig), STINFO(misquery), STINFO(permfail)
};

static int sti_compar(const void *key, const void *elem)
{
    const adns_status *st = key;
    const struct stinfo *sti = elem;

    adns_status here, min, max;

    here = *st;
    min = (sti == stinfos) ? 0 : sti[-1].stmax + 1;
    max = sti->stmax;

    return here < min ? -1 : here > max ? 1 : 0;
}

const char *adns_errtypeabbrev(adns_status st)
{
    const struct stinfo *sti;

    sti = bsearch(&st, stinfos, sizeof(stinfos) / sizeof(*stinfos),
                  sizeof(*stinfos), sti_compar);
    return sti->abbrev;
}


void adns__isort(void *array, int nobjs, int sz, void *tempbuf,
                 int (*needswap) (void *context, const void *a,
                                  const void *b), void *context)
{
    byte *adata = array;
    int i, place;

    for (i = 0; i < nobjs; i++) {
        for (place = i;
             place > 0
             && needswap(context, adata + (place - 1) * sz,
                         adata + i * sz); place--);
        if (place != i) {
            memcpy(tempbuf, adata + i * sz, sz);
            memmove(adata + (place + 1) * sz, adata + place * sz,
                    (i - place) * sz);
            memcpy(adata + place * sz, tempbuf, sz);
        }
    }
}

void adns_checkconsistency(adns_state ads, adns_query qu)
{
    adns__consistency(ads, qu, cc_user);
}

#define DLIST_CHECK1(list, nodevar, body)					\
  if ((list).head) {								\
    assert(! (list).head->back);						\
    for ((nodevar)= (list).head; (nodevar); (nodevar)= (nodevar)->next) {	\
      assert((nodevar)->next						\
	     ? (nodevar) == (nodevar)->next->back			\
	     : (nodevar) == (list).tail);					\
      body									\
    }										\
  }

#define DLIST_CHECK2(list, nodevar, part, body)					\
  if ((list).head) {								\
    assert(! (list).head->part.back);						\
    for ((nodevar)= (list).head; (nodevar); (nodevar)= (nodevar)->part.next) {	\
      assert((nodevar)->part.next						\
	     ? (nodevar) == (nodevar)->part.next->part.back			\
	     : (nodevar) == (list).tail);					\
      body									\
    }										\
  }

#define DLIST_ASSERTON(node, nodevar, list, part)				\
  do {										\
    for ((nodevar)= (list).head;						\
	 (nodevar) != (node);							\
	 (nodevar)= (nodevar)->part next) {					\
      assert((nodevar));							\
    }										\
  } while(0)

static void checkc_query_alloc(__attribute__((unused))adns_state ads, adns_query qu)
{
    allocnode *an;

    DLIST_CHECK1(qu->allocations, an, {
                 });
}

static void checkc_query(adns_state ads, adns_query qu)
{
    adns_query child;

    assert(qu->udpnextserver < ads->nservers);
    assert(!(qu->udpsent & (~0UL << ads->nservers)));
    assert(qu->search_pos <= ads->nsearchlist);
    if (qu->parent)
        DLIST_ASSERTON(qu, child, qu->parent->children, siblings.);
}

static void checkc_notcpbuf(adns_state ads)
{
    assert(!ads->tcpsend.used);
    assert(!ads->tcprecv.used);
    assert(!ads->tcprecv_skip);
}

static void checkc_global(adns_state ads)
{
    int i;

    assert(ads->udpsocket >= 0);

    for (i = 0; i < ads->nsortlist; i++)
        assert(!
               (ads->sortlist[i].base.s_addr & ~ads->sortlist[i].mask.
                s_addr));

    assert(ads->tcpserver >= 0 && ads->tcpserver < ads->nservers);

    switch (ads->tcpstate) {
    case server_connecting:
        assert(ads->tcpsocket >= 0);
        checkc_notcpbuf(ads);
        break;
    case server_disconnected:
    case server_broken:
        assert(ads->tcpsocket == -1);
        checkc_notcpbuf(ads);
        break;
    case server_ok:
        assert(ads->tcpsocket >= 0);
        assert(ads->tcprecv_skip <= ads->tcprecv.used);
        break;
    default:
        assert(!"ads->tcpstate value");
    }

    assert(ads->searchlist || !ads->nsearchlist);
}

static void checkc_queue_udpw(adns_state ads)
{
    adns_query qu;

    DLIST_CHECK1(ads->udpw, qu, {
                 assert(qu->state == query_tosend);
                 assert(qu->retries <= UDPMAXRETRIES);
                 assert(qu->udpsent);
                 assert(!qu->children.head && !qu->children.tail);
                 checkc_query(ads, qu);
                 checkc_query_alloc(ads, qu);
                 });
}

static void checkc_queue_tcpw(adns_state ads)
{
    adns_query qu;

    DLIST_CHECK1(ads->tcpw, qu, {
                 assert(qu->state == query_tcpw);
                 assert(!qu->children.head && !qu->children.tail);
                 assert(qu->retries <= ads->nservers + 1);
                 checkc_query(ads, qu);
                 checkc_query_alloc(ads, qu);
                 });
}

static void checkc_queue_childw(adns_state ads)
{
    adns_query parent, child;

    DLIST_CHECK1(ads->childw, parent, {
                 assert(parent->state == query_childw);
                 assert(parent->children.head);
                 DLIST_CHECK2(parent->children, child, siblings, {
                              assert(child->parent == parent);
                              assert(child->state != query_done);
                              });
                 checkc_query(ads, parent);
                 checkc_query_alloc(ads, parent);
                 });
}

static void checkc_queue_output(adns_state ads)
{
    adns_query qu;

    DLIST_CHECK1(ads->output, qu, {
                 assert(qu->state == query_done);
                 assert(!qu->children.head && !qu->children.tail);
                 assert(!qu->parent);
                 assert(!qu->allocations.head && !qu->allocations.tail);
                 checkc_query(ads, qu);
                 });
}

void adns__consistency(adns_state ads, adns_query qu,
                       consistency_checks cc)
{
    adns_query search;

    switch (cc) {
    case cc_user:
        break;
    case cc_entex:
        if (!(ads->iflags & adns_if_checkc_entex))
            return;
        break;
    case cc_freq:
        if ((ads->iflags & adns_if_checkc_freq) != adns_if_checkc_freq)
            return;
        break;
    default:
        abort();
    }

    checkc_global(ads);
    checkc_queue_udpw(ads);
    checkc_queue_tcpw(ads);
    checkc_queue_childw(ads);
    checkc_queue_output(ads);

    if (qu) {
        switch (qu->state) {
        case query_tosend:
            DLIST_ASSERTON(qu, search, ads->udpw,);
            break;
        case query_tcpw:
            DLIST_ASSERTON(qu, search, ads->tcpw,);
            break;
        case query_childw:
            DLIST_ASSERTON(qu, search, ads->childw,);
            break;
        case query_done:
            DLIST_ASSERTON(qu, search, ads->output,);
            break;
        default:
            assert(!"specific query state");
        }
    }
}

static adns_status mkquery_header(adns_state ads, vbuf * vb, int *id_r,
                                  int qdlen)
{
    int id;
    byte *rqp;

    if (!adns__vbuf_ensure(vb, DNS_HDRSIZE + qdlen + 4))
        return adns_s_nomemory;

    vb->used = 0;
    MKQUERY_START(vb);

    *id_r = id = (ads->nextid++) & 0x0ffff;
    MKQUERY_ADDW(id);
    MKQUERY_ADDB(0x01);         /* QR=Q(0), OPCODE=QUERY(0000), !AA, !TC, RD */
    MKQUERY_ADDB(0x00);         /* !RA, Z=000, RCODE=NOERROR(0000) */
    MKQUERY_ADDW(1);            /* QDCOUNT=1 */
    MKQUERY_ADDW(0);            /* ANCOUNT=0 */
    MKQUERY_ADDW(0);            /* NSCOUNT=0 */
    MKQUERY_ADDW(0);            /* ARCOUNT=0 */

    MKQUERY_STOP(vb);

    return adns_s_ok;
}

static adns_status mkquery_footer(vbuf * vb, adns_rrtype type)
{
    byte *rqp;

    MKQUERY_START(vb);
    MKQUERY_ADDW(type & adns__rrt_typemask);    /* QTYPE */
    MKQUERY_ADDW(DNS_CLASS_IN); /* QCLASS=IN */
    MKQUERY_STOP(vb);
    assert(vb->used <= vb->avail);

    return adns_s_ok;
}

adns_status adns__mkquery(adns_state ads, vbuf * vb, int *id_r,
                          const char *owner, int ol,
                          const typeinfo * typei, adns_queryflags flags)
{
    int ll, c, nbytes;
    byte label[255], *rqp;
    const char *p, *pe;
    adns_status st;

    st = mkquery_header(ads, vb, id_r, ol + 2);
    if (st)
        return st;

    MKQUERY_START(vb);

    p = owner;
    pe = owner + ol;
    nbytes = 0;
    while (p != pe) {
        ll = 0;
        while (p != pe && (c = *p++) != '.') {
            if (c == '\\') {
                if (!(flags & adns_qf_quoteok_query))
                    return adns_s_querydomaininvalid;
                if (ctype_digit(p[0])) {
                    if (ctype_digit(p[1])
                        && ctype_digit(p[2])) {
                        c = (p[0] - '0') * 100 +
                            (p[1] - '0') * 10 + (p[2] - '0');
                        p += 3;
                        if (c >= 256)
                            return adns_s_querydomaininvalid;
                    } else {
                        return adns_s_querydomaininvalid;
                    }
                } else if (!(c = *p++)) {
                    return adns_s_querydomaininvalid;
                }
            }
            if (!(flags & adns_qf_quoteok_query)) {
                if (c == '-') {
                    if (!ll)
                        return adns_s_querydomaininvalid;
                } else if (!ctype_alpha(c)
                           && !ctype_digit(c)) {
                    return adns_s_querydomaininvalid;
                }
            }
            if (ll == sizeof(label))
                return adns_s_querydomaininvalid;
            label[ll++] = c;
        }
        if (!ll)
            return adns_s_querydomaininvalid;
        if (ll > DNS_MAXLABEL)
            return adns_s_querydomaintoolong;
        nbytes += ll + 1;
        if (nbytes >= DNS_MAXDOMAIN)
            return adns_s_querydomaintoolong;
        MKQUERY_ADDB(ll);
        memcpy(rqp, label, ll);
        rqp += ll;
    }
    MKQUERY_ADDB(0);

    MKQUERY_STOP(vb);

    st = mkquery_footer(vb, typei->type);

    return adns_s_ok;
}

adns_status adns__mkquery_frdgram(adns_state ads, vbuf * vb, int *id_r,
                                  const byte * qd_dgram, int qd_dglen,
                                  int qd_begin, adns_rrtype type,
                                  __attribute__((unused))adns_queryflags flags)
{
    byte *rqp;
    findlabel_state afls;
    int lablen;
    int labstart = 0;
    adns_status st;

    st = mkquery_header(ads, vb, id_r, qd_dglen);
    if (st) {
        return st;
    }

    MKQUERY_START(vb);

    adns__findlabel_start(&afls, ads, -1, 0, qd_dgram, qd_dglen,
                          qd_dglen, qd_begin, 0);
    for (;;) {
        st = adns__findlabel_next(&afls, &lablen, &labstart);
        assert(!st);
        if (!lablen)
            break;
        assert(lablen < 255);
        MKQUERY_ADDB(lablen);
        memcpy(rqp, qd_dgram + labstart, lablen);
        rqp += lablen;
    }
    MKQUERY_ADDB(0);

    MKQUERY_STOP(vb);

    st = mkquery_footer(vb, type);

    return adns_s_ok;
}

void adns__querysend_tcp(adns_query qu, __attribute__((unused))struct timeval now)
{
    byte length[2];
#ifndef WIN32
    struct iovec iov[2];
#endif
    int wr, r;
    adns_state ads;

    if (qu->ads->tcpstate != server_ok)
        return;

    assert(qu->state == query_tcpw);

    length[0] = (qu->query_dglen & 0x0ff00U) >> 8;
    length[1] = (qu->query_dglen & 0x0ff);

    ads = qu->ads;
    if (!adns__vbuf_ensure
        (&ads->tcpsend, ads->tcpsend.used + qu->query_dglen + 2))
        return;

    qu->retries++;

    /* Reset idle timeout. */
    ads->tcptimeout.tv_sec = ads->tcptimeout.tv_usec = 0;

    if (ads->tcpsend.used) {
        wr = 0;
    } else {
#ifdef WIN32
        char *buf = NULL;

        buf = (char *) malloc((2 + qu->query_dglen));
        memcpy(buf, length, 2);
        memcpy((buf + 2), qu->query_dgram, qu->query_dglen);
        wr = deno_sockwrite(qu->ads->tcpsocket, buf,
                            (2 + qu->query_dglen));
        errno = deno_sockgeterr();
        free(buf);
#else
        iov[0].iov_base = length;
        iov[0].iov_len = 2;
        iov[1].iov_base = qu->query_dgram;
        iov[1].iov_len = qu->query_dglen;
        wr = writev(qu->ads->tcpsocket, iov, 2);
#endif
        if (wr < 0) {
            if (!
                (errno == EAGAIN || errno == SOCKERR_EINTR
                 || errno == ENOSPC || errno == SOCKERR_ENOBUFS
                 || errno == ENOMEM)) {
                adns__tcp_broken(ads, "write", strerror(errno));
                return;
            }
            wr = 0;
        }
    }

    if (wr < 2) {
        r = adns__vbuf_append(&ads->tcpsend, length, 2 - wr);
        assert(r);
        wr = 0;
    } else {
        wr -= 2;
    }
    if (wr < qu->query_dglen) {
        r = adns__vbuf_append(&ads->tcpsend, qu->query_dgram + wr,
                              qu->query_dglen - wr);
        assert(r);
    }
}

static void query_usetcp(adns_query qu, struct timeval now)
{
    qu->state = query_tcpw;
    qu->timeout = now;
    timevaladd(&qu->timeout, TCPWAITMS);
    ALIST_LINK_TAIL(qu->ads->tcpw, qu);
    adns__querysend_tcp(qu, now);
    adns__tcp_tryconnect(qu->ads, now);
}

void adns__query_send(adns_query qu, struct timeval now)
{
    struct sockaddr_in servaddr;
    int serv, r;
    adns_state ads;

    assert(qu->state == query_tosend);
    if ((qu->flags & adns_qf_usevc) || (qu->query_dglen > DNS_MAXUDP)) {
        query_usetcp(qu, now);
        return;
    }
    if (qu->retries >= UDPMAXRETRIES) {
        adns__query_fail(qu, adns_s_timeout);
        return;
    }

    serv = qu->udpnextserver;
    memset(&servaddr, 0, sizeof(servaddr));

    ads = qu->ads;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = ads->servers[serv].addr;
    servaddr.sin_port = htons(DNS_PORT);

    r = sendto(ads->udpsocket, (char *) qu->query_dgram, qu->query_dglen,
               0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    if (r < 0 && deno_sockgeterr() == SOCKERR_EMSGSIZE) {
        qu->retries = 0;
        query_usetcp(qu, now);
        return;
    }
    if (r < 0 && deno_sockgeterr() != SOCKERR_EAGAIN) {
        alog(LOG_ADNS, "sendto failed: %s", deno_sockgetstrerr());
    }

    qu->timeout = now;
    timevaladd(&qu->timeout, UDPRETRYMS);
    qu->udpsent |= (1 << serv);
    qu->udpnextserver = (serv + 1) % ads->nservers;
    qu->retries++;
    ALIST_LINK_TAIL(ads->udpw, qu);
}

static void tcp_close(adns_state ads)
{
    int serv;

    serv = ads->tcpserver;

    deno_sockclose(ads->tcpsocket);
    ads->tcpsocket = -1;
    ads->tcprecv.used = ads->tcprecv_skip = ads->tcpsend.used = 0;
}

void adns__tcp_broken(adns_state ads, const char *what, const char *why)
{
    int serv;
    adns_query qu;

    assert(ads->tcpstate == server_connecting
           || ads->tcpstate == server_ok);
    serv = ads->tcpserver;

    if (what) {
        alog(LOG_DEBUG, "TCP connection failed: %s: %s", what, why);
    }

    if (ads->tcpstate == server_connecting) {
        /* Counts as a retry for all the queries waiting for TCP. */
        for (qu = ads->tcpw.head; qu; qu = qu->next) {
            qu->retries++;
        }
    }

    tcp_close(ads);
    ads->tcpstate = server_broken;
    ads->tcpserver = (serv + 1) % ads->nservers;
}

static void tcp_connected(adns_state ads, struct timeval now)
{
    adns_query qu, nqu;

    alog(LOG_ADNS, "ADNS: TCP connected");
    ads->tcpstate = server_ok;

    for (qu = ads->tcpw.head; qu && ads->tcpstate == server_ok; qu = nqu) {
        nqu = qu->next;
        assert(qu->state == query_tcpw);
        adns__querysend_tcp(qu, now);
    }
}

void adns__tcp_tryconnect(adns_state ads, struct timeval now)
{
    int r, tries;
    deno_socket_t fd;
    struct sockaddr_in addr;
    struct protoent *proto;

    for (tries = 0; tries < ads->nservers; tries++) {
        switch (ads->tcpstate) {
        case server_connecting:
        case server_ok:
        case server_broken:
            return;
        case server_disconnected:
            break;
        default:
            abort();
        }

        assert(!ads->tcpsend.used);
        assert(!ads->tcprecv.used);
        assert(!ads->tcprecv_skip);

        proto = getprotobyname("tcp");
        if (!proto) {
            alog(LOG_ADNS, "ADNS: unable to find protocol no. for TCP !");
            return;
        }
        fd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
        if (fd < 0) {
            alog(LOG_ADNS, "ADNS: cannot create TCP socket: %s",
                 deno_sockgetstrerr());
            return;
        }
        if (deno_socksetnonb(fd) < 0) {
            alog(LOG_ADNS, "ADNS: cannot make TCP socket nonblocking: %s",
                 deno_sockgetstrerr());
            deno_sockclose(fd);
            return;
        }
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(DNS_PORT);
        addr.sin_addr = ads->servers[ads->tcpserver].addr;
        r = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
        ads->tcpsocket = fd;
        ads->tcpstate = server_connecting;
        if (r == 0) {
            tcp_connected(ads, now);
            return;
        }
        if (deno_sockgeterr() == SOCKERR_EWOULDBLOCK
            && deno_sockgeterr() == SOCKERR_EINPROGRESS) {
            ads->tcptimeout = now;
            timevaladd(&ads->tcptimeout, TCPCONNMS);
            return;
        }
        adns__tcp_broken(ads, "connect", deno_sockgetstrerr());
        ads->tcpstate = server_disconnected;
    }
}

/* Timeout handling functions. */

void adns__must_gettimeofday(adns_state ads, const struct timeval **now_io,
                             struct timeval *tv_buf)
{
    const struct timeval *now;
    int r;

    now = *now_io;
    if (now)
        return;
    r = gettimeofday(tv_buf, 0);
    if (!r) {
        *now_io = tv_buf;
        return;
    }
    alog(LOG_ADNS, "ADNS: gettimeofday failed: %s", strerror(errno));
    adns_globalsystemfailure(ads);
    return;
}

static void inter_immed(struct timeval **tv_io, struct timeval *tvbuf)
{
    struct timeval *rbuf;

    if (!tv_io)
        return;

    rbuf = *tv_io;
    if (!rbuf) {
        *tv_io = rbuf = tvbuf;
    }

    timerclear(rbuf);
}

static void inter_maxto(struct timeval **tv_io, struct timeval *tvbuf,
                        struct timeval maxto)
{
    struct timeval *rbuf;

    if (!tv_io)
        return;
    rbuf = *tv_io;
    if (!rbuf) {
        *tvbuf = maxto;
        *tv_io = tvbuf;
    } else {
        if (timercmp(rbuf, &maxto, >))
            *rbuf = maxto;
    }
/*fprintf(stderr,"inter_maxto maxto=%ld.%06ld result=%ld.%06ld\n",
	maxto.tv_sec,maxto.tv_usec,(**tv_io).tv_sec,(**tv_io).tv_usec);*/
}

static void inter_maxtoabs(struct timeval **tv_io, struct timeval *tvbuf,
                           struct timeval now, struct timeval maxtime)
{
    /* tv_io may be 0 */
    ldiv_t dr;

/*fprintf(stderr,"inter_maxtoabs now=%ld.%06ld maxtime=%ld.%06ld\n",
	now.tv_sec,now.tv_usec,maxtime.tv_sec,maxtime.tv_usec);*/
    if (!tv_io)
        return;
    maxtime.tv_sec -= (now.tv_sec + 2);
    maxtime.tv_usec -= (now.tv_usec - 2000000);
    dr = ldiv(maxtime.tv_usec, 1000000);
    maxtime.tv_sec += dr.quot;
    maxtime.tv_usec -= dr.quot * 1000000;
    if (maxtime.tv_sec < 0)
        timerclear(&maxtime);
    inter_maxto(tv_io, tvbuf, maxtime);
}

static void timeouts_queue(__attribute__((unused))adns_state ads, int act, struct timeval **tv_io,
                           struct timeval *tvbuf, struct timeval now,
                           struct query_queue *queue)
{
    adns_query qu, nqu;

    for (qu = queue->head; qu; qu = nqu) {
        nqu = qu->next;
        if (!timercmp(&now, &qu->timeout, >)) {
            inter_maxtoabs(tv_io, tvbuf, now, qu->timeout);
        } else {
            if (!act) {
                inter_immed(tv_io, tvbuf);
                return;
            }
            ALIST_UNLINK(*queue, qu);
            if (qu->state != query_tosend)
                adns__query_fail(qu, adns_s_timeout);
            else
                adns__query_send(qu, now);
            nqu = queue->head;
        }
    }
}


static void tcp_events(adns_state ads, int act, struct timeval **tv_io,
                       struct timeval *tvbuf, struct timeval now)
{
    adns_query qu, nqu;

    for (;;) {
        switch (ads->tcpstate) {
        case server_broken:
            if (!act) {
                inter_immed(tv_io, tvbuf);
                return;
            }
            for (qu = ads->tcpw.head; qu; qu = nqu) {
                nqu = qu->next;
                assert(qu->state == query_tcpw);
                if (qu->retries > ads->nservers) {
                    ALIST_UNLINK(ads->tcpw, qu);
                    adns__query_fail(qu, adns_s_allservfail);
                }
            }
            ads->tcpstate = server_disconnected;
        case server_disconnected:      /* fall through */
            if (!ads->tcpw.head)
                return;
            if (!act) {
                inter_immed(tv_io, tvbuf);
                return;
            }
            adns__tcp_tryconnect(ads, now);
            break;
        case server_ok:
            if (ads->tcpw.head)
                return;
            if (!ads->tcptimeout.tv_sec) {
                assert(!ads->tcptimeout.tv_usec);
                ads->tcptimeout = now;
                timevaladd(&ads->tcptimeout, TCPIDLEMS);
            }
        case server_connecting:        /* fall through */
            if (!act || !timercmp(&now, &ads->tcptimeout, >)) {
                inter_maxtoabs(tv_io, tvbuf, now, ads->tcptimeout);
                return;
            }
            {
                /* TCP timeout has happened */
                switch (ads->tcpstate) {
                case server_connecting:        /* failed to connect */
                    adns__tcp_broken(ads,
                                     "unable to make connection",
                                     "timed out");
                    break;
                case server_ok:        /* idle timeout */
                    tcp_close(ads);
                    ads->tcpstate = server_disconnected;
                    return;
                default:
                    abort();
                }
            }
            break;
        default:
            abort();
        }
    }
    return;
}

void adns__timeouts(adns_state ads, int act, struct timeval **tv_io,
                    struct timeval *tvbuf, struct timeval now)
{
    timeouts_queue(ads, act, tv_io, tvbuf, now, &ads->udpw);
    timeouts_queue(ads, act, tv_io, tvbuf, now, &ads->tcpw);
    tcp_events(ads, act, tv_io, tvbuf, now);
}

void adns_firsttimeout(adns_state ads, struct timeval **tv_io,
                       struct timeval *tvbuf, struct timeval now)
{
    adns__consistency(ads, 0, cc_entex);
    adns__timeouts(ads, 0, tv_io, tvbuf, now);
    adns__consistency(ads, 0, cc_entex);
}

void adns_processtimeouts(adns_state ads, const struct timeval *now)
{
    struct timeval tv_buf;

    adns__consistency(ads, 0, cc_entex);
    adns__must_gettimeofday(ads, &now, &tv_buf);
    if (now)
        adns__timeouts(ads, 1, 0, 0, *now);
    adns__consistency(ads, 0, cc_entex);
}

/* fd handling functions.  These are the top-level of the real work of
 * reception and often transmission.
 */

int adns__pollfds(adns_state ads, struct pollfd pollfds_buf[MAX_POLLFDS])
{
    /* Returns the number of entries filled in.  Always zeroes revents. */

    (void) assert(MAX_POLLFDS == 2);

    pollfds_buf[0].fd = ads->udpsocket;
    pollfds_buf[0].events = POLLIN;
    pollfds_buf[0].revents = 0;

    switch (ads->tcpstate) {
    case server_disconnected:
    case server_broken:
        return 1;
    case server_connecting:
        pollfds_buf[1].events = POLLOUT;
        break;
    case server_ok:
        pollfds_buf[1].events =
            ads->tcpsend.
            used ? POLLIN | POLLOUT | POLLPRI : POLLIN | POLLPRI;
        break;
    default:
        abort();
    }
    pollfds_buf[1].fd = ads->tcpsocket;
    return 2;
}

int adns_processreadable(adns_state ads, deno_socket_t fd,
                         const struct timeval *now)
{
    int want, dgramlen, r;
    deno_socklen_t udpaddrlen;
    int serv, old_skip;
    byte udpbuf[DNS_MAXUDP];
    struct sockaddr_in udpaddr;

    adns__consistency(ads, 0, cc_entex);

    switch (ads->tcpstate) {
    case server_disconnected:
    case server_broken:
    case server_connecting:
        break;
    case server_ok:
        if (fd != ads->tcpsocket)
            break;
        assert(!ads->tcprecv_skip);
        do {
            if (ads->tcprecv.used >= ads->tcprecv_skip + 2) {
                dgramlen =
                    ((ads->tcprecv.
                      buf[ads->tcprecv_skip] << 8) | ads->
                     tcprecv.buf[ads->tcprecv_skip + 1]);
                if (ads->tcprecv.used >= ads->tcprecv_skip + 2 + dgramlen) {
                    old_skip = ads->tcprecv_skip;
                    ads->tcprecv_skip += 2 + dgramlen;
                    adns__procdgram(ads,
                                    ads->tcprecv.buf +
                                    old_skip + 2,
                                    dgramlen, ads->tcpserver, 1, *now);
                    continue;
                } else {
                    want = 2 + dgramlen;
                }
            } else {
                want = 2;
            }
            ads->tcprecv.used -= ads->tcprecv_skip;
            memmove(ads->tcprecv.buf,
                    ads->tcprecv.buf + ads->tcprecv_skip,
                    ads->tcprecv.used);
            ads->tcprecv_skip = 0;
            if (!adns__vbuf_ensure(&ads->tcprecv, want)) {
                r = ENOMEM;
                goto xit;
            }
            assert(ads->tcprecv.used <= ads->tcprecv.avail);
            if (ads->tcprecv.used == ads->tcprecv.avail)
                continue;
            r = deno_sockread(ads->tcpsocket,
                              (char *) ads->tcprecv.buf +
                              ads->tcprecv.used,
                              ads->tcprecv.avail - ads->tcprecv.used);
            if (r > 0) {
                ads->tcprecv.used += r;
            } else {
                if (r) {
                    if (deno_sockgeterr() == EAGAIN
                        || deno_sockgeterr() == SOCKERR_EWOULDBLOCK) {
                        r = 0;
                        goto xit;
                    }
                    if (deno_sockgeterr() == SOCKERR_EINTR)
                        continue;
                    if (errno_resources(deno_sockgeterr())) {
                        r = deno_sockgeterr();
                        goto xit;
                    }
                }
                adns__tcp_broken(ads, "read",
                                 r ? deno_sockgetstrerr() : "closed");
            }
        } while (ads->tcpstate == server_ok);
        r = 0;
        goto xit;
    default:
        abort();
    }
    if (fd == ads->udpsocket) {
        for (;;) {
            udpaddrlen = sizeof(udpaddr);
            r = recvfrom(ads->udpsocket, (char *) udpbuf, sizeof(udpbuf),
                         0, (struct sockaddr *) &udpaddr, &udpaddrlen);
            if (r < 0) {
                if (deno_sockgeterr() == EAGAIN
                    || deno_sockgeterr() == SOCKERR_EWOULDBLOCK) {
                    r = 0;
                    goto xit;
                }
                if (deno_sockgeterr() == SOCKERR_EINTR)
                    continue;
                if (errno_resources(deno_sockgeterr())) {
                    r = deno_sockgeterr();
                    goto xit;
                }
                alog(LOG_ADNS, "ADNS: datagram receive error: %s",
                     deno_sockgetstrerr());
                r = 0;
                goto xit;
            }
            if (udpaddrlen != sizeof(udpaddr)) {
                alog(LOG_ADNS,
                     "ADNS: datagram received with wrong address length %d (expected %lu)",
                     udpaddrlen, (unsigned long) sizeof(udpaddr));
                continue;
            }
            if (udpaddr.sin_family != AF_INET) {
                alog(LOG_ADNS,
                     "ADNS: datagram received with wrong protocol family %u (expected %u)",
                     udpaddr.sin_family, AF_INET);
                continue;
            }
            if (ntohs(udpaddr.sin_port) != DNS_PORT) {
                alog(LOG_ADNS,
                     "ADNS: datagram received from wrong port %u (expected %u)",
                     ntohs(udpaddr.sin_port), DNS_PORT);
                continue;
            }
            for (serv = 0;
                 serv < ads->nservers &&
                 ads->servers[serv].addr.s_addr !=
                 udpaddr.sin_addr.s_addr; serv++);
            if (serv >= ads->nservers) {
                alog(LOG_ADNS,
                     "ADNS: datagram received from unknown nameserver %s",
                     inet_ntoa(udpaddr.sin_addr));
                continue;
            }
            adns__procdgram(ads, udpbuf, r, serv, 0, *now);
        }
    }
    r = 0;
  xit:
    adns__consistency(ads, 0, cc_entex);
    return r;
}

int adns_processwriteable(adns_state ads, deno_socket_t fd,
                          const struct timeval *now)
{
    int r;

    adns__consistency(ads, 0, cc_entex);

    switch (ads->tcpstate) {
    case server_disconnected:
    case server_broken:
        break;
    case server_connecting:
        if (fd != ads->tcpsocket)
            break;
        assert(ads->tcprecv.used == 0);
        assert(ads->tcprecv_skip == 0);
        for (;;) {
            if (!adns__vbuf_ensure(&ads->tcprecv, 1)) {
                r = ENOMEM;
                goto xit;
            }
            /* Is & intended? cast to fix warning but that is how the original 
             * references the buffer. Mighty suspicious 
             */
            r = deno_sockread(ads->tcpsocket, (char *) &ads->tcprecv.buf,
                              1);
            if (r == 0
                || (r < 0
                    && (deno_sockgeterr() == EAGAIN
                        || deno_sockgeterr() == SOCKERR_EWOULDBLOCK))) {
                tcp_connected(ads, *now);
                r = 0;
                goto xit;
            }
            if (r > 0) {
                adns__tcp_broken(ads, "connect/read",
                                 "sent data before first request");
                r = 0;
                goto xit;
            }
            if (deno_sockgeterr() == SOCKERR_EINTR)
                continue;
            if (errno_resources(deno_sockgeterr())) {
                r = deno_sockgeterr();
                goto xit;
            }
            adns__tcp_broken(ads, "connect/read", deno_sockgetstrerr());
            r = 0;
            goto xit;
        }                       /* not reached */
    case server_ok:
        if (fd != ads->tcpsocket)
            break;
        while (ads->tcpsend.used) {
            r = deno_sockwrite(ads->tcpsocket, (char *) ads->tcpsend.buf,
                               ads->tcpsend.used);
            if (r < 0) {
                if (deno_sockgeterr() == SOCKERR_EINTR)
                    continue;
                if (deno_sockgeterr() == EAGAIN
                    || deno_sockgeterr() == SOCKERR_EWOULDBLOCK) {
                    r = 0;
                    goto xit;
                }
                if (errno_resources(deno_sockgeterr())) {
                    r = deno_sockgeterr();
                    goto xit;
                }
                adns__tcp_broken(ads, "write", deno_sockgetstrerr());
                r = 0;
                goto xit;
            } else if (r > 0) {
                ads->tcpsend.used -= r;
                memmove(ads->tcpsend.buf,
                        ads->tcpsend.buf + r, ads->tcpsend.used);
            }
        }
        r = 0;
        goto xit;
    default:
        abort();
    }
    r = 0;
  xit:
    adns__consistency(ads, 0, cc_entex);
    return r;
}

int adns_processexceptional(adns_state ads, deno_socket_t fd,
                            __attribute__((unused))const struct timeval *now)
{
    adns__consistency(ads, 0, cc_entex);
    switch (ads->tcpstate) {
    case server_disconnected:
    case server_broken:
        break;
    case server_connecting:
    case server_ok:
        if (fd != ads->tcpsocket)
            break;
        adns__tcp_broken(ads, "poll/select",
                         "exceptional condition detected");
        break;
    default:
        abort();
    }
    adns__consistency(ads, 0, cc_entex);
    return 0;
}

static void fd_event(adns_state ads, deno_socket_t fd, int revent,
                     int pollflag, int maxfd, const fd_set * fds,
                     int (*func) (adns_state, deno_socket_t fd,
                                  const struct timeval * now),
                     struct timeval now, int *r_r)
{
    int r;

    if (!(revent & pollflag))
        return;
    if (fds && !(fd < maxfd && FD_ISSET(fd, fds)))
        return;
    r = func(ads, fd, &now);
    if (r) {
        if (r_r) {
            *r_r = r;
        } else {
            alog(LOG_ADNS, "ADNS: process fd failed after select: %s",
                 strerror(errno));
            adns_globalsystemfailure(ads);
        }
    }
}

void adns__fdevents(adns_state ads, const struct pollfd *pollfds,
                    int npollfds, int maxfd, const fd_set * readfds,
                    const fd_set * writefds, const fd_set * exceptfds,
                    struct timeval now, int *r_r)
{
    int i, revents;
    deno_socket_t fd;

    for (i = 0; i < npollfds; i++) {
        fd = pollfds[i].fd;
        if (fd >= maxfd)
            maxfd = fd + 1;
        revents = pollfds[i].revents;
        fd_event(ads, fd, revents, POLLIN, maxfd, readfds,
                 adns_processreadable, now, r_r);
        fd_event(ads, fd, revents, POLLOUT, maxfd, writefds,
                 adns_processwriteable, now, r_r);
        fd_event(ads, fd, revents, POLLPRI, maxfd, exceptfds,
                 adns_processexceptional, now, r_r);
    }
}

/* Wrappers for select(2). */

void adns_beforeselect(adns_state ads, int *maxfd_io, fd_set * readfds_io,
                       fd_set * writefds_io, fd_set * exceptfds_io,
                       struct timeval **tv_mod, struct timeval *tv_tobuf,
                       const struct timeval *now)
{
    struct timeval tv_nowbuf;
    struct pollfd pollfds[MAX_POLLFDS];
    int i, maxfd, npollfds;
    deno_socket_t fd;

    adns__consistency(ads, 0, cc_entex);

    if (tv_mod && (!*tv_mod || (*tv_mod)->tv_sec || (*tv_mod)->tv_usec)) {
        /* The caller is planning to sleep. */
        adns__must_gettimeofday(ads, &now, &tv_nowbuf);
        if (!now) {
            inter_immed(tv_mod, tv_tobuf);
            adns__consistency(ads, 0, cc_entex);
            return;
        }
        adns__timeouts(ads, 0, tv_mod, tv_tobuf, *now);
    }

    npollfds = adns__pollfds(ads, pollfds);
    maxfd = *maxfd_io;
    for (i = 0; i < npollfds; i++) {
        fd = pollfds[i].fd;
        if ((int) fd >= maxfd)
            maxfd = fd + 1;
        if (pollfds[i].events & POLLIN)
            FD_SET(fd, readfds_io);
        if (pollfds[i].events & POLLOUT)
            FD_SET(fd, writefds_io);
        if (pollfds[i].events & POLLPRI)
            FD_SET(fd, exceptfds_io);
    }
    *maxfd_io = maxfd;
    adns__consistency(ads, 0, cc_entex);
}

void adns_afterselect(adns_state ads, int maxfd, const fd_set * readfds,
                      const fd_set * writefds, const fd_set * exceptfds,
                      const struct timeval *now)
{
    struct timeval tv_buf;
    struct pollfd pollfds[MAX_POLLFDS];
    int npollfds, i;

    adns__consistency(ads, 0, cc_entex);
    adns__must_gettimeofday(ads, &now, &tv_buf);
    if (now) {
        adns_processtimeouts(ads, now);

        npollfds = adns__pollfds(ads, pollfds);
        for (i = 0; i < npollfds; i++)
            pollfds[i].revents = POLLIN | POLLOUT | POLLPRI;
        adns__fdevents(ads,
                       pollfds, npollfds,
                       maxfd, readfds, writefds, exceptfds, *now, 0);
    }
    adns__consistency(ads, 0, cc_entex);
}

/* General helpful functions. */

void adns_globalsystemfailure(adns_state ads)
{
    adns__consistency(ads, 0, cc_entex);

    while (ads->udpw.head)
        adns__query_fail(ads->udpw.head, adns_s_systemfail);
    while (ads->tcpw.head)
        adns__query_fail(ads->tcpw.head, adns_s_systemfail);

    switch (ads->tcpstate) {
    case server_connecting:
    case server_ok:
        adns__tcp_broken(ads, 0, 0);
        break;
    case server_disconnected:
    case server_broken:
        break;
    default:
        abort();
    }
    adns__consistency(ads, 0, cc_entex);
}

/*************************************************************************/

int adns_processany(adns_state ads)
{
    int r, i;
    struct timeval now;
    struct pollfd pollfds[MAX_POLLFDS];
    int npollfds;

    adns__consistency(ads, 0, cc_entex);

    r = gettimeofday(&now, 0);
    if (!r)
        adns_processtimeouts(ads, &now);

    npollfds = adns__pollfds(ads, pollfds);
    for (i = 0; i < npollfds; i++)
        pollfds[i].revents = pollfds[i].events & ~POLLPRI;
    adns__fdevents(ads, pollfds, npollfds, 0, 0, 0, 0, now, &r);

    adns__consistency(ads, 0, cc_entex);
    return 0;
}

void adns__autosys(adns_state ads, __attribute__((unused))struct timeval now)
{
    if (ads->iflags & adns_if_noautosys)
        return;
    adns_processany(ads);
}

int adns__internal_check(adns_state ads, adns_query * query_io,
                         adns_answer ** answer, void **context_r)
{
    adns_query qu;

    qu = *query_io;
    if (!qu) {
        if (ads->output.head) {
            qu = ads->output.head;
        } else if (ads->udpw.head || ads->tcpw.head) {
            return EAGAIN;
        } else {
            return ESRCH;
        }
    } else {
        if (qu->id >= 0)
            return EAGAIN;
    }
    ALIST_UNLINK(ads->output, qu);
    *answer = qu->answer;
    if (context_r)
        *context_r = qu->ctx.ext;
    *query_io = qu;
    free(qu);
    return 0;
}

int adns_wait(adns_state ads,
              adns_query * query_io,
              adns_answer ** answer_r, void **context_r)
{
    int r, maxfd, rsel;
    fd_set readfds, writefds, exceptfds;
    struct timeval tvbuf, *tvp;

    adns__consistency(ads, *query_io, cc_entex);
    for (;;) {
        r = adns__internal_check(ads, query_io, answer_r, context_r);
        if (r != EAGAIN)
            break;
        maxfd = 0;
        tvp = 0;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        adns_beforeselect(ads, &maxfd, &readfds, &writefds,
                          &exceptfds, &tvp, &tvbuf, 0);
        assert(tvp);
        rsel = select(maxfd, &readfds, &writefds, &exceptfds, tvp);
        if (rsel == -1) {
            if (deno_sockgeterr() == SOCKERR_EINTR) {
                if (ads->iflags & adns_if_eintr) {
                    r = deno_sockgeterr();
                    break;
                }
            } else {
                alog(LOG_ADNS, "ADNS: select failed in wait: %s",
                     deno_sockgetstrerr());
                adns_globalsystemfailure(ads);
            }
        } else {
            assert(rsel >= 0);
            adns_afterselect(ads, maxfd, &readfds, &writefds,
                             &exceptfds, 0);
        }
    }
    adns__consistency(ads, 0, cc_entex);
    return r;
}

/*************************************************************************/

int adns_check(adns_state ads,
               adns_query * query_io,
               adns_answer ** answer_r, void **context_r)
{
    struct timeval now;
    int r;

    adns__consistency(ads, *query_io, cc_entex);
    r = gettimeofday(&now, 0);
    if (!r) {
        adns__autosys(ads, now);
    }

    r = adns__internal_check(ads, query_io, answer_r, context_r);
    adns__consistency(ads, 0, cc_entex);
    return r;
}

#define R_NOMEM           return adns_s_nomemory
#define CSP_ADDSTR(s)     do { if (!adns__vbuf_appendstr(vb,(s))) R_NOMEM; } while (0)

/*************************************************************************/

static adns_status pap_qstring(const parseinfo * pai, int *cbyte_io,
                               int max, int *len_r, char **str_r)
{
    /* Neither len_r nor str_r may be null.
     * End of datagram (overrun) is indicated by returning adns_s_invaliddata;
     */
    const byte *dgram = pai->dgram;
    int l, cbyte;
    char *str;

    cbyte = *cbyte_io;

    if (cbyte >= max)
        return adns_s_invaliddata;
    GET_B(cbyte, l);
    if (cbyte + l > max)
        return adns_s_invaliddata;

    str = adns__alloc_interim(pai->qu, l + 1);
    if (!str)
        R_NOMEM;

    str[l] = 0;
    memcpy(str, dgram + cbyte, l);

    *len_r = l;
    *str_r = str;
    *cbyte_io = cbyte + l;

    return adns_s_ok;
}

/*************************************************************************/

static adns_status csp_qstring(vbuf * vb, const char *dp, int len)
{
    unsigned char ch;
    char buf[10];
    int cn;

    CSP_ADDSTR("\"");
    for (cn = 0; cn < len; cn++) {
        ch = *dp++;
        if (ch == '\\') {
            CSP_ADDSTR("\\\\");
        } else if (ch == '"') {
            CSP_ADDSTR("\\\"");
        } else if (ch >= 32 && ch <= 126) {
            if (!adns__vbuf_append(vb, &ch, 1))
                R_NOMEM;
        } else {
            ircsprintf(buf, "\\x%02x", ch);
            CSP_ADDSTR(buf);
        }
    }
    CSP_ADDSTR("\"");

    return adns_s_ok;
}

static void mf_str(adns_query qu, void *datap)
{
    char **rrp = datap;

    adns__makefinal_str(qu, rrp);
}

static void mf_intstr(adns_query qu, void *datap)
{
    adns_rr_intstr *rrp = datap;

    adns__makefinal_str(qu, &rrp->str);
}

/*
 * _manyistr   (mf)
 */

static void mf_manyistr(adns_query qu, void *datap)
{
    adns_rr_intstr **rrp = datap;
    adns_rr_intstr *te, *table;
    void *tablev;
    int tc;

    for (tc = 0, te = *rrp; te->i >= 0; te++, tc++);
    tablev = *rrp;
    adns__makefinal_block(qu, &tablev, sizeof(*te) * (tc + 1));
    *rrp = table = tablev;
    for (te = *rrp; te->i >= 0; te++)
        adns__makefinal_str(qu, &te->str);
}

/*
 * _txt   (pa,cs)
 */

static adns_status pa_txt(const parseinfo * pai, int cbyte, int max,
                          void *datap)
{
    adns_rr_intstr **rrp = datap, *table, *te;
    const byte *dgram = pai->dgram;
    int ti, tc, l, startbyte;
    adns_status st;

    startbyte = cbyte;
    if (cbyte >= max)
        return adns_s_invaliddata;
    tc = 0;
    while (cbyte < max) {
        GET_B(cbyte, l);
        cbyte += l;
        tc++;
    }
    if (cbyte != max || !tc)
        return adns_s_invaliddata;

    table = adns__alloc_interim(pai->qu, sizeof(*table) * (tc + 1));
    if (!table)
        R_NOMEM;

    for (cbyte = startbyte, ti = 0, te = table; ti < tc; ti++, te++) {
        st = pap_qstring(pai, &cbyte, max, &te->i, &te->str);
        if (st)
            return st;
    }
    assert(cbyte == max);

    te->i = -1;
    te->str = 0;

    *rrp = table;
    return adns_s_ok;
}

static adns_status cs_txt(vbuf * vb, const void *datap)
{
    const adns_rr_intstr *const *rrp = datap;
    const adns_rr_intstr *current;
    adns_status st;
    int spc;

    for (current = *rrp, spc = 0; current->i >= 0; current++, spc = 1) {
        if (spc)
            CSP_ADDSTR(" ");
        st = csp_qstring(vb, current->str, current->i);
        if (st)
            return st;
    }
    return adns_s_ok;
}

/*
 * _hinfo   (cs)
 */

static adns_status cs_hinfo(vbuf * vb, const void *datap)
{
    const adns_rr_intstrpair *rrp = datap;
    adns_status st;

    st = csp_qstring(vb, rrp->array[0].str, rrp->array[0].i);
    if (st)
        return st;
    CSP_ADDSTR(" ");
    st = csp_qstring(vb, rrp->array[1].str, rrp->array[1].i);
    if (st)
        return st;
    return adns_s_ok;
}

/*
 * _inaddr   (pa,dip,di)
 */

static adns_status pa_inaddr(const parseinfo * pai, int cbyte, int max,
                             void *datap)
{
    struct in_addr *storeto = datap;

    if (max - cbyte != 4)
        return adns_s_invaliddata;
    memcpy(storeto, pai->dgram + cbyte, 4);
    return adns_s_ok;
}

static int search_sortlist(adns_state ads, struct in_addr ad)
{
    const struct sortlist *slp;
    int i;

    for (i = 0, slp = ads->sortlist;
         i < ads->nsortlist
         && !((ad.s_addr & slp->mask.s_addr) == slp->base.s_addr);
         i++, slp++);
    return i;
}

static int dip_inaddr(adns_state ads, struct in_addr a, struct in_addr b)
{
    int ai, bi;

    if (!ads->nsortlist)
        return 0;

    ai = search_sortlist(ads, a);
    bi = search_sortlist(ads, b);
    return bi < ai;
}

static int di_inaddr(adns_state ads, const void *datap_a,
                     const void *datap_b)
{
    const struct in_addr *ap = datap_a, *bp = datap_b;

    return dip_inaddr(ads, *ap, *bp);
}

static adns_status cs_inaddr(vbuf * vb, const void *datap)
{
    const struct in_addr *rrp = datap, rr = *rrp;
    const char *ia;

    ia = inet_ntoa(rr);
    assert(ia);
    CSP_ADDSTR(ia);
    return adns_s_ok;
}

/*
 * _addr   (pa,di,csp,cs)
 */

static adns_status pa_addr(const parseinfo * pai, int cbyte, int max,
                           void *datap)
{
    adns_rr_addr *storeto = datap;
    const byte *dgram = pai->dgram;

    if (max - cbyte != 4)
        return adns_s_invaliddata;
    storeto->len = sizeof(storeto->addr.inet);
    memset(&storeto->addr, 0, sizeof(storeto->addr.inet));
    storeto->addr.inet.sin_family = AF_INET;
    memcpy(&storeto->addr.inet.sin_addr, dgram + cbyte, 4);
    return adns_s_ok;
}

static int di_addr(adns_state ads, const void *datap_a,
                   const void *datap_b)
{
    const adns_rr_addr *ap = datap_a, *bp = datap_b;

    assert(ap->addr.sa.sa_family == AF_INET);
    return dip_inaddr(ads, ap->addr.inet.sin_addr, bp->addr.inet.sin_addr);
}

static int div_addr(void *context, const void *datap_a,
                    const void *datap_b)
{
    const adns_state ads = context;

    return di_addr(ads, datap_a, datap_b);
}

static adns_status csp_addr(vbuf * vb, const adns_rr_addr * rrp)
{
    const char *ia;
    static char buf[30];

    switch (rrp->addr.inet.sin_family) {
    case AF_INET:
        CSP_ADDSTR("INET ");
        ia = inet_ntoa(rrp->addr.inet.sin_addr);
        assert(ia);
        CSP_ADDSTR(ia);
        break;
    default:
        ircsprintf(buf, "AF=%u", rrp->addr.sa.sa_family);
        CSP_ADDSTR(buf);
        break;
    }
    return adns_s_ok;
}

static adns_status cs_addr(vbuf * vb, const void *datap)
{
    const adns_rr_addr *rrp = datap;

    return csp_addr(vb, rrp);
}

/*
 * _domain      (pap,csp,cs)
 * _dom_raw     (pa)
 */

static adns_status pap_domain(const parseinfo * pai, int *cbyte_io,
                              int max, char **domain_r,
                              parsedomain_flags flags)
{
    adns_status st;
    char *dm;

    st = adns__parse_domain(pai->qu->ads, pai->serv, pai->qu,
                            &pai->qu->vb, flags, pai->dgram,
                            pai->dglen, cbyte_io, max);
    if (st)
        return st;
    if (!pai->qu->vb.used)
        return adns_s_invaliddata;

    dm = adns__alloc_interim(pai->qu, pai->qu->vb.used + 1);
    if (!dm)
        R_NOMEM;

    dm[pai->qu->vb.used] = 0;
    memcpy(dm, pai->qu->vb.buf, pai->qu->vb.used);

    *domain_r = dm;
    return adns_s_ok;
}

static adns_status csp_domain(vbuf * vb, const char *domain)
{
    CSP_ADDSTR(domain);
    if (!*domain)
        CSP_ADDSTR(".");
    return adns_s_ok;
}

static adns_status cs_domain(vbuf * vb, const void *datap)
{
    const char *const *domainp = datap;
    return csp_domain(vb, *domainp);
}

static adns_status pa_dom_raw(const parseinfo * pai, int cbyte, int max,
                              void *datap)
{
    char **rrp = datap;
    adns_status st;

    st = pap_domain(pai, &cbyte, max, rrp, pdf_quoteok);
    if (st)
        return st;

    if (cbyte != max)
        return adns_s_invaliddata;
    return adns_s_ok;
}

/*
 * _host_raw   (pa)
 */

static adns_status pa_host_raw(const parseinfo * pai, int cbyte, int max,
                               void *datap)
{
    char **rrp = datap;
    adns_status st;

    st = pap_domain(pai, &cbyte, max, rrp,
                    pai->qu->
                    flags & adns_qf_quoteok_anshost ? pdf_quoteok : 0);
    if (st)
        return st;

    if (cbyte != max)
        return adns_s_invaliddata;
    return adns_s_ok;
}

/*
 * _hostaddr   (pap,pa,dip,di,mfp,mf,csp,cs +icb_hostaddr, pap_findaddrs)
 */

static adns_status pap_findaddrs(const parseinfo * pai,
                                 adns_rr_hostaddr * ha, int *cbyte_io,
                                 int count, int dmstart)
{
    int rri, naddrs;
    int type, class, rdlen, rdstart, ownermatched;
    unsigned long ttl;
    adns_status st;

    for (rri = 0, naddrs = -1; rri < count; rri++) {
        st = adns__findrr_anychk(pai->qu, pai->serv, pai->dgram,
                                 pai->dglen, cbyte_io, &type,
                                 &class, &ttl, &rdlen, &rdstart,
                                 pai->dgram, pai->dglen, dmstart,
                                 &ownermatched);
        if (st)
            return st;
        if (!ownermatched || class != DNS_CLASS_IN || type != adns_r_a) {
            if (naddrs > 0)
                break;
            else
                continue;
        }
        if (naddrs == -1) {
            naddrs = 0;
        }
        if (!adns__vbuf_ensure
            (&pai->qu->vb, (naddrs + 1) * sizeof(adns_rr_addr)))
            R_NOMEM;
        adns__update_expires(pai->qu, ttl, pai->now);
        st = pa_addr(pai, rdstart, rdstart + rdlen,
                     pai->qu->vb.buf + naddrs * sizeof(adns_rr_addr));
        if (st)
            return st;
        naddrs++;
    }
    if (naddrs >= 0) {
        ha->addrs =
            adns__alloc_interim(pai->qu, naddrs * sizeof(adns_rr_addr));
        if (!ha->addrs)
            R_NOMEM;
        memcpy(ha->addrs, pai->qu->vb.buf, naddrs * sizeof(adns_rr_addr));
        ha->naddrs = naddrs;
        ha->astatus = adns_s_ok;

        adns__isort(ha->addrs, naddrs, sizeof(adns_rr_addr),
                    pai->qu->vb.buf, div_addr, pai->ads);
    }
    return adns_s_ok;
}

static void icb_hostaddr(adns_query parent, adns_query child)
{
    adns_answer *cans = child->answer;
    adns_rr_hostaddr *rrp = child->ctx.info.hostaddr;
    adns_state ads = parent->ads;
    adns_status st;

    st = cans->status;
    rrp->astatus = st;
    rrp->naddrs = (st > 0 && st <= adns_s_max_tempfail) ? -1 : cans->nrrs;
    rrp->addrs = cans->rrs.addr;
    adns__transfer_interim(child, parent, rrp->addrs,
                           rrp->naddrs * sizeof(adns_rr_addr));

    if (parent->children.head) {
        ALIST_LINK_TAIL(ads->childw, parent);
    } else {
        adns__query_done(parent);
    }
}

static adns_status pap_hostaddr(const parseinfo * pai, int *cbyte_io,
                                int max, adns_rr_hostaddr * rrp)
{
    adns_status st;
    int dmstart, cbyte;
    qcontext ctx;
    int id;
    adns_query nqu;
    adns_queryflags nflags;

    dmstart = cbyte = *cbyte_io;
    st = pap_domain(pai, &cbyte, max, &rrp->host,
                    pai->qu->
                    flags & adns_qf_quoteok_anshost ? pdf_quoteok : 0);
    if (st)
        return st;
    *cbyte_io = cbyte;

    rrp->astatus = adns_s_ok;
    rrp->naddrs = -1;
    rrp->addrs = 0;

    cbyte = pai->nsstart;

    st = pap_findaddrs(pai, rrp, &cbyte, pai->nscount, dmstart);
    if (st)
        return st;
    if (rrp->naddrs != -1)
        return adns_s_ok;

    st = pap_findaddrs(pai, rrp, &cbyte, pai->arcount, dmstart);
    if (st)
        return st;
    if (rrp->naddrs != -1)
        return adns_s_ok;

    st = adns__mkquery_frdgram(pai->ads, &pai->qu->vb, &id,
                               pai->dgram, pai->dglen, dmstart,
                               adns_r_addr, adns_qf_quoteok_query);
    if (st)
        return st;

    ctx.ext = 0;
    ctx.callback = icb_hostaddr;
    ctx.info.hostaddr = rrp;

    nflags = adns_qf_quoteok_query;
    if (!(pai->qu->flags & adns_qf_cname_loose))
        nflags |= adns_qf_cname_forbid;

    st = adns__internal_submit(pai->ads, &nqu,
                               adns__findtype(adns_r_addr),
                               &pai->qu->vb, id, nflags, pai->now, &ctx);
    if (st)
        return st;

    nqu->parent = pai->qu;
    ALIST_LINK_TAIL_PART(pai->qu->children, nqu, siblings.);

    return adns_s_ok;
}

static adns_status pa_hostaddr(const parseinfo * pai, int cbyte, int max,
                               void *datap)
{
    adns_rr_hostaddr *rrp = datap;
    adns_status st;

    st = pap_hostaddr(pai, &cbyte, max, rrp);
    if (st)
        return st;
    if (cbyte != max)
        return adns_s_invaliddata;

    return adns_s_ok;
}

static int dip_hostaddr(adns_state ads, const adns_rr_hostaddr * ap,
                        const adns_rr_hostaddr * bp)
{
    if (ap->astatus != bp->astatus)
        return ap->astatus;
    if (ap->astatus)
        return 0;

    assert(ap->addrs[0].addr.sa.sa_family == AF_INET);
    assert(bp->addrs[0].addr.sa.sa_family == AF_INET);
    return dip_inaddr(ads,
                      ap->addrs[0].addr.inet.sin_addr,
                      bp->addrs[0].addr.inet.sin_addr);
}

static int di_hostaddr(adns_state ads, const void *datap_a,
                       const void *datap_b)
{
    const adns_rr_hostaddr *ap = datap_a, *bp = datap_b;

    return dip_hostaddr(ads, ap, bp);
}

static void mfp_hostaddr(adns_query qu, adns_rr_hostaddr * rrp)
{
    void *tablev;

    adns__makefinal_str(qu, &rrp->host);
    tablev = rrp->addrs;
    adns__makefinal_block(qu, &tablev, rrp->naddrs * sizeof(*rrp->addrs));
    rrp->addrs = tablev;
}

static void mf_hostaddr(adns_query qu, void *datap)
{
    adns_rr_hostaddr *rrp = datap;

    mfp_hostaddr(qu, rrp);
}

static adns_status csp_hostaddr(vbuf * vb, const adns_rr_hostaddr * rrp)
{
    adns_status st;
    char buf[20];
    int i;
    const struct sinfo *si;

    st = csp_domain(vb, rrp->host);
    if (st)
        return st;

    si = findsinfo(rrp->astatus);

    CSP_ADDSTR(" ");
    CSP_ADDSTR(adns_errtypeabbrev(rrp->astatus));

    ircsprintf(buf, " %d ", rrp->astatus);
    CSP_ADDSTR(buf);

    CSP_ADDSTR(si->abbrev);
    CSP_ADDSTR(" ");

    st = csp_qstring(vb, si->string, strlen(si->string));
    if (st)
        return st;

    if (rrp->naddrs >= 0) {
        CSP_ADDSTR(" (");
        for (i = 0; i < rrp->naddrs; i++) {
            CSP_ADDSTR(" ");
            st = csp_addr(vb, &rrp->addrs[i]);
        }
        CSP_ADDSTR(" )");
    } else {
        CSP_ADDSTR(" ?");
    }
    return adns_s_ok;
}

static adns_status cs_hostaddr(vbuf * vb, const void *datap)
{
    const adns_rr_hostaddr *rrp = datap;

    return csp_hostaddr(vb, rrp);
}

/*
 * _mx_raw   (pa,di)
 */

static adns_status pa_mx_raw(const parseinfo * pai, int cbyte, int max,
                             void *datap)
{
    const byte *dgram = pai->dgram;
    adns_rr_intstr *rrp = datap;
    adns_status st;
    int pref;

    if (cbyte + 2 > max)
        return adns_s_invaliddata;
    GET_W(cbyte, pref);
    rrp->i = pref;
    st = pap_domain(pai, &cbyte, max, &rrp->str,
                    pai->qu->
                    flags & adns_qf_quoteok_anshost ? pdf_quoteok : 0);
    if (st)
        return st;

    if (cbyte != max)
        return adns_s_invaliddata;
    return adns_s_ok;
}

static int di_mx_raw(__attribute__((unused))adns_state ads, const void *datap_a,
                     const void *datap_b)
{
    const adns_rr_intstr *ap = datap_a, *bp = datap_b;

    if (ap->i < bp->i)
        return 0;
    if (ap->i > bp->i)
        return 1;
    return 0;
}

/*
 * _mx   (pa,di)
 */

static adns_status pa_mx(const parseinfo * pai, int cbyte, int max,
                         void *datap)
{
    const byte *dgram = pai->dgram;
    adns_rr_inthostaddr *rrp = datap;
    adns_status st;
    int pref;

    if (cbyte + 2 > max)
        return adns_s_invaliddata;
    GET_W(cbyte, pref);
    rrp->i = pref;
    st = pap_hostaddr(pai, &cbyte, max, &rrp->ha);
    if (st)
        return st;

    if (cbyte != max)
        return adns_s_invaliddata;
    return adns_s_ok;
}

static int di_mx(adns_state ads, const void *datap_a, const void *datap_b)
{
    const adns_rr_inthostaddr *ap = datap_a, *bp = datap_b;

    if (ap->i < bp->i)
        return 0;
    if (ap->i > bp->i)
        return 1;
    return dip_hostaddr(ads, &ap->ha, &bp->ha);
}

/*
 * _inthostaddr  (mf,cs)
 */

static void mf_inthostaddr(adns_query qu, void *datap)
{
    adns_rr_inthostaddr *rrp = datap;

    mfp_hostaddr(qu, &rrp->ha);
}

static adns_status cs_inthostaddr(vbuf * vb, const void *datap)
{
    const adns_rr_inthostaddr *rrp = datap;
    char buf[10];

    ircsprintf(buf, "%u ", rrp->i);
    CSP_ADDSTR(buf);

    return csp_hostaddr(vb, &rrp->ha);
}

/*
 * _inthost  (cs)
 */

static adns_status cs_inthost(vbuf * vb, const void *datap)
{
    const adns_rr_intstr *rrp = datap;
    char buf[10];

    ircsprintf(buf, "%u ", rrp->i);
    CSP_ADDSTR(buf);
    return csp_domain(vb, rrp->str);
}

/*
 * _ptr   (pa, +icb_ptr)
 */

static void icb_ptr(adns_query parent, adns_query child)
{
    adns_answer *cans = child->answer;
    const adns_rr_addr *queried, *found;
    adns_state ads = parent->ads;
    int i;

    if (cans->status == adns_s_nxdomain || cans->status == adns_s_nodata) {
        adns__query_fail(parent, adns_s_inconsistent);
        return;
    } else if (cans->status) {
        adns__query_fail(parent, cans->status);
        return;
    }

    queried = &parent->ctx.info.ptr_parent_addr;
    for (i = 0, found = cans->rrs.addr; i < cans->nrrs; i++, found++) {
        if (queried->len == found->len &&
            !memcmp(&queried->addr, &found->addr, queried->len)) {
            if (!parent->children.head) {
                adns__query_done(parent);
                return;
            } else {
                ALIST_LINK_TAIL(ads->childw, parent);
                return;
            }
        }
    }

    adns__query_fail(parent, adns_s_inconsistent);
}

static adns_status pa_ptr(const parseinfo * pai, int dmstart, int max,
                          void *datap)
{
    static const char *(expectdomain[]) = {
    DNS_INADDR_ARPA};

    char **rrp = datap;
    adns_status st;
    adns_rr_addr *ap;
    findlabel_state afls;
    char *ep;
    byte ipv[4];
    char labbuf[4];
    int cbyte, i, lablen, labstart, l, id;
    adns_query nqu;
    qcontext ctx;

    cbyte = dmstart;
    st = pap_domain(pai, &cbyte, max, rrp,
                    pai->qu->
                    flags & adns_qf_quoteok_anshost ? pdf_quoteok : 0);
    if (st)
        return st;
    if (cbyte != max)
        return adns_s_invaliddata;

    ap = &pai->qu->ctx.info.ptr_parent_addr;
    if (!ap->len) {
        adns__findlabel_start(&afls, pai->ads, -1, pai->qu,
                              pai->qu->query_dgram,
                              pai->qu->query_dglen,
                              pai->qu->query_dglen, DNS_HDRSIZE, 0);
        for (i = 0; i < 4; i++) {
            st = adns__findlabel_next(&afls, &lablen, &labstart);
            assert(!st);
            if (lablen <= 0 || lablen > 3)
                return adns_s_querydomainwrong;
            memcpy(labbuf, pai->qu->query_dgram + labstart, lablen);
            labbuf[lablen] = 0;
            ipv[3 - i] = strtoul(labbuf, &ep, 10);
            if (*ep)
                return adns_s_querydomainwrong;
            if (lablen > 1 && pai->qu->query_dgram[labstart] == '0')
                return adns_s_querydomainwrong;
        }
        for (i = 0;
             i < (int) sizeof(expectdomain) / (int) sizeof(*expectdomain);
             i++) {
            st = adns__findlabel_next(&afls, &lablen, &labstart);
            assert(!st);
            l = strlen(expectdomain[i]);
            if (lablen != l
                || memcmp(pai->qu->query_dgram + labstart,
                          expectdomain[i], l))
                return adns_s_querydomainwrong;
        }
        st = adns__findlabel_next(&afls, &lablen, 0);
        assert(!st);
        if (lablen)
            return adns_s_querydomainwrong;

        ap->len = sizeof(struct sockaddr_in);
        memset(&ap->addr, 0, sizeof(ap->addr.inet));
        ap->addr.inet.sin_family = AF_INET;
        ap->addr.inet.sin_addr.s_addr =
            htonl((ipv[0] << 24) | (ipv[1] << 16) | (ipv[2] << 8) |
                  (ipv[3]));
    }

    st = adns__mkquery_frdgram(pai->ads, &pai->qu->vb, &id,
                               pai->dgram, pai->dglen, dmstart,
                               adns_r_addr, adns_qf_quoteok_query);
    if (st)
        return st;

    ctx.ext = 0;
    ctx.callback = icb_ptr;
    memset(&ctx.info, 0, sizeof(ctx.info));
    st = adns__internal_submit(pai->ads, &nqu,
                               adns__findtype(adns_r_addr),
                               &pai->qu->vb, id, adns_qf_quoteok_query,
                               pai->now, &ctx);
    if (st)
        return st;

    nqu->parent = pai->qu;
    ALIST_LINK_TAIL_PART(pai->qu->children, nqu, siblings.);
    return adns_s_ok;
}

/*
 * _strpair   (mf)
 */

static void mf_strpair(adns_query qu, void *datap)
{
    adns_rr_strpair *rrp = datap;

    adns__makefinal_str(qu, &rrp->array[0]);
    adns__makefinal_str(qu, &rrp->array[1]);
}

/*************************************************************************/

static void mf_intstrpair(adns_query qu, void *datap)
{
    adns_rr_intstrpair *rrp = datap;

    adns__makefinal_str(qu, &rrp->array[0].str);
    adns__makefinal_str(qu, &rrp->array[1].str);
}

/*************************************************************************/

static adns_status pa_hinfo(const parseinfo * pai, int cbyte, int max,
                            void *datap)
{
    adns_rr_intstrpair *rrp = datap;
    adns_status st;
    int i;

    for (i = 0; i < 2; i++) {
        st = pap_qstring(pai, &cbyte, max, &rrp->array[i].i,
                         &rrp->array[i].str);
        if (st)
            return st;
    }

    if (cbyte != max)
        return adns_s_invaliddata;

    return adns_s_ok;
}

/*************************************************************************/

static adns_status pap_mailbox822(const parseinfo * pai, int *cbyte_io,
                                  int max, char **mb_r)
{
    int lablen = 0;
    int labstart = 0;
    int i, needquote, c, r, neednorm;
    const unsigned char *p;
    char *str;
    findlabel_state afls;
    adns_status st;
    vbuf *vb;

    vb = &pai->qu->vb;
    vb->used = 0;
    adns__findlabel_start(&afls, pai->ads,
                          -1, pai->qu,
                          pai->dgram, pai->dglen, max,
                          *cbyte_io, cbyte_io);
    st = adns__findlabel_next(&afls, &lablen, &labstart);
    if (!lablen) {
        adns__vbuf_appendstr(vb, ".");
        goto x_ok;
    }

    neednorm = 1;
    for (i = 0, needquote = 0, p = pai->dgram + labstart; i < lablen; i++) {
        c = *p++;
        if ((c & ~128) < 32 || (c & ~128) == 127)
            return adns_s_invaliddata;
        if (c == '.' && !neednorm)
            neednorm = 1;
        else if (c == ' ' || c >= 127 || ctype_822special(c))
            needquote++;
        else
            neednorm = 0;
    }

    if (needquote || neednorm) {
        r = adns__vbuf_ensure(vb, lablen + needquote + 4);
        if (!r)
            R_NOMEM;
        adns__vbuf_appendq(vb, (const byte *) "\"", 1);
        for (i = 0, needquote = 0, p = pai->dgram + labstart;
             i < lablen; i++, p++) {
            c = *p;
            if (c == '"' || c == '\\')
                adns__vbuf_appendq(vb, (const byte *) "\\", 1);
            adns__vbuf_appendq(vb, p, 1);
        }
        adns__vbuf_appendq(vb, (const byte *) "\"", 1);
    } else {
        r = adns__vbuf_append(vb, pai->dgram + labstart, lablen);
        if (!r)
            R_NOMEM;
    }

    r = adns__vbuf_appendstr(vb, "@");
    if (!r)
        R_NOMEM;

    st = adns__parse_domain_more(&afls, pai->ads, pai->qu, vb, 0,
                                 pai->dgram);
    if (st)
        return st;

  x_ok:
    str = adns__alloc_interim(pai->qu, vb->used + 1);
    if (!str)
        R_NOMEM;
    memcpy(str, vb->buf, vb->used);
    str[vb->used] = 0;
    *mb_r = str;
    return adns_s_ok;
}

static adns_status pap_mailbox(const parseinfo * pai, int *cbyte_io,
                               int max, char **mb_r)
{
    if (pai->qu->typei->type & adns__qtf_mail822) {
        return pap_mailbox822(pai, cbyte_io, max, mb_r);
    } else {
        return pap_domain(pai, cbyte_io, max, mb_r, pdf_quoteok);
    }
}

static adns_status csp_mailbox(vbuf * vb, const char *mailbox)
{
    return csp_domain(vb, mailbox);
}

/*************************************************************************/

static adns_status pa_rp(const parseinfo * pai, int cbyte, int max,
                         void *datap)
{
    adns_rr_strpair *rrp = datap;
    adns_status st;

    st = pap_mailbox(pai, &cbyte, max, &rrp->array[0]);
    if (st)
        return st;

    st = pap_domain(pai, &cbyte, max, &rrp->array[1], pdf_quoteok);
    if (st)
        return st;

    if (cbyte != max)
        return adns_s_invaliddata;
    return adns_s_ok;
}

static adns_status cs_rp(vbuf * vb, const void *datap)
{
    const adns_rr_strpair *rrp = datap;
    adns_status st;

    st = csp_mailbox(vb, rrp->array[0]);
    if (st)
        return st;
    CSP_ADDSTR(" ");
    st = csp_domain(vb, rrp->array[1]);
    if (st)
        return st;

    return adns_s_ok;
}

/*
 * _soa   (pa,mf,cs)
 */

static adns_status pa_soa(const parseinfo * pai, int cbyte, int max,
                          void *datap)
{
    adns_rr_soa *rrp = datap;
    const byte *dgram = pai->dgram;
    adns_status st;
    int msw, lsw, i;

    st = pap_domain(pai, &cbyte, max, &rrp->mname,
                    pai->qu->
                    flags & adns_qf_quoteok_anshost ? pdf_quoteok : 0);
    if (st)
        return st;

    st = pap_mailbox(pai, &cbyte, max, &rrp->rname);
    if (st)
        return st;

    if (cbyte + 20 != max)
        return adns_s_invaliddata;

    for (i = 0; i < 5; i++) {
        GET_W(cbyte, msw);
        GET_W(cbyte, lsw);
        (&rrp->serial)[i] = (msw << 16) | lsw;
    }

    return adns_s_ok;
}

static void mf_soa(adns_query qu, void *datap)
{
    adns_rr_soa *rrp = datap;

    adns__makefinal_str(qu, &rrp->mname);
    adns__makefinal_str(qu, &rrp->rname);
}

static adns_status cs_soa(vbuf * vb, const void *datap)
{
    const adns_rr_soa *rrp = datap;
    char buf[20];
    int i;
    adns_status st;

    st = csp_domain(vb, rrp->mname);
    if (st)
        return st;
    CSP_ADDSTR(" ");
    st = csp_mailbox(vb, rrp->rname);
    if (st)
        return st;

    for (i = 0; i < 5; i++) {
        ircsprintf(buf, " %lu", (&rrp->serial)[i]);
        CSP_ADDSTR(buf);
    }

    return adns_s_ok;
}

/*************************************************************************/

static void mf_flat(__attribute__((unused))adns_query qu, __attribute__((unused))void *adata)
{
}

/*************************************************************************/

#define TYPESZ_M(member)           (sizeof(*((adns_answer*)0)->rrs.member))

#define DEEP_MEMB(memb) TYPESZ_M(memb), mf_##memb, cs_##memb
#define FLAT_MEMB(memb) TYPESZ_M(memb), mf_flat, cs_##memb

#define DEEP_TYPE(code,rrt,fmt,memb,parser,comparer,printer) \
 { adns_r_##code, rrt, fmt, TYPESZ_M(memb), mf_##memb, printer, parser, comparer }
#define FLAT_TYPE(code,rrt,fmt,memb,parser,comparer,printer) \
 { adns_r_##code, rrt, fmt, TYPESZ_M(memb), mf_flat, printer, parser, comparer }

static const typeinfo typeinfos[] = {
/* Must be in ascending order of rrtype ! */
/* mem-mgmt code  rrt     fmt     member      parser      comparer    printer       */

    FLAT_TYPE(a, "A", 0, inaddr, pa_inaddr, di_inaddr, cs_inaddr),
    DEEP_TYPE(ns_raw, "NS", "raw", str, pa_host_raw, 0, cs_domain),
    DEEP_TYPE(cname, "CNAME", 0, str, pa_dom_raw, 0, cs_domain),
    DEEP_TYPE(soa_raw, "SOA", "raw", soa, pa_soa, 0, cs_soa),
    DEEP_TYPE(ptr_raw, "PTR", "raw", str, pa_host_raw, 0, cs_domain),
    DEEP_TYPE(hinfo, "HINFO", 0, intstrpair, pa_hinfo, 0, cs_hinfo),
    DEEP_TYPE(mx_raw, "MX", "raw", intstr, pa_mx_raw, di_mx_raw,
              cs_inthost),
    DEEP_TYPE(txt, "TXT", 0, manyistr, pa_txt, 0, cs_txt),
    DEEP_TYPE(rp_raw, "RP", "raw", strpair, pa_rp, 0, cs_rp),

    FLAT_TYPE(addr, "A", "addr", addr, pa_addr, di_addr, cs_addr),
    DEEP_TYPE(ns, "NS", "+addr", hostaddr, pa_hostaddr, di_hostaddr,
              cs_hostaddr),
    DEEP_TYPE(ptr, "PTR", "checked", str, pa_ptr, 0, cs_domain),
    DEEP_TYPE(mx, "MX", "+addr", inthostaddr, pa_mx, di_mx,
              cs_inthostaddr),

    DEEP_TYPE(soa, "SOA", "822", soa, pa_soa, 0, cs_soa),
    DEEP_TYPE(rp, "RP", "822", strpair, pa_rp, 0, cs_rp),
};

const typeinfo *adns__findtype(adns_rrtype type)
{
    const typeinfo *begin, *end, *mid;

    begin = typeinfos;
    end = typeinfos + (sizeof(typeinfos) / sizeof(typeinfo));

    while (begin < end) {
        mid = begin + ((end - begin) >> 1);
        if (mid->type == type)
            return mid;
        if (type > mid->type)
            begin = mid + 1;
        else
            end = mid;
    }
    return 0;
}

static adns_query query_alloc(adns_state ads, const typeinfo * typei,
                              adns_queryflags flags, struct timeval now)
{
    /* Allocate a virgin query and return it. */
    adns_query qu;

    qu = malloc(sizeof(*qu));
    if (!qu)
        return 0;
    qu->answer = malloc(sizeof(*qu->answer));
    if (!qu->answer) {
        free(qu);
        return 0;
    }

    qu->ads = ads;
    qu->state = query_tosend;
    qu->back = qu->next = qu->parent = 0;
    ALIST_INIT(qu->children);
    ALINK_INIT(qu->siblings);
    ALIST_INIT(qu->allocations);
    qu->interim_allocd = 0;
    qu->preserved_allocd = 0;
    qu->final_allocspace = 0;

    qu->typei = typei;
    qu->query_dgram = 0;
    qu->query_dglen = 0;
    adns__vbuf_init(&qu->vb);

    qu->cname_dgram = 0;
    qu->cname_dglen = qu->cname_begin = 0;

    adns__vbuf_init(&qu->search_vb);
    qu->search_origlen = qu->search_pos = qu->search_doneabs = 0;

    qu->id = -2;                /* will be overwritten with real id before we leave adns */
    qu->flags = flags;
    qu->retries = 0;
    qu->udpnextserver = 0;
    qu->udpsent = 0;
    timerclear(&qu->timeout);
    qu->expires = now.tv_sec + MAXTTLBELIEVE;

    memset(&qu->ctx, 0, sizeof(qu->ctx));

    qu->answer->status = adns_s_ok;
    qu->answer->cname = qu->answer->owner = 0;
    qu->answer->type = typei->type;
    qu->answer->expires = -1;
    qu->answer->nrrs = 0;
    qu->answer->rrs.untyped = 0;
    qu->answer->rrsz = typei->rrsz;

    return qu;
}

static void query_submit(__attribute__((unused))adns_state ads, adns_query qu,
                         __attribute__((unused))const typeinfo * typei, vbuf * qumsg_vb, int id,
                         __attribute__((unused))adns_queryflags flags, struct timeval now)
{
    /* Fills in the query message in for a previously-allocated query,
     * and submits it.  Cannot fail.  Takes over the memory for qumsg_vb.
     */

    qu->vb = *qumsg_vb;
    adns__vbuf_init(qumsg_vb);

    qu->query_dgram = malloc(qu->vb.used);
    if (!qu->query_dgram) {
        adns__query_fail(qu, adns_s_nomemory);
        return;
    }

    qu->id = id;
    qu->query_dglen = qu->vb.used;
    memcpy(qu->query_dgram, qu->vb.buf, qu->vb.used);

    adns__query_send(qu, now);
}

adns_status adns__internal_submit(adns_state ads, adns_query * query_r,
                                  const typeinfo * typei, vbuf * qumsg_vb,
                                  int id, adns_queryflags flags,
                                  struct timeval now, const qcontext * ctx)
{
    adns_query qu;

    qu = query_alloc(ads, typei, flags, now);
    if (!qu) {
        adns__vbuf_free(qumsg_vb);
        return adns_s_nomemory;
    }
    *query_r = qu;

    memcpy(&qu->ctx, ctx, sizeof(qu->ctx));
    query_submit(ads, qu, typei, qumsg_vb, id, flags, now);

    return adns_s_ok;
}

static void query_simple(adns_state ads, adns_query qu,
                         const char *owner, int ol,
                         const typeinfo * typei, adns_queryflags flags,
                         struct timeval now)
{
    vbuf vb_new;
    int id;
    adns_status astat;

    astat = adns__mkquery(ads, &qu->vb, &id, owner, ol, typei, flags);
    if (astat) {
        if (astat == adns_s_querydomaintoolong && (flags & adns_qf_search)) {
            adns__search_next(ads, qu, now);
            return;
        } else {
            adns__query_fail(qu, astat);
            return;
        }
    }

    vb_new = qu->vb;
    adns__vbuf_init(&qu->vb);
    query_submit(ads, qu, typei, &vb_new, id, flags, now);
}

void adns__search_next(adns_state ads, adns_query qu, struct timeval now)
{
    const char *nextentry;
    adns_status astat;

    if (qu->search_doneabs < 0) {
        nextentry = 0;
        qu->search_doneabs = 1;
    } else {
        if (qu->search_pos >= ads->nsearchlist) {
            if (qu->search_doneabs) {
                astat = adns_s_nxdomain;
                goto x_fail;
                return;
            } else {
                nextentry = 0;
                qu->search_doneabs = 1;
            }
        } else {
            nextentry = ads->searchlist[qu->search_pos++];
        }
    }

    qu->search_vb.used = qu->search_origlen;
    if (nextentry) {
        if (!adns__vbuf_append(&qu->search_vb, (const byte *) ".", 1) ||
            !adns__vbuf_appendstr(&qu->search_vb, nextentry)) {
            astat = adns_s_nomemory;
            goto x_fail;
        }
    }

    free(qu->query_dgram);
    qu->query_dgram = 0;
    qu->query_dglen = 0;

    query_simple(ads, qu, (const char *) qu->search_vb.buf,
                 qu->search_vb.used, qu->typei, qu->flags, now);
    return;

  x_fail:
    adns__query_fail(qu, astat);
}

static int save_owner(adns_query qu, const char *owner, int ol)
{
    /* Returns 1 if OK, otherwise there was no memory. */
    adns_answer *ans;

    ans = qu->answer;
    assert(!ans->owner);

    ans->owner = adns__alloc_preserved(qu, ol + 1);
    if (!ans->owner)
        return 0;

    memcpy(ans->owner, owner, ol);
    ans->owner[ol] = 0;
    return 1;
}

int adns_submit(adns_state ads, const char *owner, adns_rrtype type,
                adns_queryflags flags, void *context, adns_query * query_r)
{
    int r, ol, ndots;
    adns_status astat;
    const typeinfo *typei;
    struct timeval now;
    adns_query qu;
    const char *p;

    adns__consistency(ads, 0, cc_entex);

    typei = adns__findtype(type);
    if (!typei)
        return ENOSYS;

    r = gettimeofday(&now, 0);
    if (r)
        goto x_errno;
    qu = query_alloc(ads, typei, flags, now);
    if (!qu)
        goto x_errno;

    qu->ctx.ext = context;
    qu->ctx.callback = 0;
    memset(&qu->ctx.info, 0, sizeof(qu->ctx.info));

    *query_r = qu;

    ol = strlen(owner);
    if (!ol) {
        astat = adns_s_querydomaininvalid;
        goto x_adnsfail;
    }
    if (ol > DNS_MAXDOMAIN + 1) {
        astat = adns_s_querydomaintoolong;
        goto x_adnsfail;
    }

    if (ol >= 1 && owner[ol - 1] == '.'
        && (ol < 2 || owner[ol - 2] != '\\')) {
        flags &= ~adns_qf_search;
        qu->flags = flags;
        ol--;
    }

    if (flags & adns_qf_search) {
        r = adns__vbuf_append(&qu->search_vb, (const byte *) owner, ol);
        if (!r) {
            astat = adns_s_nomemory;
            goto x_adnsfail;
        }

        for (ndots = 0, p = owner; (p = strchr(p, '.')); p++, ndots++);
        qu->search_doneabs = (ndots >= ads->searchndots) ? -1 : 0;
        qu->search_origlen = ol;
        adns__search_next(ads, qu, now);
    } else {
        if (flags & adns_qf_owner) {
            if (!save_owner(qu, owner, ol)) {
                astat = adns_s_nomemory;
                goto x_adnsfail;
            }
        }
        query_simple(ads, qu, owner, ol, typei, flags, now);
    }
    adns__autosys(ads, now);
    adns__consistency(ads, qu, cc_entex);
    return 0;

  x_adnsfail:
    adns__query_fail(qu, astat);
    adns__consistency(ads, qu, cc_entex);
    return 0;

  x_errno:
    r = errno;
    assert(r);
    adns__consistency(ads, 0, cc_entex);
    return r;
}

int adns_submit_reverse_any(adns_state ads, const struct sockaddr *addr,
                            const char *zone, adns_rrtype type,
                            adns_queryflags flags, void *context,
                            adns_query * query_r)
{
    const unsigned char *iaddr;
    char *buf, *buf_free;
    char shortbuf[100];
    int r, lreq;

    flags &= ~adns_qf_search;

    if (addr->sa_family != AF_INET)
        return ENOSYS;
    iaddr = (const unsigned char *)
        &(((const struct sockaddr_in *) addr)->sin_addr);

    lreq = strlen(zone) + 4 * 4 + 1;
    if (lreq > (int) sizeof(shortbuf)) {
        buf = malloc(strlen(zone) + 4 * 4 + 1);
        if (!buf)
            return errno;
        buf_free = buf;
    } else {
        buf = shortbuf;
        buf_free = 0;
    }
    ircsprintf(buf, "%d.%d.%d.%d.%s", iaddr[3], iaddr[2], iaddr[1],
               iaddr[0], zone);
    r = adns_submit(ads, buf, type, flags, context, query_r);
    free(buf_free);
    return r;
}

int adns_submit_reverse(adns_state ads, const struct sockaddr *addr,
                        adns_rrtype type, adns_queryflags flags,
                        void *context, adns_query * query_r)
{
    if (type != adns_r_ptr && type != adns_r_ptr_raw)
        return EINVAL;
    return adns_submit_reverse_any(ads, addr, "in-addr.arpa", type,
                                   flags, context, query_r);
}

int adns_synchronous(adns_state ads, const char *owner, adns_rrtype type,
                     adns_queryflags flags, adns_answer ** answer_r)
{
    adns_query qu;
    int r;

    r = adns_submit(ads, owner, type, flags, 0, &qu);
    if (r)
        return r;

    r = adns_wait(ads, &qu, answer_r, 0);
    if (r)
        adns_cancel(qu);

    return r;
}

static void *alloc_common(adns_query qu, size_t sz)
{
    allocnode *an;

    if (!sz)
        return qu;              /* Any old pointer will do */
    assert(!qu->final_allocspace);
    an = malloc(MEM_ROUND(MEM_ROUND(sizeof(*an)) + sz));
    if (!an)
        return 0;
    ALIST_LINK_TAIL(qu->allocations, an);
    return (byte *) an + MEM_ROUND(sizeof(*an));
}

void *adns__alloc_interim(adns_query qu, size_t sz)
{
    void *rv;

    sz = MEM_ROUND(sz);
    rv = alloc_common(qu, sz);
    if (!rv)
        return 0;
    qu->interim_allocd += sz;
    return rv;
}

void *adns__alloc_preserved(adns_query qu, size_t sz)
{
    void *rv;

    sz = MEM_ROUND(sz);
    rv = adns__alloc_interim(qu, sz);
    if (!rv)
        return 0;
    qu->preserved_allocd += sz;
    return rv;
}

void *adns__alloc_mine(adns_query qu, size_t sz)
{
    return alloc_common(qu, MEM_ROUND(sz));
}

void adns__transfer_interim(adns_query from, adns_query to, void *block,
                            size_t sz)
{
    allocnode *an;

    if (!block)
        return;
    an = (void *) ((byte *) block - MEM_ROUND(sizeof(*an)));

    assert(!to->final_allocspace);
    assert(!from->final_allocspace);

    ALIST_UNLINK(from->allocations, an);
    ALIST_LINK_TAIL(to->allocations, an);

    sz = MEM_ROUND(sz);
    from->interim_allocd -= sz;
    to->interim_allocd += sz;

    if (to->expires > from->expires)
        to->expires = from->expires;
}

void *adns__alloc_final(adns_query qu, size_t sz)
{
    /* When we're in the _final stage, we _subtract_ from interim_alloc'd
     * each allocation, and use final_allocspace to point to the next free
     * bit.
     */
    void *rp;

    sz = MEM_ROUND(sz);
    rp = qu->final_allocspace;
    assert(rp);
    qu->interim_allocd -= sz;
    assert(qu->interim_allocd >= 0);
    qu->final_allocspace = (byte *) rp + sz;
    return rp;
}

static void cancel_children(adns_query qu)
{
    adns_query cqu, ncqu;

    for (cqu = qu->children.head; cqu; cqu = ncqu) {
        ncqu = cqu->siblings.next;
        adns_cancel(cqu);
    }
}

void adns__reset_preserved(adns_query qu)
{
    assert(!qu->final_allocspace);
    cancel_children(qu);
    qu->answer->nrrs = 0;
    qu->answer->rrs.untyped = 0;
    qu->interim_allocd = qu->preserved_allocd;
}

static void free_query_allocs(adns_query qu)
{
    allocnode *an, *ann;

    cancel_children(qu);
    for (an = qu->allocations.head; an; an = ann) {
        ann = an->next;
        free(an);
    }
    ALIST_INIT(qu->allocations);
    adns__vbuf_free(&qu->vb);
    adns__vbuf_free(&qu->search_vb);
    free(qu->query_dgram);
    qu->query_dgram = 0;
}

void adns_cancel(adns_query qu)
{
    adns_state ads;

    ads = qu->ads;
    adns__consistency(ads, qu, cc_entex);
    if (qu->parent)
        ALIST_UNLINK_PART(qu->parent->children, qu, siblings.);
    switch (qu->state) {
    case query_tosend:
        ALIST_UNLINK(ads->udpw, qu);
        break;
    case query_tcpw:
        ALIST_UNLINK(ads->tcpw, qu);
        break;
    case query_childw:
        ALIST_UNLINK(ads->childw, qu);
        break;
    case query_done:
        ALIST_UNLINK(ads->output, qu);
        break;
    default:
        abort();
    }
    free_query_allocs(qu);
    free(qu->answer);
    free(qu);
    adns__consistency(ads, 0, cc_entex);
}

void adns__update_expires(adns_query qu, unsigned long ttl,
                          struct timeval now)
{
    time_t max;

    assert(ttl <= MAXTTLBELIEVE);
    max = now.tv_sec + ttl;
    if (qu->expires < max)
        return;
    qu->expires = max;
}

static void makefinal_query(adns_query qu)
{
    adns_answer *ans;
    int rrn;

    ans = qu->answer;

    if (qu->interim_allocd) {
        ans =
            realloc(qu->answer,
                    MEM_ROUND(MEM_ROUND(sizeof(*ans)) +
                              qu->interim_allocd));
        if (!ans)
            goto x_nomem;
        qu->answer = ans;
    }

    qu->final_allocspace = (byte *) ans + MEM_ROUND(sizeof(*ans));
    adns__makefinal_str(qu, &ans->cname);
    adns__makefinal_str(qu, &ans->owner);

    if (ans->nrrs) {
        adns__makefinal_block(qu, &ans->rrs.untyped,
                              ans->nrrs * ans->rrsz);

        for (rrn = 0; rrn < ans->nrrs; rrn++)
            qu->typei->makefinal(qu, ans->rrs.bytes + rrn * ans->rrsz);
    }

    free_query_allocs(qu);
    return;

  x_nomem:
    qu->preserved_allocd = 0;
    qu->answer->cname = 0;
    qu->answer->owner = 0;
    adns__reset_preserved(qu);  /* (but we just threw away the preserved stuff) */

    qu->answer->status = adns_s_nomemory;
    free_query_allocs(qu);
}

void adns__query_done(adns_query qu)
{
    adns_answer *ans;
    adns_query parent;

    cancel_children(qu);

    qu->id = -1;
    ans = qu->answer;

    if (qu->flags & adns_qf_owner && qu->flags & adns_qf_search &&
        ans->status != adns_s_nomemory) {
        if (!save_owner
            (qu, (const char *) qu->search_vb.buf, qu->search_vb.used)) {
            adns__query_fail(qu, adns_s_nomemory);
            return;
        }
    }

    if (ans->nrrs && qu->typei->diff_needswap) {
        if (!adns__vbuf_ensure(&qu->vb, qu->typei->rrsz)) {
            adns__query_fail(qu, adns_s_nomemory);
            return;
        }
        adns__isort(ans->rrs.bytes, ans->nrrs, ans->rrsz,
                    qu->vb.buf,
                    (int (*)(void *, const void *, const void *))
                    qu->typei->diff_needswap, qu->ads);
    }

    ans->expires = qu->expires;
    parent = qu->parent;
    if (parent) {
        ALIST_UNLINK_PART(parent->children, qu, siblings.);
        ALIST_UNLINK(qu->ads->childw, parent);
        qu->ctx.callback(parent, qu);
        free_query_allocs(qu);
        free(qu->answer);
        free(qu);
    } else {
        makefinal_query(qu);
        ALIST_LINK_TAIL(qu->ads->output, qu);
        qu->state = query_done;
    }
}

void adns__query_fail(adns_query qu, adns_status astat)
{
    adns__reset_preserved(qu);
    qu->answer->status = astat;
    adns__query_done(qu);
}

void adns__makefinal_str(adns_query qu, char **strp)
{
    int l;
    char *before, *after;

    before = *strp;
    if (!before)
        return;
    l = strlen(before) + 1;
    after = adns__alloc_final(qu, l);
    memcpy(after, before, l);
    *strp = after;
}

void adns__makefinal_block(adns_query qu, void **blpp, size_t sz)
{
    void *before, *after;

    before = *blpp;
    if (!before)
        return;
    after = adns__alloc_final(qu, sz);
    memcpy(after, before, sz);
    *blpp = after;
}

void adns__procdgram(adns_state ads, const byte * dgram, int dglen,
                     int serv, int viatcp, struct timeval now)
{
    int cbyte, rrstart, wantedrrs, rri, foundsoa, foundns, cname_here;
    int id, f1, f2, qdcount, ancount, nscount, arcount;
    int flg_ra, flg_rd, flg_tc, flg_qr, opcode;
    int rrtype, rrclass, rdlength, rdstart;
    int anstart, nsstart, arstart;
    int ownermatched, l, nrrs;
    unsigned long ttl, soattl;
    const typeinfo *typei;
    adns_query qu, nqu;
    dns_rcode rcode;
    adns_status st;
    vbuf tempvb;
    byte *newquery, *rrsdata;
    parseinfo pai;

    if (dglen < DNS_HDRSIZE) {
        alog(LOG_ADNS,
             "ADNS: received datagram too short for message header (%d)",
             dglen);
        return;
    }
    cbyte = 0;
    GET_W(cbyte, id);
    GET_B(cbyte, f1);
    GET_B(cbyte, f2);
    GET_W(cbyte, qdcount);
    GET_W(cbyte, ancount);
    GET_W(cbyte, nscount);
    GET_W(cbyte, arcount);
    assert(cbyte == DNS_HDRSIZE);

    flg_qr = f1 & 0x80;
    opcode = (f1 & 0x78) >> 3;
    flg_tc = f1 & 0x02;
    flg_rd = f1 & 0x01;
    flg_ra = f2 & 0x80;
    rcode = (f2 & 0x0f);

    cname_here = 0;

    if (!flg_qr) {
        alog(LOG_ADNS, "ADNS: server sent us a query, not a response");
        return;
    }
    if (opcode) {
        alog(LOG_ADNS,
             "ADNS: server sent us unknown opcode %d (wanted 0=QUERY)",
             opcode);
        return;
    }

    qu = 0;
    /* See if we can find the relevant query, or leave qu=0 otherwise ... */

    if (qdcount == 1) {
        for (qu = viatcp ? ads->tcpw.head : ads->udpw.head; qu; qu = nqu) {
            nqu = qu->next;
            if (qu->id != id)
                continue;
            if (dglen < qu->query_dglen)
                continue;
            if (memcmp(qu->query_dgram + DNS_HDRSIZE,
                       dgram + DNS_HDRSIZE, qu->query_dglen - DNS_HDRSIZE))
                continue;
            if (viatcp) {
                assert(qu->state == query_tcpw);
            } else {
                assert(qu->state == query_tosend);
                if (!(qu->udpsent & (1 << serv)))
                    continue;
            }
            break;
        }
        if (qu) {
            /* We're definitely going to do something with this query now */
            if (viatcp)
                ALIST_UNLINK(ads->tcpw, qu);
            else
                ALIST_UNLINK(ads->udpw, qu);
        }
    }

    switch (rcode) {
    case rcode_noerror:
    case rcode_nxdomain:
        break;
    case rcode_formaterror:
        alog(LOG_ADNS,
             "ADNS warning: server cannot understand our query (Format Error)");
        if (qu) {
            adns__query_fail(qu, adns_s_rcodeformaterror);
        }
        return;
    case rcode_servfail:
        if (qu) {
            adns__query_fail(qu, adns_s_rcodeservfail);
        } else {
            alog(LOG_ADNS, "ADNS: server failure on unidentifiable query");
        }
        return;
    case rcode_notimp:
        alog(LOG_ADNS, "ADNS: server claims not to implement our query");
        if (qu) {
            adns__query_fail(qu, adns_s_rcodenotimplemented);
        }
        return;
    case rcode_refused:
        alog(LOG_ADNS, "ADNS: server refused our query");
        if (qu) {
            adns__query_fail(qu, adns_s_rcoderefused);
        }
        return;
    default:
        alog(LOG_ADNS, "ADNS: server gave unknown response code %d",
             rcode);
        if (qu) {
            adns__query_fail(qu, adns_s_rcodeunknown);
        }
        return;
    }

    if (!qu) {
        if (!qdcount) {
            alog(LOG_ADNS,
                 "ADNS: server sent reply without quoting our question");
        } else if (qdcount > 1) {
            alog(LOG_ADNS,
                 "ADNS: server claimed to answer %d questions with one message",
                 qdcount);
        } else if (ads->iflags & adns_if_debug) {
            adns__vbuf_init(&tempvb);
            alog(LOG_ADNS,
                 "ADNS: reply not found, id %02x, query owner %s",
                 id, adns__diag_domain(ads, serv, 0,
                                       &tempvb, dgram,
                                       dglen, DNS_HDRSIZE));
            adns__vbuf_free(&tempvb);
        }
        return;
    }

    anstart = qu->query_dglen;
    arstart = -1;

    wantedrrs = 0;
    cbyte = anstart;
    for (rri = 0; rri < ancount; rri++) {
        rrstart = cbyte;
        st = adns__findrr(qu, serv, dgram, dglen, &cbyte,
                          &rrtype, &rrclass, &ttl, &rdlength,
                          &rdstart, &ownermatched);
        if (st) {
            adns__query_fail(qu, st);
            return;
        }
        if (rrtype == -1)
            goto x_truncated;

        if (rrclass != DNS_CLASS_IN) {
            alog(LOG_ADNS,
                 "ADNS: ignoring answer RR with wrong class %d (expected IN=%d)",
                 rrclass, DNS_CLASS_IN);
            continue;
        }
        if (!ownermatched) {
            if (ads->iflags & adns_if_debug) {
                alog(LOG_ADNS,
                     "ADNS: ignoring RR with an unexpected owner %s",
                     adns__diag_domain(ads, serv,
                                       qu, &qu->vb,
                                       dgram, dglen, rrstart));
            }
            continue;
        }
        if (rrtype == adns_r_cname &&
            (qu->typei->type & adns__rrt_typemask) != adns_r_cname) {
            if (qu->flags & adns_qf_cname_forbid) {
                adns__query_fail(qu, adns_s_prohibitedcname);
                return;
            } else if (qu->cname_dgram) {
                alog(LOG_ADNS,
                     "ADNS: allegedly canonical name %s is actually alias for %s",
                     qu->answer->cname,
                     adns__diag_domain(ads, serv,
                                       qu, &qu->vb,
                                       dgram, dglen, rdstart));
                adns__query_fail(qu, adns_s_prohibitedcname);
                return;
            } else if (wantedrrs) {
                alog(LOG_ADNS,
                     "ADNS: ignoring CNAME (to %s) coexisting with RR",
                     adns__diag_domain(ads, serv,
                                       qu, &qu->vb,
                                       dgram, dglen, rdstart));
            } else {
                qu->cname_begin = rdstart;
                qu->cname_dglen = dglen;
                st = adns__parse_domain(ads, serv, qu,
                                        &qu->vb,
                                        qu->
                                        flags &
                                        adns_qf_quotefail_cname
                                        ? 0 : pdf_quoteok,
                                        dgram, dglen,
                                        &rdstart, rdstart + rdlength);
                if (!qu->vb.used)
                    goto x_truncated;
                if (st) {
                    adns__query_fail(qu, st);
                    return;
                }
                l = strlen((char *) qu->vb.buf) + 1;
                qu->answer->cname = adns__alloc_preserved(qu, l);
                if (!qu->answer->cname) {
                    adns__query_fail(qu, adns_s_nomemory);
                    return;
                }

                qu->cname_dgram = adns__alloc_mine(qu, dglen);
                memcpy(qu->cname_dgram, dgram, dglen);

                memcpy(qu->answer->cname, qu->vb.buf, l);
                cname_here = 1;
                adns__update_expires(qu, ttl, now);
            }
        } else if ((int) rrtype ==
                   (int) (qu->typei->type & adns__rrt_typemask)) {
            wantedrrs++;
        } else {
            alog(LOG_ADNS,
                 "ADNS: ignoring answer RR with irrelevant type %d",
                 rrtype);
        }
    }

    if (flg_tc) {
        goto x_truncated;
    }

    nsstart = cbyte;

    if (!wantedrrs) {
        foundsoa = 0;
        soattl = 0;
        foundns = 0;
        for (rri = 0; rri < nscount; rri++) {
            rrstart = cbyte;
            st = adns__findrr(qu, serv, dgram, dglen, &cbyte,
                              &rrtype, &rrclass, &ttl,
                              &rdlength, &rdstart, 0);
            if (st) {
                adns__query_fail(qu, st);
                return;
            }
            if (rrtype == -1)
                goto x_truncated;
            if (rrclass != DNS_CLASS_IN) {
                alog(LOG_ADNS,
                     "ADNS: ignoring authority RR with wrong class %d (expected IN=%d)",
                     rrclass, DNS_CLASS_IN);
                continue;
            }
            if (rrtype == adns_r_soa_raw) {
                foundsoa = 1;
                soattl = ttl;
                break;
            } else if (rrtype == adns_r_ns_raw) {
                foundns = 1;
            }
        }

        if (rcode == rcode_nxdomain) {
            /* We still wanted to look for the SOA so we could find the TTL. */
            adns__update_expires(qu, soattl, now);

            if (qu->flags & adns_qf_search) {
                adns__search_next(ads, qu, now);
            } else {
                adns__query_fail(qu, adns_s_nxdomain);
            }
            return;
        }

        if (foundsoa || !foundns) {
            /* Aha !  A NODATA response, good. */
            adns__update_expires(qu, soattl, now);
            adns__query_fail(qu, adns_s_nodata);
            return;
        }

        /* Now what ?  No relevant answers, no SOA, and at least some NS's.
         * Looks like a referral.  Just one last chance ... if we came across
         * a CNAME in this datagram then we should probably do our own CNAME
         * lookup now in the hope that we won't get a referral again.
         */
        if (cname_here)
            goto x_restartquery;

        /* Bloody hell, I thought we asked for recursion ? */
        if (!flg_ra) {
            alog(LOG_ADNS,
                 "ADNS: server is not willing to do recursive lookups for us");
            adns__query_fail(qu, adns_s_norecurse);
        } else {
            if (!flg_rd) {
                alog(LOG_ADNS,
                     "ADNS: server thinks we didn't ask for recursive lookup");
            } else {
                alog(LOG_ADNS,
                     "server claims to do recursion, but gave us a referral");
            }
            adns__query_fail(qu, adns_s_invalidresponse);
        }
        return;
    }

    qu->answer->rrs.untyped =
        adns__alloc_interim(qu, qu->typei->rrsz * wantedrrs);
    if (!qu->answer->rrs.untyped) {
        adns__query_fail(qu, adns_s_nomemory);
        return;
    }

    typei = qu->typei;
    cbyte = anstart;
    rrsdata = qu->answer->rrs.bytes;

    pai.ads = qu->ads;
    pai.qu = qu;
    pai.serv = serv;
    pai.dgram = dgram;
    pai.dglen = dglen;
    pai.nsstart = nsstart;
    pai.nscount = nscount;
    pai.arcount = arcount;
    pai.now = now;

    for (rri = 0, nrrs = 0; rri < ancount; rri++) {
        st = adns__findrr(qu, serv, dgram, dglen, &cbyte,
                          &rrtype, &rrclass, &ttl, &rdlength,
                          &rdstart, &ownermatched);
        assert(!st);
        assert(rrtype != -1);
        if (rrclass != DNS_CLASS_IN ||
            (int) rrtype != (int) (qu->typei->type & adns__rrt_typemask) ||
            !ownermatched)
            continue;
        adns__update_expires(qu, ttl, now);
        st = typei->parse(&pai, rdstart, rdstart + rdlength,
                          rrsdata + nrrs * typei->rrsz);
        if (st) {
            adns__query_fail(qu, st);
            return;
        }
        if (rdstart == -1)
            goto x_truncated;
        nrrs++;
    }
    assert(nrrs == wantedrrs);
    qu->answer->nrrs = nrrs;

    /* This may have generated some child queries ... */
    if (qu->children.head) {
        qu->state = query_childw;
        ALIST_LINK_TAIL(ads->childw, qu);
        return;
    }
    adns__query_done(qu);
    return;

  x_truncated:

    if (!flg_tc) {
        alog(LOG_ADNS,
             "ADNS: server sent datagram which points outside itself");
        adns__query_fail(qu, adns_s_invalidresponse);
        return;
    }
    qu->flags |= adns_qf_usevc;

  x_restartquery:
    if (qu->cname_dgram) {
        st = adns__mkquery_frdgram(qu->ads, &qu->vb, &qu->id,
                                   qu->cname_dgram,
                                   qu->cname_dglen,
                                   qu->cname_begin,
                                   qu->typei->type, qu->flags);
        if (st) {
            adns__query_fail(qu, st);
            return;
        }

        newquery = realloc(qu->query_dgram, qu->vb.used);
        if (!newquery) {
            adns__query_fail(qu, adns_s_nomemory);
            return;
        }

        qu->query_dgram = newquery;
        qu->query_dglen = qu->vb.used;
        memcpy(newquery, qu->vb.buf, qu->vb.used);
    }

    if (qu->state == query_tcpw)
        qu->state = query_tosend;
    qu->retries = 0;
    adns__reset_preserved(qu);
    adns__query_send(qu, now);
}

static void readconfig(adns_state ads, const char *filename,
                       int warnmissing);

static void addserver(adns_state ads, struct in_addr addr)
{
    int i;
    struct server *ss;

    for (i = 0; i < ads->nservers; i++) {
        if (ads->servers[i].addr.s_addr == addr.s_addr) {
            alog(LOG_ADNS,
                 "ADNS: duplicate nameserver %s ignored", inet_ntoa(addr));
            return;
        }
    }

    if (ads->nservers >= MAXSERVERS) {
        alog(LOG_ADNS,
             "ADNS: too many nameservers, ignoring %s", inet_ntoa(addr));
        return;
    }

    ss = ads->servers + ads->nservers;
    ss->addr = addr;
    ads->nservers++;
}

static void freesearchlist(adns_state ads)
{
    if (ads->nsearchlist)
        free(*ads->searchlist);
    if (ads->searchlist)
        free(ads->searchlist);
}

static void saveerr(adns_state ads, int en)
{
    if (!ads->configerrno)
        ads->configerrno = en;
}

static void configparseerr(adns_state ads, const char *fn, int lno,
                           const char *fmt, ...)
{
    va_list al;

    saveerr(ads, EINVAL);
    if (!ads->diagfile || (ads->iflags & adns_if_noerrprint))
        return;

    if (lno == -1)
        fprintf(ads->diagfile, "adns: %s: ", fn);
    else
        fprintf(ads->diagfile, "adns: %s:%d: ", fn, lno);
    va_start(al, fmt);
    vfprintf(ads->diagfile, fmt, al);
    va_end(al);
    fputc('\n', ads->diagfile);
}

static int nextword(const char **bufp_io, const char **word_r, int *l_r)
{
    const char *p, *q;

    p = *bufp_io;
    while (ctype_whitespace(*p))
        p++;
    if (!*p)
        return 0;

    q = p;
    while (*q && !ctype_whitespace(*q))
        q++;

    *l_r = q - p;
    *word_r = p;
    *bufp_io = q;

    return 1;
}

static void ccf_nameserver(adns_state ads, const char *fn, int lno,
                           const char *buf)
{
    struct in_addr ia;

    if (!inet_aton(buf, &ia)) {
        configparseerr(ads, fn, lno, "invalid nameserver address `%s'",
                       buf);
        return;
    }
    alog(LOG_ADNS, "ADNS: using nameserver %s", inet_ntoa(ia));
    addserver(ads, ia);
}

static void ccf_search(adns_state ads, __attribute__((unused))const char *fn, __attribute__((unused))int lno,
                       const char *buf)
{
    const char *bufp, *word;
    char *newchars, **newptrs, **pp;
    int count, tl, l;

    if (!buf)
        return;

    bufp = buf;
    count = 0;
    tl = 0;
    while (nextword(&bufp, &word, &l)) {
        count++;
        tl += l + 1;
    }

    newptrs = malloc(sizeof(char *) * count);
    if (!newptrs) {
        saveerr(ads, errno);
        return;
    }
    newchars = malloc(tl);
    if (!newchars) {
        saveerr(ads, errno);
        free(newptrs);
        return;
    }

    bufp = buf;
    pp = newptrs;
    while (nextword(&bufp, &word, &l)) {
        *pp++ = newchars;
        memcpy(newchars, word, l);
        newchars += l;
        *newchars++ = 0;
    }

    freesearchlist(ads);
    ads->nsearchlist = count;
    ads->searchlist = newptrs;
}

static void ccf_sortlist(adns_state ads, const char *fn, int lno,
                         const char *buf)
{
    const char *word;
    char tbuf[200], *slash, *ep;
    struct in_addr base, mask;
    int l;
    unsigned long initial, baselocal;

    if (!buf)
        return;

    ads->nsortlist = 0;
    while (nextword(&buf, &word, &l)) {
        if (ads->nsortlist >= MAXSORTLIST) {
            alog(LOG_ADNS,
                 "ADNS: too many sortlist entries, ignoring %.*s onwards",
                 l, word);
            return;
        }

        if (l >= (int) sizeof(tbuf)) {
            configparseerr(ads, fn, lno,
                           "sortlist entry `%.*s' too long", l, word);
            continue;
        }

        memcpy(tbuf, word, l);
        tbuf[l] = 0;
        slash = strchr(tbuf, '/');
        if (slash)
            *slash++ = 0;

        if (!inet_aton(tbuf, &base)) {
            configparseerr(ads, fn, lno,
                           "invalid address `%s' in sortlist", tbuf);
            continue;
        }

        if (slash) {
            if (strchr(slash, '.')) {
                if (!inet_aton(slash, &mask)) {
                    configparseerr(ads, fn, lno,
                                   "invalid mask `%s' in sortlist", slash);
                    continue;
                }
                if (base.s_addr & ~mask.s_addr) {
                    configparseerr(ads, fn, lno,
                                   "mask `%s' in sortlist overlaps address `%s'",
                                   slash, tbuf);
                    continue;
                }
            } else {
                initial = strtoul(slash, &ep, 10);
                if (*ep || initial > 32) {
                    configparseerr(ads, fn, lno,
                                   "mask length `%s' invalid", slash);
                    continue;
                }
                mask.s_addr = htonl((0x0ffffffffUL) << (32 - initial));
            }
        } else {
            baselocal = ntohl(base.s_addr);
            if (!baselocal & 0x080000000UL)     /* class A */
                mask.s_addr = htonl(0x0ff000000UL);
            else if ((baselocal & 0x0c0000000UL) == 0x080000000UL)
                mask.s_addr = htonl(0x0ffff0000UL);     /* class B */
            else if ((baselocal & 0x0f0000000UL) == 0x0e0000000UL)
                mask.s_addr = htonl(0x0ff000000UL);     /* class C */
            else {
                configparseerr(ads, fn, lno,
                               "network address `%s' in sortlist is not in classed ranges,"
                               " must specify mask explicitly", tbuf);
                continue;
            }
        }

        ads->sortlist[ads->nsortlist].base = base;
        ads->sortlist[ads->nsortlist].mask = mask;
        ads->nsortlist++;
    }
}

static void ccf_options(adns_state ads, const char *fn, int lno,
                        const char *buf)
{
    const char *word;
    char *ep;
    unsigned long v;
    int l;

    if (!buf)
        return;

    while (nextword(&buf, &word, &l)) {
        if (l == 5 && !memcmp(word, "debug", 5)) {
            ads->iflags |= adns_if_debug;
            continue;
        }
        if (l >= 6 && !memcmp(word, "ndots:", 6)) {
            v = strtoul(word + 6, &ep, 10);
            if (l == 6 || ep != word + l || v > INT_MAX) {
                configparseerr(ads, fn, lno,
                               "option `%.*s' malformed or has bad value",
                               l, word);
                continue;
            }
            ads->searchndots = v;
            continue;
        }
        if (l >= 12 && !memcmp(word, "adns_checkc:", 12)) {
            if (!strcmp(word + 12, "none")) {
                ads->iflags &= ~adns_if_checkc_freq;
                ads->iflags |= adns_if_checkc_entex;
            } else if (!strcmp(word + 12, "entex")) {
                ads->iflags &= ~adns_if_checkc_freq;
                ads->iflags |= adns_if_checkc_entex;
            } else if (!strcmp(word + 12, "freq")) {
                ads->iflags |= adns_if_checkc_freq;
            } else {
                configparseerr(ads, fn, lno,
                               "option adns_checkc has bad value `%s' "
                               "(must be none, entex or freq", word + 12);
            }
            continue;
        }
        alog(LOG_ADNS, "ADNS: %s:%d: unknown option `%.*s'", fn, lno, l,
             word);
    }
}

static void ccf_clearnss(adns_state ads, __attribute__((unused))const char *fn,
                         __attribute__((unused))int lno,
                         __attribute__((unused))const char *buf)
{
    ads->nservers = 0;
}

static void ccf_include(adns_state ads, const char *fn, int lno,
                        const char *buf)
{
    if (!*buf) {
        configparseerr(ads, fn, lno,
                       "`include' directive with no filename");
        return;
    }
    readconfig(ads, buf, 1);
}

static int gl_file(adns_state ads, getline_ctx * src_io,
                   const char *filename, int lno, char *buf, int buflen)
{
    FILE *file = src_io->file;
    int c, i;
    char *p;

    p = buf;
    buflen--;
    i = 0;

    for (;;) {                  /* loop over chars */
        if (i == buflen) {
            alog(LOG_DEBUG, "ADNS: %s:%d: line too long, ignored",
                 filename, lno);
            goto x_badline;
        }
        c = getc(file);
        if (!c) {
            alog(LOG_ADNS, "ADNS: %s:%d: line contains nul, ignored",
                 filename, lno);
            goto x_badline;
        } else if (c == '\n') {
            break;
        } else if (c == EOF) {
            if (ferror(file)) {
                saveerr(ads, errno);
                alog(LOG_ADNS, "ADNS: %s:%d: read error: %s",
                     filename, lno, strerror(errno));
                return -1;
            }
            if (!i)
                return -1;
            break;
        } else {
            *p++ = c;
            i++;
        }
    }

    *p++ = 0;
    return i;

  x_badline:
    saveerr(ads, EINVAL);
    while ((c = getc(file)) != EOF && c != '\n');
    return -2;
}

static int gl_text(adns_state ads, getline_ctx * src_io,
                   const char *filename, int lno, char *buf, int buflen)
{
    const char *cp = src_io->text;
    int l;

    if (!cp || !*cp)
        return -1;

    if (*cp == ';' || *cp == '\n')
        cp++;
    l = strcspn(cp, ";\n");
    src_io->text = cp + l;

    if (l >= buflen) {
        alog(LOG_ADNS, "ADNS: %s:%d: line too long, ignored", filename,
             lno);
        saveerr(ads, EINVAL);
        return -2;
    }

    memcpy(buf, cp, l);
    buf[l] = 0;
    return l;
}

static void readconfiggeneric(adns_state ads, const char *filename,
                              int (*getline) (adns_state ads,
                                              getline_ctx *,
                                              const char *filename,
                                              int lno, char *buf,
                                              int buflen),
                              getline_ctx gl_ctx)
{
    char linebuf[2000], *p, *q;
    int lno, l, dirl;
    const struct configcommandinfo *ccip;

    for (lno = 1; (l = getline(ads, &gl_ctx, filename, lno, linebuf, sizeof(linebuf))) != -1; lno++) {
        if (l == -2)
            continue;
        while (l > 0 && ctype_whitespace(linebuf[l - 1]))
            l--;
        linebuf[l] = 0;
        p = linebuf;
        while (ctype_whitespace(*p)) {
            p++;
		}
        if (*p == '#' || !*p) {
            continue;
		}
        q = p;
        while (*q && !ctype_whitespace(*q)) {
            q++;
		}
        dirl = q - p;
        for (ccip = configcommandinfos; ccip->name && !((int) strlen(ccip->name) == dirl && !memcmp(ccip->name, p, q - p)); ccip++);
        if (BadPtr(ccip->name)) {
            alog(LOG_ADNS,
                 "ADNS: %s:%d: unknown configuration directive `%.*s'",
                 filename, lno, q - p, p);
            continue;
        }
        while (ctype_whitespace(*q))
            q++;
        ccip->fn(ads, filename, lno, q);
    }
}

static const char *instrum_getenv(__attribute__((unused))adns_state ads, const char *envvar)
{
    const char *value;

    value = getenv(envvar);
    if (!value) {
        alog(LOG_ADNS, "ADNS: environment variable %s not set", envvar);
    } else {
        alog(LOG_ADNS, "ADNS: environment variable %s set to `%s'", envvar,
             value);
    }
    return value;
}

static void readconfig(adns_state ads, const char *filename,
                       int warnmissing)
{
    getline_ctx gl_ctx;

    if (!FileExists((char *) filename)) {
        if (warnmissing) {
            alog(LOG_ADNS,
                 "ADNS: configuration file `%s' does not exist", filename);
        }
        return;
    }

    gl_ctx.file = FileOpen((char *) filename, FILE_READ);
    if (!gl_ctx.file) {
        if (errno == ENOENT) {
            return;
        }
        saveerr(ads, errno);
        alog(LOG_ADNS, "ADNS: cannot open configuration file `%s': %s",
             filename, strerror(errno));
        return;
    }

    readconfiggeneric(ads, filename, gl_file, gl_ctx);

    fclose(gl_ctx.file);
}

static void readconfigtext(adns_state ads, const char *text,
                           const char *showname)
{
    getline_ctx gl_ctx;

    gl_ctx.text = text;
    readconfiggeneric(ads, showname, gl_text, gl_ctx);
}

static void readconfigenv(adns_state ads, const char *envvar)
{
    const char *filename;

    if (ads->iflags & adns_if_noenv) {
        alog(LOG_ADNS, "ADNS: not checking environment variable `%s'",
             envvar);
        return;
    }
    filename = instrum_getenv(ads, envvar);
    if (filename) {
        readconfig(ads, filename, 1);
    }
}

static void readconfigenvtext(adns_state ads, const char *envvar)
{
    const char *textdata;

    if (ads->iflags & adns_if_noenv) {
        alog(LOG_ADNS, "ADNS: not checking environment variable `%s'",
             envvar);
        return;
    }
    textdata = instrum_getenv(ads, envvar);
    if (textdata)
        readconfigtext(ads, textdata, envvar);
}


static int init_begin(adns_state * ads_r, adns_initflags flags,
                      FILE * diagfile)
{
    adns_state ads;

    ads = malloc(sizeof(*ads));
    if (!ads)
        return errno;

    ads->iflags = flags;
    ads->diagfile = diagfile;
    ads->configerrno = 0;
    ALIST_INIT(ads->udpw);
    ALIST_INIT(ads->tcpw);
    ALIST_INIT(ads->childw);
    ALIST_INIT(ads->output);
    ads->forallnext = 0;
    ads->nextid = 0x311f;
    ads->udpsocket = ads->tcpsocket = -1;
    adns__vbuf_init(&ads->tcpsend);
    adns__vbuf_init(&ads->tcprecv);
    ads->tcprecv_skip = 0;
    ads->nservers = ads->nsortlist = ads->nsearchlist = ads->tcpserver = 0;
    ads->searchndots = 1;
    ads->tcpstate = server_disconnected;
    timerclear(&ads->tcptimeout);
    ads->searchlist = 0;

    *ads_r = ads;
    return 0;
}

static int init_finish(adns_state ads)
{
    struct in_addr ia;
    struct protoent *proto;
    int r;

    if (!ads->nservers) {
        if (ads->diagfile && ads->iflags & adns_if_debug)
            fprintf(ads->diagfile,
                    "adns: no nameservers, using localhost\n");
        ia.s_addr = htonl(INADDR_LOOPBACK);
        addserver(ads, ia);
    }

    proto = getprotobyname("udp");
    if (!proto) {
        alog(LOG_DEBUG, "ADNS: unable to find protocol no. for UDP !");
        r = ENOPROTOOPT;
    } else {
        ads->udpsocket = socket(AF_INET, SOCK_DGRAM, proto->p_proto);
        if (ads->udpsocket < 0) {
            r = deno_sockgeterr();
        } else {
            if (deno_socksetnonb(ads->udpsocket) < 0) {
                r = deno_sockgeterr();
                deno_sockclose(ads->udpsocket);
            } else {
                return 0;
            }
        }
    }

    free(ads);
    return r;
}

static void init_abort(adns_state ads)
{
    if (ads->nsearchlist) {
        free(ads->searchlist[0]);
        free(ads->searchlist);
    }
    free(ads);
}

int adns_init(adns_state * ads_r, adns_initflags flags, FILE * diagfile)
{
    adns_state ads = NULL;
    const char *res_options, *adns_res_options;
    int r;

    r = init_begin(&ads, flags, diagfile ? diagfile : stderr);
    if (r)
        return r;

    res_options = instrum_getenv(ads, "RES_OPTIONS");
    adns_res_options = instrum_getenv(ads, "ADNS_RES_OPTIONS");
    ccf_options(ads, "RES_OPTIONS", -1, res_options);
    ccf_options(ads, "ADNS_RES_OPTIONS", -1, adns_res_options);

    readconfig(ads, "/etc/resolv.conf", 1);
    readconfig(ads, "/etc/resolv-adns.conf", 0);

    readconfigenv(ads, "RES_CONF");
    readconfigenv(ads, "ADNS_RES_CONF");

    readconfigenvtext(ads, "RES_CONF_TEXT");
    readconfigenvtext(ads, "ADNS_RES_CONF_TEXT");

    ccf_options(ads, "RES_OPTIONS", -1, res_options);
    ccf_options(ads, "ADNS_RES_OPTIONS", -1, adns_res_options);

    ccf_search(ads, "LOCALDOMAIN", -1, instrum_getenv(ads, "LOCALDOMAIN"));
    ccf_search(ads, "ADNS_LOCALDOMAIN", -1,
               instrum_getenv(ads, "ADNS_LOCALDOMAIN"));

    if (ads->configerrno && ads->configerrno != EINVAL) {
        r = ads->configerrno;
        init_abort(ads);
        return r;
    }
    r = init_finish(ads);
    if (r)
        return r;

    adns__consistency(ads, 0, cc_entex);
    *ads_r = ads;
    return 0;
}

int adns_init_strcfg(adns_state * ads_r, adns_initflags flags,
                     FILE * diagfile, const char *configtext)
{
    adns_state ads = NULL;
    int r;

    r = init_begin(&ads, flags, diagfile);
    if (r)
        return r;

    readconfigtext(ads, configtext, "<supplied configuration text>");
    if (ads->configerrno) {
        r = ads->configerrno;
        init_abort(ads);
        return r;
    }

    r = init_finish(ads);
    if (r)
        return r;
    adns__consistency(ads, 0, cc_entex);
    *ads_r = ads;
    return 0;
}


void adns_finish(adns_state ads)
{
    adns__consistency(ads, 0, cc_entex);
    for (;;) {
        if (ads->udpw.head)
            adns_cancel(ads->udpw.head);
        else if (ads->tcpw.head)
            adns_cancel(ads->tcpw.head);
        else if (ads->childw.head)
            adns_cancel(ads->childw.head);
        else if (ads->output.head)
            adns_cancel(ads->output.head);
        else
            break;
    }
    deno_sockclose(ads->udpsocket);
    if (ads->tcpsocket >= 0) {
        deno_sockclose(ads->tcpsocket);
    }
    adns__vbuf_free(&ads->tcpsend);
    adns__vbuf_free(&ads->tcprecv);
    freesearchlist(ads);
    free(ads);
}

void adns_forallqueries_begin(adns_state ads)
{
    adns__consistency(ads, 0, cc_entex);
    ads->forallnext =
        ads->udpw.head ? ads->udpw.head :
        ads->tcpw.head ? ads->tcpw.head :
        ads->childw.head ? ads->childw.head : ads->output.head;
}

adns_query adns_forallqueries_next(adns_state ads, void **context_r)
{
    adns_query qu, nqu;

    adns__consistency(ads, 0, cc_entex);
    nqu = ads->forallnext;
    for (;;) {
        qu = nqu;
        if (!qu)
            return 0;
        if (qu->next) {
            nqu = qu->next;
        } else if (qu == ads->udpw.tail) {
            nqu =
                ads->tcpw.head ? ads->tcpw.head :
                ads->childw.head ? ads->childw.head : ads->output.head;
        } else if (qu == ads->tcpw.tail) {
            nqu = ads->childw.head ? ads->childw.head : ads->output.head;
        } else if (qu == ads->childw.tail) {
            nqu = ads->output.head;
        } else {
            nqu = 0;
        }
        if (!qu->parent)
            break;
    }
    ads->forallnext = nqu;
    if (context_r)
        *context_r = qu->ctx.ext;
    return qu;
}

int vbuf__append_quoted1035(vbuf * vb, const byte * buf, int len)
{
    char qbuf[10];
    int i, ch;

    while (len) {
        qbuf[0] = 0;
        for (i = 0; i < len; i++) {
            ch = buf[i];
            if (ch <= ' ' || ch >= 127) {
                ircsprintf(qbuf, "\\%03o", ch);
                break;
            } else if (!ctype_domainunquoted(ch)) {
                ircsprintf(qbuf, "\\%c", ch);
                break;
            }
        }
        if (!adns__vbuf_append(vb, buf, i)
            || !adns__vbuf_append(vb, (const byte *) qbuf, strlen(qbuf)))
            return 0;
        if (i < len)
            i++;
        buf += i;
        len -= i;
    }
    return 1;
}

void adns__findlabel_start(findlabel_state * afls, adns_state ads,
                           int serv, adns_query qu,
                           const byte * dgram, int dglen, int max,
                           int dmbegin, int *dmend_rlater)
{
    afls->ads = ads;
    afls->qu = qu;
    afls->serv = serv;
    afls->dgram = dgram;
    afls->dglen = dglen;
    afls->max = max;
    afls->cbyte = dmbegin;
    afls->namelen = 0;
    afls->dmend_r = dmend_rlater;
}

/*************************************************************************/

adns_status adns__findlabel_next(findlabel_state * afls,
                                 int *lablen_r, int *labstart_r)
{
    int lablen, jumpto;
    const char *dgram;

    dgram = (const char *) afls->dgram;
    for (;;) {
        if (afls->cbyte >= afls->dglen)
            goto x_truncated;
        if (afls->cbyte >= afls->max)
            goto x_badresponse;
        GET_B(afls->cbyte, lablen);
        if (!(lablen & 0x0c0))
            break;
        if ((lablen & 0x0c0) != 0x0c0)
            return adns_s_unknownformat;
        if (afls->cbyte >= afls->dglen)
            goto x_truncated;
        if (afls->cbyte >= afls->max)
            goto x_badresponse;
        GET_B(afls->cbyte, jumpto);
        jumpto |= (lablen & 0x3f) << 8;
        if (afls->dmend_r)
            *(afls->dmend_r) = afls->cbyte;
        afls->cbyte = jumpto;
        afls->dmend_r = 0;
        afls->max = afls->dglen + 1;
    }
    if (labstart_r)
        *labstart_r = afls->cbyte;
    if (lablen) {
        if (afls->namelen)
            afls->namelen++;
        afls->namelen += lablen;
        if (afls->namelen > DNS_MAXDOMAIN)
            return adns_s_answerdomaintoolong;
        afls->cbyte += lablen;
        if (afls->cbyte > afls->dglen)
            goto x_truncated;
        if (afls->cbyte > afls->max)
            goto x_badresponse;
    } else {
        if (afls->dmend_r)
            *(afls->dmend_r) = afls->cbyte;
    }
    *lablen_r = lablen;
    return adns_s_ok;

  x_truncated:
    *lablen_r = -1;
    return adns_s_ok;

  x_badresponse:
    alog(LOG_ADNS, "ADNS: label in domain runs beyond end of domain");
    return adns_s_invalidresponse;
}

/*************************************************************************/

adns_status adns__parse_domain(adns_state ads, int serv, adns_query qu,
                               vbuf * vb, adns_queryflags flags,
                               const byte * dgram, int dglen,
                               int *cbyte_io, int max)
{
    findlabel_state afls;

    adns__findlabel_start(&afls, ads, serv, qu, dgram, dglen, max,
                          *cbyte_io, cbyte_io);
    vb->used = 0;
    return adns__parse_domain_more(&afls, ads, qu, vb, flags, dgram);
}

/*************************************************************************/

adns_status adns__parse_domain_more(findlabel_state * afls, __attribute__((unused))adns_state ads,
                                    __attribute__((unused))adns_query qu, vbuf * vb,
                                    parsedomain_flags flags,
                                    const byte * dgram)
{
    int lablen, labstart, i, ch, first;
    adns_status st;

    first = 1;
    for (;;) {
        st = adns__findlabel_next(afls, &lablen, &labstart);
        if (st)
            return st;
        if (lablen < 0) {
            vb->used = 0;
            return adns_s_ok;
        }
        if (!lablen)
            break;
        if (first) {
            first = 0;
        } else {
            if (!adns__vbuf_append(vb, (const byte *) ".", 1))
                return adns_s_nomemory;
        }
        if (flags & pdf_quoteok) {
            if (!vbuf__append_quoted1035(vb, dgram + labstart, lablen))
                return adns_s_nomemory;
        } else {
            ch = dgram[labstart];
            if (!ctype_alpha(ch) && !ctype_digit(ch))
                return adns_s_answerdomaininvalid;
            for (i = labstart + 1; i < labstart + lablen; i++) {
                ch = dgram[i];
                if (ch != '-' && !ctype_alpha(ch)
                    && !ctype_digit(ch))
                    return adns_s_answerdomaininvalid;
            }
            if (!adns__vbuf_append(vb, dgram + labstart, lablen))
                return adns_s_nomemory;
        }
    }
    if (!adns__vbuf_append(vb, (const byte *) "", 1))
        return adns_s_nomemory;
    return adns_s_ok;
}

/*************************************************************************/

adns_status adns__findrr_anychk(adns_query qu, int serv,
                                const byte * dgram, int dglen,
                                int *cbyte_io, int *type_r, int *class_r,
                                unsigned long *ttl_r, int *rdlen_r,
                                int *rdstart_r, const byte * eo_dgram,
                                int eo_dglen, int eo_cbyte,
                                int *eo_matched_r)
{
    findlabel_state afls;
    findlabel_state eo_fls;
    int cbyte;

    int tmp, rdlen, mismatch;
    unsigned long ttl;
    int lablen, labstart, ch;
    int eo_lablen, eo_labstart, eo_ch;
    adns_status st;

    eo_fls.dgram = NULL;
    eo_fls.dglen = 0;
    eo_fls.max = 0;
    eo_fls.cbyte = 0;

    cbyte = *cbyte_io;

    adns__findlabel_start(&afls, qu->ads, serv, qu, dgram, dglen, dglen,
                          cbyte, &cbyte);
    if (eo_dgram) {
        adns__findlabel_start(&eo_fls, qu->ads, -1, 0, eo_dgram,
                              eo_dglen, eo_dglen, eo_cbyte, 0);
        mismatch = 0;
    } else {
        mismatch = 1;
    }

    for (;;) {
        st = adns__findlabel_next(&afls, &lablen, &labstart);
        if (st)
            return st;
        if (lablen < 0)
            goto x_truncated;

        if (!mismatch) {
            st = adns__findlabel_next(&eo_fls, &eo_lablen, &eo_labstart);
            assert(!st);
            assert(eo_lablen >= 0);
            if (lablen != eo_lablen)
                mismatch = 1;
            while (!mismatch && eo_lablen-- > 0) {
                ch = dgram[labstart++];
                if (ctype_alpha(ch))
                    ch &= ~32;
                eo_ch = eo_dgram[eo_labstart++];
                if (ctype_alpha(eo_ch))
                    eo_ch &= ~32;
                if (ch != eo_ch)
                    mismatch = 1;
            }
        }
        if (!lablen)
            break;
    }
    if (eo_matched_r)
        *eo_matched_r = !mismatch;

    if (cbyte + 10 > dglen)
        goto x_truncated;
    GET_W(cbyte, tmp);
    *type_r = tmp;
    GET_W(cbyte, tmp);
    *class_r = tmp;

    GET_L(cbyte, ttl);
    if (ttl > MAXTTLBELIEVE)
        ttl = MAXTTLBELIEVE;
    *ttl_r = ttl;

    GET_W(cbyte, rdlen);
    if (rdlen_r)
        *rdlen_r = rdlen;
    if (rdstart_r)
        *rdstart_r = cbyte;
    cbyte += rdlen;
    if (cbyte > dglen)
        goto x_truncated;
    *cbyte_io = cbyte;
    return adns_s_ok;

  x_truncated:
    *type_r = -1;
    return 0;
}

/*************************************************************************/

adns_status adns__findrr(adns_query qu, int serv,
                         const byte * dgram, int dglen, int *cbyte_io,
                         int *type_r, int *class_r, unsigned long *ttl_r,
                         int *rdlen_r, int *rdstart_r,
                         int *ownermatchedquery_r)
{
    if (!ownermatchedquery_r) {
        return adns__findrr_anychk(qu, serv,
                                   dgram, dglen, cbyte_io,
                                   type_r, class_r, ttl_r, rdlen_r,
                                   rdstart_r, 0, 0, 0, 0);
    } else if (!qu->cname_dgram) {
        return adns__findrr_anychk(qu, serv,
                                   dgram, dglen, cbyte_io,
                                   type_r, class_r, ttl_r, rdlen_r,
                                   rdstart_r, qu->query_dgram,
                                   qu->query_dglen, DNS_HDRSIZE,
                                   ownermatchedquery_r);
    } else {
        return adns__findrr_anychk(qu, serv,
                                   dgram, dglen, cbyte_io,
                                   type_r, class_r, ttl_r, rdlen_r,
                                   rdstart_r, qu->cname_dgram,
                                   qu->cname_dglen,
                                   qu->cname_begin, ownermatchedquery_r);
    }
}

/* *INDENT-ON* */

#endif
