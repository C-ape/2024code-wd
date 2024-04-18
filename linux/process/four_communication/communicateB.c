#include "comm.h"

int msgid;
int *p;
int p_shmid;
int pfdr;
int pfdw;

void closeAll()
{
    msgctl(msgid,IPC_RMID,NULL);
    shmdt(p);
    shmctl(p_shmid, IPC_RMID, NULL);
    close(pfdr);
    close(pfdw);
    exit(0);
}

void sigFunc2(int num)
{
    closeAll();
}
void sigFunc(int num)
{
    kill(pid1, SIGUSR1);
    kill(pid3, SIGUSR1);
    kill(pid4, SIGUSR1);
    kill(pid2, SIGUSR1);
}

int main()
{
    p_shmid = shmget(PID_SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(p_shmid, -1, "shmget");
    p = (int*)shmat(p_shmid, NULL, 0);
    p[1] = getpid();
    while(p[0] == 0 || p[1] == 0 || p[2] == 0 || p[3] == 0){}
    print();
    pid1 = p[0];
    pid2 = p[1];
    pid3 = p[2];
    pid4 = p[3];
    signal(SIGINT, sigFunc);
    signal(SIGUSR1, sigFunc2);
    pfdr = open("pipe1", O_RDONLY);
    ERROR_CHECK(pfdr, -1, "open pipe1");
    pfdw = open("pipe2", O_WRONLY);
    ERROR_CHECK(pfdw, -1, "open pipe2");
    char buf[MSG_LENGTH] = {0};
    //创建消息队列用于和B1通信
    msgid = msgget(1000, IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget 1000");
    struct mymsg send_msg;
    send_msg.mtype = 1;
    //监听读集合
    fd_set bfds; 
    int ret;
    while(1)
    {
        FD_ZERO(&bfds);
        FD_SET(STDIN_FILENO, &bfds);
        FD_SET(pfdr, &bfds);
        ret = select(pfdr + 1, &bfds, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        memset(buf, 0, sizeof(buf));
        if(FD_ISSET(STDIN_FILENO, &bfds))
        {
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read STDIN_FILENO");
            write(pfdw, buf, ret);
        }
        if(FD_ISSET(pfdr, &bfds))
        {
            ret = read(pfdr, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read pfdr");
            if(ret == 0)
            {
                printf("chata is closed\n");
                break;
            }
            memset(send_msg.mtext, 0, sizeof(send_msg.mtext));
            strncpy(send_msg.mtext, buf, ret); 
            int msgret = msgsnd(msgid, &send_msg, strlen(send_msg.mtext), 0);
            ERROR_CHECK(msgret, -1, "msgsnd");
        }
    }
    closeAll();
}