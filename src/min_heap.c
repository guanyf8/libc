
#include<min_heap.h>

void heapInit(minHeap* H, int** space, int size) {
	H->size = size;
	H->heap = space;
}

void heapCorrect(minHeap* h) {
	int i = h->size, index;
	int* temp;
	short int LRflag;

	for (; i > 0; i--) {
		index = i;
		while ((index<<1) <= h->size) {
			if ((index<<1)  + 1 <= h->size) {
				LRflag = *h->heap[(index<<1)  + 1] < *h->heap[(index<<1) ] ? 1 : 0;      //1为右边小，0为左边小
				temp = LRflag ? h->heap[(index<<1) +1] : h->heap[(index<<1) ];            //只取地址
			}
			else {
				LRflag = 0;
				temp = h->heap[(index<<1) ];              
			}
			if (*temp < *h->heap[index]) {
				if (LRflag) {
					h->heap[(index<<1)  + 1] = h->heap[index];
					h->heap[index] = temp;
					index = (index<<1)  + 1;
				}
				else {
					h->heap[(index<<1) ] = h->heap[index];
					h->heap[index] = temp;
					index =(index<<1) ;
				}

			}
			else break;
		}
	}

}

int* heapPop(minHeap* h) {
	int index = 1, LRflag;
	int* temp,* Value;

	Value = h->heap[1];
	h->heap[1] = h->heap[h->size];
	h->size--;

	while ((index<<1)  <= h->size) {
		if ((index<<1)  + 1 <= h->size) {
			LRflag = *h->heap[(index<<1)  + 1] < *h->heap[(index<<1) ] ? 1 : 0;         //1为右边小，0为左边小
			temp = LRflag ? h->heap[(index<<1)  + 1] : h->heap[(index<<1) ];
		}
		else {
			LRflag = 0;
			temp = h->heap[(index<<1) ];
		}
		if (*temp < *h->heap[index]) {
			if (LRflag) {
				h->heap[(index<<1)  + 1] = h->heap[index];
				h->heap[index] = temp;
				index = (index<<1)  + 1;
			}
			else {
				h->heap[(index<<1) ] = h->heap[index];
				h->heap[index] = temp;
				index =(index<<1) ;
			}
		}
		else break;
	}

	return Value;
}

void heapPush(minHeap* h, int* elem) {
		int *temp_data, temp_index;
		h->size++;
		h->heap[h->size] = elem;

		temp_index = h->size;
		while (temp_index > 1 && *h->heap[temp_index] < *h->heap[temp_index>>1]) {
			temp_data = h->heap[temp_index];
			h->heap[temp_index] = h->heap[temp_index>>1];
			h->heap[temp_index>>1] = temp_data;
			temp_index = temp_index>>1;
		}
}