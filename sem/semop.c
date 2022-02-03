#include<func.h>

#define NUM 10000000
int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget")
    printf("shmid=%d\n",shmid);
    int *p=shmat(shmid,NULL,0);
    ERROR_CHECK(p,-1,"shmat")
    p[0]=0;
    int i;
    int semid=semget(1000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget")
    int ret=semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    struct sembuf sopp,sopv;//结构体
    sopp.sem_num=0;//操作的是几号信号量
    sopp.sem_op=-1;//减一操作就是p(加锁)操作
    sopp.sem_flg=SEM_UNDO;
    sopv.sem_num=0;
    sopv.sem_op=1;//加一操作就是v(解锁)操作
    sopv.sem_flg=SEM_UNDO;
    time_t start,end;//统计运行的秒数
    start=time(NULL);
    if(!fork())
    {
        for(i=0;i<NUM;i++)
        {
            //加锁，p操作，原子操作
            semop(semid,&sopp,1);
            p[0]++;
            //解锁，v操作，原子操作
            semop(semid,&sopv,1);
        }
        return 0;
    }else{
        for(i=0;i<NUM;i++)
        {
            //加锁，p操作，原子操作
            semop(semid,&sopp,1);
            p[0]++;
            //解锁，v操作，原子操作
            semop(semid,&sopv,1);
        }
        wait(NULL);
        end=time(NULL);
        printf("p[0]=%d,use time=%ld\n",p[0],end-start);
        return 0;
    }
}