#include<func.h>
//创建一个信号量集合
int main()
{
    int semid=semget(1000,1,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(semid,-1,"semget");
    return 0;
}