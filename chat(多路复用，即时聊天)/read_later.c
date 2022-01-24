#include<func.h>
//先定向标准输入输入数据，5s后再读
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    sleep(5);
    char buf[128]={0};
    int ret=read(STDIN_FILENO,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read")
    printf("%s\n",buf);
    return 0;
}