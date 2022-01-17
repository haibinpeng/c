#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;
typedef struct BtNode
{
	ElemType data;
	struct BtNode* lchild;
	struct BtNode* rchild;
}BtNode,*Bitree;

typedef struct queue
{
	Bitree pos;
	struct queue* next;
}queue,*pqueue;

void buildBinaryTree(Bitree* bt, pqueue* head, pqueue* rail, ElemType val);
void preOrder(Bitree bt);
void inOrder(Bitree bt);
void postOrder(Bitree bt);