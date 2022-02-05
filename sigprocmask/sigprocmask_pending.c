#include<func.h>

int main()
{
    sigset_t mask;
    //清空mask并填入2号信号到mask中
    sigemptyset(&mask);//清空信号掩码集合
    sigaddset(&mask, SIGINT);
    int ret=sigprocmask(SIG_BLOCK, &mask, NULL);
    ERROR_CHECK(ret,-1,"sigprocmask")
    printf("关键代码开始\n");
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
    printf("关键代码结束\n");
    //解除2号信号的阻塞
    ret=sigprocmask(SIG_UNBLOCK, &mask, NULL);
    ERROR_CHECK(ret,-1,"sigprocmask1")
    return 0;
}