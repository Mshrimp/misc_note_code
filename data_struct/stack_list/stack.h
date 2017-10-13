#ifndef	__STACK_LISE_H__
#define	__STACK_LISE_H__

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

struct node_t {
	void *data;
	struct node_t *next;
	struct node_t *prev;
};


//抽象数据类型
typedef struct stack_t {
	struct node_t head;
	int size;
	int top;
	void *save;
} STACK_LIST;

typedef void stack_op_t(void *);

//用链表封装栈 --> 链表栈
STACK_LIST *stack_create(int data_size, int stack_max);	//初始化栈
void stack_destory(STACK_LIST *handle);	//销毁栈
int stack_num(STACK_LIST *handle);	//栈长度
int stack_is_empty(STACK_LIST *handle);	//判断栈是否为空栈
void stack_clear(STACK_LIST *handle);	//清空栈
int stack_push(STACK_LIST *handle, void *data);	//压栈
void *stack_pop(STACK_LIST *handle);	//出栈
void stack_travel(STACK_LIST *handle, stack_op_t *op);	//遍历栈


#endif	/* __STACK_H__ */
