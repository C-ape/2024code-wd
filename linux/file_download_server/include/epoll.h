#ifndef _EPOLL_H
#define _EPOLL_H

#include "tcp.h"

#define MAXEVENTS 1024

//epoll相关的事件设置函数
int epollInit();
int epollAddReadEvent(int epfd, int fd);
int epollAddReadWriteEvent(int epfd, int fd);
int epollSetWriteEvent(int epfd, int fd);
int epollClearWriteEvent(int epdf, int fd);
int epollDelEvent(int epfd, int fd);

//事件处理函数
void epollEventLoop(int epfd, int fd);
void epollHandleNewConnection(int epfd, int fd, TcpCallback connectcb);
void epollHandleReadEvent(int epfd, int fd, TcpCallback messageReadCallback, TcpCallback closeCb);
void epollHandleWriteEvent(int epfd, int fd);


#endif