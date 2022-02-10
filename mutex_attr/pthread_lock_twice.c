#include<func.h>
//同一个线程对一把锁加锁两次会产生死锁

int main()
{
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_mutex_lock(&mutex);//加锁
    printf("The first lock was succeeded\n");
    pthread_mutex_lock(&mutex);//加锁
    printf("The second lock was succeeded\n");
    return 0;
}