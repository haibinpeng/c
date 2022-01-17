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
		//���
		(*rail)->next = que;
		*rail = que;
		//����
		if (NULL == (*head)->pos->lchild)
		{
			(*head)->pos->lchild = p;
		}
		else if (NULL == (*rail)->pos->rchild)
		{
			(*head)->pos->rchild = p;
			//����
			q = *head;
			*head = q->next;
			free(q);
			q = NULL;
		}
	}
}
//���������������ȱ���
void preOrder(Bitree bt)
{
	if (bt)
	{
		putchar(bt->data);
		preOrder(bt->lchild);
		preOrder(bt->rchild);
	}
}
//�������
void inOrder(Bitree bt)
{
	if (bt)
	{
		inOrder(bt->lchild);
		putchar(bt->data);
		inOrder(bt->rchild);
	}
}
//�������
void postOrder(Bitree bt)
{
	if (bt)
	{
		postOrder(bt->lchild);
		postOrder(bt->rchild);
		putchar(bt->data);
	}
}