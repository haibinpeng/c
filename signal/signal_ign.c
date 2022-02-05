#include<func.h>
//用函数signal注册一个信号捕捉函数

int main()
{
    if(SIG_ERR==signal(SIGINT, SIG_IGN))//提前设定好2号信号的信号处理行为
    {
        printf("signal failed\n");
        return -1;
    }
    sleep(10);
    signal(SIGINT, SIG_DFL);//回归默认
    while(1);
    return 0;
}