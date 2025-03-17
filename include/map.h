//
// Created by guan on 2025/3/11.
//

#ifndef PARALLELLJ_MAP_H
#define PARALLELLJ_MAP_H

#include <string.h>
#include "rbtree.h"
#include "stdlib.h"

typedef struct map {
    struct rb_node rb_node;
    void* key;
    void *value;
} mapNode;

typedef struct {
    struct rb_root root;
    int (*cmp)(void* key1,void* key2);
} treeMap;

typedef treeMap Map;

int numcmp(void* a,void* b);

int mystrcmp(void* a,void* b);

treeMap* map_init(int (*cmp)(void* a,void* b));
// 插入键值对
int map_insert(treeMap* treemap, void* key, void *value) ;

// 查找键值对
void *map_find(treeMap* treemap, void* key);

mapNode *map_search(treeMap* treemap, void* key);

// 删除键值对
void map_erase(treeMap* treemap, void* key) ;

void map_free(treeMap* treemap);

#endif //PARALLELLJ_MAP_H
