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
    act.sa_flags=SA_SIGINFO|SA_RESTART;//使用了SA_RESTART标志后，卡在系统调用时，信号来了，系统调用不会出错
    int ret=sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction")
    char buf[128]={0};
    ret=read(STDIN_FILENO,buf,sizeof(buf));//用read读取标准输入
    printf("ret=%d,buf=%s",ret,buf);
    return 0;
}