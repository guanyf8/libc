#include<stdio.h>
#include<stdlib.h>
#include "AVLTree.h"

#define n 100

// int height(bstNode* node) {
//     if (node) {
//         int a = height(node->left);
//         int b = height(node->right);
//         return a > b ? a + 1 : b + 1;
//     }
//     else return 0;
// }

static inline int getheight(bstNode* node){
    if(!node)
        return 0;
    return node->height;
}




//平衡检测是自下向上的，这意味着到此处遍历到的节点的子节点高度总为最新值
//记住插入新node一定要先初始化height=1，否则报错
static bstNode* avlCheck(bst* bst, bstNode* _node,char* flag) {
    bstNode* node=_node;
    bstNode* pre1=NULL,*pre2=NULL;
    while(node){
        int left=getheight(node->left);
        int right=getheight(node->right);
        if(left-right<-1||left-right>1){
            *flag=(pre1==node->left)?0:1;
            *flag=(*flag<<1)&(pre2==pre1->left)?0:1;
            break;
        }
        node->height=left>right?left+1:right+1;
        pre2=pre1;
        pre1=node;
        node=node->parent;
    }
    return node;
}

inline bstNode* bstLinkNode(bst* root,bstNode* parent, bstNode** link){
    bstNode* child = NodeAllocate(root->aq);
    child->parent = parent;
	child->left = child->right = NULL;
    child->height = 1;
	*link = child;
    return child;
}

void leftRotate(bst* r, bstNode* node) {
    if (!node || !node->right) return; // 确保节点和右子存在

    
    bstNode* new_root=node->right;

    if(!node->parent) 
        r->root=new_root;
    else{
        bstNode** link=node==node->parent->left?&node->parent->left:&node->parent->right;
        *link=new_root;
    }

    node->right=new_root->left;
    if(node->right)
        node->right->parent=node;

    new_root->left=node;
    new_root->parent=node->parent;
    node->parent=new_root;
    
    node->height=1+getheight(node->left)>getheigth(node->right)
                        ?getheight(node->left)
                        :getheigth(node->right);
    new_root->height=1+getheight(node)>getheigth(new_root->right)
                        ?getheight(node)
                        :getheigth(new_root->right);
}

void rightRotate(bst* r,bstNode* node){
    if (!node || !node->left) return; // 确保节点和左子存在
    bstNode* new_root=node->right;

    if(!node->parent) 
    r->root=new_root;
    else{
        bstNode** link=node==node->parent->left?&node->parent->left:&node->parent->right;
        *link=new_root;
    }
    
    node->left=new_root->right;
    if(node->left)
        node->left->parent=node;

    new_root->right=node;
    new_root->parent=node->parent;
    node->parent=new_root;

    node->height=1+getheight(node->left)>getheight(node->right)
                        ?getheight(node->left)
                        :getheight(node->right);
    new_root->height=1+getheight(node)>getheight(new_root->right)
                        ?getheight(node)
                        :getheight(new_root->right);

}

void avlInsert(bst* tree, bstNode* node) {
    char flag;
    bstNode* base = avlCheck(tree,node,&flag);

    if(base){
        //存在失衡

        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                rightRotate(&tree->root, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(&tree->root, base->left); // 先左旋左子树
                rightRotate(&tree->root, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(&tree->root, base->right); // 先右旋右子树
                leftRotate(&tree->root, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                leftRotate(&tree->root, base);
                break;
        }
        //全局更新高度
        avlCheck(tree, base->parent,&flag);
    }
            
}

void avlErase(bst* tree, bstNode* node){
    bstNode* base;
    char flag;
    while(base=avlCheck(tree,node,&flag)){
        //存在失衡

        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                rightRotate(&tree->root, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(&tree->root, base->left); // 先左旋左子树
                rightRotate(&tree->root, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(&tree->root, base->right); // 先右旋右子树
                leftRotate(&tree->root, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                leftRotate(&tree->root, base);
                break;
        }
    }
}


bst* bstInit() {
    bst *tree = (bst*)malloc(sizeof(bst));
    tree->root = NULL;
    tree->aq = AdressQueueInit();
    return tree;
}

// int main(void) {
//     bst tree;
//     QueInit(&que, avl_ptr, n);
//     bstCreate(&tree);
//     BT_Traverse(tree.root);
//     return 0;
// }
