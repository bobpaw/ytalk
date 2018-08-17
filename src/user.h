#ifndef YTALK_USER_H_
#define YTALK_USER_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_PWD_H
	#include <pwd.h>
#endif

#ifdef HAVE_STRING_H
	#include <string.h>
#endif

#include "ytypes.h"
#include "menu.h"
#include "mem.h"

void   init_user          (char *vhost);
yuser *new_user           (char *name, char *hostname, char *tty);
void   free_user          (yuser *user);
yuser *find_user          (char *name, ylong host_addr, ylong pid);
void   generate_full_name (yuser *user);
void   free_users         (void);

#endif // YTALK_USER_H_
