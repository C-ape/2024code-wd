#include "func.h"

pid_t pid1;
pid_t pid2;
pid_t pid3;
pid_t pid4;

void print()
{
    printf("pid[0] = %d\n", pid1);
    printf("pid[1] = %d\n", pid2);
    printf("pid[2] = %d\n", pid3);
    printf("pid[3] = %d\n", pid4);
}


//消息队列长度
#define MSG_LENGTH 4096
//共享内存大小
#define SHM_LENGTH 4096

struct mymsg
{
    long mtype;
    char mtext[MSG_LENGTH];
};
//消息队列key
#define MSG_KEY 1000
//共享内存key
#define SHM_KEY 1001
//信号量的key
#define SEM_KEY 1002
//进程id的共享内存key
#define PID_SHM_KEY 1005


