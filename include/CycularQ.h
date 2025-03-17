#ifndef Q_H
#define Q_H

#include "stdio.h"
#include "stdlib.h"

#define QUEUE_SIZE 512

typedef struct {
    int f;        // 原子队首索引
    int r;
    int size;
    int cap;
    void* *space;
} Queue;

Queue *QueueInit(int cap);

void QueueClose(Queue *que);

int QueueGetF(Queue *q);

void* QueuePop(Queue *q);

int QueueGetR(Queue *q);

int QueuePush(Queue *q,void* val);

#endif


