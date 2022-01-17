#include"mysort.h"

void print(int *a)
{
	int i;
	for (i = 0; i < N; i++)
	{
		printf("%3d", a[i]);
	}
	putchar('\n');
}

//冒泡排序
void bubble(int* a)
{
	int i, j, flag;
	for (i = N - 1;i > 0; i--)
	{
		flag = 0;
		for (j = 0; j < i ; j++)
		{
			if (a[j] > a[j + 1])
			{
				SWAP(a[j], a[j + 1]);
				flag = 1;
			}
		}
		if (flag == 0)
		{
			break;
		}
	}
}

//选择排序
void select(int* a)
{
	int i, j, pos;
	for (i = 0; i < N - 1; i++)
	{
		pos = i;
		for (j = i + 1; j < N; j++)
		{
			if (a[j] < a[pos])
			{
				pos = j;
			}
		}
		SWAP(a[i], a[pos]);
	}
}

//插入排序
void insert(int* a)
{
	int i, j, val;
	for (i = 1; i < N; i++)
	{
		val = a[i];
		for (j=i-1;i>=0;j--)
		{
			if (a[j] > val)
			{
				a[j + 1] = a[j];
			}
			else
			{
				break;
			}
		}
		a[j + 1] = val;
	}
}

//快速排序
int partition(int* a, int left, int right)
{
	//交换法,选取最右边为分隔值最简单
	int i;
	int j=left;//j始终指向比分隔值小的值要插入的位置
	for (i=left; i < right; i++)
	{
		if (a[i] < a[right])
		{
			SWAP(a[i],a[j]);
			j++;
		}
	}
	SWAP(a[j], a[right]);
	return j;
}
void quicksort(int* a,int left,int right)
{
	int pivot;
	if (left < right)
	{
		pivot = partition(a, left, right);
		quicksort(a, left, pivot - 1);
		quicksort(a, pivot + 1, right);
	}
}