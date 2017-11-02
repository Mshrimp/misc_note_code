
#include "sort.h"


// 基本思想：所有数据与该数据之后的所有数据进行比较，如果反序则交换
void sort(int *arr, int num)
{
	int i = 0, j = 0;

	for (i = 0; i < num; i++) {
		for (j = i + 1; j < num; j++) {
			if (arr[i] > arr[j]) {
				swap(&arr[i], &arr[j]);
			}
		}
	}
}

// 冒泡排序：
// 基本思想:两两比较相邻记录的关键字,如果反序则交换
void sort_bubble(int *arr, int num)
{
	int i = 0, j = 0;

	for (i = 0; i < num; i++) {
		for (j = 1; j < num - i; j++) {
			if (arr[j-1] > arr[j]) {
				swap(&arr[j-1], &arr[j]);
			}
		}
	}
}


void sort_fast(int *arr, int num)
{
	int i = 0, j = 0;

}


void swap(int *a, int *b)
{
	int temp = 0;

	temp = *a;
	*a = *b;
	*b = temp;
}

void swap_1(int *a, int *b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void swap_2(int *a, int *b)
{
	*a = *a * *b;
	*b = *a / *b;
	*a = *a / *b;
}

void swap_3(int *a, int *b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

