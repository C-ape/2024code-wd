#include "func.h"
int main()
{
    pthread_t tid;
    tid = pthread_self(); //这个函数得到的tid是线程库自己维护的(不同的进程可能有相同的tid，不是内核中的tid，不真实)
    pthread_t real_tid;
    real_tid = syscall(SYS_gettid); //由系统调用得到的tid才是真实的tid，可以用于通信
    printf("pid = %d,real_tid = %lu, tid = %lu\n", getpid(), real_tid, tid);
    return 0;
}