#include <stdio.h>
#include <stdlib.h>


int main()
{
    int i = 8;       //两个字节，低字节是34，高字节是12
    char* p = (char*)&i;    //转换为char*,这样就可以一个字节一个字节的加了
    printf("p的地址是 %p\n", p);  
    printf("p的第一个字节是 %x\n", *p);
    printf("p的第一个字节是 %d\n", *p);
    printf("p的第二个字节是 %x\n", *(p+1));
    // printf("p的第三个字节是 %x\n", *(p+2));
    // printf("p的第四个字节是 %x\n", *(p+3));

    char * s = "hello";
    printf("sizeof(s) is %ld\n", sizeof(s));

    return 0;
}

