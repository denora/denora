dnl *********************************************************************
dnl ** PYTHON ***********************************************************
dnl *********************************************************************


AC_DEFUN([AUTOTOOLS_EMBED_PYTHON], [
        AC_PATH_PROG(pythonpath, python2)
        if test "_$pythonpath" = _ ; then
                AC_PATH_PROG(pythonpath, python)
        fi
        if test "_$pythonpath" = _ ; then
                python=no
        else
                AC_MSG_CHECKING(Python version)
                changequote(<<, >>)dnl
                PY_VER=`$pythonpath -c 'import distutils.sysconfig; print distutils.sysconfig.get_config_vars("VERSION")[0];'`
                PY_LIB=`$pythonpath -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib(standard_lib=1);'`
                PY_INC=`$pythonpath -c 'import distutils.sysconfig; print distutils.sysconfig.get_config_vars("INCLUDEPY")[0];'`
                $pythonpath -c "import sys; map(int,sys.version[:3].split('.')) >= [2,2] or sys.exit(1)"
                changequote([, ])dnl
                AC_MSG_RESULT($PY_VER)
                if test "$?" != "1"; then
                        AC_MSG_CHECKING(Python compile flags)
                        PY_PREFIX=`$pythonpath -c 'import sys; print sys.prefix'`
                        PY_EXEC_PREFIX=`$pythonpath -c 'import sys; print sys.exec_prefix'`
                        if test -f $PY_INC/Python.h; then
                                AC_DEFINE(WITH_PYTHON, 1, [Define if using python])
                                PY_LIBS="-L$PY_LIB/config -lpython$PY_VER -lpthread -lutil"
                                PY_CFLAGS="-I$PY_INC"
                                AC_MSG_RESULT(ok)
                                CFLAGS="$CFLAGS $PY_CFLAGS"
                                LIBS="$LIBS $PY_LIBS"
                        else
                                python=no
                                AC_MSG_RESULT([Can't find Python.h])
                        fi
                else
                        echo "Python too old. Only 2.2 or above is supported."
                        python=no
                fi
        fi
fi
])