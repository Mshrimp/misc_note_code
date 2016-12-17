#include "llist.h"



LLIST *llist_create(int size)
{
	LLIST *handle = NULL;

	handle = (LLIST *)malloc(sizeof(LLIST));
	ERRP(handle == NULL, malloc, goto ERR1);

	handle->size = size;
	handle->num = 0;
	
	handle->head.data = NULL;
	
	handle->head.next = &handle->head;
	handle->head.prev = &handle->head;

	return handle;
ERR1:
	return NULL;
}


void llist_append(LLIST *handle, void *data)
{
	struct node_t *new = NULL;

	ERRP(data == NULL, llist_append, goto ERR1);

	new = (struct node_t *)malloc(sizeof(struct node_t));
	ERRP(new == NULL, malloc new, goto ERR2);
	
	new->data = (void *)malloc(handle->size);
	ERRP(new->data == NULL, malloc data, goto ERR2);

	memcpy(new->data, data, handle->size);

	new->next = &handle->head;
	new->prev = handle->head.prev;
	handle->head.prev->next = new;
	handle->head.prev = new;

	handle->num++;

	return ;
ERR2:
	free(new);
ERR1:
	return ;
}



void llist_prepend(LLIST *handle, void *data)
{
	struct node_t *new = NULL;

	ERRP(data == NULL, llist_append, goto ERR1);

	new = (struct node_t *)malloc(sizeof(struct node_t));
	ERRP(new == NULL, malloc new, goto ERR2);
	
	new->data = (void *)malloc(handle->size);
	ERRP(new->data == NULL, malloc data, goto ERR2);

	memcpy(new->data, data, handle->size);

	new->next = &handle->head;
	new->prev = handle->head.prev;
	handle->head.next->prev = new;
	handle->head.next = new;

	handle->num++;

	return ;
ERR2:
	free(new);
ERR1:
	return ;
}






void llist_travel(LLIST *handle, llist_op_t *op)
{
	struct node_t *tail = NULL;

	for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
	{
		op(tail->data);
	}

	return ;
}


void llist_destory(LLIST *handle)
{
	struct node_t *tail = NULL;
	struct node_t *save = NULL;

	for (tail = handle->head.next; tail != &handle->head; tail = save)
	{
		save = tail->next;
		free(tail->data);
		free(tail);
	}

	free(handle);

	return ;
}




