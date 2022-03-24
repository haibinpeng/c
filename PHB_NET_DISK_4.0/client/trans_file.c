#include "func.h"

//循环接收数据，直到接收到自己想要数量
int recv_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        if(0==ret)//代表对端断开了
        {
            return -1;
        }
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
        total+=ret;
    }
    return 0;
}