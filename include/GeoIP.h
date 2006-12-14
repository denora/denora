/*************************************************************************/
/* (C) 2004-2006 Denora Team                                             */
/* Contact us at info@nomadirc.net                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Copyright (C) 2003 MaxMind LLC                                        */
/*                                                                       */
/* $Id: GeoIP.h 606 2006-05-13 11:10:09Z trystan $                      */
/*                                                                       */
/*************************************************************************/

#ifndef GEOIP_H
#define GEOIP_H

typedef struct GeoIPRecordTag {
	char *country_code;
	char *country_code3;
	char *country_name;
	char *region;
	char *city;
	char *postal_code;
	float latitude;
	float longitude;
	int dma_code;
	int area_code;
} GeoIPRecord;

typedef struct GeoIPTag {
  FILE *GeoIPDatabase;
  char *file_path;
	unsigned char *cache;
	unsigned char *index_cache;
	unsigned int *databaseSegments;
	char databaseType;
	time_t mtime;
	int flags;
	char record_length;
	int record_iter; /* used in GeoIP_next_record */
} GeoIP;

typedef enum {
	GEOIP_STANDARD = 0,
	GEOIP_MEMORY_CACHE = 1,
	GEOIP_CHECK_CACHE = 2,
	GEOIP_INDEX_CACHE = 4
} GeoIPOptions;

typedef enum {
	GEOIP_COUNTRY_EDITION     = 1
} GeoIPDBTypes;

GeoIP* GeoIP_open_type (int type, int flags);
GeoIP* GeoIP_new(int flags);
GeoIP* GeoIP_open(char *filename, int flags);
int GeoIP_db_avail(int type);
void GeoIP_delete(GeoIP* gi);
const char *GeoIP_country_code_by_addr (GeoIP* gi, const char *addr);
const char *GeoIP_country_code_by_name (GeoIP* gi, const char *host);
const char *GeoIP_country_code3_by_addr (GeoIP* gi, const char *addr);
const char *GeoIP_country_code3_by_name (GeoIP* gi, const char *host);
const char *GeoIP_country_name_by_addr (GeoIP* gi, const char *addr);
const char *GeoIP_country_name_by_name (GeoIP* gi, const char *host);
const char *GeoIP_country_code_by_ipnum (GeoIP* gi, unsigned long ipnum);
int GeoIP_id_by_addr (GeoIP* gi, const char *addr);
int GeoIP_id_by_name (GeoIP* gi, const char *host);
int GeoIP_id_by_ipnum (GeoIP* gi, unsigned long ipnum);
char *GeoIP_name_by_addr (GeoIP* gi, const char *addr);
char *GeoIP_name_by_name (GeoIP* gi, const char *host);
char *GeoIP_database_info (GeoIP* gi);
unsigned int _seek_record (GeoIP *gi, unsigned long ipnum);
unsigned long _addr_to_num (const char *addr);

#endif /* GEOIP_H */
