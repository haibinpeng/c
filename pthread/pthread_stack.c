#include<func.h>

void *pthread_func(void *p)
{
    int *q=(int *)p;
    *q=5;
    printf("I am child thread\n");
}

//pthread_create给子线程传递参数
int main()
{
    pthread_t pthid;
    int num=10;
    int ret=pthread_create(&pthid, NULL,pthread_func,&num);//第四个参数是给线程函数传递的参数
    if(ret!=0)
    {
        printf("pthread_create failed\n");
        return -1; 
    }
    usleep(1);//睡一下，否则子线程还没有创建成功，主线程就关闭了标准输出
    printf("I am main thread,num=%d\n",num);
    return 0;
}
