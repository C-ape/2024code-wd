#include "func.h"

//被调函数中函数值传递和指针传递的问题
void* threadFunc(void* arg)
{
    printf("this is child thread\n");
    //long i = 1234; //long就是8个字节，可以直接转成void*
    //pthread_exit((void*)i);
    //这里必须使用static，因为如果子线程退出了，不用static i就被销毁了，传给pthread_exit函数的指针就是乱止的，结果就是不对的
    //而使用long是ok，是因为long类型直接传的值，相当于复制了一份，所以pthread_join能够接收到
    static int i = 1234; // 这里需要使用static，因为子线程退出了，
    pthread_exit((void*)&i);
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    int* thread_ret; // 获取线程的返回值
    //void* thread_ret;
    //pthread_join(tid, &thread_ret);
    //void*就是八个字节，可以直接用long强转 
    //printf("thread_ret = %ld\n", (long)thread_ret);
    pthread_join(tid, (void**)&thread_ret);
    printf("thread_ret = %d\n", *thread_ret);
    return 0;
}