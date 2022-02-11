#include<func.h>
//ctime_r是一个线程安全函数

void *thread_func(void *q)
{//立马打印当前时间，3s后再打印一次
    time_t now;
    time(&now);
    char buf_time[128];
    ctime_r(&now, buf_time);
    printf("I am child thread,ctime_r=%s\n",buf_time);
    sleep(3);
    printf("I am child thread,second ctime_r=%s\n",buf_time);
    pthread_exit(NULL);
}

int main()
{//主线程创建子线程，1s后获取当前时间打印
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,thread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create")
    sleep(1);
    time_t now;
    time(&now);
    char buf_time[128];
    ctime_r(&now, buf_time);
    printf("I am main thread,ctime_r=%s\n",buf_time);
    pthread_join(pthid,NULL);
    return 0;
}