#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int m, n;//�ӣ�0��0������m,n)�ܹ�Ҫ��m+n��
int way[2][100];//��һ�д洢�����꣬�ڶ��д洢������
int path(int i, int j,int step)
{
	way[0][step] = i;
	way[1][step] = j;
	if (i == m && j == n)
	{
		for (int k = 0; k < step; k++)
		{
			printf("(%d,%d)", way[0][k], way[1][k]);
		}
		printf("(%d,%d)\n", m, n);
		return;
	}
	else if (i > m || j > n)
	{
		return;
	}
	else 
	{
		path(i+1,j,step+1);//������
		path(i,j+1,step+1);//������
	}
}
int main()
{
	scanf("%d,%d", &m, &n);
	path(0, 0, 0);
	return 0;
 }