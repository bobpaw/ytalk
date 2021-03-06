/*
 * src/cwin.c
 * curses interface
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

#include "cwin.h"

static ywin *head;		/* head of linked list */

/*
 * Take input from the user.
 */
static void curses_input(int fd) {
	int rc;
	static ychar buf[MAXBUF];

	if ((rc = read(fd, buf, MAXBUF)) <= 0) {
		if (rc == 0)
			bail(YTE_SUCCESS);
		bail(YTE_ERROR);
	}
	my_input(me, buf, rc);
}

static ywin * new_ywin(yuser * user, char * title) {
	ywin *out;
	int len;

	len = strlen(title);
	out = (ywin *) get_mem(sizeof(ywin) + len + 1);
	memset(out, 0, sizeof(ywin));
	out->user = user;
	out->title = ((char *) out) + sizeof(ywin);
	strcpy(out->title, title);
	return out;
}

static void make_win(ywin * w, int height, int width, int row, int col) {
	if ((w->win = newwin(height, width, row, col)) == NULL) {
		w = (ywin *) (me->term);
		if (w->win != NULL)
			show_error("make_win: newwin() failed");
		bail(YTE_ERROR);
	}
	w->height = height;
	w->width = width;
	w->row = row;
	w->col = col;
	scrollok(w->win, FALSE);
	wmove(w->win, 0, 0);
}

static void draw_title(ywin * w) {
	int pad, x;
	char *t;

	if ((int) strlen(w->title) > w->width) {
		for (x = 0; x < w->width; x++)
			addch('-');
		return;
	}
	pad = (w->width - strlen(w->title)) / 2;
	move(w->row - 1, w->col);
	x = 0;
	for (; x < pad - 2; x++)
		addch('-');
	if (pad >= 2) {
		addch('=');
		addch(' ');
		x += 2;
	}
	for (t = w->title; *t && x < w->width; x++, t++)
		addch(*t);
	if (pad >= 2) {
		addch(' ');
		addch('=');
		x += 2;
	}
	for (; x < w->width; x++)
		addch('-');
}

/*
 * Return number of lines per window, given "wins" windows.
 */
static int win_size(int wins) {
	if (wins == 0)
		return 0;
	return (LINES - 1) / wins;
}

/*
 * Break down and redraw all user windows.
 */
static void curses_redraw(void) {
	ywin *w;
	int row, wins, wsize;

	/* kill old windows */

	wins = 0;
	for (w = head; w; w = w->next) {
		if (w->win) {
			delwin(w->win);
			w->win = NULL;
		}
		wins++;
	}
	if ((wsize = win_size(wins)) < 3) {
		end_term();
		errno = 0;
		show_error("curses_redraw: window size too small");
		bail(YTE_ERROR);
	}

	/* make new windows */

	clear();
	refresh();
	row = 0;
	for (w = head; w; w = w->next) {
		if (w->next) {
			make_win(w, wsize - 1, COLS, row + 1, 0);
			resize_win(w->user, wsize - 1, COLS);
		} else {
			make_win(w, LINES - row - 2, COLS, row + 1, 0);
			resize_win(w->user, LINES - row - 2, COLS);
		}
		draw_title(w);
		row += wsize;
		refresh();
		wrefresh(w->win);
	}
}

/*
 * Start curses and set all options.
 */
static void curses_start(void) {
	char *term;
	if (initscr() == NULL) {
		term = getenv("TERM");
		fprintf(stderr, "Error opening terminal: %s.\n", (term ? term : "(null)"));
		bail(YTE_INIT);
	}
	noraw();
	crmode();
	noecho();
	clear();
}

/*
 * Restart curses... window size has changed.
 */
static RETSIGTYPE curses_restart(void) {
	ywin *w;

	/* kill old windows */

	for (w = head; w; w = w->next)
		if (w->win) {
			delwin(w->win);
			w->win = NULL;
		}

	/* restart curses */

	endwin();
	curses_start();

	/* fix for ncurses from Max <Marc.Espie@liafa.jussieu.fr> */

	refresh();
	curses_redraw();

#ifdef SIGWINCH
	/* some systems require we do this again */
	signal(SIGWINCH, (void (*)())curses_restart);
#endif
}

/* ---- global functions ---- */

void init_curses(void) {
	curses_start();
	refresh();
	head = NULL;
	add_fd(0, curses_input);/* set up user's input function */

#ifdef SIGWINCH
	/* set up SIGWINCH signal handler */
	signal(SIGWINCH, (void (*)())curses_restart);
#endif
}

void end_curses(void) {
	move(LINES - 1, 0);
	clrtoeol();
	refresh();
	endwin();
}

/*
 * Open a new window.
 */
int open_curses(yuser * user, char * title) {
	ywin *w;
	int wins;

	/*
	 * count the open windows.  We want to ensure at least three lines
	 * per window.
	 */
	wins = 0;
	for (w = head; w; w = w->next)
		wins++;
	if (win_size(wins + 1) < 3)
		return -1;

	/* add the new user */

	if (head == NULL)
		w = head = new_ywin(user, title);
	else
		for (w = head; w; w = w->next)
			if (w->next == NULL) {
				w->next = new_ywin(user, title);
				w = w->next;
				break;
			}
	user->term = w;

	/* redraw */
	curses_redraw();

	return 0;
}

/*
 * Close a window.
 */
void close_curses(yuser * user) {
	ywin *w, *p;

	/* zap the old user */

	w = (ywin *) (user->term);
	if (w == head)
		head = w->next;
	else {
		for (p = head; p; p = p->next)
			if (w == p->next) {
				p->next = w->next;
				break;
			}
		if (p == NULL) {
			show_error("close_curses: user not found");
			return;
		}
	}
	delwin(w->win);
	free_mem(w);
	w = NULL;
	user->term = NULL;
	curses_redraw();
}

void addch_curses(yuser * user, ychar c) {
	ywin *w;
	int x, y;

	w = (ywin *) (user->term);
	getyx(w->win, y, x);
	waddch(w->win, c);
	if (x >= COLS - 1)
		wmove(w->win, y, x);
}

void move_curses(yuser * user, int y, int x) {
	ywin *w;

	w = (ywin *) (user->term);
	wmove(w->win, y, x);
}

void clreol_curses(yuser * user) {
	ywin *w;

	w = (ywin *) (user->term);
	wclrtoeol(w->win);
}

void clreos_curses(yuser * user) {
	ywin *w;

	w = (ywin *) (user->term);
	wclrtobot(w->win);
}

void scroll_curses(yuser * user) {
	ywin *w;

	/*
	 * Curses uses busted scrolling.  In order to call scroll()
	 * effectively, scrollok() must be TRUE.  However, if scrollok() is
	 * TRUE, then placing a character in the lower right corner will
	 * cause an auto-scroll.  *sigh*
	 */
	w = (ywin *) (user->term);
	scrollok(w->win, TRUE);
	scroll(w->win);
	scrollok(w->win, FALSE);

	/*
	 * Some curses won't leave the cursor in the same place, and some
	 * curses programs won't erase the bottom line properly.
	 */
	wmove(w->win, user->t_rows - 1, 0);
	wclrtoeol(w->win);
	wmove(w->win, user->y, user->x);
}

void keypad_curses(bool bf) {
#ifdef HAVE_KEYPAD
	keypad(((ywin *) (me->term))->win, bf);
#endif
}

void flush_curses(yuser * user) {
	ywin *w;

	w = (ywin *) (user->term);
	wrefresh(w->win);
}

/*
 * Clear and redisplay.
 */
void redisplay_curses(void) {
	ywin *w;

	clear();
	refresh();
	for (w = head; w; w = w->next) {
		redraw_term(w->user, 0);
		draw_title(w);
		refresh();
		wrefresh(w->win);
	}
}

/*
 * Set raw mode.
 */
void set_raw_curses(void) {
	raw();
}

/*
 * Set cooked mode.
 */
void set_cooked_curses(void) {
	noraw();
	crmode();
	noecho();
}
