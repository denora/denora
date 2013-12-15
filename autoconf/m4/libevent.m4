AC_DEFUN([CHK_LIBEVNT], [
dnl Check for optional stuff
AC_ARG_WITH(rtsig,
   AC_HELP_STRING([--with-rtsig], [compile with support for real time signals (experimental)]),
   [usertsig=yes], [usertsig=no])

dnl Checks for header files.
if test "x$ac_cv_header_sys_queue_h" = "xyes"; then
	AC_MSG_CHECKING(for TAILQ_FOREACH in sys/queue.h)
	AC_EGREP_CPP(yes,
[
#include <sys/queue.h>
#ifdef TAILQ_FOREACH
 yes
#endif
],	[AC_MSG_RESULT(yes)
	 AC_DEFINE(HAVE_TAILQFOREACH, 1,
		[Define if TAILQ_FOREACH is defined in <sys/queue.h>])],
	AC_MSG_RESULT(no)
	)
fi

if test "x$ac_cv_header_sys_time_h" = "xyes"; then
	AC_MSG_CHECKING(for timeradd in sys/time.h)
	AC_EGREP_CPP(yes,
[
#include <sys/time.h>
#ifdef timeradd
 yes
#endif
],	[ AC_DEFINE(HAVE_TIMERADD, 1,
		[Define if timeradd is defined in <sys/time.h>])
	  AC_MSG_RESULT(yes)] ,AC_MSG_RESULT(no)
)
fi

AC_MSG_CHECKING(for F_SETFD in fcntl.h)
AC_EGREP_CPP(yes,
[
#define _GNU_SOURCE
#include <fcntl.h>
#ifdef F_SETFD
yes
#endif
],	[ AC_DEFINE(HAVE_SETFD, 1,
	      [Define if F_SETFD is defined in <fcntl.h>])
	  AC_MSG_RESULT(yes) ], AC_MSG_RESULT(no))

needsignal=no
haveselect=no
AC_CHECK_FUNCS(select, [haveselect=yes], )
if test "x$haveselect" = "xyes" ; then
	EVNTLIBOBJS="$EVNTLIBOBJS select.o"
	EVNTLIBSRC="$EVNTLIBSRC select.c"

	needsignal=yes
fi

havepoll=no
havertsig=no
AC_CHECK_FUNCS(poll, [havepoll=yes], )
if test "x$havepoll" = "xyes" ; then
	EVNTLIBOBJS="$EVNTLIBOBJS poll.o"
	EVNTLIBSRC="$EVNTLIBSRC poll.c"

	needsignal=yes

	if test "x$usertsig" = "xyes" ; then
		AC_CHECK_FUNCS(sigtimedwait, [havertsig=yes], )
	fi
fi
if test "x$havertsig" = "xyes" ; then
	AC_MSG_CHECKING(for F_SETSIG in fcntl.h)
	AC_EGREP_CPP(yes,
[
#define _GNU_SOURCE
#include <fcntl.h>
#ifdef F_SETSIG
yes
#endif
],	[ AC_MSG_RESULT(yes) ], [ AC_MSG_RESULT(no); havertsig=no])
fi
if test "x$havertsig" = "xyes" ; then
	AC_DEFINE(HAVE_RTSIG, 1, [Define if your system supports POSIX realtime signals])
	EVNTLIBOBJS="$EVNTLIBOBJS rtsig.o"
	EVNTLIBSRC="$EVNTLIBSRC rtsig.c"

	AC_MSG_CHECKING(for working rtsig on pipes)
	AC_TRY_RUN(
[
#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

int sigio()
{
	exit(0);
}

int main()
{
	int fd[2];

	pipe(fd);
	signal(SIGIO, sigio);
	fcntl(fd[0], F_SETOWN, getpid());
	fcntl(fd[0], F_SETSIG, SIGIO);
	fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_ASYNC);
	write(fd[1], "", 1);
	return 1;
}
],	[ AC_MSG_RESULT(yes)
	  AC_DEFINE(HAVE_WORKING_RTSIG, 1, [Define if realtime signals work on pipes])],
	AC_MSG_RESULT(no))
fi

haveepoll=no
AC_CHECK_FUNCS(epoll_ctl, [haveepoll=yes], )
if test "x$haveepoll" = "xyes" ; then
	AC_DEFINE(HAVE_EPOLL, 1,
		[Define if your system supports the epoll system calls])
	EVNTLIBOBJS="$EVNTLIBOBJS epoll.o"
	EVNTLIBSRC="$EVNTLIBSRC epoll.c"

	needsignal=yes
fi

havedevpoll=no
if test "x$ac_cv_header_sys_devpoll_h" = "xyes"; then
	AC_DEFINE(HAVE_DEVPOLL, 1,
		    [Define if /dev/poll is available])
	EVNTLIBOBJS="$EVNTLIBOBJS devpoll.o"
	EVNTLIBSRC="$EVNTLIBSRC devpoll.c"

fi

havekqueue=no
if test "x$ac_cv_header_sys_event_h" = "xyes"; then
	AC_CHECK_FUNCS(kqueue, [havekqueue=yes], )
	if test "x$havekqueue" = "xyes" ; then
		AC_MSG_CHECKING(for working kqueue)
		AC_TRY_RUN(
#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
	int kq;
	int n;
	int fd[[2]];
	struct kevent ev;
	struct timespec ts;
	char buf[[8000]];

	if (pipe(fd) == -1)
		exit(1);
	if (fcntl(fd[[1]], F_SETFL, O_NONBLOCK) == -1)
		exit(1);

	while ((n = write(fd[[1]], buf, sizeof(buf))) == sizeof(buf))
		;

        if ((kq = kqueue()) == -1)
		exit(1);

	ev.ident = fd[[1]];
	ev.filter = EVFILT_WRITE;
	ev.flags = EV_ADD | EV_ENABLE;
	n = kevent(kq, &ev, 1, NULL, 0, NULL);
	if (n == -1)
		exit(1);
	
	read(fd[[0]], buf, sizeof(buf));

	ts.tv_sec = 0;
	ts.tv_nsec = 0;
	n = kevent(kq, NULL, 0, &ev, 1, &ts);
	if (n == -1 || n == 0)
		exit(1);

	exit(0);
}, [AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_WORKING_KQUEUE, 1,
		[Define if kqueue works correctly with pipes])
    EVNTLIBOBJS="$EVNTLIBOBJS kqueue.o"
	EVNTLIBSRC="$EVNTLIBSRC kqueue.c"],
    AC_MSG_RESULT(no), AC_MSG_RESULT(no))
	fi
fi

haveepollsyscall=no
if test "x$ac_cv_header_sys_epoll_h" = "xyes"; then
	if test "x$haveepoll" = "xno" ; then
		AC_MSG_CHECKING(for epoll system call)
		AC_TRY_RUN(
#include <stdint.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/epoll.h>
#include <unistd.h>

int
epoll_create(int size)
{
	return (syscall(__NR_epoll_create, size));
}

int
main(int argc, char **argv)
{
	int epfd;

	epfd = epoll_create(256);
	exit (epfd == -1 ? 1 : 0);
}, [AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_EPOLL, 1,
	[Define if your system supports the epoll system calls])
    needsignal=yes
    EVNTLIBOBJS="$EVNTLIBOBJS epoll_sub.o epoll.o"
    EVNTLIBOBJS="$EVNTLIBSRC epoll_sub.c epoll.c"],
    AC_MSG_RESULT(no), AC_MSG_RESULT(no))
	fi
fi

if test "x$needsignal" = "xyes" ; then
    	EVNTLIBOBJS="$EVNTLIBOBJS signal.o"
    	EVNTLIBSRC="$EVNTLIBSRC signal.c"
fi

AC_REPLACE_FUNCS(err)

AC_MSG_CHECKING([for socklen_t])
AC_TRY_COMPILE([
 #include <sys/types.h>
 #include <sys/socket.h>],
  [socklen_t x;],
  AC_MSG_RESULT([yes]),
  [AC_MSG_RESULT([no])
  AC_DEFINE(socklen_t, unsigned int,
	[Define to unsigned int if you dont have it])]
)
])