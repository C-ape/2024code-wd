#include "comm.h"

//设置为全局变量方便退出资源清理
int *p; //进程共享内存
int p_shmid; //
int pfdw;
int pfdr;
int semid; //信号量
int shmid; //通信共享内存id
char* shm_buf; //通信共享内存

void closeAll()
{
    //解除映射
    shmdt(shm_buf);
    //删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(p);
    shmctl(p_shmid, IPC_RMID, NULL);
    //删除信号量
    semctl(semid, 0, IPC_RMID);
    close(pfdr);
    close(pfdw);
    exit(0);
}

void sigFunc2(int num)
{
    closeAll();
}

//先给别人发信号，自己再给自己发,不然自己先退出了没有给别人发信号
void sigFunc(int num)
{
    kill(pid2, SIGUSR1);
    kill(pid3, SIGUSR1);
    kill(pid4, SIGUSR1);
    kill(pid1, SIGUSR1);
}

int main()
{
    //所有进程都启动过后才开始后面的行为
    p_shmid = shmget(PID_SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(p_shmid, -1, "shmget");
    p = (int*)shmat(p_shmid, NULL, 0);
    p[0] = getpid();
    while(p[0] == 0 || p[1] == 0 || p[2] == 0 || p[3] == 0){}
    pid1 = p[0];
    pid2 = p[1];
    pid3 = p[2];
    pid4 = p[3];
    print();
    //信号处理函数，一个端口关闭后，其他的全部关闭
    signal(SIGINT, sigFunc);
    signal(SIGUSR1, sigFunc2);
    pfdw = open("pipe1", O_WRONLY);
    ERROR_CHECK(pfdw, -1, "open pipe1");
    pfdr = open("pipe2", O_RDONLY);
    ERROR_CHECK(pfdr, -1, "open pipe2");
    char buf[4096] = {0};
    //创建信号量，pv操作保护共享内存
    semid = semget(SEM_KEY, 1, IPC_CREAT|0600);
    //设置信号量初值为0,共享内存对这个进程随时开放，只有更新共享内存的时候显示进程才能获得资源
    int semret = semctl(semid, 0, SETVAL, 0);
    // struct sembuf P;
    // P.sem_num = 0; //信号量在集合中的下标
    // P.sem_flg = SEM_UNDO; //如果p操作被中断，减掉的信号量会自动加回来
    // P.sem_op = -1; //每次p操作执行减一的操作
    struct sembuf V;
    V.sem_num = 0; //信号量在集合中的下标
    V.sem_flg = SEM_UNDO; //如果p操作被中断，减掉的信号量会自动加回来
    V.sem_op = 1; //每次p操作执行减一的操作
    ERROR_CHECK(semret, -1, "semctl");
    //创建一个共享内存
    shmid = shmget(SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
    //NULL表示在堆空间上自动分配地址，flag一般都是多余的，填0
    shm_buf = (char*)shmat(shmid, NULL, 0);
    //监听集合
    fd_set afds;
    int ret;
    while(1)
    {
        FD_ZERO(&afds);
        FD_SET(STDIN_FILENO, &afds);
        FD_SET(pfdr, &afds);
        ret = select(pfdr + 1, &afds, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        memset(buf, 0, sizeof(buf));
        if(FD_ISSET(STDIN_FILENO, &afds))
        {
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read STDIN_FILENO");
            write(pfdw, buf, ret);
        }
        if(FD_ISSET(pfdr, &afds))
        {
            ret = read(pfdr, buf, sizeof(buf));
            if(ret == 0)
            {
                printf("chatb is closed\n");
                break;
            }
            ERROR_CHECK(ret, -1, "read pfdr");
            //semop(semid, &P, 1);
            memcpy(shm_buf, buf, SHM_LENGTH);
            // 写入共享内存过后才释放资源
            semop(semid, &V, 1);
        }
    }
    closeAll();
}