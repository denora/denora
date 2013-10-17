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

all: denora.h extern.h version.h

version.h: Makefile version.sh denora.h $(SRCS)
	sh version.sh

denora.h: sysconf.h
	touch $@

clean: 
	rm -f language.h

distclean:
	rm -f sysconf.h version.h language.h
