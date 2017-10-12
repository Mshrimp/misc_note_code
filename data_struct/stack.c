#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX	10

int count = 0;

void stack_travel(int *stack, int top)
{
	int i = 0;
	for (i = 0; i < top; i++) {
		printf("%d  ", *(stack + i));
	}
	printf("\n");
}

int is_stack_empty(int top)
{
	if (top == 0) {
		return 1;
	}
	return 0;
}

int is_stack_full(int top)
{
	if (top == STACK_MAX) {
		return 1;
	}
	return 0;
}

void stack_push(int *stack, int data, int *top)
{
	if (is_stack_full(*top)) {
		return;
	}

	*(stack + *top) = data;
	(*top)++;
}

int stack_pop(int *stack, int *top)
{
	if (is_stack_empty(*top)) {
		return -1;
	}

	(*top)--;
	return *(stack + *top);
}

int main(void)
{
	int stack[STACK_MAX] = { 0 };
	int stack_top = 0;
	int num = 0;
	int i = 0;
	
	for (i = 0; i < STACK_MAX; i++) {
		num = rand() % 100;
		printf("%d  ", num);
		stack_push(stack, num, &stack_top);
	}
	printf("\n");

	stack_travel(stack, stack_top);
	printf("stack_top = %d\n", stack_top);

	for (i = 0; i < STACK_MAX; i++) {
		num = stack_pop(stack, &stack_top);
		if (num < 0) {
			printf("空  ");
		} else {
			printf("%d  ", num);
		}
	}
	printf("\n");

	stack_travel(stack, stack_top);

	return 0;
}

