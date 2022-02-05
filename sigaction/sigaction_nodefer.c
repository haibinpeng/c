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
    act.sa_flags=SA_SIGINFO|SA_NODEFER;//加上SA_NODEFER后相同信号也可以打断了
    int ret=sigaction(SIGINT, &act, NULL);//act是传入参数;注册2号信号
    ERROR_CHECK(ret,-1,"sigaction")
    ret=sigaction(SIGQUIT, &act, NULL);//注册3号信号
    ERROR_CHECK(ret,-1,"sigaction")
    while(1);
    return 0;
}