#include "func.h"

//火车票问题
//初始票数20张，两个窗口卖票
//当票卖完了，另一个窗口补票10张，只补一次票
#define TICKETS 20000000
#define ADDTICKETS 10000000

typedef struct shareRes_s
{
    int tickets; //票数
    int flag; //是否可以加票的标志位
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;


void* sellTickets1(void* arg)
{
    shareRes_t* share_res = (shareRes_t*)arg;
    int mysell = 0;
    while(1)
    {
        pthread_mutex_lock(&share_res->mutex);
        if (share_res->tickets > 0)
        {
            //printf("before selltickets1 ticket = %d\n", share_res->tickets);
            share_res->tickets--;
            //printf("after selltickets1 ticket = %d\n", share_res->tickets);
            mysell++;
            if (share_res->tickets == 0 && share_res->flag == 0)
            {
                printf("sell1 signal\n");
                pthread_cond_signal(&share_res->cond);
                share_res->flag = 1;
            }
            pthread_mutex_unlock(&share_res->mutex);        
        }else if(share_res->tickets == 0 && share_res->flag == 2)
        {
            pthread_mutex_unlock(&share_res->mutex);
            break;
        }else
        {
            pthread_mutex_unlock(&share_res->mutex);
        }
    }
    printf("sellticket1 is %d\n", mysell);
    pthread_exit(NULL);
}

void* sellTickets2(void* arg)
{
    shareRes_t* share_res = (shareRes_t*)arg;
    int mysell = 0;
    while(1)
    {
        pthread_mutex_lock(&share_res->mutex);
        if (share_res->tickets > 0)
        {
            //printf("before selltickets2 ticket = %d\n", share_res->tickets);
            share_res->tickets--;
            mysell++;
            if (share_res->tickets == 0 && share_res->flag == 0)
            {
                printf("sell2 signal\n");
                pthread_cond_signal(&share_res->cond);
                share_res->flag = 1;
            }
            pthread_mutex_unlock(&share_res->mutex);
        }else if(share_res->tickets == 0 && share_res->flag == 2)
        {
            pthread_mutex_unlock(&share_res->mutex);
            break;
        }else
        {
            //其他情况都继续循环
            pthread_mutex_unlock(&share_res->mutex);
        }
    }
    printf("sellticket2 is %d\n", mysell);
    pthread_exit(NULL);
}

void* addTickets(void* arg)
{
    shareRes_t* share_res = (shareRes_t*)arg;
    pthread_mutex_lock(&share_res->mutex);
    if (share_res->flag == 0)
    {
        pthread_cond_wait(&share_res->cond, &share_res->mutex);
    }
    share_res->tickets = ADDTICKETS;
    share_res->flag = 2;    //要设置一下标志位，不然可能sell线程将flag置为1后，add线程仍然抢不到锁，sell线程就退出了
    printf("after addtickets ticket = %d\n", share_res->tickets);
    pthread_mutex_unlock(&share_res->mutex);
    pthread_exit(NULL);
}

int main()
{
    shareRes_t share_res;
    share_res.tickets = TICKETS;
    share_res.flag = 0;
    int ret = pthread_mutex_init(&share_res.mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    ret = pthread_cond_init(&share_res.cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_cond_init");
    //创建三个线程
    pthread_t tid1, tid2, tid3;
    ret = pthread_create(&tid3, NULL, addTickets, (void*)&share_res);
    THREAD_ERROR_CHECK(ret, "pthread_create3");
    usleep(50000);  //先创建一个加票的线程wait，不然后面的线程的signal接收不到
    ret = pthread_create(&tid1, NULL, sellTickets1, (void*)&share_res);
    THREAD_ERROR_CHECK(ret, "pthread_create1");
    ret = pthread_create(&tid2, NULL, sellTickets2, (void*)&share_res);
    THREAD_ERROR_CHECK(ret, "pthread_create2");

    //资源回收
    ret = pthread_join(tid1, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join1");
    ret = pthread_join(tid2, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join2");
    ret = pthread_join(tid3, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join3");
    ret = pthread_cond_destroy(&share_res.cond);
    THREAD_ERROR_CHECK(ret, "pthread_cond_destory");
    ret = pthread_mutex_destroy(&share_res.mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_destory");
    return 0;
}