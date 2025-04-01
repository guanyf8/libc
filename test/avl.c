#include "BST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Macro.h"
void test_BST() {
    // 初始化 BST
    Bst* bst = bstInit(new(Bst),numcmp);
    if (!bst) {
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

    for(i=0;i<15;i++){
        bstNode* node=new(bstNode);
        node->avlnode=*new(avlNode);
        node->key=(void*)&(key[i]);
        node->value=(void*)NULL;
        bstInsert(bst,node);
        bstTraverse(bst);
        printf("i is %d\n",i);
    }
    
    // 插入键值对
    // int key1 = 10;
    // int value1 = 100;
    // int key2 = 20;
    // int value2 = 200;
    // int key3 = 30;
    // int value3 = 300;

    // bstNode node1 = {*new(avlNode),(void*)&key1,(void*)&value1}, 
    //         node2 = {*new(avlNode),(void*)&key2,(void*)&value2}, 
    //         node3 = {*new(avlNode),(void*)&key3,(void*)&value3};

    for(i=0;i<15;i++){
        bstErase(bst,&key[rand()%30]);
        bstTraverse(bst);

    }

    // if (bstInsert(bst, &node2) != 0) {
    //     printf("Failed to insert key1\n");
    // }
    // bstTraverse(bst);
    // fflush(stdout); 
    // if (bstInsert(bst, &node3) != 0) {
    //     printf("Failed to insert key2\n");
    // }
    // bstTraverse(bst);
    // if (bstInsert(bst, &node1) != 0) {
    //     printf("Failed to insert key3\n");
    // }
    // bstTraverse(bst);

    // // 查找键值对
    // int* foundValue1 = (int*)bstFind(bst, &key1);
    // int* foundValue2 = (int*)bstFind(bst, &key2);
    // int* foundValue3 = (int*)bstFind(bst, &key3);

    // if (foundValue1) {
    //     printf("Found key1 with value: %d\n", *foundValue1);
    // } else {
    //     printf("Key1 not found\n");
    // }

    // if (foundValue2) {
    //     printf("Found key2 with value: %d\n", *foundValue2);
    // } else {
    //     printf("Key2 not found\n");
    // }

    // if (foundValue3) {
    //     printf("Found key3 with value: %d\n", *foundValue3);
    // } else {
    //     printf("Key3 not found\n");
    // }

    // 删除键值对
    // bstErase(bst, &key2);

    // 再次查找键值对
    // int* foundValue2AfterErase = (int*)bstSearchValue(bst, &key2);
    // if (foundValue2AfterErase) {
        // printf("Key2 found after erase\n");
    // } else {
        // printf("Key2 not found after erase\n");
    // }

    // 释放 BST
    bstFree(bst);
}

int main() {
    test_BST();
    return 0;
}