#include"tree.h"

int main()
{
	ElemType val;
	Bitree bt = NULL;
	pqueue head = NULL, rail = NULL;
	while (scanf("%c", &val) != EOF)
	{
		if (val == '\n')
		{
			break;
		}
		buildBinaryTree(&bt, &head, &rail, val);
	}
	preOrder(bt);
	printf("\n--------------\n");
	inOrder(bt);
	printf("\n--------------\n");
	postOrder(bt);
	printf("\n--------------\n");
	return 0;
}