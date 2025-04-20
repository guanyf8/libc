#include<stdio.h>
#include"bintree.h"
#include"stack.h"
#define n 200

typedef struct {
	int* space;
	int b;
	int t;
} SqStack;

typedef struct TreeNode{
	int weight;
	struct TreeNode* left;
	struct TreeNode* right;
	struct TreeNode* parent;
} treeNode;

typedef treeNode elemtype;

typedef struct Tree {
	treeNode* root;
	treeNode* head;
	SqStack indexRecord;
} binTree;

typedef struct {
	int data;
	treeNode* node;
} vector2;

typedef struct {
	vector2* a;
	int count;
} Sequece;


void StackInit(SqStack* st, int* s) {
	st->space = s;
	st->b=st->t = -1;
}

int StackPop(SqStack* st) {
	if (st->t != -1) {
		int a = st->space[st->t];
		st->t--;
		return a;
	}
	else {
		printf("empty");
		return;
	}
}

void StackPush(SqStack* stack, int x) {
	if (stack->t> n-2) {
		printf("space out");
		return;
	}
	else {
		stack->t++;
		stack->space[stack->t] = x;
		return;
	}
}

void treeCreate(int* stac,elemtype* tree,binTree* a) {
	a->head = tree;
	a->root = NULL;
	int i;
	for (i = n - 1; i >= 0; i--) {
		tree[i].parent = tree[i].left = tree[i].right = NULL;
		tree[i].weight = 0;
	}
	StackInit(&a->indexRecord, stac);
	for (i = n - 1; i >= 1; i--) {
		StackPush(&a->indexRecord, i);
	}
	return;
}

void HuffmanCreate(binTree* tree, Sequece* a) {
	int i = 0,j,m,temp;
	treeNode* temp_ptr1,* temp_ptr2;
	while (a->count > 1) {
		temp = 0;
		if (a->a[i].node == NULL) {
			j = StackPop(&tree->indexRecord);
			tree->head[j].weight = a->a[i++].data;  //取序列中第一小的数
			temp += tree->head[j].weight;                  //temp记录该数
			temp_ptr1 = &tree->head[j];             //记录该数存入的结点地址
		}
		else {
			temp += a->a[i].data;
			temp_ptr1 = a->a[i++].node;
		}
		if (a->a[i].node == NULL) {
			j = StackPop(&tree->indexRecord);
			tree->head[j].weight = a->a[i].data;        //取序列中第二小的数
			temp_ptr2 = &tree->head[j];                  //记录第二个结点的地址
			temp += tree->head[j].weight;                //temp当前为父节点的权
		}
		else {
			temp += a->a[i].data;
			temp_ptr2 = a->a[i].node;
		}
			temp_ptr1->parent = temp_ptr2->parent = &tree->head[j = StackPop(&tree->indexRecord)];
			tree->head[j].weight = temp;          //创建父节点，并把地址与权放入
			tree->head[j].left = temp_ptr1;
			tree->head[j].right = temp_ptr2;
			
		m = i;
			while (a->a[m + 1].data < temp&&m-i<a->count-2) {
				a->a[m].data = a->a[m + 1].data;
				a->a[m].node = a->a[m + 1].node;
				m++;
				
			}
			a->a[m].data = temp;                      //将新权插入sequece
			a->a[m].node = &tree->head[j];
			a->count--;                //i指向当前序列中最小的数
	}
	tree->root = &tree->head[j];
}

void ordering(Sequece* s) {
	int i = s->count-1;
	int j;
	int min;
	for (; i >= 0; i--) {
		j = s->count - 1 - i;
		for (; j >= 1; j--) {
			if (s->a[i].data > s->a[j].data) {
				min=s->a[j].data;
				s->a[j].data = s->a[i].data;
				s->a[i].data = min;
			}
		}
	}
}

void BT_Traverse(treeNode *node) {
		if (node != NULL) {
			printf("%d ",node->weight);      // 访问结点数据域
			BT_Traverse(node->left);
			BT_Traverse(node->right);
		}
	}

void Huffman_code(treeNode *node) {
	
	if (node->left != NULL&&node->right!=NULL) { 
		Huffman_code(node->left);
		Huffman_code(node->right);
	}
	else {
		SqStack stac;
		int a[n];
		StackInit(&stac, a);
		treeNode* p=node;
		printf("%d  ", node->weight);
		while (p->parent!=NULL) {
			if (p == p->parent->left) {
				StackPush(&stac, 1);
			}
			if (p == p->parent->right) {
				StackPush(&stac, 0);
			}
			p = p->parent;
		}
		while (stac.b != stac.t) {
			printf("%d", StackPop(&stac));
		}
		printf("\n");

	}
}

int main(void) {
	SqStack stack1;
	int stac1[n],stack_in_tree[n];
	elemtype tree[n];
	binTree bintree;
	Sequece seq;
	vector2 seq_arra[n];
	StackInit(&stack1, stac1);
	treeCreate(stack_in_tree, tree, &bintree);
	seq.count = 0;
	seq.a= seq_arra;
	while (scanf("%d", &seq.a[seq.count].data)) {
		seq.a[seq.count].node = NULL;
		seq.count++;
	}
	int i = 0;
	ordering(&seq);
	HuffmanCreate(&bintree, &seq);
	BT_Traverse(bintree.root);
	printf("\n");
	Huffman_code(bintree.root);
	return 0;
}