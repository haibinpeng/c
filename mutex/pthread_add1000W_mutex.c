#include <func.h>
//主线程和子线程各加1千万,没加锁时会产生并发

#define N 10000000
typedef struct{
    int num;
    pthread_mutex_t mutex;
}data_t;

void *pthread_func(void *p)
{
    int i;
    data_t *d = (data_t *)p;
    for(i=0;i<N;i++)
    {//子线程加1千万
        pthread_mutex_lock(&d->mutex);//加锁
        d->num+=1;
        pthread_mutex_unlock(&d->mutex);//解锁
    }
}

int main()
{
    data_t d;
    d.num=0;
    int ret=pthread_mutex_init(&d.mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_t pthid;
    ret=pthread_create(&pthid, NULL,pthread_func,(void *)&d.num);
    THREAD_ERR_CHECK(ret,"pthread_create")
    int i;
    for(i=0;i<N;i++)
    {//主线程加1千万
        pthread_mutex_lock(&d.mutex);//加锁
        d.num+=1;
        pthread_mutex_unlock(&d.mutex);//解锁
    }
    pthread_join(pthid,NULL);//等待子线程
    printf("result=%d\n",d.num);
    ret=pthread_mutex_destroy(&d.mutex);//销毁锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
}