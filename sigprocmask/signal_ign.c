#include<func.h>

int main()
{
    signal(SIGINT,SIG_IGN);
    sleep(5);
    sigset_t pending;
    sigemptyset(&pending);//清空，避免里面有脏数据
    sigpending(&pending);//拿出内核正在挂起的信号
    if(sigismember(&pending,SIGINT))
    {
        printf("SIGINT is pending\n");
    }else{
        printf("SIGINT is not pending\n");
    }
    printf("you can see me\n");
    return 0;
}