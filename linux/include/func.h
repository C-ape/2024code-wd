#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <time.h>
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

// 现成不能使用perror，因为errno是一个全局变量，线程之间共享全程变量，可以通过返回值来打印报错
//backslash-newline at end of file这个警告需要再末尾加一个空行
#define THREAD_ERROR_CHECK(ret, msg)                       \
    {                                                      \
        if (ret != 0)                                      \
        {                                                  \
            fprintf(stderr, "%s: %s", msg, strerror(ret)); \
        }                                                  \
    }
