#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_PWD_H
	#include <pwd.h>
#endif

#include "mem.h"
#include "ytypes.h"

#ifndef IS_WHITE
	#define IS_WHITE(c)	((c)==' ' || (c)=='\t' || (c)=='\n')
#endif

#ifndef YTALK_RC_H_
#define YTALK_RC_H_

void read_ytalkrc   (void);
char *resolve_alias (char *uh);

#endif // YTALK_RC_H_
