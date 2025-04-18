#include "LCRS_tree.h"

void lcrsTree2binaryTree(treeNode* a, btNode* b) {
	if (a->first_child) {
		b->left = malloc(sizeof(btNode));
        b->left->data = a->first_child->data;
		lcrsTree2binaryTree(a->first_child, b->left);
	}
	if (a->next) {
		b->right = malloc(sizeof(btNode));
        b->right->data = a->next->data;
		lcrsTree2binaryTree(a->next, b->right);
	}
}

void LCRS2binary(tree* t,binTree* b){
    treeNode* root = t->root;
    b->root = malloc(sizeof(btNode));
    b->root->left=b->root->right=NULL;
    b->root->data = root->data;
    lcrsTree2binaryTree(root,b->root);

}