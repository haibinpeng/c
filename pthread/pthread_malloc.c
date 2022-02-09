#include<func.h>

void *pthread_func(void *p)
{//p=q;
    strcpy(p,"hello");//子线程给空间中放一个hello
    printf("I am child thread\n");
}

//pthread_create给子线程传递参数
int main()
{
    pthread_t pthid;
    char *q=(char *)malloc(100);
    int ret=pthread_create(&pthid, NULL,pthread_func,q);//第四个参数是给线程函数传递的参数
    if(ret!=0)
    {
        printf("pthread_create failed\n");
        return -1; 
    }
    sleep(1);//睡一下，否则子线程还没有创建成功，主线程就关闭了标准输出
    printf("I am main thread\n");
    printf("get %s\n",q);
    return 0;
}