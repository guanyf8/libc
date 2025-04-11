#include "stack.h"


ArrStack* stackInit(ArrStack* st,int size) {
	st->space = (void**)malloc(sizeof(void*) * size);
	st->b = st->t = -1;
}

void* stackPop(ArrStack* st) {
    if(st->b == st->t){
        printf("empty");
        return NULL;
    }
    void* a = st->space[st->t];
    st->t--;
    return a;
}

void* stackPush(ArrStack* stack, void* x) {
	if (stack->t > stack_size - 2) { 
		printf("space out");
		return NULL;
	}
    stack->t++;
    stack->space[stack->t] = x;
    return x;
}