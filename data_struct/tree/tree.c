#include "tree.h"


TREE *tree_create(int data_size, int tree_max)
{
	TREE *handle = NULL;

	handle = (TREE *)malloc(sizeof(TREE));
	ERRP(NULL == handle, tree_create handle malloc, goto ERR1);

	handle->save = malloc(data_size);
	ERRP(NULL == handle->save, tree_create handle->save malloc, goto ERR2);

	handle->size = data_size;
	handle->max = tree_max;

	handle->root.data = NULL;
	handle->root.left = NULL;
	handle->root.right = NULL;

ERR2:
	free(handle);
	handle = NULL;
ERR1:
	return NULL;
}

void tree_destory(TREE *handle)
{
	
}

void tree_travel_front(struct node_t *root, tree_op_t *operation)
{
	if (NULL == root) {
		printf("error: tree_travel_front root null\n");
		return;
	}

	operation(root->data);
	tree_travel_front(root->left, operation);
	tree_travel_front(root->right, operation);
}

void tree_travel_mid(struct node_t *root, tree_op_t *operation)
{
	if (NULL == root) {
		printf("error: tree_travel_mid root null\n");
		return;
	}

	tree_travel_mid(root->left, operation);
	operation(root->data);
	tree_travel_mid(root->right, operation);
}

void tree_travel_back(struct node_t *root, tree_op_t *operation)
{
	if (NULL == root) {
		printf("error: tree_travel_end root null\n");
		return;
	}

	tree_travel_back(root->left, operation);
	tree_travel_back(root->right, operation);
	operation(root->data);
}

