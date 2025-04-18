#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "bin_heap.h"

#define N 100
#define num 10

static int maxcmp(void* a, void* b){
	return *(int*)a > *(int*)b;
}

static int minPtrcmp(void* a, void* b){
	int *av,*bv;
	memcpy(&av, a, sizeof(void*));
	memcpy(&bv, b, sizeof(void*));
	return *av < *bv;
}

int heapSort(int a[],int size) {
	binHeap maxheap;
	
	heapInit(&maxheap, a, size,sizeof(int),maxcmp); 

	heapCorrect(&maxheap);
	int i = maxheap.size;
	for (; i > 0; i--) {
		int temp;
		heapPopIn(&maxheap,&temp);

		a[i-1] = temp;
	}
	
}

static inline int terminate(int* temp,int** end,int n){
	for(n--;n>=0;n--){
		if(temp==end[n])
			return 1;
	}
	return 0;
}

int mergeSortN(int* a, int* b,int size,int n) {         //
	binHeap h;
	int index=0,i=0,left=size%n, jump=left?(size-left)/(n-1):size/n,seg_size=jump;
	int *ptr[n],*end[n];
	int *temp;

	for (i = 0; i < n; i++,seg_size=left&&(n-i==1)?left:jump){
		ptr[i] = a+jump*i;          
		end[i] = ptr[i]+seg_size-1;
		heapSort(ptr[i], seg_size);      //在数组上造大顶堆
	}

	heapInit(&h, ptr, n,sizeof(int*),minPtrcmp);
	heapCorrect(&h);

	while(h.size) {
		heapPopIn(&h,&temp);
		b[index++] = *temp;
		if(terminate(temp,end,n))
			continue;
		temp++;
		heapPush(&h, temp);
	}
	return index;
}
