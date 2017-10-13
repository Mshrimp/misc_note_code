#include "stack.h"

void print(void *data)
{
	if (NULL == data) {
		printf("null ");
	} else {
		printf("%d  ", *(int *)data);
	}
}

int main(void)
{
	STACK *handle = NULL;
	int *data = NULL;
	int num = 0;
	int i = 0;

	handle = stack_create(sizeof(int), STACK_MAX);
	ERRP(NULL == handle, main handle stack_create, goto ERR1);

	stack_clear(handle);

	for (i = 0; i < STACK_MAX; i++)	{
		num = rand() % 100;
		printf("%d  ", num);
		if (stack_push(handle, &num) < 0) {
			printf("error: stack_push failed\n");
		}
	}
	printf("\n");

	stack_travel(handle, print);

	for (i = 0; i < STACK_MAX; i++) {
		data = stack_pop(handle);
		print(data);
		free(data);
		data = NULL;
	}
	printf("\n");

	stack_travel(handle, print);
	stack_destory(handle);

	return 0;
ERR1:
	return -1;
}

