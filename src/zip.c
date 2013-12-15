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

#ifdef HAVE_LIBZ

static char unzipbuf[UNZIP_BUFFER_SIZE];
static char zipbuf[ZIP_BUFFER_SIZE];
void zip_free(void);

aZdata *zip;

/*************************************************************************/

int zip_init(int compressionlevel)
{
	zip = (aZdata *) malloc(sizeof(aZdata));
	zip->incount = 0;
	zip->outcount = 0;

	zip->in = (z_stream *) malloc(sizeof(z_stream));
	bzero(zip->in, sizeof(z_stream));
	zip->in->total_in = 0;
	zip->in->total_out = 0;
	zip->in->zalloc = NULL;
	zip->in->zfree = NULL;
	zip->in->data_type = Z_ASCII;
	if (inflateInit(zip->in) != Z_OK)
	{
		zip->out = NULL;
		return 0;
	}

	zip->out = (z_stream *) malloc(sizeof(z_stream));
	bzero(zip->out, sizeof(z_stream));
	zip->out->total_in = 0;
	zip->out->total_out = 0;
	zip->out->zalloc = NULL;
	zip->out->zfree = NULL;
	zip->out->data_type = Z_ASCII;
	if (deflateInit(zip->out, compressionlevel) != Z_OK)
	{
		return 0;
	}

	return 1;
}

/*************************************************************************/

void zip_free(void)
{
	if (zip)
	{
		if (zip->in)
			inflateEnd(zip->in);
		free(zip->in);
		zip->in = NULL;
		if (zip->out)
			deflateEnd(zip->out);
		free(zip->out);
		zip->out = NULL;
		free(zip);
		zip = NULL;
	}
}

/*************************************************************************/

char *unzip_packet(const char *buffer, int length)
{
	z_stream *zin = zip->in;
	int r;
	char *p;

	if (zip->incount)
	{
		memcpy((void *) unzipbuf, (void *) zip->inbuf, zip->incount);
		zin->avail_out = UNZIP_BUFFER_SIZE - zip->incount;
		zin->next_out = (Bytef *) (unzipbuf + zip->incount);
		zip->incount = 0;
		zip->inbuf[0] = '\0';
	}
	else
	{
		if (!buffer)
		{
			length = -1;
			return ((char *) NULL);
		}
		zin->next_in = (Bytef *) buffer;
		zin->avail_in = length;
		zin->next_out = (Bytef *) unzipbuf;
		zin->avail_out = UNZIP_BUFFER_SIZE;
	}

	switch (r = inflate(zin, Z_NO_FLUSH))
	{
		case Z_OK:
			if (zin->avail_in)
			{
				zip->incount = 0;

				if (zin->avail_out == 0)
				{
					alog(LOG_ERROR,
					     "Overflowed unzipbuf increase UNZIP_BUFFER_SIZE");
					if ((zin->next_out[0] == '\n')
					        || (zin->next_out[0] == '\r'))
					{
						zip->inbuf[0] = '\n';
						zip->incount = 1;
					}
					else
					{
						for (p = (char *) zin->next_out; p >= unzipbuf;)
						{
							if ((*p == '\r') || (*p == '\n'))
								break;
							zin->avail_out++;
							p--;
							zip->incount++;
						}
						if (p == unzipbuf)
						{
							zip->incount = 0;
							zip->inbuf[0] = '\0';   /* only for debugger */
							length = -1;
							return ((char *) NULL);
						}
						p++;
						zip->incount--;
						memcpy((void *) zip->inbuf, (void *) p, zip->incount);
					}
				}
				else
				{
					length = -1;
					return ((char *) NULL);
				}
			}

			length = UNZIP_BUFFER_SIZE - zin->avail_out;
			return (char *) unzipbuf;

		case Z_BUF_ERROR:
			if (zin->avail_out == 0)
			{
				alog(LOG_ERROR, "inflate() returned Z_BUF_ERROR: %s",
				     (zin->msg) ? zin->msg : "?");
				length = -1;
			}
			break;

		case Z_DATA_ERROR:         /* the buffer might not be compressed.. */
			if (!strncmp("ERROR ", buffer, 6))
			{
				zip->first = 0;
				return (char *) buffer;
			}
			alog(LOG_ERROR,
			     "inflate() error: * Are you perhaps linking zipped with non-zipped? *");

		default:
			alog(LOG_ERROR, "inflate() error(%d): %s", r,
			     (zin->msg) ? zin->msg : "?");
			length = -1;
			break;
	}
	return ((char *) NULL);
}

/*************************************************************************/

char *zip_buffer(char *buffer, int *length, int flush)
{
	z_stream *zout = zip->out;
	int r;

	if (buffer)
	{
		memcpy((void *) (zip->outbuf + zip->outcount), (void *) buffer,
		       *length);
		zip->outcount += *length;
	}
	*length = 0;

#if 0
	if (!flush && ((zip->outcount < ZIP_MINIMUM) ||
	               ((zip->outcount < (ZIP_MAXIMUM - BUFSIZE)) &&
	                CBurst(cptr))))
#else
	if (!flush && (zip->outcount < ZIP_MINIMUM))
#endif
		return ((char *) NULL);

	zout->next_in = (Bytef *) zip->outbuf;
	zout->avail_in = zip->outcount;
	zout->next_out = (Bytef *) zipbuf;
	zout->avail_out = ZIP_BUFFER_SIZE;

	switch (r = deflate(zout, Z_PARTIAL_FLUSH))
	{
		case Z_OK:
			if (zout->avail_in)
			{
				alog(LOG_ERROR,
				     "deflate() didn't process all available data!");
			}
			zip->outcount = 0;
			*length = ZIP_BUFFER_SIZE - zout->avail_out;
			return zipbuf;

		default:
			alog(LOG_ERROR, "deflate() error(%d): %s", r,
			     (zout->msg) ? zout->msg : "?");
			*length = -1;
			break;
	}
	return ((char *) NULL);
}

/*************************************************************************/

#endif
