#include "factory.h"

//接收文件
int recv_file(int new_fd,char *filename,char *getname)
{
    train_state_t train;
    train_t train1;
    train_state_int traint;
    //pathbuf存文件路径
    char pathbuf[1000];
    bzero(pathbuf,sizeof(pathbuf));
    sprintf(pathbuf,"file_pool/%s",filename);
    //int fd=open(pathbuf,O_RDWR|O_CREAT);
    recv_int_train(new_fd,&traint);
    off_t file_size=traint.data;
    //扩大文件大小至要接收的数据量
    int fd=open(pathbuf,O_RDWR|O_CREAT,0666);
    perror("open");
    ftruncate(fd,file_size);
    void *p=mmap(NULL,file_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    perror("mmap");
    //接收文件数据
    int total=0;
    while(total<file_size)
    {
        int ret=recv_n(new_fd,p+total,4096);
        //printf("new_fd=%d\n",new_fd);
        if(ret==-1)
        {
            perror("recv");
        }
        if(0==ret)//代表对端断开了
        {
            break;
        }
        total+=ret;
    }
    munmap(p,file_size);
    printf("\ndownload complete.\n\n");
}


//发送文件
int tran_file(int new_fd,char *filename,int offset)
{
    train_state_t train;
    train_t train1;
    train_state_int traint;
    //pathbuf存文件路径
    char pathbuf[1000];
    bzero(pathbuf,sizeof(pathbuf));
    sprintf(pathbuf,"file_pool/%s",filename);

    struct stat buf;
    int ret=stat(pathbuf,&buf);//获取文件状态并复制到buf中
    //发文件长度
    off_t file_lenth=buf.st_size;
    off_t send_size=0;
    traint.data=file_lenth;
    //printf("6 lenth=%ld\n",file_lenth);
    send_int_train(new_fd,traint);
    //printf("7 send lenth=%d\n",traint.data);
    //打开文件
    int fd=open(pathbuf,O_RDWR);
    //读文件内容
    char databuf[4096]={0};
    if(offset==0)//偏移为0
    {
        if(file_lenth<-5000000)//小文件,逻辑有点绕，还没有写好
        {
            while(file_lenth>4096)
            {
                bzero(databuf,4096);
                read(fd,databuf,4096);
                send(new_fd,databuf,4096,0);
                file_lenth-=4096;
                send_size+=4096;
            }
            bzero(databuf,4096);
            read(fd,databuf,file_lenth);
            send(new_fd,databuf,file_lenth,0);
        }
        else
        {
            //发送文件内容
            void* p=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
            ret=send_n(new_fd,p,buf.st_size);
            munmap(p,buf.st_size);
        }
    }
    else//断点续传
    {
        printf("%d DATA FILE EXIST\n",offset);
        //发送文件内容
        void* p=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
        ret=send_n(new_fd,p+offset,buf.st_size-offset);
        munmap(p,buf.st_size);
    }
    printf("\nupload complete%%.\n\n");
}

//循环接收数据，直到接收到自己想要数量
int recv_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}

//循环发送数据，直到发送到自己想要数量
int send_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=send(sfd,p+total,data_len-total,0);
        if(-1==ret)//代表对端断开了
        {
            return -1;
        }
        total+=ret;
    }
    return 0;
}