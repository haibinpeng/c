#include <func.h>
//两个子线程卖票

typedef struct{
    int tickets;
    pthread_mutex_t mutex;
}data_t;

void *saler1(void *p)
{//窗口1卖票
    data_t *d = (data_t *)p;
    while(1)
    {
        //先加锁，后去读
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)
        {
            printf("I am saler1,start sale,rest tickets=%d\n",d->tickets);
            d->tickets--;
            pthread_mutex_unlock(&d->mutex);
            printf("I am saler1,finish sale,rest tickets=%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }
}

void *saler2(void *p)
{//窗口2卖票
    data_t *d = (data_t *)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)
        {
            printf("I am saler2,start sale,rest tickets=%d\n",d->tickets);
            d->tickets--;
            pthread_mutex_unlock(&d->mutex);
            printf("I am saler2,finish sale,rest tickets=%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }
}

int main()
{
    data_t d;
    d.tickets=20;//初始有20张票
    int ret=pthread_mutex_init(&d.mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_t pthid[2];
    ret=pthread_create(&pthid[0], NULL,saler1,(void *)&d);//创建窗口1的线程
    THREAD_ERR_CHECK(ret,"pthread_create")
    ret=pthread_create(&pthid[1], NULL,saler2,(void *)&d);//创建窗口2的线程
    THREAD_ERR_CHECK(ret,"pthread_create")
    int i;
    for(i=0;i<2;i++)
    {
        pthread_join(pthid[i],NULL);//等待子线程
    }
    printf("I am main thread,rest tickets=%d\n",d.tickets);
    ret=pthread_mutex_destroy(&d.mutex);//销毁锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
}