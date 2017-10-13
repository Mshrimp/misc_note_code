#include "queue.h"


QUEUE *queue_create(int data_size, int queue_max)
{
	QUEUE *handle = NULL;

	handle = (QUEUE *)malloc(sizeof(QUEUE));
	ERRP(NULL == handle, queue_create handle malloc, goto ERR1);

	handle->data = malloc(data_size, queue_max);
	ERRP(NULL == handle->data, queue_create handle->data malloc, goto ERR2);

	handle->size = data_size;
	handle->max = queue_max;
	handle->front = 0;
	handle->end = 0;

	return handle;
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
	if (handle->front > handle->end) {
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
	void *data = NULL;

	data = handle->data + handle->size * handle->end;

}

