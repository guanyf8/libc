#ifndef Q_H
#define Q_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define QUEUE_SIZE 512

typedef struct {
    int f;        // 原子队首索引
    int r;
    int size;
    int cap;
    int step_size;
    char* space;
} cirQueue;

typedef cirQueue Queue;

#define QueueInit(q,cap) QueueInitStruct(q,cap,sizeof(void*))

Queue *QueueInitStruct(Queue *,int cap,int step);

void QueueClose(Queue *que);

int QueueGetF(Queue *q);

void QueueResize(Queue *q,int new_cap);

//返回存放返回值的内存地址

#define QueuePop(q) ({ char* arg=malloc((q)->step_size);  \
                            QueuePopIn(q,arg);\
                            arg;})

void* QueuePopIn(Queue *q,void* val);

int QueueGetR(Queue *q);

#define QueuePush(q,val) ({ typeof(val) __temp=(val); char arg[(q)->step_size];\
                        memcpy(arg,&__temp,(q)->step_size);\
                        QueuePushFrom(q,&arg);})

//需要传入地址
void* QueuePushFrom(Queue *q,void* val);

#endif


