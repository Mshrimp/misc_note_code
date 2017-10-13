#include <stdio.h>
#include <stdlib.h>

#define QUEUE_MAX	10

int queue_num(int front, int end)
{
	if (front > end) {
		return front - end;
	} else if (front < end) {
		return QUEUE_MAX - (end - front);
	}
}

void queue_travel(int *queue, int front, int end)
{
	int i = 0, j = 0;
	for (i = end, j = 0; j < queue_num(front, end); i++, j++) {
		if (i == QUEUE_MAX) {
			i = 0;
		}
		printf("%d  ", *(queue + i));
	}
	printf("\n");
}

int is_queue_empty(int front, int end)
{
	if (front == end) {
		return 1;
	}
	return 0;
}

int is_queue_full(int front, int end)
{
	if ((front + 1) % QUEUE_MAX == end) {
		return 1;
	}
	return 0;
}

int enqueue(int *queue, int *front, int end, int data)
{
	if (is_queue_full(*front, end)) {
		return -1;
	}
	*(queue + *front) = data;
	(*front)++;

	if (*front == QUEUE_MAX) {
		*front = 0;
	}
	return 0;
}

int dequeue(int *queue, int front, int *end)
{
	int data = 0;
	if (is_queue_empty(front, *end)) {
		return -1;
	}

	data = *(queue + *end);
	(*end)++;
	
	if (*end == QUEUE_MAX) {
		*end = 0;
	}

	return data;
}

int main(void)
{
	int queue[QUEUE_MAX] = { 0 };
	int front = 0;
	int end = 0;
	int data = 0;
	int i = 0;
	int num = 0;

	for (i = 0; i < QUEUE_MAX; i++) {
		data = rand() % 100;
		printf("%d  ", data);
		if (enqueue(queue, &front, end, data) < 0) {
			printf("enqueue error, data = %d\n", data);
		}
	}
	printf("\n");

	num = queue_num(front, end);
	queue_travel(queue, front, end);

	for (i = 0; i < num; i++) {
		data = dequeue(queue, front, &end);
		if (data < 0) {
			printf("空  ");
		} else {
			printf("%d  ", data);
		}
	}
	printf("\n");

	return 0;
}
