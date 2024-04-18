#include <iostream>


using std::cout;
using std::endl;

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
        cout << "x = " << _x << " " << "y = " << _y << endl;
    }
private:
    int _x;
    int _y;
};
