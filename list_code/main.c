#include "llist.h"

#define	STU_NUM	10



struct stu{
	char name[32];
	char sex;
	int id;
	int score;
};


void ls(void *data)
{
	struct stu *sp = (struct stu *)data;

	printf("name: %s | sex: %c | id: %d | score: %d\n", sp->name, sp->sex, sp->id, sp->score);

	return ;
}


int main(int argc, char *argv[])
{
	int i = 0;
	LLIST *handle = NULL;
	struct stu cls = {"stu", 'M', 10001, 100};

	handle = llist_create(sizeof(struct stu));
	ERRP(handle == NULL, main llist_create, goto ERR1);
	
	
	for (i = 0; i < STU_NUM; i++)
	{
		snprintf(cls.name, sizeof(struct stu), "stu_%c%c", rand()%26 + 'a', rand()%26 + 'a');
		cls.sex = "MF"[rand() % 2];
		cls.id = rand() % 100 + 10000;
		cls.score = rand() % 20 + 80;

		llist_append(handle, &cls);
	}

	llist_travel(handle, ls);
	printf("========================================\n");

	llist_destory(handle);

ERR1:
	return 0;
}

