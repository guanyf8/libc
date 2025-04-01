#ifndef AVL_H
#define AVL_H

typedef struct avlNode {
    int height;      // 以此为根的树高
    struct avlNode* left;
    struct avlNode* right;
    struct avlNode* parent;
} avlNode;


//只负责维持平衡，有关avl的部分需要自己实现

typedef struct avlTree {
    avlNode* root;
} avlTree;


avlNode *avlLinkNode(avlNode *node, avlNode *parent, avlNode **link);

void avlInsert(avlTree *tree, avlNode *node);

void avlErase(avlTree *tree, avlNode *node);

avlTree *avlInit(avlTree *tree);


#endif
