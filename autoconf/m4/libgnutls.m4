AC_DEFUN([AUTOTOOLS_GNUTLS_SSL], [
	hold_denora=$DENORALIBS
    AC_CHECK_LIB(gnutls, gnutls_global_init, DENORALIBS="$DENORALIBS-lgnutls" )
    AC_CHECK_FUNCS(gnutls_global_init, x_ssl_gnutls=yes, x_ssl_gnutls=no)
	if test "$x_ssl_gnutls" = "no"; then
		DENORALIBS=$hold_denora
		DIS_GNUTLS=NO
	else
		AC_DEFINE_UNQUOTED(USE_GNUTLS,1,"Use GNUTLS")
		DIS_GNUTLS=Yes
	fi
 ]
)