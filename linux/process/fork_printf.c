#include "func.h"

//fork的时候，父子进程用户态空间的数据是拷贝的(包括堆、栈、数据段)
//内核态的文件对象是共享的
int main()
{
    //for循环的fork问题需要展开for循环来求解
    for(int i = 0; i < 2; i++)
    {
        fork();
        //有换行，用户态缓冲区的数据已经被刷新到内核态文件对象中，父进程已经打印在屏幕上，此时fork，用户态里已经没有数据，所以只打印6个
        printf("-\n"); 
        //没有换行，父进程的-还在用户态缓冲区中，fork进程的时候用户态的数据也要复制，所以父进程的-也会复制给子进程
        //printf("-"); 
    }
    return 1;
}
