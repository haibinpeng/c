#include<func.h>
//通过父进程和子进程各加1千万
//把线程内的锁运用到进程间

#define NUM 10000000

typedef struct{
    int num;
    pthread_mutex_t mutex;
}data_t;

int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget")
    printf("shmid=%d\n",shmid);
    data_t *p=shmat(shmid,NULL,0);//共享内存连接到本进程内
    ERROR_CHECK(p,(data_t *)-1,"shmat")
    p->num=0;//要加的值进行初始化
    pthread_mutexattr_t mutex_attr;//定义一个锁属性
    pthread_mutexattr_init(&mutex_attr);//初始化锁属性
    pthread_mutexattr_setpshared(&mutex_attr,PTHREAD_PROCESS_SHARED);//把锁属性设置为可以在进程之间使用的
    pthread_mutex_init(&p->mutex,&mutex_attr);//初始化锁
    int i;
    time_t start,end;//统计运行的秒数
    start=time(NULL);
    if(!fork())
    {
        for(i=0;i<NUM;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->num+=1;
            pthread_mutex_unlock(&p->mutex);
        }
        return 0;
    }else{
        for(i=0;i<NUM;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->num+=1;
            pthread_mutex_unlock(&p->mutex);
        }
        wait(NULL);
        end=time(NULL);
        printf("p->num=%d,use time=%ld\n",p->num,end-start);
        return 0;
    }
}