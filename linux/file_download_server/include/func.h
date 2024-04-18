#ifndef _FUNC_H
#define _FUNC_H

#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <grp.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#include <errno.h>
#include<mysql/mysql.h>


#define OPERATION_FAILED_STR "opreation failed!"    //出错返回的字符串

#define SUCCESSFUL_STR "opreation successful!"   
#define REGISTER_FAILED_STR "username already exist!"
#define REGISTER_FAILED_STR2 "register falied!"
#define MESSAGE_LOGIN_SALT_FAILED_STR "user is not exists!"
#define LOGIN_FAILED_STR  "passwd error!"

#define SERVER_FILE_PWD "/root/linux/file_download_server/all_file"     //服务器存放文件的路径

//支持的命令
#define LS "ls"
#define CD "cd"
#define PWD "pwd"
#define RM "rm"
#define PUTS "puts"
#define GETS "gets"
#define MKDIR "mkdir"

#define ID "id"

#define COMMAND_LENGTH 100
//server 的IP和端口
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234
#define PWD_LENGTH 100
#define NAME_LENGTH 50  //名字长度，和数据库保持一致
#define PASSWD_LENGTH 100
#define CRYPTPASSWD_LENGTH 100  //加密密码的长度
#define ORI_PWD "/"    //原始的用户路径
#define DIR_FILE_LENGTH 50 //一个目录下的文件数量

#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stderr, "argc is err\n"); \
            return -1;                        \
        }                                     \
    }

#define ERROR_CHECK(fp, num, msg) \
    {                             \
        if (fp == num)            \
        {                         \
            perror("msg");        \
            return -1;            \
        }                         \
    }

//返回值为0的判断，用于客户端断开recvn
#define RET_CHECK(fp, num, msg) \
    {                             \
        if (fp == num)            \
        {                         \
            printf("msg");        \
            return 0;            \
        }                         \
    }

#define ERROR_CHECK_RET_NULL(fp, num, msg) \
    {                             \
        if (fp == num)            \
        {                         \
            perror("msg");        \
            return;            \
        }                         \
    }

// 现成不能使用perror，因为errno是一个全局变量，线程之间共享全程变量，可以通过返回值来打印报错
//backslash-newline at end of file这个警告需要再末尾加一个空行
#define THREAD_ERROR_CHECK(ret, msg)                       \
    {                                                      \
        if (ret != 0)                                      \
        {                                                  \
            fprintf(stderr, "%s: %s", msg, strerror(ret)); \
        }                                                  \
    }

#define TCP_BUFFSIZE 1024

enum MessageType
{
    MESSAGE_NONE,      //
    MESSAGE_REGISTER_REQUEST = 1,   //从1开始，避免0误用
    OPERATION_FAILED,           //操作失败，用于出错处理
    MESSAGE_LOGIN,      //用户登录的请求
    MESSAGE_LONG_COMMAND,
    MESSAGE_SHORT_COMMAND,
    INVAILD_MESSAGE,        //无效的参数
    DOWNLOAD_FILE_DATA,     //下载文件的状态
    UPLOAD_FILE_DATA,       //上传文件的状态
    MESSAGE_REGISTER_SUCCESSFUL,  //注册成功
    MESSAGE_LOGIN_SUCCESSFUL,   //登录成功
    MESSAGE_LOGIN_FAILED, //登录失败
    MESSAGE_REGISTER_FAILED,
    MESSAGE_LOGIN_SALT,     //server返回用户的salt给client生成密文
    MESSAGE_LOGIN_SALT_FAILED,     //server返回salt失败，一般是没有对应的用户名
    MESSAGE_LOGIN_CRYPT,    //client 返回salt和密码生成的密文
    MESSAGE_CD_REQUEST,    
    MESSAGE_LS_REQUEST,
    MESSAGE_PWD_REQUEST,
    MESSAGE_PWD_SUCCESSFUL,
    MESSAGE_MKDIR_REQUEST,
    MESSAGE_MKDIR_SUCCESSFUL,
    MESSAGE_RM_REQUEST,     //短命令
    MESSAGE_PUTS_REQUEST,   //上传文件，长命令
    MESSAGE_GETS_REQUEST,   //下载文件，长命令
    MESSAGE_COMMAND_FAILED,     //命令执行失败
    MESSAGE_COMMAND_SUCCESSFUL, 
    MESSAGE_SUCCESSFUL = 200
};


typedef struct train_s
{
    int length;
    enum MessageType type;
    char buf[TCP_BUFFSIZE];
}Train_t;


//登录的用户名和密码
typedef struct loginMsg_s
{
    char name[NAME_LENGTH];
    char passwd[PASSWD_LENGTH];
    char token[PASSWD_LENGTH];
}loginMsg_t;

//ls查询时的文件结构体
typedef struct fileInfo_s
{
    char type;
    char filename[NAME_LENGTH];
}fileInfo_t;


#endif