/*
 * src/mem.c
 * memory function wrappers and leak tracking
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

#include "mem.h"

#ifdef YTALK_DEBUG
/* some statistical accumulators */
unsigned int bad_free = 0;
unsigned int bad_realloc = 0;
unsigned int realloc_null = 0;
unsigned int leaked = 0;

static mem_list *glist = NULL;

/*
 * Add to linked list
 */
mem_list * add_area(mem_list *list, yaddr addr, size_t size, int line, char *file) {
  mem_list *entry;
  if (addr == 0)
	return list;
  entry = (mem_list *) malloc(sizeof(mem_list));
  entry->addr = addr;
  entry->size = size;
  entry->line = line;
  entry->file = file;
  entry->prev = NULL;
  if (list != NULL)
	list->prev = entry;
  entry->next = list;
  return entry;
}

mem_list * find_area(mem_list *list, yaddr addr) {
  mem_list *it = list;
  while (it != NULL) {
	if (it->addr == addr)
	  break;
	it = it->next;
  }
  return it;
}

/*
 * Delete from list
 */
mem_list * del_area(mem_list *list, mem_list *entry) {
  if (list == entry)
	list = entry->next;

  if (entry->next == NULL && entry->prev == NULL) {
	free(entry);
	return NULL;
  }
  if (entry->prev != NULL)
	entry->prev->next = entry->next;
  else
	entry->next->prev = NULL;

  if (entry->next != NULL)
	entry->next->prev = entry->prev;
  else
	entry->prev->next = NULL;

  free(entry);

  return list;
}

/*
 * Change stored size
 */
void change_area(mem_list *list, yaddr addr, yaddr new_addr, size_t size) {
  mem_list *it = list;
  while (it != NULL) {
	if (it->addr == addr) {
	  it->addr = new_addr;
	  it->size = size;
	  return;
	}
	it = it->next;
  }
  show_error("Reallocation failed: Not in allocation list");
  bad_realloc++;
}
#endif				/* YTALK_DEBUG */

/*
 * Allocate memory
 */
#ifdef YTALK_DEBUG
yaddr real_get_mem(size_t n, int line, char *file)
#else
  yaddr get_mem(size_t n)
#endif
{
  yaddr out;
  if ((out = (yaddr) malloc((size_t) n)) == NULL) {
	show_error("malloc() failed");
	bail(YTE_NO_MEM);
  }
#ifdef YTALK_DEBUG
  glist = add_area(glist, out, n, line, file);
#endif
  return out;
}

/*
 * Free and clear memory
 */
#ifndef YTALK_DEBUG
void free_mem(yaddr addr) {
  free(addr);
}
#else
void real_free_mem(yaddr addr, int line, char *file) {
  mem_list *entry;
  if ((entry = find_area(glist, addr)) != NULL) {
	memset(entry->addr, '\0', (size_t) entry->size);
	free(entry->addr);
	glist = del_area(glist, entry);
  } else {
#ifdef HAVE_SNPRINTF
	snprintf(errstr, MAXERR, "Free failed: Not in allocation list: 0x%lx (%s:%d)", (long unsigned) addr, file, line);
#else
	sprintf(errstr, "Free failed: Not in allocation list: 0x%lx (%s:%d)", (long unsigned) addr, file, line);
#endif
	show_error(errstr);
	bad_free++;
  }
}
#endif				/* YTALK_DEBUG */

/*
 * Reallocate memory
 */
#ifdef YTALK_DEBUG
yaddr real_realloc_mem(yaddr p, size_t n, int line, char *file)
#else
  yaddr realloc_mem(yaddr p, size_t n)
#endif
{
  yaddr out;
  if (p == NULL) {
#ifdef YTALK_DEBUG
	realloc_null++;
	return real_get_mem(n, line, file);
#else
	return get_mem(n);
#endif
  }
  if ((out = (yaddr) realloc(p, (size_t) n)) == NULL) {
	show_error("realloc() failed");
	bail(YTE_NO_MEM);
  }
#ifdef YTALK_DEBUG
  change_area(glist, p, out, n);
#endif
  return out;
}

#ifdef YTALK_DEBUG
/*
 * Clear all memory
 */
void clear_all(void) {
  mem_list *it;
  fprintf(stderr, "Clearing memory\n");
  while (glist != NULL) {
	fprintf(stderr, "0x%lx: %d\t(%s:%d)\n", (long unsigned) glist->addr, (int) glist->size,
			(char *) glist->file, (int) glist->line);
	/* The next 5 rows are a simpler and faster version of free_mem() */
	it = glist;
	glist = glist->next;
	memset(it->addr, '\0', (size_t) it->size);
	free(it->addr);
	free(it);
	leaked++;
  }
  fprintf(stderr, "Statistics:\n");
  fprintf(stderr, "Bad free_mem() calls:    %u\n", bad_free);
  fprintf(stderr, "Bad realloc_mem() calls: %u\n", bad_realloc);
  fprintf(stderr, "realloc_mem(NULL) calls: %u\n", realloc_null);
  fprintf(stderr, "Leaked allocations:      %u\n", leaked);
}
#endif				/* YTALK_DEBUG */

/*
 * Copy a string.
 */
char * str_copy(char *str) {
  char *out;
  size_t len;

  if (str == NULL)
	return NULL;
  len = strlen(str) + 1;
  out = get_mem(len);
  memcpy(out, str, len);
  return out;
}
