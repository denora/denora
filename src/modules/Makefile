# Make file
#
# (c) 2004-2011 Denora Team
# Contact us at info@denorastats.org
#
# Please read COPYING and README for further details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id$
#

include ./Makefile.inc

MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'ANOPELIBS=${ANOPELIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
   	   'RUNGROUP=${RUNGROUP}' 'MODULE_PATH=${MODULE_PATH}' \
		'PROFILE=${PROFILE}' 'MODULEFLAGS=${MODULEFLAGS}' 'RDYNAMNIC=${RDYNAMNIC}'

OBJECTS= $(SRCS:.c=.o)
SO_FILES=$(OBJECTS:.o=.s)
CDEFS= 

all: modules  subs

modules: $(OBJECTS) $(SO_FILES)

install: 
	$(CP) ./*.so $(MODULE_PATH) 

distclean: spotless

.c.o:
	$(CC) ${CFLAGS} ${CDEFS} ${RDYNAMNIC} ${MODULEFLAGS} -I../${INCLUDEDIR} -c $<

.o.s:
	$(CC) -shared ../mod_version.o $< -o $*.so ${PROFILE}

subs:
	@for i in $(SUBS); do \
        echo "make all in $$i..."; \
        (cd $$i; $(MAKE) $(MAKEARGS) all); done

subs_clean:
	@for i in $(SUBS); do \
	echo "cleaning in $$i..."; \
	(cd $$i; $(MAKE) clean); done

clean:
	rm -f *.o *.so *.c~ core

spotless:
	rm -f *.o *.so *.c~ core *.so Makefile.inc

