#ifndef BINTREE_H
#define BINTREE_H


typedef struct bintree_node {
	int data;
	struct bintree_node* left;
	struct bintree_node* right;
} btNode;

typedef struct bintree {
	btNode* root;
} binTree;

#endif