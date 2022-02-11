#include<func.h>
//设置线程属性

void *thread_func(void *q)
{
    pthread_exit((void *)3);
}

int main()
{
    pthread_t pthid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);//线程属性初始化
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//设置线程属性为DETACHED
    int ret=pthread_create(&pthid, &attr,thread_func,NULL);//把线程属性传进来
    THREAD_ERR_CHECK(ret,"pthread_create")
    long thread_ret;
    ret=pthread_join(pthid,(void **)&thread_ret);//设置线程属性为DETACHED后,不能再join,会报错Invalid argument
    THREAD_ERR_CHECK(ret,"pthread_join")
    printf("child thread_ret=%ld\n",thread_ret);
    return 0;
}