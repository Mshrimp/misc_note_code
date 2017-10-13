#include "stack.h"


STACK_LIST *stack_create(int data_size)
{
	STACK_LIST *handle = NULL;

	handle = (STACK_LIST *)malloc(sizeof(STACK_LIST));
	ERRP(NULL == handle, stack_create handle malloc, goto ERR1);

	handle->save = malloc(data_size);
	ERRP(NULL == handle->save, stack_create handle->save malloc, goto ERR2);

	handle->size = data_size;
	handle->top = 0;

	handle->head.data = NULL;
	handle->head.next = &handle->head;
	handle->head.prev = &handle->head;

	return handle;
ERR2:
	free(handle);
	handle = NULL;
ERR1:
	return NULL;
}

void stack_destory(STACK_LIST *handle)
{
	struct node_t *tail = NULL;
	
	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		free(tail->data);
		tail->data = NULL;
		free(tail);
		tail = NULL;
	}
	free(handle);
	handle = NULL;
}

int stack_num(STACK_LIST *handle)
{
	return handle->top;
}

int stack_is_empty(STACK_LIST *handle)
{
	if (0 == handle->top) {
		return 1;
	} else {
		return 0;
	}
}

void stack_clear(STACK_LIST *handle)
{
	struct node_t *tail = NULL;

	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		free(tail->data);
		tail->data = NULL;
		free(tail);
		tail = NULL;
	}
	free(handle->head.data);
	handle->data = NULL;

	handle->head.prev = &handle->head;
	handle->head.next = &handle->head;
}

int stack_push(STACK_LIST *handle, void *data)
{
	struct node_t *new_node = NULL;

	if (NULL == data) {
		printf("error: stack_push data null\n");
		return -1;
	}

	new_node = (struct node_t *)malloc(sizeof(struct node_t));
	ERRP(NULL == new_node, stack_push new_node malloc, goto ERR1);

	new_node->data = malloc(handle->size);
	ERRP(NULL == new_node->data, stack_push new_node->data malloc, goto ERR2);

	memcpy(new_node->data, data, handle->size);

	new_node->next = handle->head.next;
	new_node->prev = &handle->head;
	handle->head.next->prev = new_node;
	handle->head.next = new_node;

	handle->top++;

	return 0;
ERR2:
	free(new_node);
	new_node = NULL;
ERR1:
	return -1;
}

void *stack_pop(STACK_LIST *handle)
{
	struct node_t *tail = NULL;

	if (stack_is_empty(handle)) {
		printf("error: stack_pop empty\n");
		return NULL;
	}

	memcpy(handle->save, handle->head.next->data, handle->size);

	tail = handle->head.next;
	tail->next->prev = &handle->head;
	handle->head.next = tail->next;
	handle->top--;

	free(tail->data);
	tail->data = NULL;
	free(tail);
	tail = NULL;

	return handle->save;
}

void stack_travel(STACK_LIST *handle, stack_op_t *operation)
{
	struct node_t *tail = NULL;

	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		opreation(tail->data);
	}
	printf("\n");
}
