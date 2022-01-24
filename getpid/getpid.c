#include <func.h>

int main()
{
    printf("pid:%d,ppid:%d\n",getpid(),getppid());
    while(1);
    return 0;
}