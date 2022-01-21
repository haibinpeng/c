#include<func.h>

int main(int argc, char *argv[])
{//改变文件大小,高速写磁盘时会用到
    ARGS_CHECK(argc,3);
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret=0;
    ret=ftruncate(fd,atoi(argv[2]));//atoi将字符串转int
    ERROR_CHECK(ret,-1,"fturncate");
    return 0;
}