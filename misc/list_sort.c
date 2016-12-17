/*****************************************************************
说明：
	使用冒泡法排序对双向无头循环链表中的数据进行排序

Date:
	2016-12-16

*****************************************************************/



#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct node *pprev;
	struct node *pnext;
}NODE;

int list_add(struct node * head, int data) 
{	

	NODE * p = (struct node *)malloc(sizeof(struct node));
	p->data = data;

	head->pprev->pnext = p;
	p->pnext = head;
	p->pprev = head->pprev;
	head->pprev = p;
	
	return 0;
}

int list_travel(struct node * head)
{
	printf("------------- \n");
	struct node * p = head->pnext;
	while(p!=head) {
		printf("%d\n", p->data);
		p=p->pnext;
	}
	printf("------------- \n");
}

int list_release(struct node * head)
{
	struct node * p = head->pnext;
	while(p!=head) {
		struct node * q = p;
		p->pnext->pprev = p->pprev;
		p->pprev->pnext = p->pnext;
		p = p->pnext;
		free(q);
	}
}

NODE *list_creat(void)
{
	int i = 0;
	NODE *head = (NODE *)malloc(sizeof(NODE));
	head->pnext = head;
	head->pprev = head;

	for (i = 0; i < 10; i++)
	{
		list_add(head, i);
	}

	return head;
}





int sort_list(struct node *head)
{
	struct node *i, *j;
	struct node *tmp = NULL;
	struct node *tmp_t = NULL;
	int cnt = 0;

	tmp = (NODE *)malloc(sizeof(NODE));

	for (i = head->pnext; i != head; i = i->pnext)
	{
		for (j = i->pnext; j != head; j = j->pnext)
		{
			if (i->data < j->data)
			{
				printf("****************count = %d\n", cnt++);
				tmp->pnext = i->pnext;
				tmp->pprev = i->pprev;
				i->pprev->pnext = tmp;
				i->pnext->pprev = tmp;
				
				i->pnext = j->pnext;
				i->pprev = j->pprev;
				j->pprev->pnext = i;
				j->pnext->pprev = i;

				j->pnext = tmp->pnext;
				j->pprev = tmp->pprev;
				tmp->pnext->pprev = j;
				tmp->pprev->pnext = j;
				tmp_t = i;
				i = j;
				j = tmp_t;
				printf("i->data = %d, j->data = %d\n", i->data, j->data);
			}
			
		}
	}

	return 0;
}

int main(int argc, char ** argv)
{
	struct node * head = list_creat();
	
	list_travel(head);
	sort_list(head);
	list_travel(head);
	list_release(head);
	return 0;
}

/*
int node_exchange(struct node **li_node_big, struct node **li_node_small)
{
	struct node *tmp = NULL;

	tmp->pnext = li_node_small->pnext;
	tmp->pprev = li_node_small->pprev;
	li_node_small->pprev->pnext = tmp;
	li_node_small->pnext->pprev = tmp;

	li_node_samll->pnext = li_node_big->pnext;
	li_node_small->pprev = li_node_big->pprev;
	li_node_big->pprev->pnext = li_node_small;
	li_node_big->pnext->pprev = li_node_small;

	li_node_big->pnext = tmp->pnext;
	li_node_big->pprev = tmp->pprev;
	tmp->pnext->pprev = li_node_big;
	tmp->pprev->pnext = li_node_big;

	return 0;
}
*/

