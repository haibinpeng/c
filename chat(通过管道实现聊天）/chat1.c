#include<func.h>
//聊天1，先打开1号管道的读端，再打开2号管道的写端
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    int fdr,fdw;
    fdr=open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open1")
    fdw=open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open2")
    printf("I am chat1\n");
    //聊天
    char buf[128]={0};
    while(1)
    {
        //先讲话
        memset(buf,0,sizeof(buf));
        read(STDIN_FILENO,buf,sizeof(buf));//读标准输入(相当于scanf)，注意read会将\n也读掉
        write(fdw,buf,strlen(buf)-1);//-1是不读那个\n
        //再读对方发过来的数据
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        printf("get=%s\n",buf);
    }
    close(fdr);
    close(fdw);
    return 0;
}