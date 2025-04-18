#ifndef S_MIN_HEAP_H
#define S_MIN_HEAP_H
#include "basic.h"

typedef struct {
	int size;
	void* heap;
	int step_size;
	int (*cmp)(void*, void*);
} binHeap;

void heapInit(binHeap* H, void* space,
				int size, int step_size, int (*cmp)(void*, void*));

//用于把已有数组重排成小顶堆，数组从index=1开始存储
void heapCorrect(binHeap* h);

#define heapPop(h) ({ char* arg=malloc((h)->step_size);  \
						heapPopIn(h,&arg);\
						arg;})

void* heapPopIn(binHeap* h,void*);

#define heapPush(h,value) ({typeof(value) __temp=(value); char arg[(h)->step_size];\
								memcpy(arg,&__temp,(h)->step_size);\
								heapPushFrom(h,&arg);})

void heapPushFrom(binHeap* h, void* x);

#endif