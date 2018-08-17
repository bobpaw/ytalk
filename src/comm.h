#ifndef YTALK_COMM_H_
#define YTALK_COMM_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_IOVEC_H
# include <iovec.h>
#endif

#ifdef HAVE_SYS_UIO_H
# include <sys/uio.h>
#endif

#ifdef TIME_WITH_SYS_TIME
	#include <sys/time.h>
	#include <time.h>
#else
	#ifdef HAVE_SYS_TYPES_H
		#include <sys/time.h>
	#else
		#include <time.h>
	#endif
#endif

#ifndef IN_ADDR
#define IN_ADDR(s)	((s).sin_addr.s_addr)
#endif // IN_ADDR

/* oops, some systems don't have this one */
#ifndef INADDR_LOOPBACK
# define INADDR_LOOPBACK 0x7f000001
#endif

#include "term.h"
#include "ytypes.h"
#include "user.h"
#include "rc.h"
#include "gtalk.h"

yuser *invite(char *name, int send_announce);
void house_clean(void);
void send_winch(yuser *user);
void send_region(void);
void send_end_region(void);
void send_users(yuser *user, ychar *buf, int len, ychar *cbuf, int clen);
void show_input(yuser *user, ychar *buf, int len);
void my_input(yuser *user, ychar *buf, int len);
void lock_flags(ylong flags);
void unlock_flags(void);
void rering_all(void);

#endif // YTALK_COMM_H_
