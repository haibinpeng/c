#include "factory.h"

//子线程流程
void* thread_handle(void *p)
{
    Factory_t* pf=(Factory_t*)p;
    pQue_t pq=&pf->que;
    pNode_t pcur=NULL;//取队列结点
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        pthread_cleanup_push(cleanup,&pq->mutex);
        if(0==pq->que_size)//如果队列为空就睡觉
        {
            pthread_cond_wait(&pf->cond,&pq->mutex);
        }
        que_get(pq,&pcur);//取结点
        pthread_cleanup_pop(1);//直接通过pop来解锁
        //printf("pcur=%d %d %d %d %s\n",pcur->new_fd,pcur->code,pcur->user_id,pcur->state,pcur->filename);
        if(pcur->state==GETS)
        {
            tran_file(pcur->new_fd,pcur->filename,pcur->offset);
            free(pcur);
            pcur=NULL;
        }
        else if(pcur->state==PUTS)
        {
            recv_file(pcur->new_fd,pcur->filename,pcur->getname);
            free(pcur);
            pcur=NULL;
        }
    }
}

//login操作
int server_login(int new_fd,train_state_t *ptrain,char *username,char *salt,char *password,pcque pclient_que)
{
    printf("login command from user %s\n",ptrain->buf);
    //存下用户名
    strcpy(username,ptrain->buf);
    //匹配用户名
    int check_ret=checkAddress(username,salt);
    //匹配成功就把salt传给客户端，如果匹配失败了，那就把这辆火车数据部分的长度设为0
    if(check_ret==0)//用户名匹配成功
    {
        //salt装车
        set_train(ptrain,salt);
        //传salt给客户端
        send_train(new_fd,*ptrain);
        //接收用户加密过后的密码
        recv_train(new_fd,ptrain);
        //存储该密码
        strcpy(password,ptrain->buf);
        //对比本地存储的加密密码
        check_ret=checkPwd(username,password);
        if(check_ret==0)//用户名密码都正确
        {
            //发一列非空火车，将salt当做token传过去
            set_train(ptrain,salt);
            send_train(new_fd,*ptrain);
            printf("user %s login success\n",username);
            //创建对应的client结点并加入队列
            enter_new_client(pclient_que,new_fd,username,salt);
            pclient_que->que_size++;
            printf("client enter,%d left\n\n",pclient_que->que_size);
            show_client(pclient_que->que_head,pclient_que->que_tail);
            userlogin_note(username);
        }else if(check_ret==-1){//如果加密密码不对，传终止火车,从头来过
            //传终止火车
            send_end_train(new_fd);
            return -1;
        }
    }else if(check_ret==-1)//用户名匹配不成功，传终止火车，从头来过
    {
        printf("login failed\n\n");
        //传终止火车
        send_end_train(new_fd);
        return -1;
    }
}

//regist操作
int server_regist(int new_fd,train_state_t* ptrain,char* username,char* salt,char* password,pcque pclient_que)
{
    printf("regist command from user %s\n",ptrain->buf);
    //存下用户名
    strcpy(username,ptrain->buf);
    //接收密码
    recv_train(new_fd,ptrain);
    //存下密码
    strcpy(password,ptrain->buf);
    //产生salt
    Makesalt(salt);
    //计算加密密码
    char *lp;
    lp=crypt(password,salt);//crypt是密码加密函数
    sprintf(password,"%s",lp);
    //查是否有重复的用户名
    int ret=check_duplicates(username);
    if(ret==-1)
    {
        printf("user %s name exited\n\n",username);
        //发一节终止火车告诉客户端注册失败
        send_end_train(new_fd);
        return -1;
    }else{//没有重复的用户名，开始注册
        //数据库中插入新用户信息
        ret=new_user_insert(username,salt,password);
        if(ret==0)//插入成功
        {
            printf("new user %s regist success\n",username);
        }
        //发一个token火车告诉客户端注册成功了，内含token(salt)
        set_train(ptrain,salt);
        send_train(new_fd,*ptrain);
        //注册后直接登陆
        //创建一个client结点，并加入队列
        enter_new_client(pclient_que,new_fd,username,salt);
        pclient_que->que_size++;
        printf("client enter,%d left\n\n",pclient_que->que_size);
        userlogin_note(username);
    }
}

//cd操作
void server_cd(pcnode p,train_state_t* ptrain)
{
    if(!strcmp(ptrain->buf,"a"))//来的是cd /
    {
        printf("cd / command in %s\n",p->client.path);
        //改code
        p->client.code=0;
        char buf[1000];
        sprintf(buf,"%s/",p->client.uname);
        //改路径
        strcpy(p->client.path,buf);
        bzero(buf,sizeof(buf));
        //buf存要发给客户端的当前路径通知
        sprintf(buf,"now user path is '%s'.",p->client.path);
        set_train(ptrain,buf);
        printf("%s\n",ptrain->buf);
        send_train(p->client.new_fd,*ptrain);
        //写cmd日志
        char* cmd="cd /";
        usercmd_note(p->client.uname,cmd,p->client.path);
        printf("command cd / success.\n");
    }
    else if(!strcmp(ptrain->buf,"b"))//来的是cd ..
    {
        printf("cd .. command in %s\n",p->client.path);
        //改code
        int nowcode=p->client.code;
        char buf[1000];
        if(nowcode==0)//在根目录
        {
            //已经在根目录了，发终止列车
            send_end_train(p->client.new_fd);
            printf("command cd .. failed.\n");
        }
        else
        {
            //拿precode
            int nowprecode=find_precode(nowcode);
            //改code
            p->client.code=nowprecode;
            //改路径
            goto_prepath(p->client.path);
            //buf存要发给客户端的当前路径通知
            bzero(buf,sizeof(buf));
            sprintf(buf,"now user path is '%s'.",p->client.path);
            set_train(ptrain,buf);
            send_train(p->client.new_fd,*ptrain);
            printf("command cd .. success.\n");
        }
        //写cmd日志
        char* cmd="cd ..";
        usercmd_note(p->client.uname,cmd,p->client.path);
    }
    else if(!strcmp(ptrain->buf,"c"))//来的是cd <dirname>
    {
        printf("cd <dirname> command in %s\n",p->client.path);
        //接收文件名
        recv_train(p->client.new_fd,ptrain);
        char file_name[101];
        strcpy(file_name,ptrain->buf);
        //找出文件名对应的code,注意不同用户文件可以重名，所以还要username
        int belongID;
        find_ID(p->client.uname,&belongID);
        int filecode=find_file_code(file_name,belongID,p->client.code);
        if(filecode==-1)//文件不存在，发送终止火车
        {
            send_end_train(p->client.new_fd);
            printf("command cd <dirname> failed.\n\n");
        }
        else//文件夹存在
        {
            //修改code
            p->client.code=filecode;
            char path_buf[101];
            bzero(path_buf,sizeof(path_buf));
            sprintf(path_buf,"%s/",file_name);
            //修改path,把当前路径和文件夹拼接
            strcat(p->client.path,path_buf);
            char buf[1000];
            bzero(buf,sizeof(buf));
            //buf存要发给客户端的当前路径通知
            sprintf(buf,"now user path is '%s'.",p->client.path);
            set_train(ptrain,buf);
            printf("%s\n",ptrain->buf);
            send_train(p->client.new_fd,*ptrain);
            //写cmd日志
            char cmd[200];
            sprintf(cmd,"cd %s",file_name);
            usercmd_note(p->client.uname,cmd,p->client.path);
            printf("command cd <dirname> success.\n");
        }
    }
}

//remove操作
void server_remove(pcnode p,train_state_t* ptrain)
{
    printf("user %s ask to remove.\n",p->client.uname);
    //接收文件名
    recv_train(p->client.new_fd,ptrain);
    char filename[100];
    strcpy(filename,ptrain->buf);
    int belongID;
    find_ID(p->client.uname,&belongID);
    long targetcode=0;
    char filemd5[200];
    //找目标文件夹,找不到返回-1;找到返回0,并取得code,拿取md5
    int ret=find_target_file(p->client.code,filename,belongID,filemd5);
    if(ret==0)//存在文件
    {
        int linknum;
        //删除文件表项
        delete_file(p->client.code,filename,belongID);
        //连接数减1
        linknum_reduce(filemd5);
        //如果减1后linknum降为0，删除表项
        delete_filepool(filemd5);
        char filermok[100]="remove success!";
        set_train(ptrain,filermok);
        send_train(p->client.new_fd,*ptrain);
        //记录cmdnote
        char cmd[200];
        sprintf(cmd,"remove %s",filename);
        usercmd_note(p->client.uname,cmd,p->client.path);
        printf("command remove <dirname> success.\n\n");
    }
    else if(ret==-1)
    {
        char nothisfile[100]="file not exist!";
        set_train(ptrain,nothisfile);
        send_train(p->client.new_fd,*ptrain);
        printf("command remove <dirname> failed.\n\n");
    }
}

//rmdir操作
void server_rmdir(pcnode p,train_state_t* ptrain)
{
    printf("user %s ask to rmdir.\n",p->client.uname);
    //接收文件名
    recv_train(p->client.new_fd,ptrain);
    char dirname[100];
    strcpy(dirname,ptrain->buf);
    int belongID;
    find_ID(p->client.uname,&belongID);
    long targetcode=0;
    //找目标文件夹,找不到返回-1，找到返回0,并取得code
    int ret=find_target_dir(p->client.code,dirname,belongID,&targetcode);
    if(ret==0)//存在文件夹
    {
        ret=find_sons_file(targetcode,belongID);
        if(ret==0)//没有子文件，可以删除
        {
            delete_dir(p->client.code,dirname,belongID);
            char dirrmok[100]="rmdir success!";
            set_train(ptrain,dirrmok);
            send_train(p->client.new_fd,*ptrain);
            //记录cmdnote
            char cmd[200];
            sprintf(cmd,"rmdir %s",dirname);
            usercmd_note(p->client.uname,cmd,p->client.path);
            printf("command rmdir <dirname> success.\n\n");
        }
        else if(ret==-1)//有子文件
        {
            char dirhasson[100]="dir has son file!";
            set_train(ptrain,dirhasson);
            send_train(p->client.new_fd,*ptrain);
            printf("command rmdir <dirname> failed1.\n\n");
        }
    }
    else if(ret==-1)
    {
        char nothisdir[100]="dir not exist!";
        set_train(ptrain,nothisdir);
        send_train(p->client.new_fd,*ptrain);
        printf("command rmdir <dirname> failed2.\n\n");
    }
}

//mkdir操作
void server_mkdir(pcnode p,train_state_t* ptrain)
{
    printf("user %s ask to mkdir.\n",p->client.uname);
    //接收文件名
    recv_train(p->client.new_fd,ptrain);
    char dirname[100];
    strcpy(dirname,ptrain->buf);
    int belongID;
    find_ID(p->client.uname,&belongID);
    //文件名查重
    int ret=check_dirname_duplicates(dirname,p->client.code,belongID);
    if(ret==0) //不重复
    {
        //插入新文件夹名
        insert_new_dir(p->client.code,dirname,belongID);
        char buf[200];
        bzero(buf,sizeof(buf));
        //buf存通知
        sprintf(buf,"mkdir %s in %s success.",dirname,p->client.path);
        set_train(ptrain,buf);
        printf("%s\n",ptrain->buf);
        send_train(p->client.new_fd,*ptrain);
        //记录cmdnote
        char cmd[200];
        sprintf(cmd,"mkdir %s",dirname);
        usercmd_note(p->client.uname,cmd,p->client.path);
        printf("command mkdir <dirname> success.\n\n");
    }
    else if(ret==-1)
    {
        send_end_train(p->client.new_fd);
        printf("command mkdir <dirname> failed.\n");
    }
}

int factory_init(Factory_t *pf,int thread_num,int capacity)
{
    que_init(&pf->que,capacity);//队列初始化
    int ret=pthread_cond_init(&pf->cond,NULL);//初始化线程池条件变量
    THREAD_ERR_CHECK(ret,"pthread_cond_init");
    pf->pthid=(pthread_t *)calloc(thread_num,sizeof(pthread_t));//为线程池的子线程id申请空间
    pf->start_flag=0;//启动标志初始化为0
    return 0;
}

int factory_start(Factory_t *pf,int thread_num)
{
    if(!pf->start_flag)//如果线程池没启动
    {
        int i;
        for(i=0;i<thread_num;i++)
        {
            pthread_create(pf->pthid+i,NULL,thread_handle,pf);
        }
        pf->start_flag=1;
    }
}

//释放锁资源
void cleanup(void *p)
{
    pthread_mutex_unlock((pthread_mutex_t*)p);
}

//接收一辆火车
int recv_train(int sfd,train_state_t *ptrain)
{
    int ret;
    bzero(ptrain,sizeof(*ptrain));
    ret=recv(sfd,&(ptrain->data_len), sizeof(int),0);
    ERROR_CHECK(ret,-1,"recv");
    ret=recv(sfd,ptrain,(ptrain->data_len)+8,0);
    ERROR_CHECK(ret,-1,"recv");
}

//发送一辆火车
int send_train(int sfd,train_state_t train)
{
    int ret;
    ret=send(sfd,&train.data_len,sizeof(int),0);
    ERROR_CHECK(ret,-1,"send");
    ret=send(sfd,&train,train.data_len+8,0);
    ERROR_CHECK(ret,-1,"send");
}

//发送一辆终止火车
int send_end_train(int new_fd)
{
    train_state_t train;
    bzero(&train,sizeof(train));//清空
    train.data_len=0;
    train.state=NORMAL;
    send_train(new_fd,train);
    return 0;
}

//火车装车
void set_train(train_state_t *ptrain,char *goods_buf)
{
    bzero(ptrain,sizeof(train_state_t));
    strcpy(ptrain->buf,goods_buf);
    ptrain->data_len=strlen(goods_buf);
    ptrain->state=NORMAL;
}

//接收int火车
int recv_int_train(int sfd,train_state_int *ptrain)
{
    int ret;
    bzero(ptrain,sizeof(train_state_int));
    ret=recv(sfd,ptrain,sizeof(train_state_int),0);
    ERROR_CHECK(ret,-1,"recv");
}

//发送int火车
int send_int_train(int sfd,train_state_int train)
{
    int ret;
    ret=send(sfd,&train,sizeof(train_state_int),0);
    ERROR_CHECK(ret,-1,"send");
}

//int火车装车
void set_int_train(train_state_int *ptrain,int data)
{
    bzero(ptrain,sizeof(train_state_int));
    ptrain->data=data;
}

//创建对应的client结点并加入队列
void enter_new_client(pcque pclientque,int new_fd,char *username,char *salt)
{
    pcnode pnew=(pcnode)malloc(sizeof(cnode));
    pnew->client.new_fd=new_fd;
    pnew->client.code=0;
    char path_buf[100];
    sprintf(path_buf,"%s/",username);
    strcpy(pnew->client.path,path_buf);
    strcpy(pnew->client.token,salt);
    strcpy(pnew->client.uname,username);
    clientque_insert(pclientque,pnew);//增加结点到用户队列
    printf("user %s's new_fd %d has entered client_que.\n",username,new_fd);
}

//遍历client队列并打印
void show_client(pcnode head,pcnode tail)
{
    if(head==tail)
    {
        printf("only username=%s\n",head->client.uname);
    }else{
        pcnode p=head;
        while(p!=tail)
        {
            printf("username=%s\n",p->client.uname);
            p=p->next_client;
        }
    }
}

//将path修改为上一级
void goto_prepath(char *path)
{
    int i=0;
    int num=0;
    int num1=0;
    char* p=path;
    //数有几个'/'
    while((*p!=' ')&&(*p!='\0'))
    {
        if(*p=='/')
        {
            num++;
        }
        p=p+1;
    }
    p=path;
    while(1)
    {
        if(*p=='/')
        {
            num1++;
        }
        if(num1==(num-1))
        {
            break;
        }
        p++;
    }
    p++;
    while(*p!='\0')
    {
        *p='\0';
    }
}

//token操作，包括puts,gets
int server_token(train_state_t* ptrain,int new_fd,pcque pclient_que,pNode_t pnew_task,pQue_t ptask_que,Factory_t *pf)
{
    train_state_t train;
    train_t train1;
    train_state_int traint;

    strcpy(train.buf,ptrain->buf);
    //printf("1 recv token=%s\n",train.buf);
    printf("\nA pthread comes\n");
    char token[100];
    bzero(token,sizeof(token));
    //存下发来的token(salt)
    strncpy(token,train.buf,sizeof(token));
    //接收filename
    char filename[100];
    recv_train(new_fd,&train);
    //printf("2 recv filename=%s\n",train.buf);
    bzero(filename,sizeof(filename));
    strncpy(filename,train.buf,sizeof(filename));
    //接收state
    int task_state;
    recv_int_train(new_fd,&traint);
    //printf("3 recv state=%d\n",traint.data);
    task_state=traint.data;
    //接收offset
    recv_int_train(new_fd,&traint);
    int task_offset=traint.data;
    //根据token(salt)找到用户名
    char username[1000];
    bzero(username,sizeof(username));
    int ret=get_salt_to_username(username,token);
    int userID=0;
    find_ID(username,&userID);
    if(ret==-1)//token错了
    {
        //发结束列车
        send_end_train(new_fd);
        //printf("4 send sign=%s\n",train.buf);
        printf("\ntoken wrong.\n");
        return -1;
    }
    else//token正确，拿到username
    {
        //发一个非空火车
        send_train(new_fd,train);
        //printf("4 send sign=%s\n",train.buf);
        //根据用户名找cnode
        int ID;
        pcnode p=pclient_que->que_head;
        while(1)
        {
            find_ID(p->client.uname,&ID);
            if(userID==ID)
            {
                break;
            }
            p=p->next_client;
        }
        char filemd5[1000];
        ret=find_target_file(p->client.code,filename,userID,filemd5);
        //如果是GETS,ret==0,表示有该文件，发给客户端
        //如果是PUTS,ret==-1,表示没有该文件，可以上传,ret=!ret,此时ret==0
        if(task_state==PUTS)
        {
            ret=!ret;
        }
        if(ret==0)//GETS时存在该文件，PUTS时不存在该文件
        {
            //发一个非空火车
            send_train(new_fd,train);
            //printf("5 send sign=%s\n",train.buf);
            if(task_state==PUTS)
            {
                //接收md5
                recv_train(new_fd,&train);
                strcpy(filemd5,train.buf);
                int ret=check_md5_exist(filemd5);
                if(ret==-1)//md5不存在
                {
                    printf("md5 not exist.\n");
                    insert_file(p->client.code,filename,userID,filemd5);
                    insert_filepool(filemd5);
                    send_train(new_fd,train);
                }
                else if(ret==0)//存在
                {   
                    printf("md5 exist.\n");
                    insert_file(p->client.code,filename,userID,filemd5);
                    linknum_add(filemd5);
                    send_end_train(new_fd);
                }
            }
            //初始化任务结点
            pnew_task=(pNode_t)malloc(sizeof(Node_t));
            pnew_task->new_fd=new_fd;
            pnew_task->offset=task_offset;
            pnew_task->code=p->client.code;
            pnew_task->user_id=userID;
            pnew_task->state=task_state;
            strncpy(pnew_task->getname,filename,sizeof(pnew_task->getname));
            strncpy(pnew_task->filename,filemd5,sizeof(pnew_task->filename));

            //结点加入链表，尾插
            pthread_mutex_lock(&ptask_que->mutex);//加锁，放队列，解锁
            que_set(ptask_que,pnew_task);
            //printf("%d\n",ptask_que->que_size);
            pthread_mutex_unlock(&ptask_que->mutex);
            pthread_cond_signal(&pf->cond);//使条件成立
            printf("\npthread wake up.\n");
            //写cmd日志
            if(task_state==GETS)
            {
                char cmd[200];
                sprintf(cmd,"gets %s",filename);
                usercmd_note(p->client.uname,cmd,p->client.path);
            }
            else
            {
                char cmd[200];
                sprintf(cmd,"puts %s",filename);
                usercmd_note(p->client.uname,cmd,p->client.path);
            }
        }
        else//GETS时不存在该文件，PUTS时已存在该文件
        {
            //printf("5 send sign=%s\n",train.buf);
            //发结束列车
            send_end_train(new_fd);
            printf("\filename wrong.\n");
            return -1;
        }
    }
}