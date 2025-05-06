#include "hashMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "basic.h"
#include <math.h>

static int rb_tree_height(struct rb_node* node) {
    if (!node) return 0;
    int lh = rb_tree_height(node->rb_left);
    int rh = rb_tree_height(node->rb_right);
    return (lh > rh ? lh : rh) + 1;
}

void rbTraverse(struct rb_root root,int tab){
    Queue* q=QueueInit(new(Queue),100);
    if (root.rb_node == NULL) return;
    int h = rb_tree_height(root.rb_node);
    const int nodeWidth = 5; // 每个节点的输出宽度，根据需要调整

    QueuePush(q,(root.rb_node));
    printf("%*s",tab,"");
    for (int level = 0; level < h; level++) {
        int currentLevelNodes = (int)pow(2, level);
        int leadingSpaces = (pow(2, h - level - 1) - 1) * nodeWidth;
        int spacing = (pow(2, h - level) - 1) * nodeWidth;

        // 打印前置空格
        for (int i = 0; i < leadingSpaces; i++) {
            printf(" ");
        }

        for (int i = 0; i < currentLevelNodes; i++) {
            struct rb_node* node=NULL;
            QueuePopIn(q,&node);
            if (node != NULL) {
                printf("%2d(%c)",(container_of(node,struct hash_node,link_point.rb)->key).num,node->rb_color?'R':'B');
                QueuePush(q, (node->rb_left));
                QueuePush(q, (node->rb_right));
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
        printf("\n%*s",tab,"");
    }
    printf("\n");
    QueueClose(q);
}

void listTraverse(struct hlist_head head,int tab){
    struct hash_node *hnode;
    struct hlist_node *tmp;
    printf("%*s",tab,"");
    hlist_for_each_entry(hnode, tmp, &head,link_point.hlist) {
        printf("%d\t",(hnode->key).num);
    }
    printf("\n");
}

void hashMapTraverse(hashMap* map){
    
    int i=0;
    for(;i<(1<<map->capacity_shift);i++){
        printf("bin %d:\n",i);
        if(map->bins[i].type==HLIST_BIN){
            listTraverse(map->bins[i].data.list,6);
        }else if(map->bins[i].type==RB_BIN){
            rbTraverse(map->bins[i].data.rb_tree,6);
        }
    }

}

void test_hashMap() {
    // 初始化 BST
    hashMap* _map =new(hashMap); 
    hashMapInit(_map,numcmp,NULL);
    //hashMap* _map = hashMapInit(strcmp,strhash);
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
        hashMapPut(_map,(union key_type)key[i],NULL);
        hashMapTraverse(_map);
        printf("i is %d\n",i);
    }

    for(i=0;i<20;i++){
        int k=key[rand()%40];
        hashMapErase(_map,(union key_type)k);
        hashMapTraverse(_map);
        printf("delete %d\n",k);
    }

    // 释放 BST
    hashMapFree(_map);
}

int main() {
    test_hashMap();
    return 0;
}