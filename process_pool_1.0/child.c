#include "process_pool.h"

int make_child(process_data_t *p_manage,int process_num)
{
    int i;
    pid_t pid;
    int fds[2];//与子进程之间的管道
    int ret;
    for(i=0;i<process_num;i++)//创建多个子进程
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//全双工管道
        pid=fork();
        if(0==pid)//子进程
        {
            close(fds[1]);//子进程关闭fds[1]
            child_handle(fds[0]);//一定要让子进程在child_handle内exit
        }
        close(fds[0]);//父进程关闭fds[0],通过fds[1]和子进程进行通信,和pipe的区别是fds[1]即可读也可写
        p_manage[i].pid=pid;
        p_manage[i].fd=fds[1];
        p_manage[i].busy=0;//默认子进程是非忙碌状态
#ifdef DEBUG
        printf("p_manage[%d].fd=%d\n",i,p_manage[i].fd);
#endif
    }
}

int child_handle(int fd)
{
    int new_fd,ret;//子进程要接收一个new_fd,通过new_fd给对应客户端传文件
    char flag=1;
    while(1)
    {
        read(fd,&new_fd,sizeof(new_fd));//接受任务
        printf("gets task,new_fd=%d\n",new_fd);
        ret=send(new_fd,"hello",5,0);
        printf("send data to client,ret=%d\n",ret);//给客户端发文件
        write(fd,&flag,sizeof(flag));//通知父进程任务完成
    }
}