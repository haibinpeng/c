#include<func.h>

void *pthread_func(void *p)
{
    while(1);
    return NULL;
}

int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create")
    //发送cancel给子线程
    ret=pthread_cancel(pthid);//pthread_cancel是异步操作,cancel执行成功，不代表子线程被cancel掉了
    THREAD_ERR_CHECK(ret,"pthread_cancel")
    ret=pthread_join(pthid,NULL);//等待子线程
    THREAD_ERR_CHECK(ret,"pthread_join")
    printf("I am main thread\n");
    return 0;
}