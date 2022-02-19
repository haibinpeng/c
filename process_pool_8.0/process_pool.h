#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<signal.h>
#include<pthread.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/epoll.h>
#include <sys/sendfile.h>

#define ARGS_CHECK(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,func_name) {if(ret==retval) {printf("errno=%d,",errno);fflush(stdout);perror(func_name);return -1;}}
#define THREAD_ERR_CHECK(ret,func_name) {if(ret!=0) {printf("%s failed,error num=%d,%s\n",func_name,ret,strerror(ret));return -1;}}
#define SELFFUNC_ERR_CHECK(ret,func_name) {if(ret!=0) {printf("%s failed\n",func_name);return -1;}}

#define FILENAME "file"

//父进程管理子进程所使用的数据结构
typedef struct{
    pid_t pid;//每个子进程的pid
    int fd;//子进程的对端
    short busy;//子进程是否忙碌，0代表非忙碌，1代表忙碌
}process_data_t;

typedef struct{
    int data_len;//火车头，记录火车装载内容长度
    char buf[1000];//火车车厢
}train_t;

//创建多个子进程
int make_child(process_data_t *p_manage,int process_num);
//子进程的代码流程
int child_handle(int fd);
//socket,bind,listen的初始化过程
int tcp_init(int* socket_fd,char* ip,char* port);
//对fd进行注册，监控它是否可读
int epoll_add(int epfd,int fd);
//发送描述符
int send_fd(int sfd,int fd,int exit_flag);
//接收描述符
int recv_fd(int sfd,int *fd,int *exit_flag);
//发送文件给客户端
int trans_file(int new_fd);
//循环接收数据
int recv_n(int sfd,void* buf,int data_len);
//循环发送数据
int send_n(int sfd,void* buf,int data_len);