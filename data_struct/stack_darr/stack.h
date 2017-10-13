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

//抽象数据类型
typedef struct stack_t {
	void *data;
	int size;
	int max;
	int top;
	void *save;
} STACK;

typedef void stack_op_t(void *);

//用顺序表封装栈 --> 数栈
STACK *stack_create(int data_size, int stack_max);	//初始化栈
void stack_destory(STACK *handle);	//销毁栈
int stack_num(STACK *handle);	//栈长度
int stack_is_empty(STACK *handle);	//判断栈是否为空栈
int stack_is_full(STACK *handle);	//判断栈是否为满栈
void stack_clear(STACK *handle);	//清空栈
int stack_resize(STACK *handle, int new_size);	//栈扩容
int stack_push(STACK *handle, void *data);	//压栈
void *stack_pop(STACK *handle);	//出栈
void stack_travel(STACK *handle, stack_op_t *op);	//遍历栈


#endif	/* __STACK_H__ */
