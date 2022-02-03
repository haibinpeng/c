#include<func.h>

struct MSG{//消息结构体
    long mtype;//消息类型
    char buf[64];
};

int main()
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct MSG msg;
    bzero(&msg,sizeof(msg));
    int ret=msgrcv(msgid,&msg,sizeof(msg),1,0);//最后一个参数flag填0时，消息读完后再读会卡住
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("get message=%s\n",msg.buf);
    return 0;
}