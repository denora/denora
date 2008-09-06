
/* GeoIP.c
 *
 * Copyright (C) 2003 MaxMind LLC  All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * $Id$
 */

#include "denora.h"

void _setup_dbfilename(void);
const int FULL_RECORD_LENGTH = 50;
char *GeoIPDBFileName;
void _setup_segments(GeoIP * lgi);
int _check_mtime(GeoIP * lgi);
unsigned long lookupaddress(const char *host);
GeoIPRecord *_extract_record(GeoIP * lgi, unsigned int seek_record,
                             int *next_record_ptr);

const char GeoIP_country_code[253][3] =
    { "--", "AP", "EU", "AD", "AE", "AF", "AG", "AI", "AL", "AM", "AN",
    "AO", "AQ", "AR", "AS", "AT", "AU", "AW", "AZ", "BA", "BB",
    "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BM", "BN", "BO",
    "BR", "BS", "BT", "BV", "BW", "BY", "BZ", "CA", "CC", "CD",
    "CF", "CG", "CH", "CI", "CK", "CL", "CM", "CN", "CO", "CR",
    "CU", "CV", "CX", "CY", "CZ", "DE", "DJ", "DK", "DM", "DO",
    "DZ", "EC", "EE", "EG", "EH", "ER", "ES", "ET", "FI", "FJ",
    "FK", "FM", "FO", "FR", "FX", "GA", "GB", "GD", "GE", "GF",
    "GH", "GI", "GL", "GM", "GN", "GP", "GQ", "GR", "GS", "GT",
    "GU", "GW", "GY", "HK", "HM", "HN", "HR", "HT", "HU", "ID",
    "IE", "IL", "IN", "IO", "IQ", "IR", "IS", "IT", "JM", "JO",
    "JP", "KE", "KG", "KH", "KI", "KM", "KN", "KP", "KR", "KW",
    "KY", "KZ", "LA", "LB", "LC", "LI", "LK", "LR", "LS", "LT",
    "LU", "LV", "LY", "MA", "MC", "MD", "MG", "MH", "MK", "ML",
    "MM", "MN", "MO", "MP", "MQ", "MR", "MS", "MT", "MU", "MV",
    "MW", "MX", "MY", "MZ", "NA", "NC", "NE", "NF", "NG", "NI",
    "NL", "NO", "NP", "NR", "NU", "NZ", "OM", "PA", "PE", "PF",
    "PG", "PH", "PK", "PL", "PM", "PN", "PR", "PS", "PT", "PW",
    "PY", "QA", "RE", "RO", "RU", "RW", "SA", "SB", "SC", "SD",
    "SE", "SG", "SH", "SI", "SJ", "SK", "SL", "SM", "SN", "SO",
    "SR", "ST", "SV", "SY", "SZ", "TC", "TD", "TF", "TG", "TH",
    "TJ", "TK", "TM", "TN", "TO", "TL", "TR", "TT", "TV", "TW",
    "TZ", "UA", "UG", "UM", "US", "UY", "UZ", "VA", "VC", "VE",
    "VG", "VI", "VN", "VU", "WF", "WS", "YE", "YT", "RS", "ZA",
    "ZM", "ME", "ZW", "A1", "A2", "O1", "AX", "GG", "IM", "JE",
    "BL", "MF"
};

const char GeoIP_country_code3[253][4] =
    { "--", "AP", "EU", "AND", "ARE", "AFG", "ATG", "AIA", "ALB", "ARM",
    "ANT",
    "AGO", "AQ", "ARG", "ASM", "AUT", "AUS", "ABW", "AZE", "BIH", "BRB",
    "BGD", "BEL", "BFA", "BGR", "BHR", "BDI", "BEN", "BMU", "BRN", "BOL",
    "BRA", "BHS", "BTN", "BV", "BWA", "BLR", "BLZ", "CAN", "CC", "COD",
    "CAF", "COG", "CHE", "CIV", "COK", "CHL", "CMR", "CHN", "COL", "CRI",
    "CUB", "CPV", "CX", "CYP", "CZE", "DEU", "DJI", "DNK", "DMA", "DOM",
    "DZA", "ECU", "EST", "EGY", "ESH", "ERI", "ESP", "ETH", "FIN", "FJI",
    "FLK", "FSM", "FRO", "FRA", "FX", "GAB", "GBR", "GRD", "GEO", "GUF",
    "GHA", "GIB", "GRL", "GMB", "GIN", "GLP", "GNQ", "GRC", "GS", "GTM",
    "GUM", "GNB", "GUY", "HKG", "HM", "HND", "HRV", "HTI", "HUN", "IDN",
    "IRL", "ISR", "IND", "IO", "IRQ", "IRN", "ISL", "ITA", "JAM", "JOR",
    "JPN", "KEN", "KGZ", "KHM", "KIR", "COM", "KNA", "PRK", "KOR", "KWT",
    "CYM", "KAZ", "LAO", "LBN", "LCA", "LIE", "LKA", "LBR", "LSO", "LTU",
    "LUX", "LVA", "LBY", "MAR", "MCO", "MDA", "MDG", "MHL", "MKD", "MLI",
    "MMR", "MNG", "MAC", "MNP", "MTQ", "MRT", "MSR", "MLT", "MUS", "MDV",
    "MWI", "MEX", "MYS", "MOZ", "NAM", "NCL", "NER", "NFK", "NGA", "NIC",
    "NLD", "NOR", "NPL", "NRU", "NIU", "NZL", "OMN", "PAN", "PER", "PYF",
    "PNG", "PHL", "PAK", "POL", "SPM", "PCN", "PRI", "PSE", "PRT", "PLW",
    "PRY", "QAT", "REU", "ROU", "RUS", "RWA", "SAU", "SLB", "SYC", "SDN",
    "SWE", "SGP", "SHN", "SVN", "SJM", "SVK", "SLE", "SMR", "SEN", "SOM",
    "SUR", "STP", "SLV", "SYR", "SWZ", "TCA", "TCD", "TF", "TGO", "THA",
    "TJK", "TKL", "TKM", "TUN", "TON", "TLS", "TUR", "TTO", "TUV", "TWN",
    "TZA", "UKR", "UGA", "UM", "USA", "URY", "UZB", "VAT", "VCT", "VEN",
    "VGB", "VIR", "VNM", "VUT", "WLF", "WSM", "YEM", "YT", "SRB", "ZAF",
    "ZMB", "MNE", "ZWE", "A1", "A2", "O1", "ALA", "GGY", "IMN", "JEY",
    "BLM", "MAF"
};

const char *GeoIP_country_name[253] =
    { "N/A", "Asia/Pacific Region", "Europe", "Andorra",
    "United Arab Emirates", "Afghanistan", "Antigua and Barbuda",
        "Anguilla", "Albania", "Armenia",
    "Netherlands Antilles",
    "Angola", "Antarctica", "Argentina", "American Samoa", "Austria",
    "Australia", "Aruba", "Azerbaijan", "Bosnia and Herzegovina",
    "Barbados",
    "Bangladesh", "Belgium", "Burkina Faso", "Bulgaria", "Bahrain",
    "Burundi", "Benin", "Bermuda", "Brunei Darussalam", "Bolivia",
    "Brazil", "Bahamas", "Bhutan", "Bouvet Island", "Botswana", "Belarus",
    "Belize", "Canada", "Cocos (Keeling) Islands",
    "Congo, The Democratic Republic of the",
    "Central African Republic", "Congo", "Switzerland", "Cote D'Ivoire",
    "Cook Islands", "Chile", "Cameroon", "China", "Colombia",
    "Costa Rica",
    "Cuba", "Cape Verde", "Christmas Island", "Cyprus", "Czech Republic",
    "Germany", "Djibouti", "Denmark", "Dominica", "Dominican Republic",
    "Algeria", "Ecuador", "Estonia", "Egypt", "Western Sahara", "Eritrea",
    "Spain", "Ethiopia", "Finland", "Fiji",
    "Falkland Islands (Malvinas)", "Micronesia, Federated States of",
    "Faroe Islands", "France", "France, Metropolitan", "Gabon",
    "United Kingdom", "Grenada", "Georgia", "French Guiana",
    "Ghana", "Gibraltar", "Greenland", "Gambia", "Guinea", "Guadeloupe",
    "Equatorial Guinea", "Greece",
    "South Georgia and the South Sandwich Islands", "Guatemala",
    "Guam", "Guinea-Bissau", "Guyana", "Hong Kong",
    "Heard Island and McDonald Islands", "Honduras", "Croatia",
    "Haiti", "Hungary", "Indonesia",
    "Ireland", "Israel", "India", "British Indian Ocean Territory", "Iraq",
    "Iran, Islamic Republic of", "Iceland", "Italy", "Jamaica",
    "Jordan",
    "Japan", "Kenya", "Kyrgyzstan", "Cambodia", "Kiribati", "Comoros",
    "Saint Kitts and Nevis", "Korea, Democratic People's Republic of",
    "Korea, Republic of", "Kuwait",
    "Cayman Islands", "Kazakhstan", "Lao People's Democratic Republic",
    "Lebanon", "Saint Lucia", "Liechtenstein", "Sri Lanka", "Liberia",
    "Lesotho", "Lithuania",
    "Luxembourg", "Latvia", "Libyan Arab Jamahiriya", "Morocco", "Monaco",
    "Moldova, Republic of", "Madagascar", "Marshall Islands",
    "Macedonia", "Mali",
    "Myanmar", "Mongolia", "Macau", "Northern Mariana Islands",
    "Martinique", "Mauritania", "Montserrat", "Malta", "Mauritius",
    "Maldives",
    "Malawi", "Mexico", "Malaysia", "Mozambique", "Namibia",
    "New Caledonia", "Niger", "Norfolk Island", "Nigeria", "Nicaragua",
    "Netherlands", "Norway", "Nepal", "Nauru", "Niue", "New Zealand",
    "Oman", "Panama", "Peru", "French Polynesia",
    "Papua New Guinea", "Philippines", "Pakistan", "Poland",
    "Saint Pierre and Miquelon", "Pitcairn Islands", "Puerto Rico",
    "Palestinian Territory", "Portugal", "Palau",
    "Paraguay", "Qatar", "Reunion", "Romania", "Russian Federation",
    "Rwanda", "Saudi Arabia", "Solomon Islands", "Seychelles", "Sudan",
    "Sweden", "Singapore", "Saint Helena", "Slovenia",
    "Svalbard and Jan Mayen", "Slovakia", "Sierra Leone", "San Marino",
    "Senegal", "Somalia", "Suriname",
    "Sao Tome and Principe", "El Salvador", "Syrian Arab Republic",
    "Swaziland", "Turks and Caicos Islands", "Chad",
    "French Southern Territories", "Togo", "Thailand",
    "Tajikistan", "Tokelau", "Turkmenistan", "Tunisia", "Tonga",
    "Timor-Leste", "Turkey", "Trinidad and Tobago", "Tuvalu", "Taiwan",
    "Tanzania, United Republic of", "Ukraine", "Uganda",
    "United States Minor Outlying Islands", "United States", "Uruguay",
    "Uzbekistan", "Holy See (Vatican City State)",
    "Saint Vincent and the Grenadines", "Venezuela",
    "Virgin Islands, British", "Virgin Islands, U.S.", "Vietnam",
    "Vanuatu", "Wallis and Futuna", "Samoa", "Yemen", "Mayotte",
    "Serbia", "South Africa",
    "Zambia", "Montenegro", "Zimbabwe", "Anonymous Proxy",
    "Satellite Provider", "Other", "Aland Islands", "Guernsey",
    "Isle of Man", "Jersey",
    "Saint Barthelemy", "Saint Martin"
};

/* Possible continent codes are AF, AS, EU, NA, OC, SA for Africa, Asia, Europe, North America, Oceania
and South America. */

const char GeoIP_country_continent[253][3] =
    { "--", "AS", "EU", "EU", "AS", "AS", "SA", "SA", "EU", "AS", "SA",
    "AF", "AN", "SA", "OC", "EU", "OC", "SA", "AS", "EU", "SA",
    "AS", "EU", "AF", "EU", "AS", "AF", "AF", "SA", "AS", "SA",
    "SA", "SA", "AS", "AF", "AF", "EU", "SA", "NA", "AS", "AF",
    "AF", "AF", "EU", "AF", "OC", "SA", "AF", "AS", "SA", "SA",
    "SA", "AF", "AS", "AS", "EU", "EU", "AF", "EU", "SA", "SA",
    "AF", "SA", "EU", "AF", "AF", "AF", "EU", "AF", "EU", "OC",
    "SA", "OC", "EU", "EU", "EU", "AF", "EU", "SA", "AS", "SA",
    "AF", "EU", "SA", "AF", "AF", "SA", "AF", "EU", "SA", "SA",
    "OC", "AF", "SA", "AS", "AF", "SA", "EU", "SA", "EU", "AS",
    "EU", "AS", "AS", "AS", "AS", "AS", "EU", "EU", "SA", "AS",
    "AS", "AF", "AS", "AS", "OC", "AF", "SA", "AS", "AS", "AS",
    "SA", "AS", "AS", "AS", "SA", "EU", "AS", "AF", "AF", "EU",
    "EU", "EU", "AF", "AF", "EU", "EU", "AF", "OC", "EU", "AF",
    "AS", "AS", "AS", "OC", "SA", "AF", "SA", "EU", "AF", "AS",
    "AF", "NA", "AS", "AF", "AF", "OC", "AF", "OC", "AF", "SA",
    "EU", "EU", "AS", "OC", "OC", "OC", "AS", "SA", "SA", "OC",
    "OC", "AS", "AS", "EU", "SA", "OC", "SA", "AS", "EU", "OC",
    "SA", "AS", "AF", "EU", "AS", "AF", "AS", "OC", "AF", "AF",
    "EU", "AS", "AF", "EU", "EU", "EU", "AF", "EU", "AF", "AF",
    "SA", "AF", "SA", "AS", "AF", "SA", "AF", "AF", "AF", "AS",
    "AS", "OC", "AS", "AF", "OC", "AS", "AS", "SA", "OC", "AS",
    "AF", "EU", "AF", "OC", "NA", "SA", "AS", "EU", "SA", "SA",
    "SA", "SA", "AS", "OC", "OC", "OC", "AS", "AF", "EU", "AF",
    "AF", "EU", "AF", "--", "--", "--", "EU", "EU", "EU", "EU",
    "SA", "SA"
};

/*************************************************************************/

void _setup_dbfilename(void)
{
    char path[BUFSIZE];

    if (NULL == GeoIPDBFileName) {
#ifdef _WIN32
#ifdef STATS_DIR
        ircsnprintf(path, BUFSIZE, "%s\\%s", STATS_DIR, "GeoIP.dat");
#else
        ircsnprintf(path, BUFSIZE, "%s", "GeoIP.dat");
#endif
#else
#ifdef STATS_DIR
        ircsnprintf(path, BUFSIZE, "%s/%s", STATS_DIR, "GeoIP.dat");
#else
        ircsnprintf(path, BUFSIZE, "%s", "GeoIP.dat");
#endif
#endif
        GeoIPDBFileName = sstrdup(path);
    }
}

/*************************************************************************/

int GeoIP_db_avail(int type)
{
    struct stat file_stat;

    if (type < 0 || type >= NUM_DB_TYPES || NULL == GeoIPDBFileName) {
        return 0;
    }
    return ((stat(GeoIPDBFileName, &file_stat) == 0) ? 1 : 0);;
}

/*************************************************************************/

void _setup_segments(GeoIP * localgi)
{
    int i;
    unsigned char delim[3];

    /* default to GeoIP Country Edition */
    localgi->databaseType = GEOIP_COUNTRY_EDITION;
    localgi->record_length = STANDARD_RECORD_LENGTH;
    fseek(localgi->GeoIPDatabase, -3l, SEEK_END);
    for (i = 0; i < STRUCTURE_INFO_MAX_SIZE; i++) {
        if (fread(delim, 1, 3, localgi->GeoIPDatabase)) {
            if (delim[0] == 255 && delim[1] == 255 && delim[2] == 255) {
                if (fread
                    (&localgi->databaseType, 1, 1,
                     localgi->GeoIPDatabase)) {
                    if (localgi->databaseType >= 106) {
                        /* backwards compatibility with databases from April 2003 and earlier */
                        localgi->databaseType -= 105;
                    }
                }
                break;
            } else {
                fseek(localgi->GeoIPDatabase, -4l, SEEK_CUR);
            }
        }
    }
    if (localgi->databaseType == GEOIP_COUNTRY_EDITION) {
        localgi->databaseSegments = malloc(sizeof(int));
        localgi->databaseSegments[0] = COUNTRY_BEGIN;
    }
}

/*************************************************************************/

int _check_mtime(GeoIP * localgi)
{
    struct stat buf;
    char *filename;
#ifdef _WIN32
    char buffer[_MAX_PATH];
    char win32filename[MAXPATHLEN];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
#endif
    if (localgi->flags & GEOIP_CHECK_CACHE) {
        if (fstat(fileno(localgi->GeoIPDatabase), &buf) != -1) {
            if (buf.st_mtime > localgi->mtime) {
                /* GeoIP Database file updated */
                if (localgi->flags & GEOIP_MEMORY_CACHE) {
                    /* reload database into memory cache */
                    if (realloc(localgi->cache, buf.st_size) != NULL) {
                        if (fread
                            (localgi->cache, sizeof(unsigned char),
                             buf.st_size,
                             localgi->GeoIPDatabase) !=
                            (size_t) buf.st_size) {
                            alog(LOG_ERROR, "Error reading file %s",
                                 localgi->file_path);
                            return -1;
                        }
                        localgi->mtime = buf.st_mtime;
                    }
                } else {
                    /* refresh filehandle */
                    fclose(localgi->GeoIPDatabase);
#ifndef _WIN32
                    filename = localgi->file_path;
#else
                    ircsnprintf(win32filename, sizeof(win32filename),
                                "%s\\%s", buffer, localgi->file_path);
                    filename = win32filename;
#endif
                    localgi->GeoIPDatabase = FileOpen(filename, FILE_READ);
                    if (localgi->GeoIPDatabase == NULL) {
                        alog(LOG_ERROR, "Error Opening file %s",
                             gi->file_path);
                        return -1;
                    }
                    _setup_segments(localgi);
                }
            }
        }
    }
    return 0;
}

/*************************************************************************/

unsigned int _seek_record(GeoIP * localgi, unsigned long ipnum)
{
    int depth;
    unsigned int x;
    unsigned char stack_buffer[2 * MAX_RECORD_LENGTH];
    const unsigned char *buf =
        (localgi->cache == NULL) ? stack_buffer : NULL;
    unsigned int offset = 0;
    const unsigned char *p;
    int j;

    SET_SEGV_LOCATION();

    _check_mtime(localgi);
    for (depth = 31; depth >= 0; depth--) {
        if (localgi->cache == NULL && localgi->index_cache == NULL) {
            /* read from disk */
            fseek(localgi->GeoIPDatabase,
                  (long) localgi->record_length * 2 * offset, SEEK_SET);
            if (!fread
                (stack_buffer, localgi->record_length, 2,
                 localgi->GeoIPDatabase)) {
                return 0;
            }
        } else if (gi->index_cache == NULL) {
            /* simply point to record in memory */
            buf =
                localgi->cache +
                (long) localgi->record_length * 2 * offset;
        } else {
            buf =
                localgi->index_cache +
                (long) localgi->record_length * 2 * offset;
        }
        SET_SEGV_LOCATION();

        if (ipnum & (1 << depth)) {
            /* Take the right-hand branch */
            if (localgi->record_length == 3) {
                /* Most common case is completely unrolled and uses constants. */
                x = (buf[3 * 1 + 0] << (0 * 8))
                    + (buf[3 * 1 + 1] << (1 * 8))
                    + (buf[3 * 1 + 2] << (2 * 8));

            } else {
                /* General case */
                j = localgi->record_length;
                p = &buf[2 * j];
                x = 0;
                do {
                    x <<= 8;
                    x += *(--p);
                } while (--j);
            }
            SET_SEGV_LOCATION();

        } else {
            /* Take the left-hand branch */
            if (localgi->record_length == 3) {
                /* Most common case is completely unrolled and uses constants. */
                x = (buf[3 * 0 + 0] << (0 * 8))
                    + (buf[3 * 0 + 1] << (1 * 8))
                    + (buf[3 * 0 + 2] << (2 * 8));
            } else {
                /* General case */
                j = localgi->record_length;
                p = &buf[1 * j];
                x = 0;
                do {
                    x <<= 8;
                    x += *(--p);
                } while (--j);
            }
        }
        SET_SEGV_LOCATION();

        if (x >= localgi->databaseSegments[0]) {
            return x;
        }
        offset = x;
    }
    SET_SEGV_LOCATION();

    /* shouldn't reach here */
    alog(LOG_ERROR,
         "Error Traversing Database for ipnum = %lu - Perhaps database is corrupt?",
         ipnum);
    return 0;
}

/*************************************************************************/

unsigned long _addr_to_num(const char *addr)
{
    int i;
    char tok[4];
    int octet;
    int j = 0, k = 0;
    unsigned long ipnum = 0;
    char c = 0;

    for (i = 0; i < 4; i++) {
        for (;;) {
            c = addr[k++];
            if (c == '.' || c == '\0') {
                tok[j] = '\0';
                octet = atoi(tok);
                if (octet > 255)
                    return 0;
                ipnum += (octet << ((3 - i) * 8));
                j = 0;
                break;
            } else if (c >= '0' && c <= '9') {
                if (j > 2) {
                    return 0;
                }
                tok[j++] = c;
            } else {
                return 0;
            }
        }
        if (c == '\0' && i < 3) {
            return 0;
        }
    }
    return ipnum;
}

/*************************************************************************/

GeoIP *GeoIP_open_type(int type, int flags)
{
    GeoIP *localgi;
    if (type < 0 || type >= NUM_DB_TYPES) {
        alog(LOG_ERROR, "Invalid database type %d\n", type);
        return NULL;
    }
    _setup_dbfilename();
    localgi = GeoIP_open(GeoIPDBFileName, flags);
    return localgi;
}

/*************************************************************************/

GeoIP *GeoIP_new(int flags)
{
    GeoIP *localgi;
    _setup_dbfilename();
    localgi = GeoIP_open(GeoIPDBFileName, flags);
    return localgi;
}

/*************************************************************************/

GeoIP *GeoIP_open(char *filename, int flags)
{
    struct stat buf;
    GeoIP *localgi = (GeoIP *) malloc(sizeof(GeoIP));
#ifdef _WIN32
    char buffer[_MAX_PATH];
    char win32filename[MAXPATHLEN];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
#endif
    if (localgi == NULL)
        return NULL;
    localgi->file_path = malloc(sizeof(char) * (strlen(filename) + 1));
    if (localgi->file_path == NULL)
        return NULL;
    strlcpy(localgi->file_path, filename, sizeof(localgi->file_path));
#ifdef _WIN32
    ircsnprintf(win32filename, sizeof(win32filename), "%s\\%s", buffer,
                filename);
    filename = sstrdup(win32filename);
#endif

    localgi->GeoIPDatabase = FileOpen(filename, FILE_READ);
    if (localgi->GeoIPDatabase == NULL) {
        alog(LOG_ERROR, "Error Opening file %s", filename);
        free(localgi->file_path);
        free(localgi);
        return NULL;
    } else {
        if (flags & GEOIP_MEMORY_CACHE) {
            if (fstat(fileno(localgi->GeoIPDatabase), &buf) == -1) {
                alog(LOG_ERROR, "Error stating file %s", filename);
                free(localgi);
                return NULL;
            }
            localgi->mtime = buf.st_mtime;
            localgi->cache =
                (unsigned char *) malloc(sizeof(unsigned char) *
                                         buf.st_size);
            if (localgi->cache != NULL) {
                if (fread
                    (localgi->cache, sizeof(unsigned char), buf.st_size,
                     localgi->GeoIPDatabase) != (size_t) buf.st_size) {
                    alog(LOG_ERROR, "Error reading file %s", filename);
                    free(localgi->cache);
                    free(localgi);
                    return NULL;
                }
            }
        } else {
            localgi->cache = NULL;
        }
        localgi->flags = flags;
        _setup_segments(localgi);
        if (flags & GEOIP_INDEX_CACHE) {
            localgi->index_cache =
                (unsigned char *) malloc(sizeof(unsigned char) *
                                         localgi->databaseSegments[0]);
            if (localgi->index_cache != NULL) {
                fseek(localgi->GeoIPDatabase, 0, SEEK_SET);
                if (fread
                    (localgi->index_cache, sizeof(unsigned char),
                     localgi->databaseSegments[0],
                     localgi->GeoIPDatabase) !=
                    (size_t) localgi->databaseSegments[0]) {
                    alog(LOG_ERROR, "Error reading file %s", filename);
                    free(localgi->index_cache);
                    free(localgi);
                    return NULL;
                }
            }
        } else {
            localgi->index_cache = NULL;
        }
        return localgi;
    }
}

/*************************************************************************/

void GeoIP_delete(GeoIP * localgi)
{
    if (localgi->GeoIPDatabase != NULL)
        fclose(localgi->GeoIPDatabase);
    if (localgi->cache != NULL)
        free(localgi->cache);
    if (localgi->index_cache != NULL)
        free(localgi->index_cache);
    if (localgi->file_path != NULL)
        free(localgi->file_path);
    if (localgi->databaseSegments != NULL)
        free(localgi->databaseSegments);
    free(localgi);
}

/*************************************************************************/

const char *GeoIP_country_code_by_name(GeoIP * localgi, const char *name)
{
    int country_id;
    country_id = GeoIP_id_by_name(localgi, name);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_code[country_id] : NULL;
}

/*************************************************************************/

const char *GeoIP_country_code3_by_name(GeoIP * localgi, const char *name)
{
    int country_id;
    country_id = GeoIP_id_by_name(localgi, name);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_code3[country_id] : NULL;
}

/*************************************************************************/

const char *GeoIP_country_name_by_name(GeoIP * localgi, const char *name)
{
    int country_id;
    country_id = GeoIP_id_by_name(localgi, name);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_name[country_id] : NULL;
}

/*************************************************************************/

unsigned long lookupaddress(const char *host)
{
#if HAVE_GETHOSTBYNAME
    unsigned long addr = inet_addr(host);
    struct hostent phe2;
    struct hostent *phe = &phe2;
    int result = 0;
    if (addr == INADDR_NONE) {
        alog(LOG_DEBUG, "debug: Looking up hostname %s", host);
        phe = gethostbyname(host);
        if (!phe || result != 0) {
            return 0;
        }
        addr = *((unsigned long *) phe->h_addr_list[0]);
    }
    return ntohl(addr);
#else
    return 0;
#endif
}

/*************************************************************************/

int GeoIP_id_by_name(GeoIP * localgi, const char *name)
{
    unsigned long ipnum;
    int ret;

    if (!localgi || name == NULL) {
        alog(LOG_DEBUG, "Invalid pointer to localgi or addr variables");
        return 0;
    }

    if (localgi->databaseType != GEOIP_COUNTRY_EDITION) {
        alog(LOG_ERROR, "Database is not the GeoIP Country Edition");
        return 0;
    }

    if (!(ipnum = lookupaddress(name)))
        return 0;
    ret = _seek_record(localgi, ipnum) - COUNTRY_BEGIN;
    return ret;

}

/*************************************************************************/

const char *GeoIP_country_code_by_addr(GeoIP * localgi, const char *addr)
{
    int country_id;
    country_id = GeoIP_id_by_addr(localgi, addr);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_code[country_id] : NULL;
}

/*************************************************************************/

const char *GeoIP_country_code3_by_addr(GeoIP * localgi, const char *addr)
{
    int country_id;
    country_id = GeoIP_id_by_addr(localgi, addr);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_code3[country_id] : NULL;
    /*return GeoIP_country_code3[country_id]; */
}

/*************************************************************************/

const char *GeoIP_country_name_by_addr(GeoIP * localgi, const char *addr)
{
    int country_id;
    country_id = GeoIP_id_by_addr(localgi, addr);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_name[country_id] : NULL;
    /*return GeoIP_country_name[country_id]; */
}

/*************************************************************************/

const char *GeoIP_country_code_by_ipnum(GeoIP * localgi,
                                        unsigned long ipnum)
{
    int country_id;
    country_id = GeoIP_id_by_ipnum(localgi, ipnum);
    return (country_id > 0
            && country_id <= 253) ? GeoIP_country_code[country_id] : NULL;
}

/*************************************************************************/

int GeoIP_id_by_addr(GeoIP * localgi, const char *addr)
{
    unsigned long ipnum;
    int ret;

    if (!localgi || addr == NULL) {
        alog(LOG_DEBUG, "Invalid pointer to localgi or addr variables");
        return 0;
    }

    if (localgi->databaseType != GEOIP_COUNTRY_EDITION) {
        alog(LOG_ERROR, "Database is Invalid for GeoIP Country Edition");
        return 0;
    }
    ipnum = _addr_to_num(addr);
    ret = _seek_record(localgi, ipnum) - COUNTRY_BEGIN;
    return ret;
}

/*************************************************************************/

int GeoIP_id_by_ipnum(GeoIP * localgi, unsigned long ipnum)
{
    int ret;
    if (ipnum == 0) {
        return 0;
    }
    if (localgi->databaseType != GEOIP_COUNTRY_EDITION) {
        alog(LOG_ERROR,
             "Database is Invalid for the GeoIP Country Edition");
        return 0;
    }
    ret = _seek_record(localgi, ipnum) - COUNTRY_BEGIN;
    return ret;
}

/*************************************************************************/

char *GeoIP_database_info(GeoIP * localgi)
{
    int i;
    unsigned char buf[3];
    char *retval;
    int hasStructureInfo = 0;

    if (gi == NULL)
        return NULL;

    _check_mtime(gi);
    fseek(localgi->GeoIPDatabase, -3l, SEEK_END);

    /* first get past the database structure information */
    for (i = 0; i < STRUCTURE_INFO_MAX_SIZE; i++) {
        if (fread(buf, 1, 3, gi->GeoIPDatabase)) {
            if (buf[0] == 255 && buf[1] == 255 && buf[2] == 255) {
                hasStructureInfo = 1;
                break;
            }
        }
        fseek(localgi->GeoIPDatabase, -4l, SEEK_CUR);
    }
    if (hasStructureInfo == 1) {
        fseek(localgi->GeoIPDatabase, -3l, SEEK_CUR);
    } else {
        /* no structure info, must be pre Sep 2002 database, go back to end */
        fseek(localgi->GeoIPDatabase, -3l, SEEK_END);
    }

    for (i = 0; i < DATABASE_INFO_MAX_SIZE; i++) {
        if (fread(buf, 1, 3, localgi->GeoIPDatabase)) {
            if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0) {
                retval = malloc(sizeof(char) * (i + 1));
                if (retval == NULL) {
                    return NULL;
                }
                if (fread(retval, 1, i, localgi->GeoIPDatabase)) {
                    retval[i] = '\0';
                }
                return retval;
            }
        }
        fseek(localgi->GeoIPDatabase, -4l, SEEK_CUR);
    }
    return NULL;
}

/*************************************************************************/

GeoIPRecord *_extract_record(GeoIP * localgi, unsigned int seek_record,
                             int *next_record_ptr)
{
    int record_pointer;
    unsigned char *record_buf = NULL;
    unsigned char *begin_record_buf = NULL;
    GeoIPRecord *record;
    int str_length = 0;
    int j;
    double latitude = 0, longitude = 0;
    int bytes_read = 0;
    if (seek_record == localgi->databaseSegments[0])
        return NULL;

    record = malloc(sizeof(GeoIPRecord));
    memset(record, 0, sizeof(GeoIPRecord));

    record_pointer =
        seek_record + (2 * localgi->record_length -
                       1) * localgi->databaseSegments[0];

    if (localgi->cache == NULL) {
        fseek(localgi->GeoIPDatabase, record_pointer, SEEK_SET);
        begin_record_buf = record_buf =
            malloc(sizeof(char) * FULL_RECORD_LENGTH);
        bytes_read =
            fread(record_buf, sizeof(char), FULL_RECORD_LENGTH,
                  localgi->GeoIPDatabase);
        if (bytes_read == 0) {
            /* eof or other error */
            return NULL;
        }
    } else {
        record_buf = localgi->cache + (long) record_pointer;
    }

    /* get country */
    record->country_code = sstrdup(GeoIP_country_code[record_buf[0]]);
    record->country_code3 = sstrdup(GeoIP_country_code3[record_buf[0]]);
    record->country_name = sstrdup(GeoIP_country_name[record_buf[0]]);
    record_buf++;

    /* get region */
    while (record_buf[str_length] != '\0')
        str_length++;
    if (str_length > 0) {
        record->region = malloc(str_length + 1);
        strncpy(record->region, (const char *) record_buf, str_length + 1);
    }
    record_buf += str_length + 1;
    str_length = 0;

    /* get city */
    while (record_buf[str_length] != '\0')
        str_length++;
    if (str_length > 0) {
        record->city = malloc(str_length + 1);
        strncpy(record->city, (const char *) record_buf, str_length + 1);
    }
    record_buf += (str_length + 1);
    str_length = 0;

    /* get postal code */
    while (record_buf[str_length] != '\0')
        str_length++;
    if (str_length > 0) {
        record->postal_code = malloc(str_length + 1);
        strncpy(record->postal_code, (const char *) record_buf,
                str_length + 1);
    }
    record_buf += (str_length + 1);

    /* get latitude */
    for (j = 0; j < 3; ++j)
        latitude += (record_buf[j] << (j * 8));
    record->latitude = latitude / 10000 - 180;
    record_buf += 3;

    /* get longitude */
    for (j = 0; j < 3; ++j)
        longitude += (record_buf[j] << (j * 8));
    record->longitude = longitude / 10000 - 180;

    if (localgi->cache == NULL)
        free(begin_record_buf);

    /* Used for GeoIP_next_record */
    if (next_record_ptr != NULL)
        *next_record_ptr = seek_record + record_buf - begin_record_buf + 3;

    return record;
}

/*************************************************************************/

int GeoIP_next_record(GeoIP * localgi, GeoIPRecord ** gir,
                      int *record_iter)
{
    if (localgi->cache != NULL) {
        alog(LOG_ERROR,
             "GeoIP_next_record not supported in memory cache mode");
        return 1;
    }
    *gir = _extract_record(localgi, *record_iter, record_iter);
    return 0;
}

/*************************************************************************/
