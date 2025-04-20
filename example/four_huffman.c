#include<stdio.h>
#include<stdlib.h>
#define n 100

int node_data;

typedef struct stack {
	int* space;
	int* f;
	int* t;
	int size;
} stack;

typedef struct elem {
	int data;
	struct treeNode* addr;
} elem;

typedef struct seq {
	elem* handle;
	int front;
	int tail;
	int size;
} seq;

typedef struct treeNode {
	int data;
	int code;
	struct treeNode* first;
	struct treeNode* next;
	struct treeNode* parent;
} treeNode;

void stacInit(int* a, stack* q) {
	q->space = a;
	q->f = q->t = a;
	q->t--;
	q->size = 0;
}

void stacPush(int i, stack* q) {
	q->t++;
	*q->t = i;
}

int stacPop(stack* q) {
	int temp = *q->t;
	q->t--;
	return temp;
}


void nodeInit(treeNode* node) {
	node->first = node->next = node->parent = NULL;
}


treeNode* huffmanCreate(seq* s) {
	treeNode* root;
	while (s->size > 1) {
		treeNode* child, * parent, * prechild = NULL, * firstchild=NULL;
		int j = s->front, weight = 0;
		elem temp;
		parent = (treeNode*)malloc(sizeof(treeNode));
		nodeInit(parent);
		while (j - s->front <= 3) {
			if (s->handle[j].addr == NULL) {
				child = (treeNode*)malloc(sizeof(treeNode));
				nodeInit(child);
			}
			else child = s->handle[j].addr;
			if (j - s->front == 0)firstchild = child;
			child->data = (s->handle[j]).data;
			child->parent = parent;
			weight += child->data;
			if (prechild != NULL)prechild->next = child;
			prechild = child;
			switch (j - s->front) {
			case 0:child->code = 0; break;
			case 1:child->code = 1; break;
			case 2:child->code = 10; break;
			case 3:child->code = 11; break;
			}
			j++;
		}
		s->front += 3;
		s->size -= 4;
		temp.data = parent->data = weight;
		temp.addr = parent;
		if(firstchild)parent->first = firstchild;
		j = s->front;
		while (weight > s->handle[j + 1].data && j - (s->front) < s->size) {
			s->handle[j] = s->handle[j + 1];
			j++;
		}
		s->handle[j] = temp;
		s->size++;
	}
	root = s->handle[s->front].addr;
	return root;
}

void huffmanCode(treeNode* node, stack* stac) {
	if (node) {
		node_data = node->data;
		stacPush(node->code, stac);
		huffmanCode(node->first, stac);
		stacPop(stac);
		if(node->next)huffmanCode(node->next, stac);
	}
	else {
		printf("%d  ", node_data);
		int* i = stac->f+1;
		while (i <= stac->t) {
			if (*i == 0)printf("00");
			else if (*i == 1)printf("01");
			else printf("%d", *i);
			i++;
		}
		printf("\n");
	}
}



void arrayInit(seq* s, elem* a) {
	int i;
	s->handle = a;
	for (i = 0; i < n; i++) {
		a[i].addr = NULL;
	}
	s->size = 0;
	s->front = s->tail = 0;
}

void order(seq* a) {
	int size = a->size, i = 0, j;
	for (; i < size; i++) {
		for (j = i + 1; j < size; j++) {
			if (a->handle[j].data < a->handle[i].data) {
				elem temp;
				temp = a->handle[i];
				a->handle[i] = a->handle[j];
				a->handle[j] = temp;
			}
		}
	}
}

int main(void) {
	seq s;
	elem a[n];
	int i = 0, b[n];
	treeNode* huffmanTree;
	stack stac;
	stacInit(b, &stac);
	arrayInit(&s, a);
	printf("Please enter a sequence of intergers which contains 3n+1 numbers:\n");
	printf("tips:End with an illegal character such as '*'\n");
	while (scanf("%d", &(a[i].data))) {
		s.size++;
		s.tail++;
		i++;
	}
	order(&s);
	huffmanTree = huffmanCreate(&s);
	huffmanCode(huffmanTree, &stac);
	return 0;
}