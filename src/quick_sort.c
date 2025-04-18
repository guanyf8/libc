#include<stdio.h>
#include<stdlib.h>
#include<time.h>



static inline void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
void sort(int* a, int head, int end) {
	int i = 0, j = 0;
	for (; i <= end-1; i++) {
		for (j=i+1; j <= end; j++) {
			if (a[i] > a[j])swap(&a[i], &a[j]);
		}
	}
}

void quickSort(int* a, int head, int last) {      //head与last是index
	if (head>=last)return;
	else if(last-head<=3){
		sort(a,head,last);
		return;
	}

	
	//a[last]就是pivot
	swap(&a[last], &a[(last + 1 + head)>>1]);
	int low=head,high=last-1;

	//此处条件互斥，low和high不可能相等
	while(a[low]<=a[last]&&low<=high)
		low++;
	while(a[high]>a[last]&&low<=high)
		high--;
	
	while (low < high) {
		while(a[low]<=a[last]&&low<=high)
			low++;
		while(a[high]>a[last]&&low<=high)
			high--;
		swap(&a[low], &a[high]);
		low++;
		high--;
		
	}
	//high有可能到-1，low有可能到last
	swap(&a[low], &a[last]);

	quickSort(a, head, low - 1);
	quickSort(a, low+1, last);
}

