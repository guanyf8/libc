#include <complete_tree.h>



cmplTree* cmplTreeInit(int size) {
    cmplTree* tree=new(cmplTree);    
	tree->space =new_array(btNode, size);
	tree->index = 0;
    return tree;
}

void cmplInsert(cmplTree* tree, int a) {
	tree->index++;
	tree->space[tree->index].data = a;
	tree->space[tree->index].left = tree->space[tree->index].right = NULL;
	if (tree->index-1) {
		if (tree->index % 2)tree->space[tree->index / 2].right = &tree->space[tree->index];
		else tree->space[tree->index / 2].left = &tree->space[tree->index];
	}
}

void cmplDelete(cmplTree* tree) {
    if (tree->index == 0) {
		return;
	}
	if (tree->index>1) {
		if (tree->index % 2)tree->space[tree->index / 2].right = NULL;
		else tree->space[tree->index / 2].left = NULL;
	}
    tree->index--;
	
}

void minHeapPush(cmplTree* tree,int a) {
	int temp_data,temp_index;
	cmplInsert(tree, a);
	temp_index = tree->index;
	while (temp_index>1&&tree->space[temp_index].data < tree->space[temp_index>>1].data) {
		temp_data = tree->space[temp_index].data;
		tree->space[temp_index].data = tree->space[temp_index>>1].data;
		tree->space[temp_index>>1].data = temp_data;
		temp_index = temp_index>>1;
	}
}

int minHeapPop(cmplTree* tree) {
	if (!tree->index) {
        return -1;
    }

    int temp = tree->space[1].data, min;
    btNode* temp_node = &tree->space[1];
    tree->space[1].data = tree->space[tree->index].data;
    cmplDelete(tree);
    while (temp_node->left && temp_node->right) {
        min = (temp_node->right->data >= temp_node->left->data) ? temp_node->left->data : temp_node->right->data;
        
        if (temp_node->data > min) {
            if (temp_node->right && temp_node->right->data > temp_node->left->data) {
                int temp_data = temp_node->data;
                temp_node->data = temp_node->left->data;
                temp_node->left->data = temp_data;
                temp_node = temp_node->left;
            }
            else {
                int temp_data = temp_node->data;
                temp_node->data = temp_node->right->data;
                temp_node->right->data = temp_data;
                temp_node = temp_node->right;
            }
        }
        else break;
    }
    if (temp_node->left&&!temp_node->left->left) {
        if (temp_node->data > temp_node->left->data) {
            int temp_data = temp_node->data;
            temp_node->data = temp_node->left->data;
            temp_node->left->data = temp_data;
            temp_node = temp_node->left;
        }
    }
    return temp;
}