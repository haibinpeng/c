#include<func.h>
//删除一个消息队列
int main()
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    int ret=msgctl(msgid,IPC_RMID,NULL);
    ERROR_CHECK(ret,-1,"msgctl")
    return 0;
}