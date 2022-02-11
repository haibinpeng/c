#include <func.h>
//两个子线程卖票,一个子线程放票

typedef struct{
    int tickets;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
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
            if(0==d->tickets)
            {
                pthread_cond_signal(&d->cond);//当票数为0时，通知放票
            }
            pthread_mutex_unlock(&d->mutex);
            printf("I am saler1,finish sale,rest tickets=%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
    }
}

void *saler2(void *p)
{//窗口2卖票
    data_t *d = (data_t *)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);//加锁
        if(d->tickets>0)
        {
            printf("I am saler2,start sale,rest tickets=%d\n",d->tickets);
            d->tickets--;
            if(0==d->tickets)
            {
                pthread_cond_signal(&d->cond);//当票数为0时，通知放票
            }
            pthread_mutex_unlock(&d->mutex);
            printf("I am saler2,finish sale,rest tickets=%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
    }
}

void *set_tickets(void *p)
{//放票
    data_t *d = (data_t *)p;
    pthread_mutex_lock(&d->mutex);//加锁
    if(d->tickets>0)
    {
        pthread_cond_wait(&d->cond, &d->mutex);//有票就去阻塞睡觉，睡觉时会解锁,另外两个窗口就可以卖票
    }
    d->tickets=10;//重新设置票数为10
    pthread_mutex_unlock(&d->mutex);//解锁
    pthread_exit(NULL);
}

int main()
{
    data_t d;
    d.tickets=20;//初始有20张票
    int ret=pthread_cond_init(&d.cond, NULL);//初始化条件变量
    THREAD_ERR_CHECK(ret,"pthread_cond_init")
    ret=pthread_mutex_init(&d.mutex, NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init")
    pthread_t pthid[3];
    ret=pthread_create(&pthid[0], NULL,saler1,(void *)&d);//创建窗口1的线程
    THREAD_ERR_CHECK(ret,"pthread_create1")
    ret=pthread_create(&pthid[1], NULL,saler2,(void *)&d);//创建窗口2的线程
    THREAD_ERR_CHECK(ret,"pthread_create2")
    ret=pthread_create(&pthid[2], NULL,set_tickets,(void *)&d);//创建放票的线程
    THREAD_ERR_CHECK(ret,"pthread_create3")
    int i;
    for(i=0;i<3;i++)
    {
        pthread_join(pthid[i],NULL);//等待子线程
    }
    printf("I am main thread,rest tickets=%d\n",d.tickets);
    ret=pthread_mutex_destroy(&d.mutex);//销毁锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy")
    ret=pthread_cond_destroy(&d.cond);//销毁条件变量
    THREAD_ERR_CHECK(ret,"pthread_cond_destroy")
    return 0;
}