#include<func.h>

int main(int argc,char *argv[]) 
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    printf("fd=%d\n",fd);
    //int fd1=fd;//文件描述符的赋值,此时fd被关闭后，文件不能再被读取，会报错：Bad file descriptor
    int fd1=dup(fd);//文件描述符的复制,文件对象的引用计数变为2
    printf("fd1=%d\n",fd1);
    close(fd);
    char buf[128]={0};
    int ret=read(fd1,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read")
    printf("%s",buf);
    close(fd1);
    return 0;
}