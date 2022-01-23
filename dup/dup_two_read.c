#include<func.h>

int main(int argc,char *argv[]) 
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    printf("fd=%d\n",fd);
    int fd1=dup(fd);//复制后只是引用计数变量变为2，其它像位置指针，基指针这些并没有变化
    printf("fd1=%d\n",fd1);
    char buf[128]={0};
    read(fd,buf,5);//先用fd读取五个字节
    printf("%s\n",buf);
    printf("--------------\n");
    memset(buf,0,sizeof(buf));
    read(fd1,buf,5);//再用fd1读取五个字节
    printf("%s\n",buf);
    close(fd);
    close(fd1);
    return 0;
}