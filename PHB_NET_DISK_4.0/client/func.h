#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "md5.h"

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)
#define SALT_LEN 15
#define _XOPEN_SOURCE

typedef struct task{
    char token[100];
    char filename[100];
    char md5name[100];
    int code;
    int state;
    int sfd;
    off_t offset;
    char ip[100];
    char port[100];
}tasknode,*ptasknode;

typedef struct{
    Que_t que;//队列
    pthread_cond_t cond;//条件变量
    pthread_t *pthid;//存放线程id
    short start_flag;//线程池启动标志，默认为0，启动后，设置为1
}Factory_t;

//不要交换data_len和buf的定义顺序
typedef struct{
    int data_len;//存放buf，也就是火车车厢中数据长度
    char buf[1000];//火车车厢长度
}train_t;

typedef struct{
    int data;//火车车厢长度
}train_state_int;

//带有状态码的火车
typedef struct{
    int data_len;//存放buf，也就是火车车厢中数据长度
    int state;//状态码
    char buf[1000];//火车车厢长度
}train_state_t;

enum command{
    REGISTER,
    LOGIN,
    LS,
    CD,
    PWD,
    REMOVE,
    GETS,
    PUTS,
    TOKEN,
    NORMAL,
    QUIT,
    FAULT,
    MKDIR,
    RMDIR,
};

// 还是token验证,登陆密码不对，发送失败后，直接close,密码正确后，会放任务到队列，子线程来处理
void check_token();
//进入登录界面之前使用，检查login锁,若为0则可登录,为1则堵塞
int check_login_mutex1(int mutex,char* mutex_buf,char* one_buf,char* zero_buf);
//客户端登陆成功后使用，每次输入命令时，检查login锁,若为0则可操作,为1则堵塞
int check_login_mutex2(int mutex,char* mutex_buf,char* one_buf,char* zero_buf,char* cmd);
//线程池初始化
int factory_init(Factory_t *pf,int thread_num,int capacity);
//启动线程池
int factory_start(Factory_t *pf,int thread_num);
//子线程gets流程
void* thread_handle_gets(void *p);
//子线程puts流程
void* thread_handle_puts(void *p);
//socket,bind,listen操作
int tcp_init(int* socket_fd,char* ip,char* port);
//发送文件
int tran_file(int new_fd);
//循环发送数据，直到发送到自己想要数量
int send_n(int sfd,void* buf,int data_len);

int recv_n(int sfd,void* buf,int data_len);
//epoll注册
int epoll_add(int epfd,int fd);
//客户需要登录，传状态码，用户名，密码,登陆成功的话取出token
int client_login(int sfd,char *token,char* username,char* userpwd);
//客户要注册
int client_register(int sfd,char *token,char* username,char* userpwd);
//客户要改变路径
int client_CD(int sfd,char* cmd);
//客户要显示当前目录下内容
int client_LS(int sfd);
//客户显示路径
int client_PWD(int sfd);
//客户删文件
int client_REMOVE(int sfd,char* cmd);
//客户登录后再登录
int client_LOGIN(int sfd);
//客户登录后再注册
int client_REGISTER(int sfd);
//客户新建文件夹
int client_mkdir(int sfd,char* cmd);
//客户删除文件夹
int client_rmdir(int sfd,char* cmd);
//客户退出
int client_QUIT(int sfd,char * username);
//接收一辆火车
int recv_train(int sfd,train_state_t *ptrain);
//发送一辆火车
int send_train(int sfd,train_state_t train);
//接收整型火车
int recv_int_train(int sfd,train_state_int *ptrain);
//发送整型火车
int send_int_train(int sfd,train_state_int train);
//整型火车装车
void set_int_train(train_state_int* ptrain,int data);
//火车装车
void set_train(train_state_t* ptrain,char* goods_buf);
//打印登录界面
void print_login();
//打印help
void print_help(char* username);
//欢迎语句以及help
void print_welcome_help();
//删除换行符
void delete_hh(char* target,int lenth);
//得到cd提供的用户的目标文件夹名
void get_filename(char* cmd,char *targetname);
//得到puts或gets的目标文件名
void get_filename_putsandgets(char* cmd,char *targetname);
//得到mkdir提供的用户的目标文件夹名
void get_dirname(char* cmd,char *targetname);
//得到remove提供的用户的目标文件名;
void get_remove_filename(char* cmd,char* targetname);
//判断是不是mkdir
int is_mkdir(char *cmd);
//判断是不是rmdir
int is_rmdir(char *cmd);
//判断是不是puts
int is_puts(char *cmd);
//判断是不是remove
int is_remove(char *cmd);
//判断是不是gets
int is_gets(char *cmd);
//根据状态判断是GETS，就调用do_gets,还是PUTS，do_puts
#endif