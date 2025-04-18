#include "graph.h"

static int hash1(union key_type key){
	int hash = 0;
	int len=strlen(key.str);

	for (int i = 0; i < len; i++) {
		//hash*31
		hash = (hash<<5)-hash + key.str[i];
	}

	return hash & 0x7FFFFFFF;
}

static int hash2(union key_type key) {
	return *(key.str)|1;
}

static int cmp(union key_type a, union key_type b) {
	if(a.str && b.str &&a.str!=-1 &&b.str!=-1)
		return !strcmp(a.str, b.str);
	else if(a.str==b.str)
		return 1;
	else 
		return 0;
}

void graphInit(graph* G) {
	G->size = 0;
	INIT_DLIST_HEAD(&G->vert_list);
	hashInitFunction(&G->name_vert_hash, 4,hash1,hash2,cmp);
}

void parse_line(graph* g,const char* line) {
	char* temp_line=line;
	while (*temp_line && (*temp_line)==' ') 
		temp_line++;
	char buffer[strlen(temp_line)+1];
	strcpy(buffer, temp_line);

	
	// 分割顶点和邻接列表
	char *arrow = strstr(buffer, "->");
	if (!arrow) {
		fprintf(stderr, "'->' not exist, illegal input\n");
		return;
	}
	char *vert_part =strtok(buffer, "->");
	char *adj_part = strtok(NULL, "->");
	// print(vert_part);
	// print(adj_part);

	// 解析顶点名称和权重
	char *name = strtok(vert_part, ":"),*num;
	char vert_name[20];
	float vert_weight = 0;
	while (*name && (*name)==' ') 
		name++;
	strcpy(vert_name, name);
	if (num=strtok(NULL, ":")) {
		sscanf(num, "%f", &vert_weight);
	}
	
	vertice *vertex = graphAddVertice(g, vert_name);
	vertex->weight = vert_weight;

	// 解析邻接列表
	char *adj_node = adj_part,*pointer;
	char* edg_name,*_edg_weight;

	while (adj_node) {
		char *edg_name = strtok_r(adj_node, ":",&pointer);
		if (!edg_name) {
			fprintf(stderr, "illegal edge form, adj_list\n");
			return;
		}
		while (*edg_name && (*edg_name)==' ') 
			edg_name++;
		char *_edg_weight = strtok_r(NULL, ",",&pointer);
		float edg_weight;
		sscanf(_edg_weight, "%f", &edg_weight);
		graphAddEdge(g,vertex,graphAddVertice(g,edg_name),edg_weight);
		// print(edg_name);
		// printf("%f\n",edg_weight);
		adj_node=pointer;
	}

}

void graphCreate(graph* g,const char* str){
	char* line,*pointer,*s=str;
	while(line = strtok_r(s,"\n",&pointer)){
	parse_line(g,line);
	s=pointer;
	}
}

void graphTraverse(graph* g){
	vertice* v;
	dlist_for_each_entry(v,&g->vert_list,vert_node){
		printf("%s : %.1f\n",v->name,v->weight);
		edge* edg;
		dlist_for_each_entry(edg,&v->adj_list,node){
			printf("\t %s : %.1f\n",edg->end->name,edg->weight);
		}
	}
}

struct edge* graphAddEdge(graph* G,vertice* current ,vertice* end,float weight) {
    struct edge* current_edge;

	dlist_for_each_entry(current_edge,&current->adj_list,node){
		if(current_edge->end == end) {
			printf("Edge already exists\n");
			return current_edge;
		}
	}

	current_edge=new(struct edge);
    dlist_add(&current_edge->node,&current->adj_list);
    current_edge->end = end;
	current_edge->weight = weight;
	return current_edge;
}

void graphDeleteEdge(graph* G,vertice* current,vertice* end) {
	struct edge* current_edge;
	dlist_for_each_entry(current_edge,&current->adj_list,node){
		if(current_edge->end == end) {
			dlist_del(&current_edge->node);
			free(current_edge);
			break;
		}
	}
}

struct vertice* graphAddVertice(graph* G,char* name) {
	struct vertice* vert;
	if(vert=hashGet(&G->name_vert_hash, (union key_type)name)){
		printf("vertexName %s is already exist\n", name);
		return vert;
	}
	vert=new(struct vertice);
	strcpy(vert->name, name);
	hashInsert(&G->name_vert_hash,vert->name,vert);
	INIT_DLIST_HEAD(&vert->adj_list);
	vert->extra_info = NULL;
	dlist_add(&vert->vert_node,&G->vert_list);
	G->size++;
	return vert;
}

void graphDeleteVertice(graph* G,char* name) {
	struct vertice* vert=hashGet(&G->name_vert_hash,(union key_type)name);
	if(vert == NULL) {
		printf("vertexName %s is not exist\n", name);
		return;
	}
	hashErase(&G->name_vert_hash,(union key_type)name);
	dlist_del(&vert->vert_node);
	free(vert);
	G->size--;
}

void topoSort(graph* G,Queue* q) {
	int i=0,size=G->size;
	
//extra_info就是indegree
#define indegree extra_info

	//从顶点遍历所有边
	vertice* temp_vert;
	dlist_for_each_entry(temp_vert,&G->vert_list,vert_node) {
		edge* adj_edg;
		dlist_for_each_entry(adj_edg,&temp_vert->adj_list,node){
			(int)(adj_edg->end->indegree)++;
		}
	}

	QueueInit(q,G->size);
	struct visit_node{
		vertice* vert;
		struct dlist_node node;
	} *temp_visit,*next_visit;
	struct dlist_node to_visit=LIST_HEAD_INIT(to_visit),*temp_node;
	dlist_for_each_entry(temp_vert,&G->vert_list,vert_node){
		temp_visit=new(struct visit_node);
		temp_visit->vert=temp_vert;
		dlist_add(&temp_visit->node,&to_visit);
	}

	short int flag=1;
	while(size&&flag){ 
		flag = 0;
		dlist_for_each_entry_safe(temp_visit,next_visit,&to_visit,node) {
			temp_vert=temp_visit->vert;
			if (!temp_vert->indegree) {
				flag++;
				QueuePush(q, (char*)temp_vert->name);
				dlist_del(&temp_visit->node);
			}
		}

		while (q->size) { 
			QueuePopIn(q,&temp_vert);
			edge* adj_edg;
			size--;
			dlist_for_each_entry(adj_edg, &temp_vert->adj_list, node) {
				(int)(adj_edg->end->indegree)--;
			}
		}
	}
	if (size) {
		printf("Cyclic graph\n");
	}
#undef indegree
}

typedef struct pathelement {
	int weight;
	struct vertice* vert;
} pathElement;
int dijkCmp(void* a, void* b){
	return ((pathElement*)a)->weight <  ((pathElement*)b)->weight;
}

void dijkstraShortest(graph* G, char* name,char* target,ArrStack* stack) {
	//初始化dijkstra算法所需信息
	typedef struct pathinfo{
		char flag; //visited
		float distance;
		struct vertice* parent;
	} pathinfo;
	
	pathinfo* a;
	a=new(pathinfo);
	a->distance = 0;
	a->flag = 0;
	a->parent = NULL;
	vertice* v,*begin=hashGet(&G->name_vert_hash, (union key_type)name);
	begin->extra_info=a;

	dlist_for_each_entry(v, &G->vert_list, vert_node) {
		if(!strcmp(v->name,name))continue;
		a=new(pathinfo);
		a->distance = INF;
		a->flag = 0;
		v->extra_info=a;
	}

	//开始算法流程
	binHeap heap;
	vertice h[G->size*2];
	//存pathElementent
	heapInit(&heap,h, 0, sizeof(pathElement),dijkCmp);
	int i;
	pathElement temp=(pathElement){.weight=0,.vert=begin};
	// int off_a=(void*)&temp.vert-(void*)&temp, off_b=(void*)&temp.weight-(void*)&temp,a__=sizeof(int),b__=sizeof(pathElement);
	// temp.weight = 0;
	// temp.vert = begin;
    heapPush(&heap, temp);

	while (heap.size > 0) {
		pathElement ver_pop;
		//会推出距离起始点最近的节点
		heapPopIn(&heap,&ver_pop);
		pathinfo *out_info = ver_pop.vert->extra_info;
		if (!out_info->flag) {
			out_info->flag = 1;
		}
		else continue;       //该节点是以前推进来的，距离不是最新最短的

		//遍历当前节点的所有边
		edge * adj_node;
		pathElement ver_push;
		dlist_for_each_entry(adj_node, &ver_pop.vert->adj_list, node) {
			pathinfo* in_info=adj_node->end->extra_info;
			if(!in_info->flag 
				&& out_info->distance+adj_node->weight<in_info->distance){
					in_info->distance = out_info->distance+adj_node->weight;
					in_info->parent = ver_pop.vert;
					
					ver_push.vert = adj_node->end;
					ver_push.weight = in_info->distance;
					heapPush(&heap, ver_push);
			}
		}
	}
	if(!target)return;	if(!stack)return;
	stackInit(stack, G->size);
	vertice* path_vert=hashGet(&G->name_vert_hash, (union key_type)target);
	do{
		stackPush(stack, (char*)path_vert->name);
		path_vert = ((pathinfo*)(path_vert->extra_info))->parent;
	}while(path_vert);
}

//AOV critical path
void critiPathAOV(graph* G) {
#define time weight
	typedef struct info{
        int indegree;
        float critiTime;
        struct vertice* parentTask;
    } info;
	int i=0, size = G->size;

	//初始化
	vertice* v;
	dlist_for_each_entry(v, &G->vert_list, vert_node) {
		info* vinfo;
		vinfo=new(info);
		vinfo->critiTime = v->time;
		vinfo->parentTask = NULL;
		vinfo->indegree=0;
		v->extra_info=vinfo;
	}
	dlist_for_each_entry(v, &G->vert_list, vert_node) {
		edge* e;
		info* vinfo;
		dlist_for_each_entry(e, &v->adj_list, node){
			vinfo=e->end->extra_info;
			vinfo->indegree++;
		}
	}

	//算法流程开始
	vertice* temp_ver;     
	Queue q;
	QueueInit(&q,G->size);
	
	dlist_for_each_entry(temp_ver, &G->vert_list, vert_node) {
		info* vinfo;
		vinfo=temp_ver->extra_info;
		if (!vinfo->indegree) {
			QueuePush(&q, temp_ver);
		}
		
	}            

	while (q.size) {                                      //�����и��ƽ����Ľ���ڽ�ȫ����ȼ�һ
		vertice* temp_pop;
		QueuePopIn(&q,&temp_pop);
		info* out_info=temp_pop->extra_info;
		float temp_time;
		size--;
		edge* temp_edge;
		dlist_for_each_entry(temp_edge, &temp_pop->adj_list, node) {
			info* in_info;
			temp_ver=temp_edge->end;
			in_info=temp_ver->extra_info;
			in_info->indegree--;
			temp_time = temp_ver->time + out_info->critiTime;
			if (temp_time > in_info->critiTime) {
				in_info->critiTime = temp_time;
				in_info->parentTask = temp_pop;
			}
			if (!in_info->indegree)
				QueuePush(&q, temp_ver);
		}
	}
	if (size) {
		printf("Cyclic graph\n");
	}
#undef time
}

//求无向图的最小生成树，这意味着边是对称存在的
void primMinSpanTree(graph* G, graph* T) {
   //初始化dijkstra算法所需信息
	typedef struct pathinfo{
		char flag; //visited
		float distance;
		struct vertice* parent;
	} pathinfo;
	
	pathinfo* a;
	a=new(pathinfo);
	a->distance = 0;
	a->flag = 0;
	a->parent = NULL;
	vertice* v,*begin=container_of(G->vert_list.next,vertice,vert_node);
	begin->extra_info=a;

	dlist_for_each_entry(v, &G->vert_list, vert_node) {
		if(!strcmp(v->name,begin->name))continue;
		a=new(pathinfo);
		a->distance = INF;
		a->flag = 0;
		v->extra_info=a;
	}
	
	binHeap heap;
	vertice h[G->size*2];
	//存pathElementent
	heapInit(&heap,h, 0, sizeof(pathElement),dijkCmp);
	int i;
	pathElement temp=(pathElement){0,begin};
    heapPush(&heap, temp);
      

	while (heap.size>0) {
		pathElement ver_pop;
		heapPopIn(&heap,&ver_pop);
		pathinfo* current_info=ver_pop.vert->extra_info;
		if (!current_info->flag) {
			current_info->flag = 1;
			vertice* curren_vertice=graphAddVertice(T, ver_pop.vert->name);
			if(current_info->parent){
				vertice* parent_vertice=graphAddVertice(T, current_info->parent->name);
				int weight=current_info->distance;
				graphAddEdge(T, parent_vertice, parent_vertice, weight);
				graphAddEdge(T, curren_vertice, curren_vertice, weight);
			}
		}
		else continue; 
		
		edge* adj_node;
		dlist_for_each_entry(adj_node, &ver_pop.vert->adj_list, node){
			vertice* temp_vert;
			pathinfo* in_info;
			temp_vert=adj_node->end;
			in_info=temp_vert->extra_info;
			if (in_info->flag==0&&adj_node->weight < in_info->distance) {
				in_info->distance = adj_node->weight;
				in_info->parent = ver_pop.vert;
				temp.vert = temp_vert;
				temp.weight=adj_node->weight;
			    heapPush(&heap, temp);
			}
		}
	}
}