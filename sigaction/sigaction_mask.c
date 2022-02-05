#include<func.h>

void sigfunc(int signum,siginfo_t *p,void *p1)
{//新的信号处理函数
    printf("before sleep,sig %d is coming\n",signum);
    sleep(3);//模拟信号处理函数执行一段时间
    printf("aftter sleep,sig %d is coming\n",signum);
}

int main()
{
    struct sigaction act;
    bzero(&act, sizeof(act));//一定要清空
    act.sa_sigaction=sigfunc;//函数指针，把信号处理函数赋给sa_sigaction
    act.sa_flags=SA_SIGINFO;
    sigemptyset(&act.sa_mask);//清空掩码集合
    sigaddset(&act.sa_mask,SIGQUIT);//填入3号信号，2号信号处理流程中就不会被3号打断，会等2号信号执行完再响应3号信号
    int ret=sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction")
    while(1);
    return 0;
}