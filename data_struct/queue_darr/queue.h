#ifndef	__QUEUE_H__
#define	__QUEUE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	QUEUE_MAX	10

#define ERRP(con, fonc, ret)						\
			do {									\
				if (con) {							\
					printf(#func" failed!\n");		\
					ret;							\
				}									\
			} while (0)


//抽象队列数据结构
typedef struct queue_t {
	void *data;
	int size;
	int max;
	int front;
	int end;
} QUEUE;

typedef void(queue_op_t)(void *);

//用顺序表封装队列 --> 数队列
QUEUE *queue_create(int data_size, int queue_max);	//初始化队列
void queue_destory(QUEUE *handle);	//销毁队列
void queue_clear(QUEUE *handle);	//清空队列
int queue_num(QUEUE *handle);	//队列中数据个数



#endif	/* __QUEUE_H__ */
