/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */


typedef struct MD5Context MD5_CTX;

/************************************************************************/
/* MD5									*/
/************************************************************************/

struct MD5Context {
	uint32 buf[4];		/* Current digest state/value.		*/
	uint32 bits[2];		/* Number of bits hashed so far.	*/
	unsigned char in[64];	/* Residual input buffer.		*/
};

static void byteReverse(unsigned char *buf, unsigned longs);
void MD5Init(struct MD5Context *ctx);
void MD5Transform(uint32 buf[4], uint32 const in[16]);
void MD5Update(struct MD5Context *ctx, unsigned const char *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
char *md5(const char *str);
int is_md5(const char *passwd);

/* The four core functions - F1 is optimized somewhat */
/* #define F1(x, y, z) (x & y | ~x & z) */
/** Helper function for first round of MD5. */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
/** Helper function for second round of MD5. */
#define F2(x, y, z) F1(z, x, y)
/** Helper function for third round of MD5. */
#define F3(x, y, z) (x ^ y ^ z)
/** Helper function for fourth round of MD5. */
#define F4(x, y, z) (y ^ (x | ~z))

/** Step function for each round of MD5 */
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

