#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"

typedef struct tag_node{
    int new_fd;
    char filename[101];//文件名，MD5
    char getname[101];//用户输入的文件名
    int code;//code为0代表是根，code为正，code是当前路径
    int state;//state是GETS,或者PUTS
    int user_id;//哪一个用户发过来的gets或者puts请求
    off_t offset;//为GETS服务，传递的偏移值
    struct tag_node *pNext;
}Node_t,*pNode_t;

typedef struct{
    pNode_t que_head,que_tail;//队列头队列尾
    int que_capacity;//队列能力，可以计算当前服务器的负载
    int que_size;//当前队列大小
    pthread_mutex_t mutex;
}Que_t,*pQue_t;

void que_init(pQue_t pq,int que_capacity);
//放元素到队列
void que_set(pQue_t pq,pNode_t pnew);
//拿队列元素
void que_get(pQue_t pq,pNode_t *p);

#endif