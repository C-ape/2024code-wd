#include <iostream>

using std::cout;
using std::endl;


class Base
{
public:
    Base(int i)
    : _x(i)
    {
        cout << "Base(int i)" << endl;
    }

    ~Base()
    {
        cout << "~Base(int i)" << endl;
    }

    int operator+(Base &rhs)
    {
        return (-1)*_x + rhs._x;
    }

private:
    int _x;
};


int main()
{
    int i = 2, j = 7;
    Base x(i);
    Base y(j);
    cout << (x+y == j-i) << endl;
}
