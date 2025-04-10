#include "hashTable.h"


int main(void){
    hashTable* t=hashInit(3);

    int key[30]={12,15,18,24,21,
        30,42,45,27,36,
        45,48,33,39,69,
        63,93,87,81,96,
        90,75,84,72,78,
        66,57,54,60,51};

    int i;

    for(i=0;i<30;i++){
        tableUnit node={key[i],NULL};
        hashInsert(t,node);
        hashTraverse(t);
        printf("i is %d\n",i);
    }
    for(i=0;i<30;i++){
        hashErase(t,(union key_type)key[i]);
        hashTraverse(t);
        printf("delete i is %d\n",key[i]);
    }

}