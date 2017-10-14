#ifndef __LLIST_H__
#define __LLIST_H__


struct node_t {
	void *data;
	struct node_t *next;
	struct node_t *prev;
}


#endif	/* __LLIST_H__ */
