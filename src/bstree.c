#include "bstree.h"
#include "basic.h"

static inline int update_height(avl_node* node){
    int a=node->left?node->left->height:0;
    int b=node->right?node->right->height:0;
    return a>b?a+1:b+1;
}

bst* bstInit(bst* m,int (*cmp)(union key_type a,union key_type b)){
    m->root=avlInit(new(avl_tree));
    m->cmp=cmp?cmp:mystrcmp;
    m->size=0;
    return m;
}
// 插入键值对
int bstInsert(bst* treemap, struct bstnode* node){
    avl_tree* root=treemap->root;
    avl_node **_new = &(root->root);
    avl_node *parent = NULL;

    union key_type key=node->key;

    // 查找插入位置
    while (*_new) {
        struct bstnode *_this =container_of(*_new, struct bstnode, avlnode);
        parent = *_new;
        int result=treemap->cmp(key,_this->key);
        if (result<0)
            _new = &((*_new)->left);
        else if (result>0)
            _new = &((*_new)->right);
        else
            return -1; // 键已存在
    }

    // 链接节点
    avlLinkNode(&node->avlnode, parent, _new);
    avlInsert(root,&node->avlnode);

    return 0;
}

// 查找键值对，返回值
void *bstSearchValue(bst* treemap, union key_type key){
    avl_tree* root=treemap->root;
    struct avl_node *node = root->root;

    while (node) {
        struct bstnode *_this = container_of(node, struct bstnode, avlnode);

        if (treemap->cmp(key,_this->key)<0)
            node = node->left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->right;
        else
            return _this->value;
    }
    return NULL;
}

// 查找键值对，返回节点
bst_node *bstSearch(bst* treemap, union key_type key){
    avl_tree* root=treemap->root;
    struct avl_node *node = root->root;

    while (node) {
        struct bstnode *_this = container_of(node, struct bstnode, avlnode);

        if (treemap->cmp(key,_this->key)<0)
            node = node->left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->right;
        else
            return _this;
    }
    return NULL;
}





// 删除键值对,浅删除
void bstErase(bst* treemap, union key_type key){
    avl_tree* root=treemap->root;
    struct avl_node *node = root->root;

    while (node) {
        struct bstnode *_this = container_of(node, struct bstnode, avlnode);

        if (treemap->cmp(key,_this->key)<0)
            node = node->left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->right;
        else {
            avlErase(treemap->root,node);
            free(_this);
            break;
        }
    }
}

static void _node_free(struct avl_node* node){
    if(node->left){
        _node_free(node->left);
    }
    if(node->right){
        _node_free(node->right);
    }
    struct bstnode* map_node= container_of(node,struct bstnode,avlnode);
    free(map_node->value);
    free(map_node);
}

void bstFree(bst* treemap){
    avl_tree* root=treemap->root;
    _node_free(root->root);
    free(treemap);
}

#include <math.h>

void bstTraverse(bst* treemap){
    Queue q;
    QueueInit(&q,100);
    if (treemap->root->root == NULL) return;
    int h = treemap->root->root->height;
    const int nodeWidth = 2; // 每个节点的输出宽度，根据需要调整

    QueuePush(&q,treemap->root->root);

    for (int level = 0; level < h; level++) {
        int currentLevelNodes = (int)pow(2, level);
        int leadingSpaces = (pow(2, h - level - 1) - 1) * nodeWidth;
        int spacing = (pow(2, h - level) - 1) * nodeWidth;

        // 打印前置空格
        for (int i = 0; i < leadingSpaces; i++) {
            printf(" ");
        }

        for (int i = 0; i < currentLevelNodes; i++) {
            avl_node* node=NULL;
            QueuePopIn(&q,&node);
            if (node != NULL) {
                printf("%*d", nodeWidth,(container_of(node,struct bstnode,avlnode)->key).num);
                QueuePush(&q, node->left);
                QueuePush(&q, node->right);
            } else {
                printf("%*s", nodeWidth, " ");
                QueuePush(&q, NULL);
                QueuePush(&q, NULL);
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
}
