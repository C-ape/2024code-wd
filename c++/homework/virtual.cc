#include <iostream>

using std::cout;
using std::endl;

class A
{
public:
    A(int x)
    :_x(x)
    {
        cout << "A(int)" << endl;
        print();
    }
    void print1()
    {
        print();
    }
    void print()
    {
        cout << "x = " << _x << endl;
    }
private:
    int _x;
};

class B
:public A
{
public:
    B(int x, int y)
    :A(x)
    ,_y(y)
    {
        cout << "B(int, int)" << endl;
    }
private:
    int _y;
};

int main()
{
    B b1(3,4);
    b1.print();
}