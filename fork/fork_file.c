#include<func.h>
//fork创建子进程,文件的共享情况(dup机制)
int main()
{
    pid_t pid;
    int fd=open("file",O_RDWR);
    char buf[128]={0};
    pid=fork();
    if(0==pid)
    {
        //执行子程序
        read(fd,buf,5);
        printf("I am child,%s\n",buf);
    }
    else
    {
        //执行父程序
        read(fd,buf,5);
        printf("I am father,%s\n",buf);
        sleep(1);
        return 0;
    }
}