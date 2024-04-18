#include "func.h"

#define NUM 10
typedef struct queue_s
{
    int elem[NUM]; //仓库元素
    int size;
    int front; //表示队首元素下标
    int rear; //表示下一个入队元素的下标
    /* data */
}queue_t;


//生产者消费者问题
//每0.5秒生产一个商品，每一秒消费一个商品，仓库大小为10，一开始有0个商品
typedef struct shareRes_s
{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

//生产者
void* produce(void* arg)
{
    shareRes_t* share_res = (shareRes_t*)arg;
    while(1)
    {
        pthread_mutex_lock(&share_res->mutex);
        if (share_res->queue.size == 10)
        {
            pthread_cond_wait(&share_res->cond, &share_res->mutex);
        }
        share_res->queue.rear = (share_res->queue.rear + 1) % NUM;
        ++share_res->queue.size;
        printf("produce size = %d, front = %d, rear = %d\n",
                share_res->queue.size, share_res->queue.front, share_res->queue.rear);
        if (share_res->queue.size > 0)
        {
            pthread_cond_signal(&share_res->cond);
        }
        pthread_mutex_unlock(&share_res->mutex);
        usleep(500000);
    }
}

//消费者
void* consumer(void* arg)
{
    shareRes_t* share_res = (shareRes_t*)arg;
    while(1)
    {
        pthread_mutex_lock(&share_res->mutex);
        if (share_res->queue.size == 0)
        {
            pthread_cond_wait(&share_res->cond, &share_res->mutex);
        }
        share_res->queue.front = (share_res->queue.front + 1) % NUM;
        --share_res->queue.size;
        printf("consum size = %d, front = %d, rear = %d\n",
                share_res->queue.size, share_res->queue.front, share_res->queue.rear);
        if (share_res->queue.size == 0)
        {
            pthread_cond_signal(&share_res->cond);
        }
        pthread_mutex_unlock(&share_res->mutex);
        sleep(1);
    }

}

int main()
{
    shareRes_t share_res;
    memset(&share_res, 0, sizeof(share_res));
    int ret = pthread_mutex_init(&share_res.mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    ret = pthread_cond_init(&share_res.cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_cond_init");
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, produce, (void*)&share_res);
    pthread_create(&tid2, NULL, consumer, (void*)&share_res);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_cond_destroy(&share_res.cond);
    pthread_mutex_destroy(&share_res.mutex);
    return 0;
}