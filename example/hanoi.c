#include<stdio.h>
#include"stack.h"

#define MAX_N 20    //测试点3 的N最大，达到20
//恰好能够解决问题的栈容量。如N=4时， 栈S最满时的INFO.n分别为[3,1,2,1,1,1,1 ] ；
//以此类推 N=n时为：[n-1,1,n-2,1,n-3,1,...,3,1,2,1,1,1,1]
#define MAX_SIZE (2*(MAX_N)-1)

typedef char ELE;
typedef struct{
    ELE n,id;
}INFO;

const char T[6][8] = {    //6种id对应的操作
	"a -> c",  // abc    000
	"a -> b",  // acb    001
	"b -> a",  // bca    010
	"b -> c",  // bac    011
	"c -> b",  // cab    100
	"c -> a"   // cba    101
};//正则：查找目标" (([abc])[abc]([abc]) )"，替换为'\"$2 -> $3\\n\",  // $1'


int main() {
    int N;
    scanf("%d",&N);
    Stack s,*S=&s;
    stackInitStruct(S,MAX_SIZE,sizeof(INFO));
    INFO temp={N,0};
    stackPush(S,temp);  //abc
    INFO cur;
    while(stackPopIn(S,&cur)){
        int n=cur.n;
        int id=cur.id;
        if(1==n){   //可以直接移动
            puts(T[id]);
        }else{
            temp=(INFO){n-1,(id+3)%6};
            stackPush(S,temp); //第三步 1~n-1号盘，从b柱移到c柱的n号盘上
            temp=(INFO){1 , id};
            stackPush(S,temp); //第二步 直接将n号盘，从a柱移到c柱上
            temp=(INFO){n-1, (1^id)};
            stackPush(S,temp); //第一步 1~n-1号盘，从a柱移到b柱上
        }
    }
	return 0;
}