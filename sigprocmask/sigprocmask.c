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
    sleep(10);
    printf("关键代码结束\n");
    //解除2号信号的阻塞
    ret=sigprocmask(SIG_UNBLOCK, &mask, NULL);
    ERROR_CHECK(ret,-1,"sigprocmask1")
    return 0;
}