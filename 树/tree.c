#include"tree.h"

void buildBinaryTree(Bitree* bt, pqueue* head, pqueue* rail,ElemType val)
{
	Bitree p = (Bitree)calloc(1, sizeof(BtNode));
	p->data = val;
	pqueue que = (pqueue)calloc(1, sizeof(queue));
	que->pos = p;
	pqueue q;
	if (NULL == *bt)
	{
		*bt = p;
		*head = que;
		*rail = que;
	}
	else
	{
		//入队
		(*rail)->next = que;
		*rail = que;
		//建树
		if (NULL == (*head)->pos->lchild)
		{
			(*head)->pos->lchild = p;
		}
		else if (NULL == (*rail)->pos->rchild)
		{
			(*head)->pos->rchild = p;
			//出队
			q = *head;
			*head = q->next;
			free(q);
			q = NULL;
		}
	}
}
//先序遍历，深度优先遍历
void preOrder(Bitree bt)
{
	if (bt)
	{
		putchar(bt->data);
		preOrder(bt->lchild);
		preOrder(bt->rchild);
	}
}
//中序遍历
void inOrder(Bitree bt)
{
	if (bt)
	{
		inOrder(bt->lchild);
		putchar(bt->data);
		inOrder(bt->rchild);
	}
}
//后序遍历
void postOrder(Bitree bt)
{
	if (bt)
	{
		postOrder(bt->lchild);
		postOrder(bt->rchild);
		putchar(bt->data);
	}
}