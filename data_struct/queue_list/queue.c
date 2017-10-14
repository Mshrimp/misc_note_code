#include "queue.h"


QUEUE_LIST *queue_create(int data_size, int queue_max)
{
	QUEUE_LIST *handle = NULL;

	handle = (QUEUE_LIST *)malloc(sizeof(QUEUE_LIST));
	ERRP(NULL == handle, queue_create handle malloc, goto ERR1);

	handle->save = malloc(data_size);
	ERRP(NULL == handle->save, queue_create handle->save malloc, goto ERR2);

	handle->size = data_size;
	handle->max = queue_max;
	handle->count = 0;

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

void queue_destory(QUEUE_LIST *handle)
{
	struct node_t *tail = NULL;
	
	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		free(tail->data);
		tail->data = NULL;
		free(tail);
		tail = NULL;
	}
	free(handle->save);
	handle->save = NULL;
	free(handle);
	handle = NULL;
}

int queue_num(QUEUE_LIST *handle)
{
	return handle->count;
}

int queue_is_empty(QUEUE_LIST *handle)
{
	if (0 == handle->count) {
		return 1;
	}
	return 0;
}

int queue_is_full(QUEUE_LIST *handle)
{
	if (handle->count == handle->max) {
		return 1;
	}
	return 0;
}

void queue_clear(QUEUE_LIST *handle)
{
	struct node_t *tail = NULL;

	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		free(tail->data);
		tail->data = NULL;
		free(tail);
		tail = NULL;
	}
	free(handle->head.data);
	handle->head.data = NULL;

	handle->head.prev = &handle->head;
	handle->head.next = &handle->head;
}

int enqueue(QUEUE_LIST *handle, void *data)
{
	struct node_t *new_node = NULL;

	if (NULL == data) {
		printf("error: queue_push data null\n");
		return -1;
	}

	new_node = (struct node_t *)malloc(sizeof(struct node_t));
	ERRP(NULL == new_node, queue_push new_node malloc, goto ERR1);

	new_node->data = malloc(handle->size);
	ERRP(NULL == new_node->data, queue_push new_node->data malloc, goto ERR2);

	memcpy(new_node->data, data, handle->size);

	new_node->next = handle->head.next;
	new_node->prev = &handle->head;
	handle->head.next->prev = new_node;
	handle->head.next = new_node;

	handle->count++;

	return 0;
ERR2:
	free(new_node);
	new_node = NULL;
ERR1:
	return -1;
}

void *dequeue(QUEUE_LIST *handle)
{
	struct node_t *tail = NULL;

	if (queue_is_empty(handle)) {
		printf("error: queue_pop empty\n");
		return NULL;
	}

	memcpy(handle->save, handle->head.prev->data, handle->size);

	tail = handle->head.prev;
	tail->prev->next = &handle->head;
	handle->head.prev = tail->prev;
	handle->count--;

	free(tail->data);
	tail->data = NULL;
	free(tail);
	tail = NULL;

	return handle->save;
}

void queue_travel(QUEUE_LIST *handle, queue_op_t *operation)
{
	struct node_t *tail = NULL;

	for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev) {
		operation(tail->data);
	}
	printf("\n");
}
