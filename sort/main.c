#include "sort.h"

#define SORT_MAX	10


void arr_travel(int *arr, int num)
{
	int i = 0;
	for (i = 0; i < num; i++) {
		printf("%d  ", *(arr + i));
	}

	printf("\n");
}

int main(void)
{
	int data[SORT_MAX] = { 0 };
	int num = 0;
	int i = 0;

	for (i = 0; i < SORT_MAX; i++) {
		data[i] = rand() % 100;
	}

	arr_travel(data, SORT_MAX);

	sort_1(data, SORT_MAX);

	arr_travel(data, SORT_MAX);

	return 0;
}

