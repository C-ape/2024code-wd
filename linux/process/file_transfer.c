#include "func.h"

//文件传输，使用进程间通信方式将一个文件传输到另一个文件
//使用无名管道，父进程读文件，子进程写文件
int main(int argc, char* argv[])
{
    //./file_transfer file1 file2
    ARGS_CHECK(argc, 3);
    int pfd[2];
    int ret = pipe(pfd);
    ERROR_CHECK(ret, -1, "pipe");
    char buf[4096] = {0};
    if(fork() == 0)
    {
        //子进程读管道， 写文件，关闭写端
        close(pfd[1]);
        int wfd = open(argv[2], O_WRONLY|O_CREAT, 0666);
        ERROR_CHECK(wfd, -1, "open O_WRONLY|O_CREAT");
        while(1)
        {
            memset(buf, 0, sizeof(buf));
            int rpret = read(pfd[0], buf, sizeof(buf));
            //读文件和读管道的行为是不一样的
            //没有数据时，读文件返回0，读管道阻塞
            //当对端关闭管道时，发送一个EOF,本端接收到EOF才关闭
            if (rpret == 0)
            {
                close(pfd[0]);
                break;
            }
            write(wfd, buf, rpret);
        }
        close(wfd);
    }else
    {
        //父进程读文件，写入管道，关闭读端
        close(pfd[0]);
        int rfd = open(argv[1], O_RDONLY);
        ERROR_CHECK(rfd, -1, "open O_RDONLY");
        while(1)
        {
            memset(buf, 0, sizeof(buf));
            int rfret = read(rfd, buf, sizeof(buf));
            if(rfret == 0)
            {
                close(pfd[1]);
                break;
            }
            write(pfd[1], buf, rfret);
        }
        close(rfd);
        wait(NULL);
    }
}