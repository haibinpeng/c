#include<func.h>
//fork创建子进程,堆内存的共享情况
int main()
{
    pid_t pid;
    char *p=malloc(20);
    strcpy(p,"helloworld");
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        printf("I am child,%s\n",p);
        return 0;
    }
    else
    {
        //执行父程序
        printf("I am father,%s\n",p);
        strncpy(p,"HELLO",5);
        printf("I am father,%s\n",p);
        sleep(1);
        return 0;
    }
}