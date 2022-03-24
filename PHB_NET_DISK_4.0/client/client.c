#include "func.h"

int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3);

    //屏蔽信号
    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet,SIGINT);
    //sigaddset(&sigSet,SIGQUIT);
    sigprocmask(SIG_BLOCK,&sigSet,NULL);
    
    int sfd;
    int ret;
    char token[100];
    train_state_t train;
    char cmd[100];
    char salt[SALT_LEN];
    char userpwd[100];
    char username[100];
    ptasknode ptask;
    pthread_t* pthid;
    pthid=(pthread_t *)calloc(10,sizeof(pthread_t));//为线程id申请空间
    int pthread_count=0;
reconnect:
    sfd=socket(AF_INET,SOCK_STREAM,0);//初始化一个网络描述符，对应了一个缓冲区
    ERROR_CHECK(sfd,-1,"socket");

    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family=AF_INET;//代表要进行ipv4通信
    ser_addr.sin_addr.s_addr=inet_addr(argv[1]);//把ip的点分十进制转为网络字节序
    ser_addr.sin_port=htons(atoi(argv[2]));//把端口转为网络字节序

    char mutex_buf[5];//存当前login锁的状态
    bzero(mutex_buf,5);
    char one_buf[5]="1";//存一个1，代表不能登录
    char zero_buf[5]="0";//存一个0，代表能登陆
    int mutex=open("login_mutex",O_RDWR);

    //检查login锁,若为0则可登录,为1则堵塞
    //check_login_mutex1(mutex,mutex_buf,one_buf,zero_buf);
    //客户端就要去连接服务器
    ret=connect(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));
    ERROR_CHECK(ret,-1,"connect");
    //检查login锁,若为0则可登录,为1则堵塞
    if(ret!=-1)
    {
        check_login_mutex1(mutex,mutex_buf,one_buf,zero_buf);
    }

    // //屏蔽信号
    // sigset_t sigSet;
    // sigemptyset(&sigSet);
    // sigaddset(&sigSet,SIGINT);
    // //sigaddset(&sigSet,SIGQUIT);
    // sigprocmask(SIG_BLOCK,&sigSet,NULL);

fail_label:
    bzero(&train,sizeof(train));
    bzero(token,sizeof(token));
    bzero(cmd,sizeof(cmd));
    print_login();
fault_command:
    printf("login or register or quit (enter l/r/q):");
    scanf("%s",cmd);

    if(!strcmp(cmd,"l"))
    {
        login_label:
        ret=client_login(sfd,token,username,userpwd);
        if(ret==-1)//登陆失败
        {
            goto fail_label;
        }
    }
    else if(!strcmp(cmd,"r"))
    {
        ret=client_register(sfd,token,username,userpwd);
        if(ret==-1)//注册失败
        {
            goto fail_label;    
        }
    }
    else if(!strcmp(cmd,"q"))
    {
        train.state=QUIT;
        //发送QUIT火车
        send_train(sfd,train);
        close(sfd);
        printf("quit netdisk success\n");
        lseek(mutex,0,SEEK_SET);
        ret=write(mutex,zero_buf,1);//解login锁
        return -1;
    }
    else
    {
        bzero(&train,sizeof(train));
        train.state=FAULT;
        send_train(sfd,train);
        goto fault_command;
    }

    lseek(mutex,0,SEEK_SET);
    ret=write(mutex,zero_buf,1);//解login锁
    close(mutex);
    //登陆成功后，循环判断用户输入了什么命令：
    while(1)
    {
        fflush(stdin);
        bzero(cmd,sizeof(cmd));
        fgets(cmd,100,stdin);
        delete_hh(cmd,100);

        mutex=open("login_mutex",O_RDWR);
        check_login_mutex2(mutex,mutex_buf,one_buf,zero_buf,cmd);

        if(cmd[0]=='c'&&cmd[1]=='d')//收到cd命令，分三种
        {
            printf("\n");//仅为了格式美观
            client_CD(sfd,cmd);
        }
        else if(!strcmp(cmd,"ls"))
        {
            printf("\n");
            client_LS(sfd);
        }
        else if(!strcmp(cmd,"pwd"))
        {
            printf("\n");
            client_PWD(sfd);
        }
        else if(!strcmp(cmd,"help"))
        {
            print_help(username);
        }
        else if(!is_remove(cmd))
        {
            printf("\n");
            client_REMOVE(sfd,cmd);
        }
        else if(!strcmp(cmd,"quit"))
        {
            client_QUIT(sfd,username);
            close(sfd);
            return -1;
        }
        else if(!strcmp(cmd,"relogin"))
        {
            client_LOGIN(sfd);
            close(sfd);
            goto reconnect;
        }
        else if(!is_mkdir(cmd))
        {
            printf("\n");
            client_mkdir(sfd,cmd);
        }
        else if(!is_rmdir(cmd))
        {
            printf("\n");
            client_rmdir(sfd,cmd);
        }
        else if(!is_puts(cmd))
        {
            char filename[200];
            bzero(filename,sizeof(filename));
            //得到目标文件名
            get_filename_putsandgets(cmd,filename);
            char pathname[200]={0};
            sprintf(pathname,"client_file/%s",filename);
            ptask=(ptasknode)malloc(sizeof(tasknode));
            bzero(ptask,sizeof(tasknode));
            Compute_file_md5(pathname,ptask->md5name);
            strcpy(ptask->ip,argv[1]);
            strcpy(ptask->port,argv[2]);
            strcpy(ptask->token,token);
            strcpy(ptask->filename,filename);
            ptask->state=PUTS;
            pthread_create(pthid+pthread_count,NULL,thread_handle_puts,ptask);
            printf("\n    Thread start to puts, but you still can give command. \n\n");
            printf("    When task finished, i will give you a noticement!\n\n");
        }
        else if(!is_gets(cmd))
        {
            char filename[200];
            bzero(filename,sizeof(filename));
            //得到目标文件名
            get_filename_putsandgets(cmd,filename);
            ptask=(ptasknode)malloc(sizeof(tasknode));
            bzero(ptask,sizeof(tasknode));
            strcpy(ptask->ip,argv[1]);
            strcpy(ptask->port,argv[2]);
            //判断offset应该是多少
            char pathname[200]={0};
            sprintf(pathname,"client_file/%s",filename);
            int testfd=open(pathname,O_RDWR);
            if(testfd==-1)//不存在该文件
            {
                ptask->offset=0;
            }
            else//存在该文件，记录偏移值
            {
                // struct stat buf;
                // int ret=stat(pathname,&buf);
                long datacount=0;
                long datatotal=0;
                char databuf[4096]={0};
                while(1)
                {
                    bzero(databuf,4096);
                    datacount=read(testfd,databuf,4096);
                    if(datacount==0)
                    {
                        break;
                    }
                    else
                    {
                        datatotal+=datacount;
                    }
                }
                ptask->offset=datatotal;
            }
            strcpy(ptask->token,token);
            strcpy(ptask->filename,filename);
            ptask->state=GETS;
            pthread_create(pthid+pthread_count,NULL,thread_handle_gets,ptask);
            printf("\n    Thread start to gets, but you still can give command. \n\n");
            printf("    When task finished, i will give you a noticement!\n\n");
        }
        fflush(stdin);
        close(mutex);
    }
    close(sfd);
    return 0;
}