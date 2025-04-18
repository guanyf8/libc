#include<stdio.h>
#include"basic.h"
#include"graph.h"


void print(char* str){
    printf("%s\n",str);
}


int main(){
    typedef union key_type k;
    k a = {.num = 48};
    // printf("%d\n",a.num);
    // printf("%c\n",(char)a.num);

    // a.str = "hello";
    // // printf("%d\n",sizeof(void*));
    // // printf("%d\n",sizeof(int));
    // // printf("%d\n",sizeof(long int));
    // printf("%x\n",a.num);
    // printf("%p\n",a.str);

    // a.num= 4960;
    // int n=4;
    // char* b = &a.num;
    // int c= *(int*)(*(char(*)[n])b);
    // printf("%x\n",*(*(char(*)[n])b));
    // printf("%p\n",a.str);
    // printf("%x\n",a.num);
    // int i=4;
    // printf("%d",i<<-2);

    // int a[10];
    // a[0]=99;
    // int* b=a-1;
    // printf("%d",b[1]);

    return 0;
}

