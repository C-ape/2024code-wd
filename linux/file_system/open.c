#include "func.h"


int main(int argc, char* argv[])
{
    //./open file
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR|O_CREAT, 0777);  //文件存在也重新创建
    ERROR_CHECK(fd, -1, "open");
    int i = 10000, j;
    char buf[20] = {0};
    //write(fd, "hahahahhah\n", 10);
    write(fd, &i, 10); // 二进制写入，需要使用二进制读出来
    int loc = lseek(fd, 0, SEEK_SET);//写完已经到文件末尾了，偏移到开始才能读
    int ret = read(fd, &j, 4);
    printf("ret is %d, j is %d\n", ret, j);
    //puts(buf);
    close(fd);
}