#include <func.h>
//主线程和子线程各加1千万,没加锁时会产生并发

#define N 10000000

void *pthread_func(void *p)
{
    int i;
    int *num=(int *)p;//定义成一个指针
    for(i=0;i<N;i++)
    {//子线程加1千万
        *num+=1;//取值后再去加
    }
}

int main()
{
    int num = 0;
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,(void *)&num);
    THREAD_ERR_CHECK(ret,"pthread_create")
    int i;
    for(i=0;i<N;i++)
    {//主线程加1千万
        num+=1;
    }
    pthread_join(pthid,NULL);//等待子线程
    printf("result=%d\n",num);
    return 0;
}