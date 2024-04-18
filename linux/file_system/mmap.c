#include "func.h"

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR|O_CREAT, 0777); // open的文件必须有大小，不然映射不了，会core
    ERROR_CHECK(fd, -1, "open");
    int ret = ftruncate(fd, 20);
    //NULL, mmap自动分配空间
    //length，内存映射区的大小，必须大于0
    //可读可写PORT_READ|PROT_WRITE
    //映射区类型
    //文件描述符
    //偏移位置
    char* file_buf = (char*)mmap(NULL, 20, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(file_buf, NULL, "mmap");
    strcpy(file_buf, "asdfsdfsdffasfsdfsadf");
    munmap(file_buf, 20);
}
