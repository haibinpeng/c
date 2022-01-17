#include "mysort.h"

int main()
{
	int i;
	int* a = (int*)malloc(N * sizeof(int));
	srand(time(NULL));//产生rand()随机序列种子
	for (i = 0; i < N; i++)
	{
		a[i] = rand() % 100;
	}
	print(a);
	//bubble(a);
	//select(a);
	//insert(a);
	quicksort(a, 0, N - 1);
	print(a);
	return 0;
}