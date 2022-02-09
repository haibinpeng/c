#include <func.h>
//创建多个子线程,不同线程打印不同的编号

void *pthread_func(void *p)
{
    //printf("I am child thread %d\n",*(int *)p);//打印出来的都是2
    printf("I am child thread %ld\n",(long)p);//注意这里是long型
    return NULL;
}

int main()
{
    pthread_t pthid[2];
    long i;//long型
    int ret;
    // //为什么打印都是2，是因为子线程去打印时，主线程for循环结束，已经把i改为2
    // for(i=0;i<2;i++)
    // {
    //     ret=pthread_create(pthid+i, NULL,pthread_func,(void *)&i);
    //     THREAD_ERR_CHECK(ret,"pthread_create")
    // }
    // pthread_join(pthid[0],NULL);
    // pthread_join(pthid[1],NULL);
    
    //打印出来就是0和1
    for(i=0;i<2;i++)
    {
        ret=pthread_create(pthid+i, NULL,pthread_func,(void *)i);//把指针的空间用来传递一个整型数据
        THREAD_ERR_CHECK(ret,"pthread_create")
    }
    pthread_join(pthid[0],NULL);
    pthread_join(pthid[1],NULL);
    printf("I am main thread\n");
    return 0;
}