
#include "stack.h"


int main(void)
{
	STACK_LIST *handle = NULL;

	handle = stack_create(sizeof(int));
	ERRP(NULL == handle, main handle stack_create, goto ERR1);

	return 0;
ERR1:
	return -1;
}

