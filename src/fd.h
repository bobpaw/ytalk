#ifndef YTALK_FD_H_
#define YTALK_FD_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef TIME_WITH_SYS_TIME
	#include <sys/time.h>
	#include <time.h>
#else
	#ifdef HAVE_SYS_TIME
		#include <sys/time.h>
	#else
		#include <time.h>
	#endif
#endif

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <signal.h>

#ifdef _AIX
# include <sys/select.h>
#endif
#include <errno.h>

#include "ytypes.h"
#include "menu.h"
#include "err.h"
#include "comm.h"

extern int user_winch;
void init_fd    (void);
void add_fd     (int fd, void (*user_func) ());
void remove_fd  (int fd);
int  full_read  (int fd, char *buf, size_t len);
void main_loop  (void);
void input_loop (void);
void bail_loop  (void);

#endif // YTALK_FD_H_
