#include "hashMap.h"

static inline int hash(int shift, long long key) {
    return key & ~(0xffffffff<<shift);
}

// 扩容哈希表
static int resize(struct hash_map *map, char shift) {
    unsigned short old_capacity = map->capacity_shift;
    unsigned short new_capacity = old_capacity + shift;
    
    struct hash_bin *new_bins = malloc(sizeof(struct hash_bin) * (1<<new_capacity));
    if (!new_bins) return -ENOMEM;

    for (int i = 0; i < (1<<new_capacity); i++) {
        new_bins[i].type = HLIST_BIN;
        INIT_HLIST_HEAD(&new_bins[i].data.list);
    }
    
    //pthread_spin_lock(&map->resize_lock);
    
    for (int i = 0; i < (1<<old_capacity); i++) {
        struct hash_bin *old_bin = &map->bins[i];
        //pthread_spin_lock(&old_bin->lock);
        
        if (old_bin->type == HLIST_BIN) {
            struct hash_node *hnode;
            struct hlist_node *tmp,*tmp_next;
            hlist_for_each_entry_safe(hnode, tmp,tmp_next, &old_bin->data.list,link_point.hlist) {
                unsigned int new_index = hash(new_capacity, *(long long*)&hnode->key);
                struct hash_bin *new_bin = &new_bins[new_index];
                hlist_add_head(&hnode->link_point.hlist, &new_bin->data.list);
            }
        } else {
            struct rb_node *node,*next;
            //todo
            for (node = post_first(&old_bin->data.rb_tree); node && ({next=post_next(node);1;}); node = next) {
                struct hash_node *hnode = rb_entry(node, struct hash_node, link_point.rb);
                unsigned int new_index = hash(new_capacity, *(long long*)&hnode->key);
                struct hash_bin *new_bin = &new_bins[new_index];
                hlist_add_head(&hnode->link_point.hlist, &new_bin->data.list);
            }
        }
        //pthread_spin_unlock(&old_bin->lock);
    }

    struct hash_bin *old_bins = map->bins;
    map->bins = new_bins;
    map->capacity_shift = new_capacity;
    map->upper_threshold = (unsigned int)((1<<new_capacity) * UPPER_LOAD_FACTOR);
    map->lower_threshold = (unsigned int)((1<<new_capacity) * LOWER_LOAD_FACTOR);
    //pthread_spin_unlock(&map->resize_lock);
    free(old_bins);
    return 0;
}

// 链表转红黑树
static void treeify(hashMap* hashmap, struct hash_bin *bin) {
    struct rb_root root = RB_ROOT;
    struct hash_node *hnode;
    struct hlist_node *tmp,*tmp_next;

    hlist_for_each_entry_safe(hnode, tmp,tmp_next, &bin->data.list,link_point.hlist) {
        struct rb_node **_new = &root.rb_node;
        struct rb_node *parent = NULL;
        
        while (*_new) {
            parent = *_new;
            //每一个hnode的linkpoint都变成了rbnode
            struct hash_node *current = rb_entry(*_new, struct hash_node, link_point.rb);
            int result = hashmap->key_compare(hnode->key,current->key);
            if (result<0)
                _new = &parent->rb_left;
            else
                _new = &parent->rb_right;
        }

        rb_link_node(&hnode->link_point.rb, parent, _new);
        rb_insert_color(&hnode->link_point.rb, &root);
    }
    bin->data.rb_tree = root;
    bin->type = RB_BIN;
}

// 初始化哈希表
struct hash_map *hashMapInit( int (*key_compare)(union key_type, union key_type)) {
    struct hash_map *map = malloc(sizeof(struct hash_map));
    if (!map) return NULL;

    map->capacity_shift = INITIAL_CAPACITY;
    map->size = 0;
    map->upper_threshold = (unsigned int)((1<<INITIAL_CAPACITY) * UPPER_LOAD_FACTOR);
    map->lower_threshold = (unsigned int)((1<<INITIAL_CAPACITY) * LOWER_LOAD_FACTOR);
    
    map->bins = malloc(sizeof(struct hash_bin) * (1<<INITIAL_CAPACITY));
    if (!map->bins) {
        free(map);
        return NULL;
    }

    for (int i = 0; i < (1<<INITIAL_CAPACITY); i++) {
        map->bins[i].type = HLIST_BIN;
        INIT_HLIST_HEAD(&map->bins[i].data.list);
        pthread_spin_init(&map->bins[i].lock, PTHREAD_PROCESS_PRIVATE);
    }

    map->key_compare = key_compare;
    pthread_spin_init(&map->resize_lock,PTHREAD_PROCESS_PRIVATE);
    return map;
}


// 插入元素
int hashMapPut(struct hash_map *hashmap, union key_type key, void *value) {
    unsigned int index;
    struct hash_bin *bin;
    struct hash_node *new_node;

    // 检查是否需要扩容
    if (hashmap->size >= hashmap->upper_threshold) {
        if (resize(hashmap,1) != 0) {
            return -ENOMEM;
        }
    }

    index = hash(hashmap->capacity_shift, *(long long*)&key);
    bin = &hashmap->bins[index];
    
    //pthread_spin_lock(&bin->lock);
    
    // 尾插防止已存在相同key
    if (bin->type == HLIST_BIN) {
        int size=0;
        struct hash_node *hnode;
        struct hlist_node *tmp,*tmp_next;
        hlist_for_each_entry_safe(hnode, tmp,tmp_next, &bin->data.list,link_point.hlist)  {
            if (hashmap->key_compare(hnode->key, key) == 0) {
                hnode->value = value;
                //pthread_spin_unlock(&bin->lock);
                return 0;
            }
            size++;
        }
        
        new_node = malloc(sizeof(struct hash_node));
        if (!new_node) {
            //pthread_spin_unlock(&bin->lock);
            return -ENOMEM;
        }
        new_node->key = key;
        new_node->value = value;
        hlist_add_head(&new_node->link_point.hlist, &bin->data.list);
        
        // 检查是否需要转红黑树
        if (size > TREEIFY_THRESHOLD) {
            treeify(hashmap,bin);
        }
        hashmap->size++;
        //pthread_spin_unlock(&bin->lock);
    } else {
        struct rb_node **_new = &bin->data.rb_tree.rb_node;
        struct rb_node *parent = NULL;
        
        while (*_new) {
            parent = *_new;
            struct hash_node *current = rb_entry(*_new, struct hash_node, link_point.rb);
            int result = hashmap->key_compare(key, current->key);
            if (result<0)
                _new = &parent->rb_left;
            else if(result>0)
                _new = &parent->rb_right;
            else{
                current->value = value;
                //pthread_spin_unlock(&bin->lock);
                return 0;
            }
        }
        
        new_node = malloc(sizeof(struct hash_node));
        if (!new_node) {
            //pthread_spin_unlock(&bin->lock);
            return -ENOMEM;
        }
        new_node->key = key;
        new_node->value = value;

        rb_link_node(&new_node->link_point.rb, parent, _new);
        rb_insert_color(&new_node->link_point.rb, &bin->data.rb_tree);

        hashmap->size++;
        //pthread_spin_unlock(&bin->lock);
    }

    return 0;
}

// 查找元素（简化版）
void *hashMapGet(struct hash_map *map, union key_type key) {
    unsigned long index = hash(map->capacity_shift, *(long long*)&key);
    struct hash_bin *bin = &map->bins[index];
    void *value = NULL;

    //pthread_spin_lock(&bin->lock);
    
    if (bin->type == HLIST_BIN) {
        struct hash_node *hnode;
        struct hlist_node *tmp;
        
        hlist_for_each_entry(hnode, tmp, &bin->data.list,link_point.hlist) {
            if (map->key_compare(hnode->key, key) == 0) {
                value = hnode->value;
                break;
            }
        }
    } else {
        struct rb_node *node = bin->data.rb_tree.rb_node;
        while (node) {
            struct hash_node *hnode = rb_entry(node, struct hash_node, link_point.rb);
            int cmp = map->key_compare(hnode->key, key);
            
            if (cmp == 0) {
                value = hnode->value;
                break;
            }
            
            node = cmp < 0 ? node->rb_left : node->rb_right;
        }
    }
    
    //pthread_spin_unlock(&bin->lock);
    return value;
}

int hashMapErase(struct hash_map *map, union key_type key){
    unsigned int index = hash(map->capacity_shift, *(long long*)&key);
    struct hash_bin* bin=&map->bins[index];
    int deleted = 0;

    //pthread_spin_lock(&bin->lock);

    if (bin->type == HLIST_BIN) {
        struct hash_node *hnode;
        struct hlist_node *tmp,*tmp_next;

        hlist_for_each_entry_safe(hnode, tmp,tmp_next, &bin->data.list,link_point.hlist)  {
            if (map->key_compare(hnode->key, key) == 0) {
                hlist_del(&hnode->link_point.hlist);
                free(hnode);
                map->size--;
                deleted = 1;
                break;
            }
        }
    } else {
        struct rb_node **node = &bin->data.rb_tree.rb_node;
        struct rb_node *parent = NULL;

        // 红黑树查找
        while (*node) {
            parent = *node;
            struct hash_node *current = rb_entry(*node, struct hash_node, link_point.rb);
            int cmp = map->key_compare(current->key, key);

            if (cmp == 0) {
                rb_erase(&current->link_point.rb, &bin->data.rb_tree);
                free(current);
                map->size--;
                deleted = 1;
                break;
            }

            node = (cmp < 0) ? &parent->rb_left : &parent->rb_right;
        }

        
    }

    if(map->size<=map->lower_threshold){
        resize(map,-1);
    }

    //pthread_spin_unlock(&bin->lock);
    return deleted; // 返回是否成功删除
}

void hashMapFree(hashMap* map){
    if (!map) return;

    for (unsigned int i = 0; i < (1<<map->capacity_shift); i++) {
        struct hash_bin *bin = &map->bins[i];
        
        //pthread_spin_lock(&bin->lock);

        if (bin->type == HLIST_BIN) {
            struct hash_node *hnode;
            struct hlist_node *tmp,*tmp_next;
        
            hlist_for_each_entry_safe(hnode, tmp,tmp_next, &bin->data.list,link_point.hlist) {
                hlist_del(&hnode->link_point.hlist);
                free(hnode);
            }
        } else {
            struct rb_node *node = post_first(&bin->data.rb_tree);
            
            // 后序遍历释放红黑树节点
            while (node) {
                struct hash_node *hnode = rb_entry(node, struct hash_node, link_point.rb);
                struct rb_node *next = post_next(node);

                rb_erase(node, &bin->data.rb_tree);
                free(hnode);
                node = next;
            }
        }

        //pthread_spin_unlock(&bin->lock);
        pthread_spin_destroy(&bin->lock); // 销毁自旋锁
    }

    free(map->bins);
    free(map);
}