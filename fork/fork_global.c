#include<func.h>
//fork创建子进程,全局变量的共享情况
int num=10;
int main()
{
    pid_t pid;
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        printf("I am child,num=%d\n",num);
        return 0;
    }
    else
    {
        //执行父程序
        printf("I am father,num=%d\n",num);
        num=5;
        printf("I am father,num=%d\n",num);
        sleep(1);
        return 0;
    }
}