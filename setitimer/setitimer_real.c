#include<func.h>
//真实计时器,计算的是程序运行的实际时间

void(sigfunc)(int signum)
{
    time_t now=time(NULL);
    printf("当前时间是%s",ctime(&now));
}

int main()
{
    signal(SIGALRM,sigfunc);//重新注册ALRM信号
    struct itimerval t;
    bzero(&t, sizeof(t));
    t.it_value.tv_sec=3;//初始间隔设置为3秒
    t.it_interval.tv_sec=2;//重复间隔设置为2秒
    int ret;
    sigfunc(0);//timer启动之前先显示一下当前时间
    ret=setitimer(ITIMER_REAL,&t,NULL);
    ERROR_CHECK(ret,-1,"setitimer");
    char buf[128]={0};
    read(STDIN_FILENO,buf,sizeof(buf));//用read使程序卡住，模拟程序睡觉，因为sleep用的也是ALRM信号，所以一般不一起用
    printf("buf=%s\n",buf);
    return 0;
}