#include <func.h>

void Daemon()
{
    if(fork())
    {
        exit(0);//创建子进程，父进程退出
    }
    setsid();//成立新会话
    chdir("/");//改变路径到根目录
    umask(0);//修改文件掩码
    int i;
    for(i=0;i<3;i++)
    {
        close(i);
    }
}
int main()
{
    Daemon();
    while(1)
    {
        sleep(1);
    }
    return 0;
}