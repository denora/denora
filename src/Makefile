# Makefile for Denora.
#
# (C) 2004-2007 Denora Team
# Contact us at info@denorastats.org
#
# Please read COPYING and README for further details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id$
#

MYSQL_OBJ = $(MYSQL:.c=.o)
POSTGRE_OBJ = $(POSTGRE:.c=.o)

OBJS =  admin.o adns.o base64.o bans.o channels.o chanstats.o ChanBanMode.o ChanMode.o core.o cron.o ctcp.o config.o db.o datafiles.o \
        exclude.o events.o extsock.o FileIO.o GeoIP.o language.o list.o log.o ircd.o main.o markup.o modules.o moddata.o moduleapi.o \
        messages.o mod_version.o Modes.o network.o numerics.o privmsg.o rdb.o send.o servers.o signals.o sockutil.o statserv.o \
        spamfilter.o sqlqueue.o strings.o tld.o users.o xmlrpc.o zip.o \
        $(MYSQL_OBJ) $(POSTGRE_OBJ)
SRCS =  admin.c adns.c base64.c bans.c channels.c chanstats.c ChanBanMode.c ChanMode.c core.c cron.c ctcp.c config.c db.c datafiles.c \
        exclude.c events.c extsock.c FileIO.c GeoIP.c language.c list.c log.c ircd.c main.c markup.c  modules.c moddata.c moduleapi.c \
        messages.c mod_version.c Modes.c network.c numerics.c privmsg.c rdb.c send.c servers.c signals.c sockutil.c statserv.c \
        spamfilter.c sqlqueue.c strings.c tld.c users.c xmlrpc.o zip.c \
        $(MYSQL) $(POSTGRE)

INCLUDES = ../include/language.h ../include/sysconf.h \
		   ../include/denora.h ../include/extern.h ../include/modules.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' 'MODULE_PATH=${MODULE_PATH}' 'POSTGRE=${POSTGRE}'\
	   'MYSQL=${MYSQL}' 'SHARED=${SHARED}' 'MODULEFLAGS=${MODULEFLAGS}' 'RDYNAMNIC=${RDYNAMNIC}'


.c.o:
	$(CC) $(CFLAGS) -I../include/ -c $<

all: stats

distclean: spotless

stats:  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(DENORALIBS) $(MLIBS) -o $@ $(LDFLAGS) 

$(OBJS): Makefile
admin.o:        admin.c     $(INCLUDES)
adns.o:         adns.c      $(INCLUDES)
base64.o:       base64.c    $(INCLUDES)
bans.o:         bans.c      $(INCLUDES)
channels.o:     channels.c  $(INCLUDES) 
chanstats.o:    chanstats.c $(INCLUDES)
ChanBanMode.o:  ChanBanMode.c  $(INCLUDES)
ChanMode.o:     ChanMode.c  $(INCLUDES)
config.o:       config.c    $(INCLUDES) 
core.o:         core.c      $(INCLUDES)
cron.o:         cron.c      $(INCLUDES)
ctcp.o:         ctcp.c      $(INCLUDES)
datafiles.o:    datafiles.c $(INCLUDES)
db.o:           db.c        $(INCLUDES)
exclude.o:      exclude.c   $(INCLUDES)
events.o:       events.c    $(INCLUDES)
FileIO.o:       FileIO.c    $(INCLUDES)
GeoIP.o:        GeoIP.c     $(INCLUDES)
language.o:     language.c  $(INCLUDES)
list.o:         list.c      $(INCLUDES)
log.o:          log.c       $(INCLUDES)
ircd.o:		ircd.c	    $(INCLUDES)
main.o:         main.c      $(INCLUDES)
markup.o:       markup.c    $(INCLUDES)
modules.o:      modules.c   $(INCLUDES)
moduleapi.o:    moduleapi.c $(INCLUDES)
moddata.o:      moddata.c   $(INCLUDES)
messages.o:     messages.c  $(INCLUDES)
mod_version.o:  mod_version.c  $(INCLUDES)
Modes.o:        Modes.c     $(INCLUDES)
network.o:      network.c   $(INCLUDES)
numerics.o:     numerics.c  $(INCLUDES)
privmsg.o:      privmsg.c   $(INCLUDES)
send.o:         send.c      $(INCLUDES)
servers.o:      servers.c   $(INCLUDES)
spamfilter.o:   spamfilter.c $(INCLUDES)
signals.o:      signals.c   $(INCLUDES)
sockutil.o:     sockutil.c  $(INCLUDES)
sqlqueue.o:     sqlqueue.c  $(INCLUDES)
strings.o:      strings.c   $(INCLUDES)
statserv.o:     statserv.c  $(INCLUDES)
tld.o:          tld.c       $(INCLUDES)
users.o:        users.c     $(INCLUDES)
xmlrpc.o:       xmlrpc.c    $(INCLUDES)
zip.o:          zip.c       $(INCLUDES)
mysql.o:        mysql.c     $(INCLUDES)
postgre.o:      postgre.c   $(INCLUDES)
rdb.o:          rdb.c       $(INCLUDES)

modules: DUMMY
	(cd modules ; ./configure ; ${MAKE} ${MAKEARGS} all)

core: DUMMY
	(cd core ; ./configure ; ${MAKE} ${MAKEARGS} all)

clean:
	@touch modules/Makefile.inc # Horribly ugly...
	(cd modules ; ${MAKE} clean)
	@touch protocol/Makefile.inc
	(cd protocol ; ${MAKE} clean)
	@touch core/Makefile.inc
	(cd core ; ${MAKE} clean)
	rm -f *.o stats a.out

spotless:
	@touch modules/Makefile.inc # Horribly ugly...
	(cd modules ; ${MAKE} distclean)
	@touch protocol/Makefile.inc
	(cd protocol ; ${MAKE} distclean)
	@touch core/Makefile.inc
	(cd core ; ${MAKE} distclean)
	rm -f *.o stats a.out

install: stats
	test -d ${BINDEST} || mkdir ${BINDEST}
	$(INSTALL) stats $(BINDEST)/stats
	$(INSTALL) bin/denorarc $(BINDEST)/denorarc
	(cd ../lang ; $(MAKE) install)
	$(CP) ../data/* $(DATDEST)
	$(INSTALL) bin/mydbgen $(DATDEST)/mydbgen
	test -d $(DATDEST)/backups || mkdir $(DATDEST)/backups
	test -d $(DATDEST)/logs || mkdir $(DATDEST)/logs
	test -d $(DATDEST)/sql || mkdir $(DATDEST)/sql
	$(CP) ../sql/* $(DATDEST)/sql
	test -d $(DATDEST)/core || mkdir $(DATDEST)/core
	test -d $(DATDEST)/core/runtime || mkdir $(DATDEST)/core/runtime
	(cd core ; ${MAKE} install)
	@if [ "$(MODULE_PATH)" ] ; then \
		test -d ${MODULE_PATH} || mkdir ${MODULE_PATH} ; \
		test -d ${MODULE_PATH}/runtime || mkdir ${MODULE_PATH}/runtime ; \
		(cd modules ; $(MAKE) install) ; \
		(cd protocol ; $(MAKE) install) ; \
	fi
	@if [ "$(RUNGROUP)" ] ; then \
		echo chgrp -R $(RUNGROUP) $(DATDEST) ; \
		chgrp -R $(RUNGROUP) $(DATDEST) ; \
		echo chmod -R g+rw $(DATDEST) ; \
		chmod -R g+rw $(DATDEST) ; \
		echo find $(DATDEST) -type d -exec chmod g+xs \'\{\}\' \\\; ; \
		find $(DATDEST) -type d -exec chmod g+xs '{}' \; ; \
	fi

DUMMY:
