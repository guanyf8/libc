#include<stdio.h>
#include "queue.h"
#include "sort.h"
#include <time.h>
#define n 30
#define N 100
#define num 10


int main(void) {
	int j=0,i = 0,a[n],*b[n];
	Queue que;
	QueueInit(&que,n);
    for(i=0;i<n;i++){
        a[i]=rand()%299;
		b[i]=&a[i];
		printf("%d ", a[i]);
    }
	printf("\n");
	mergeSort(a, i);
	// for (j; j<n; j++) {
	// 	printf("%d ", *b[j]);
	// 	b[j]=&a[j];
	// }
	// printf("\n");
	// quickSort(a, 0, 29);
	for (j=0; j<n; j++) {
		printf("%d ", a[j]);
	}
	// int a[50];
	// int b[50];
	// int* ptr[9];           //该数组存行首地址，即堆
	// int i,j,number;
	// int jump= 1024/8;
	// binHeap h;
	// srand(time(NULL));
	
	// for(i = 0; i < 50; i++){
	// 	a[i] = rand() % 500;
	// 	printf("%d ", a[i]);
	// }
	// printf("\n", a[j]);
	// // heapSort(a,50);
	// mergeSortN(a, b,50,3);  
	// for (j=0; j<50; j++) {
	// 	printf("%d ", b[j]);
	// }
	// number=mergeSortN(&h, a,1024,8);           //再对八个队列归并为一个数组排序

	// printf("num:\t");
	// for (i = 0; i < number; i++)
	// 	printf("%d ", *a[i]);
	// printf("\nquene:\t");
	// for (i = 0; i < number; i++)
	// 	printf("%d ", (a[i] - obj) / 128 + 1);         //输出排序结果和对应原队列
	
	return 0;
}