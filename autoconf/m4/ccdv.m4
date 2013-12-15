AC_DEFUN([wi_ARG_DISABLE_CCDV], [
AC_ARG_ENABLE(ccdv,[  --disable-ccdv          disable use of ccdv program in Makefiles],use_ccdv="$enableval",use_ccdv=yes)
])
dnl
dnl
dnl
dnl

AC_DEFUN([wi_PATH_PWD], [
AC_PATH_PROG(wi_PWD, "pwd", "pwd")
])


AC_DEFUN([wi_RESOLVE_PATH], [
AC_REQUIRE([wi_PATH_PWD])
wi_path="$1"
if test -d "$wi_path" ; then
	wi_resolved_path=`cd "$wi_path" ; $wi_PWD 2>/dev/null`
elif test ! -f "$wi_path" ; then
	wi_resolved_path="$wi_path"
else
	wi_parent_dir="$wi_path"
	wi_file_name="$wi_path"
	wi_DIRNAME(wi_parent_dir)
	wi_BASENAME(wi_file_name)
	wi_resolved_path=`cd "$wi_parent_dir" ; $wi_PWD 2>/dev/null`
	if test "x$wi_resolved_path" != x ; then
		wi_resolved_path="$wi_resolved_path/$wi_file_name"
	fi
	unset wi_parent_dir wi_file_name
fi
if test "x$wi_resolved_path" = x ; then
	$2="[$]$1"
else
	$2="$wi_resolved_path"
fi
unset wi_resolved_path wi_path
])
dnl

AC_DEFUN([wi_PROG_CCDV_NO_CREATE], [
if test "x$use_ccdv" = "xno" ; then
	AC_MSG_CHECKING([for ccdv])
	AC_MSG_RESULT([(disabled)])
else
	AC_PATH_PROG(CCDV, "CCDV", "")
	CCDV="$wi_cv_path_ccdv"
	if test "x$CCDV" != x ; then
		CCDV="@$CCDV "	# trailing space needed
	fi
AC_SUBST(CCDV)
fi
])
dnl
dnl
dnl
dnl
AC_DEFUN([wi_PROG_CCDV], [
if test "x$use_ccdv" = "xno" ; then
	AC_MSG_CHECKING([for ccdv])
	AC_MSG_RESULT([(disabled)])
else
unset wi_cv_path_ccdv	# can't use cache if it was a temp prog last time
wi_used_cache_path_ccdv="yes"
AC_CACHE_CHECK([for ccdv], [wi_cv_path_ccdv], [
wi_used_cache_path_ccdv="no"
for CCDV in /usr/bin/ccdv /usr/local/bin/ccdv /usr/ccs/bin/ccdv autoconf NONE
do
	if test "$CCDV" = NONE ; then CCDV="" ; break ; fi
	$CCDV >/dev/null 2>&1
	if test $? -eq 96 ; then
		break
	fi
done
if test "x$CCDV" = "x" ; then
	${CC-cc} $DEFS $CPPFLAGS $CFLAGS "ccdv.c" -o "ccdv" >/dev/null 2>&1
	strip ./ccdv >/dev/null 2>&1
	./ccdv >/dev/null 2>&1
	if test $? -eq 96 ; then
		CCDV="./ccdv"
		USECCDV="#"
	else
		/bin/rm -f ccdv
		USECCDV=""
	fi
fi
if test "x$CCDV" != x ; then
	wi_RESOLVE_PATH([$CCDV], [CCDV])
	wi_cv_path_ccdv="$CCDV"
	CCDV="@$CCDV "	# trailing space needed
	USECCDV="#"
else
	wi_cv_path_ccdv=""
	USECCDV=""

fi
])
if test "$wi_used_cache_path_ccdv" = yes ; then
	CCDV="$wi_cv_path_ccdv"
	if test "x$CCDV" != x ; then
		CC="$CCDV $CC"	# trailing space needed
	fi
fi
AC_SUBST(CCDV)
AC_SUBST(USECCDV)
fi
])
AC_DEFUN([wi_DIRNAME], [
changequote({{, }})dnl
$1=`echo "{{$}}$1" | sed -n '
s-//*-/-g
s-/*$--
/^\/\/*[^/]*$/{
	c\\
/
	p
	q
}
/^[^/]*$/{
	c\\
.
	p
	q
}
/./{
	s-/[^/]*$--
	p
	q
}'`dnl
changequote([, ])dnl
])

AC_DEFUN([wi_BASENAME], [
$1=`echo "[$]$1" | sed -n '
s-//*-/-g
s-/*$--
s-^/$--
s-^.*/--
p
q'`dnl
])