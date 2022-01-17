#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int path_num(int i, int j)
{
	if (i == 0 || j == 0)
	{
		return 1;
	}
	else
	{
		return path_num(i, j - 1) + path_num(i - 1, j);
	}
}

int main()
{
	int m, n, num;
	scanf("%d,%d", &m, &n);
	num = path_num(m, n);
	printf("到达坐标（%d,%d）的路径数有%d种\n", m, n, num);
	return 0;
}