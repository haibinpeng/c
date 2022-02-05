#include<func.h>
//用函数signal注册一个信号捕捉函数
void sigfunc(int signum)
{//sigfunc是自定义一个信号处理函数
    printf("sig %d is coming\n",signum);
}

int main()
{
    if(SIG_ERR==signal(SIGINT, sigfunc))//提前设定好2号信号的信号处理行为
    {
        printf("signal failed\n");
        return -1;
    }
    while(1);
    return 0;
}