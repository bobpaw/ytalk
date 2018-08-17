#ifndef YTALK_ERR_H_
#define YTALK_ERR_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef HAVE_UNISTD_H_
	#include <unistd.h>
#endif

#include <stdio.h>

#include "menu.h"
#include "ytypes.h"

void bail (int n);
void show_error (char * str);

#endif // YTALK_ERR_H_
