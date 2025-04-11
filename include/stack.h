#ifndef STACK_H
#define STACK_H

#include "stdio.h"
#include "stdlib.h"

//顺序栈的结构与基本操作

#define stack_size 100

typedef struct {
	void** space;
	int b;
	int t;
} ArrStack;

ArrStack *stackInit(ArrStack* ,int size);

void *stackPop(ArrStack *st);

void *stackPush(ArrStack *stack, void *x);

#endif


