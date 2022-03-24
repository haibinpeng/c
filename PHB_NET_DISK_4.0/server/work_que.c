#include "work_que.h"

void que_init(pQue_t pq,int que_capacity)
{
    memset(pq,0,sizeof(Que_t));//队列头，尾为NULL，size设置为零
    pq->que_capacity=que_capacity;
    pthread_mutex_init(&pq->mutex,NULL);
}

void que_set(pQue_t pq,pNode_t pnew)
{
    if(NULL==pq->que_head)//队列为空
    {
        pq->que_head=pq->que_tail=pnew;
    }else{
        //尾插法
        pq->que_tail->pNext=pnew;
        pq->que_tail=pnew;
    }
    pq->que_size++;
}

void que_get(pQue_t pq,pNode_t *p)
{
    //拿的时候要不要判断队列为空,要判断
    if(pq->que_size)
    {
        *p=pq->que_head;
        pq->que_head=pq->que_head->pNext;
        if(NULL==pq->que_head)
        {
            pq->que_tail=NULL;
        }
        pq->que_size--;
    }
}

void search_que(pQue_t pq,int new_fd,pNode_t pnew)
{
    pNode_t p= pq->que_head;
    while(p->new_fd!=new_fd&&p!=NULL)
    {
        p=p->pNext;
    }
    if(p==NULL)
    {
        printf("add user");
        que_set(pq,pnew);
    }
    else{
        printf("user exists");
    }
}