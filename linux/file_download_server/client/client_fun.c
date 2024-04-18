#include "func.h"
#include "client_func.h"


void print()
{
//    system("clear");
//    printf("Hello %s\n",login_msg.name);
    printf("[%s@Netdisk:%s]# ",loginMsg.name, clientPwd);
    fflush(stdout);
}

int tcpInit(int* psockFd, char* ip, int port)
{
    *psockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*psockFd, -1, "socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    // addr.sin_port = htons(atoi(argv[2]));
    // addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int ret = connect(*psockFd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
}

int regLoginFunc(Train_t* train, int type)
{
    switch(type)
    {
        case 1:  return regsiterFunc(train);
        case 2:  return loginFunc(train);
        default: return -1;
    }
    return 0;
}

int loginFunc(Train_t* train)
{
    puts("login func");

    printf("请输入用户名：\n");
    scanf("%s", loginMsg.name);
    strncpy(loginMsg.passwd, getpass("请输入密码："), PASSWD_LENGTH);

    //组装train发送给服务端认证
    //客户端只用发送name，服务器会返回salt
    train->type = MESSAGE_LOGIN;
    strncpy(train->buf, loginMsg.name, PASSWD_LENGTH);
    train->length = strlen(train->buf);
    return 0;

}

int regsiterFunc(Train_t* train)
{
    puts("regsiterfunc");
    char *passwd1;
    int cnt = RETRY_CNT;
    printf("请输入用户名：\n");
    scanf("%s", loginMsg.name);
    while(cnt)
    {
        memset(loginMsg.passwd, 0, PASSWD_LENGTH);
        strncpy(loginMsg.passwd, getpass("请输入密码："), PASSWD_LENGTH);
        passwd1 = getpass("请再次输入密码:");
        int ret = strcmp(loginMsg.passwd, passwd1);
        if (ret != 0)
        {
            printf("两次输入的密码不一样\n");
            cnt--;
            continue;
        }else
        {
            break;
        }
    }
    if (cnt == 0)
    {
        printf("次数已用完, 返回主页面\n");
        sleep(1);
        return -1;
    }

    //组装train发送给服务端注册
    train->type = MESSAGE_REGISTER_REQUEST;
    train->length = sizeof(loginMsg);
    memcpy(train->buf, &loginMsg, sizeof(loginMsg));
    return 0;
}




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

void closeCb(int netfd)
{
    close(netfd);
}

//1、fd的内核缓冲区中有空间(缓冲区没满)，就会触发写事件
//2、从不可写状态变为可写状态。
//这里发送的是结构体，注意结构体对齐的问题，这里length和type都是int，没有问题
void epollHandleWriteEvent(int epfd, int netFd, Train_t* train)
{

    int sendLength = train->length + 8; //8是length和type的长度
    int nsend = send(netFd, train, sendLength, 0);
    if (nsend = sendLength)
    {
        //如果缓冲区的数据发送完，不要再监听写事件，发送完了一定要清除，这里是水平触发，不清除读事件会一直触发
        epollClearWriteEvent(epfd, netFd);
        clearTrainBuffer(train);
    }else if (nsend > 0 && nsend < sendLength)
    {
        //如果只发送了部分数据，将未发送的数据移动到开始为止，下一个epoll_wait再发
        train->length = sendLength - nsend;
        memmove(train, train + nsend, sendLength - nsend);
    }
    //printf("send data over\n");
}



void clearTrainBuffer(Train_t* train)
{
    memset(train->buf, 0, TCP_BUFFSIZE);
    train->type = MESSAGE_NONE;
    train->length = 0;
}


void successfulFunc(Train_t* train)
{
    strncpy(clientPwd, train->buf, PWD_LENGTH);
    print();
    clearTrainBuffer(train);
}

void returnBufFunc(Train_t* train)
{
    puts(train->buf);
    print();
    clearTrainBuffer(train);
}

void failedFunc(Train_t* train)
{
    puts(train->buf);
    clearTrainBuffer(train);
}

void loginSalt(Train_t* train)
{
    //获取salt，生成cpypt，返回给server认证
    printf("login salt, salt is %s, name is %s, passwd is %s\n", loginMsg.name, train->buf, loginMsg.passwd);
    char cryptStr[CRYPTPASSWD_LENGTH] = {0};
    strncpy(loginMsg.token, crypt(loginMsg.passwd, train->buf), CRYPTPASSWD_LENGTH);
    printf("crypt is %s\n", loginMsg.token);
    clearTrainBuffer(train);
    train->type = MESSAGE_LOGIN_CRYPT;
    memcpy(train->buf, &loginMsg, sizeof(loginMsg));
    train->length = sizeof(loginMsg);
    printf("length = %d\n", train->length);
}

int netMessageDeal(Train_t* train)
{
    int type  = train->type;
    int ret = 0;
    switch(type)
    {
        case MESSAGE_REGISTER_SUCCESSFUL:
        case MESSAGE_LOGIN_SUCCESSFUL:
            successfulFunc(train);
            break;
        case MESSAGE_PWD_SUCCESSFUL:
            returnBufFunc(train);
        case MESSAGE_LOGIN_SALT:
            loginSalt(train);
            break;
        case MESSAGE_COMMAND_FAILED:
            returnBufFunc(train);
            break;
        case OPERATION_FAILED:
        case MESSAGE_LOGIN_FAILED:
        case MESSAGE_REGISTER_FAILED:
        case MESSAGE_LOGIN_SALT_FAILED:
            //走重新登录流程
            failedFunc(train);
            return -1;
        default:
            return -1;
    }
    return 0;
}


//调用这个函数命令是必须需要文件的,比如rm, cd, gets, puts等 
int getCommandFileName(char* buf, char* filename)
{
    int i = 0, j = 0;
    while(buf[i] != '\n' && buf[i] != 0 && buf[i] != ' '){ i++;}
    while(buf[i] == ' ' && i < COMMAND_LENGTH) { i++; }   //去掉空格,随变一个数，避免死循环在这里
    if (buf[i] == '\n' || i == COMMAND_LENGTH - 1)
    {
        printf("请输入需要操作的文件名\n");
        return -1;
    }
    i++;
    while (buf[i] != 0 && buf[i] != '\n')
    {
        filename[j] = buf[i];
        i++;
        j++;
    }
    printf("filename is %s\n", filename);
    return 0;
}


//这个函数应该将需要文件的和不需要文件的分开写，这样好判断一些，不想改了，能用
int parseCommand(char* buf, char* filename)
{
    //这里的buf还有换行符
    char command[20] = {0};
    int ret = 0;
    int i = 0;
    int type = 0;

    while(buf[i] != '\n' && buf[i] != 0 && buf[i] != ' ')
    {
        command[i] = buf[i];
        i++;
    }
    printf("command is %s\n", command);
    if (strcmp(command, LS) == 0)
    {
        type = MESSAGE_LS_REQUEST;
    }else if (strcmp(command, CD) == 0)
    {
        ret = getCommandFileName(buf, filename);
        type = MESSAGE_CD_REQUEST;
    }else if (strcmp(command, PWD) == 0)
    {
        type = MESSAGE_PWD_REQUEST;
    }else if (strcmp(command, PUTS) == 0)
    {
        ret = getCommandFileName(buf, filename);
        type = MESSAGE_PUTS_REQUEST;
    }else if (strcmp(command, GETS) == 0)
    {
        ret = getCommandFileName(buf, filename);
        type = MESSAGE_GETS_REQUEST;
    }else if (strcmp(command, RM) == 0)
    {
        ret = getCommandFileName(buf, filename);
        type = MESSAGE_RM_REQUEST;
    }else if (strcmp(command, MKDIR) == 0)
    {
        ret = getCommandFileName(buf, filename);
        type = MESSAGE_MKDIR_REQUEST;
    }else 
    {
        return -1;
    }
    if (ret == -1)
    {
        return -1;
    }else
    {
        return type;
    }

}

int stdinMessageDeal(char* buf, Train_t* clientTrain)
{
    if (buf[0] == '\n')
    {
        return 0;
    }
    char filename[40] = {0};
    memset(clientTrain, 0, sizeof(Train_t));
    int ret = parseCommand(buf, filename);
    if (ret == -1)
    {
        printf("请输入正确的命令\n");
        return -1;
    }
    clientTrain->type = ret;
    if (strlen(filename) != 0)
    {
        strncpy(clientTrain->buf, filename, TCP_BUFFSIZE);
    }else
    {
        //如果没有文件或者目录，就把当前目录发送过去， 因为外面设置读事件的条件是len>0,小坑
        //针对ls，pwd命令
        strncpy(clientTrain->buf, clientPwd, TCP_BUFFSIZE);
    }
    clientTrain->length = strlen(clientTrain->buf);
    printf("stdin message buf is %s, length is %d, type is %d\n", clientTrain->buf, clientTrain->length, clientTrain->type);
}



