#include "process_pool.h"
//客户端接收文件

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    //socket
    int sfd;//定义一个网络描述符
    sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络描述符，在内核中对应一个缓冲区
    ERROR_CHECK(sfd,-1,"socket")
    printf("sfd=%d\n",sfd);
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family = AF_INET;//IPV4通信
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);//把ip的点分十进制转换为网络字节序
    ser_addr.sin_port=htons(atoi(argv[2]));//把端口号转为网络字节序
    //connect,客户端去连接服务器
    int ret=connect(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"connect")
    //客户端接收文件
    //接收文件名
    int data_len;
    char buf[1000]={0};
    ret=recv_n(sfd,&data_len,4);//接文件名长度
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    ret=recv_n(sfd,buf,data_len);//接文件名,此时文件名就存在了buf里
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    int fd=open(buf,O_WRONLY|O_CREAT,0666);//根据buf里的文件名创建并打开文件
    ERROR_CHECK(fd,-1,"open")
    //接文件大小
    off_t file_size,download_size=0,last_size=0;//download_size为当前已经下载的大小，last_size为上一次打印时的大小
    ret=recv_n(sfd,&data_len,4);
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    ret=recv_n(sfd,&file_size,data_len);
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    off_t slice_size=file_size/10000;//文件大小的0.01%
    //接收文件内容
    while(1)
    {
        ret=recv_n(sfd,&data_len,4);//接文件内容长度
        SELFFUNC_ERR_CHECK(ret,"recv_n")
        if(data_len>0)
        {
            ret=recv_n(sfd,buf,data_len);//接文件内容
            SELFFUNC_ERR_CHECK(ret,"recv_n")
            write(fd,buf,data_len);//把文件内容写到文件里
            download_size+=data_len;
            if(download_size-last_size>slice_size)//每0.01%打印一次
            {
                printf("%6.2lf%%\r",(double)download_size/file_size*100);
                fflush(stdout);
                last_size=download_size;
            }
        }else{
            printf("100.00%%\n");
            break;
        }
    }
    close(sfd);
    return 0;
}