#include <iostream>
#include "Line.h"



using std::cout;
using std::endl;



class Line::LinePimpl
//class LinePimpl
{
public:
    LinePimpl(int x1, int y1, int x2, int y2)
    :_p1(x1, y1)
    ,_p2(x2, y2)
    {
        cout << "LinePimpl(int, int, int, int )" << endl;
    }

    //计算两点的距离
    void printLine()
    {
        _p1.print();
        cout << "------>";
        _p2.print();
        cout << endl;
    }

    ~LinePimpl()
    {
        cout << "~LinePimpl()" << endl;
    }
private:
    class Point
    {
    public:
        Point(int x, int y)
        :_x(x)
        ,_y(y)
        {
            cout << "Point(int x, int y)" << endl;
        }

        ~Point()
        {
            cout << "~Point()" << endl;
        }

        void print()
        {
            cout << "(" << _x << ","<< _y << ")";
        }
    private:
        int _x;
        int _y;
    };

private:
    Point _p1;
    Point _p2;
};

Line::Line(int x1, int x2, int y1, int y2)
:_linePimpl(new LinePimpl(x1, x2, y1, y2))
{
    cout << "Line(int, int, int, int)" << endl;   
}

Line::~Line()
{
    cout << "~Line()" << endl;
    if(_linePimpl)
    {
        delete _linePimpl;
        _linePimpl = nullptr;
    }
}

void Line::printLine() const
{
    _linePimpl->printLine();
}


int main()
{
    Line l1(1,2,3,5);
    l1.printLine();
    return 0;
}