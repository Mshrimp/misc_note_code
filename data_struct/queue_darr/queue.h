#ifndef	__QUEUE_H__
#define	__QUEUE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	QUEUE_MAX	10

#define ERRP(con, func, ret)						\
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
	void *save;
} QUEUE;

typedef void(queue_op_t)(void *);

//用顺序表封装队列 --> 数队列
QUEUE *queue_create(int data_size, int queue_max);	//初始化队列
void queue_destory(QUEUE *handle);	//销毁队列
void queue_clear(QUEUE *handle);	//清空队列
int queue_num(QUEUE *handle);	//队列中数据个数
int queue_is_empty(QUEUE *handle);	//判断队列是否为空队列
int queue_is_full(QUEUE *handle);	//判断队列是否为满队列
int enqueue(QUEUE *handle, void *data);	//入队
void *dequeue(QUEUE *handle);	//出队
void queue_travel(QUEUE *handle, queue_op_t *operation);	//遍历队列
int queue_resize(QUEUE *handle, int new_size);	//队列扩容

#endif	/* __QUEUE_H__ */
