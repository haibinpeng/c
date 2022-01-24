#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    int fdr,fdw;
    fdr=open(argv[1],O_RDONLY);//先打开1管道的读端
    ERROR_CHECK(fdr,-1,"open1")
    fdw=open(argv[2],O_WRONLY);//再打开2管道的写端
    ERROR_CHECK(fdw,-1,"open2")
    printf("I am chat1\n");
    //select辅助聊天
    char buf[128]={0};
    fd_set rdset;
    int ret;
    int ready_fdnum;
    while(1)
    {
        FD_ZERO(&rdset);//清空
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr,&rdset);
        ready_fdnum=select(fdr+1, &rdset, NULL, NULL, NULL);//rdset是传入传出参数
        ERROR_CHECK(ready_fdnum,-1,"select");
        if(FD_ISSET(STDIN_FILENO,&rdset))//标准输入有数据
        {
            memset(buf,0,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            if(0==ret)//按"CTRL D"可以结束聊天
            {
                printf("I have to leave\n");
                break;
            }
            write(fdw,buf,strlen(buf)-1);
        }
        if(FD_ISSET(fdr,&rdset))//fdr有数据
        {
            memset(buf,0,sizeof(buf));
            ret=read(fdr,buf,sizeof(buf));
            if(0==ret)//对端断开了
            {
                printf("byebye\n");
                break;
            }
            printf("%s\n",buf);
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}