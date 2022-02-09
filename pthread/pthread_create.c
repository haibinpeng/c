#include<func.h>

void *pthread_func(void *p)
{//线程函数,pthread_func执行结束，线程就结束了
    printf("I am child thread\n");
}

//pthread开头的函数都不可以用perror来定位错误
//主线程结束后就会回收整个进程地址空间的资源
int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);//第四个参数是给线程函数传递的参数
    if(ret!=0)
    {
        printf("pthread_create failed\n");
        return -1; 
    }
    usleep(1);//睡一下，否则子线程还没有创建成功，主线程就关闭了标准输出
    printf("I am main thread\n");
    return 0;
}