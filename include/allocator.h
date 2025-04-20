#ifndef ALLOC_H
#define ALLOC_H


#include "stack.h"
#include "basic.h"
#include "list.h"

#define MAX_NODE 1024
#define BATCH_NODE 128
#define allocInit(type) allocatorInit(sizeof(type))

typedef struct memUnit{
    void* space;
    struct list_node node;
} memUnit;

//节点地址分配器
typedef struct allocator {
    Stack* q;
    struct list_node mem_list;
    int unit_size;
} allocator;


allocator* allocatorInit(int size);

void* nodeAlloc(allocator* aq);

void nodeFree(allocator* aq,void* node);

void allocatorFree(allocator* aq);

#endif
