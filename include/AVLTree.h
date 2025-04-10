#ifndef AVL_H
#define AVL_H
#include<stdio.h>
#include<stdlib.h>

typedef struct avl_node {
    int height;      // 以此为根的树高
    struct avl_node* left;
    struct avl_node* right;
    struct avl_node* parent;
} avl_node;


//只负责维持平衡，有关avl的部分需要自己实现

typedef struct avl_tree {
    avl_node* root;
} avl_tree;


avl_node *avlLinkNode(avl_node *node, avl_node *parent, avl_node **link);

void avlInsert(avl_tree *tree, avl_node *node);

void avlErase(avl_tree *tree, avl_node *node);

avl_tree *avlInit(avl_tree *tree);


#endif
