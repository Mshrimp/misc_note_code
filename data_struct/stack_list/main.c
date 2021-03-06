
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
	STACK_LIST *handle = NULL;
	int *data = NULL;
	int num = 0;
	int i = 0;

	handle = stack_create(sizeof(int), STACK_MAX);
	ERRP(NULL == handle, main handle stack_create, goto ERR1);

	for (i = 0; i < STACK_MAX; i++) {
		num = rand() % 100;
		printf("%d  ", num);
		stack_push(handle, &num);
	}
	printf("\n");

	stack_travel(handle, print);

	for (i = 0; i < STACK_MAX; i++) {
		data = stack_pop(handle);
		printf("%d  ", *data);
	}
	printf("\n");

	return 0;
ERR1:
	return -1;
}

