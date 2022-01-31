#include <func.h>

int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);//第一次是创建，第二次相同的key值就是获取
    ERROR_CHECK(shmid,-1,"shmget")
    printf("shmid=%d\n",shmid);
    char *p=shmat(shmid,NULL,0);//共享内存连接到本进程的进程地址空间
    ERROR_CHECK(p,(char*)-1,"shmat")
    int ret=shmdt(p);//解除和共享内存段的映射
    ERROR_CHECK(ret,-1,"shmdt")
    while(1);
    return 0;
}