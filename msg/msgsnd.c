#include<func.h>

struct MSG{//消息结构体
    long mtype;//消息类型
    char buf[64];
};

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3)
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct MSG msg;
    msg.mtype=atoi(argv[1]);
    strcpy(msg.buf,argv[2]);
    int ret=msgsnd(msgid,&msg,strlen(msg.buf),0);
    ERROR_CHECK(ret,-1,"msgsnd");
    return 0;
}