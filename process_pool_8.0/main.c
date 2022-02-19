#include "process_pool.h"

//退出机制
int exit_pipefd[2];
void sig_exit_func(int signum)
{
    write(exit_pipefd[1],&signum,1);//异步拉起同步
}

int main(int argc, char **argv)
{
    if(argc!=4)
    {
        printf("process_pool_server IP PORT PROCESS_NUM\n");
        return -1;
    }
    int process_num=atoi(argv[3]);
    //p_manage用来管理所有的子进程信息
    process_data_t *p_manage=(process_data_t *)calloc(process_num, sizeof(process_data_t));
    make_child(p_manage, process_num);
    int socket_fd;
    tcp_init(&socket_fd,argv[1],argv[2]);
    printf("socket_fd=%d\n",socket_fd);
    //初始化epoll，父进程既要监控socke_fd,还要监控每一个子进程的管道对端
    int epfd=epoll_create(1);
    int ret=epoll_add(epfd,socket_fd);//注册socke_fd
    SELFFUNC_ERR_CHECK(ret,"epoll_add")
    int i;
    for(i=0;i<process_num;i++)
    {
        //注册每一个子进程的管道对端
        ret=epoll_add(epfd,p_manage[i].fd);
        SELFFUNC_ERR_CHECK(ret,"epoll_add")
    }
    //退出准备
    pipe(exit_pipefd);
    signal(SIGINT,sig_exit_func);
    ret=epoll_add(epfd,exit_pipefd[0]);
    SELFFUNC_ERR_CHECK(ret,"epoll_add");
    //退出准备
    //父进程等待是否有客户端连接
    int new_fd,j;
    int read_fd_num;//就绪的描述符数量
    struct epoll_event *evs=(struct epoll_event *)calloc(process_num+2,sizeof(struct epoll_event));
    struct sockaddr_in client_addr;
    char flag;
    while(1)
    {
        read_fd_num=epoll_wait(epfd,evs,process_num+2,-1);
        for(i=0;i<read_fd_num;i++)
        {
            if(evs[i].data.fd==socket_fd)
            {
                //有客户端连接，accept，得到new_fd,将new_fd传递给非忙碌的子进程，并标记为忙碌
                bzero(&client_addr,sizeof(client_addr));
                int addr_len = sizeof(client_addr);
                new_fd=accept(socket_fd,(struct sockaddr *)&client_addr,&addr_len);//accept，得到new_fd
                printf("%s %d is connected\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                for(j=0;j<process_num;j++)
                {
                    //找到非忙碌的子进程,发送任务
                    if(0==p_manage[j].busy)
                    {
                        send_fd(p_manage[j].fd,new_fd,0);//发送任务
                        p_manage[j].busy=1;//对应子进程标记为忙碌
                        printf("child %d is busy\n",p_manage[j].pid);
                        break;
                    }
                }
                close(new_fd);//无论是否找到空闲的子进程，都关闭；找到了相当于引用计数-1
            }
            for(j=0;j<process_num;j++)
            {
                if(evs[i].data.fd==p_manage[j].fd)
                {
                    //如果收到子进程的通知，把对应子进程标记为非忙碌
                    read(p_manage[j].fd,&flag,sizeof(flag));//把子进程写的提示读出来，不然会被一直标记为可读状态
                    p_manage[j].busy=0;
                    printf("child %d is not busy\n",p_manage[j].pid);
                    break;
                }
            }
            if(evs[i].data.fd==exit_pipefd[0])//程序收到信号要退出了
            {
                for(j=0;j<process_num;j++)
                {
                    send_fd(p_manage[j].fd,0,1);//发退出通知给子进程
                }
                for(j=0;j<process_num;j++)
                {
                    wait(NULL);//回收每个子进程
                }
                close(socket_fd);
                return 0;
            }
        }
    }
    return 0;
}