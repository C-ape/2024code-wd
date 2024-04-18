#include "func.h"

void cleanup(void* arg)
{
    printf("clean up malloc space\n");
    char* p = (char*)arg;
    free(p);
}


void* threadFunc(void* arg)
{
    char* buf = (char*)malloc(10);
    //一定要在cancel之前push
    pthread_cleanup_push(cleanup, (void*)buf);  //手动将资源清理函数入栈，如果线程意外终止了会执行资源清理函数
    strcpy(buf, "hello");
    puts(buf);
    pthread_exit((void*)0);    //线程启动函数中使用return的话，清理函数不会弹栈，最好使用pthread_exit
    //pthread_testcancel(); //手动增加取消点，这样pthread_cancel就不用等取消点了
    //int i;  //push和pop不能中间申请变量，外面使用变量，因为这两个函数合起来是一个do{}while
    pthread_cleanup_pop(1); //为0时，弹出栈顶的清理函数不执行，不为0时，弹出栈顶的清理函数执行,必须在同一个函数中和push成对出现
    //i = 0;
}   

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    printf("I am main pthread, tid = %lu\n", pthread_self());
    ret = pthread_cancel(tid);
    THREAD_ERROR_CHECK(ret, "pthread_cancel");
    void* pret;
    ret = pthread_join(tid, &pret);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    printf("pthread ret is %ld\n", (long)pret);
    return 0;

}