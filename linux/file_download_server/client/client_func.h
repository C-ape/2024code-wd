#include "func.h"

#define MAXEVENTS 1024


#define RETRY_CNT 3 //密码重试次数



extern loginMsg_t loginMsg;
extern char clientPwd[PWD_LENGTH];     //全局的路径

//epoll相关的事件设置函数
int epollInit();
int epollAddReadEvent(int epfd, int fd);
int epollAddReadWriteEvent(int epfd, int fd);
int epollSetWriteEvent(int epfd, int fd);
int epollClearWriteEvent(int epdf, int fd);
int epollDelEvent(int epfd, int fd);
void epollHandleWriteEvent(int epfd, int netFd, Train_t* train);

int tcpInit(int* psockFd, char* ip, int port);
int regLoginFunc(Train_t* notloginTrain, int type);
int loginFunc(Train_t* train);
int regsiterFunc(Train_t* train);



int recvn(int sockFd, void *pStart, int length);
int recv_train(int fd, Train_t* train);
void closeCb(int netfd);

void clearTrainBuffer(Train_t* train);

int netMessageDeal(Train_t* train);
int stdinMessageDeal(char* buf, Train_t* clientTrain);

void print();