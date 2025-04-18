
#include<bin_heap.h>


static int cmpdefault(void* a, void* b){
	return *(int*)a<*(int*)b;
}

static inline void memswap(void* a, void* b,int size){
	char temp[size];
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);
}
void heapInit(binHeap* H, void* space, int size, int step_size, int (*cmp)(void*, void*)) {
	H->size = size;
	step_size=step_size?step_size:sizeof(int);
	//退一格，但不会造成内存安全问题，因为不会访问这个单元，仅有计数意义
	H->heap = space-step_size;
	H->step_size = step_size;
	H->cmp = cmp?cmp:cmpdefault;
}

void heapCorrect(binHeap* h) {
	int memsize = h->size*h->step_size, index,step=h->step_size;
	int temp_index,i = (h->size>>1)*step;
	short int LRflag;

	for (; i > 0; i-=step) {
		index = i;
		while ((temp_index=index<<1) <= memsize) {
			if (temp_index  + step <= memsize) 
				temp_index = h->cmp(h->heap+ temp_index  + step , h->heap+temp_index) 
						? temp_index + step
						: temp_index;      //1为右边小，0为左边小
			if (h->cmp(h->heap+temp_index, h->heap+index)) {
				memswap(h->heap+temp_index, h->heap+index,step);
				index= temp_index;
			}
			else break;
		}
	}

}


void* heapPopIn(binHeap* h,void* Value) {
	//index是内存地址，而非单元地址
	int index=h->step_size, LRflag;
	int temp_index,step=h->step_size,memsize=h->size*h->step_size;

	memset(Value, 0, step);
	memswap(Value, h->heap+index,step);
	memswap(h->heap+index, h->heap+memsize,step);
	h->size--;
	memsize-= step;
	while ((temp_index=index<<1) <= memsize) {
		if (temp_index  + step <= memsize) 
			temp_index = h->cmp(h->heap+ temp_index  + step , h->heap+temp_index) 
					? temp_index + step
					: temp_index;      //1为右边小，0为左边小
		if (h->cmp(h->heap+temp_index, h->heap+index)) {
			memswap(h->heap+temp_index, h->heap+index,step);
			index= temp_index;
		}
		else break;
	}

	return Value;
}

void heapPushFrom(binHeap* h, void* elem) {
	int temp_index;
	h->size++;
	memcpy(h->heap+h->size*h->step_size, elem, h->step_size);

	temp_index = h->size;
	while (temp_index > 1 && h->cmp(h->heap+temp_index*h->step_size, h->heap+(temp_index>>1)*h->step_size)) {
		memswap(h->heap+temp_index*h->step_size, h->heap+(temp_index>>1)*h->step_size,h->step_size);
		temp_index = temp_index>>1;
	}
}