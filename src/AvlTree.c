#include<stdio.h>
#include<stdlib.h>
#include "AVLTree.h"

#define n 100

// int height(avlNode* node) {
//     if (node) {
//         int a = height(node->left);
//         int b = height(node->right);
//         return a > b ? a + 1 : b + 1;
//     }
//     else return 0;
// }

static inline int getheight(avlNode* node){
    if(!node)
        return 0;
    return node->height;
}




//平衡检测是自下向上的，这意味着到此处遍历到的节点的子节点高度总为最新值
//记住插入新node一定要先初始化height=1，否则报错
static avlNode* avlCheck(avlTree* avlTree, avlNode* _node,char* flag) {
    avlNode* node=_node;
    avlNode* pre1=NULL,*pre2=NULL;
    while(node){
        int left=getheight(node->left);
        int right=getheight(node->right);
        int factor=left-right;

        if(factor<-1||factor>1){
            if(factor==2){
                //左倾
                avlNode* left_child=node->left;
                int child_factor=getheight(left_child->left)-getheight(left_child->right);
                *flag=child_factor==-1?1:0;
            }else{
                //右倾
                avlNode* right_child=node->right;
                int child_factor=getheight(right_child->left)-getheight(right_child->right);
                *flag=child_factor==1?2:3;
            }
            break;
        }
        node->height=left>right?left+1:right+1;
        pre2=pre1;
        pre1=node;
        node=node->parent;
    }
    return node;
}

inline avlNode* avlLinkNode(avlNode* child, avlNode* parent, avlNode** link){
    child->parent = parent;
	child->left = child->right = NULL;
    child->height = 1;
	*link = child;
    return child;
}

void leftRotate(avlTree* r, avlNode* node) {
    if (!node || !node->right) return; // 确保节点和右子存在

    
    avlNode* new_root=node->right;

    if(!node->parent) 
        r->root=new_root;
    else{
        avlNode** link=node==node->parent->left?&node->parent->left:&node->parent->right;
        *link=new_root;
    }

    node->right=new_root->left;
    if(node->right)
        node->right->parent=node;

    new_root->left=node;
    new_root->parent=node->parent;
    node->parent=new_root;
    
    node->height=1+(getheight(node->left)>getheight(node->right)
                        ?getheight(node->left)
                        :getheight(node->right));
    new_root->height=1+(getheight(node)>getheight(new_root->right)
                        ?getheight(node)
                        :getheight(new_root->right));
}

void rightRotate(avlTree* r,avlNode* node){
    if (!node || !node->left) return; // 确保节点和左子存在
    avlNode* new_root=node->left;

    if(!node->parent) 
    r->root=new_root;
    else{
        avlNode** link=node==node->parent->left?&node->parent->left:&node->parent->right;
        *link=new_root;
    }
    
    node->left=new_root->right;
    if(node->left)
        node->left->parent=node;

    new_root->right=node;
    new_root->parent=node->parent;
    node->parent=new_root;

    node->height=1+(getheight(node->left)>getheight(node->right)
                        ?getheight(node->left)
                        :getheight(node->right));
    new_root->height=1+(getheight(node)>getheight(new_root->right)
                        ?getheight(node)
                        :getheight(new_root->right));

}

void avlInsert(avlTree* tree, avlNode* node) {
    char flag;
    avlNode* base = avlCheck(tree,node,&flag);

    //

    if(base){
        //存在失衡

        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                rightRotate(tree, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(tree, base->left); // 先左旋左子树
                rightRotate(tree, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(tree, base->right); // 先右旋右子树
                leftRotate(tree, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                leftRotate(tree, base);
                break;
        }
        //全局更新高度
        avlCheck(tree, base->parent,&flag);
    }
            
}

void avlErase(avlTree* tree, avlNode* node){
    char flag;
    avlNode* base=avlCheck(tree,node,&flag);;
   
    
    //一直回溯
    while(base){
        //存在失衡

        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                rightRotate(tree, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(tree, base->left); // 先左旋左子树
                rightRotate(tree, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(tree, base->right); // 先右旋右子树
                leftRotate(tree, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                leftRotate(tree, base);
                break;
        }
        base=avlCheck(tree,base->parent,&flag);
    }
}


avlTree* avlInit(avlTree* tree) {
    tree->root = NULL;
    return tree;
}


