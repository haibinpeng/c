#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"

typedef struct tag_node{
    int new_fd;
    struct tag_node *pNext;
}Node_t,*pNode_t;

typedef struct{
    pNode_t que_head,que_tail;//队列头、队列尾
    int que_capacity;//队列能力，可以计算当前服务器的负载
    int que_size;//当前队列大小
    pthread_mutex_t mutex;//锁
}Que_t,*pQue_t;

//初始化队列
void que_init(pQue_t pq,int que_capacity);
//放元素到队列
void que_set(pQue_t pq,pNode_t pnew_node);
//拿队列元素
void que_get(pQue_t pq,pNode_t *p);

#endif