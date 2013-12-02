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

OBJS =  base64.o DateTime.o Denora_Mem.o Denora_SQL.o denoralib.o FileIO.o markup.o match.o md5.o String.o sqlite3.o xmlparse.o \
		xmlconfig.o GeoIP.o GeoIPCity.o regionName.o timeZone.o GeoIP_deprecated.o

SRCS =  base64.c DateTime.c Denora_Mem.c Denora_SQL.c denoralib.c FileIO.c markup.c match.c md5.c String.c sqlite3.c xmlparse.c \
		xmlconfig.c GeoIP.c GeoIPCity.c regionName.c timeZone.c GeoIP_deprecated.c
        

INCLUDES = denoralib.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' 'MODULE_PATH=${MODULE_PATH}'\
	   'SHARED=${SHARED}' 'MODULEFLAGS=${MODULEFLAGS}' 'RDYNAMNIC=${RDYNAMNIC}'

.c.o:
	$(CC) ${CFLAGS} ${CDEFS} ${RDYNAMNIC} ${MODULEFLAGS} -I../${INCLUDEDIR} -c $<

.o.s:
	$(CC) ${SHARED} $< -o $*.s ${PROFILE}


all: libdenora.so

distclean: spotless

OBJECTS= $(SRCS:.c=.o)
SO_FILES=$(OBJECTS:.o=.s)

libdenora.so:  $(OBJECTS) $(SO_FILES)
	$(CC) $(CFLAGS) $(OBJECTS) $(DENORALIBS) -o $@ $(LDFLAGS) 
	ar rvs libdenora.a libdenora.so

clean:
	rm -f *.o  *.s libdenora.so a.out

spotless:
	rm -f *.o libdenora.so a.out

install: libdenora.so

DUMMY:
