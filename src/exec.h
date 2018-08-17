#ifndef YTALK_EXEC_H_
#define YTALK_EXEC_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_PWD_H
	#include <pwd.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif

#ifdef HAVE_FCNTL_H
	#include <fcntl.h>
#endif

#include <signal.h>
#include <sys/wait.h>

#ifdef HAVE_STROPTS_H
	#include <stropts.h>
#endif

#ifdef HAVE_SYS_CONF_H
	#include <sys/conf.h>
#endif

#ifdef HAVE_TERMIOS_H
	#include <termios.h>
#else
	#ifdef HAVE_SGTTY_H
		#include <sgtty.h>
		#ifdef hpux
			#include <sys/bsdtty.h>
		#endif
	#endif
#endif

#ifdef HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif

#ifdef HAVE_OPENPTY
	#ifdef HAVE_UTIL_H
		#include <util.h>
	#else
		int openpty(int *, int *, char *, struct termios *, struct winsize *);
	#endif
#endif

#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#if defined(HAVE_PTSNAME) && defined(HAVE_GRANTPT) && defined(HAVE_UNLOCKPT)
	#define USE_DEV_PTMX
#endif

#include "user.h"
#include "term.h"
#include "socket.h"

void execute     (char *command);
void update_exec (void);
void kill_exec   (void);
void winch_exec  (void);

#endif // YTALK_EXEC_H_
