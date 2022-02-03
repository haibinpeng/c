#include<func.h>
//删除一个信号量集合
int main()
{
    int semid=semget(1000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    int ret=semctl(semid,0,IPC_RMID);
    ERROR_CHECK(ret,-1,"semctl")
    return 0;
}