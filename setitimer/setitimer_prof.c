#include<func.h>
//实用计时器,程序不运行时不计时

void(sigfunc)(int signum)
{
    time_t now=time(NULL);
    printf("当前时间是%s",ctime(&now));
}

int main()
{
    signal(SIGPROF,sigfunc);//重新注册ALRM信号
    struct itimerval t;
    bzero(&t, sizeof(t));
    t.it_value.tv_sec=3;//初始间隔设置为3秒
    t.it_interval.tv_sec=2;//重复间隔设置为2秒
    int ret;
    sigfunc(0);//timer启动之前先显示一下当前时间
    ret=setitimer(ITIMER_PROF,&t,NULL);
    ERROR_CHECK(ret,-1,"setitimer");
    sleep(3);//睡觉的3秒不计入初始间隔时间
    while(1);
    return 0;
}