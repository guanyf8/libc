#include "concurrent_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CycularQ.h"
#include "basic.h"
#include <math.h>

static int rb_tree_height(struct rb_node* node) {
    if (!node) return 0;
    int lh = rb_tree_height(node->rb_left);
    int rh = rb_tree_height(node->rb_right);
    return (lh > rh ? lh : rh) + 1;
}

void rbTraverse(map* treemap){
    Queue* q=QueueInit(new(Queue),100);
    if (treemap->root.rb_node == NULL) return;
    int h = rb_tree_height(treemap->root.rb_node);
    const int nodeWidth = 5; // 每个节点的输出宽度，根据需要调整

    QueuePush(q,treemap->root.rb_node);

    for (int level = 0; level < h; level++) {
        int currentLevelNodes = (int)pow(2, level);
        int leadingSpaces = (pow(2, h - level - 1) - 1) * nodeWidth;
        int spacing = (pow(2, h - level) - 1) * nodeWidth;

        // 打印前置空格
        for (int i = 0; i < leadingSpaces; i++) {
            printf(" ");
        }

        for (int i = 0; i < currentLevelNodes; i++) {
            struct rb_node* node=QueuePop(q);
            if (node != NULL) {
                printf("%2d(%c)",(container_of(node,struct map_node,rb_node)->key).num,node->rb_color?'R':'B');
                QueuePush(q, node->rb_left);
                QueuePush(q, node->rb_right);
            } else {
                printf("%*s", nodeWidth, "N");
                QueuePush(q, NULL);
                QueuePush(q, NULL);
            }

            // 打印节点之间的空格
            if (i < currentLevelNodes - 1) {
                for (int j = 0; j < spacing; j++) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    QueueClose(q);
}

void test_Map() {
    // 初始化 BST
    map* _map = map_init(new(map),numcmp);
    if (!_map) {
        printf("Failed to initialize BST\n");
        return;
    }

    int key[40]={12,15,18,24,21,
                30,42,46,27,36,
                45,48,33,39,69,
                63,93,87,81,96,
                90,75,84,72,78,
                66,57,54,60,51,
                71,79,59,34,55,
                77,88,33,62,60,
                };
    
    int i;

    for(i=0;i<40;i++){
        map_node* node=new(map_node);
        node->rb_node=*new(struct rb_node);
        node->key.num=key[i];
        node->value=(void*)NULL;
        map_insert(_map,node);
        rbTraverse(_map);
        printf("i is %d\n",i);
    }

    for(i=0;i<20;i++){
        int k=key[rand()%40];
        map_erase(_map,(union key_type)k);
        rbTraverse(_map);
        printf("delete %d\n",k);
    }

    // 释放 BST
    map_free(_map);
}

int main() {
    test_Map();
    return 0;
}