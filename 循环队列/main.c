#include"queue.h"

int main()
{
	SqQueue Q;
	ElemType x;
	InitQueue(&Q);
	if (isEmpty(&Q))
	{
		printf("queue is empty\n");
	}
	EnQueue(&Q, 1);
	EnQueue(&Q, 2);
	EnQueue(&Q, 3);
	EnQueue(&Q, 4);
	EnQueue(&Q, 5);
	DeQueue(&Q, &x);
	printf("x=%d\n", x);
	return 0;
}