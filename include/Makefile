# Make file
#
# (C) 2004-2006 Denora Team
# Contact us at info@nomadirc.net
#
# Please read COPYING and README for further details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id: Makefile 581 2006-03-21 09:29:03Z trystan $
#

all: denora.h extern.h version.h

version.h: Makefile version.sh denora.h $(SRCS)
	sh version.sh

denora.h: sysconf.h
	touch $@

clean: 
	rm -f language.h

distclean:
	rm -f sysconf.h version.h language.h
