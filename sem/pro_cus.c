#include<func.h>
//计数信号量的优势
//通过计数信号量实现生产者消费者模型
int main()
{
    int semid=semget(1000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    //0号信号量代表产品个数,1号信号量代表空位个数
    unsigned short arr[2]={5,5};
    //给两个信号量同时赋值
    int ret=semctl(semid,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl")
    //父进程生产者，子进程消费者
    if(!fork())
    {
        //消费者
        while(1)
        {
            struct sembuf sopp,sopv;
            sopp.sem_num=0;//0号信号量，产品
            sopp.sem_op=-1;
            sopp.sem_flg=SEM_UNDO;
            sopv.sem_num=1;//1号信号量，空位
            sopv.sem_op=1;
            sopv.sem_flg=SEM_UNDO;
            printf("我是消费者,产品数目=%d,空位数=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            semop(semid,&sopp,1);
            printf("开始消费...\n");
            semop(semid,&sopv,1);
            sleep(2);
        }
        return 0;
    }
    else
    {
        while(1)
        {
            struct sembuf sopp,sopv;
            sopp.sem_num=1;//1号信号量，空位
            sopp.sem_op=-1;
            sopp.sem_flg=SEM_UNDO;
            sopv.sem_num=0;//0号信号量，产品
            sopv.sem_op=1;
            sopv.sem_flg=SEM_UNDO;
            printf("我是生产者,产品数目=%d,空位数=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            semop(semid,&sopp,1);
            printf("开始生产...\n");
            semop(semid,&sopv,1);
            sleep(2);
        }
        return 0;
    }
}