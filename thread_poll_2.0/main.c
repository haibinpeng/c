#include "factory.h"

int exit_fds[2];
//退出机制信号处理函数
void sig_exit_func(int signum)
{
    write(exit_fds[1],&signum,1);
}

int main(int argc, char **argv)
{
    if(argc!=5)
    {
        printf("./thread_poll_server IP PORT THREAD_NUM CAPACITY\n");
        return -1;
    }
    pipe(exit_fds);
    if(fork())
    {
        //父进程,用来协助完成退出机制
        close(exit_fds[0]);
        signal(SIGUSR1,sig_exit_func);
        wait(NULL);
        return 0;
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
    ////epoll注册监控socket_fd,exit_fd[0]
    int epfd=epoll_create(1);
    struct epoll_event evs[2];
    epoll_add(epfd,exit_fds[0]);
    epoll_add(epfd,socket_fd);
    int ready_fd_num,i,j;
    while(1)
    {
        ready_fd_num=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<ready_fd_num;i++)
        {
            if(evs[i].data.fd==socket_fd)
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
            if(evs[i].data.fd==exit_fds[0])//线程池要退出了
            {
                for(j=0;j<thread_num;j++)//给每一个子线程发送cancel信号
                {
                    pthread_cancel(f.pthid[j]);
                    printf("cancel success\n");
                }
                for(j=0;j<thread_num;j++)//等待每一个子线程
                {
                    pthread_join(f.pthid[j],NULL);
                }
                exit(0);
            }
        }
    }
    return 0;
}