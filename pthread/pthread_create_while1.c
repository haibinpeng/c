#include<func.h>

void *pthread_func(void *p)
{//p=q;
    printf("I am child thread\n");
    while(1);
}

//pthread_create给子线程传递参数
int main()
{
    pthread_t pthid;
    char *q=(char *)malloc(100);
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);//第四个参数是给线程函数传递的参数
    if(ret!=0)
    {
        printf("pthread_create failed\n");
        return -1; 
    }
    usleep(1);//睡一下，否则子线程还没有创建成功，主线程就关闭了标准输出
    printf("I am main thread\n");
    while(1);
    return 0;
}