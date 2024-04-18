#include "func.h"

int main()
{
    if (fork() == 0)
    {
        printf("i am child, pid is %d, ppid id %d\n", getpid(), getppid());
        sleep(2);
    }else
    {
        int p_status;
        sleep(1);
        printf("i am father, pid is %d, ppid is %d\n", getpid(), getppid());
        while(1)
        {
            //-1表示回收所有进程，WNOHANG表示及时响应，可以使父进程非阻塞, status保持子进程退出的状态信息
            int ret = waitpid(-1, &p_status, WNOHANG);
            if (ret == 0)
            {
                printf("child is not dead yet\n");
            }else
            {
                if (WIFEXITED(p_status))
                {
                    printf("Normal exit, return value is %d\n", WEXITSTATUS(p_status));
                }else if (WIFSIGNALED(p_status))
                {
                    printf("signal interrupt, Signal is %d\n", WTERMSIG(p_status));
                }
                break;
            }
        }
        //wait(NULL); //阻塞，会一直等待子进程执行完
    }
    return 0;
}