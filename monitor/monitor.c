#include "func.h"
//当服务器崩溃后，立即重新拉起

int main()
{
    while(1)
    {
        pid_t pid=fork();
        if(0==pid)
        {
            //子进程
            printf("I am task\n");
            while(1);//模拟task函数
            return 0;
        }else{
            int status;
            wait(&status);
            if(WIFEXITED(status))//如果子进程正常退出，WIFEXITED(status)是一个非零值
            {
                exit(0);
            }else{
                printf("child crash,start again\n");
            }
        }
    }
}