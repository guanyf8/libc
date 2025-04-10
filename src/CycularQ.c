

#include "CycularQ.h"


Queue* QueueInit(Queue* q,int cap) {
    q->f=0;
    q->r=0;
    q->size=0;
    q->space=(void**) malloc(sizeof(void*)*cap);
    q->cap=cap;
    return q;
}

void QueueClose(Queue* que){
    free(que->space);
    free(que);
}


//when pop element, getfront to get the value then pop
int QueueGetF(Queue* q){
    return q->f;
}

void* QueuePop(Queue* q) {
    if(q->f==q->r&&q->size==0){
        return NULL;
    }
    void* val=q->space[q->f];
    q->f= ++q->f % q->cap;
    q->size--;
    return val;
}

//when push, assign value with getback then push
int QueueGetR(Queue* q){
	return q->r;
}

void* QueuePush(Queue* q, void* val) {
    if (q->size==q->cap) {
        return NULL; // 队列已满
    }
    q->space[q->r]=val;
    q->r= ++q->r % q->cap;
    q->size++;
    return val;
}




