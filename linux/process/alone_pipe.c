#include "func.h"

//pipe创建匿名管道，管道是半双工的，只能用于父子进程中的通信
//一条管道，单工通信
int main()
{
    int pipefd[2]; //一条管道的两端，pipefd[0]只能读，pipefd[1]只能写
    int ret = pipe(pipefd);
    ERROR_CHECK(ret, -1, "pipe");
    pid_t pid = fork(); //子进程也复制了父进程的一条管道，只是用户态，内核中还是只有一个管道缓冲区
    ERROR_CHECK(pid, -1, "fork");
    printf("pipefd[0] = %d, pipefd[1] = %d\n", pipefd[0], pipefd[1]);
    char buf[1024] = {0};
    if(pid == 0)
    {
        close(pipefd[0]);
        printf("this is child, pid = %d\n", getpid());
        write(pipefd[1], "hahaha", 6);
    }else
    {
        close(pipefd[1]);
        sleep(1);
        printf("this is father, pid = %d\n", getpid());
        read(pipefd[0], buf, sizeof(buf));
        puts(buf);
        wait(NULL);
    }
    return 0;
}