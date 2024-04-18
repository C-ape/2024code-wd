#include "func.h"


#define THREAD_NUM 8    //线程池线程的数量


//线程池主要用于处理长命令
typedef struct threadPool_s
{
    pthread_t* tid;  //子线程数组
    int threadNum;  //子线程数量
}threadPool_t;



void threadPoolInit(threadPool_t* pthreadPool, int num);