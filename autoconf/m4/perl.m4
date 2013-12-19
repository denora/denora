AC_DEFUN([AUTOTOOLS_EMBED_PERL], [
                AC_MSG_NOTICE([Configuring for Perl Modules Now])
                AC_PATH_PROG(sedpath, sed)
                if test "_$sedpath" = _; then
                        AC_MSG_WARN("Cannot find sed: I need it\!")
                fi
                AC_PATH_PROG(PERL, perl)
                AC_MSG_CHECKING(for Perl compile flags)
                PERL_CFLAGS=`$PERL -MExtUtils::Embed -e ccopts 2>/dev/null`
                if test "_$PERL_CFLAGS" = _ ; then
                        AC_MSG_RESULT([not found, building without perl.])
                        perl=no
				else
                        AC_MSG_RESULT([yes])
                        AC_MSG_CHECKING([Checking for perl includes directory])
                        PERL_INCLUDES=`$PERL -MExtUtils::Embed -e perl_inc 2>/dev/null`
                        AC_MSG_RESULT($PERL_INCLUDES)
                        CFLAGS="$CFLAGS "$PERL_INCLUDES
                        PERL_LDFLAGS=`$PERL -MExtUtils::Embed -e ldopts |$sedpath 's/-lgdbm //'`
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-ldb //'`
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lndbm //'`
                        if test "$system" = "Linux"; then
                                PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lnsl //'`
                                PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lposix //'`
                        fi
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lc //'`

                                AC_CHECK_HEADERS([EXTERN.h perl.h XSUB.h], [
                                        use_perl=yes
                                ],
                                [
                                        use_perl=no
                                        PERL_LDFLAGS=""
										DIS_PERL=No
                                        PERL_CFLAGS=""
                                        AC_MSG_RESULT([Failed. Perl Support Disabled. See the RELNOTES file])
                                        break
                                ],
                                [
                                #ifdef HAVE_EXTERN_H
                                #include <EXTERN.h>
                                #endif
                                #ifdef HAVE_PERL_H
                                #include <perl.h>
                                #endif
                                #ifdef HAVE_XSUB_H
                                #include <XSUB.h>
                                #endif
                                ]
                                )

                fi
AC_SUBST(PERL_CFLAGS)
AC_SUBST(PERL_LDFLAGS)
if test "x$use_perl" = "xyes"; then
		DIS_PERL=Yes
        AC_DEFINE_UNQUOTED(USE_PERL, 1, 'Enable Perl Support')
fi
])