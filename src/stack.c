#include "stack.h"


ArrStack* stackInitStruct(ArrStack* st,int size,int step) {
	st->space = (void*)malloc(step * size);
	st->t = -1;
    st->size=size;
    st->step_size = step;
}

void* stackPopIn(ArrStack* st,void* a) {
    if(st->t==-1){
        memset(a,0,st->step_size);
        return NULL;
    }
    int step = st->step_size;
    memcpy(a,st->space+st->t*step,step);
    st->t--;
    return a;
}

void* stackPushFrom(ArrStack* stack, void* x) {
	if (stack->t == stack->size - 1) { 
		return NULL;
	}
    stack->t++;
    int step = stack->step_size;
    memcpy(stack->space+stack->t*step, x, step);
    return x;
}

void stackResize(Stack* s, int new_cap){
    s->size=new_cap;
    s->space=(void*) realloc(s->space,s->step_size*new_cap);
}

void stackClose(Stack* s){
    free(s->space);
}