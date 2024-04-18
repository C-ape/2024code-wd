#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * mergeString(char* str1, char* str2)
{
    int len1 = strlen(str1);
    printf("len1 = %d\n", len1);    //strlen计算不包括\0
    int len2 = strlen(str2);
    char *buf = (char*)malloc(sizeof(char)*(len1 + len2 + 1));
    memset(buf, 0, sizeof(char) * (len1 + len2 + 1));
    sprintf(buf, "%s %s", str1, str2);
    return buf;
}

int main()
{
    char * str1 = "hello";
    char * str2 = "world";
    char * buf = mergeString(str1, str2);
    printf("buf is %s\n", buf);
    return 0;
}
