
#include "allocator.h"

allocator* allocatorInit(int size){
    allocator *alloc = (allocator*)malloc(sizeof(allocator));
    alloc->q=stackInit(new(Stack),BATCH_NODE);
    alloc->unit_size=size;
    INIT_LIST_HEAD(&alloc->mem_list);

    memUnit* node=new(memUnit);
    node->space=malloc(size*BATCH_NODE);
    list_add(&node->node,&alloc->mem_list);

    void* temp=node->space;
    for(;temp<node->space+BATCH_NODE;temp+=size){
        stackPush(alloc->q,temp);
    }
    return alloc;
}

void resize(allocator* alloc){
    memUnit* node=new(memUnit);
    node->space=malloc(alloc->unit_size*BATCH_NODE);
    list_add(&node->node,&alloc->mem_list);

    stackResize(alloc->q,alloc->q->size+BATCH_NODE);

    void* temp=node->space;
    for(;temp<node->space+BATCH_NODE;temp+=alloc->unit_size){
        stackPush(alloc->q,temp);
    }
}

void* nodeAlloc(allocator* alloc){
    void * ret;
    stackPopIn(alloc->q,&ret);
    if(ret==NULL){
        resize(alloc);
        stackPopIn(alloc->q,&ret);
    }
    return ret;
}

void nodeFree(allocator* alloc,void* node){
    stackPush(alloc->q,node);
}

void allocatorFree(allocator* alloc){
    stackClose(alloc->q);
    memUnit* node,*tnode;
    list_for_each_entry_safe(node,tnode,&alloc->mem_list,node){
        free(node->space);
        free(node);
    }
    free(alloc);
}