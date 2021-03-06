/*
 * src/socket.h
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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <stdio.h>

#ifdef HAVE_STRING_H
	#include <string.h>
#endif

#ifdef HAVE_NETDB_H
	#include <netdb.h>
#endif

#include <errno.h>

#ifdef TIME_WITH_SYS_TIME
	#include <sys/time.h>
	#include <time.h>
#else
	#ifdef HAVE_SYS_TIME
		#include <sys/time.h>
	#else
		#include <time.h>
	#endif
#endif

#ifdef HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#else
ylong inet_addr();
char *inet_ntoa();
#endif

#ifdef _AIX
	#include <sys/select.h>
#endif

#include "ytypes.h"
#include "mem.h"

/* ---- talk daemon information structure ---- */

#ifndef YTALK_SOCKET_H_
#define YTALK_SOCKET_H_

#define MAXDAEMON	5

#ifndef IN_ADDR
	#define IN_ADDR(s)	((s).sin_addr.s_addr)
#endif

#ifndef IN_PORT
	#define IN_PORT(s)	((s).sin_port)
#endif

#ifndef SOCK_EQUAL
	#define SOCK_EQUAL(s,c)	(IN_PORT(s) == IN_PORT(c) && IN_ADDR(s) == IN_ADDR(c))
#endif

struct _talkd {
	struct sockaddr_in sock;/* socket */
	int fd;			/* socket file descriptor */
	short port;		/* port number */
	yaddr mptr;		/* message pointer */
	size_t mlen;		/* message length */
	yaddr rptr;		/* response pointer */
	size_t rlen;		/* response length */
};

typedef struct _hostinfo {
	struct _hostinfo *next;	/* next in linked list */
	ylong host_addr;	/* host address */
	int dtype;		/* active daemon types bitmask */
} hostinfo;

typedef struct _readdr {
	struct _readdr *next;	/* next in linked list */
	ylong from_addr;	/* does this apply to me? */
	ylong from_mask;
	ylong addr;		/* this net address [group?], */
	ylong mask;		/* with this mask, */
	ylong id_addr;		/* thinks I'm at this net address, */
	ylong id_mask;		/* with this mask. */
} readdr;

extern struct _talkd talkd[MAXDAEMON + 1];
extern int daemons;

/* ---- talk daemon I/O structures ---- */

#define NAME_SIZE 9
#define TTY_SIZE 16

typedef struct bsd42_sockaddr_in {
	u_short sin_family;
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
} BSD42_SOCK;

/*
 * Control Message structure for earlier than BSD4.2
 */
typedef struct {
	char type;
	char l_name[NAME_SIZE];
	char r_name[NAME_SIZE];
	char filler;
	ylong id_num;
	ylong pid;
	char r_tty[TTY_SIZE];
	BSD42_SOCK addr;
	BSD42_SOCK ctl_addr;
} CTL_MSG;

/*
 * Control Response structure for earlier than BSD4.2
 */
typedef struct {
	char type;
	char answer;
	u_short filler;
	ylong id_num;
	BSD42_SOCK addr;
} CTL_RESPONSE;

#define NTALK_NAME_SIZE 12

/*
 * Control Message structure for BSD4.2
 */
typedef struct {
	u_char vers;
	char type;
	u_short filler;
	ylong id_num;
	BSD42_SOCK addr;
	BSD42_SOCK ctl_addr;
	ylong pid;
	char l_name[NTALK_NAME_SIZE];
	char r_name[NTALK_NAME_SIZE];
	char r_tty[TTY_SIZE];
} CTL_MSG42;

/*
 * Control Response structure for BSD4.2
 */
typedef struct {
	u_char vers;
	char type;
	char answer;
	char filler;
	ylong id_num;
	BSD42_SOCK addr;
} CTL_RESPONSE42;

#define	TALK_VERSION	1	/* protocol version */

/*
 * Dgram Types.
 *
 * These are the "type" arguments to feed to send_dgram().  Each acts either on
 * the remote daemon or the local daemon, as marked.
 */

#define LEAVE_INVITE	0	/* leave an invitation (local) */
#define LOOK_UP		1	/* look up an invitation (remote) */
#define DELETE		2	/* delete erroneous invitation (remote) */
#define ANNOUNCE	3	/* ring a user (remote) */
#define DELETE_INVITE	4	/* delete my invitation (local) */
#define AUTO_LOOK_UP	5	/* look up auto-invitation (remote) */
#define AUTO_DELETE	6	/* delete erroneous auto-invitation (remote) */

void init_socket();	/* socket.c */
void close_all();	/* socket.c */
int send_dgram(yuser * user, u_char type);	/* socket.c */
int send_auto(u_char type);	/* socket.c */
void kill_auto();	/* socket.c */
int newsock( /* yuser */ );	/* socket.c */
int connect_to( /* yuser */ );	/* socket.c */
ylong get_host_addr( /* hostname */ );	/* socket.c */
char *host_name( /* addr */ );	/* socket.c */
void readdress_host( /* from, to, on */ );	/* socket.c */

#endif // YTALK_SOCKET_H_
