#ifndef S_MIN_HEAP_H
#define S_MIN_HEAP_H

typedef struct {
	int size;
	int** heap;
} minHeap;

void heapInit(minHeap* H, int** space, int size);

//用于把已有数组重排成小顶堆，数组从index=1开始存储
void heapCorrect(minHeap* h);

int* heapPop(minHeap* h);

void heapPush(minHeap* h, int* elem);

#endif