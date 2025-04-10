
#include "avltree.h"

// int height(avl_node* node) {
//     if (node) {
//         int a = height(node->left);
//         int b = height(node->right);
//         return a > b ? a + 1 : b + 1;
//     }
//     else return 0;
// }

static inline int getheight(avl_node* node){
    if(!node)
        return 0;
    return node->height;
}



//平衡检测是自下向上的，这意味着到此处遍历到的节点的子节点高度总为最新值
//记住插入新node一定要先初始化height=1，否则报错
static avl_node* avlCheck(avl_tree* tree, avl_node* _node,char* flag) {
    avl_node* node=_node;
    while(node){
        int left=getheight(node->left);
        int right=getheight(node->right);
        int factor=left-right;

        if(factor<-1||factor>1){
            if(factor==2){
                //左倾
                avl_node* left_child=node->left;
                int child_factor=getheight(left_child->left)-getheight(left_child->right);
                *flag=child_factor==-1?1:0;
            }else{
                //右倾
                avl_node* right_child=node->right;
                int child_factor=getheight(right_child->left)-getheight(right_child->right);
                *flag=child_factor==1?2:3;
            }
            break;
        }
        node->height=left>right?left+1:right+1;
        node=node->parent;
    }
    return node;
}

inline avl_node* avlLinkNode(avl_node* child, avl_node* parent, avl_node** link){
    child->parent = parent;
	child->left = child->right = NULL;
    child->height = 1;
	*link = child;
    return child;
}

int leftRotate(avl_tree* r, avl_node* node) {
    if (!node || !node->right) return; // 确保节点和右子存在

    
    avl_node* new_root=node->right;

    if(!node->parent) 
        r->root=new_root;
    else{
        avl_node** link=node==node->parent->left?&node->parent->left:&node->parent->right;
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

    return new_root->height;
}

int rightRotate(avl_tree* r,avl_node* node){
    if (!node || !node->left) return; // 确保节点和左子存在
    avl_node* new_root=node->left;

    if(!node->parent) 
    r->root=new_root;
    else{
        avl_node** link=node==node->parent->left?&node->parent->left:&node->parent->right;
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

    return new_root->height;

}

void avlInsert(avl_tree* tree, avl_node* node) {
    char flag;
    //如果没失衡就从node开始上溯更新高度，返回失衡节点base时上面还是删除或插入前的高度
    avl_node* base = avlCheck(tree,node,&flag);

    if(base){
        //存在失衡
        int old_h=base->height,new_h;
        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                new_h=rightRotate(tree, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(tree, base->left); // 先左旋左子树
                new_h=rightRotate(tree, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(tree, base->right); // 先右旋右子树
                new_h=leftRotate(tree, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                new_h=leftRotate(tree, base);
                break;
        }
        //全局更新高度
        if(old_h!=new_h)
            avlCheck(tree, base->parent,&flag);
    }
            
}

static int isLeaf(avl_node* node){
    return !node->left&&!node->right;
}

//只维护树，不释放内存，因为节点内存与map节点相关
static avl_node *bst_erase(avl_tree* tree,avl_node* avlnode){
    avl_node* parent=avlnode->parent;
    avl_node** link;

    //处理根节点
    if(!parent)
        link=&tree->root;
    else
        link=parent->left==avlnode?&parent->left:&parent->right;
    
    if(!avlnode){
        return NULL;
    }else if(isLeaf(avlnode)){
        //leaf
        *link=NULL;
        // update_height(parent);
        return parent;
    }else if(avlnode->left&&avlnode->right){
        //有两个子树
        avl_node* _this=avlnode->right;
        avl_node** _thislink=_this->parent->left==_this?
                                    &_this->parent->left
                                    :&_this->parent->right;
        while(_this->left){
            _thislink=&_this->left;
            _this=_this->left;
        }
        *_thislink=_this->right;
        if(_this->right){
            //最小值有右子树
            _this->right->parent=_this->parent;   
        }
        // update_height(_this->parent);

        *link=_this;
        avl_node* victim=_this->parent==avlnode?_this:_this->parent;


        _this->parent=parent;
        _this->left=avlnode->left;
        _this->right=avlnode->right;
        _this->height=avlnode->height;
        if(_this->left)_this->left->parent=_this;
        if(_this->right)_this->right->parent=_this;
        //不需要对victim更新高度，因为不准，推迟到avlCheck更新高度

        return victim;
    }else{
        //只有一个子树
        *link=avlnode->left?avlnode->left:avlnode->right;
        (*link)->parent=parent;
        // update_height(parent);
        return parent;
    }
}

void avlErase(avl_tree* tree, avl_node* node){
    //此时_this上面有树删除前的高度
    avl_node* victim=bst_erase(tree,node);


    char flag;
    //如果没失衡就从node开始上溯更新高度，返回失衡节点base时上面还是删除或插入前的高度
    avl_node* base=avlCheck(tree,victim,&flag);;
    
    //一直回溯
    while(base){
        //存在失衡
        int old_h=base->height,new_h;
        switch (flag) {
            // 左左型（右单旋）
            case 0b00: // flag1=0, flag2=0
                new_h=rightRotate(tree, base);
                break;

            // 左右型（先左旋后右旋）
            case 0b01: // flag1=0, flag2=1
                leftRotate(tree, base->left); // 先左旋左子树
                new_h=rightRotate(tree, base);      // 再右旋当前节点
                break;

            // 右左型（先右旋后左旋）
            case 0b10: // flag1=1, flag2=0
                rightRotate(tree, base->right); // 先右旋右子树
                new_h=leftRotate(tree, base);         // 再左旋当前节点
                break;

            // 右右型（左单旋）
            case 0b11: // flag1=1, flag2=1
                new_h=leftRotate(tree, base);
                break;
        }
        if(new_h==old_h)
            break;
        base=avlCheck(tree,base->parent,&flag);
    }
}


avl_tree* avlInit(avl_tree* tree) {
    tree->root = NULL;
    return tree;
}


