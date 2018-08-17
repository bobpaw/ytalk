#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <errno.h>

#ifdef HAVE_UNISTD_H_
	#include <unistd.h>
#endif

#ifdef HAVE_STRING_H
	#include <string.h>
#endif

#ifdef HAVE_FCNTL_H
	#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifndef SIGCHLD
	#define SIGCLD SIGCHLD
#endif

#if defined(HAVE_PTSNAME) && defined(HAVE_GRANTPT) && defined(HAVE_UNLOCKPT)
extern int grantpt();
extern int unlockpt();
extern char *ptsname();
#endif

#if !defined(HAVE_OPENPTY) || defined(YTALK_TEST)

int getpty(char *name);

#endif /* !HAVE_OPENPTY || YTALK_TEST */
