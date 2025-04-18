#ifndef LCRS_TREE_H
#define LCRS_TREE_H

#include "bintree.h"
#include "stdlib.h"

typedef struct treeNode {
	int depth;
	int data;
	struct treeNode* first_child;
	struct treeNode* next;
} treeNode;

typedef struct tree {
	treeNode* root;
} tree;

#endif