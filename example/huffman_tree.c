#include<stdio.h>
#include"bintree.h"
#include"stack.h"
#include "allocator.h"
#define n 200

typedef btNode elemtype;

typedef struct staticTree {
	binTree tree;
	allocator* aq;
} staticTree;

typedef struct {
	btNode** trees;
	int count;
} Forest;

void treeCreate(staticTree* t) {
	t->tree.root = NULL;
	int i;
	t->aq = allocatorInit(sizeof(btNode));
	return;
}

void HuffmanCreate(staticTree* tree, Forest* forest) {
	int i = 0,j,m,temp;
	btNode* temp_ptr1,* temp_ptr2,*parent;
	while (forest->count > 1) {
		temp = 0;

		temp += forest->trees[i]->data;  //temp记录该数
		temp_ptr1 = forest->trees[i++]; //取序列中第一小的数

		temp += forest->trees[i]->data; //temp当前为父节点的权
		temp_ptr2 = forest->trees[i];   //取序列中第二小的数

		parent=nodeAlloc(tree->aq);    //创建父节点，并把地址与权放入
		parent->left = temp_ptr1;
		parent->right = temp_ptr2;
		parent->data = temp;
		
		m = i;
		while (forest->trees[m + 1]->data < temp&&m-i<forest->count-2) {
			forest->trees[m] = forest->trees[m + 1];
			m++;
		}
		forest->trees[m]=parent;
		forest->count--;                //i指向当前序列中最小的数
	}
	tree->tree.root = forest->trees[i];
}

void ordering(Forest* s) {
	int i = s->count-1;
	int j;
	int min;
	for (; i >= 0; i--) {
		j = s->count - 1 - i;
		for (; j >= 1; j--) {
			if (s->trees[i]->data > s->trees[j]->data) {
				min=s->trees[j]->data;
				s->trees[j]->data = s->trees[i]->data;
				s->trees[i]->data = min;
			}
		}
	}
}

void BT_Traverse(btNode *node) {
		if (node != NULL) {
			printf("%d ",node->data);      // 访问结点数据域
			BT_Traverse(node->left);
			BT_Traverse(node->right);
		}
	}

void Huffman_code(btNode *node,Stack* stac) {
	//huffman 树一定有两个儿子或者没有儿子
	if (node->left != NULL&&node->right!=NULL) {
		stackPush(stac, 1); 
		Huffman_code(node->left,stac);
		free(stackPop(stac));
		stackPush(stac, 0);
		Huffman_code(node->right,stac);
		free(stackPop(stac));
	}
	else {
		btNode* p=node;
		printf("%d  ", node->data);
		int i=0;
		for(;i<stac->t;i++){
			printf("%d", ((int*)(stac->space))[i]);
		}
		printf("\n");

	}
}

int main(void) {

	staticTree bintree;
	treeCreate(&bintree);


	btNode* seq_arra[n];
	Forest seq;
	seq.count = 0;
	seq.trees= seq_arra;
	while (scanf("%d", &(seq.trees[seq.count]=nodeAlloc(bintree.aq))->data)) {
		seq.count++;
	}

	int i = 0;
	ordering(&seq);
	HuffmanCreate(&bintree, &seq);
	BT_Traverse(bintree.tree.root);
	printf("\n");

	Stack stack1;
	stackInitStruct(&stack1, n, sizeof(int));
	Huffman_code(bintree.tree.root, &stack1);
	return 0;
}