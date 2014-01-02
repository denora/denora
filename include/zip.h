/*
 *
 * (c) 2004-2014 Denora Team
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

#ifdef HAVE_LIBZ

typedef struct  Zdata   aZdata;

/* the minimum amount of data needed to trigger compression */
#define ZIP_MINIMUM     4096

/* the maximum amount of data to be compressed (can actually be a bit more) */
#define ZIP_MAXIMUM     8192	/* WARNING: *DON'T* CHANGE THIS!!!! */

struct Zdata
{
	z_stream *in;		/* input zip stream data */
	z_stream *out;		/* output zip stream data */
	char inbuf[ZIP_MAXIMUM];	/* incoming zipped buffer */
	char outbuf[ZIP_MAXIMUM];	/* outgoing (unzipped) buffer */
	int  incount;		/* size of inbuf content */
	int  outcount;		/* size of outbuf content */
	int first; /* First message? */
};

#define ZIP_DEFAULT_LEVEL 2

#define ZIP_BUFFER_SIZE         (ZIP_MAXIMUM + BUFSIZE)
#define UNZIP_BUFFER_SIZE       6 * ZIP_BUFFER_SIZE

#endif
