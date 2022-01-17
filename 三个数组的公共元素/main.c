#include<stdio.h>

int compare(int A[], int n1, int B[], int n2, int C[])
{
	int i=0, j=0;
	int count = 0;
	while (i < n1 && j < n2)
	{
		while (A[i] < B[j])
		{
			i++;
		}
		while (A[i] > B[j])
		{
			j++;
		}
		while (A[i] == B[j])
		{
			C[count]=A[i];
			i++;
			j++;
			count++;
		}
	}
	return count;
}

//int main()//打印两个有序数组的公共元素
//{
//	int A[6] = { 1,2,3,5,7,9 };
//	int B[7] = { 1,3,5,8,9,10,11 };
//	int C[10];
//	int count=compare(A, 6, B, 7,C);
//	for (int i = 0; i < count; i++)
//	{
//		printf("%-2d", C[i]);
//	}
//	printf("\n");
//	printf("%d", count);
//	return 0;
//}

int main()//打印三个有序数组的公共元素
{
	int A[5] = { 1,2,3,4,5 };
	int B[7] = { 1,3,5,7,9,11,13 };
	int C[4] = { 3,5,8,10 };
	int D[10];
	int E[10];
	int count1 = compare(A, 5, B, 7, D);
	int count2 = compare(C, 4, D, count1, E);
	for (int i = 0; i < count2; i++)
	{
		printf("%-2d", C[i]);
	}
	printf("\n");
	printf("%d", count2);
	return 0;
}