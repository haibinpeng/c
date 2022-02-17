#include "process_pool.h"

int tcp_init(int* socket_fd,char* ip,char* port)
{
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络描述符，在内核中其实对应了一个缓冲区
    ERROR_CHECK(sfd,-1,"socket")
    //在bind之前执行setsockopt,设定端口重用
    int reuse=1,ret;
    ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt")
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family = AF_INET;//IPV4通信
    ser_addr.sin_addr.s_addr=inet_addr(ip);//把ip的点分十进制转换为网络字节序
    ser_addr.sin_port = htons(atoi(port));//把端口转为网络字节序
    //开始绑定
    ret=bind(sfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"bind")
    //打开端口，开始监听
    ret=listen(sfd,10);
    ERROR_CHECK(ret,-1,"listen")
    *socket_fd=sfd;//socket_fd是一个传出参数
    return 0;
}

int epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.events=EPOLLIN;//监控是否可读
    event.data.fd=fd;//要监控的描述符
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl_add")
    return 0;
}