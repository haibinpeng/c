#include<stdio.h>

int main()
{
	int max1, max2;
	int i;
	int a[7] = { 13,2,5,7,4,3,9 };
	max1 = a[0] > a[1] ? a[0] : a[1];
	max2 = a[0] < a[1] ? a[0] : a[1];
	for (i = 2; i < 7; i++)
	{
		if (a[i] > max1)
		{
			max2 = max1;
			max1 = a[i];
		}
		else if (a[i] > max2)
		{
			max2 = a[i];
		}
	}
	printf("最大值=%d,次大值=%d", max1, max2);
	return 0;
}