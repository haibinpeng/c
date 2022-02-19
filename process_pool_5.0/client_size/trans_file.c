#include "process_pool.h"

int recv_n(int sfd,void* buf,int data_len)
{
    //循环接收数据
    char *p=(char *)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        if(0==ret)//服务器端断开
        {
            return -1;
        }
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