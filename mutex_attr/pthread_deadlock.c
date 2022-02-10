#include<func.h>
//检错锁

int main()
{
    pthread_mutexattr_t mutex_attr;//定义一个锁属性
    pthread_mutexattr_init(&mutex_attr);//初始化锁属性
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);//设置为检错锁
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex,&mutex_attr);//初始化锁,第二个参数为锁属性
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_mutex_lock(&mutex);//加锁
    printf("The first lock was succeeded\n");
    ret=pthread_mutex_lock(&mutex);//设置了检错锁属性后,第二次加锁时会被检测
    THREAD_ERR_CHECK(ret,"pthread_mutex_lock")
    printf("The second lock was succeeded\n");
    return 0;
}