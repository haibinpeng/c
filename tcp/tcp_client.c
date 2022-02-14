#include<func.h>

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
    //客户端发送数据
    ret=send(sfd,"hello",5,0);
    ERROR_CHECK(ret,-1,"send")
    //客户端接收数据
    char buf[128]={0};
    ret=recv(sfd,buf,sizeof(buf),0);
    ERROR_CHECK(ret,-1,"recv")
    printf("I am clent,gets=%s\n",buf);
    close(sfd);
    return 0;
}