#include<stdio.h>
#include<stdlib.h>
#include <list.h>
#include <allocator.h>
#define n 30


typedef struct {
	int* space;
	int b;
	int t;
} SqStack;

//定义链表结点的结构

typedef struct PolyTerm {
	int exp;
	float coe;
	struct list_node node;
} _term;

typedef  _term elemtype;

allocator* alloc;

void ListInit(struct list_node* x){
	INIT_LIST_HEAD(x);
}

void order_Insert(elemtype t, struct list_node* x) {
	struct list_node *p=x,* newnode;
	_term* p_elem,*temp_elem;
	list_for_each_entry(temp_elem, x, node){
		p_elem=temp_elem;
		p=&p_elem->node;
		if (t.exp > p_elem->exp) break;
	}
	//p是正好需要插入的节点
	if (t.exp == p_elem->exp) {
		printf("exp= %d has existed", t.exp);
		return;
	}
	else {
		int j;
		_term* temp_term=nodeAlloc(alloc);
		temp_term->coe = t.coe;
		temp_term->exp = t.exp;
		list_add_tail(&temp_term->node, p);
		printf("term in\n");
	}
	
}

void Add(struct list_node* x, struct list_node* y) {            
	struct list_node* p = x->next, * q = y->next;
	struct list_node* P = p, * Q = q;
	if(!p||!q)return;
	while (Q!= NULL) {
		p = P;
		while (p->next!=NULL &&list_entry(Q,_term,node)->exp > list_entry(p,_term,node)->exp) {
			p = p->next;
		}                      //此时p指向的位置大于等于Q
		q = Q;
		Q = q->next;
		if (list_entry(q,_term,node)->exp == list_entry(p,_term,node)->exp) {
			list_entry(p,_term,node)->coe = list_entry(q,_term,node)->coe + list_entry(p,_term,node)->coe;
		}
		else {
			_term* temp_term = nodeAlloc(alloc);
			memcpy(temp_term, list_entry(q,_term,node), sizeof(elemtype));
			list_add_tail(&temp_term->node, p);
		}
	}
}

int main(void) {
	int j = 0;
	int* a1[n];
	struct list_node LP, LQ, LPQ;
	elemtype term;

	alloc=allocInit(elemtype);
	ListInit(&LP);
	ListInit(&LQ);

	printf("now Please determine the number of terms in polynomial LP\n");
	scanf("%d", &j);
	printf("coefficient\texponent\n");
	for (int i = 1; i <= j; i++) {
		scanf("%f %d", &term.coe, &term.exp);
		order_Insert(term, &LP);
	}
	printf("now Please determine the number of terms in polynomial LQ\n");
	scanf("%d", &j);
	printf("coefficient\texponent\n");
	for (int i = 1; i <= j; i++) {
		scanf("%f %d", &term.coe, &term.exp);
		order_Insert(term,&LQ);
	}

	Add(&LP, &LQ);
	LPQ = LP;
	_term* traverse;
	printf("now the result is\n");
	list_for_each_entry(traverse, &LPQ, node) {
		printf("%f %d\n", traverse->coe, traverse->exp);
	}

	return 0;
}

