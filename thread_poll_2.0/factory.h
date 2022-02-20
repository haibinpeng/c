#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"

typedef struct{
    Que_t que;//队列
    pthread_cond_t cond;//条件变量
    pthread_t *pthid;//存放线程id
    short start_flag;//线程池启动标志，默认为0，启动后设置为1
}Factory_t;

#define FILENAME "file"

//不要交换data_len和buf的定义顺序
typedef struct{
    int data_len;//存放buf，也就是火车车厢中数据长度
    char buf[1000];//火车车厢长度
}train_t;

//线程池初始化
int factory_init(Factory_t *pf,int thread_num,int capacity);
//启动线程池
int factory_start(Factory_t *pf,int thread_num);
//子线程流程
void* thread_handle(void *p);
//socket,bind,listen操作
int tcp_init(int* socket_fd,char* ip,char* port);
//发送文件
int tran_file(int new_fd);
//循环发送数据，直到发送到自己想要的数量
int send_n(int sfd,void* buf,int data_len);
//epoll注册
int epoll_add(int epfd,int fd);

#endif