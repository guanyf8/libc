#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"
#define n 1000

typedef struct A {
	char mission_ID[40];
	int space;
} arrive;

typedef struct U {
	char mission_ID[40];
	int index;
	int size;
	struct list_node node;
} used_list;

typedef struct F {
	int index;
	int size;
	struct dlist_node node;
} empty_list;

typedef struct S {
	char* space;
	int capicity;
	struct list_node u;
	struct dlist_node f;
} admin;

void adminInit(admin* s, char* c,int capicity) {
	s->space = c;
	s->capicity = capicity;
	INIT_LIST_HEAD(&s->u);
	empty_list* f= (empty_list*)malloc(sizeof(empty_list));
	dlist_add(&f->node,&s->f);
	f->index = 0;
	f->size = capicity;
}

void adminDistr(admin* s, arrive a) {
	used_list* m,* temp_used,*temp_used2=&s->u;
	empty_list* empt=&s->f;
	char temp[40];
	strcpy(temp, a.mission_ID);
	list_for_each_entry(m, &s->u, node){
		if (strcmp(m->mission_ID, temp)==0) {
			printf("ID existed\n");
			free(temp);
			return;
		}
		temp_used2 = m;
	}
	dlist_for_each_entry(empt, &s->f, node){
		if(empt->size >= a.space)break;
	}
	temp_used = (used_list*)malloc(sizeof(used_list));
	list_add_tail(temp_used,&temp_used2->node);
	temp_used->index = empt->index;
	temp_used->size = a.space;
	memcpy(temp_used->mission_ID, temp,40);
	empt->index += a.space;
	empt->size -= a.space;
	if (empt->size == 0) {
		dlist_del(&empt->node);
		free(empt);
	}
}

void adminRecy(admin* s, char* a) {
	if (s->u.next) {
		used_list* m;
		struct list_node* temp_used=&s->u;
		empty_list* empt = s->f.next;
		int temp;
		list_for_each_entry(m, &s->u, node) {
			if (strcmp(m->mission_ID, a) == 0) {
				temp = m->size;
				list_del(&m->node,temp_used);

				while (m->index > empt->index) {      
					empt = dlist_entry(empt->node.next, empty_list, node);
				}                                    
				//m->index <= empt->index
				empty_list* empt_pre=dlist_entry(empt->node.prev,empty_list,node);
				if (m->size == empt->index - empt_pre->index - empt_pre->size) {      //释放空间恰好填满空洞
					empt_pre->size += m->size + empt->size;
					dlist_del(&empt->node);
					free(empt);
				}
				else if (m->index + m->size == empt->index) {      //  释放空间与empt的开始相接
					empt->index -= m->size;
					empt->size += m->size;
				}
				else if (empt_pre->index + empt_pre->size == m->index) {    //释放空间与empt_pre的尾部相接
					empt_pre->size += m->size;
				}
				else {                                                 //产生新的空洞
					empty_list* temp_empt = (empty_list*)malloc(sizeof(empty_list));
					temp_empt->index = m->index;
					temp_empt->size = m->size;
					__dlist_add(temp_empt,empt_pre,empt);
				}

				free(m);
				break;
			}
			temp_used = &m->node;
		}
		if (!m) {
			printf("unexisted\n"); return;
		}
	}
}

int main(void) {
	char a[n],input, b[40], c[40];
	int d;
	admin S;
	arrive A;
	used_list* temp;
	empty_list* temp_empt;
	adminInit(&S, a, n);
	do {
		printf("allocate space (y)\n recycle mission's space enter(n)   [y\\n]\n");
		
		scanf("%c", &input);
		switch (input) {
		case 'y': {
			printf("The ID of the coming mission\n");
			scanf("%s", A.mission_ID);
			printf("The space you need\n");
			scanf("%d", &A.space);
			adminDistr(&S, A);
			break;
		}
		case 'n': {
			printf("The ID of the mission\n");
			scanf("%s", c);
			adminRecy(&S, c);
			break;
		}
		default:break;
		}
		list_for_each_entry(temp, &S.u, node) {
			printf("%s", temp->mission_ID);
			printf("\n%d\n%d\n", temp->index, temp->size);
		}
		dlist_for_each_entry(temp_empt, &S.f, node){
			printf("%d\n%d\n", temp_empt->index, temp_empt->size);
		}
		printf("To end the program please enter a not interger,otherwise an integer to go on\n");
	} while(scanf("%d",&d)&& getchar());
	return 0;
}