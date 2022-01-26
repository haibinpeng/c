#include<func.h>
//fork创建子进程
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
        sleep(1);
        return 0;
    }
}