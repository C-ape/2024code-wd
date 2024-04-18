#include "tcp.h"
#include "mysql.h"

int channelSize = 0; // 记录当前通道已经有多少连接

// 这里时数组的方式，如果支持更多连接的话要使用hash表(解决冲突)
tcpChannel_t channels[CHANNELS_SIZE] = {0};

// 循环读取数据，直到读完，配合小火车使用
int recvn(int sockFd, void *pStart, int length)
{
    int total = 0;
    int ret;
    char *p = (char *)pStart;
    while (total < length)
    {
        ret = recv(sockFd, p + total, length - total, 0);
        if (ret == 0)
        {
            printf("connect is closed\n");
            return 0;
        }else if (ret == -1 && errno != EINTR)
        {
            perror("recv");
            return -1;
        }
        total += ret;
    }
    return length;
}


//接收一整个train,返回数据区的长度
int recv_train(int fd, Train_t* train)
{
    //先读取数据长度
    int ret = recvn(fd, &train->length, sizeof(int));
    if(ret == -1 || ret == 0)
    {
        return ret;
    }
    //读取发送的消息类型
    ret = recvn(fd, &train->type, sizeof(int));
    if (ret == 0 || ret == -1)
    {
        return ret;
    }
    ret = recvn(fd, train->buf, train->length);
    if (ret == 0 || ret == -1)
    {
        return ret;
    }
    return train->length;
}

//接收文件
int recvFile(int netFd)
{
    Train_t t;
    bzero(&t, sizeof(t));
    // 先接收文件名长度
    recvn(netFd, &t.length, sizeof(int));
    //接收状态码, 状态码可以不校验，反正后面要检查md5sum
    recvn(netFd, &t.type, sizeof(int));
    // 再接收文件名
    recvn(netFd, t.buf, t.length);
    // 接收方创建一个同名文件
    int fd = open(t.buf, O_WRONLY | O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    while (1)
    {
        bzero(&t, sizeof(t));
        recvn(netFd, &t.length, sizeof(int));
        if (0 == t.length)
        {
            break;
        }
        recvn(netFd, t.buf, t.length);
        write(fd, t.buf, t.length);
    }
    close(fd);
}

int tcpInit(int *psockFd, const char *ip, const int port)
{
    *psockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*psockFd, -1, "socket");
    int optval = 1;
    int ret = setsockopt(*psockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(struct sockaddr_in));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(ip);
    serAddr.sin_port = htons(port);
    ret = bind(*psockFd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");
    listen(*psockFd, 10);
}

// 通道管理操作
void initChannel()
{
    for (int i = 0; i < CHANNELS_SIZE; i++)
    {
        channels[i].netfd = -1;
        channels[i].fileId = -1;    //目录文件id置为-1
        channels[i].recvTrain.length = 0;
        channels[i].sendTrain.length = 0;
        memset(channels[i].username, 0, NAME_LENGTH);
        memset(channels[i].recvTrain.buf, 0, TCP_BUFFSIZE);
        memset(channels[i].sendTrain.buf, 0, TCP_BUFFSIZE);
    }
}

int currentChannelIndex(int netfd)
{
    for (int idx = 0; idx < CHANNELS_SIZE; idx++)
    {
        if (channels[idx].netfd == netfd)
        {
            return idx;
        }
    }
    return -1; // 不在通道中
}

// 这里netfd不会重复，可以直接当数组的下标，但是为了能用满这个数组，还是遍历插入吧
int insertChannel(int netfd)
{
    for (int idx = 0; idx < CHANNELS_SIZE; idx++)
    {
        if (channels[idx].netfd == -1)
        {
            channels[idx].netfd = netfd;
            ++channelSize;
            return idx;
        }
    }
    printf("channel is full\n");
    return -1;
}

void deleteChannel(int netfd)
{
    int idx = currentChannelIndex(netfd);
    if (-1 != idx)
    {
        clearRecvBufferByIdx(idx);
        clearSendBufferByIdx(idx);
        channels[idx].netfd = -1;
        --channelSize;
    }
}

// 对某一个通道的操作
void clearSendBufferByIdx(int idx)
{
    memset(channels[idx].sendTrain.buf, 0, TCP_BUFFSIZE);
    channels[idx].sendTrain.length = 0;
    channels[idx].sendTrain.type = MESSAGE_NONE;
}

void clearRecvBufferByIdx(int idx)
{
    memset(channels[idx].recvTrain.buf, 0, TCP_BUFFSIZE);
    channels[idx].recvTrain.length = 0;
    channels[idx].recvTrain.type = MESSAGE_NONE;
}

void clearSendBuffer(tcpChannel_t* channel)
{
    memset(channel->sendTrain.buf, 0, TCP_BUFFSIZE);
    channel->sendTrain.length = 0;
    channels->sendTrain.type = MESSAGE_NONE;
}

void clearRecvBuffer(tcpChannel_t* channel)
{
    memset(channel->recvTrain.buf, 0, TCP_BUFFSIZE);
    channel->recvTrain.length = 0;
    channels->recvTrain.type = MESSAGE_NONE;
}

// 获取本端ip结构
struct sockaddr_in tcpLocalInetAddress(int netfd)
{
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getsockname(netfd, (struct sockaddr *)&localAddr, &len);
    if (ret < 0)
    {
        perror("getsockname");
    }
    return localAddr;
};

struct sockaddr_in tcpPeerInetAddress(int netfd)
{
    struct sockaddr_in peerAddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(netfd, (struct sockaddr *)&peerAddr, &len);
    if (ret < 0)
    {
        perror("getsockname");
    }
    return peerAddr;
};

char *getIp(struct sockaddr_in addr)
{
    return inet_ntoa(addr.sin_addr);
}

unsigned short getPort(struct sockaddr_in addr)
{
    return ntohs(addr.sin_port);
}

// 连接建立时的回调函数
void connectCallback(int netfd)
{
    char buf[100] = {0};
    sprintf(buf, "tcp conn [%s:%d --> %s:%d] connect successful!",
            getIp(tcpLocalInetAddress(netfd)),
            getPort(tcpLocalInetAddress(netfd)),
            getIp(tcpPeerInetAddress(netfd)),
            getPort(tcpPeerInetAddress(netfd)));
    puts(buf);
}

void serverLogin(tcpChannel_t* channel)
{
    printf("server login\n");
    loginMsg_t login;
    memset(&login, 0, sizeof(login));
    memcpy(&login, channel->recvTrain.buf, channel->recvTrain.length);
    char salt[20] = {0};
    int ret = findSaltByName(login.name, salt);
    if (ret == 0)
    {
        //找到salt，将salt发送给client
        printf("find salt\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_LOGIN_SALT;
        strcpy(channel->sendTrain.buf, salt);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else if(ret == 1)
    {
        //没有找到用户，用户不存在数据库中
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_LOGIN_SALT_FAILED;
        strcpy(channel->sendTrain.buf, MESSAGE_LOGIN_SALT_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else
    {
        //查询出错
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_LOGIN_SALT_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }

}


void serverLoginVerifyCrypt(tcpChannel_t* channel)
{
    printf("server login verify crypt\n");
    loginMsg_t login;
    memset(&login, 0, sizeof(login));
    memcpy(&login, channel->recvTrain.buf, channel->recvTrain.length);
    //先检查名字是否存在数据库中
    char crypt[100] = {0};
    int ret = findCryptByName(login.name, login.token);
    if (ret == 0)
    {
        //用户名密码验证成功
        printf("crypt verify successful\n");
        clearRecvBuffer(channel);
        //登录成功也将fileId设置为根，记着退出的时候将数据库中目录置为根目录
        findRootIdByNameFromVir(login.name, &channel->fileId, ID, 0);
        channel->sendTrain.type = MESSAGE_LOGIN_SUCCESSFUL;
        strncpy(channel->username, login.name, NAME_LENGTH);
        char pwd[100] = {0};
        findStrArgByName(login.name, pwd, "pwd");
        strcpy(channel->sendTrain.buf, pwd);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else if(ret == 1)
    {
        //密码错误
        printf("passwd error\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_LOGIN_FAILED;
        strcpy(channel->sendTrain.buf, LOGIN_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else
    {
        printf("crypt verify failed\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_LOGIN_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }
}

void serverRegister(tcpChannel_t* channel)
{
    printf("server register\n");
    loginMsg_t login_t;
    memset(&login_t, 0, sizeof(login_t));
    memcpy(&login_t, channel->recvTrain.buf, channel->recvTrain.length);
    char salt[15] = {0};
    get_salt(salt);
    //先检查名字是否存在数据库中
    int ret = findName(login_t.name);
    if (ret == 0)
    {
        //用户名没在数据库中可以添加
        addUser(login_t);
        //虚拟目录表中也要添加一条目录信息
        addRootDir(login_t);
        clearRecvBuffer(channel);
        //注册成功文件id置为根目录
        findRootIdByNameFromVir(login_t.name, &channel->fileId, ID, 0);
        channel->sendTrain.type = MESSAGE_REGISTER_SUCCESSFUL;
        strncpy(channel->username, login_t.name, NAME_LENGTH);
        strcpy(channel->sendTrain.buf, ORI_PWD);  //所有用户注册都返回初始目录
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else if(ret == 1)
    {
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_REGISTER_FAILED;
        strcpy(channel->sendTrain.buf, REGISTER_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else
    {
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_REGISTER_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }

}


void messageReadCallback(int fd)
{
    int idx = currentChannelIndex(fd);
    ERROR_CHECK_RET_NULL(idx, -1, "currentChannelIndex netfd is not in channles");
    tcpChannel_t* channel = &channels[idx];
    switch (channel->recvTrain.type)
    {
    case MESSAGE_LOGIN:
        serverLogin(channel);
        break;
    case MESSAGE_LOGIN_CRYPT:
        serverLoginVerifyCrypt(channel);
        break;    
    case MESSAGE_REGISTER_REQUEST:
        serverRegister(channel);
        break;
    //命令执行函数
    case MESSAGE_CD_REQUEST:
        commandCd(channel);
        break;
    case MESSAGE_LS_REQUEST:
        commandLs(channel);
        break;
    case MESSAGE_MKDIR_REQUEST:
        commandMkdir(channel);
        break;
    case MESSAGE_PWD_REQUEST:
        commandPwd(channel);
        break;
    case MESSAGE_RM_REQUEST:
        commandRm(channel);
        break;
    case MESSAGE_GETS_REQUEST:
        commandGets(channel);
        break;
    case MESSAGE_PUTS_REQUEST:
        commandPuts(channel);
        break;

    default:
        break;
    }

}

void commandCd(tcpChannel_t* channel)
{

}

int lsQuery(char* name, char* result)
{

}

void commandLs(tcpChannel_t* channel)
{
    char res[1024] = {0};
    int ret = lsQuery(channel->username, res);

    if (ret != 0)
    {
        printf("operate pwd failed\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_COMMAND_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
        return;
    }

}

void commandMkdir(tcpChannel_t* channel)
{
    char dirName[NAME_LENGTH] = {0};
    strncpy(dirName, channel->recvTrain.buf, NAME_LENGTH);
    printf("mkdir dirname is %s", dirName);
    int pre_id = channel->fileId;
    int owner_id = 0;
    findIdArgByName(channel->username, &owner_id, ID);
    int ret = insertDirVir(dirName, pre_id, owner_id);

    if (ret == 0)
    {
        printf("mkdir exec successful\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_MKDIR_SUCCESSFUL;
    }else
    {
        printf("operate failed\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_COMMAND_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }
}


void commandPwd(tcpChannel_t* channel)
{   
    char pwd[PWD_LENGTH] = {0};
    int ret = findStrArgByName(channel->username, pwd, PWD);
    if (ret == 0)
    {
        printf("pwd exec successful\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_PWD_SUCCESSFUL;
        strcpy(channel->sendTrain.buf, pwd);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }else
    {
        printf("operate failed\n");
        clearRecvBuffer(channel);
        channel->sendTrain.type = MESSAGE_COMMAND_FAILED;
        strcpy(channel->sendTrain.buf, OPERATION_FAILED_STR);
        channel->sendTrain.length = strlen(channel->sendTrain.buf);
    }
}

//删除目录的时候目录下的文件也要删除
void commandRm(tcpChannel_t* channel)
{

}
void commandGets(tcpChannel_t* channel)
{

}

void commandPuts(tcpChannel_t* channel)
{


}

void closeCb(int netfd)
{
    close(netfd);
}