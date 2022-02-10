#include<func.h>
//嵌套锁,重复加锁时不会死锁

int main()
{
    pthread_mutexattr_t mutex_attr;//定义一个锁属性
    pthread_mutexattr_init(&mutex_attr);//初始化锁属性
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);//设置为嵌套锁
    pthread_mutex_t mutex;
    int ret=pthread_mutex_init(&mutex,&mutex_attr);//初始化锁,第二个参数为锁属性
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_mutex_lock(&mutex);//加锁
    printf("The first lock was succeeded\n");
    pthread_mutex_lock(&mutex);//加锁
    printf("The second lock was succeeded\n");
    return 0;
}