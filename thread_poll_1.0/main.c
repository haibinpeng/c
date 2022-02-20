#include "factory.h"

int main(int argc, char **argv)
{
    if(argc!=5)
    {
        printf("./thread_poll_server IP PORT THREAD_NUM CAPACITY\n");
        return -1;
    }
    Factory_t f;//主数据结构
    int thread_num=atoi(argv[3]);
    int capacity=atoi(argv[4]);
    factory_init(&f,thread_num,capacity);
    factory_start(&f,thread_num);
    int socket_fd;
    tcp_init(&socket_fd,argv[1],argv[2]);
    //接收请求，加锁，放入队列，解锁，signal
    int new_fd;
    struct sockaddr_in client_addr;
    pNode_t pnew;
    pQue_t pq=&f.que;//让一个队列指针指向队列
    while(1)
    {
        bzero(&client_addr,sizeof(client_addr));
        socklen_t addr_len=sizeof(client_addr);
        new_fd=accept(socket_fd,(struct sockaddr *)&client_addr,&addr_len);
        printf("%s %d is connected\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        pnew=(pNode_t)calloc(1,sizeof(Node_t));
        pnew->new_fd=new_fd;
        pthread_mutex_lock(&pq->mutex);//加锁
        que_set(pq,pnew);//结点放进队列
        pthread_mutex_unlock(&pq->mutex);//解锁
        pthread_cond_signal(&f.cond);//使条件成立
    }
    return 0;
}