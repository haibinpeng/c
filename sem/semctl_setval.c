#include<func.h>

int main()
{
    //创建一个信号量集合
    int semid=semget(1000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget")
    //初始化信号量的值为1
    int ret=semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl")
    //获取信号量里面的值
    ret=semctl(semid,0,GETVAL);
    ERROR_CHECK(ret,-1,"semctl")
    printf("信号量中的值=%d\n",ret);
    return 0;
}