#ifndef COMTREE_H
#define COMTREE_H

#include "bintree.h"
#include "basic.h"
#include <stdlib.h>

typedef struct completeTree {
	btNode* space;
	int index;
} cmplTree;

cmplTree* cmplTreeInit(int size);

void cmplInsert(cmplTree* tree, int a);

void cmplDelete(cmplTree* tree);

//使用完全树实现的小顶堆
void minHeapPush(cmplTree* tree,int a) ;

int minHeapPop(cmplTree* tree);

#endif