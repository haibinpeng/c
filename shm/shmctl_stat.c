#include <func.h>
//获取和修改共享内存的属性信息
int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget")
    struct shmid_ds buf;
    int ret=shmctl(shmid,IPC_STAT,&buf);//查看
    ERROR_CHECK(ret,1,"shmctl")
    printf("cuid=%d,mode=%o,size=%ld,attach=%ld\n",buf.shm_perm.cuid,buf.shm_perm.mode,\
    buf.shm_segsz,buf.shm_nattch);
    buf.shm_perm.mode=0666;
    ret=shmctl(shmid,IPC_SET,&buf);//修改
    ERROR_CHECK(ret,1,"shmctl1")
    return 0;
}