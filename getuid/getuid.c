#include <func.h>

int main()
{
    printf("uid:%d,gpid:%d,euid:%d,egid:%d\n",getuid(),getgid(),geteuid(),getegid());
    return 0;
}