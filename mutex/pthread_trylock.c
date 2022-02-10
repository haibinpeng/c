#include<func.h>

int main()
{
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_mutex_lock(&mutex);//加锁
    ret=pthread_mutex_trylock(&mutex);
    THREAD_ERR_CHECK(ret,"pthread_mutex_trylock")
    return 0;
}