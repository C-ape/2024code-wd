#include "comm.h"

char* shm_buf;
int shmid;
int* p;
int p_shmid;
int semid;

void closeAll()
{
    //解除虚拟内存到共享内存的映射
    shmdt(shm_buf);
    //删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(p);
    shmctl(p_shmid, IPC_RMID, NULL);
    //删除信号量
    semctl(semid, 0, IPC_RMID);
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
    kill(pid4, SIGUSR1);
    kill(pid3, SIGUSR1);
}
int main()
{
    p_shmid = shmget(PID_SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(p_shmid, -1, "shmget");
    p = (int*)shmat(p_shmid, NULL, 0);
    p[2] = getpid();
    while(p[0] == 0 || p[1] == 0 || p[2] == 0 || p[3] == 0){}
    pid1 = p[0];
    pid2 = p[1];
    pid3 = p[2];
    pid4 = p[3];
    print();
    signal(SIGINT, sigFunc);
    signal(SIGUSR1, sigFunc2);
    shmid = shmget(SHM_KEY, SHM_LENGTH, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
    shm_buf;
    //创建信号量，pv操作保护共享内存
    semid = semget(SEM_KEY, 1, IPC_CREAT|0600);
    //这里将信号量初值设为0
    //因为这个显示进程要求只有数据过来的时候才能获得资源，其他时间阻塞
    int semret = semctl(semid, 0, SETVAL, 0);
    struct sembuf P;
    P.sem_num = 0; //信号量在集合中的下标
    P.sem_flg = SEM_UNDO; //如果p操作被中断，减掉的信号量会自动加回来
    P.sem_op = -1; //每次p操作执行减一的操作
    //struct sembuf V;
    //V.sem_num = 0; //信号量在集合中的下标
    //V.sem_flg = SEM_UNDO; //如果p操作被中断，减掉的信号量会自动加回来
    //V.sem_op = 1; //每次p操作执行减一的操作
    while(1)
    {
        //操作共享内存先减少资源，资源只能在主进程释放
        semop(semid, &P, 1);
        shm_buf = (char*)shmat(shmid, NULL, 0);
        ERROR_CHECK(shm_buf, (void*)-1, "shmat");
        write(STDOUT_FILENO, shm_buf, strlen(shm_buf));
        memset(shm_buf, 0, sizeof(shm_buf));
        //semop(semid, &V, 1);
    }
    closeAll();
}