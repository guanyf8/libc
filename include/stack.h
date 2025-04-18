#ifndef STACK_H
#define STACK_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//顺序栈的结构与基本操作

#define stack_size 100

typedef struct {
	void* space;
	int size;
	int t;
	int step_size;
} ArrStack;

#define stackInit(s,size) stackInitStruct(s,size,sizeof(void*))

ArrStack *stackInitStruct(ArrStack* ,int size,int step);

#define stackPop(s) ({ char* arg=malloc((s)->step_size);  \
						stackPopIn(s,&arg);\
						arg;})

void *stackPopIn(ArrStack *st,void* dst);

#define stackPush(s,val) ({ typeof(val) __temp=(val); char arg[(s)->step_size];\
							memcpy(arg,&__temp,(s)->step_size);\
							stackPushFrom(s,&arg);})

void *stackPushFrom(ArrStack *stack, void *x);

#endif


