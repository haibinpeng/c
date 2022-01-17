#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main()
{
	int i;
	int sum = 0;
	int a[1001];
	for (i = 0; i < 1001; i++)
	{
		scanf("%d", a[i]);
	}
	for (i = 0; i < 1001; i++)
	{
		sum = sum + a[i] - i;
	}
	printf("%d", sum);
}