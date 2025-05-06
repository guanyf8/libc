#include "avltree.h"
#include "basic.h"
#include "queue.h"
#include <string.h>
//实现一棵bst,bst一定会有key

typedef struct bstnode {
    avl_node avlnode;
    union key_type key;
    void *value;
} bst_node;

typedef struct {
    avl_tree* root;
    int (*cmp)(union key_type key1,union key_type key2);
    int size;
} bst;

typedef bst_node avlMapNode;
typedef bst avlMap;

bst* bstInit(bst *m, int (*cmp)(union key_type a, union key_type b));

// 插入键值对
int bstInsert(bst *treemap, struct bstnode *node);

// 查找节点
void *bstSearchValue(bst *treemap, union key_type key);
bst_node *bstSearch(bst* treemap, union key_type key);


void bstErase(bst* treemap, union key_type key);

void bstFree(bst* treemap);

void bstTraverse(bst* treemap);