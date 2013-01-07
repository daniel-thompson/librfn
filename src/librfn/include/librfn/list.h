/*
 * list.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_LIST_H_
#define RF_LIST_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct list_node {
	struct list_node *next;
} list_node_t;

typedef struct {
	list_node_t *head;
	list_node_t *tail;
} list_t;

typedef struct {
	list_node_t **prevnext;
	list_t *list;
} list_iterator_t;

typedef int list_node_compare_t(list_node_t *, list_node_t *);

/*
 * insertion operations
 */

void list_insert(list_t *list, list_node_t *node);
void list_insert_sorted(list_t *list, list_node_t *node,
		list_node_compare_t *nodecmp);
void list_push(list_t *list, list_node_t *node);

/*
 * extraction operations
 */

list_node_t * list_extract(list_t *list);

/*
 * list traversal
 */

list_node_t * list_iterate(list_t *list, list_iterator_t *iter);
list_node_t * list_iterator_next(list_iterator_t *iter);
void list_iterator_insert(list_iterator_t *iter, list_node_t *node);
list_node_t * list_iterator_remove(list_iterator_t *iter);

/*
 * search operations
 */

bool list_contains(list_t *list, list_node_t *node, list_iterator_t *iter);
bool list_remove(list_t *list, list_node_t *node);


#endif // RF_LIST_H_
