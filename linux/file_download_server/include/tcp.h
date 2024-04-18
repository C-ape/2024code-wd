
#ifndef _TCP_H
#define _TCP_H
#include "func.h"


#define CHANNELS_SIZE 1000  //1000连接的tcp通道



//tcp通道相关，用于主线程处理客户端的请求
typedef struct TcpChannel{
    int netfd;
    int fileId;     //当前用户所在目录的对应虚拟文件表中的id，/root/dir,fileId是dir的id
    char username[NAME_LENGTH]; //用户登录过后通过这个name查询数据库
    Train_t recvTrain;  //发送缓冲区
    Train_t sendTrain;  //接收缓冲区
}tcpChannel_t;

extern tcpChannel_t channels[CHANNELS_SIZE];


typedef void(*TcpCallback) (int);

//通道管理操作
void initChannel();
int currentChannelIndex(int netfd);
int insertChannel(int netfd);
void deleteChannel(int netfd);

//对某一个通道的操作
// 对某一个通道的操作
void clearSendBufferByIdx(int idx);
void clearRecvBufferByIdx(int idx);
void clearSendBuffer(tcpChannel_t* channel);
void clearRecvBuffer(tcpChannel_t* channel);

struct sockaddr_in tcpLocalInetAddress(int netfd);
struct sockaddr_in tcpPeerInetAddress(int netfd);
char* getIp(struct sockaddr_in addr);

int recvn(int sockFd, void *pStart, int length);
int recv_train(int fd, Train_t* train);

int tcpInit(int* psockFd, const char* ip, const int port);
void connectCallback(int fd);
void messageReadCallback(int fd);
void closeCb(int fd);

void serverLogin(tcpChannel_t* channel);
void serverLoginVerifyCrypt(tcpChannel_t* channel);
void serverRegister(tcpChannel_t* channel);


void commandCd(tcpChannel_t* channel);
void commandLs(tcpChannel_t* channel);
void commandMkdir(tcpChannel_t* channel);
void commandPwd(tcpChannel_t* channel);
void commandRm(tcpChannel_t* channel);
void commandGets(tcpChannel_t* channel);
void commandPuts(tcpChannel_t* channel);


#endif