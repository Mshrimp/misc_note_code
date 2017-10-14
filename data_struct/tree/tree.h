#ifndef	__TREE_H__
#define	__TREE_H__


#define	ERRP(con, func, ret)								\
						do {								\
							if (con) {						\
								printf(#func" failed\n");	\
								ret;						\
						} while (0)

struct node_t {
	void *data;
	struct node_t *left;
	struct node_t *right;
};

typedef struct tree_t {
	struct node_t root;
	int size;
	int max;
	void *save;
} TREE;

typedef void tree_op_t(void *);

TREE *tree_create(int data_size, int tree_max);
void tree_destory(TREE *handle);
void tree_travel_front(struct node_t *root, tree_op_t *operation);
void tree_travel_mid(struct node_t *root, tree_op_t *operation);
void tree_travel_back(struct node_t *root, tree_op_t *operation);


#endif /* __TREE_H__ */
