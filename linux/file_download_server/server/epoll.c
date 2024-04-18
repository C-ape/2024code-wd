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

//删除事件
int epollDelEvent(int epfd, int fd)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    ERROR_CHECK(ret, -1, "epoll_ctl del");
    return 0;
}

//事件处理函数
void epollEventLoop(int epfd, int listenFd)
{
    epollAddReadEvent(epfd, listenFd);
    struct epoll_event* readyEvent = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
    while(1)
    {
        int nready = epoll_wait(epfd, readyEvent, MAXEVENTS, 100000);
        if (nready == 0)
        {
            printf("epoll time out\n");
        }else if (nready == -1 && errno == EINTR)
        {
            continue;
        }else
        {
            printf("nready = %d\n", nready);
            for (int i = 0; i < nready; i++)
            {
                int netfd = readyEvent[i].data.fd;
                if (netfd == listenFd)
                {
                    epollHandleNewConnection(epfd, netfd, connectCallback);
                }else
                {
                    //都是网络事件
                    if (readyEvent[i].events & EPOLLIN)
                    {
                        epollHandleReadEvent(epfd, netfd, messageReadCallback, closeCb);
                    }
                    if (readyEvent[i].events & EPOLLOUT)
                    {
                        epollHandleWriteEvent(epfd, netfd);
                    }
                }
            }
        }
    }
}
void epollHandleNewConnection(int epfd, int listenFd, TcpCallback connectcb)
{
    int netfd = accept(listenFd, NULL, NULL);
    ERROR_CHECK_RET_NULL(netfd, -1, "accept");
    //将新连接的accept的fd加入监听
    epollAddReadEvent(epfd, netfd);
    insertChannel(netfd);

    //连接回调函数
    connectcb(netfd);
}

void epollHandleReadEvent(int epfd, int netFd, TcpCallback messageReadCallback, TcpCallback closeCb)
{
    printf("handle read event\n");
    int idx = currentChannelIndex(netFd);
    ERROR_CHECK_RET_NULL(idx, -1, "currentChannelIndex netfd is not in channles");
    tcpChannel_t* channel = &channels[idx];

    //接收完一整个的train，后面的消息处理做具体的处理
    int nread = recv_train(netFd, &channel->recvTrain);

    if (nread == 0) //连接断开的处理
    {
        epollDelEvent(epfd, netFd); //从epoll监听队列中删除
        closeCb(netFd);
        deleteChannel(netFd);
    }else if (nread == -1)
    {
        perror("recv");
        return;
    }else
    {
        //消息处理函数
        messageReadCallback(netFd);
        //如果发送缓冲区中有数据，将事件设置为写事件，下一次epoll_wait处理
        if (channel->sendTrain.length > 0)
        {
            epollSetWriteEvent(epfd, netFd);
        }
        //处理完接收的数据后清空
        clearRecvBufferByIdx(idx);
    }
}



//1、fd的内核缓冲区中有空间(缓冲区没满)，就会触发写事件
//2、从不可写状态变为可写状态。
//这里直接发送一个train
void epollHandleWriteEvent(int epfd, int netFd)
{
    int idx = currentChannelIndex(netFd);
    if (idx != -1)
    {
        int sendLength = channels[idx].sendTrain.length + 8;  //8是length和type的长度
        int nsend = send(netFd, &channels[idx].sendTrain, sendLength, 0);
        if (nsend = sendLength)
        {
            //如果缓冲区的数据发送完，不要再监听写事件，发送完了一定要清除，这里是水平触发，不清除读事件会一直触发
            epollClearWriteEvent(epfd, netFd);
            clearSendBufferByIdx(idx);
        }else if (nsend > 0 && nsend < sendLength)
        {
            //如果只发送了部分数据，将未发送的数据移动到开始为止，下一个epoll_wait再发
            channels[idx].sendTrain.length = sendLength - nsend;
            memmove(&channels[idx].sendTrain, &channels[idx].sendTrain + nsend, sendLength - nsend);
        }
        printf("send data over\n");
    }else
    {
        printf("handle write idx not exist!\n");
    }
}