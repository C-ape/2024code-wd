#ifndef __LINE_H_
#define __LINE_H_

#include <iostream>

using std::cout;
using std::endl;

//pimpl模式，编译防火墙，实现封装和解耦
//class LinePimpl;    //前向声明或者内部类都可以
class Line
{
public:
    Line(int x1, int x2, int y1, int y2);
    ~Line();

    void printLine() const;
    class LinePimpl;    //内部类
private:
    LinePimpl * _linePimpl;
};

#endif