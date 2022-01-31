#include <func.h>

int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget")
    int ret=shmctl(shmid,IPC_RMID,NULL);
    ERROR_CHECK(ret,1,"shmctl")
    return 0;
}