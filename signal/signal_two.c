#include<func.h>
//用函数signal注册一个信号捕捉函数
void sigfunc(int signum)
{//sigfunc是自定义一个信号处理函数
    printf("before sleep,sig %d is coming\n",signum);
    sleep(3);
    printf("aftter sleep,sig %d is coming\n",signum);
}

int main()
{
    signal(SIGINT, sigfunc);//提前设定好2号信号的信号处理行为
    signal(SIGQUIT, sigfunc);//提前设定好3号信号的信号处理行为
    while(1);
    return 0;
}