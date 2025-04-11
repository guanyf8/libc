#include "complete_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

int main(void) {
	int a[64],i;
	cmplTree* tree;
	tree=cmplTreeInit(64);
	srand(time(NULL));
	for (i = 1; i < 64; i++) {
		a[i]=rand() % 100;
		printf("%d ", a[i]);
		minHeapPush(tree, a[i]);
	}
	printf("\n");
	for (i = 1; i < 64; i++) {
		printf("%d ", tree->space[i].data);
	}
	printf("\n");
	
	printf("\n");
	for (i = 1; i < 64; i++) {
		printf("%d ", minHeapPop(tree));
	}
	return 0;

}