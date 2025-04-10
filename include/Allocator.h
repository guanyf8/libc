#ifndef ALLOC_H
#define ALLOC_H


#include "CycularQ.h"
#include "basic.h"

#define MAX_NODE 1024
#define BATCH_NODE 128
#define AllocInit(type) AllocatorInit(sizeof(type))
//节点地址分配器
typedef struct Allocator {
    Queue* q;
    void* node_space;
} Allocator;

Allocator* AllocatorInit(int size);

void* NodeAlloc(Allocator* aq);

void NodeFree(Allocator* aq,void* node);

void AllocatorFree(Allocator* aq);
#endif
