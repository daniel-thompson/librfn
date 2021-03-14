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

static void free_node(void *ctx, bintree_node_t *node, bintree_node_t *parent,
		      int depth)
{
	bintree_free_t *dealloc = ctx;
	if (node)
		dealloc(node);
}

void bintree_free(bintree_node_t *tree, bintree_free_t *dealloc)
{
	bintree_traverse_post_order(tree, free_node, dealloc);
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
