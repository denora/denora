/* Hacks for Minix
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

#include <net/hton.h>
#include <net/netlib.h>
#include <net/gen/in.h>
#include <net/gen/tcp.h>
#include <net/gen/tcp_io.h>
#include <net/gen/udp.h>
#include <net/gen/udp_io.h>
#include <sys/ioctl.h>

/* mmap and munmap hack */
#include <sys/mman.h>

#ifndef mmap
#define mmap(a, b, c, d, e, f) minix_mmap(a, b, c, d, e, f)
#endif

#ifndef munmap
#define munmap(a, b) minix_munmap(a,b)
#endif