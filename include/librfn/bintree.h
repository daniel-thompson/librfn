/*
 * bintree.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2021 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_BINTREE_H_
#define RF_BINTREE_H_

#include <stdbool.h>
#include <stdio.h>

/*!
 * \defgroup librfn_bintree Binary tree
 *
 * \brief A binary tree implementation.
 *
 * No form of internal locking or other thread-safety is provided.
 *
 * \note Fibres on uniprocessor systems do not require thread-safety support
 *       and may freely use the list.
 *
 * @{
 */

typedef struct bintree_node {
	struct bintree_node *left;
	struct bintree_node *right;
} bintree_node_t;

#define BINTREE_NODE_VAR_INIT { 0 }

typedef void(bintree_free_t)(bintree_node_t *);
typedef char *(bintree_labeller_t)(bintree_node_t *);
typedef bool(bintree_is_list_t)(bintree_node_t *);
typedef void(bintree_list_visitor_t)(void *, bintree_node_t *);
typedef void(bintree_visitor_t)(void *, bintree_node_t *, bintree_node_t *,
				int);

static inline bool bintree_is_leaf(bintree_node_t *tree)
{
	return !tree->left && !tree->right;
}

void bintree_traverse_in_order(bintree_node_t *tree,
			       bintree_visitor_t *visitor, void *ctx);

/*!
 * \brief Traverse a sub-tree that is acts like a list.
 *
 * Any node that where is_list() is true will be traversed and other nodes
 * are handed to the visitor. This works for tree regardless of their
 * balance. For example the following trees are both traversed 1, 2, 3, 4.
 *
 * ~~~
 *            L                 L
 *           / \               / \
 *          L   4             1   L
 *         / \                   / \
 *        L   3                 2   L
 *       / \                       / \
 *      1   2                     3   4
 * ~~~
 */
void bintree_traverse_list(bintree_node_t *tree, bintree_is_list_t *is_list,
			   bintree_list_visitor_t *visitor, void *ctx);
void bintree_traverse_post_order(bintree_node_t *tree,
				 bintree_visitor_t *visitor, void *ctx);
void bintree_traverse_pre_order(bintree_node_t *tree,
				bintree_visitor_t *visitor, void *ctx);
void bintree_free(bintree_node_t *tree, bintree_free_t *dealloc);
void bintree_visualize(bintree_node_t *tree, FILE *f,
		       bintree_labeller_t *label);
void bintree_graphviz(bintree_node_t *tree, FILE *f,
		      bintree_labeller_t *labeller);

/*! @} */
#endif // RF_BINTREE_H_
