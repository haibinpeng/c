#include <func.h>
//用execl启动进程
int main()
{
    execl("./add","add","3","4",NULL);
    printf("You can not see me\n");
    return 0;
}