#ifndef	__STACK_H__
#define	__STACK_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STACK_MAX	10

#define	ERRP(con, func, ret)					\
			do {								\
				if (con) {						\
					printf(#func" failled!\n");	\
					ret;						\
				}								\
			} while(0)


typedef struct stack_t {
	void *data;
	int size;
	int max;
	int top;
} STACK;

typedef void stack_op_t(void *);

STACK *stack_create(int data_size, int stack_max);
void stack_destory(STACK *handle);
int stack_num(STACK *handle);
int stack_is_empty(STACK *handle);
int stack_is_full(STACK *handle);
void stack_clear(STACK *handle);
int stack_resize(STACK *handle, int new_size);
int stack_push(STACK *handle, void *data);
void *stack_pop(STACK *handle);
void stack_travel(STACK *handle, stack_op_t *op);


#endif
