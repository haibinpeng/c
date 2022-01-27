#include<func.h>
//孤儿进程(父进程先于子进程退出)
int main()
{
    pid_t pid;
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        printf("I am child,pid=%d,ppid=%d\n",getpid(),getppid());
        while(1);
        return 0;
    }
    else
    {
        //执行父程序
        printf("I am father,mychildpid=%d,pid=%d,ppid=%d\n",pid,getpid(),getppid());
        sleep(1);
        return 0;
    }
}