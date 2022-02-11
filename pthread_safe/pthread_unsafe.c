#include<func.h>
//ctime不是一个线程安全函数

void *thread_func(void *q)
{//立马打印当前时间，3s后再打印一次
    time_t now;
    time(&now);
    char *p=ctime(&now);
    printf("I am child thread,p=%p,ctime=%s\n",p,p);
    sleep(3);
    printf("I am child thread,second p=%p,second ctime=%s\n",p,p);//再次打印发现输出的时间和刚才打印的不一样,这里p指向的还是那段地址，但是主线程已经修改了
    pthread_exit(NULL);
}

int main()
{//主线程创建子线程，1s后获取当前时间打印
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,thread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create")
    sleep(1);
    time_t now;
    time(&now);
    char *p=ctime(&now);//事实上ctime里设置的是一个静态局部变量数组，p所指向的空间是一个静态局部变量,静态局部变量在函数执行以后不会得到释放
    printf("I am main thread,p=%p,ctime=%s\n",p,p);
    pthread_join(pthid,NULL);
    return 0;
}