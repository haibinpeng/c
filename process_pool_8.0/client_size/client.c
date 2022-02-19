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
    //统计下载时间
    struct timeval start,end;
    gettimeofday(&start,NULL);
    //接收文件名
    int data_len;
    char buf[1000]={0};
    ret=recv_n(sfd,&data_len,4);//接文件名长度
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    ret=recv_n(sfd,buf,data_len);//接文件名,此时文件名就存在了buf里
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    int fd=open(buf,O_RDWR|O_CREAT,0666);//根据buf里的文件名创建并打开文件
    ERROR_CHECK(fd,-1,"open")
    //接文件大小
    ret=recv_n(sfd,&data_len,4);//火车头
    SELFFUNC_ERR_CHECK(ret,"recv_n")
    off_t file_size;//file_size是文件大小
    ret=recv_n(sfd,&file_size,data_len);//火车信息
    SELFFUNC_ERR_CHECK(ret,"recv_n");
    //接收文件内容,使用splice
    int pipefd[2];
    pipe(pipefd);
    int splice_bytes;
    off_t total=0;
    while(total<file_size)
    {
        splice_bytes=splice(sfd,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);//从sfd里面读了写到管道里
        ret=splice(pipefd[0],NULL,fd,NULL,splice_bytes,SPLICE_F_MORE|SPLICE_F_MOVE);//再从管道写到fd
        total+=ret;
    }
    gettimeofday(&end,NULL);
    printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    close(sfd);
    return 0;
}