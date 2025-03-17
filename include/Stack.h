#ifndef STACK_H
#define STACK_H

//顺序栈的结构与基本操作

#define n 100

typedef struct {
	void** space;
	int b;
	int t;
} ArrStack;    //只存数字，即只存索引，需要协同element数组

ArrStack *stackInit(int size);

void *stackPop(ArrStack *st);

void *stackPush(ArrStack *stack, void *x);

#endif


