/*
实现把一个short类型的值写入buffer中和从buffer中读出一个short类型的值
要求在不同的操作系统间写入和读出的值相同，请用两个格式实现:二进制和文本
*/

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>

using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::stringstream;

//使用二进制的方式存储，内存中存储的方式直接就是二进制的方式，直接memcpy就行了
int write_short_data_binary(short value, char* buf)
{
    memcpy(buf, &value, sizeof(value));     //写入buf
    printf("%x\n", value);
    for (int i = 0; i < sizeof(value); i++)
    {
        cout << (int)buf[i] << " ";         //这里是以十进制的方式打印每个字节
    }
    cout << endl;

    //从buf读出
    short i;
    memcpy(&i, buf, sizeof(short));
    cout << "从buf中读出: " << i << endl;
    return 0;
}


//以文本方式存储
int write_short_value_by_text(short value, char* buf)
{
    //value转换为字符串存储到buf(字符模式就是文本模式)
    ostringstream osin;
    osin << value;
    //cout << osin.str() << endl;
    strcpy(buf, osin.str().c_str());
    cout << buf << endl;

    //将文本模式读取到short
    short i;
    // istringstream ss(buf);
    // ss >> i;
    stringstream ss;
    ss << buf;
    ss >> i;
    cout << " i = " << i << endl;

    return 0;
}

//以文本方式存储2
int write_short_value_by_text2(short value, char* buf)
{
    sprintf(buf, "%d", value);

    printf("buf is %s\n", buf);

    short i = atoi(buf);
    cout << "i = " << i << endl;
    return 0;
}

int main()
{
    short value = 12345;
    char buf[10] = {0};
    //write_short_data_binary(value, buf);
    //write_short_value_by_text(value, buf);
    write_short_value_by_text2(value, buf);

}


