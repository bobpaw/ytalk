#ifndef YTALK_TERM_H_
#define YTALK_TERM_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif

#ifdef HAVE_TERMIOS_H
	#include <termios.h>
#else
	#ifdef HAVE_SGTTY
		#include <sgtty.h>
		#ifdef hpux
			#include <sys/bsdtty.h>
		#endif
		#define USE_SGTTY
	#endif
#endif

#include "ytypes.h"
#include "mem.h"
#include "cwin.h"
#include "menu.h"
#include "vt100.h"

void init_term(void);
void set_terminal_size(int fd, int rows, int cols);
void set_terminal_flags(int fd);
int what_term(void);
void keypad_term(yuser *user, int bf);
void end_term(void);
int open_term(yuser *user, char *title);
void close_term(yuser *user);
void addch_term(yuser *user, ychar c);
void move_term(yuser *user, int y, int x);
void fill_term(yuser *user, int y1, int x1, int y2, int x2, ychar c);
void clreol_term(yuser *user);
void clreos_term(yuser *user);
void scroll_term(yuser *user);
void rev_scroll_term(yuser *user);
void flush_term(yuser *user);
void rub_term(yuser *user);
void word_term(yuser *user);
void kill_term(yuser *user);
void tab_term(yuser *user);
void lf_term(yuser *user);
void newline_term(yuser *user);
void add_line_term(yuser *user, int num);
void del_line_term(yuser *user, int num);
void add_char_term(yuser *user, int num);
void del_char_term(yuser *user, int num);
void redraw_term(yuser *user, int y);
void resize_win(yuser *user, int height, int width);
void set_win_region(yuser *user, int height, int width);
void end_win_region(yuser *user);
void set_scroll_region(yuser *user, int top, int bottom);
void msg_term(yuser *user, char *str);
void spew_term(yuser *user, int fd, int rows, int cols);
void raw_term(yuser *user, int y, int x, ychar *str, int len);
int center(int width, int n);
void redraw_all_terms(void);
void set_raw_term(void);
void set_cooked_term(void);

#endif // YTALK_TERM_H_
