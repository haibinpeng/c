#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

void min(int a[],int n,int *x,int *y)
{
	int min = 1000;
	int i,j;
	for (i = 0 ; i < n-1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (min > abs(a[i] - a[j]))
			{
				min = abs(a[i] - a[j]);
				*x = i;
				*y = j;
			}
		}
	}
}
int main()
{
	int a[10];
	int x, y;
	int i;
	for (i = 0; i < 10; i++)
	{
		scanf("%d", &a[i]);
	}
	min(a, 10, &x, &y);
	printf("%d %d\n", a[x], a[y]);
	return 0;
}