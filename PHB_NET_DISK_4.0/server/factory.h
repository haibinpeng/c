#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "md5.h"

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)
#define SALT_LEN 15
#define PSWD_LEN 100
#define USERNAME_LEN 100

typedef struct{
    int data;//火车车厢长度
}train_state_int;

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
    //默认类型
    NORMAL,
    //退出指令
    QUIT,
    FAULT,
    MKDIR,
    RMDIR,
};
//针对每个客户端需要有状态维护
typedef struct{
    int new_fd;//对应客户端连接
    char uname[20];//监控的用户名，用来管理队列，防止重复
    char path[100];//路径，给cd服务的，pwd服务的
    int code;//code为0代表是根，code为正，直接数据库select
    char token[100];//一旦登录，随机生成一个token并发送给客户端
}Client_State_t;

//用户服务表结点
typedef struct client_node{
    Client_State_t client;
    struct client_node * next_client;
}cnode,*pcnode;

//用户服务队列
typedef struct client_que{
    pcnode que_head,que_tail;//队列头队列尾
    int que_capacity;//队列能力
    int que_size;//当前队列大小
}cque,*pcque;

//释放锁资源
void cleanup(void *p);
//login操作
int server_login(int new_fd,train_state_t* ptrain,char* username,char* salt,char* password,pcque pclient_que);
//regist操作
int server_regist(int new_fd,train_state_t* ptrain,char* username,char* salt,char* password,pcque pclient_que);
//cd操作
void server_cd(pcnode p,train_state_t* ptrain);
//remove操作
void server_remove(pcnode p,train_state_t* ptrain);
//rmdir操作
void server_rmdir(pcnode p,train_state_t* ptrain);
//mkdir操作
void server_mkdir(pcnode p,train_state_t* ptrain);
//token操作，包括puts和gets
int server_token(train_state_t* ptrain,int new_fd,pcque pclient_que,pNode_t pnew_task,pQue_t ptask_que,Factory_t* pf);
//用户队列，初始化
void clientque_init(pcque clientque);
//用户队列，增
void clientque_insert(pcque clientque,pcnode pcnew);
//用户队列，寻找当前username，若找到，返回0，该节点指针存入ptarget，没找到返回-1
int clientque_find(pcque clientque,char* username,pcnode ptarget);
//线程池初始化
int factory_init(Factory_t *pf,int thread_num,int capacity);
//启动线程池
int factory_start(Factory_t *pf,int thread_num);
//socket,bind,listen操作
int tcp_init(int* socket_fd,char* ip,char* port);
//发送文件
int tran_file(int new_fd,char *filename,int offset);
//循环发送数据，直到发送到自己想要数量
int send_n(int sfd,void* buf,int data_len);
//循环接收数据，直到发送到自己想要数量
int recv_n(int sfd,void* buf,int data_len);
//epoll注册
int epoll_add(int epfd,int fd);
//产生salt
int Makesalt(char *str);
//判断用户名是否存在，匹配成功获取salt
int checkAddress(char*,char*);
//判断密码是否正确
int checkPwd(char *,char*);
//用户登录记录
int userlogin_note(char *username);
//用户指令记录
int usercmd_note(char *username,char *usercmd,char *cmdpath);
//接收一辆火车
int recv_train(int sfd,train_state_t *ptrain);
//发送一辆火车
int send_train(int sfd,train_state_t train);
//发送一辆终焉の火车
int send_end_train(int new_fd);
//火车装车
void set_train(train_state_t* ptrain,char* goods_buf);
//接收int火车
int recv_int_train(int sfd,train_state_int *ptrain);
//发送int火车
int send_int_train(int sfd,train_state_int train);
//装int火车
void set_int_train(train_state_int* ptrain,int data);
//用户名查重，没有重复返回0，重复返回-1
int check_duplicates(char * usrA);
//查MD5是否存在存在返回0，不存在返回-1
int check_md5_exist(char* md5);
//文件夹名查重,没有重复返回0，重复返回-1
int check_dirname_duplicates(char* dirname,int precode,int belongID);
//插入新用户信息
int new_user_insert(char* Username,char* salt,char* UserPasswd);
//进行一次遍历查找，创建对应的client加入队列
void enter_new_client(pcque pclientque,int new_fd,char* username,char* salt);
//打开数据库
int mySqlInit(MYSQL **conn,char *query);
//遍历clientque并打印
void show_client(pcnode head,pcnode tail);
//根据用户名找到ID，存入pID指向的位置
int find_ID(char *username,int *pID);

int find_ID2(char *username,int *pID);
//获取当前路径下某个用户的文件信息，并发给用户
int send_in_path(int new_fd,int code,int belongID);
//找到文件对应的code,成功返回code,失败返回-1
int find_file_code(char *file_name,int belongID,int precode);
//找到code对应的precode,由返回值传出
int find_precode(int code);
//把path修改为上一级
void goto_prepath(char *path);
//插入新文件夹名
int insert_new_dir(int precode,char* dirname,int belongID);
//找是否有目标文件夹
int find_target_dir(int precode,char* dirname,int belongID,long* targetcode);
//找是否有目标文件,有的话就拿取md5
int find_target_file(int precode,char* filename,int belongID,char* filemd5);
//找文件夹内是否有子文件,找到返回-1，没找到返回0
int find_sons_file(int code,int belongID);
//删除目标文件夹
int delete_dir(int precode,char* dirname,int belongID);
//删除目标文件
int delete_file(int precode,char* filename,int belongID);
//删除文件的文件池表项
void delete_filepool(char* filemd5);
//被remove的文件linknum减1
void linknum_reduce(char* filemd5);
//得到对应文件linknum并返回这个值
int get_linknum(char* filemd5);
//根据token得到username,如果salt是对的，返回0，否则返回-1
int get_salt_to_username(char* username,char* salt);
//子线程流程
void* thread_handle(void *p);
//得到任务结点
void que_get(pQue_t pq,pNode_t *p);
//插入文件池项
void insert_filepool(char* md5);
//文件linknum加一
void linknum_add(char* filemd5);
//接收文件
int recv_file(int new_fd,char* filename,char* getname);
//插入一个文件项
void insert_file(int precode,char* filename,int belongID,char* md5);
#endif