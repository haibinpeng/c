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

int recv_n_show(int sfd,void* buf,int data_len)
{
    //显示接收数据进度的recv_n
    char *p=(char *)buf;
    int total=0,ret;
    off_t slice_size=data_len/10000;//文件大小的0.01%
    off_t last_size=0;//上一次的大小
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        if(0==ret)//服务器端断开
        {
            return -1;
        }
        total+=ret;
        if(total-last_size>slice_size)//打印进度
        {
            printf("%6.2lf%%\r",(double)total/data_len*100);
            fflush(stdout);
            last_size=total;
        }
    }
    printf("100.00%%\n");
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