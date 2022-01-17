#include"queue.h"

//初始化队列
void InitQueue(SqQueue* Q)
{
	Q->front = Q->rear = 0;
}
//判断队是否为空
int isEmpty(SqQueue* Q)
{
	return Q->front == Q->rear;
}
//入队
int EnQueue(SqQueue* Q, ElemType x)
{
	if ((Q->rear + 1) % MaxSize == Q->front)
	{
		printf("queue is full\n");
		return -1;
	}
	Q->data[Q->rear] = x;
	Q->rear = (Q->rear + 1) % MaxSize;
	return 0;
}
//出队
int DeQueue(SqQueue* Q, ElemType* x)
{
	if (isEmpty(Q))
	{
		printf("queue is empty\n");
		return -1;
	}
	*x = Q->data[Q->front];
	Q->front = (Q->front + 1) % MaxSize;
	return 0;
}