#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([YTalk], [3.4.0], [aiden.woodruff@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

dnl See if we want debugging output.
AX_IS_RELEASE([git-directory])
AX_CHECK_ENABLE_DEBUG([yes], [YTALK_DEBUG])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr])

dnl Check for keypad()
AC_CHECK_LIB(ncurses, keypad,
	[AC_DEFINE(HAVE_KEYPAD, 1, [Define to 1 if you have keypad()])],
[AC_CHECK_LIB(curses, keypad,
	[AC_DEFINE(HAVE_KEYPAD, 1, [Define to 1 if you have keypad()])],
[])])

dnl Look for termios, and if that fails, try sgtty.
AC_CHECK_FUNC(tcgetattr, [\
  AC_CHECK_HEADERS(termios.h,, no_termios=1)], no_termios=1)
if test "x$no_termios" = "x1" ; then
  AC_CHECK_HEADERS(sgtty.h,, AC_MSG_ERROR(Cannot find either termios or sgtty))
fi

dnl Some systems need extra libraries for sockets and dns...
AC_CHECK_FUNC(socket,,AC_CHECK_LIB(socket, socket,, [AC_MSG_ERROR(Can't compile without socket)]))
AC_CHECK_FUNC(gethostbyname,,AC_CHECK_LIB(nsl, gethostbyname,, [AC_MSG_ERROR(Can't compile without gethostbyname)]))

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h sgtty.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h termios.h unistd.h sys/stat.h stdlib.h string.h util.h time.h pwd.h])
AC_HEADER_STDC
AC_HEADER_SYS_WAIT

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
AC_HEADER_TIME
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(u_int)
AC_CHECK_TYPE([socklen_t], [AC_DEFINE(HAVE_SOCKLEN_T, 1, [Define to 1 if you have the `socklen_t` type])], [], [
	#include <sys/types.h>
	#include <sys/socket.h>])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 endpwent gethostbyaddr gethostbyname gethostname inet_ntoa memset putenv select socket strchr strdup strerror putenv strerror ttyname ptsname grantpt unlockpt setsid tcflush snprintf sigprocmask sigsetmask sighold, break])
AC_CHECK_FUNCS(openpty,,
	[AC_CHECK_LIB(util, openpty, [AC_DEFINE(HAVE_OPENPTY)] [LIBS="$LIBS -lutil"])
])
AC_CHECK_FUNC(gethostname,,AC_MSG_ERROR(Can't run without gethostname))
AC_CHECK_FUNC(select,,AC_MSG_ERROR(Can't compile without select))


AC_CONFIG_FILES([Makefile
                 docs/Makefile
                 src/Makefile
                 test/Makefile
                 docs/ytalk.1])
AM_INIT_AUTOMAKE([subdir-objects])
AC_OUTPUT
