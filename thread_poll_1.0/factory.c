#include "factory.h"

int factory_init(Factory_t *pf,int thread_num,int capacity)
{
    que_init(&pf->que,capacity);//队列初始化
    int ret=pthread_cond_init(&pf->cond,NULL);
    THREAD_ERR_CHECK(ret,"pthread_cond_init");
    pf->pthid=(pthread_t *)calloc(thread_num,sizeof(pthread_t));//为线程id申请空间
    pf->start_flag=0;
    return 0;
}

int factory_start(Factory_t *pf,int thread_num)
{
    if(!pf->start_flag)//如果线程池没启动
    {
        int i;
        for(i=0;i<thread_num;i++)
        {
            pthread_create(pf->pthid+i,NULL,thread_handle,pf);
        }
        pf->start_flag=1;
    }
}
//子线程流程
void* thread_handle(void *p)
{
    Factory_t* pf=(Factory_t*)p;
    pQue_t pq=&pf->que;
    pNode_t pcur=NULL;//取队列结点
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        if(0==pq->que_size)//如果队列为空就睡觉
        {
            pthread_cond_wait(&pf->cond,&pq->mutex);
        }
        que_get(pq,&pcur);//取结点
        pthread_mutex_unlock(&pq->mutex);
        if(pcur)//pcur不为NULL代表拿到了结点
        {
            tran_file(pcur->new_fd);
            free(pcur);
            pcur=NULL;
        }
    }
}