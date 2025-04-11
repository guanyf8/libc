
#include "allocator.h"

allocator* allocatorInit(int size){
    allocator *alloc = (allocator*)malloc(sizeof(allocator));
    alloc->q=QueueInit(new(Queue),BATCH_NODE);
    alloc->unit_size=size;

    memUnit* node=new(memUnit);
    node->space=malloc(size*BATCH_NODE);
    node->node.next=NULL;
    alloc->mem_list=&node->node;

    void* temp=node->space;
    for(;temp<node->space+BATCH_NODE;temp+=size){
        QueuePush(alloc->q,(void*)temp);
    }
}

void resize(allocator* alloc){
    memUnit* node=new(memUnit);
    node->space=malloc(alloc->unit_size*BATCH_NODE);
    list_add(&node->node,alloc->mem_list);
    QueueResize(alloc->q,alloc->q->cap+BATCH_NODE);

    void* temp=node->space;
    for(;temp<node->space+BATCH_NODE;temp+=alloc->unit_size){
        QueuePush(alloc->q,(void*)temp);
    }
}

void* nodeAlloc(allocator* alloc){
    void * ret=QueuePop(alloc->q);
    if(ret==NULL){
        resize(alloc);
        ret=QueuePop(alloc->q);
    }
    return ret;
}

void nodeFree(allocator* alloc,void* node){
    QueuePush(alloc->q,node);
}

void allocatorFree(allocator* alloc){
    QueueClose(alloc->q);
    memUnit* node,*tnode;
    list_for_each_entry_safe(node,tnode,alloc->mem_list,node){
        free(node->space);
        free(node);
    }
    free(alloc);
}