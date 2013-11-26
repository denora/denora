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

OBJS =  denoralib.o FileIO.o Denora_Mem.o Denora_SQL.o sqlite3.o xmlparse.o
        
SRCS =  denoralib.c FileIO.c Denora_Mem.c Denora_SQL.c sqlite3.c xmlparse.c
        

INCLUDES = denoralib.h denoralib_err.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' \
	   'SHARED=${SHARED}' 'RDYNAMNIC=${RDYNAMNIC}'


.c.o:
	$(CC) $(CFLAGS) -I../include/ -c $<

all: stats

distclean: spotless

stats:  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(DENORALIBS) $(MLIBS) -o $@ $(LDFLAGS) 

$(OBJS): Makefile
admin.o:        admin.c     $(INCLUDES)

clean:
	rm -f *.o stats a.out

spotless:
	rm -f *.o stats a.out

install: stats

DUMMY:
