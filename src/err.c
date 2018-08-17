#include "err.h"

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
void show_error(char *str) {
  char *syserr;
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
