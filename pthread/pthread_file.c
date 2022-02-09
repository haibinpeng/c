#include <func.h>
//主线程和子线程任何一个线程关闭了文件后，其他线程就都读不了了

void *pthread_func(void *p)
{
    int fd=*(int *)p;
    close(fd);//子线程关闭文件
    printf("I am child thread\n");
}

int main()
{
    pthread_t pthid;
    int fd=open("file",O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    int ret=pthread_create(&pthid, NULL,pthread_func,&fd);
    THREAD_ERR_CHECK(ret,"pthread_create")
    sleep(1);
    printf("I am main thread\n");
    char buf[128]={0};
    ret=read(fd,buf,sizeof(buf));//子线程关闭文件后，主线程读不了
    printf("I am main thread,ret=%d,%s\n",ret,buf);
    return 0;
}