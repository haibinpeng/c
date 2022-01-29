#include <func.h>
///获取进程组id
int main()
{
    if(!fork())
    {
        printf("I am child,pid=%d,ppid=%d,pgid=%d,sid=%d\n",\
        getpid(),getppid(),getpgid(0),getsid(0));
        return 0;
    }
    else
    {
        printf("I am parent,pid=%d,ppid=%d,pgid=%d,sid=%d\n",\
        getpid(),getppid(),getpgid(0),getsid(0));
        return 0;
    }
}