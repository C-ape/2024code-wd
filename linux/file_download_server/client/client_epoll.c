#include "epoll.h"

int epollInit()
{
    int epfd = epoll_create(1);
    return epfd;
}

//添加读事件
int epollAddReadEvent(int epfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl add");
    return 0;
}
//添加读写事件
int epollAddReadWriteEvent(int epfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLOUT|EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl epollout");
    return 0;
}

//将事件设置为读写事件
int epollSetWriteEvent(int epfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN|EPOLLOUT;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl set read");
    return 0;
}

//修改后，只监听读事件
int epollClearWriteEvent(int epfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl clear write");
    return 0;
}

//删除读写事件
int epollDelEvent(int epfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN|EPOLLOUT;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl del");
    return 0;
}