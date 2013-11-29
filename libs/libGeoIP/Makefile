# Makefile for Denora.
#
# (c) 2004-2013 Denora Team
# Contact us at info@denorastats.org
#
# Please read COPYING and README for further details.
#
#
# 
#

OBJS =  GeoIP.o GeoIP_deprecated.o GeoIPCity.o regionName.o timeZone.o
SRCS =  GeoIP.c GeoIP_deprecated.c GeoIPCity.c regionName.c timeZone.c

INCLUDES = GeoIP.h GeoIPCity.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' \
	   'SHARED=${SHARED}' 'RDYNAMNIC=${RDYNAMNIC}'


.c.o:
	$(CC) $(CFLAGS) -I../include/ -c $<

all: libGeoIP.la

distclean: spotless

libGeoIP.la:  $(OBJS)
	$(CC) $(CFLAGS) -DGEOIPDATADIR=\"${DATDEST}\" $(OBJS) $(DENORALIBS) $(MLIBS) -o $@ $(LDFLAGS) 

$(OBJS): Makefile
GeoIP.lo GeoIP.o:        GeoIP.c GeoIP_deprecated.c     $(INCLUDES)
GeoIPCity.lo GeoIPCity.o:      GeoIPCity.c   $(INCLUDES)
regionName.lo regionName.o:       regionName.c     $(INCLUDES)
timeZone.lo timeZone.o:        timeZone.c     $(INCLUDES)

clean:
	rm -f *.o libGeoIP.la a.out

spotless:
	rm -f *.o libGeoIP.la a.out

install: libGeoIP.la

DUMMY:


