#ifndef	__LLIST_H__
#define	__LLIST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define ERRP(con, func, ret)	do{											\
									if (con)								\
									{										\
										fprintf(stderr, #func" failed!\n");	\
										ret;								\
									}										\
								}while(0)									

struct node_t{
	void *data;
	struct node_t *next;
	struct node_t *prev;
};

typedef struct llist_t{
	struct node_t head;
	int size;
	int num;
}LLIST;

//有头 无序 循环 双向链表


LLIST *llist_creat(int size);

void llist_travel(LLIST *handle);



#endif

