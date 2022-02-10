#include<func.h>
//未解锁的情况下销毁锁,报错Device or resource busy

int main()
{
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_mutex_lock(&mutex);//加锁
    ret=pthread_mutex_destroy(&mutex);
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy")
    return 0;
}