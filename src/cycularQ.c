

#include "cycularQ.h"


Queue* QueueInitStruct(Queue* q,int cap,int step_size) {
    q->f=0;
    q->r=0;
    q->size=0;
    q->step_size=step_size;
    q->space=malloc(step_size*cap);
    q->cap=cap;
    return q;
}

void QueueClose(Queue* que){
    free(que->space);
    free(que);
}

void* QueuePopIn(Queue* q,void* val) {
    if(q->f==q->r&&q->size==0){
        memset(val,0,q->step_size);
        return NULL;
    }
    unsigned int step=q->step_size;
    //等效为*val=q->space[q->f]
    memcpy(val,(q->space+q->f*step),step);
    q->f= ++q->f % q->cap;
    q->size--;
    return val;
}

void* QueuePushFrom(Queue* q, void* src) {
    if (q->size==q->cap) {
        return NULL; // 队列已满
    }
    int step=q->step_size;
    // 等效为q->space[q->r]=*src;
    memcpy((q->space+q->r*step),src,step);
    
    q->r= ++q->r % q->cap;
    q->size++;
    return src;
}

//when pop element, getfront to get the value then pop
int QueueGetF(Queue* q){
    return q->f;
}

//when push, assign value with getback then push
int QueueGetR(Queue* q){
	return q->r;
}

void QueueResize(Queue* q, int new_cap){
    q->cap=new_cap;
    q->space=(void*) realloc(q->space,q->step_size*new_cap);
}



