/*
 * src/mem.h
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

#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
	#include <string.h>
#endif

#include "ytypes.h"
#include "err.h"

typedef void *yaddr;

#ifndef YTALK_MEM_H_
#define YTALK_MEM_H_

#ifdef YTALK_DEBUG
typedef struct __mem_list {
	yaddr addr;
	size_t size;
	char *file;
	int line;
	struct __mem_list *prev;
	struct __mem_list *next;
} mem_list;

#define get_mem(size) (real_get_mem(size, __LINE__, __FILE__))
#define realloc_mem(addr, size) (real_realloc_mem(addr, size, __LINE__, __FILE__))
#define free_mem(addr) (real_free_mem(addr, __LINE__, __FILE__))

yaddr     real_get_mem     (size_t n, int line, char *file);
yaddr     real_realloc_mem (yaddr p, size_t n, int line, char *file);
void      real_free_mem    (yaddr addr, int line, char *file);
void      change_area      (mem_list *list, yaddr addr, yaddr new_addr, size_t size);
mem_list *find_area        (mem_list *list, yaddr addr);
void      clear_all        (void);
mem_list *add_area         (mem_list *list, yaddr addr, size_t size, int line, char *file);
mem_list *del_area         (mem_list *list, mem_list *entry);
#else
yaddr     get_mem(size_t n);
yaddr     realloc_mem(yaddr p, size_t n);
void      free_mem(yaddr addr);
#endif // YTALK_DEBUG
char *    str_copy (char *str);

#endif // YTALK_MEM_H_
