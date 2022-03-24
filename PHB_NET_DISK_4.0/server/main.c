#include "factory.h"

int main(int argc,char** argv)
{
    if(argc!=5)
    {
        printf("./thread_pool_server IP PORT THREAD_NUM CAPACITY\n");
        return -1;
    }
//*************************************************************************
context1:                   //*快速定位工具*//
    if(0)
    {
        goto CD;
        goto LS;
        goto PWD;
        goto QUIT;
        goto MKDIR;
        goto REGIST;
        goto LOGIN;
        goto RELOGIN;
        goto FAULT;
        goto RMDIR;
        goto REMOVE;
        goto TOKEN;
        goto context2;
        //goto TOKEN;
    }
//**************************************************************************
    //初始化用户服务队列
    cque client_que;
    clientque_init(&client_que);

    Factory_t f;//主数据结构
    int thread_num =atoi(argv[3]);
    int capacity =atoi(argv[4]);

    factory_init(&f,thread_num,capacity);
    factory_start(&f,thread_num);

    int socket_fd;
    tcp_init(&socket_fd,argv[1],argv[2]);

    int new_fd;
    struct sockaddr_in client_addr;
    Client_State_t *pnew;
    pNode_t pnew_task;
    pQue_t ptask_que=&f.que;//让一个队列指针指向队列

    int epfd=epoll_create(1);
    struct epoll_event evs[10];
    epoll_add(epfd,socket_fd);

    train_state_t train;
    train_t train1;
    train_state_int traint;
    char salt[SALT_LEN];
    char password[PSWD_LEN];
    char username[USERNAME_LEN];

    int ret;
    //用来存用户名和密码匹配查询的结果
    int check_ret;
    int ready_fd_num,i,j;
    while(1)
    {
        bzero(evs,sizeof(evs));
        ready_fd_num=epoll_wait(epfd,evs,10,-1);
        for(i=0;i<ready_fd_num;i++)
        {
            //有新的连接请求，接受连接请求并判断客户端是要登陆还是注册
            if(evs[i].data.fd==socket_fd)
            {
                bzero(&client_addr,sizeof(client_addr));
                socklen_t addr_len=sizeof(client_addr);
                new_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&addr_len);
                if(new_fd>0)
                {
                    printf("%s %d connect\n\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                }
                else{
                    printf("connect has failed\n\n");
                }
                //第一辆火车
                //登录或者失败的话，都从这里重新运行
             fail_label:
                //接收用户请求,同时发来的还有用户名
                recv_train(new_fd,&train);
                //-------------------------------------------------------------------------------
                //-------------------------------------------------------------------------------
                //login,regist,token,quit流程
                if(train.state==TOKEN)//token验证
                {
TOKEN:              ret=server_token(&train,new_fd,&client_que,pnew_task,ptask_que,&f);
                    if(ret==-1)
                    {
                        continue;
                    }
                }
                else if(train.state==LOGIN)//用户要求登录
                {
LOGIN:              ret=server_login(new_fd,&train,username,salt,password,&client_que);
                    if(ret==-1)
                    {
                        goto fail_label;
                    }
                }
                else if(train.state==REGISTER)//客户要求注册
                {
REGIST:             ret=server_regist(new_fd,&train,username,salt,password,&client_que);
                    if(ret==-1)
                    {
                        goto fail_label;
                    }
                }
                else if(train.state==QUIT)//客户要求退出
                {
                    printf("client quit.\n\n");
                    close(new_fd);
                    break;
                }
                else if(train.state==FAULT)
                {
FAULT:              printf("fault command ignored.\n\n");
                    goto fail_label;      
                }
                epoll_add(epfd,new_fd);//登陆成功，监视对应new_fd
            }
            else//当前evs不是连接请求
            {
                //从头部开始遍历用户服务队列，如果发来数据的new_fd在队列中找到了，用p指向该client结点
                pcnode p=client_que.que_head;
                while(1)
                {
                    if(p->client.new_fd==evs[i].data.fd)
                    {
                        break;
                    }
                    p=p->next_client;
                }
                //此时p指向要服务的client结点
                //接收用户请求
                recv_train(p->client.new_fd,&train);
                if(train.state==CD)
CD:             {
                    server_cd(p,&train);
                }
                else if(train.state==LS)
LS:             {
                    printf("ls command in %s\n",p->client.path);
                    int belongID;
                    //根据用户名找到ID，存入pID指向位置
                    find_ID(p->client.uname,&belongID);
                    //获取当前路径下某个用户的文件信息，并发给用户
                    send_in_path(p->client.new_fd,p->client.code,belongID);
                    char* cmd="ls";
                    usercmd_note(p->client.uname,cmd,p->client.path);
                    printf("command ls success.\n\n");
                }
                else if(train.state==PWD)
PWD:            {
                    printf("pwd command from new_fd %d.\n",p->client.new_fd);
                    set_train(&train,p->client.path);
                    //发送路径火车
                    send_train(p->client.new_fd,train);
                    //记录cmdnote
                    char* cmd="pwd";
                    usercmd_note(p->client.uname,cmd,p->client.path);
                    printf("command pwd success.\n\n");
                }
                else if(train.state==QUIT)
QUIT:           {
                    printf("quit command from new_fd %d.\n",p->client.new_fd);
                    close(p->client.new_fd);
                    //调整client队列大小，打印现在的成员
                    client_que.que_size--;
                    printf("client quit,%d left\n\n",client_que.que_size);
                    show_client(client_que.que_head,client_que.que_tail);
                    char* cmd="quit";
                    usercmd_note(p->client.uname,cmd,p->client.path);
                    printf("command quit success.\n\n");
                    continue;
                }
                else if(train.state==LOGIN)
RELOGIN:        {
                    printf("user %s ask to relogin.\n",p->client.uname);
                    //调整client队列大小，打印现在的成员
                    client_que.que_size--;
                    printf("client quit,%d left\n\n",client_que.que_size);
                    show_client(client_que.que_head,client_que.que_tail);
                    //记录cmdnote
                    char* cmd="relogin";
                    usercmd_note(p->client.uname,cmd,p->client.path);
                    printf("command login success.\n\n");
                    continue;
                }
                else if(train.state==MKDIR)
MKDIR:          {
                    server_mkdir(p,&train);
                }
                else if(train.state==RMDIR)
RMDIR:          {
                    server_rmdir(p,&train);
                }
                else if(train.state==REMOVE)
REMOVE:         {
                    server_remove(p,&train);
                }
            printf("\n");
        }
    }
}
    //*************************************************************************
context2:                   //*快速定位工具*//
    if(0)
    {
        goto CD;
        goto LS;
        goto PWD;
        goto QUIT;
        goto MKDIR;
        goto REGIST;
        goto LOGIN;
        goto RELOGIN;
        goto FAULT;
        goto RMDIR;
        goto REMOVE;
        goto context1;
        goto TOKEN;
        //goto TOKEN;
    }
//**************************************************************************
}