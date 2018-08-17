/*
 * src/cwin.h
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

#ifndef YTALK_CWIN_H_
#define YTALK_CWIN_H_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_NCURSES
	#include <ncurses.h>
#else
	#include <curses.h>
#endif

#include "err.h"
#include "mem.h"
#include "term.h"
#include "fd.h"

#include <signal.h>

typedef struct _ywin {
	struct _ywin *next;	/* next ywin in linked list */
	yuser *user;		/* user pointer */
	WINDOW *win;		/* window pointer */
	int height, width;	/* height and width in characters */
	int row, col;		/* row and column position on screen */
	char *title;		/* window title string */
} ywin;

void init_curses();
void end_curses();
int open_curses( /* yuser, title */ );
void close_curses( /* yuser */ );
void addch_curses(yuser *user, ychar c);
void move_curses( /* yuser, y, x */ );
void clreol_curses( /* yuser */ );
void clreos_curses( /* yuser */ );
void scroll_curses( /* yuser */ );
void keypad_curses(bool bf);
void flush_curses( /* yuser */ );
void redisplay_curses();
void set_raw_curses();
void set_cooked_curses();

#ifndef getyx
# define getyx(w,y,x)	(getcuryx(w, y, x))
#endif

#endif // YTALK_CWIN_H_
