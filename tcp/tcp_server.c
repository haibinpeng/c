#include<func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    //socket
    int sfd;//定义一个网络描述符
    sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络描述符，在内核中对应一个缓冲区
    ERROR_CHECK(sfd,-1,"socket")
    printf("sfd=%d\n",sfd);
    //bind
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family = AF_INET;//IPV4通信
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);//把ip的点分十进制转换为网络字节序
    ser_addr.sin_port=htons(atoi(argv[2]));//把端口号转为网络字节序
    //开始绑定
    int ret=bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"bind")
    //listen
    //开始监听,端口就开启了
    ret=listen(sfd,10);
    ERROR_CHECK(ret,-1,"listen")
    //accept，当没有连接时，accept会阻塞
    int new_fd;//new_fd才是和客户端进行交流的描述符
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    new_fd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);//accept完成了三次握手
    ERROR_CHECK(new_fd,-1,"accept")
    printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    //服务器接收数据
    char buf[128]={0};
    ret=recv(new_fd,buf,sizeof(buf),0);
    ERROR_CHECK(ret,-1,"recv")
    printf("I am server,gets=%s\n",buf);
    //服务器发送数据
    ret=send(new_fd,"world",5,0);
    ERROR_CHECK(ret,-1,"send")
    close(new_fd);
    close(sfd);
    return 0;
}