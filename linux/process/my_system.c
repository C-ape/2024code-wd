#include "func.h"

//一般程序中不适用system函数去执行命令，太慢了，影响效率，可以自己写一个比较快的
int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        printf("before execl\n");
        //exec族，将一个可执行文件加载到本进程的地址空间
        //将原进程的数据清空，包括栈、堆、数据段，
        //将path中的代码加载进来，取代原进程的代码，重置pc指针从path中执行
        //execl("add", "./add", "2", "3", NULL); // 参数使用格式化的方式输入，最后要填NULL，表示到最后了
        char* exec_argvs[] = {"./add", "2", "3", NULL};
        execv("add", exec_argvs);
        printf("after execl\n"); //这句话不会执行
    }else
    {
        int p_status;
        while(1)
        {
            if (0 != waitpid(-1, &p_status, WNOHANG))
            {
                break;
            }
        }
    }
    return 0;
}
