#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "basic.h"
#include "list.h"
#include "rbtree.h"
#include "pthread.h"

#define INITIAL_CAPACITY 4
#define LOAD_FACTOR 0.75
#define TREEIFY_THRESHOLD 8

enum bin_type {
    HLIST_BIN,
    RB_BIN
};

union bin_data {
    struct hlist_head list;
    struct rb_root rb_tree;
};

struct hash_bin {
    enum bin_type type;
    union bin_data data;
    pthread_spinlock_t lock;
};

struct hash_node {
    union {
        struct hlist_node hlist;
        struct rb_node rb;
    } link_point;
    union key_type key;
    void *value;
};

typedef struct hash_map {
    unsigned int size;
    //总是2的倍数，因此只记位移
    unsigned short capacity_shift;
    unsigned int resize_threshold;

    struct hash_bin *bins;

    pthread_spinlock_t resize_lock;
    int (*key_compare)(union key_type key1, union key_type key2);
} hashMap;

hashMap* hashMapInit(int (*key_compare)(union key_type, union key_type));

void hashMapFree(hashMap* map);

int hashMapPut(struct hash_map *map, union key_type key, void *value);

void* hashMapGet(struct hash_map *map,  union key_type key);

int hashMapErase(struct hash_map *map, union key_type key);

#endif