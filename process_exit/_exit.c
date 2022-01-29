#include<func.h>
//_exit从子函数让进程结束
int print()
{
    printf("I am print func\n");
    printf("I am print func");//不加/n没有刷新标准输出，打印不出来，也可以用fflush(stdout)
    fflush(stdout);//主动刷新标准输出,刷新就是把缓存区的内容推到屏幕显示，\n也会
    _exit(-1);//系统调用
}
int main()
{
    print();
    printf("I am main func\n");
}