#include<func.h>
//无名管道进行通信
int main()
{
    int fds[2];
    pipe(fds);
    char buf[128]={0};
    if(!fork())
    {
        close(fds[1]);//子进程读
        read(fds[0],buf,sizeof(buf));//管道没数据会阻塞
        printf("I am child,get=%s\n",buf);
        return 0;
    }
    else
    {
        close(fds[0]);//父进程写
        write(fds[1],"hello",5);
        wait(NULL);//等子进程关闭
        return 0;
    }
}