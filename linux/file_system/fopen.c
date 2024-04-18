#include"func.h"

int main(int argc, char* argv[])
{
    //if (argc != 2)
    //{
    //    printf("argc is error\n");
    //    return -1;
    //}
    ARGS_CHECK(argc, 2);
    //FILE* fp = fopen(argv[1], "a"); //a模式只写追加，默认从文件尾部写入
    FILE* fp = fopen(argv[1], "a+"); //a+模式读写追加，打开时处于文件的开始，写入时跳到文件的末尾
    //if(fp == NULL)
    //{
    //    perror("fopen");
    //    return -1;
    //}
    ERROR_CHECK(fp, NULL, "fopen");
    char buf[] = {"1234234234\n"};
    fwrite(buf, 1, sizeof(buf)/sizeof(char), fp);
    memset(buf, 0, sizeof(buf)/sizeof(char));
    printf("after write ,loc is %ld\n", ftell(fp));
    fseek(fp, 0, SEEK_SET);
    fread(buf, 1, sizeof(buf)/sizeof(char), fp);
    printf("after fread ,loc is %ld\n", ftell(fp));
    puts(buf);
    fclose(fp);
    return 0;

}