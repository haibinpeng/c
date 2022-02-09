#include<func.h>
//创建子线程后，子线程先malloc空间，然后在free之前被cancel掉，如果不清理，就会出现内存泄漏
//清理函数得到执行的情况1：线程被取消

void cleanup(void *p)
{//清理函数
    free(p);
    printf("I am cleanup func\n");
}

void *pthread_func(void *p)
{
    char *q=(char *)malloc(20);
    pthread_cleanup_push(cleanup,q);
    printf("malloc success\n");
    sleep(1);
    printf("start free\n");
    pthread_cleanup_pop(1);//pop(1)弹出清理函数并执行；pop(0)弹出清理函数但不执行
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