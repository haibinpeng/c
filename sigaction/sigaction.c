#include<func.h>

void sigfunc(int signum,siginfo_t *p,void *p1)
{//新的信号处理函数
    printf("sig %d is coming\n",signum);
}

//sigaction基本使用，捕捉2号信号
int main()
{
    struct sigaction act;
    bzero(&act, sizeof(act));//一定要清空
    act.sa_sigaction=sigfunc;//函数指针，把信号处理函数赋给sa_sigaction
    act.sa_flags=SA_SIGINFO;//SA_SIGINFO加上后，sa_sigaction信号处理函数生效
    int ret=sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction")
    char buf[128]={0};
    ret=read(STDIN_FILENO,buf,sizeof(buf));//用read读取标准输入
    printf("ret=%d,buf=%s",ret,buf);
    return 0;
}