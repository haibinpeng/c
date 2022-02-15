#include<func.h>
//利用UDP实现即时聊天

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    int sfd=socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sfd,-1,"socket")
    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));//清空
    ser_addr.sin_family = AF_INET;//IPV4通信
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);//把ip的点分十进制转换为网络字节序
    ser_addr.sin_port=htons(atoi(argv[2]));//把端口号转为网络字节序
    int ret=bind(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"bind")
    char buf[128]={0};
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&addr_len);
    ERROR_CHECK(ret,-1,"recvfrom")
    printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    fd_set rdset;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret=select(sfd+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset))
        {
            //读取标准输入并发送给对方
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
            ERROR_CHECK(ret,-1,"sendto")
        }
        if(FD_ISSET(sfd,&rdset))
        {
            //sfd可读，对方发了数据，读取
            bzero(buf,sizeof(buf));
            ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&addr_len);
            ERROR_CHECK(ret,-1,"recvfrom")
            printf("%s\n",buf);
        }
    }
    close(sfd);
    return 0;
}