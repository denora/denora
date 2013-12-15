
AC_DEFUN([AUTOTOOLS_EMBED_PERL], [
        AC_ARG_ENABLE(perl,
        [  --disable-perl          don't build the perl plugin],
                perl=$enableval, perl=yes)

        if test "$perl" = yes; then
                AC_MSG_CHECKING([Configuring for Perl Modules Now])
                AC_PATH_PROG(sedpath, sed)
                if test "_$sedpath" = _; then
                        AC_MSG_ERROR("Cannot find sed: I need it\!")
                fi

                AC_PATH_PROG(perlpath, perl)

                AC_MSG_CHECKING(for Perl compile flags)
                PERL_CFLAGS=`$perlpath -MExtUtils::Embed -e ccopts 2>/dev/null`
dnl                PERL_CFLAGS=`echo $PERL_CFLAGS |$sedpath 's/-Wdeclaration-after-statement //'`
                if test "_$PERL_CFLAGS" = _ ; then
                        AC_MSG_RESULT([not found, building without perl.])
                        perl=no
                else
                        AC_MSG_RESULT(ok)
                        AC_MSG_CHECKING([Checking for perl includes directory])
                        PERL_INCLUDES=`$perlpath -MExtUtils::Embed -e perl_inc 2>/dev/null`
                        AC_MSG_RESULT($PERL_INCLUDES)
                        CFLAGS="$CFLAGS "$PERL_INCLUDES
                        PERL_LDFLAGS=`$perlpath -MExtUtils::Embed -e ldopts |$sedpath 's/-lgdbm //'`
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-ldb //'`
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lndbm //'`
                        if test "$system" = "Linux"; then
                                PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lnsl //'`
                                PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lposix //'`
                        fi
                        PERL_LDFLAGS=`echo $PERL_LDFLAGS |$sedpath 's/-lc //'`

                        AC_MSG_CHECKING(for $perlpath >= 5.8.0)
                        PERL_VER=`$perlpath -e 'print $]>= 5.008?"yes":"no"'`

                        if test "$PERL_VER" = "yes"; then
                                AC_MSG_RESULT(yes)
                                perl=no
dnl                                AC_MSG_CHECKING([Checking for perl compiler, linker, libraries and headers])
                                AC_CHECK_HEADERS([EXTERN.h perl.h XSUB.h], [
                                        perl=yes
                                ],
                                [
                                        perl=no
                                        PERL_LDFLAGS=""
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
                        else
                                AC_MSG_RESULT(no)
                                PERL_LDFLAGS=""
                                PERL_CFLAGS=""
                                perl=no
                        fi
                fi
        fi


AC_SUBST(PERL_CFLAGS)
AC_SUBST(PERL_LDFLAGS)
if test "x$perl" = "xyes"; then
        AC_DEFINE_UNQUOTED(USE_PERL, 1, 'Enable Perl Support')
fi
])