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

OBJS =  base64.o DateTime.o Denora_Mem.o Denora_SQL.o denoralib.o FileIO.o markup.o match.o md5.o String.o sqlite3.o xmlparse.o zip.o
        
SRCS =  base64.c DateTime.c Denora_Mem.c Denora_SQL.c denoralib.c FileIO.c markup.c match.c md5.c String.c sqlite3.c xmlparse.c zip.c
        

INCLUDES = denoralib.h denoralib_err.h sqlite.h sqlite3ext.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' \
	   'SHARED=${SHARED}' 'RDYNAMNIC=${RDYNAMNIC}'


.c.o:
	$(CC) $(CFLAGS) -I../include/ -c $<

all: denoralib

distclean: spotless

denoralib:  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(DENORALIBS) $(MLIBS) -o $@ $(LDFLAGS) 

$(OBJS): Makefile
base64.o:        base64.c     $(INCLUDES)
DateTime.o:      DateTime.c   $(INCLUDES)
Denora_Mem.o:       Denora_Mem.c     $(INCLUDES)
Denora_SQL.o:        Denora_SQL.c     $(INCLUDES)
denoralib.o:        denoralib.c     $(INCLUDES)
FileIO.o:        FileIO.c     $(INCLUDES)

markup.o:        markup.c     $(INCLUDES)
match.o:      match.c   $(INCLUDES)
md5.o:       md5.c     $(INCLUDES)
String.o:        String.c     $(INCLUDES)
sqlite3.o:        sqlite3.c     $(INCLUDES)
xmlparse.o:        xmlparse.c     $(INCLUDES)
zip.o:        zip.c     $(INCLUDES)


clean:
	rm -f *.o denoralib a.out

spotless:
	rm -f *.o denoralib a.out

install: denoralib

DUMMY:
