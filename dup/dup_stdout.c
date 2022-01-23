#include<func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    printf("\n");//刷新标准输出
    close(STDOUT_FILENO);
    int fd1=dup(fd);//dup复制放置的位置，是没有使用的文件描述符的最小编号
    printf("fd1=%d\n",fd1);//printf就是write 1，printf的原理就是将内容输入到描述符为1的文件里面
    close(fd);
    printf("the out of stdout");
    return 0;
}