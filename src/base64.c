/*
 *
 * (c) 2004-2013 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * Based on the original code of Unreal IRCD
 * Based on the original code of NeoStats
 *
 *
 *
 */

#include "denora.h"

static char *int_to_base64(long);
static long base64_to_int(char *);
static const char Base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Pad64 = '=';
static unsigned char dtable[512];

/*************************************************************************/

void buffer_new(struct buffer_st *b)
{
	b->length = 512;
	b->data = malloc(sizeof(char) * (b->length));
	b->data[0] = 0;
	b->ptr = b->data;
	b->offset = 0;
}

/*************************************************************************/

void buffer_add(struct buffer_st *b, char c)
{
	*(b->ptr++) = c;
	b->offset++;
	if (b->offset == b->length)
	{
		b->length += 512;
		b->data = realloc(b->data, b->length);
		b->ptr = b->data + b->offset;
	}
}

/*************************************************************************/

void buffer_delete(struct buffer_st *b)
{
	if (b->data)
		free(b->data);
	b->length = 0;
	b->offset = 0;
	b->ptr = NULL;
	b->data = NULL;
}

/*************************************************************************/

void base64_encode(struct buffer_st *b, const char *source, int length)
{
	int i, hiteof = 0;
	int offset = 0;

	buffer_new(b);

	/*    Fill dtable with character encodings.  */

	for (i = 0; i < 26; i++)
	{
		dtable[i] = 'A' + i;
		dtable[26 + i] = 'a' + i;
	}
	for (i = 0; i < 10; i++)
	{
		dtable[52 + i] = '0' + i;
	}
	dtable[62] = '+';
	dtable[63] = '/';

	while (!hiteof)
	{
		unsigned char igroup[3], ogroup[4];
		int c, n;

		igroup[0] = igroup[1] = igroup[2] = 0;
		for (n = 0; n < 3; n++)
		{
			c = *(source++);
			offset++;
			if (offset > length)
			{
				hiteof = 1;
				break;
			}
			igroup[n] = (unsigned char) c;
		}
		if (n > 0)
		{
			ogroup[0] = dtable[igroup[0] >> 2];
			ogroup[1] = dtable[((igroup[0] & 3) << 4) | (igroup[1] >> 4)];
			ogroup[2] =
			    dtable[((igroup[1] & 0xF) << 2) | (igroup[2] >> 6)];
			ogroup[3] = dtable[igroup[2] & 0x3F];

			/* Replace characters in output stream with "=" pad
			   characters if fewer than three characters were
			   read from the end of the input stream. */

			if (n < 3)
			{
				ogroup[3] = '=';
				if (n < 2)
				{
					ogroup[2] = '=';
				}
			}
			for (i = 0; i < 4; i++)
			{
				buffer_add(b, ogroup[i]);
			}
		}
	}
}

/*************************************************************************/

void base64_decode(struct buffer_st *bfr, const char *source)
{
	int i;
	int offset = 0;
	int count;

	buffer_new(bfr);

	for (i = 0; i < 255; i++)
	{
		dtable[i] = 0x80;
	}
	for (i = 'A'; i <= 'Z'; i++)
	{
		dtable[i] = 0 + (i - 'A');
	}
	for (i = 'a'; i <= 'z'; i++)
	{
		dtable[i] = 26 + (i - 'a');
	}
	for (i = '0'; i <= '9'; i++)
	{
		dtable[i] = 52 + (i - '0');
	}
	dtable['+'] = 62;
	dtable['/'] = 63;
	dtable['='] = 0;


	/*CONSTANTCONDITION*/
	while (1)
	{
		unsigned char a[4], b[4], o[3];

		for (i = 0; i < 4; i++)
		{
			int c;
			while (1)
			{
				c = *(source++);
				offset++;
				if (isspace(c) || c == '\n' || c == '\r')
					continue;
				break;
			}
			a[i] = (unsigned char) c;
			b[i] = (unsigned char) dtable[c];
		}
		o[0] = (b[0] << 2) | (b[1] >> 4);
		o[1] = (b[1] << 4) | (b[2] >> 2);
		o[2] = (b[2] << 6) | b[3];
		i = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);
		count = 0;
		while (count < i)
		{
			buffer_add(bfr, o[count++]);
		}
		if (i < 3)
		{
			return;
		}
	}
}

/*************************************************************************/

static const char convert2y[] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'[', ']'
};

/* (From RFC1521 and draft-ietf-dnssec-secext-03.txt)
   The following encoding technique is taken from RFC 1521 by Borenstein
   and Freed.  It is reproduced here in a slightly edited form for
   convenience.

   A 65-character subset of US-ASCII is used, enabling 6 bits to be
   represented per printable character. (The extra 65th character, "=",
   is used to signify a special processing function.)

   The encoding process represents 24-bit groups of input bits as output
   strings of 4 encoded characters. Proceeding from left to right, a
   24-bit input group is formed by concatenating 3 8-bit input groups.
   These 24 bits are then treated as 4 concatenated 6-bit groups, each
   of which is translated into a single digit in the base64 alphabet.

   Each 6-bit group is used as an index into an array of 64 printable
   characters. The character referenced by the index is placed in the
   output string.

                         Table 1: The Base64 Alphabet

      Value Encoding  Value Encoding  Value Encoding  Value Encoding
          0 A            17 R            34 i            51 z
          1 B            18 S            35 j            52 0
          2 C            19 T            36 k            53 1
          3 D            20 U            37 l            54 2
          4 E            21 V            38 m            55 3
          5 F            22 W            39 n            56 4
          6 G            23 X            40 o            57 5
          7 H            24 Y            41 p            58 6
          8 I            25 Z            42 q            59 7
          9 J            26 a            43 r            60 8
         10 K            27 b            44 s            61 9
         11 L            28 c            45 t            62 +
         12 M            29 d            46 u            63 /
         13 N            30 e            47 v
         14 O            31 f            48 w         (pad) =
         15 P            32 g            49 x
         16 Q            33 h            50 y

   Special processing is performed if fewer than 24 bits are available
   at the end of the data being encoded.  A full encoding quantum is
   always completed at the end of a quantity.  When fewer than 24 input
   bits are available in an input group, zero bits are added (on the
   right) to form an integral number of 6-bit groups.  Padding at the
   end of the data is performed using the '=' character.

   Since all base64 input is an integral number of octets, only the
         -------------------------------------------------
   following cases can arise:

       (1) the final quantum of encoding input is an integral
           multiple of 24 bits; here, the final unit of encoded
	   output will be an integral multiple of 4 characters
	   with no "=" padding,
       (2) the final quantum of encoding input is exactly 8 bits;
           here, the final unit of encoded output will be two
	   characters followed by two "=" padding characters, or
       (3) the final quantum of encoding input is exactly 16 bits;
           here, the final unit of encoded output will be three
	   characters followed by one "=" padding character.
*/

/*************************************************************************/

/**
 * Encode a long int to base64 char
 * @param i long integer
 * @return base64 char
 */
char *base64enc(long i)
{
	SET_SEGV_LOCATION();
	return (i < 0 ? (char *) "0" : int_to_base64(i));
}

/*************************************************************************/

/**
 * Decode a base64 char to long int
 * @param b64 base64 character string
 * @return long int
 */
long base64dec(char *b64)
{
	SET_SEGV_LOCATION();
	return (b64 ? base64_to_int(b64) : 0);
}

/*************************************************************************/

/**
 * Encode a char into a Base64 string
 * @param src Source string to encode
 * @param srclength The length of the source
 * @param target Target string to place the base64 into
 * @param targsize the length of the target
 * @return int total lenght of base64 word
 */
int b64_encode(char *src, size_t srclength, char *target, size_t targsize)
{
	size_t datalength = 0;
	unsigned char input[3];
	unsigned char output[4];
	size_t i;

	SET_SEGV_LOCATION();

	while (2 < srclength)
	{
		input[0] = *src++;
		input[1] = *src++;
		input[2] = *src++;
		srclength -= 3;

		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);
		output[3] = input[2] & 0x3f;
		SET_SEGV_LOCATION();
		if (datalength + 4 > targsize)
			return (-1);
		target[datalength++] = Base64[output[0]];
		target[datalength++] = Base64[output[1]];
		target[datalength++] = Base64[output[2]];
		target[datalength++] = Base64[output[3]];
	}

	/* Now we worry about padding. */
	if (0 != srclength)
	{
		/* Get what's left. */
		input[0] = input[1] = input[2] = '\0';
		for (i = 0; i < srclength; i++)
			input[i] = *src++;

		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);
		SET_SEGV_LOCATION();
		if (datalength + 4 > targsize)
			return (-1);
		target[datalength++] = Base64[output[0]];
		target[datalength++] = Base64[output[1]];
		if (srclength == 1)
		{
			target[datalength++] = Pad64;
		}
		else
		{
			target[datalength++] = Base64[output[2]];
		}
		SET_SEGV_LOCATION();
		target[datalength++] = Pad64;
	}
	if (datalength >= targsize)
		return (-1);
	target[datalength] = '\0';  /* Returned value doesn't count \0. */
	return (datalength);
}

/*************************************************************************/

/**
 * Decode a base64 string into a char
 * @param src Source string to encode
 * @param target Target string to place the base64 into
 * @param targsize the length of the target
 * @return int total lenght of base64 word
 */
int b64_decode(char *src, char *target, size_t targsize)
{
	int tarindex, state, ch;
	char *pos;

	SET_SEGV_LOCATION();

	state = 0;
	tarindex = 0;

	while ((ch = *src++) != '\0')
	{
		if (isspace(ch))        /* Skip whitespace anywhere. */
			continue;

		if (ch == Pad64)
			break;

		pos = strchr(Base64, ch);
		if (pos == 0)           /* A non-base64 character. */
			return (-1);

		switch (state)
		{
			case 0:
				if (target)
				{
					if ((size_t) tarindex >= targsize)
						return (-1);
					target[tarindex] = (pos - Base64) << 2;
				}
				state = 1;
				break;
			case 1:
				SET_SEGV_LOCATION();
				if (target)
				{
					if ((size_t) tarindex + 1 >= targsize)
						return (-1);
					target[tarindex] |= (pos - Base64) >> 4;
					target[tarindex + 1] = ((pos - Base64) & 0x0f)
					                       << 4;
				}
				tarindex++;
				state = 2;
				break;
			case 2:
				SET_SEGV_LOCATION();
				if (target)
				{
					if ((size_t) tarindex + 1 >= targsize)
						return (-1);
					target[tarindex] |= (pos - Base64) >> 2;
					target[tarindex + 1] = ((pos - Base64) & 0x03)
					                       << 6;
				}
				tarindex++;
				state = 3;
				break;
			case 3:
				if (target)
				{
					if ((size_t) tarindex >= targsize)
						return (-1);
					target[tarindex] |= (pos - Base64);
				}
				tarindex++;
				state = 0;
				break;
			default:
				SET_SEGV_LOCATION();
				abort();
		}
	}

	/*
	 * We are done decoding Base-64 chars.  Let's see if we ended
	 * on a byte boundary, and/or with erroneous trailing characters.
	 */

	if (ch == Pad64)            /* We got a pad char. */
	{
		ch = *src++;            /* Skip it, get next. */
		switch (state)
		{
			case 0:                /* Invalid = in first position */
			case 1:                /* Invalid = in second position */
				return (-1);

			case 2:                /* Valid, means one byte of info */
				/* Skip any number of spaces. */
				for ((void) NULL; ch != '\0'; ch = *src++)
					if (!isspace(ch))
						break;
				/* Make sure there is another trailing = sign. */
				if (ch != Pad64)
					return (-1);
				ch = *src++;        /* Skip the = */
				/* Fall through to "single trailing =" case. */
				/* FALLTHROUGH */

			case 3:                /* Valid, means two bytes of info */
				/*
				 * We know this char is an =.  Is there anything but
				 * whitespace after it?
				 */
				for ((void) NULL; ch != '\0'; ch = *src++)
					if (!isspace(ch))
						SET_SEGV_LOCATION();
				return (-1);

				/*
				 * Now make sure for cases 2 and 3 that the "extra"
				 * bits that slopped past the last full byte were
				 * zeros.  If we don't check them, they become a
				 * subliminal channel.
				 */
				if (target && target[tarindex] != 0)
					return (-1);
		}
	}
	else
	{
		/*
		 * We ended by seeing the end of the string.  Make sure we
		 * have no partial bytes lying around.
		 */
		if (state != 0)
			SET_SEGV_LOCATION();
		return (-1);
	}

	return (tarindex);
}

/*************************************************************************/

/**
 * Encode IP address to base64
 * @param ip unsigned char
 * @return base64 address
 */
char *encode_ip(unsigned char *ip)
{
	static char buf[25];
	char ipbuf[50];
	struct in_addr ia;          /* For IPv4 */
	char *s_ip;                 /* Signed ip string */

	SET_SEGV_LOCATION();

	if (!ip)
		return sstrdup("*");

	if (strchr((char *) ip, ':'))
	{
		SET_SEGV_LOCATION();
		return NULL;
	}
	else
	{
		s_ip = str_signed(ip);
		inet_aton(s_ip, &ia);
		ircsnprintf(ipbuf, 50, "%uld", ia.s_addr);
		SET_SEGV_LOCATION();
		b64_encode((char *) &ipbuf, sizeof(struct in_addr), buf, 25);
		free(s_ip);
	}
	return sstrdup(buf);
}

/*************************************************************************/

char* decode_ip(char *buf)
{
	int len = strlen(buf);
	char targ[25];

	SET_SEGV_LOCATION();

	b64_decode(buf, targ, 25);

	if (len == 24)                /* IPv6 */
	{
		static char result[INET6_ADDRSTRLEN];
		return (char *)_GeoIP_inet_ntop(AF_INET6, (const struct in6_addr *)targ, result, INET6_ADDRSTRLEN);
	}
	else if (len == 8)            /* IPv4 */
	{
		static char result[INET_ADDRSTRLEN];
		return (char *)_GeoIP_inet_ntop(AF_INET, (const struct in_addr *)targ, result, INET_ADDRSTRLEN);
	}
	else                          /* Error */
		return 0;
}

/*************************************************************************/

/* ':' and '#' and '&' and '+' and '@' must never be in this table. */
/* these tables must NEVER CHANGE! >) */
char int6_to_base64_map[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D',
	'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V',
	'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'{', '}'
};

char base64_to_int6_map[] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
	-1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, -1, 63, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

static const unsigned int convert2n[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 62, 0, 63, 0, 0,
	0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*************************************************************************/

static char *int_to_base64(long val)
{
	/* 32/6 == max 6 bytes for representation,
	 * +1 for the null, +1 for byte boundaries
	 */
	static char base64buf[8];
	long i = 7;

	base64buf[i] = '\0';

	SET_SEGV_LOCATION();

	/* Temporary debugging code.. remove before 2038 ;p.
	 * This might happen in case of 64bit longs (opteron/ia64),
	 * if the value is then too large it can easily lead to
	 * a buffer underflow and thus to a crash. -- Syzop
	 */
	if (val > 2147483647L)
	{
		abort();
	}

	do
	{
		base64buf[--i] = int6_to_base64_map[val & 63];
	}
	while (val >>= 6);
	SET_SEGV_LOCATION();

	return base64buf + i;
}

/*************************************************************************/

static long base64_to_int(char *b64)
{
	int v;

	SET_SEGV_LOCATION();

	v = base64_to_int6_map[(unsigned char) *b64++];

	if (!b64)
		return 0;

	while (*b64)
	{
		v <<= 6;
		v += base64_to_int6_map[(unsigned char) *b64++];
	}
	SET_SEGV_LOCATION();

	return v;
}

/*************************************************************************/

/**
 * Decode a base64 time stamp to long int
 * @param ts base64 time stamp
 * @return long int
 */
long base64dec_ts(char *ts)
{
	SET_SEGV_LOCATION();

	if (!ts)
	{
		return 0;
	}
	if (*ts == '!')
	{
		(void) *ts++;
		return base64dec(ts);
	}
	else
	{
		return strtoul(ts, NULL, 10);
	}
}

/*************************************************************************/

const char *inttobase64(char *buf, unsigned int v, unsigned int count)
{
	SET_SEGV_LOCATION();

	buf[count] = '\0';
	while (count > 0)
	{
		buf[--count] = convert2y[(v & 63)];
		v >>= 6;
	}
	SET_SEGV_LOCATION();
	return buf;
}

/*************************************************************************/

unsigned int base64toint(char *s)
{
	unsigned int i = convert2n[(unsigned char) *s++];
	while (*s)
	{
		i <<= 6;
		i += convert2n[(unsigned char) *s++];
	}
	return i;
}

/** Decode an IP address from base64.
 * @param[in] input Input buffer to decode.
 * @param[out] addr IP address structure to populate.
 */
void base64toip(char* input, char* addr)
{
	uint16 addrint[8];
	if (strlen(input) == 6)
	{
		unsigned int in = base64toint(input);
		/* An all-zero address should stay that way. */
		if (in)
		{
			addrint[0] = htons(in >> 16);
			addrint[1] = htons(in & 65535);
		}
		_GeoIP_inet_ntop(AF_INET, &addrint, addr, INET6_ADDRSTRLEN);
	}
	else
	{
		unsigned int pos = 0;
		do
		{
			if (*input == '_')
			{
				unsigned int left;
				for (left = (25 - strlen(input)) / 3 - pos; left; left--)
					addrint[pos++] = 0;
				input++;
			}
			else
			{
				unsigned short accum = convert2n[(unsigned char)*input++];
				accum = (accum << 6) | convert2n[(unsigned char)*input++];
				accum = (accum << 6) | convert2n[(unsigned char)*input++];
				addrint[pos++] = ntohs(accum);
			}
		}
		while (pos < 8);
		_GeoIP_inet_ntop(AF_INET6, &addrint, addr, INET6_ADDRSTRLEN);
	}
}

