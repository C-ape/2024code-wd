#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//n通常是des的长度
char* mystrncpy(char* des, const char* src, int n)
{
    int i;
    for (i = 0; i < n && src[i]!= '\0'; i++)
    {
        des[i] = src[i];
    }
    for (; i < n; i++)
    {
        des[i] = '\0';
    }
    return des;

}

int mystrcmp(char* src, char *des)
{
    int i = 0;
    while (src[i] == '\0' && des[i] == '\0')
    {
        if (src[i] == des[i])
        {
            continue;
        }else if(src[i] < des[i])
        {
            return -1;
        }else
        {
            return 1;
        }
    }
    return 0;
}


int STRCMP (const char *p1, const char *p2)  // (const char) *p1 表示: 读取p1指向的类型的值,接着转换为const char. 如果p1是double*,则将读取double所有字节的内容,接着转换为const char. [*p1也表示完整的将实参传了进来]
{
    const unsigned char *s1 = (const unsigned char *) p1;  // 初始化一个常量无符号字符指针s1, 为了字符方便比较, 全都转换为无符号常量char指针.
    const unsigned char *s2 = (const unsigned char *) p2;  // (const unsigned char *) p2 表示: 首先指针指向为p2, 然后读取p2中指向位置的第一个字节内容. 即使是double*也只读取第一个字节.
    unsigned char c1, c2;
    do
    {
        c1 = (unsigned char) *s1++;  // 指针后移一位sizeof(char),也就是1字节到后一个字符处.
        c2 = (unsigned char) *s2++;
        if (c1 == '\0')  // 结束循环: 字符指针移动到s1字符串的结尾处'\0', 如果此时的c2还有字符时,相减为负; 如果此时的c2恰好也是'\0',相减为0.
            return c1 - c2;
    }
    while (c1 == c2);  // 循环条件: 通过字符指针s1和s2的自增自减,移动到的字符都是一样的.
    return c1 - c2;  // 结束循环: 字符指针移动到s2字符串的结尾处'\0', 而此时的c1还有字符,相减为正.
}


int main()
{
    char buf[20] = {0};
    char* buf1 = "hello";
    mystrncpy(buf, buf1, sizeof(buf)/sizeof(char) - 1);
    buf[20] = '\0';     //手动加上结束符
    printf("buf is %s\n", buf);
    if (STRCMP(buf, buf1) == 0)
    {
        printf("buf is equal to buf1\n");
    }
    return 1;
}
