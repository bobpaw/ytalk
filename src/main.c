/*
 * src/main.c
 *
 * YTalk
 *
 * Copyright (C) 1990,1992,1993 Britt Yenne
 * Currently maintained by Andreas Kling
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "config.h"
#include "cmdline-ytalk.h"
#include "header.h"
#include "menu.h"
#include "mem.h"

#include <signal.h>

char errstr[MAXERR];		/* temporary string for errors */
char *vhost = NULL;		/* specified virtual host */
ylong myuid;			/* global uid */

/*
 * Clean up and exit.
 */
void bail(int n) {
	kill_auto();
	if (n == YTE_SUCCESS_PROMPT && (def_flags & FL_PROMPTQUIT))
		if (show_mesg("Press any key to quit.", NULL) == 0) {
			update_menu();
			bail_loop();
		}
	end_term();
	free_users();
#ifdef YTALK_DEBUG
	clear_all();
#endif
	exit(n == YTE_SUCCESS_PROMPT ? YTE_SUCCESS : n);
}

#ifndef HAVE_STRERROR
# define strerror(n)	(sys_errlist[(n)])
#endif

/*
 * Display an error.
 */
void show_error(register char *str) {
	register char *syserr;
	static int in_error = 0;

	if (errno == 0)
		syserr = NULL;
	else
		syserr = strerror(errno);

	if (def_flags & FL_BEEP)
		putc(7, stderr);
	if (in_error == 0 && what_term() != 0) {
		in_error = 1;
		if (show_error_menu(str, syserr) < 0) {
			show_error("show_error: show_error_menu() failed");
			show_error(str);
		} else
			update_menu();
		in_error = 0;
	} else {
		if (syserr != NULL)
			fprintf(stderr, "%s: %s\n", str, syserr);
		else
			fprintf(stderr, "%s\n", str);
		sleep(2);
	}
}

/*
 * Copy a string.
 */
char * str_copy(register char *str) {
	register char *out;
	register size_t len;

	if (str == NULL)
		return NULL;
	len = strlen(str) + 1;
	out = get_mem(len);
	memcpy(out, str, len);
	return out;
}

/*
 * Process signals.
 */
static RETSIGTYPE got_sig(int n) {
	if (n == SIGINT) {
		if (def_flags & FL_IGNBRK)
			return;
		bail(YTE_SUCCESS);
	}
	bail(YTE_SIGNAL);
}

/* MAIN  */
int main(int argc, char * argv[]) {
	int sflg = 0, yflg = 0, iflg = 0, vflg = 0, qflg = 0, eflg = 0;
	char *prog;

#if SIZEOF_U_INT != 4
	/* check for a 64-bit mis-compile */
	fprintf(stderr, "The definition for ylong in header.h is wrong;\n\
please change it to an unsigned 32-bit type that works on your computer,\n\
then type 'make clean' and 'make'.\n");
	exit(YTE_INIT);
#endif

	struct gengetopt_args_info args_info;
	if (cmdline_parser(argc, argv, &args_info) != 0) exit(EXIT_FAILURE);

	if (args_info.no_auto_invite_given) iflg++;
	if (args_info.caps_given) yflg++;
	if (args_info.escape_given) eflg++;
	if (args_info.shell_given) sflg++;
	if (args_info.prompt_given) qflg++;
	if (args_info.host_given) vhost = args_info.host_arg;
	if (args_info.inputs_num == 0) {
		cmdline_parser_print_help();
		cmdline_parser_free(&args_info);
		exit(EXIT_SUCCESS);
	}

	/* set up signals */
	signal(SIGINT, got_sig);
	signal(SIGHUP, got_sig);
	signal(SIGQUIT, got_sig);
	signal(SIGABRT, got_sig);
	signal(SIGPIPE, SIG_IGN);

	/* save the uid for later use */
	myuid = getuid();

	/* set default options */
	def_flags = FL_PROMPTRING | FL_RING | FL_BEEP | FL_SCROLL;

	/* go for it! */
	errno = 0;
	init_fd();
	read_ytalkrc();
	init_user(vhost);
	if (yflg)
		def_flags |= FL_CAPS;
	if (eflg)
		def_flags |= FL_ESC_YN;
	if (iflg)
		def_flags |= FL_NOAUTO;
	if (qflg)
		def_flags |= FL_PROMPTQUIT;

	init_term();
	init_socket();
	for (int i = 0; i < args_info.inputs_num;++i)
		invite(args_info.inputs[i], 1);
	if (sflg)
		execute(NULL);
	else
		msg_term(me, "Waiting for connection...");
	main_loop();
	bail(YTE_SUCCESS_PROMPT);
	cmdline_parser_free(&args_info);
	exit(EXIT_SUCCESS);
}
