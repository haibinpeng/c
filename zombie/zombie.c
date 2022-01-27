#include<func.h>
//僵尸进程(子进程先退出，父进程在忙碌，未清理子进程的环境)
int main()
{
    pid_t pid;
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        printf("I am child,pid=%d,ppid=%d\n",getpid(),getppid());
        return 0;
    }
    else
    {
        //执行父程序
        printf("I am father,mychildpid=%d,pid=%d,ppid=%d\n",pid,getpid(),getppid());
        while(1);
        return 0;
    }
}