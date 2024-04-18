#include "func.h"

//通过一个进程能够创建多少线程的方式检查THREAD_ERROR_CHECK这个接口对不对
void* threadFunc(void* arg)
{
    while(1){
        sleep(1); //不睡会卡死，主线程拿不到cpu了，退出不了
    }
}

int main()
{
    pthread_t tid;
    for (int i = 0; ;i++)
    {
        int ret = pthread_create(&tid, NULL, threadFunc, NULL);
        THREAD_ERROR_CHECK(ret, "pthread_create");
        if (ret != 0)
        {
            printf("i = %d\n", i);
            break;
        }
    }
}