#include "func.h"

//popen创建一个子进程，执行一个shell命令
//popen库函数首先会使用fork创建一个子进程，并且在父子进程中创建一条管道用于通信,然后调用exec执行命令
//“w”模式，父进程可以写入管道，子进程把自己的stdin重定向为管道
//“r”模式，子进程把自己的stdout重定向为管道，父进程可以读出来

int main(int argc, char* argv[])
{
    //./popen ./add1
    ARGS_CHECK(argc, 2);
    FILE* fp = popen(argv[1], "w");
    ERROR_CHECK(fp, NULL, "popen");
    fwrite("1234 100", 1, 8, fp);
    pclose(fp);
    return 0;
}
