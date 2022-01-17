#include<stdio.h>

int main()
{
	int a[8] = { 1,2,1,3,1,1,1,2 };
	int candidate = a[0];
	int vote=1;
	int i;
	for (i = 1; i < 8; i++)
	{
		if (a[i] == candidate)
		{
			vote++;
		}
		else
		{
			vote--;
			if (vote < 0)
			{
				candidate = a[i];
				vote = 1;
			}
		}
	}
	printf("%d", candidate);
	return 0;
}