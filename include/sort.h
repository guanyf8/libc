#ifndef SORT_H
#define SORT_H

#include "cycularQ.h"
#include "bin_heap.h"
#include "basic.h"

void mergeSort(int* s,int size) ;

void quickSort(int* a, int head, int last);

void sort(int* a, int head, int last);

void heapSort(int a[],int size);

int mergeSortN(int* a,int* target,int size,int n);

#endif