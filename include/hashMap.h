#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "basic.h"
#include "list.h"
#include "rbtree.h"
#include "pthread.h"

#define INITIAL_CAPACITY 4
#define UPPER_LOAD_FACTOR 0.75
#define LOWER_LOAD_FACTOR 0.10
#define TREEIFY_THRESHOLD 8

enum bin_type {
    HLIST_BIN,
    RB_BIN
};

struct hash_bin {
    enum bin_type type;
    union bin_data {
        struct hlist_head list;
        struct rb_root rb_tree;
    } data;
    pthread_spinlock_t lock;
};

struct hash_node {
    union {
        struct hlist_node hlist;
        struct rb_node rb;
    } link_point;
    unsigned int hash;
    union key_type key;
    void *value;
};

typedef struct hash_map {
    unsigned int size;
    //总是2的倍数，因此只记位移
    unsigned short capacity_shift;
    unsigned int upper_threshold;
    unsigned int lower_threshold;

    struct hash_bin *bins;

    //全局锁
    pthread_spinlock_t resize_lock;

    //哈希与比较函数
    unsigned int (*hash)(union key_type key_type);
    int (*key_compare)(union key_type key1, union key_type key2);
} hashMap;

hashMap* hashMapInit(struct hash_map* map,
                    int (*key_compare)(union key_type, union key_type),
                    unsigned int (*hash)(union key_type));

void hashMapFree(hashMap* map);

int hashMapPut(struct hash_map *map, union key_type key, void *value);

void* hashMapGet(struct hash_map *map,  union key_type key);

int hashMapErase(struct hash_map *map, union key_type key);

#endif