#include "func.h"

//ipcs查看已分配的共享内存, ipcrm -M key:删除
int main()
{
    //key_t key = ftok("file", 1); //通过文件生成一个shmget需要的key，可以不用生成，直接使用一个int型整数
    //ERROR_CHECK(key, -1, "ftok");
    key_t key = 1000;
    //key为1000 大小为4096 创建一个0600的共享内存,;//如果key为1000的共享内存已经存在，则找到它的描述符
    int shmid = shmget(key, 4096, 0600|IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    printf("shmid = %d\n", shmid);
    //共享的物理内存分配好过后，需要将虚拟内存映射过去
    //shmaddr用NULL，表示在堆空间上自动分配映射共享内存
    //shmflg表示权限，一般使用0
    char* buf = (char*)shmat(shmid, NULL, 0);
    ERROR_CHECK(buf, (void*)-1, "shmat");
    strcpy(buf, "hello");
    puts(buf);
    int ret = shmdt(buf); //解除堆空间到共享内存的映射，共享内存的内容不会消失
    ERROR_CHECK(ret, -1, "shmdt");
    return 0;
}