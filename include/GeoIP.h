/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/* GeoIP.h
 *
 * Copyright (C) 2006 MaxMind LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GEOIP_H
#define GEOIP_H

#if defined(_WIN32)
#define FILETIME_TO_USEC(ft) (((unsigned __int64) ft.dwHighDateTime << 32 | ft.dwLowDateTime) / 10)
#endif /* !defined(_WIN32) */

#define SEGMENT_RECORD_LENGTH 3
#define STANDARD_RECORD_LENGTH 3
#define ORG_RECORD_LENGTH 4
#define MAX_RECORD_LENGTH 4
#define NUM_DB_TYPES 20

/* 128 bit address in network order */
typedef struct in6_addr geoipv6_t;

#define GEOIP_CHKBIT_V6(bit,ptr) (ptr[((127UL - bit) >> 3)] & (1UL << (~(127 - bit) & 7)))

typedef struct GeoIPTag {
  FILE *GeoIPDatabase;
  char *file_path;
	unsigned char *cache;
	unsigned char *index_cache;
	unsigned int *databaseSegments;
	char databaseType;
	time_t mtime;
	int flags;
	off_t	size;
	char record_length;
	int charset; /* 0 iso-8859-1 1 utf8 */
	int record_iter; /* used in GeoIP_next_record */
	int netmask; /* netmask of last lookup - set using depth in _GeoIP_seek_record */
	time_t last_mtime_check;
} GeoIP;


typedef enum {
	GEOIP_CHARSET_ISO_8859_1 = 0,
	GEOIP_CHARSET_UTF8       = 1
} GeoIPCharset;

typedef struct GeoIPRegionTag {
	char country_code[3];
	char region[3];
} GeoIPRegion;

typedef enum {
	GEOIP_STANDARD = 0,
	GEOIP_MEMORY_CACHE = 1,
	GEOIP_CHECK_CACHE = 2,
	GEOIP_INDEX_CACHE = 4,
	GEOIP_MMAP_CACHE = 8
} GeoIPOptions;

typedef enum {
	GEOIP_COUNTRY_EDITION     = 1,
	GEOIP_REGION_EDITION_REV0 = 7,
	GEOIP_CITY_EDITION_REV0   = 6,
	GEOIP_ORG_EDITION         = 5,
	GEOIP_ISP_EDITION         = 4,
	GEOIP_CITY_EDITION_REV1   = 2,
	GEOIP_REGION_EDITION_REV1 = 3,
	GEOIP_PROXY_EDITION       = 8,
	GEOIP_ASNUM_EDITION       = 9,
	GEOIP_NETSPEED_EDITION    = 10,
	GEOIP_DOMAIN_EDITION      = 11,
	GEOIP_COUNTRY_EDITION_V6  = 12
} GeoIPDBTypes;

typedef enum {
	GEOIP_ANON_PROXY = 1,
	GEOIP_HTTP_X_FORWARDED_FOR_PROXY = 2,
	GEOIP_HTTP_CLIENT_IP_PROXY = 3
} GeoIPProxyTypes;

typedef enum {
	GEOIP_UNKNOWN_SPEED = 0,
	GEOIP_DIALUP_SPEED = 1,
	GEOIP_CABLEDSL_SPEED = 2,
	GEOIP_CORPORATE_SPEED = 3
} GeoIPNetspeedValues;

extern char **GeoIPDBFileName;
extern const char * GeoIPDBDescription[NUM_DB_TYPES];
extern const char *GeoIPCountryDBFileName;
extern const char *GeoIPRegionDBFileName;
extern const char *GeoIPCityDBFileName;
extern const char *GeoIPOrgDBFileName;
extern const char *GeoIPISPDBFileName;

/* Warning: do not use those arrays as doing so may break your
 * program with newer GeoIP versions */
extern const char GeoIP_country_code[253][3];
extern const char GeoIP_country_code3[253][4];
extern const char * GeoIP_country_name[253];
extern const char GeoIP_country_continent[253][3];

#ifdef DLL
#define GEOIP_API __declspec(dllexport)
#else
#define GEOIP_API
#endif  /* DLL */

GEOIP_API void GeoIP_setup_custom_directory(char *dir);
GEOIP_API GeoIP* GeoIP_open_type (int type, int flags);
GEOIP_API GeoIP* GeoIP_new(int flags);
GEOIP_API GeoIP* GeoIP_open(const char * filename, int flags);
GEOIP_API int GeoIP_db_avail(int type);
GEOIP_API void GeoIP_delete(GeoIP* gi);
GEOIP_API const char *GeoIP_country_code_by_addr (GeoIP* gi, const char *addr);
GEOIP_API const char *GeoIP_country_code_by_name (GeoIP* gi, const char *host);
GEOIP_API const char *GeoIP_country_code3_by_addr (GeoIP* gi, const char *addr);
GEOIP_API const char *GeoIP_country_code3_by_name (GeoIP* gi, const char *host);
GEOIP_API const char *GeoIP_country_name_by_addr (GeoIP* gi, const char *addr);
GEOIP_API const char *GeoIP_country_name_by_name (GeoIP* gi, const char *host);
GEOIP_API const char *GeoIP_country_name_by_ipnum (GeoIP* gi, unsigned long ipnum);
GEOIP_API const char *GeoIP_country_code_by_ipnum (GeoIP* gi, unsigned long ipnum);
GEOIP_API const char *GeoIP_country_code3_by_ipnum (GeoIP* gi, unsigned long ipnum);

/* */
GEOIP_API const char *GeoIP_country_name_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
GEOIP_API const char *GeoIP_country_code_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
GEOIP_API const char *GeoIP_country_code3_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);

/* Deprecated - for backwards compatibility only */
GEOIP_API int GeoIP_country_id_by_addr (GeoIP* gi, const char *addr);
GEOIP_API int GeoIP_country_id_by_name (GeoIP* gi, const char *host);
GEOIP_API char *GeoIP_org_by_addr (GeoIP* gi, const char *addr);
GEOIP_API char *GeoIP_org_by_name (GeoIP* gi, const char *host);
GEOIP_API char *GeoIP_org_by_ipnum (GeoIP* gi, unsigned long ipnum);

GEOIP_API char *GeoIP_org_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
GEOIP_API char *GeoIP_org_by_addr_v6 (GeoIP* gi, const char *addr);
GEOIP_API char *GeoIP_org_by_name_v6 (GeoIP* gi, const char *name);

/* End deprecated */

GEOIP_API int GeoIP_id_by_addr (GeoIP* gi, const char *addr);
GEOIP_API int GeoIP_id_by_name (GeoIP* gi, const char *host);
GEOIP_API int GeoIP_id_by_ipnum (GeoIP* gi, unsigned long ipnum);

GEOIP_API int GeoIP_id_by_addr_v6 (GeoIP* gi, const char *addr);
GEOIP_API int GeoIP_id_by_name_v6 (GeoIP* gi, const char *host);
GEOIP_API int GeoIP_id_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);

GEOIP_API GeoIPRegion * GeoIP_region_by_addr (GeoIP* gi, const char *addr);
GEOIP_API GeoIPRegion * GeoIP_region_by_name (GeoIP* gi, const char *host);
GEOIP_API GeoIPRegion * GeoIP_region_by_ipnum (GeoIP *gi, unsigned long ipnum);

GEOIP_API GeoIPRegion * GeoIP_region_by_addr_v6 (GeoIP* gi, const char *addr);
GEOIP_API GeoIPRegion * GeoIP_region_by_name_v6 (GeoIP* gi, const char *host);
GEOIP_API GeoIPRegion * GeoIP_region_by_ipnum_v6 (GeoIP *gi, geoipv6_t ipnum);

/* Warning - don't call this after GeoIP_assign_region_by_inetaddr calls */
GEOIP_API void GeoIPRegion_delete (GeoIPRegion *gir);

GEOIP_API void GeoIP_assign_region_by_inetaddr(GeoIP* gi, unsigned long inetaddr, GeoIPRegion *gir);

GEOIP_API void GeoIP_assign_region_by_inetaddr_v6(GeoIP* gi, geoipv6_t inetaddr, GeoIPRegion *gir);

/* Used to query GeoIP Organization, ISP and AS Number databases */
GEOIP_API char *GeoIP_name_by_ipnum (GeoIP* gi, unsigned long ipnum);
GEOIP_API char *GeoIP_name_by_addr (GeoIP* gi, const char *addr);
GEOIP_API char *GeoIP_name_by_name (GeoIP* gi, const char *host);

GEOIP_API char *GeoIP_name_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum);
GEOIP_API char *GeoIP_name_by_addr_v6 (GeoIP* gi, const char *addr);
GEOIP_API char *GeoIP_name_by_name_v6 (GeoIP* gi, const char *name);

/** return two letter country code */
GEOIP_API const char* GeoIP_code_by_id(int id);

/** return three letter country code */
GEOIP_API const char* GeoIP_code3_by_id(int id);

/** return full name of country */
GEOIP_API const char* GeoIP_name_by_id(int id);

/** return continent of country */
GEOIP_API const char* GeoIP_continent_by_id(int id);

/** return id by country code **/
GEOIP_API int GeoIP_id_by_code(const char *country);

/** return return number of known countries */
GEOIP_API unsigned GeoIP_num_countries(void);

GEOIP_API char *GeoIP_database_info (GeoIP* gi);
GEOIP_API unsigned char GeoIP_database_edition (GeoIP* gi);

GEOIP_API int GeoIP_charset (GeoIP* gi);
GEOIP_API int GeoIP_set_charset (GeoIP* gi, int charset);

GEOIP_API int GeoIP_last_netmask (GeoIP* gi);
GEOIP_API char **GeoIP_range_by_ip (GeoIP* gi, const char *addr);
GEOIP_API void GeoIP_range_by_ip_delete(char **ptr);

/* Convert region code to region name */
GEOIP_API const char * GeoIP_region_name_by_code(const char *country_code, const char *region_code);

/* Get timezone from country and region code */
GEOIP_API const char * GeoIP_time_zone_by_country_and_region(const char *country_code, const char *region_code);

#ifdef BSD
#define memcpy(dest, src, n) bcopy(src, dest, n)
#endif

#ifdef __cplusplus
}
#endif

#endif /* GEOIP_H */
