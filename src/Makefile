# Makefile for Denora.
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

OBJS =  admin.o base64.o bans.o channels.o chanstats.o ChanBanMode.o ChanMode.o core.o cron.o ctcp.o config.o db.o datafiles.o Denora_Mem.o Denora_SQL.o \
        denoralib.o exclude.o events.o extsock.o FileIO.o GeoIP.o GeoIPCity.o language.o list.o log.o ircd.o main.o markup.o modules.o moddata.o moduleapi.o \
        messages.o md5.o mod_version.o Modes.o mysql.o network.o numerics.o privmsg.o rdb.o send.o servers.o signals.o sockutil.o statserv.o \
        regionName.o shell.o spamfilter.o sqlqueue.o sqlite3.o strings.o timeZone.o tld.o users.o xmlrpc.o xmlparse.o xmlconfig.o zip.o \
        
SRCS =  admin.c base64.c bans.c channels.c chanstats.c ChanBanMode.c ChanMode.c core.c cron.c ctcp.c config.c db.c datafiles.c Denora_Mem.c Denora_SQL.c \
        denoralib.c exclude.c events.c extsock.c FileIO.c GeoIP.c GeoIPCity.c language.c list.c log.c ircd.c main.c markup.c  modules.c moddata.c moduleapi.c \
        messages.c md5.c mod_version.c Modes.c mysql.c network.c numerics.c privmsg.c rdb.c send.c servers.c signals.c sockutil.c statserv.c \
        regionName.c shell.c spamfilter.c sqlqueue.c sqlite3.c strings.c timeZone.c tld.c users.c xmlrpc.o xmlparse.c xmlconfig.c zip.c \
        

INCLUDES = ../include/language.h ../include/sysconf.h \
		   ../include/denora.h ../include/extern.h ../include/modules.h


MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
      	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
  	   'RUNGROUP=${RUNGROUP}' 'MODULE_PATH=${MODULE_PATH}' \
	   'MYSQL=${MYSQL}' 'SHARED=${SHARED}' 'MODULEFLAGS=${MODULEFLAGS}' 'RDYNAMNIC=${RDYNAMNIC}'


.c.o:
	$(CC) $(CFLAGS) -I../include/ -c $<

all: stats

distclean: spotless

stats:  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(DENORALIBS) -o $@ $(LDFLAGS) 

$(OBJS): Makefile
admin.o:        admin.c     $(INCLUDES)
base64.o:       base64.c    $(INCLUDES)
bans.o:         bans.c      $(INCLUDES)
channels.o:     channels.c  $(INCLUDES) 
chanstats.o:    chanstats.c $(INCLUDES)
ChanBanMode.o:  ChanBanMode.c $(INCLUDES)
ChanMode.o:     ChanMode.c    $(INCLUDES)
config.o:       config.c      $(INCLUDES) 
core.o:         core.c        $(INCLUDES)
cron.o:         cron.c       $(INCLUDES)
ctcp.o:         ctcp.c       $(INCLUDES)
datafiles.o:    datafiles.c  $(INCLUDES)
Denora_Mem.o:   Denora_Mem.c $(INCLUDES)
Denora_SQL.o:   Denora_SQL.c $(INCLUDES)
denoralib.o:    denoralib.c  $(INCLUDES)
db.o:           db.c        $(INCLUDES)
exclude.o:      exclude.c   $(INCLUDES)
events.o:       events.c    $(INCLUDES)
FileIO.o:       FileIO.c    $(INCLUDES)
GeoIP.o:        GeoIP.c     $(INCLUDES)
GeoIPCity.o:    GeoIPCity.c $(INCLUDES)
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
md5.o:          md5.c       $(INCLUDES)
mod_version.o:  mod_version.c  $(INCLUDES)
Modes.o:        Modes.c     $(INCLUDES)
network.o:      network.c   $(INCLUDES)
numerics.o:     numerics.c  $(INCLUDES)
privmsg.o:      privmsg.c   $(INCLUDES)
send.o:         send.c      $(INCLUDES)
servers.o:      servers.c   $(INCLUDES)
shell.o:        shell.c   $(INCLUDES)
spamfilter.o:   spamfilter.c $(INCLUDES)
signals.o:      signals.c   $(INCLUDES)
sockutil.o:     sockutil.c  $(INCLUDES)
sqlqueue.o:     sqlqueue.c  $(INCLUDES)
sqlite3.o:      sqlite3.c  $(INCLUDES)
strings.o:      strings.c   $(INCLUDES)
statserv.o:     statserv.c  $(INCLUDES)
tld.o:          tld.c       $(INCLUDES)
users.o:        users.c     $(INCLUDES)
xmlrpc.o:       xmlrpc.c    $(INCLUDES)
xmlparse.o:     xmlparse.c  $(INCLUDES)
xmlconfig.o:    xmlconfig.c $(INCLUDES)
zip.o:          zip.c       $(INCLUDES)
mysql.o:        mysql.c     $(INCLUDES)
rdb.o:          rdb.c       $(INCLUDES)
timeZone.o:     timeZone.c  $(INCLUDES)
regionName.o:   regionName.c       $(INCLUDES)


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
	$(INSTALL) bin/geoipupd $(BINDEST)/geoipupd
	(cd ../lang ; $(MAKE) install)
	$(CP) ../data/* $(DATDEST)
	$(INSTALL) bin/mydbgen $(DATDEST)/mydbgen
	test -d $(DATDEST)/backups || mkdir $(DATDEST)/backups
	test -d $(DATDEST)/logs || mkdir $(DATDEST)/logs
	test -d $(DATDEST)/sql || mkdir $(DATDEST)/sql
	test -d $(DATDEST)/ircdconfig || mkdir $(DATDEST)/ircdconfig
	$(CP) ../sql/* $(DATDEST)/sql
	$(CP) ../data/ircdconfig/* $(DATDEST)/sql/ircdconfig
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
