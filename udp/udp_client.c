#include<func.h>

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
    int ret=sendto(sfd,"hello",5,0,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"sendto")
    char buf[128]={0};
    ret=recvfrom(sfd,buf,sizeof(buf),0,NULL,NULL);//已经连上了，因此填NULL也可以
    ERROR_CHECK(sfd,-1,"recvfrom")
    printf("%s\n",buf);
    close(sfd);
    return 0;
}