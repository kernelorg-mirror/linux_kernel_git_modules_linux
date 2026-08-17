/* SPDX-License-Identifier: GPL-2.0 */

#ifndef RB_TREE_LATCH_TYPES_H
#define RB_TREE_LATCH_TYPES_H

#include <linux/rbtree_types.h>
#include <linux/seqlock_types.h>

struct latch_tree_node {
	struct rb_node node[2];
};

struct latch_tree_root {
	seqcount_latch_t	seq;
	struct rb_root		tree[2];
};

#endif /* RB_TREE_LATCH_TYPES_H */
