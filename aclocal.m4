dnl Macro file
dnl
dnl Denora (c) 2004-2012 Nomad Dev Team
dnl Contact us at info@denorastats.org
dnl
dnl This program is free but copyrighted software; see the file COPYING for
dnl details.
dnl
dnl Based on the original code of Anope by Anope Team.
dnl Based on the original code of Thales by Lucas.
dnl
dnl 
dnl
dnl Check the size of several types and define a valid int16_t and int32_t.
dnl

AC_DEFUN([AC_C_LONG_LONG],
[AC_CACHE_CHECK(for long long int, ac_cv_c_long_long,
[if test "$GCC" = yes; then
  ac_cv_c_long_long=yes
  else
        AC_TRY_COMPILE(,[long long int i;],
   ac_cv_c_long_long=yes,
   ac_cv_c_long_long=no)
   fi])
   if test $ac_cv_c_long_long = yes; then
	AC_DEFINE([HAVE_LONG_LONG], 1, "System supports long long")
   fi
])


AC_DEFUN(DENORA_CHECK_TYPE_SIZES,
[dnl Check type sizes
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
if test "$ac_cv_sizeof_int" = 2 ; then
  AC_CHECK_TYPE(int16_t, int)
  AC_CHECK_TYPE(u_int16_t, unsigned int)
elif test "$ac_cv_sizeof_short" = 2 ; then
  AC_CHECK_TYPE(int16_t, short)
  AC_CHECK_TYPE(u_int16_t, unsigned short)
else
  AC_MSG_ERROR([Cannot find a type with size of 16 bits])
fi
if test "$ac_cv_sizeof_int" = 4 ; then
  AC_CHECK_TYPE(int32_t, int)
  AC_CHECK_TYPE(u_int32_t, unsigned int)
elif test "$ac_cv_sizeof_short" = 4 ; then
  AC_CHECK_TYPE(int32_t, short)
  AC_CHECK_TYPE(u_int32_t, unsigned short)
elif test "$ac_cv_sizeof_long" = 4 ; then
  AC_CHECK_TYPE(int32_t, long)
  AC_CHECK_TYPE(u_int32_t, unsigned long)
else
  AC_MSG_ERROR([Cannot find a type with size of 32 bits])
fi
AC_C_LONG_LONG
AC_CHECK_TYPE(int64_t, long long)
AC_CHECK_TYPE(uint64_t, unsigned long long)
])

dnl # Configure paths for mysql-client, GPLv2
dnl #
dnl #ken restivo modified 2001/08/04 to remove NULLs and use 0, in case NULL undefined.
dnl # Markus Fischer <[EMAIL PROTECTED]>,  23.9.1999
dnl # URL : http://josefine.ben.tuwien.ac.at/~mfischer/m4/mysql-client.m4
dnl # Last Modified : Thu Sep 23 14:24:15 CEST 1999
dnl #
dnl # written from scratch

dnl Test for libmysqlclient and 
dnl define MYSQLCLIENT_CFLAGS, MYSQLCLIENT_LDFLAGS and MYSQLCLIENT_LIBS
dnl usage:
dnl AM_PATH_MYSQLCLIENT(
dnl     [MINIMUM-VERSION, 
dnl     [ACTION-IF-FOUND [, 
dnl     ACTION-IF-NOT-FOUND ]]])
dnl

AC_DEFUN(AM_PATH_MYSQLCLIENT,
[
AC_ARG_WITH(mysqlclient-prefix, 
                [  --with-mysqlclient-prefix=PFX Prefix where mysqlclient is 
installed],
            mysqlclient_prefix="$withval",
            mysqlclient_prefix="")

AC_ARG_WITH(mysqlclient-include, [  --with-mysqlclient-include=DIR Directory pointing 
             to mysqlclient include files],
            mysqlclient_include="$withval",
            mysqlclient_include="")

AC_ARG_WITH(mysqlclient-lib,
[  --with-mysqlclient-lib=LIB  Directory pointing to mysqlclient library
                          (Note: -include and -lib do override
                           paths found with -prefix)
],
            mysqlclient_lib="$withval",
            mysqlclient_lib="")

    AC_MSG_CHECKING([for mysqlclient ifelse([$1], , ,[>= v$1])])
    MYSQLCLIENT_LDFLAGS=""
    MYSQLCLIENT_CFLAGS=""
    MYSQLCLIENT_LIBS="-lmysqlclient"
    mysqlclient_fail=""

    dnl test --with-mysqlclient-prefix
        for tryprefix in /usr /usr/local /usr/mysql /usr/local/mysql /usr/pkg $msqlclient_prefix; do
                #testloop
                for hloc in lib/mysql lib ; do
                        if test -e "$tryprefix/$hloc/libmysqlclient.so"; then
                MYSQLCLIENT_LDFLAGS="-L$tryprefix/$hloc"
                        fi
                done

                for iloc in include/mysql include; do
                        if test -e "$tryprefix/$iloc/mysql.h"; then
                MYSQLCLIENT_CFLAGS="-I$tryprefix/$iloc"
            fi
        done
                # testloop
        done

    dnl test --with-mysqlclient-include
    if test "x$mysqlclient_include" != "x" ; then
                echo "checking for mysql includes... "
        if test -d "$mysqlclient_include/mysql" ; then
            MYSQLCLIENT_CFLAGS="-I$mysqlclient_include"
                        echo " found $MYSQLCLIENT_CFLAGS"
        elif test -d "$mysqlclient_include/include/mysql" ; then
            MYSQLCLIENT_CFLAGS="-I$mysqlclient_include/include"
                        echo " found $MYSQLCLIENT_CFLAGS"
        elif test -d "$mysqlclient_include" ; then
            MYSQLCLIENT_CFLAGS="-I$mysqlclient_include"
                        echo "found $MYSQLCLIENT_CFLAGS"
                else
                        echo "not found!  no include dir found in $mysqlclient_include"
        fi
    fi

    dnl test --with-mysqlclient-lib
    if test "x$mysqlclient_lib" != "x" ; then
                echo "checking for mysql libx... "
        if test -d "$mysqlclient_lib/lib/mysql" ; then
            MYSQLCLIENT_LDFLAGS="-L$mysqlclient_lib/lib/mysql"
                        echo "found $MYSQLCLIENT_LDFLAGS"
        elif test -d "$mysqlclient_lib/lin" ; then
            MYSQLCLIENT_LDFLAGS="-L$mysqlclient_lib/lib"
                        echo "found $MYSQLCLIENT_LDFLAGS"
        else
            MYSQLCLIENT_LDFLAGS="-L$mysqlclient_lib"
                        echo "defaultd to $MYSQLCLIENT_LDFLAGS"
        fi
    fi

    ac_save_CFLAGS="$CFLAGS"
    ac_save_LDFLAGS="$LDFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="-v $CFLAGS $MYSQLCLIENT_CFLAGS"
    LDFLAGS="$LDFLAGS $MYSQLCLIENT_LDFLAGS"
    LIBS="$LIBS $MYSQLCLIENT_LIBS"
    dnl if no minimum version is given, just try to compile
    dnl else try to compile AND run
        AC_TRY_COMPILE([
            #include <mysql.h>
            #include <mysql_version.h>
        ],[
            mysql_connect( 0, 0, 0, 0);
        ], [AC_MSG_RESULT(yes $MYSQLCLIENT_CFLAGS $MYSQLCLIENT_LDFLAGS)
           CFLAGS="$ac_save_CFLAGS"
           LDFLAGS="$ac_save_LDFLAGS"
           LIBS="$ac_save_LIBS"
           ifelse([$2], ,:,[$2])
        ],[
                        echo "no"
                        echo "can't compile a simple app with mysql_connnect in it. 
bad."
          mysqlclient_fail="yes"
        ])

    if test "x$mysqlclient_fail" != "x" ; then
            dnl AC_MSG_RESULT(no)
            echo
            echo "***"
            echo "*** mysqlclient test source had problems, check your config.log ."
            echo "*** Also try one of the following switches :"
            echo "***   --with-mysqlclient-prefix=PFX"
            echo "***   --with-mysqlclient-include=DIR"
            echo "***   --with-mysqlclient-lib=DIR"
            echo "***"
            CFLAGS="$ac_save_CFLAGS"
            LDFLAGS="$ac_save_LDFLAGS"
            LIBS="$ac_save_LIBS"
            ifelse([$3], ,:,[$3])
    fi

    CFLAGS="$ac_save_CFLAGS"
    LDFLAGS="$ac_save_LDFLAGS"
    LIBS="$ac_save_LIBS"
    AC_SUBST(MYSQLCLIENT_LDFLAGS)
    AC_SUBST(MYSQLCLIENT_CFLAGS)
    AC_SUBST(MYSQLCLIENT_LIBS)
])

define(X86_PATTERN,
[[i?86*-*-* | k[5-8]*-*-* | pentium*-*-* | athlon-*-* | viac3*-*-*]])

define(POWERPC64_PATTERN,
[[powerpc64-*-* | powerpc64le-*-* | powerpc620-*-* | powerpc630-*-*]])

dnl Borrowed from Atheme
dnl DENORA_GCC_TRY_FLAGS(<warnings>,<cachevar>)
AC_DEFUN([DENORA_GCC_TRY_FLAGS],[
 AC_MSG_CHECKING([GCC flag(s) $1])
 if test "${GCC-no}" = yes
 then
  AC_CACHE_VAL($2,[
   oldcflags="${CFLAGS-}"
   CFLAGS="${CFLAGS-} ${STRICT} $1 -Werror"
   AC_TRY_COMPILE([
#include <string.h>
#include <stdio.h>
int main(void);
],[
    strcmp("a","b"); fprintf(stdout,"test ok\n");
], [$2=yes], [$2=no])
   CFLAGS="${oldcflags}"])
  if test "x$$2" = xyes; then
   STRICT="${STRICT}$1 "
   AC_MSG_RESULT(ok)  
  else
   $2=''
   AC_MSG_RESULT(no)
  fi
 else
  AC_MSG_RESULT(no, not using GCC)
 fi
])

dnl Borrowed from Atheme
dnl DENORA_CPU_GCC_TRY_FLAGS(<warnings>,<cachevar>)
AC_DEFUN([DENORA_CPU_GCC_TRY_FLAGS],[
 AC_MSG_CHECKING([GCC Optimizing flag(s) $1])
 if test "${GCC-no}" = yes
 then
  AC_CACHE_VAL($2,[
   oldcflags="${CFLAGS}"
   CFLAGS="${CFLAGS} ${OPCFLAGS} $1 -Werror"
   AC_TRY_COMPILE([
#include <string.h>
#include <stdio.h>
int main(void);
],[
    strcmp("a","b"); fprintf(stdout,"test ok\n");
], [$2=yes], [$2=no])
   CFLAGS="${oldcflags}"])
  if test "x$$2" = xyes; then
   OPCFLAGS="${OPCFLAGS}$1 "
   AC_MSG_RESULT(ok)  
  else
   $2=''
   AC_MSG_RESULT(no)
  fi
 else
  AC_MSG_RESULT(no, not using GCC)
 fi
])

dnl Borrowed from Atheme
dnl DENORA_CFLAGS_GCC_TRY_FLAGS(<warnings>,<cachevar>)
AC_DEFUN([DENORA_CFLAGS_GCC_TRY_FLAGS],[
 AC_MSG_CHECKING([GCC flag(s) $1])
 if test "${GCC-no}" = yes
 then
  AC_CACHE_VAL($2,[
   oldcflags="${CFLAGS}"
   CFLAGS="${CFLAGS} $1 -Werror"
   AC_TRY_COMPILE([
#include <string.h>
#include <stdio.h>
int main(void);
],[
    strcmp("a","b"); fprintf(stdout,"test ok\n");
], [$2=yes], [$2=no])
   CFLAGS="${oldcflags}"])
  if test "x$$2" = xyes; then
   CFLAGS="${oldcflags} $1 "
   AC_MSG_RESULT(ok)  
  else
   $2=''
   AC_MSG_RESULT(no)
  fi
 else
  AC_MSG_RESULT(no, not using GCC)
 fi
])

AC_DEFUN([AC_C_VAR_PRETTY_FUNC],
[AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK(whether $CC recognizes __PRETTY_FUNCTION__, ac_cv_c_var_prettyfunc,
AC_TRY_COMPILE(,
[int main() {
char *s = __PRETTY_FUNCTION__;
}],
AC_DEFINE(HAVE_PRETTY_FUNCTION,,
[Define if the C complier supports __PRETTY_FUNCTION__]) ac_cv_c_var_prettyfunc=yes,
ac_cv_c_var_prettyfunc=no) )
])

AC_DEFUN([AC_C_VAR_FUNC],
[AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK(whether $CC recognizes __func__, ac_cv_c_var_func,
AC_TRY_COMPILE(,
[int main() {
char *s = __func__;
}],
AC_DEFINE(HAVE_FUNC,,
[Define if the C complier supports __func__]) ac_cv_c_var_func=yes,
ac_cv_c_var_func=no) )
])

AC_DEFUN([AC_C_VAR_FUNCTION],
[AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK(whether $CC recognizes __FUNCTION__, ac_cv_c_var_function,
AC_TRY_COMPILE(,
[int main() {
char *s = __FUNCTION__;
}],
AC_DEFINE(HAVE_FUNCTION,,
[Define if the C complier supports __FUNCTION__]) ac_cv_c_var_function=yes,
ac_cv_c_var_function=no) )
])

AC_DEFUN([TYPE_SOCKLEN_T],
[AC_CACHE_CHECK([for socklen_t], ac_cv_type_socklen_t,
[
  AC_TRY_COMPILE(
  [#include <sys/types.h>
   #include <sys/socket.h>],
  [socklen_t len = 42; return 0;],
  ac_cv_type_socklen_t=yes,
  ac_cv_type_socklen_t=no)
])
  if test $ac_cv_type_socklen_t != yes; then
    AC_DEFINE(socklen_t, int, [Substitute for socklen_t])
  fi
])

AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all, and "pthread-config"
# which is a program returning the flags for the Pth emulation library.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt pthread-config"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
#      ... -mt is also the pthreads flag for HP/aCC
# pthread: Linux, etcetera
# --thread-safe: KAI C++
# pthread-config: use pthread-config program (for GNU Pth library)

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthreads/-mt/
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthreads pthread -mt -pthread $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                pthread-config)
                AC_CHECK_PROG(acx_pthread_config, pthread-config, yes, no)
                if test x"$acx_pthread_config" = xno; then continue; fi
                PTHREAD_CFLAGS="`pthread-config --cflags`"
                PTHREAD_LIBS="`pthread-config --ldflags` `pthread-config --libs`"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: JOINABLE attribute is called UNDETACHED.
        AC_MSG_CHECKING([for joinable pthread attribute])
        attr_name=unknown
        for attr in PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED; do
            AC_TRY_LINK([#include <pthread.h>], [int attr=$attr; return attr;],
                        [attr_name=$attr; break])
        done
        AC_MSG_RESULT($attr_name)
        if test "$attr_name" != PTHREAD_CREATE_JOINABLE; then
            AC_DEFINE_UNQUOTED(PTHREAD_CREATE_JOINABLE, $attr_name,
                               [Define to necessary symbol if this constant
                                uses a non-standard name on your system.])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
            *-aix* | *-freebsd* | *-darwin*) flag="-D_THREAD_SAFE";;
            *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
            PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD

AC_DEFUN([CHECK_SSL],
[AC_MSG_CHECKING(if ssl is wanted)
AC_ARG_WITH(ssl,
[  --with-ssl enable ssl [will check /usr/local/ssl
                            /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr ]
],
[   AC_MSG_RESULT(yes)
    for dir in $withval /usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr; do
        ssldir="$dir"
        if test -f "$dir/include/openssl/ssl.h"; then
            found_ssl="yes";
            CFLAGS="$CFLAGS -I$ssldir/include/openssl -DHAVE_SSL";
            CXXFLAGS="$CXXFLAGS -I$ssldir/include/openssl -DHAVE_SSL";
            break;
        fi
        if test -f "$dir/include/ssl.h"; then
            found_ssl="yes";
            CFLAGS="$CFLAGS -I$ssldir/include/ -DHAVE_SSL";
            CXXFLAGS="$CXXFLAGS -I$ssldir/include/ -DHAVE_SSL";
            break
        fi
    done
    if test x_$found_ssl != x_yes; then
        AC_MSG_ERROR(Cannot find ssl libraries)
    else
        printf "OpenSSL found in $ssldir\n";
        DENORALIBS="$DENORALIBS -lssl -lcrypto ";
        LDFLAGS="$LDFLAGS -L$ssldir/lib";
        HAVE_SSL=yes
    fi
    AC_SUBST(HAVE_SSL)
],
[
    AC_MSG_RESULT(no)
])
])

