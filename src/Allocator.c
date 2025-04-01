
#include "Allocator.h"


Allocator* AllocatorInit(int size){
    Allocator *aq = (Allocator*)malloc(sizeof(Allocator));
    aq->q=QueueInit(new(Queue),BATCH_NODE);
    aq->node_space=(Allocator*)malloc(sizeof(size)*BATCH_NODE);
    char* node=(char*)aq->node_space;
    for(;node<node+BATCH_NODE;node+=size){
        QueuePush(aq->q,(void*)node);
    }
}

void* NodeAlloc(Allocator* aq){
    return QueuePop(aq->q);
}

void NodeFree(Allocator* aq,void* node){
    QueuePush(aq->q,node);
}

void AllocatorFree(Allocator* aq){
    QueueClose(aq->q);
    free(aq->node_space);
}