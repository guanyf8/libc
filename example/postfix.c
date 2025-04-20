#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "stack.h"

#define n 100
typedef int elemtype;
elemtype PostfixEvaluation(char a[]) {
	ArrStack stack,*stac=&stack; 
	stackInitStruct(stac, stack_size, sizeof(elemtype));
	int i = 0;
	while (a[i]!=0) {
		if (a[i] >= 48 && a[i] <= 57) {
			elemtype b = a[i] - 48;
			while (a[i + 1] >= 48 && a[i + 1] <= 57) {
				b = b * 10 + a[i + 1] - 48;
				i++;
			}
			stackPush(stac, b);
			i++;
		}
		else if (a[i] == 42 || a[i] == 45 || a[i] == 43 || a[i] == 47) {
			elemtype d,b,c;
			stackPopIn(stac,&d);
			stackPopIn(stac,&b);
			switch(a[i]){
				case 42:
					c = d * b;
					break;
				case 45:
					c = b - d;
					break;
				case 43:
					c = d + b;
					break;
				case 47:
					c = b / d;
					break;
			}
			stackPush(stac, c);
			i++;
		}
		else i++;
	}
	printf("end,%d\n", i);
	if (stac->t > 0)printf("The expression is not permitted");
	int result;
	stackPopIn(stac, &result);
	return result;
}

void infix_to_postfix(char a[],Stack* stack) {
	int i,size=strlen(a);
	Stack temp,*ret=stack;
	stackInitStruct(&temp,size,sizeof(short));
	stackInitStruct(&ret, 2*size,sizeof(short));
	int offset = sizeof(short);

	for (i = 0; i < strlen(a); i++) {
		if (a[i] >= 48 && a[i] <= 57){stackPush(ret, a[i]);continue;}
		
		stackPush(ret, ' ');
		if (a[i] >= 40 &&a[i] <= 43 || a[i] == 45 || a[i] == 47){ //40(41)42*43+45-47/	
			//越小越优先
			char priority = 0;
			if (a[i] == 41) {
				priority = 1;
			}
			else if (a[i] == 42 || a[i] == 47)priority = 2;
			else if (a[i] == 43 || a[i] == 45)priority = 3;

			if(temp.t>=0 &&(*(short*)(temp.space+offset*temp.t))>>8<priority){
				short temp_element;
				while(temp_element=*(short*)stackPopIn(&temp, &temp_element)){
					if(temp_element&0xff==41)continue;
					else if(temp_element&0xff==40)break;
					stackPush(ret, temp_element);
				}
			}
			stackPush(&temp, a[i]);
		}else{
			sprintf(stderr, "error: illegal character");
		}
	}
}

int main(void) {
	elemtype s[n+1];
	ArrStack stac;
	char e[n+1],b[n+1];
	gets(e);
	printf("\n");
	infix_to_postfix(e, &stac);
	printf("avert Done\n");
	int i = 0;
	int size=stac.t;
	for (char* p = (char*)stac.space+1; p <= (char*)stac.space+1+size*2; p+=2) {
		b[i] = *p;
		i++;
	}
	b[i] = '\0';
	puts(b);
	printf("puts ready\n");
	int result;
	result = PostfixEvaluation(b);
	printf("%d", result);
	return 0;
}
