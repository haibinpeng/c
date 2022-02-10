#include<func.h>
//主线程加锁，子线程解锁，主线程再加锁，是可以的

void *thread_func(void *p)
{
    sleep(1);
    pthread_mutex_unlock((pthread_mutex_t *)p);//子线程解锁
}
int main()
{
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_t pthid;
    pthread_create(&pthid, NULL,thread_func,(void *)&mutex);
    pthread_mutex_lock(&mutex);//加锁
    printf("The first lock was succeeded\n");
    pthread_mutex_lock(&mutex);//加锁
    printf("The second lock was succeeded\n");
    return 0;
}