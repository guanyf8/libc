#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"
#include "basic.h"
#include "bin_heap.h"
#include "cycularQ.h"
#include "stack.h"
#include "hashTable.h"
#include "string.h"

#define INF 32767

typedef struct edge {
	struct vertice* end;
	float weight;   
    struct dlist_node node;
} edge;

typedef struct vertice {
	char name[20];
	float weight;       //for AOV
	struct dlist_node adj_list;
	struct dlist_node vert_node;
	void* extra_info;
} vertice;

typedef struct graph {
	int size;
	// 链表存储节点，有利于快速插入删除
	struct dlist_node vert_list;
	hashTable name_vert_hash;
} graph;

#endif

void graphInit(graph *G);

/*
graph: lines 

lines: lines line
	 | line

line: vertax "->" adjacency_list '\n'

adjacency_list: adjacency_list "," edge
			  | edge 

vertax: name ':' weight 
	  | name

edge: name ':' weight

name: [a-zA-Z] [a-zA-Z0-9]*

weight: [0-9]+
*/
void graphCreate(graph *g, const char *str);

void graphTraverse(graph *g);

edge *graphAddEdge(graph *G, vertice *current, vertice *end, float weight);

void graphDeleteEdge(graph *G, vertice *current, vertice *end);

vertice *graphAddVertice(graph *G, char *name);

void graphDeleteVertice(graph *G, char *name);

void topoSort(graph *G, Queue *q);

void dijkstraShortest(graph *G, char *name, char *target, ArrStack *stack);

void critiPathAOV(graph *G);

void primMinSpanTree(graph *G, graph *T);
