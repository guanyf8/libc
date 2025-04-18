#include<stdio.h>
#include "cycularQ.h"
#define n 30

static void merge(int *s,Queue* q, int f1, int t1, int f2, int t2) {
	int mark1=f1,mark2=f2,i;

	while (mark1 <= t1 && mark2 <= t2) {
		if (s[mark1] < s[mark2]) {
			QueuePush(q,s[mark1]);
			mark1++;
		}
		else {
			QueuePush(q, s[mark2]);
			mark2++;
		}
	}
	while (mark1 <= t1) {
		QueuePush(q,s[mark1]);
		mark1++;
	}
	while (mark2 <= t2) {
		QueuePush(q, s[mark2]);
		mark2++;
	}
	for (i = f1; i <= t2; i++) {
		QueuePopIn(q,&s[i]);
	}
}

void mergesort(int* s,Queue* q,int f,int t) {
	int size=t - f+1;
	if (size > 1) {
		int temp = f-1 + (size >> 1);
		mergesort(s,q,f,temp);
		mergesort(s,q,temp+1,t);
		merge(s,q, f, temp, temp + 1, t);
	}
	
}

void mergeSort(int* s,int size){
	Queue q;
	QueueInitStruct(&q,size,sizeof(int));
	mergesort(s,&q,0,size-1);
}