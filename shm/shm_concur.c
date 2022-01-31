#include <func.h>
//并发
#define NUM 10000000
int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);//第一次是创建，第二次相同的key值就是获取
    ERROR_CHECK(shmid,-1,"shmget")
    printf("shmid=%d\n",shmid);
    int *p=shmat(shmid,NULL,0);//共享内存连接到本进程的进程地址空间
    ERROR_CHECK(p,-1,"shmat")
    p[0]=0;
    int i;
    //对内存操作产生了并发，没有加锁，造成最终不等于2千万
    if(!fork())
    {
        for(i=0;i<NUM;i++)
        {
            p[0]++;
        }
        return 0;
    }else{
        for(i=0;i<NUM;i++)
        {
            p[0]++;
        }
        wait(NULL);
        printf("p[0]=%d\n",p[0]);
        return 0;
    }
}