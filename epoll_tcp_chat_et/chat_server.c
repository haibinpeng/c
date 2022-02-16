#include<func.h>
//epoll的边缘触发,优势是触发次数少

void fcntl_nonblock(int fd)
{
    int status=fcntl(fd, F_GETFL);//拿出标准输入所有属性
    status=status|O_NONBLOCK;//修改其为非阻塞
    fcntl(fd, F_SETFL,status);
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
    new_fd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);//accept完成了三次握手
    ERROR_CHECK(new_fd,-1,"accept")
    printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    fcntl_nonblock(new_fd);
    //即时聊天
    int epfd=epoll_create(1);
    struct epoll_event event,evs[2];
    //先注册标准输入
    event.events=EPOLLIN;//监控是否可读
    event.data.fd=STDIN_FILENO;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl")
    //再注册new_fd
    event.events=EPOLLIN|EPOLLET;//监控是否可读,并切换为边缘触发
    event.data.fd=new_fd;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl")
    char buf[6]={0};
    int ready_fdnum,i;
    while(1)
    {
        ready_fdnum=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<ready_fdnum;i++)
        {
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
                while(1)
                {
                    bzero(buf,sizeof(buf));
                    //接收数据
                    ret=recv(new_fd,buf,sizeof(buf)-1,0);
                    if(0==ret)//对端断开
                    {
                        printf("byebye\n");
                        goto label;
                    }else if(-1==ret){//非阻塞会返回-1，代表读空了
                        printf("\n");
                        break;
                    }else{
                        printf("%s",buf);
                    }
                }
            }
        }
    }
    label:
    close(new_fd);
    close(sfd);
    return 0;
}