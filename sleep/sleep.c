#include <func.h>

void sigfunc(int signum)
{
    //除了唤醒，什么都不做
}
int main()
{
    signal(SIGALRM,sigfunc);//注册ALRM信号的信号处理行为
    alarm(3);//3s后向当前进程发送alarm信号
    pause();//将自身进程挂起（去睡眠队列），直到有信号发生时才从pause返回
    return 0;
}