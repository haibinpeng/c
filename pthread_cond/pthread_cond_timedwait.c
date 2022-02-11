#include<func.h>
//线程等待,未超时退出，返回值为0

typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data_t;

void *thread_func(void *p)
{
    //子线程等待在对应的条件变量上
    printf("I am child thread,will wait\n");
    struct timespec abstime;
    abstime.tv_sec=time(NULL)+2;//绝对时间
    abstime.tv_nsec=0;//纳秒
    data_t *d=(data_t *)p;
    pthread_mutex_lock(&d->mutex);
    int ret=pthread_cond_timedwait(&d->cond, &d->mutex,&abstime);
    pthread_mutex_unlock(&d->mutex);
    printf("I am child thread,wake up,timedwait ret=%d\n",ret);
    pthread_exit(NULL);
}

int main()
{
    data_t d;
    int ret;
    ret=pthread_mutex_init(&d.mutex, NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    ret=pthread_cond_init(&d.cond,NULL);//初始化条件变量
    THREAD_ERR_CHECK(ret,"pthread_cond_init")
    //创建子线程
    pthread_t pthid;
    pthread_create(&pthid,NULL,thread_func,&d);
    sleep(1);
    //负责唤醒
    ret=pthread_cond_signal(&d.cond);//signal发出的像一个脉冲信号，只在一瞬间让条件成立，如果成立后才去排队，就不会被唤醒
    THREAD_ERR_CHECK(ret,"pthread_cond_signal")
    //join并回收子线程资源
    ret=pthread_join(pthid,NULL);
    THREAD_ERR_CHECK(ret,"pthread_join")
    printf("I am main thread\n");
    return 0;
}