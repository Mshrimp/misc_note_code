/********************************************
 	用顺序表封装队列函数
 ********************************************/
#include "queue.h"


QUEUE *queue_create(int data_size, int queue_max)
{
	QUEUE *handle = NULL;

	handle = (QUEUE *)malloc(sizeof(QUEUE));
	ERRP(NULL == handle, queue_create handle malloc, goto ERR1);

	handle->data = malloc(data_size * queue_max);
	ERRP(NULL == handle->data, queue_create handle->data malloc, goto ERR2);

	handle->save = malloc(data_size);
	ERRP(NULL == handle->save, queue_create handle->save malloc, goto ERR3);

	handle->size = data_size;
	handle->max = queue_max;
	handle->front = 0;
	handle->end = 0;

	return handle;
ERR3:
	free(handle->data);
	handle->data = NULL;
ERR2:
	free(handle);
	handle = NULL;
ERR1:
	return NULL;
}

void queue_destory(QUEUE *handle)
{
	free(handle->data);
	handle->data = NULL;
	free(handle);
	handle = NULL;
}

void queue_clear(QUEUE *handle)
{
	memset(handle->data, 0, handle->size * handle->max);
}

int queue_num(QUEUE *handle)
{
	if (handle->front >= handle->end) {
		return handle->front - handle->end;
	} else {
		return handle->max - (handle->end - handle->front);
	}
}

int queue_is_empty(QUEUE *handle)
{
	if (handle->front == handle->end) {
		return 1;
	}
	return 0;
}

int queue_is_full(QUEUE *handle)
{
	if ((handle->front + 1) % handle->max == handle->end) {
		return 1;
	}
	return 0;
}

int enqueue(QUEUE *handle, void *data)
{
	if (queue_is_full(handle)) {
		printf("error: queue is full\n");
		return -1;
	}

	memcpy(handle->data + handle->size * handle->front, data, handle->size);
	handle->front++;

	if (handle->front == handle->max) {
		handle->front = 0;
	}

	return 0;
}

void *dequeue(QUEUE *handle)
{
	if (queue_is_empty(handle)) {
		printf("error: queue is empty\n");
		return NULL;
	}

	memcpy(handle->save, handle->data + handle->size * handle->end, handle->size);
	memset(handle->data + handle->size * handle->end, 0, handle->size);
	handle->end++;

	if (handle->end == handle->max) {
		handle->end = 0;
	}

	return handle->save;
}

void queue_travel(QUEUE *handle, queue_op_t *operation)
{
	int i = 0, j = 0;

	for (i = handle->end, j = 0; j < queue_num(handle); i++, j++) {
		if (i == handle->max) {
			i = 0;
		}
		operation(handle->data + handle->size * i);
	}
	printf("\n");
}

int queue_resize(QUEUE *handle, int new_size)
{
	void *new_data = NULL;

	if (queue_num(handle) > new_size) {
		printf("error: queue_resize queue_num > new_size");
		return -1;
	}

	new_data = malloc(handle->size * new_size);
	ERRP(NULL == new_data, queue_resize new_data malloc, goto ERR1);

	if (handle->front > handle->end) {
		memcpy(new_data, handle->data + handle->size * handle->end, handle->size * queue_num(handle));
	} else {
		memcpy(new_data, handle->data + handle->size * handle->end, handle->size * (handle->max - handle->end));
		memcpy(new_data + handle->size * (handle->max - handle->end), handle->data, handle->size * handle->front);
	}

	free(handle->data);
	handle->data = new_data;
	handle->max = new_size;

	return 0;
ERR1:
	return -1;
}

