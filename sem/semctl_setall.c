#include<func.h>
//一次设置1个信号量集合中多个信号量的值
int main()
{
    int semid=semget(1000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    unsigned short arr[2]={2,6};
    //给两个信号量同时赋值
    int ret=semctl(semid,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl")
    //获取信号量里边每一个信号量的值
    bzero(arr,sizeof(arr));//清空arr
    ret=semctl(semid,0,GETALL,arr);
    printf("0号信号量=%d,1号信号量=%d\n",arr[0],arr[1]);
    return 0;
}