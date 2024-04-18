#include "threadPool.h"


void* pollHandleEvent(void* arg)
{
    threadPool_t* pthreadPool = (threadPool_t*)arg;
    printf("thread pool start, tid = %ld\n", *pthreadPool->tid);
    while(1)
    {

    }
}

//初始化线程池
void threadPoolInit(threadPool_t* pthreadPool, int thread_num)
{
    pthreadPool->threadNum = thread_num;
    pthreadPool->tid = (pthread_t*)calloc(thread_num, sizeof(pthread_t));
    for (int i = 0; i < thread_num; i++)
    {
        pthread_create(&pthreadPool->tid[i], NULL, pollHandleEvent, (void*)pthreadPool);
    }
}
