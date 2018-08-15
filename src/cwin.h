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
#ifdef HAVE_NCURSES
#include <ncurses.h>
#else
#include <curses.h>
#endif
extern void init_curses();
extern void end_curses();
extern int open_curses( /* yuser, title */ );
extern void close_curses( /* yuser */ );
extern void addch_curses(yuser *user, register ychar c);
extern void move_curses( /* yuser, y, x */ );
extern void clreol_curses( /* yuser */ );
extern void clreos_curses( /* yuser */ );
extern void scroll_curses( /* yuser */ );
extern void keypad_curses(bool bf);
extern void flush_curses( /* yuser */ );
extern void redisplay_curses();
extern void set_raw_curses();
extern void set_cooked_curses();

#ifndef getyx
# define getyx(w,y,x)	y = w->_cury, x = w->_curx
#endif
