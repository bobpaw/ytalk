bin_PROGRAMS = ytalk
check_PROGRAMS = ptyalloc

ytalk_SOURCES = \
	main.c \
	term.c \
	user.c \
	fd.c \
	comm.c \
	menu.c \
	socket.c \
	rc.c \
	exec.c \
	getpty.c \
	cwin.c \
	mem.c \
	vt100.c \
	gtalk.c \
	err.c \
	cmdline-ytalk.c

ptyalloc_SOURCES = ptyalloc.c getpty.c
ptyalloc_CPPFLAGS = -DYTALK_TEST

AM_CFLAGS = -Wall -pedantic

AM_CPPFLAGS =

noinst_HEADERS = cmdline-ytalk.h comm.h cwin.h err.h exec.h fd.h getpty.h gtalk.h mem.h menu.h rc.h socket.h term.h user.h vt100.h ytypes.h
