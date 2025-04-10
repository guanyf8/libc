//
// Created by guan on 2025/3/11.
//

#include "concurrent_map.h"
#include <stdatomic.h>
#define max_depth 64


tree_map* map_init(tree_map *m, int (*cmp)(union key_type a,union key_type b)){
    m->root=RB_ROOT;
    m->cmp=cmp?cmp:mystrcmp;
    return m;
}

int map_insert(tree_map* treemap, struct map_node* node) {
    struct rb_root* root=&treemap->root;

    pthread_rwlock_wrlock(&root->lock);
    struct rb_node **_new = &(root->rb_node);
    struct rb_node *parent = NULL;
    union key_type key=node->key;

    // //CAS插入树根，用于节点级细粒度锁
    // if(!root->rb_node){
    //     void* expected=NULL;
    //     if(atomic_compare_exchange_strong(&root->rb_node,&expected,node))
    //         return 0;
    // }
    
    // 查找插入位置
    while (*_new) {
        struct map_node *_this = rb_entry(*_new, struct map_node, rb_node);
        parent = *_new;

        if (treemap->cmp(key,_this->key)<0){
            _new = &((*_new)->rb_left);
        }
        else if (treemap->cmp(key,_this->key)>0){
            _new = &((*_new)->rb_right);
        }
        else
            return -1; // 键已存在
    }

    // 链接节点
    rb_link_node(&node->rb_node, parent, _new);
    rb_insert_color(&node->rb_node, root);

    pthread_rwlock_unlock(&root->lock);
    return 0;
}

// 查找键值对
void *map_find(tree_map* treemap, union key_type key) {
    struct rb_root* root=&treemap->root;
    pthread_rwlock_rdlock(&root->lock);
    struct rb_node *node = root->rb_node;
    while (node) {
        struct map_node *_this = rb_entry(node, struct map_node, rb_node);

        if (treemap->cmp(key,_this->key)<0)
            node = node->rb_left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->rb_right;
        else
            return _this->value;
    }
    pthread_rwlock_unlock(&root->lock);
    return NULL;
}

// 查找键值对
map_node *map_search(tree_map* treemap, union key_type key) {
    struct rb_root* root=&treemap->root;
    pthread_rwlock_rdlock(&root->lock);
    struct rb_node *node = root->rb_node;

    while (node) {
        struct map_node *_this = rb_entry(node, struct map_node, rb_node);

        if (treemap->cmp(key,_this->key)<0)
            node = node->rb_left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->rb_right;
        else
            return _this;
    }
    pthread_rwlock_unlock(&root->lock);
    return NULL;
}

// 删除键值对,浅删除
void map_erase(tree_map* treemap, union key_type key) {
    struct rb_root* root=&treemap->root;

    pthread_rwlock_wrlock(&root->lock);
    struct rb_node *node = root->rb_node;

    while (node) {
        struct map_node *_this = rb_entry(node, struct map_node, rb_node);

        if (treemap->cmp(key,_this->key)<0)
            node = node->rb_left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->rb_right;
        else {
            rb_erase(node, root);
            free(_this);
            break;
        }
    }
    pthread_rwlock_unlock(&root->lock);
}

static void _map_node_free(struct rb_node* node){
    if(node->rb_left){
        _map_node_free(node->rb_left);
    }
    if(node->rb_right){
        _map_node_free(node->rb_right);
    }
    struct map_node* mapNode= rb_entry(node,struct map_node,rb_node);
    free(mapNode->value);
    free(mapNode);
}

void map_free(tree_map* treemap){
    struct rb_root* root=&treemap->root;
    pthread_rwlock_wrlock(&root->lock);
    _map_node_free(root->rb_node);
    pthread_rwlock_unlock(&root->lock);
    free(treemap);
}
