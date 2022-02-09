#include<func.h>
//清理函数的执行顺序

void cleanup(void *p)
{//清理函数
    printf("I am cleanup func%ld\n",(long)p);
}

void *pthread_func(void *p)
{
    pthread_cleanup_push(cleanup,(void *)1);
    pthread_cleanup_push(cleanup,(void *)2);
    sleep(1);
    pthread_cleanup_pop(0);//pop(1)弹出清理函数并执行；pop(0)弹出清理函数但不执行
    pthread_cleanup_pop(0);
    return NULL;
}

int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create")
    //发送cancel给子线程
    ret=pthread_cancel(pthid);
    THREAD_ERR_CHECK(ret,"pthread_cancel")
    ret=pthread_join(pthid,NULL);//等待子线程
    THREAD_ERR_CHECK(ret,"pthread_join")
    printf("I am main thread\n");
    return 0;
}