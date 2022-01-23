#include<func.h>
//写进程，负责写管道
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    int fd;
    fd=open(argv[1],O_WRONLY);
    ERROR_CHECK(fd,-1,"open");
    printf("I am writer\n");
    write(fd,"hello",5);//除写满的情况，写是不会阻塞的
    close(fd);
    return 0;
}