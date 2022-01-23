#include<func.h>
//读进程，负责读管道
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    printf("I am reader\n");
    //读端读
    char buf[128];
    read(fd,buf,sizeof(buf));//管道没数据就会阻塞；写端关闭时不会阻塞，对面断开后会得到一个返回值0，从而得知已经断开
    printf("reader=%s\n",buf);
    return 0;
}