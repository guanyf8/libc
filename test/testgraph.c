#include<stdio.h>
#include"basic.h"
#include"graph.h"


void print(char* str){
    printf("%s\n",str);
}


int main(){
    char str[]="1->2:28,3:2,5:1\n\
    2->4:9,6:10\n\
    3->6:24,8:27\n\
    4->7:8,8:7\n\
    5->2:8,6:26\n\
    6->7:7,8:1\n\
    7->8:8\n\
    8->";
    graph g;
    graphInit(&g);
    graphCreate(&g,str);
    // ArrStack stack;
    // stackInit(&stack,20);
    // dijkstraShortest(&g,"1","8",&stack);
    // char* name;
    // while(name=stackPop(&stack)){
    //     printf("%s ",name);
    // }
    // printf("\n");
    Queue q;
    topoSort(&g,&q);
    char** name=q.space;
    int i=0;
    for(;i<q.cap;i++)
        printf("%s ",name[i]);
    // graph T;
    // graphInit(&T);
    // primMinSpanTree(&g,&T);
    // graphTraverse(&T);

    return 0;
}