#include "func.h"
#include "client_func.h"

// 登录的结构体
loginMsg_t loginMsg;

char clientPwd[PWD_LENGTH] = {0};

int main(int argc, char *argv[])
{
    // ./client 192.168.14.9 1234
    // ARGS_CHECK(argc,3);
    Train_t loginTrain;
    int sockFd;
    tcpInit(&sockFd, SERVER_IP, SERVER_PORT);

    int epfd = epollInit();
    ERROR_CHECK(epfd, -1, "epoll_create");
    epollAddReadEvent(epfd, STDIN_FILENO); // 把stdin以读事件加入监听
    epollAddReadEvent(epfd, sockFd);       // 把已连接socket以读事件加入监听
    char buf[4096] = {0};
    struct epoll_event readyArr[2];
    Train_t clientTrain;        //client的小火车
    
login_start:
    while (1)
    {
        printf("请选择注册或者登陆：\n1:注册新账号\n2:已有账号直接登陆\n");
        int type;
        scanf("%d", &type);
        memset(&loginMsg, 0, sizeof(loginMsg_t));
        int ret = regLoginFunc(&loginTrain, type);
        if (ret == -1)
        {
            while (getchar() != '\n');
            printf("请输入正确的编号\n");
            continue;
        }
        else
        {
            break;
        }
    }
    // 先发送name给server，server返回salt再进行校验
    printf("send login message\n");
    int nsend = send(sockFd, &loginTrain, loginTrain.length + 8, 0);
    memset(clientPwd, 0, PWD_LENGTH);

    while (1)
    {
        int readyNum = epoll_wait(epfd, readyArr, 2, -1); // epoll_wait的返回值是就绪事件的个数
        for (int i = 0; i < readyNum; ++i)
        {
            if (readyArr[i].data.fd == STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                int ret = read(STDIN_FILENO, buf, sizeof(buf));
                if (ret == 0)
                {
                    goto end;
                }
                stdinMessageDeal(buf, &clientTrain);
                print();
                // 如果发送缓冲区中有数据，将事件设置为写事件，下一次epoll_wait处理
                if (clientTrain.length > 0)
                {
                    epollSetWriteEvent(epfd, sockFd);
                }
            }
            else if (readyArr[i].data.fd == sockFd)
            {
                if (readyArr[i].events & EPOLLIN)
                {
                    int nread = recv_train(sockFd, &clientTrain);
                    if (nread == 0) // 连接断开的处理
                    {
                        epollDelEvent(epfd, sockFd); // 从epoll监听队列中删除
                        goto end;
                    }
                    else if (nread == -1)
                    {
                        perror("recv");
                        goto end;
                    }
                    else
                    {
                        // 消息处理函数
                        if (netMessageDeal(&clientTrain) == -1)
                        {
                            memset(&loginMsg, 0, sizeof(loginMsg));
                            sleep(1);
                            goto login_start;
                        }
                        // 如果发送缓冲区中有数据，将事件设置为写事件，下一次epoll_wait处理
                        if (clientTrain.length > 0)
                        {
                            epollSetWriteEvent(epfd, sockFd);
                        }
                        // 处理完接收的数据后清空
                        //clearTrainBuffer(&clientTrain);
                    }
                }
                if (readyArr[i].events & EPOLLOUT)
                {
                    epollHandleWriteEvent(epfd, sockFd, &clientTrain);
                }
            }
        }
    }
end:

    close(epfd);
    close(sockFd);
}