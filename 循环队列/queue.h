#include<stdio.h>

#define MaxSize 5
typedef int ElemType;
typedef struct SqQueue
{
	ElemType data[MaxSize];
	int front, rear;
}SqQueue;

void InitQueue(SqQueue* Q);
int isEmpty(SqQueue* Q);
int EnQueue(SqQueue* Q, ElemType x);
int DeQueue(SqQueue* Q, ElemType* x);