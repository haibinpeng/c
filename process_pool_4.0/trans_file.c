#include "process_pool.h"

int trans_file(int new_fd)
{
    //发送文件名
    train_t t;
    t.data_len=strlen(FILENAME);//文件名长度
    strcpy(t.buf,FILENAME);//文件名
    send_n(new_fd,&t,4+t.data_len);
    //发送文件内容
    int fd=open(FILENAME,O_RDONLY);
    ERROR_CHECK(fd,-1,"open")
    while(t.data_len=read(fd,t.buf,sizeof(t.buf)))
    {
        send_n(new_fd,&t,4+t.data_len);
    }
    t.data_len=0;
    send_n(new_fd,&t,4+t.data_len);//发送文件内容发送完毕的标志
}

int recv_n(int sfd,void* buf,int data_len)
{
    //循环接收数据
    char *p=(char *)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}

int send_n(int sfd,void* buf,int data_len)
{
    //循环发送数据
    char *p=(char *)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=send(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}