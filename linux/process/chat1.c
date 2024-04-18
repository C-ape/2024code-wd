#include "func.h"

//使用有名管道通信
//mkfifo pipe1, mkfifo pipe2,pipe1和pipe2通信
int main(int argc, char* argv[])
{
    //./chat1 pipe1 pipe2
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    int fdr = open(argv[2], O_RDONLY);
    //单工通信，只能一边读，一边写
    // write(fdw, "hello", 5);
    // char buf[20] = {0};
    // read(fdr, buf, sizeof(buf));
    // puts(buf);

    //全双工通信
    char buf[4096] = {0};
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        write(fdw, buf, sizeof(buf));
        memset(buf, 0, sizeof(buf));
        read(fdr, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, sizeof(buf));
    }
    return 0;
}
