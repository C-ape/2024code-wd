#include "func.h"

//两条管道，实现父子进程间的双工通信
int main()
{
    int pfr[2]; //父读，子写
    int pfw[2]; //父写，子读
    int ret1 = pipe(pfr);
    ERROR_CHECK(ret1, -1, "pipe pfr");
    int ret2 = pipe(pfw);
    ERROR_CHECK(ret2, -1, "pipe pfw");
    char buf[4094] = {0};
    pid_t pid;
    pid = fork();
    ERROR_CHECK(pid, -1, "fork");
    if(pid == 0)
    {
        close(pfr[0]);
        close(pfw[1]);
        write(pfr[1], "i am son", 8);
        read(pfw[0], buf, sizeof(buf));
        puts(buf);
    }else
    {
        close(pfr[1]);
        close(pfw[0]);
        sleep(1);
        memset(buf, 0, sizeof(buf));
        read(pfr[0], buf, sizeof(buf));
        puts(buf);
        write(pfw[1], "i am father", 20);
    }
}