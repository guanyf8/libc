#include "AVLTree.h"


//实现一棵bst,bst一定会有key

typedef struct bstnode {
    avlNode avlnode;
    void* key;
    void *value;
} bstNode;

typedef struct {
    avlTree* root;
    int (*cmp)(void* key1,void* key2);
    int size;
} Bst;

typedef bstNode avlmapNode;
typedef Bst avlMap;

int numcmp(void* a,void* b);

int mystrcmp(void* a,void* b);

Bst* bstInit(Bst *m, int (*cmp)(void *a, void *b));

// 插入键值对
int bstInsert(Bst *treemap, struct bstnode *node) ;

// 查找节点
void *bstSearchValue(Bst *treemap, void *key);
bstNode *bstSearch(Bst* treemap, void* key);

void bstErase(Bst* treemap, void* key);

void bstFree(Bst* treemap);

void* bstTraverse(Bst* treemap);