# Make file
#
# (c) 2004-2013 Denora Team
# Contact us at info@denorastats.org
#
# Please read COPYING and README for further details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# 
#

include ./Makefile.inc

MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' 'MODULE_PATH=${MODULE_PATH}'\
	   'SHARED=${SHARED}' 'MODULEFLAGS=${MODULEFLAGS}' 'RDYNAMNIC=${RDYNAMNIC}'

OBJECTS= $(SRCS:.c=.o)
SO_FILES=$(OBJECTS:.o=.s)

all: modules subs

modules: $(OBJECTS) $(SO_FILES)

install: 
	$(CP) ./*.so $(DATDEST)/core

distclean: spotless

.c.o:
	$(CC) ${CFLAGS} ${RDYNAMNIC} ${MODULEFLAGS} -I../${INCLUDEDIR} -c $<

.o.s:
	$(CC) ${SHARED} $(DENORALIBS) ../mod_version.o $< -o $*.so ${PROFILE}

subs:
	@for i in $(SUBS); do \
        echo "make all in $$i..."; \
        (cd $$i; $(MAKE) $(MAKEARGS) all); done

subs_clean:
	@for i in $(SUBS); do \
	echo "cleaning in $$i..."; \
	(cd $$i; $(MAKE) clean); done

clean: subs_clean
	rm -f *.o *.so *.c~ core

spotless:  subs_clean
	rm -f *.o *.so *.c~ core Makefile.inc

