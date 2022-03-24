#include "factory.h"

//用户队列，初始化
void clientque_init(pcque clientque)
{
    clientque->que_head=clientque->que_tail=NULL;
    clientque->que_capacity=1000;
    clientque->que_size=0;
}

//用户队列，增
void clientque_insert(pcque clientque,pcnode pcnew)
{
    if(clientque->que_head==NULL)//当前队列为空
    {
        clientque->que_head=clientque->que_tail=pcnew;
    }
    else//当前不为空,头插
    {
        pcnew->next_client=clientque->que_head;
        clientque->que_head=pcnew;
    }
}

//用户队列，寻找当前username，若找到，返回0，该节点指针存入ptarget，没找到返回-1
int clientque_find(pcque clientque,char* username,pcnode ptarget)
{
    pcnode pc=clientque->que_head;
    while(pc!=NULL&&strcmp(pc->client.uname,username))//如果pc不为空且指向的位置的用户名不是当前用户名
    {   
        //pc向后找
        pc=pc->next_client;
    }
    if(pc==NULL)//如果是因为到最后了还没找到，返回-1
    {
        return -1;
    }
    else//如果是因为uname相同而停下来的，传出当前结点，返回0
    {
        ptarget=pc;
        return 0;
    }
}