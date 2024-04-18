#include "threadPool.h"
#include "epoll.h"
#include "mysql.h"

void parent_exec()
{
    if (fork() != 0)
    {
        wait(NULL);
        exit(0);
    }
}

int main()
{
    parent_exec();
    mysqlInit(&conn);
    initChannel();
    int thread_num = THREAD_NUM;
    threadPool_t threadPool;
    threadPoolInit(&threadPool, thread_num);
    int listenFd;
    tcpInit(&listenFd, SERVER_IP, SERVER_PORT);
    int epfd = epollInit();
    ERROR_CHECK(epfd, -1, "epoll_create");
    epollEventLoop(epfd, listenFd);

    close(epfd);
    close(listenFd);
    return 0;
}
