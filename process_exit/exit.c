#include<func.h>
//exit从子函数让进程结束
int print()
{
    printf("I am print func\n");
    exit(-1);
    //return 0;
}
int main()
{
    print();
    printf("I am main func\n");
}