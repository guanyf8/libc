#include "bstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"
void test_BST() {
    // 初始化 BST
    bst* _bst = bstInit(new(bst),numcmp);
    if (!_bst) {
        printf("Failed to initialize BST\n");
        return;
    }

    int key[30]={12,15,18,24,21,
                30,42,45,27,36,
                45,48,33,39,69,
                63,93,87,81,96,
                90,75,84,72,78,
                66,57,54,60,51};
    
    int i;

    for(i=0;i<30;i++){
        bst_node* node=new(bst_node);
        node->avlnode=*new(avl_node);
        node->key.num=key[i];
        node->value=(void*)NULL;
        bstInsert(_bst,node);
        bstTraverse(_bst);
        printf("i is %d\n",i);
    }

    for(i=0;i<15;i++){
        int k=key[rand()%30];
        bstErase(_bst,(union key_type)k);
        bstTraverse(_bst);
        printf("delete %d\n",k);
    }

    // 释放 BST
    bstFree(_bst);
}

int main() {
    test_BST();
    return 0;
}