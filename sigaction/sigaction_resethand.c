#include<func.h>

void sigfunc(int signum,siginfo_t *p,void *p1)
{//新的信号处理函数
    printf("sig %d is coming\n",signum);
}

int main()
{
    struct sigaction act;
    bzero(&act, sizeof(act));//一定要清空
    act.sa_sigaction=sigfunc;//函数指针，把信号处理函数赋给sa_sigaction
    act.sa_flags=SA_SIGINFO|SA_RESETHAND;//SA_RESETHAND加上后，sa_sigaction信号处理函数只生效一次
    int ret=sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction")
    while(1);
    return 0;
}