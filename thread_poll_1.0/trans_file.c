#include "factory.h"

int tran_file(int new_fd)
{
    int fd=open(FILENAME,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    //发送文件名
    int ret;
    train_t t;
    t.data_len=strlen(FILENAME);//文件名长度装在火车头
    strcpy(t.buf,FILENAME);
    ret=send_n(new_fd,&t,4+t.data_len);
    SELFFUNC_ERR_CHECK(ret,"send_n");
    //发送文件大小给客户端
    struct stat buf;
    ret=stat(FILENAME,&buf);
    ERROR_CHECK(ret,-1,"stat");
    t.data_len=sizeof(buf.st_size);
    memcpy(t.buf,&buf.st_size,sizeof(buf.st_size));
    ret=send_n(new_fd,&t,4+t.data_len);
    SELFFUNC_ERR_CHECK(ret,"send_n");
    //发送文件内容
    while(t.data_len=read(fd,t.buf,sizeof(t.buf)))
    {
        ret=send_n(new_fd,&t,4+t.data_len);
        SELFFUNC_ERR_CHECK(ret,"send_n");
    }
    t.data_len=0;
    ret=send_n(new_fd,&t,4+t.data_len);//文件内容发送完成标志
    SELFFUNC_ERR_CHECK(ret,"send_n");
    return 0;
}
//循环接收数据，直到接收到自己想要数量
int recv_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}

//循环发送数据，直到发送到自己想要数量
int send_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=send(sfd,p+total,data_len-total,0);
        if(-1==ret)//代表对端断开了
        {
            return -1;
        }
        total+=ret;
    }
    return 0;
}