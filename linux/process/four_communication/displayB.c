#include "comm.h"

int msgid;
int *p;
int p_shmid;

void closeAll()
{
    msgctl(msgid,IPC_RMID,NULL);
    shmdt(p);
    shmctl(p_shmid, IPC_RMID, NULL);
    exit(0);
}

void sigFunc2(int num)
{
    closeAll();
}

void sigFunc(int num)
{
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR1);
    kill(pid3, SIGUSR1);
    kill(pid4, SIGUSR1);
}

int main()
{
    p_shmid = shmget(PID_SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(p_shmid, -1, "shmget");
    p = (int*)shmat(p_shmid, NULL, 0);
    p[3] = getpid();
    while(p[0] == 0 || p[1] == 0 || p[2] == 0 || p[3] == 0){}
    pid1 = p[0];
    pid2 = p[1];
    pid3 = p[2];
    pid4 = p[3];
    print();
    signal(SIGINT, sigFunc);
    signal(SIGUSR1, sigFunc2);
    struct mymsg rcv_msg;
    msgid = msgget(MSG_KEY, IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget");
    while(1)
    {
        memset(rcv_msg.mtext, 0, sizeof(rcv_msg.mtext));
        //msgtyp为0时， 无视类型取出第一个消息
        //消息队列发送和读取的长度不包括前面type的大小
        int rcv_ret = msgrcv(msgid, &rcv_msg, MSG_LENGTH, 0, 0);
        ERROR_CHECK(rcv_ret, -1, "msgrcv");
        if(rcv_ret == 0)
        {
            printf("msg queue is closed\n");
            break;
        }
        write(STDOUT_FILENO, rcv_msg.mtext, rcv_ret);
    }
    closeAll();
}
