#include<stdio.h>
#include<stdlib.h>
#include<LCRS_tree.h>
#include<stack.h>
#define n 100

int node_data;

typedef struct array {
	treeNode** handle;
	int front;
	int tail;
	int size;
} array;

void nodeInit(treeNode* node) {
	node->first_child = node->next = NULL;
}

#define code depth
treeNode* huffmanCreate(array* s) {
	treeNode* root;
	while (s->size > 1) {
		treeNode* child, * parent, * prechild = NULL, * firstchild=NULL;
		int j = s->front, weight = 0;

		parent = (treeNode*)malloc(sizeof(treeNode));
		nodeInit(parent);
		while (j - s->front <= 3) {
			child = s->handle[j];
			if (j - s->front == 0)firstchild = child;
			child->data = (s->handle[j])->data;
			// child->parent = parent;
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
		parent->data = weight;
		if(firstchild)parent->first_child = firstchild;
		j = s->front;
		while (weight > s->handle[j + 1]->data && j - (s->front) < s->size) {
			s->handle[j] = s->handle[j + 1];
			j++;
		}
		s->handle[j] = parent;
		s->size++;
	}
	root = s->handle[s->front];
	return root;
}

void huffmanCode(treeNode* node, Stack* stac) {
	if (node) {
		node_data = node->data;
		stackPush(stac,node->code);
		huffmanCode(node->first_child, stac);
		free(stackPop(stac));
		if(node->next)huffmanCode(node->next, stac);
	}
	else {
		printf("%d  ", node_data);
		int* i = 0;
		while (i <= stac->t) {
			if (*i == 0)printf("00");
			else if (*i == 1)printf("01");
			else printf("%d", *i);
			i++;
		}
		printf("\n");
	}
}



void arrayInit(array* s, treeNode* a[]) {
	int i;
	s->handle = a;
	s->size = 0;
	s->front = s->tail = 0;
}

void order(array* a) {
	int size = a->size, i = 0, j;
	for (; i < size; i++) {
		for (j = i + 1; j < size; j++) {
			if (a->handle[j]->data < a->handle[i]->data) {
				btNode* temp;
				temp = a->handle[i];
				a->handle[i] = a->handle[j];
				a->handle[j] = temp;
			}
		}
	}
}

int main(void) {
	array s;
	treeNode* a[n];
	int i = 0;
	treeNode* huffmanTree;
	Stack stac;
	stackInitStruct(&stac,n,sizeof(int));
	arrayInit(&s, a);
	printf("Please enter a sequence of intergers which contains 3n+1 numbers:\n");
	printf("tips:End with an illegal character such as '*'\n");
	while (scanf("%d", &((a[i]=(treeNode*)malloc(sizeof(treeNode)))->data))) {
		s.size++;
		s.tail++;
		i++;
	}
	order(&s);
	huffmanTree = huffmanCreate(&s);
	huffmanCode(huffmanTree, &stac);
	return 0;
}