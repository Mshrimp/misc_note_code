#include "queue.h"

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
	QUEUE *handle = NULL;
	int *data = NULL;
	int num = 0;
	int i = 0;

	handle = queue_create(sizeof(int), QUEUE_MAX);
	ERRP(NULL == handle, main handle queue_create, goto ERR1);

	for (i = 0; i < QUEUE_MAX - 1; i++) {
		num = rand() % 100;
		printf("%d  ", num);
		enqueue(handle, &num);
	}
	printf("\n");
	queue_travel(handle, print);

	for (i = 0; i < QUEUE_MAX - 1; i++) {
		data = dequeue(handle);
		print(data);
		free(data);
		data = NULL;
	}
	printf("\n");

	queue_travel(handle, print);
	queue_destory(handle);

	return 0;

ERR1:
	return -1;
}
