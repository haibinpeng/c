#include <func.h>
//子线程的退出方式:线程函数return；任何一个函数调用pthread_exit
//任何一个线程调用exit，都会造成进程终止

void print()
{
    //return;//线程不会退出，能看到after print
    pthread_exit(NULL);//线程会退出，不能看到after print
}

void *pthread_func(void *p)
{
    printf("I am child thread,my id=%ld\n",pthread_self());
    print();
    printf("after print\n");
    return NULL;
}

int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid, NULL,pthread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create")
    sleep(1);
    printf("I am main thread,my child id=%ld\n",pthid);
    return 0;
}