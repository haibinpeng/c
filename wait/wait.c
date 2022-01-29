#include<func.h>

int main()
{
    pid_t pid;
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        printf("I am child,pid=%d,ppid=%d\n",getpid(),getppid());
        exit(3);
    }
    else
    {
        //执行父程序
        printf("I am father,mychildpid=%d,pid=%d,ppid=%d\n",pid,getpid(),getppid());
        int status;
        pid=wait(&status);//子进程结束后，会给父进程发送SIGCHLD信号，唤醒父进程,回收资源
        if(WIFEXITED(status))//如果子进程正常退出，WIFEXITED(status)是一个非零值；WIFEXITED是一个宏，传入整型值，而不是地址
        {
            printf("child exit code=%d\n",WEXITSTATUS(status));//WEXITSTATUS(status)返回子进程的退出码
        }
        else
        {
            printf("child crash\n");
        }
        printf("wait pid=%d\n",pid);
        return 0;
    }
}