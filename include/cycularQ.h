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

Queue *QueueInit(Queue *,int cap);

void QueueClose(Queue *que);

int QueueGetF(Queue *q);

void QueueResize(Queue *q,int new_cap);

void* QueuePop(Queue *q);

int QueueGetR(Queue *q);

void* QueuePush(Queue *q,void* val);

#endif


