#include "func.h"

//线程启动函数的参数使用
//这里的参数传递，一个long类型刚好和指针的大小一样8个字节
//可以使用long类型的参数对函数编号
void* threadFunc(void* arg)
{
    long *num = (long*)arg; //这里得到的arg是8个字节，可以直接强转为long类型就行了
    (*num)++;
    printf("child num = %ld\n", *num);
    //while(1);
}

int main()
{
    pthread_t tid; //无符号长整型，用%lu
    long num = 1;  //long是8个字节，可以作为线程启动函数参数，不同的线程拥有不同的栈空间，所以传递方式是值传递(复制),传地址可修改
    int ret = pthread_create(&tid, NULL, threadFunc, (void*)&num);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    pthread_join(tid, NULL);
    printf("main long num is %ld\n", num);
    exit(0);
}
