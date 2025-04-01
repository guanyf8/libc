#include "BST.h"
#include "Macro.h"
#include "CycularQ.h"
#include <string.h>
int numcmp(void* a,void* b){
    int a1=*(int*)a;
    int a2=*(int*)b;
    return a1-a2;
}

int mystrcmp(void* a,void* b){
    return strcmp((const char*)a,(const char*)b);
}


static int bstIsLeaf(avlNode* node){
    return !node->left&&!node->right;
}

static inline int update_height(avlNode* node){
    int a=node->left?node->left->height:0;
    int b=node->right?node->right->height:0;
    return a>b?a+1:b+1;
}

Bst* bstInit(Bst* m,int (*cmp)(void* a,void* b)){
    m->root=avlInit(new(avlTree));
    m->cmp=cmp?cmp:mystrcmp;
    m->size=0;
    return m;
}
// 插入键值对
int bstInsert(Bst* treemap, struct bstnode* node){
    avlTree* root=treemap->root;
    avlNode **_new = &(root->root);
    avlNode *parent = NULL;

    void* key=node->key;

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
void *bstSearchValue(Bst* treemap, void* key){
    avlTree* root=treemap->root;
    struct avlNode *node = root->root;

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
avlmapNode *bstSearch(Bst* treemap, void* key){
    avlTree* root=treemap->root;
    struct avlNode *node = root->root;

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


//只维护树，不释放内存，因为节点内存与map节点相关
static avlNode *bst_erase(Bst* tree,avlNode* avlnode){
    avlNode* parent=avlnode->parent;
    avlNode** link;
    if(!parent){
        link=&tree->root->root;
    }else
        link=parent->left==avlnode?&parent->left:&parent->right;
    
    if(!avlnode){
        return NULL;
    }else if(bstIsLeaf(avlnode)){
        //leaf
        *link=NULL;
        update_height(parent);
        return parent;
    }else if(avlnode->left&&avlnode->right){
        //有两个子树
        avlNode* _this=avlnode->right;
        avlNode** _thislink=_this->parent->left==_this?
                                    &_this->parent->left
                                    :&_this->parent->right;
        while(_this->left){
            _thislink=&_this->left;
            _this=_this->left;
        }
        if(_this->right){
            //最小值有右子树
            *_thislink=_this->right;
            _this->right->parent=_this->parent;
            
        }else{
            *_thislink=NULL;
        }
        update_height(_this->parent);

        *link=_this;
        avlNode* change_spot=_this->parent==avlnode?_this:_this->parent;
        _this->parent=parent;
        _this->left=avlnode->left;
        _this->right=avlnode->right;
        if(_this->left)_this->left->parent=_this;
        if(_this->right)_this->right->parent=_this;
        //不更新高度是因为没有意义，后面还要avlcheck

        return change_spot;
    }else{
        //只有一个子树
        *link=avlnode->left?avlnode->left:avlnode->right;
        (*link)->parent=parent;
        update_height(parent);
        return parent;
    }
}


// 删除键值对,浅删除
void bstErase(Bst* treemap, void* key){
    avlTree* root=treemap->root;
    struct avlNode *node = root->root;

    while (node) {
        struct bstnode *_this = container_of(node, struct bstnode, avlnode);

        if (treemap->cmp(key,_this->key)<0)
            node = node->left;
        else if (treemap->cmp(key,_this->key)>0)
            node = node->right;
        else {
            avlNode* n=bst_erase(treemap,node);
            // bstNode *__this = container_of(n, struct bstnode, avlnode);
            //
            printf("after delete %d\n",*(int*)key);
            bstTraverse(treemap);
            avlErase(treemap->root,n);
            //
            printf("after rotate\n");
            free(_this);
            break;
        }
    }
}

static void _node_free(struct avlNode* node){
    if(node->left){
        _node_free(node->left);
    }
    if(node->right){
        _node_free(node->right);
    }
    struct bstnode* map_node= container_of(node,struct bstnode,avlnode);
    free(map_node->value);
    free(map_node->key);
    free(map_node);
}

void bstFree(Bst* treemap){
    avlTree* root=treemap->root;
    _node_free(root->root);
    free(treemap);
}

#include <math.h>

void* bstTraverse(Bst* treemap){
    Queue* q=QueueInit(new(Queue),100);
    // QueuePush(q,treemap->root->root);
    // int current_height=treemap->root->root->height;
    // while(q->size){
    //     avlNode* node=QueuePop(q);
    //     if(!node){
    //         printf("Nil ");
    //         continue;
    //     }
    //     if(node->height!=current_height)
    //         printf("\n");
    //     current_height=node->height;
    //     printf("%d ",*(int*)(container_of(node,struct bstnode,avlnode)->key));
    //     QueuePush(q,node->left);
    //     QueuePush(q,node->right);
    // }
    if (treemap->root->root == NULL) return;
    int h = treemap->root->root->height;
    const int nodeWidth = 4; // 每个节点的输出宽度，根据需要调整

    QueuePush(q,treemap->root->root);

    for (int level = 0; level < h; level++) {
        int currentLevelNodes = (int)pow(2, level);
        int leadingSpaces = (pow(2, h - level - 1) - 1) * nodeWidth;
        int spacing = (pow(2, h - level) - 1) * nodeWidth;

        // 打印前置空格
        for (int i = 0; i < leadingSpaces; i++) {
            printf(" ");
        }

        for (int i = 0; i < currentLevelNodes; i++) {
            avlNode* node=QueuePop(q);
            if (node != NULL) {
                printf("%*d:%d", nodeWidth,*(int*)(container_of(node,struct bstnode,avlnode)->key),node->height);
                QueuePush(q, node->left);
                QueuePush(q, node->right);
            } else {
                printf("%*s", nodeWidth, " ");
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
