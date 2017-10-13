#include "stack.h"



STACK *stack_create(int data_size, int stack_max)
{
	STACK *handle = NULL;

	handle = (STACK *)malloc(sizeof(STACK));
	ERRP(NULL == handle, stack_create handle malloc, goto ERR1);

	handle->data = malloc(data_size * stack_max);
	ERRP(NULL == handle->data, stack_create handle->data malloc, goto ERR2);

	handle->size = data_size;
	handle->max = stack_max;
	handle->top = 0;

	return handle;
ERR2:
	free(handle);
ERR1:
	return NULL;
}

void stack_destory(STACK *handle)
{
	free(handle->data);
	handle->data = NULL;
	free(handle);
	handle = NULL;
}

int stack_num(STACK *handle)
{
	return handle->top;
}

int stack_is_empty(STACK *handle)
{
	if (handle->top == 0) {
		return 1;
	}
	return 0;
}

int stack_is_full(STACK *handle)
{
	if (handle->top == handle->max) {
		return 1;
	}
	return 0;
}

void stack_clear(STACK *handle)
{
	memset(handle->data, 0, handle->size * handle->max);
}

int stack_resize(STACK *handle, int new_size)
{
	void *new_data = NULL;

	new_data = malloc(handle->size * new_size);
	ERRP(NULL == new_data, stack_resize new_data malloc, goto ERR1);

	if (handle->top <= new_size) {
		memcpy(new_data, handle->data, handle->size * handle->top);
	} else {
		printf("error: stack_resize top > new_size\n");
		goto ERR2;
	}
	free(handle->data);
	handle->data = new_data;
	handle->max = new_size;

	return 0;
ERR2:
	free(new_data);
	new_data = NULL;
ERR1:
	return -1;
}

int stack_push(STACK *handle, void *data)
{
	if (stack_is_full(handle)) {
		printf("error: stach_push stack full\n");
		return -1;
	}

	memcpy(handle->data + handle->size * handle->top, data, handle->size);
	handle->top++;

	return 0;
}

void *stack_pop(STACK *handle)
{
	void *data = NULL;

	if (stack_is_empty(handle)) {
		printf("error: stack_pop stack empty");
		return NULL;
	}

	data = malloc(handle->size);
	ERRP(NULL == data, stack_pop data malloc, goto ERR1);

	handle->top--;
	memcpy(data, handle->data + handle->top, handle->size);
	memset(handle->data + handle->top, 0, handle->size);

	return data;
ERR1:
	return NULL;
}

void stack_travel(STACK *handle, stack_op_t *operation)
{
	int i = 0;
	for (i = 0; i < handle->top; i++) {
		operation(handle->data + handle->size * i);
	}
	printf("\n");
}

