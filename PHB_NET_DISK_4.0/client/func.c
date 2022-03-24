#include "func.h"

//子线程gets流程
void* thread_handle_puts(void *p)
{
    ptasknode ptask=(ptasknode)p;
    train_state_t train;
    train_t train1;
    train_state_int traint;
    ptask->sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family=AF_INET;//代表要进行ipv4通信
    ser_addr.sin_addr.s_addr=inet_addr(ptask->ip);//把ip的点分十进制转为网络字节序
    ser_addr.sin_port=htons(atoi(ptask->port));//把端口转为网络字节序
    // ser_addr.sin_addr.s_addr=inet_addr("192.168.2.130");//把ip的点分十进制转为网络字节序
    // ser_addr.sin_port=htons(2000);//把端口转为网络字节序
    int ret=connect(ptask->sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));

    //发TOKEN,和token
    set_train(&train,ptask->token);
    train.state=TOKEN;
    send_train(ptask->sfd,train);
    //printf("1 send token=%s\n",ptask->token);
    //发文件名
    set_train(&train,ptask->filename);
    send_train(ptask->sfd,train);
    //printf("2 send filename=%s\n",ptask->filename);
    //发state
    set_int_train(&traint,ptask->state);
    send_int_train(ptask->sfd,traint);
    //发offset
    set_int_train(&traint,(int)ptask->offset);
    send_int_train(ptask->sfd,traint);
    //printf("3 send state=%d\n",traint.data);
    //接收结果火车
    recv_train(ptask->sfd,&train);
    if(train.data_len==0)//空火车,代表token错误
    {
        printf("\n    Who are you, boy?\n\n");
    }
    else//非空火车，代表token正确
    {
        char pathbuf[1200];
        char databuf[4096]={0};
        recv_train(ptask->sfd,&train);
        if(train.data_len==0)//空火车,代表filename错误
        {
            printf("\n    wrong filename or file exist!\n\n");
        }
        else//非空火车，开始接收文件
        {
            //传MD5
            set_train(&train,ptask->md5name);
            send_train(ptask->sfd,train);
            //接收判断结果
            recv_train(ptask->sfd,&train);
            if(train.data_len==0)//MD5已经有了
            {
                printf("\n    ****** file source exist, puts file success. ******\n\n");
            }
            else
            {
                sprintf(pathbuf,"client_file/%s",ptask->filename);
                struct stat buf;
                int ret=stat(pathbuf,&buf);
                off_t file_size=buf.st_size;
                set_int_train(&traint,(int)file_size);
                send_int_train(ptask->sfd,traint);

                int fd=open(pathbuf,O_RDWR);
                //发送文件内容
                void* p=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
                ret=send_n(ptask->sfd,p,buf.st_size);
                munmap(p,buf.st_size);
                printf("\n    ****** upload end, puts file success. ******\n\n");
                close(fd);
            }
        }
    }
}

//子线程gets流程
void* thread_handle_gets(void *p)
{
    ptasknode ptask=(ptasknode)p;
    train_state_t train;
    train_t train1;
    train_state_int traint;
    ptask->sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));//清空
    ser_addr.sin_family=AF_INET;//代表要进行ipv4通信
    ser_addr.sin_addr.s_addr=inet_addr(ptask->ip);//把ip的点分十进制转为网络字节序
    ser_addr.sin_port=htons(atoi(ptask->port));//把端口转为网络字节序
    // ser_addr.sin_addr.s_addr=inet_addr("192.168.2.130");//把ip的点分十进制转为网络字节序
    // ser_addr.sin_port=htons(2000);//把端口转为网络字节序
    int ret=connect(ptask->sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));

    //发TOKEN,和token
    set_train(&train,ptask->token);
    train.state=TOKEN;
    send_train(ptask->sfd,train);
    //printf("1 send token=%s\n",ptask->token);
    //发文件名
    set_train(&train,ptask->filename);
    send_train(ptask->sfd,train);
    //printf("2 send filename=%s\n",ptask->filename);
    //发state
    set_int_train(&traint,ptask->state);
    send_int_train(ptask->sfd,traint);
    //发offset
    set_int_train(&traint,(int)ptask->offset);
    send_int_train(ptask->sfd,traint);
    //printf("3 send state=%d\n",traint.data);
    //接收结果火车
    recv_train(ptask->sfd,&train);
    //printf("4 recv sign=%s\n",train.buf);
    if(train.data_len==0)//空火车,代表token错误
    {
        printf("\n    wrong token\n\n");
    }
    else//非空火车，代表token正确
    {
        char pathbuf[1200];
        char databuf[4096]={0};
        recv_train(ptask->sfd,&train);
        //printf("5 recv sign=%s\n",train.buf);
        if(train.data_len==0)//空火车,代表filename错误
        {
            printf("\n    wrong filename\n\n");
        }
        else//非空火车，开始接收文件
        {
            //接收整型火车，内存文件长度
            recv_int_train(ptask->sfd,&traint);
            //printf("6 recv filelenth=%d\n",traint.data);
            off_t file_size=traint.data;
            off_t recv_size=0;
            sprintf(pathbuf,"client_file/%s",ptask->filename);
            int fd=open(pathbuf,O_RDWR|O_CREAT,0666);
            if(ptask->offset==0)
            {
                if(file_size<-5000000)//小文件，逻辑有点绕，还没有写好
                {
                    while(file_size>4096)
                    {
                        bzero(databuf,4096);
                        recv(ptask->sfd,databuf,4096,0);//接文件内容
                        write(fd,databuf,4096);//把文件内容写到文件里
                        file_size-=4096;
                        recv_size+=4096;
                    }
                    bzero(databuf,4096);
                    recv(ptask->sfd,databuf,file_size,0);//接文件内容
                    write(fd,databuf,file_size);//把文件内容写到文件里
                    close(fd);
                }
                else
                {
                    printf("\n    .....DMA START.....\n\n");
                    //扩大文件大小至要接收的数据量
                    ftruncate(fd,file_size);
                    //DMA
                    void *p=mmap(NULL,file_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
                    int total=0;
                    while(total<file_size)
                    {
                        int ret=recv(ptask->sfd,p+total,4096,0);
                        if(0==ret)//代表对端断开了
                        {
                            break;
                        }
                        total+=ret;
                    }
                    munmap(p,file_size);
                    if(total==file_size)
                    {
                        printf("\n    ****** %s download 100.00%%. ******\n\n",pathbuf);
                    }
                    else
                    {
                        printf("\n****** %s download stoped,server wrong. ******\n\n",pathbuf);
                    }
                    ftruncate(fd,total);
                    close(fd);
                }
            }
            else
            {
                printf("\n     .....%ld DATA FILE EXIST.....\n",ptask->offset);
                printf("\n            .....DMA START.....\n\n");
                //扩大文件大小至要接收的数据量
                ftruncate(fd,file_size);
                //DMA
                void *p=mmap(NULL,file_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
                //接收文件数据，打印进度
                int total=ptask->offset;
                while(total<file_size)
                {
                    int ret=recv(ptask->sfd,p+total,4096,0);
                    if(0==ret)//代表对端断开了
                    {
                        break;
                    }
                    total+=ret;
                }
                ftruncate(fd,total);
                munmap(p,file_size);
                if(total==file_size)
                {
                    printf("\n    ****** %s download 100.00%%. ******\n\n",pathbuf);
                }
                else
                {
                    printf("\n****** %s download stoped,server wrong. ******\n\n",pathbuf);
                }
                ftruncate(fd,total);
                close(fd);
            }
        }
    }
}

void set_train(train_state_t* ptrain,char* goods_buf)
{
    bzero(ptrain,sizeof(train_state_t));
    strcpy(ptrain->buf,goods_buf);
    ptrain->data_len=strlen(goods_buf);
    ptrain->state=NORMAL;
}

int recv_train(int sfd,train_state_t *ptrain)
{
    int ret;
    bzero(ptrain,sizeof(*ptrain));
    ret=recv(sfd,&(ptrain->data_len),sizeof(int),0);
    ERROR_CHECK(ret,-1,"recv");
    ret=recv(sfd,ptrain,(ptrain->data_len)+8,0);
    ERROR_CHECK(ret,-1,"recv");
}

int send_train(int sfd,train_state_t train)
{
    int ret;
    ret=send(sfd,&train.data_len,sizeof(int),0);
    ERROR_CHECK(ret,-1,"send");
    ret=send(sfd,&train,train.data_len+8,0);
    ERROR_CHECK(ret,-1,"send");
}

int recv_int_train(int sfd,train_state_int *ptrain)
{
    int ret;
    //printf("wait lenth\n");
    bzero(ptrain,sizeof(train_state_int));
    ret=recv(sfd,ptrain,sizeof(train_state_int),0);
    ERROR_CHECK(ret,-1,"recv");
}

int send_int_train(int sfd,train_state_int train)
{
    int ret;
    ret=send(sfd,&train,sizeof(train_state_int),0);
    ERROR_CHECK(ret,-1,"send");
}

void set_int_train(train_state_int* ptrain,int data)
{
    bzero(ptrain,sizeof(train_state_int));
    ptrain->data=data;
}

int check_login_mutex1(int mutex,char* mutex_buf,char* one_buf,char* zero_buf)
{
    int ret;
    read(mutex,mutex_buf,sizeof(mutex_buf));
    if(!strcmp(mutex_buf,"0"))//锁文件里为0，代表可以登录
    {
        //加login锁
        lseek(mutex,0,SEEK_SET);
        ret=write(mutex,one_buf,1);
        ERROR_CHECK(ret,-1,"write1");
    }
    else if(!strcmp(mutex_buf,"1"))
    {
        printf("\n");
        printf("-----------------------------------------------------------------------\n");
        printf("-------Someone else is preparing to login/regist/quit,please wait------\n");
        printf("-----------------------------------------------------------------------\n");
        printf("\n");
        while(1)//检测到锁打开，就解锁
        {
            lseek(mutex,0,SEEK_SET);
            read(mutex,mutex_buf,sizeof(mutex_buf));
            if(!strcmp(mutex_buf,"0"))//锁文件里为0，代表可以登录
            {
                //加锁
                lseek(mutex,0,SEEK_SET);
                ret=write(mutex,one_buf,1);
                ERROR_CHECK(ret,-1,"write0");
                break;//退出
            }
        }
    }
}

int check_login_mutex2(int mutex,char* mutex_buf,char* one_buf,char* zero_buf,char* cmd)
{
    int ret;
    lseek(mutex,0,SEEK_SET);
    read(mutex,mutex_buf,sizeof(mutex_buf));
    if(!strcmp(mutex_buf,"1"))
    {
        printf("-----------------------------------------------------------------------\n");
        printf("-------Someone else is preparing to login/regist/quit,please wait------\n");
        printf("-----------------------------------------------------------------------\n");
        while(1)//检测到锁打开给提示
        {
            lseek(mutex,0,SEEK_SET);
            read(mutex,mutex_buf,sizeof(mutex_buf));
            if(!strcmp(mutex_buf,"0"))//锁文件里为0，代表可以登录
            {
                printf("-----------------------------------------------------------------------\n");
                printf("-----------------------You can enter command now!----------------------\n");
                printf("-----------------------------------------------------------------------\n");
                printf("\n");
                bzero(cmd,sizeof(cmd));
                break;//退出
            }
        }
        bzero(cmd,sizeof(cmd));
    }
}

int client_login(int sfd,char *token,char* username,char* userpwd)
{
    train_state_t train;
    bzero(&train,sizeof(train));

    printf("user name:");
    scanf("%s",username);
    printf("user password:");
    scanf("%s",userpwd);
    //用户名装车
    set_train(&train,username);
    train.state=LOGIN;
    //发送用户名；
    send_train(sfd,train);
    //准备salt容器
    char salt[SALT_LEN];
    bzero(salt,sizeof(salt));
    //接收salt或是一个空火车
    recv_train(sfd,&train);
    if(train.data_len==0)//如果是空火车
    {
        printf("wrong user name!\n");
        return -1;
    }
    //如果火车数据长度不是0，存下salt
    strcpy(salt,train.buf);
    //存token
    strcpy(token,salt);
    //计算密码
    char* lp;
    lp=crypt(userpwd,salt);
    sprintf(userpwd,"%s",lp);
    //密码装车
    set_train(&train,userpwd);
    //发送加密后的密码
    send_train(sfd,train);
    //如果收到终止火车，说明不密码匹配
    recv_train(sfd,&train);
    if(train.data_len==0)//收到空火车，终止
    {
        printf("wrong user password!\n");
        return -1;
    }
    //收到非空火车，欢迎用户
    print_welcome_help(username);
    return 0;
}

int client_register(int sfd,char *token,char* username,char* userpwd)
{
    train_state_t train;
    bzero(&train,sizeof(train));

    printf("user name:");
    scanf("%s",username);
    printf("user password:");
    scanf("%s",userpwd);
    //用户名装车
    set_train(&train,username);
    train.state=REGISTER;
    //发送用户名
    send_train(sfd,train);
    //密码装车
    set_train(&train,userpwd);
    //发送密码
    send_train(sfd,train);
    //接收代表结果和token的火车
    recv_train(sfd,&train);
    if(train.data_len==0)//注册失败
    {
        printf("username exist!\n");
        return -1;
    }
    //存下token
    strcpy(token,train.buf);
    //注册成功
    printf("regist success!\n");
    print_welcome_help(username);
    return 0;
}

void print_welcome_help(char* username)
{
    printf("\n");
    printf("***********************************************************************\n");
    printf("---------------Hello, user %s ! Welcome to PHB_NET_DISK!---------------\n",username);
    printf("---------------------(Enter 'quit' to leave!!!)------------------------\n");
    printf("***********************************************************************\n");
    printf("\n");
    printf("***********************************************************************\n");
    printf("---- COMMAND LIST: ----------------------------------------------------\n");
    printf("-------------- cd .. ---- cd <dirname> ------ cd / --------------------\n");
    printf("-------------- ls-------- remove <filename> - pwd ---------------------\n");
    printf("-------------- quit ----- gets <filename> --- puts <filename> ---------\n");
    printf("-------------- relogin -- mkdir <dirname> --- rmdir <dirname> ---------\n");
    printf("-------------- help ---------------------------------------------------\n");
    printf("***********************************************************************\n");
    printf("\n");
}

void print_help(char* username)
{
    printf("\n");
    printf("***********************************************************************\n");
    printf("--------- HELP: -------------------------------------------------------\n");
    printf("-------------- cd .. ---- cd <dirname> ------ cd / --------------------\n");
    printf("-------------- ls-------- remove <filename> - pwd ---------------------\n");
    printf("-------------- quit ----- gets <filename> --- puts <filename> ---------\n");
    printf("-------------- relogin -- mkdir <dirname> --- rmdir <dirname> ---------\n");
    printf("-------------- help ---------------------------------------------------\n");
    printf("***********************************************************************\n");
    printf("\n");
}

void print_login()
{
    printf("-----------------------------------------------------------------------\n");
    printf("-------------------------WELCOME TO PHB_NET_DISK-----------------------\n");
    printf("-----请按照提示尽快完成登录界面的操作,以免阻塞其他用户,感谢您的配合----\n");
    printf("-----------------------------------------------------------------------\n");
}

int client_CD(int sfd,char cmd[])
{
    char targetname[100];
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=CD;
    train.data_len=1;
    //如果是cd /,buf里装一个a,开始流程
    if(cmd[3]=='/'&&cmd[4]==0)
    {
        strcpy(train.buf,"a");
        //该火车告诉服务器是cd的哪一种
        send_train(sfd,train);
        //接收当前路径
        recv_train(sfd,&train);
        printf("    %s\n\n",train.buf);
    }
    //如果是cd ..,buf里装一个b,开始流程
    else if(cmd[3]=='.'&&cmd[4]=='.')
    {
        strcpy(train.buf,"b");
        //该火车告诉服务器是cd的哪一种
        send_train(sfd,train);
        //接收当前路径
        recv_train(sfd,&train);
        if(train.data_len==0)
        {
            printf("    You has been in root!\n\n");
        }
        else
        {
            printf("    %s\n\n",train.buf);
        }
    }
    //如果是cd <dirname>,buf里装一个c,开始流程
    else if(cmd[3]!=' ')
    {   
        strcpy(train.buf,"c");
        //该火车告诉服务器是cd的哪一种
        send_train(sfd,train);
        //要把文件夹名摘下来
        get_filename(cmd,targetname);
        //把文件名传给服务器端
        set_train(&train,targetname);
        send_train(sfd,train);
        //接收当前路径
        recv_train(sfd,&train);
        if(train.data_len==0)
        {
            printf("    wrong dirname.\n\n");
        }
        else
        {
            printf("    %s\n\n",train.buf);
        }
    }
}

int client_LS(int sfd)
{
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=LS;
    //传ls火车
    send_train(sfd,train);
    //循环接收文件信息
    while(1)
    {
        recv_train(sfd,&train);
        if(train.data_len==0)
        {
            break;
        }
        //delete_hh(train.buf);
        printf("    %s",train.buf);
    }
    return 0;
}

int client_PWD(int sfd)
{
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=PWD;
    //传pwd火车
    send_train(sfd,train);
    //接收path路径
    recv_train(sfd,&train);
    printf("    %s\n\n",train.buf);
    return 0;
}

int client_REMOVE(int sfd,char* cmd)
{
    char targetname[100];
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=REMOVE;
    //传REMOVE火车
    send_train(sfd,train);
    bzero(targetname,sizeof(targetname));
    //得到文件夹名字
    get_remove_filename(cmd,targetname);
    //文件名装车
    set_train(&train,targetname);
    //发送文件名火车
    send_train(sfd,train);
    //接收结果
    recv_train(sfd,&train);
    printf("    %s\n\n",train.buf);
}

int client_QUIT(int sfd,char * username)
{
    train_state_t train;
    bzero(&train,sizeof(train));
    set_train(&train,username);
    train.state=QUIT;
    //传QUIT火车
    send_train(sfd,train);
    return 0;
}

int client_LOGIN(int sfd)
{
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=QUIT;
    //传LOGIN火车
    send_train(sfd,train);
    return 0;
}

int client_REGISTER(int sfd)
{
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=REGISTER;
    //传REGISTER火车
    send_train(sfd,train);
    return 0;
}

int client_mkdir(int sfd,char* cmd)
{
    char targetname[100];
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=MKDIR;
    //传MKDIR火车
    send_train(sfd,train);
    bzero(targetname,sizeof(targetname));
    //得到文件夹名字
    get_dirname(cmd,targetname);
    //文件名装车
    set_train(&train,targetname);
    //发送文件名火车
    send_train(sfd,train);
    //接收结果
    recv_train(sfd,&train);
    if(train.data_len==0)
    {
        printf("    dirname existed!\n\n");
    }
    else{
        printf("    %s\n\n",train.buf);
    }
}

int client_rmdir(int sfd,char* cmd)
{
    char targetname[100];
    train_state_t train;
    bzero(&train,sizeof(train));
    train.state=RMDIR;
    //传RMDIR火车
    send_train(sfd,train);
    bzero(targetname,sizeof(targetname));
    //得到文件夹名字
    get_dirname(cmd,targetname);
    //文件名装车
    set_train(&train,targetname);
    //发送文件名火车
    send_train(sfd,train);
    //接收结果
    recv_train(sfd,&train);
    printf("    %s\n\n",train.buf);
}

void delete_hh(char* target,int lenth)
{
    for(int j=0;j<lenth;j++)//去除多余的换行
    {
        if(!strcmp(target+j,"\n"))
        {
            bzero(target+j,sizeof(char));
        }
    }
}

void get_filename(char* cmd,char *targetname)
{
    bzero(targetname,sizeof(targetname));
    int i,j;
    for(i=3;cmd[i]!=0&&cmd[i]!=' ';i++)
    {
        memcpy(&targetname[i-3],&cmd[i],sizeof(char));
    }
    //printf("%s\n",targetname);
}

void get_filename_putsandgets(char* cmd,char *targetname)
{
    bzero(targetname,sizeof(targetname));
    int i,j;
    for(i=5;cmd[i]!=0&&cmd[i]!=' ';i++)
    {
        memcpy(&targetname[i-5],&cmd[i],sizeof(char));
    }
    //printf("%s\n",targetname);
}

void get_remove_filename(char* cmd,char* targetname)
{
    bzero(targetname,sizeof(targetname));
    int i,j;
    for(i=7;cmd[i]!=0&&cmd[i]!=' ';i++)
    {
        memcpy(&targetname[i-7],&cmd[i],sizeof(char));
    }
    //printf("%s\n",targetname);
}

void get_dirname(char* cmd,char *targetname)
{
    bzero(targetname,sizeof(targetname));
    int i,j;
    for(i=6;cmd[i]!=0&&cmd[i]!=' ';i++)
    {
        memcpy(&targetname[i-6],&cmd[i],sizeof(char));
    }
    //printf("%s\n",targetname);
}

int is_mkdir(char *cmd)
{
    if(cmd[0]=='m'&&cmd[1]=='k')
    {
        if(cmd[2]=='d'&&cmd[3]=='i')
        {
            if(cmd[4]=='r')
            {
                return 0;
            }
        }
    }
    return -1;
}

int is_remove(char *cmd)
{
    if(cmd[0]=='r'&&cmd[1]=='e')
    {
        if(cmd[2]=='m'&&cmd[3]=='o')
        {
            if(cmd[4]=='v'&&cmd[5]=='e')
            {
                return 0;
            }
        }
    }
    return -1;
}

int is_rmdir(char *cmd)
{
    if(cmd[0]=='r'&&cmd[1]=='m')
    {
        if(cmd[2]=='d'&&cmd[3]=='i')
        {
            if(cmd[4]=='r')
            {
                return 0;
            }
        }
    }
    return -1;
}

int is_puts(char *cmd)
{
    if(cmd[0]=='p'&&cmd[1]=='u')
    {
        if(cmd[2]=='t'&&cmd[3]=='s')
        {
            return 0;
        }
    }
    return -1;
}

int is_gets(char *cmd)
{
    if(cmd[0]=='g'&&cmd[1]=='e')
    {
        if(cmd[2]=='t'&&cmd[3]=='s')
        {
            return 0;
        }
    }
    return -1;
}