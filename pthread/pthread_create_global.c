#include <func.h>
//全局变量，主线程和子线程的访问情况

int global=10;//定义一个全局变量

void *pthread_func(void *p)
{
    global=5;
    printf("I am child thread,global=%d\n",global);
}

int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);
    if(ret!=0)
    {
        printf("pthread_create failed %s\n",strerror(ret));
        return -1;
    }
    sleep(1);
    printf("I am main thread,global=%d\n",global);
    return 0;
}