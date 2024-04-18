#include "func.h"

int main(int argc, char* argv[])
{
    //./chat2 pipe1 pipe2
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    int fdw = open(argv[2], O_WRONLY);
    char buf[4096] = {0};
    // read(fdr, buf, sizeof(buf));
    // puts(buf);
    // write(fdw, "hello", 5);
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        read(fdr, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, sizeof(buf));
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        write(fdw, buf, sizeof(buf));
    }
    return 0;
}
