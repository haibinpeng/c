#include<func.h>
//实现客户端断开后，服务器不断，能够支持客户端再次连上:服务器端要监控3个描述符

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
    int new_fd;//new_fd才是和客户端进行交流的描述符
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    //即时聊天
    char buf[128]={0};
    fd_set rdset,tmpset;//rdset一直监控STDIN_FILENO和sfd，tmpset做传入传出参数
    FD_ZERO(&rdset);
    FD_SET(STDIN_FILENO,&rdset);
    FD_SET(sfd,&rdset);
    while(1)
    {
        memcpy(&tmpset,&rdset,sizeof(fd_set));//把rdset监控的先copy过来
        ret=select(11,&tmpset,NULL,NULL,NULL);
        if(FD_ISSET(sfd,&tmpset))//如果sfd可读,就去accept
        {
            new_fd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);//accept完成了三次握手
            ERROR_CHECK(new_fd,-1,"accept")
            printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            //accept后就会产生一个new_fd,将new_fd添加到要监控的集合里
            FD_SET(new_fd,&rdset);
        }
        if(FD_ISSET(STDIN_FILENO,&tmpset))//如果标准输入可读
        {
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));//把标准输入读到buf里面
            if(0==ret)
            {
                printf("I have to go\n");
                break;
            }
            //读到了信息，发送对应的字符串到对端，不带\n
            ret=send(new_fd,buf,strlen(buf)-1,0);
            ERROR_CHECK(ret,-1,"send")
        }
        if(FD_ISSET(new_fd,&tmpset))//如果new_fd可读
        {
            bzero(buf,sizeof(buf));
            //接收数据
            ret=recv(new_fd,buf,sizeof(buf),0);
            ERROR_CHECK(ret,-1,"recv")
            if(0==ret)//对端断开
            {
                printf("byebye\n");
                FD_CLR(new_fd,&rdset);//从监控集合中移除
            }else{
                printf("%s\n",buf);
            }
        }
    }
    close(new_fd);
    close(sfd);
    return 0;
}