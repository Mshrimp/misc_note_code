#ifndef	__QUEUE_LISE_H__
#define	__QUEUE_LISE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QUEUE_MAX	10

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
typedef struct queue_t {
	struct node_t head;
	int size;
	int max;
	int count;
	void *save;
} QUEUE_LIST;

typedef void queue_op_t(void *);

//用链表封装队列 --> 链表队列
QUEUE_LIST *queue_create(int data_size, int queue_max);	//初始化队列
void queue_destory(QUEUE_LIST *handle);	//销毁队列
int queue_num(QUEUE_LIST *handle);	//队列长度
int queue_is_empty(QUEUE_LIST *handle);	//判断队列是否为空队列
int queue_is_full(QUEUE_LIST *handle);	//判断队列是否为满队列
void queue_clear(QUEUE_LIST *handle);	//清空队列
int enqueue(QUEUE_LIST *handle, void *data);	//入队列
void *dequeue(QUEUE_LIST *handle);	//出队列
void queue_travel(QUEUE_LIST *handle, queue_op_t *op);	//遍历队列


#endif	/* __QUEUE_H__ */
