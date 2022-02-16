#include<func.h>
//epoll实现即时聊天，并且客户端断开后可服务器端不断

int epoll_add(int epfd,int fd)
{//注册
    struct epoll_event event;
    event.events=EPOLLIN;//监控是否可读
    event.data.fd=fd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl_add")
}

int epoll_del(int epfd,int fd)
{//解除注册
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl_del")
}

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    //socket
    int sfd;//定义一个网络描述符
    sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络描述符，在内核中对应一个缓冲区
    ERROR_CHECK(sfd,-1,"socket")
    printf("sfd=%d\n",sfd);
    //bind之前设置reuse,设置后服务器端异常退出后可以再次快速启动
    int reuse=1,ret;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    //bind
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family = AF_INET;//IPV4通信
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);//把ip的点分十进制转换为网络字节序
    ser_addr.sin_port=htons(atoi(argv[2]));//把端口号转为网络字节序
    //开始绑定
    ret=bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
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
    //即时聊天
    int epfd=epoll_create(1);
    struct epoll_event evs[3];
    //先注册标准输入
    epoll_add(epfd,STDIN_FILENO);
    //再注册sfd
    epoll_add(epfd,sfd);
    char buf[128]={0};
    int ready_fdnum,i;
    while(1)
    {
        ready_fdnum=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<ready_fdnum;i++)
        {
            if(evs[i].data.fd==sfd)//如果sfd可读
            {
                new_fd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);//accept完成了三次握手
                ERROR_CHECK(new_fd,-1,"accept")
                printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                epoll_add(epfd,new_fd);
            }
            if(evs[i].data.fd==STDIN_FILENO)//如果标准输入可读
            {
                bzero(buf,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));//把标准输入读到buf里面
                if(0==ret)
                {
                    printf("I have to go\n");
                    goto label;
                }
                //读到了信息，发送对应的字符串到对端，不带\n
                ret=send(new_fd,buf,strlen(buf)-1,0);
                ERROR_CHECK(ret,-1,"send")
            }
            if(evs[i].data.fd==new_fd)//如果new_fd可读
            {
                bzero(buf,sizeof(buf));
                //接收数据
                ret=recv(new_fd,buf,sizeof(buf),0);
                ERROR_CHECK(ret,-1,"recv")
                if(0==ret)//对端断开
                {
                    printf("byebye\n");
                    epoll_del(epfd,new_fd);//解除注册
                    close(new_fd);
                    break;
                }
                printf("%s\n",buf);
            }
        }
    }
    label:
    close(new_fd);
    close(sfd);
    return 0;
}