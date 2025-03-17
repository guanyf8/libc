#ifndef AVL_H
#define AVL_H

#include "CycularQ.h"

#define MAX_NODE 1024
#define BATCH_NODE 64

typedef struct bstNode {
    int height;      // 以此为根的树高
    struct bstNode* left;
    struct bstNode* right;
    struct bstNode* parent;
} bstNode;

//节点地址分配器
typedef struct adressQueue {
    Queue* q;
    bstNode* node_space;
} AdressQueue;

AdressQueue* AdressQueueInit(){
    AdressQueue *aq = (AdressQueue*)malloc(sizeof(AdressQueue));
    aq->q=QueueInit(BATCH_NODE);
    aq->node_space=(bstNode*)malloc(sizeof(bstNode)*BATCH_NODE);
    bstNode* node=aq->node_space;
    for(;node<node+BATCH_NODE;node++){
        QueuePush(aq->q,(void*)node);
    }
}

bstNode* NodeAlloc(AdressQueue* aq){
    bstNode* node=(bstNode*)QueuePop(aq->q);
    return node;
}

void NodeFree(AdressQueue* aq,bstNode* node){
    QueuePush(aq->q,node);
}

typedef struct bst {
    bstNode* root;
    AdressQueue* aq;
} bst;

//只负责维持平衡，有关bst的部分需要自己实现

bstNode *bstLinkNode(bst *root, bstNode *parent, bstNode **link);

void avlInsert(bst *tree, bstNode *node);

void avlErase(bst *tree, bstNode *node);

bst *bstInit();


#endif
