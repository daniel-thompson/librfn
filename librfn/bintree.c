/*
 * bintree.c
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

#include "librfn/bintree.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "librfn/util.h"

static void bintree_traverse_in_order_depth(bintree_node_t *tree,
					    bintree_node_t *parent,
					    bintree_visitor_t visitor,
					    void *ctx, int depth)
{
	if (tree) {
		bintree_traverse_in_order_depth(tree->left, tree, visitor, ctx,
						depth + 1);
		visitor(ctx, tree, parent, depth);
		bintree_traverse_in_order_depth(tree->right, tree, visitor, ctx,
						depth + 1);
	} else {
		visitor(ctx, tree, parent, depth);
	}
}

void bintree_traverse_in_order(bintree_node_t *tree, bintree_visitor_t visitor,
			       void *ctx)
{
	bintree_traverse_in_order_depth(tree, NULL, visitor, ctx, 0);
}

void bintree_traverse_list(bintree_node_t *tree, bintree_is_list_t *is_list,
			   bintree_list_visitor_t *visitor, void *ctx)
{
	if (!tree)
		return;

	if (is_list(tree)) {
		bintree_traverse_list(tree->left, is_list, visitor, ctx);
		bintree_traverse_list(tree->right, is_list, visitor, ctx);
	} else {
		visitor(ctx, tree);
	}
}

static void bintree_traverse_post_order_depth(bintree_node_t *tree,
					      bintree_node_t *parent,
					      bintree_visitor_t visitor,
					      void *ctx, int depth)
{
	if (tree) {
		bintree_traverse_post_order_depth(tree->left, tree, visitor,
						 ctx, depth + 1);
		bintree_traverse_post_order_depth(tree->right, tree, visitor,
						 ctx, depth + 1);
	}
	visitor(ctx, tree, parent, depth);
}

void bintree_traverse_post_order(bintree_node_t *tree,
				 bintree_visitor_t visitor, void *ctx)
{
	bintree_traverse_post_order_depth(tree, NULL, visitor, ctx, 0);
}

static void bintree_traverse_pre_order_depth(bintree_node_t *tree,
					     bintree_node_t *parent,
					     bintree_visitor_t visitor,
					     void *ctx, int depth)
{
	visitor(ctx, tree, parent, depth);
	if (tree) {
		bintree_traverse_pre_order_depth(tree->left, tree, visitor,
						 ctx, depth + 1);
		bintree_traverse_pre_order_depth(tree->right, tree, visitor,
						 ctx, depth + 1);
	}
}

void bintree_traverse_pre_order(bintree_node_t *tree, bintree_visitor_t visitor,
				void *ctx)
{
	bintree_traverse_pre_order_depth(tree, NULL, visitor, ctx, 0);
}

void bintree_free(bintree_node_t *tree, bintree_free_t *dealloc)
{
	bintree_iterator_t iter;

	for (bintree_node_t *n = bintree_iterate_post_order(&iter, tree); n;
	     n = bintree_next(&iter)) {
		dealloc(n);

		if (iter.parent) {
			if (iter.parent->right == n)
				iter.parent->right = NULL;
			else
				iter.parent->left = (bintree_node_t *) 1;
		}
	}
}

void bintree_free_left(bintree_node_t *tree, bintree_free_t *dealloc) {
	if (tree->left) {
		bintree_free(tree->left, dealloc);
		tree->left = NULL;
	}
}

void bintree_free_right(bintree_node_t *tree, bintree_free_t *dealloc) {
	if (tree->right) {
		bintree_free(tree->right, dealloc);
		tree->right = NULL;
	}
}

static bintree_node_t *in_order_iterator(bintree_iterator_t *iter)
{
	bintree_node_t *curr = iter->curr, *prev;

	while (curr) {
		/* Identify left mode nodes with a sub-tree */
		if (curr->left == NULL) {
			iter->curr = curr->right;
			return curr;
		}

		/* Search for the in-order predecessor */
		prev = curr->left;
		while (prev->right != NULL && prev->right != curr)
			prev = prev->right;

		/*
		 * Either:
		 *   Modify the tree so that curr is the right child of that predecessor
		 * Or:
		 *   Undo the modification!
		 */
		if (prev->right == NULL) {
			prev->right = curr;
			curr = curr->left;
		} else {
			prev->right = NULL;
			iter->curr = curr->right;
			return curr;
		}
	}

	iter->curr = NULL;
	return NULL;
}

bintree_node_t *bintree_iterate_in_order(bintree_iterator_t *iter,
					 bintree_node_t *tree)
{
	iter->next = in_order_iterator;
	iter->curr = tree;
	return in_order_iterator(iter);
}

static bintree_node_t *list_left_iterator(bintree_iterator_t *iter)
{
	bintree_node_t *curr = iter->curr;
	bintree_node_t *parent = iter->parent;

	/* already at end */
	if (!curr)
		return NULL;

	/* one from end */
	if (curr == parent) {
		iter->curr = NULL;
		return curr->right;
	}

	/* walk from the top until we reach the current element */
	while (parent->left != curr)
		parent = parent->left;

	/* step up and return whatever is on the right */
	iter->curr = parent;
	return curr->right;
}

static bintree_node_t *list_right_iterator(bintree_iterator_t *iter)
{
	bintree_node_t *result;

	if (!iter->curr || !iter->filter(iter->curr)) {
		result = iter->curr;
		iter->curr = NULL;
	} else {
		result = iter->curr->left;
		iter->curr = iter->curr->right;
	}

	return result;
}

bintree_node_t *bintree_iterate_list(bintree_iterator_t *iter,
				     bintree_node_t *tree,
				     bintree_is_list_t *is_list)
{
	iter->filter = is_list;

	if (is_list(tree) && tree->left && is_list(tree->left)) {
		iter->next = list_left_iterator;
		iter->parent = tree;

		do {
			tree = tree->left;
		} while (is_list(tree->left));

		iter->curr = tree;
		return tree->left;
	} else {
		iter->next = list_right_iterator;
		iter->filter = is_list;
		iter->curr = tree;
		return list_right_iterator(iter);
	}
}

static inline bool is_visited(bintree_node_t *node)
{
	return !((uintptr_t) node->left & 1);
}

static bintree_node_t *post_order_iterator(bintree_iterator_t *iter)
{
	bintree_node_t *tmp = iter->curr, *prev = NULL; /* load the head */

	while (tmp && is_visited(tmp) == false) {
		/* go left if we have not visited that subtree */
		bintree_node_t *left =
		    (bintree_node_t *)(((uintptr_t)tmp->left) & (uintptr_t)-2);
		if (left && is_visited(left) == false) {
			prev = tmp;
			tmp = left;
			continue;
		}

		/* go right if we have not visited that subtree */
		if (tmp->right && is_visited(tmp->right) == false) {
			prev = tmp;
			tmp = tmp->right;
			continue;
		}

		/* unmark this node */
		tmp->left =
		    (bintree_node_t *)(((uintptr_t)tmp->left) & (uintptr_t)-2);

		/*
		 * check if we have finished (we must avoid a use-after-free
		 * if the caller frees the node)
		 */
		if (tmp == iter->curr)
			iter->curr = NULL;

		/* visit the node */
		iter->parent = prev;
		return tmp;
	}

	return NULL;
}

/*
 * This is an O(n^2) algorithm but it does not require any memory allocation
 * *and* when we visit a node then that node and all it's children are
 * unmodified meaning is is both safe to free the subtree *and* safe to
 * further iterate within the sub-tree.
 */
bintree_node_t *bintree_iterate_post_order(bintree_iterator_t *iter,
					   bintree_node_t *tree)
{
	bintree_node_t *curr;

	/*
	 * in-order iteration means that by the time we visit a node then
	 * there is no longer any reason to visit the left pointer... which
	 * makes is safe to misalign it without breaking the iterator.
	 */
	for (curr = bintree_iterate_in_order(iter, tree); curr;
	     curr = bintree_next(iter)) {
		curr->left = (bintree_node_t *) (((uintptr_t) curr->left) | 1);
	}

	/*
	 * now we have all the left pointers mis-aligned (meaning not visited)
	 * we can adopt the iterator... which will restore the left pointers
	 * as it goes.
	 */
	iter->next = post_order_iterator;
	iter->curr = tree;
	return post_order_iterator(iter);
}

static bintree_node_t *pre_order_iterator(bintree_iterator_t *iter)
{
	bintree_node_t *curr = iter->curr, *prev;

	while (curr) {
		/* Identify left mode nodes with a sub-tree */
		if (curr->left == NULL) {
			iter->curr = curr->right;
			return curr;
		}

		/* Search for the in-order predecessor */
		prev = curr->left;
		while (prev->right && prev->right != curr)
			prev = prev->right;

		/*
		 * Either:
		 *   Modify the tree so that curr is the right child of that predecessor
		 * Or:
		 *   Undo the modification!
		 */
		if (prev->right == curr) {
			prev->right = NULL;
			curr = curr->right;
		} else {
			prev->right = curr;
			iter->curr = curr->left;
			return curr;
		}
	}

	iter->curr = NULL;
	return NULL;
}

bintree_node_t *bintree_iterate_pre_order(bintree_iterator_t *iter,
					   bintree_node_t *tree)
{
	iter->next = pre_order_iterator;
	iter->curr = tree;
	return pre_order_iterator(iter);
}

struct visualize {
	FILE *f;
	bintree_labeller_t *labeller;
};

static void visualize_node(void *ctx, bintree_node_t *node, bintree_node_t *parent,
		       int depth)
{
	FILE *f = ((struct visualize *)ctx)->f;
	bintree_labeller_t *labeller = ((struct visualize *)ctx)->labeller;

	for (int i = 0; i < depth; i++)
		fprintf(f, "\t");

	char *label = labeller(node);
	fprintf(f, "%s\n", label);
	free(label);
}

void bintree_visualize(bintree_node_t *tree, FILE *f,
		       bintree_labeller_t *labeller)
{
	struct visualize v = {f, labeller};

	bintree_traverse_pre_order(tree, visualize_node, &v);
}

static char *escape(char *s)
{
	char *si, *t, *ti;
	int len = 0;

	for (si = s; *si; si++)
		len += (*si == '\\' || *si == '"') ? 2 : 1;
	if (len == si-s)
		return s;
	len += 1;

	t = xmalloc(len);
	for (si = s, ti = t; *si; si++) {
		if (*si == '\\' || *si == '"')
			*ti++ = '\\';
		*ti++ = *si;
	}
	*ti = '\0';

	free(s);
	return t;
}

static void graph_node(void *ctx, bintree_node_t *node, bintree_node_t *parent,
		       int depth)
{
	FILE *f = ((struct visualize *)ctx)->f;
	bintree_labeller_t *labeller = ((struct visualize *)ctx)->labeller;

	if (!node)
		return;

	char *label = labeller(node);
	label = escape(label);
	fprintf(f, "    n%" PRIxPTR " [ label=\"%s\" ];\n", (uintptr_t)node,
		label);
	free(label);

	if (node->left)
		fprintf(f,
			"    n%" PRIxPTR " -- n%" PRIxPTR " [ label=\"l\" ];\n",
			(uintptr_t)node, (uintptr_t)(node->left));
	if (node->right)
		fprintf(f,
			"    n%" PRIxPTR " -- n%" PRIxPTR " [ label=\"r\" ];\n",
			(uintptr_t)node, (uintptr_t)(node->right));
}

void bintree_graphviz(bintree_node_t *tree, FILE *f,
		      bintree_labeller_t *labeller)
{
	struct visualize v = {f, labeller};

	fprintf(f, "graph {\n");
	fprintf(f, "    ordering=\"out\";\n");
	bintree_traverse_pre_order(tree, graph_node, &v);
	fprintf(f, "}\n");
}
